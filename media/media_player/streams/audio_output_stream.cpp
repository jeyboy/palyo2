#include "audio_output_stream.h"

AudioOutputStream::AudioOutputStream(QObject * parent, QAudioFormat & format, Priority priority) : Stream(parent, priority), buffersLength(0) {
    soundOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, this);
    soundOutput -> setVolume(1.0);
    audioIO = soundOutput -> start();

    start(priority);
}

AudioOutputStream::~AudioOutputStream() {
   qDebug() << "a o STream";
   soundOutput -> stop();
}

void AudioOutputStream::addBuffer(QByteArray & frame) {
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

            audioIO -> write(audioBuffers.takeFirst());

            mutex -> unlock();
            msleep(12);
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
