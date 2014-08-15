#include "audio_output_stream.h"

AudioOutputStream::AudioOutputStream(QObject * parent, int bytesPerSecond, QAudioFormat & format, Priority priority)
    : Stream(parent, priority) {
    bytes_per_sec = bytesPerSecond;

    soundOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, this);
    soundOutput -> setVolume(1.0);
    audioIO = soundOutput -> start();

    start(priority);
}

AudioOutputStream::~AudioOutputStream() {
   qDebug() << "a o STream";
   soundOutput -> stop();

   qDeleteAll(audioBuffers);
}

void AudioOutputStream::addBuffer(QByteArray * frame) {
    mutex -> lock();
        audioBuffers.append(frame);
    mutex -> unlock();
}

//TODO: move delays on decoder level and remove buffer (maybe this class not should be a thread)
void AudioOutputStream::routine() {

//    uint delay = MasterClock::instance() -> computeAudioDelay();
//    if (delay > 0) { msleep(delay); }

    mutex -> lock();
        if (!audioBuffers.isEmpty() && soundOutput -> bytesFree() > 0) {
            QByteArray * ar = audioBuffers.takeFirst();
            audioIO -> write(*ar);
            mutex -> unlock();

//            MasterClock::instance() -> iterateAudioOutput(last_buff_delay - ((double)audioIO -> bytesToWrite() / bytes_per_sec));
            double last_buff_delay = ((double)ar -> size()) / bytes_per_sec;
            MasterClock::instance() -> iterateAudioOutput(last_buff_delay);

            delete ar;
            msleep(last_buff_delay * 100);
            return;
        } else { msleep(20); }
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
