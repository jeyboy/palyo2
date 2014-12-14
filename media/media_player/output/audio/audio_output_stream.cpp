#include "audio_output_stream.h"
#include "media/media_player/streams/decoders/audio_stream.h"
#include <QDebug>

AudioOutputStream::AudioOutputStream(void * decoderStream, QAudioFormat & format) : Stream((QObject *)decoderStream) {
    stream = decoderStream;
    soundOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format);
    soundOutput -> setVolume(1.0);
    audioIO = soundOutput -> start();

    start();
}

AudioOutputStream::~AudioOutputStream() {
   qDebug() << "a o STream";
   soundOutput -> stop();
   delete soundOutput;
}

void AudioOutputStream::playBuffer(QByteArray * frame) {
    audioIO -> write(*frame);
}

void AudioOutputStream::routine() {
    AudioStream * dStream = (AudioStream *) stream;

    AudioFrame * currFrame = 0; /*dStream -> decoded();*/
//    if (MasterClock::instance() -> audio() != 0) {
////        int lo = MasterClock::instance() -> computeAudioDelay();
////        usleep(lo);
//    }

    audioIO -> write(*currFrame -> buffer);
//    MasterClock::instance() -> setAudio(currFrame -> bufferPTS);

    delete currFrame;
}
