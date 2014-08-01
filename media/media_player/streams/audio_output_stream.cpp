#include "audio_output_stream.h"

AudioOutputStream::AudioOutputStream(QObject * parent, QAudioFormat & format, Priority priority) : Stream(parent, priority) {
    soundOutput = new QAudioOutput(format, this);
    soundOutput -> setVolume(1.0);
    audioIO = soundOutput -> start();
}

void AudioOutputStream::routine() {

}
