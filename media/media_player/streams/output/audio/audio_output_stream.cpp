#include "audio_output_stream.h"

AudioOutputStream::AudioOutputStream(QObject * parent, AVCodecContext* codec_context, QAudioFormat & format, Priority priority) : Stream(parent, priority)
  , buffersLength(0)
  , last_buff_delay(0) {

    bytes_per_sec = codec_context -> sample_rate * codec_context -> channels * av_get_bytes_per_sample(codec_context -> sample_fmt);

    soundOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, this);
    soundOutput -> setVolume(1.0);
    audioIO = soundOutput -> start();

    start(priority);
}

AudioOutputStream::~AudioOutputStream() {
   qDebug() << "a o STream";
   soundOutput -> stop();
}

void AudioOutputStream::addBuffer(QByteArray frame) {
    mutex -> lock();
        audioBuffers.append(frame);
        buffersLength += frame.size();
    mutex -> unlock();
}

void AudioOutputStream::routine() {
    mutex -> lock();
        if (!audioBuffers.isEmpty() && soundOutput -> bytesFree() > 0) {

            // If we're too slow/accumulating delay, drop audio frames
//            while (buffersLength > MAX_AUDIO_DATA_PENDING) {
//                buffersLength -= audioBuffers.takeFirst().length();
//            }

            QByteArray ar = audioBuffers.takeFirst();
            MasterClock::instance() -> iterateAudioOutput(last_buff_delay - ((double)audioIO -> bytesToWrite() / bytes_per_sec));
            last_buff_delay = ((double)ar.size()) / bytes_per_sec;
            audioIO -> write(ar);

            mutex -> unlock();
            msleep(last_buff_delay * 100);
            return;
        }
    mutex -> unlock();
}

//void AudioOutputStream::playbackAudioThread() {
//if (mAudioBufferSize.size() > 0 && mBuffered >= AUDIO_BUFFERING
//&& mAudioOutput->bytesFree() > 0)
//{
//mAudioMutex.lock();

//int bufferSize = mAudioBufferSize.front();

//// If we're too slow/accumulating delay, drop audio frames
//while (mAudioBuffer.size() > MAX_AUDIO_DATA_PENDING)
//{
//bufferSize = mAudioBufferSize.front();
//mAudioBuffer.remove(0, bufferSize);
//mAudioBufferSize.pop_front();
//}

//// Write to our audio channel
//QByteArray frame = mAudioBuffer.left(bufferSize);
//mAudioIO->write(frame);
//mAudioBuffer.remove(0, bufferSize);

//mAudioMutex.unlock();

//mAudioBufferSize.pop_front();
//}

//std::this_thread::sleep_for(std::chrono::milliseconds(1));
//}
//}
