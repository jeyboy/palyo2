#include "audio_output_stream.h"

AudioOutputStream::AudioOutputStream(QObject * parent, AVFormatContext * context, MasterClock * clock, QSemaphore * sema, int streamIndex, Priority priority)
    : AudioStream(parent, context, clock, sema, streamIndex, priority), QIODevice(parent) {

    if (valid) {
        output = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, parent);
//        output -> setNotifyInterval(20);
        output -> setVolume(1.0);

        open(QIODevice::ReadOnly);
        output -> start(this);
    }
}

AudioOutputStream::~AudioOutputStream() {
    qDebug() << "Audio output";
    output -> stop();
    output -> reset();
    close();
    delete output;
}

uint AudioOutputStream::getVolume() const { return output -> volume() * 1000; }
void AudioOutputStream::setVolume(uint val) { output -> setVolume((qreal)val / 1000); }
bool AudioOutputStream::deviceInAction() { return output -> state() == QAudio::ActiveState; }

void AudioOutputStream::suspendStream() {
    output -> suspend();
    AudioStream::suspendStream();
}
void AudioOutputStream::resumeStream() {
    AudioStream::resumeStream();
    output -> resume();
}

qint64 AudioOutputStream::readData(char *data, qint64 maxlen) {
    return fillBuffer(data, maxlen);
}

qint64 AudioOutputStream::writeData(const char */*data*/, qint64 /*len*/) {
    return 0;
}
