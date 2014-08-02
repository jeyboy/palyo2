#include "audio_output_stream.h"

AudioOutputStream::AudioOutputStream(QObject * parent, QAudioFormat & format, QMutex * blockMutex, Priority priority) : Stream(parent, priority) {
    mutex = blockMutex;
    soundOutput = new QAudioOutput(format, this);
    soundOutput -> setVolume(1.0);
    audioIO = soundOutput -> start();
}

AudioOutputStream::~AudioOutputStream() {
   qDebug() << "a o STream";
   soundOutput -> stop();
}

//void AudioOutputStream::addBuffer() {

//}

void AudioOutputStream::routine() {

}
