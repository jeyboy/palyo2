#include "audio_stream.h"

AudioStream::AudioStream(QObject * parent, AVFormatContext * context, MasterClock * clock, QSemaphore * sema, int streamIndex, Priority priority)
    : MediaStream(context, clock, streamIndex, parent, priority)
    , defaultChannelLayout(AV_CH_LAYOUT_STEREO)
    , sleep_correlation(10)
    , resampleRequire(false)
    , resampler(0) {

    if (valid) {
        setSemaphore(sema);
        // if (is_planar)
        //    std::cout << "The audio stream (and its frames) have too many channels to fit in\n"
        //              << "frame->data. Therefore, to access the audio data, you need to use\n"
        //              << "frame->extended_data to access the audio data. It's planar, so\n"
        //              << "each channel is in a different element. That is:\n"
        //              << "  frame->extended_data[0] has the data for channel 1\n"
        //              << "  frame->extended_data[1] has the data for channel 2\n"
        //              << "  etc.\n";
        framesPerBuffer = codec_context -> channels; //is_planar ? codec_context -> channels : 1;
        packetsBufferLen = framesPerBuffer * 10;
        framesBufferLen = framesPerBuffer * 15;
    }
}

AudioStream::~AudioStream() {
    delete resampler;
    flushData();
    frames.clear();
}

//int AudioStream::calcDelay() {
////    return frames.size() == 0 ? 0 : (frames.size() / framesPerBuffer) * 4;
//    return time_buff == 0 ? 0 : time_buff * 80; // take only 8/10 from the time buffer for delay
//}

bool AudioStream::isBlocked() {
    return MediaStream::isBlocked() || frames.size() >= framesBufferLen;
}

void AudioStream::suspendStream() {
    MediaStream::suspend();
}
void AudioStream::resumeStream() {
    MediaStream::resume();
}

void AudioStream::flushData() {
    MediaStream::dropPackets();
    frameMutex -> lock();
    qDeleteAll(frames);
    frameMutex -> unlock();
    avcodec_flush_buffers(codec_context);
}

void AudioStream::routine() {
    if (pauseRequired) return;

    mutex -> lock();
    bool isEmpty = packets.isEmpty();
    mutex -> unlock();

    if (!pauseRequired && isEmpty && eof && !deviceInAction()) suspendStream();

    // TODO: mutex required for frames
    if (isEmpty) {
        if (frames.size() <= framesBufferLen / 2)
            semaphore -> release(semaphore -> available() == 0 ? 1 : 0);
        msleep(sleep_correlation);
        return;
    }

//    if (frames.size() >= framesBufferLen) {
////        qDebug() << " AUDIO FULL";
//        sleep_correlation = time_buff * 50; // take half of time buff // 2
//        msleep(sleep_correlation);
//        return;
//    } else if (frames.size() >= framesBufferLen / 2)
//        msleep(time_buff * 80);

    int len, got_frame;
    AVPacket * packet = 0;

    mutex -> lock();
    packet = packets.takeFirst();
    mutex -> unlock();

    while (packet -> size > 0) {
        len = avcodec_decode_audio4(codec_context, frame, &got_frame, packet);

        if (len < 0) {
            qDebug() << "Error while decoding audio frame";
            break;
        } else {
            if (got_frame) {
                //TODO: copy data direct to the buffer
                QByteArray * ar = new QByteArray();
                if (resampleRequire) {
                    if (!resampler -> proceed(frame, ar))
                        qDebug() << "RESAMPLER FAIL";
                } else {
                    memcpy(ar -> data(), (const char*)frame -> data[0], frame -> linesize[0]);
                }

                float packet_time = packet -> duration * av_q2d(stream -> time_base);
                time_buff += packet_time;
                frames.append(new AudioFrame(ar, calcPts(packet), packet_time));
            } else {
                qDebug() << "Could not get audio data from this frame";
            }

            packet -> size -= len;
            packet -> data += len;
        }
    }

    av_frame_unref(frame);

    av_free_packet(packet);
}

qint64 AudioStream::fillBuffer(void * data, qint64 maxlen) {
    int reslen = 0;
    AudioFrame * currFrame;
    memset(data, 0, maxlen); // clear buffer while buffer is idle
    int copy_size;

    if (!pauseRequired && !frames.isEmpty()) {
        char * out = (char *)data;
        while(!frames.isEmpty()) {
            if (reslen == maxlen)
                break;

            frameMutex -> lock();
            currFrame = frames.at(0);
            copy_size = qMin(maxlen - reslen, (qint64)currFrame -> buffer -> size());

            if (frames.at(0) -> buffer -> size() == copy_size) {
                currFrame = frames.takeFirst();
                frameMutex -> unlock();
            }

            memcpy(out, currFrame -> buffer -> data(), copy_size);
            reslen += copy_size;
            out += copy_size;

            if (copy_size == currFrame -> buffer -> size()) {
                time_buff -= currFrame -> time_length;
                clock -> setAudio(currFrame -> bufferPTS);
                delete currFrame;
            } else {
                float time_shift = (copy_size / (float)currFrame -> buffer -> size()) * currFrame -> time_length;
                currFrame -> buffer -> remove(0, copy_size);

                currFrame -> time_length -= time_shift;
                time_buff -= time_shift;

                clock -> iterateAudio(time_shift);
                frameMutex -> unlock();
            }
        }

        return reslen;
    }

    if (pauseRequired || eof) suspendStream();
    qDebug() << "IS EMPTY";
    return maxlen;
}

//void AudioStream::sync(double delay, char *data, int & len, qint64 maxlen) {
//    int n = output -> format().channelCount();
//    int wanted_size = len + output -> format().bytesForDuration(delay * 1000);//(delay * output -> format().sampleRate() * n);
//    qDebug() << "DELAY " << delay << " " << len << " NEW " << wanted_size;
//    if (wanted_size > maxlen)
//        wanted_size = maxlen;

//    if(wanted_size < len) {
//        /* remove samples */
//        len = wanted_size;
//    } else if(wanted_size > len) {
//        int copyNum = wanted_size - len;
//        memcpy((uint8_t *)data + len, (uint8_t *)data + len - copyNum , copyNum);


////        uint8_t *samples_end, *q;
////        int nb;

////        /* add samples by copying final samples */
////        nb = (len - wanted_size);
////        samples_end = (uint8_t *)data + len;
////        q = samples_end + n;
////        while(nb > 0) {
////            memcpy(q, samples_end, n);
////            q += n;
////            nb -= n;
////        }
//        len = wanted_size;
//    }
//}

double AudioStream::calcPts(AVPacket * packet) {
    double clock;

    if (packet -> pts != AV_NOPTS_VALUE) {
        clock = av_q2d(stream -> time_base) * packet -> pts;
    } else {
        int data_size = av_samples_get_buffer_size(
                    NULL,
                    codec_context -> channels,
                    frame -> nb_samples,
                    codec_context -> sample_fmt,
                    1
        );

        /* if no pts, then compute it */
        clock = (double)data_size / (
                codec_context -> channels *
                codec_context -> sample_rate *
                av_get_bytes_per_sample(codec_context -> sample_fmt)
            );
    }

//    qDebug() << "AUDIO PTS " << av_q2d(stream -> time_base) << " : " << clock;

    return clock;
}

int AudioStream::bytesPerSecond() {
    if (resampleRequire)
        return resampler -> bytesPerSecond();
    else
        return codec_context -> sample_rate * codec_context -> channels * av_get_bytes_per_sample(codec_context -> sample_fmt);
}
