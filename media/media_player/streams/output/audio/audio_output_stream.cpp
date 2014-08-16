#include "audio_output_stream.h"

AudioOutputStream::AudioOutputStream(QObject * parent, QAudioFormat & format, Priority priority)
    : Stream(parent, priority)
    , millisPeriod(0.1) {

    soundOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, this);
    soundOutput -> setVolume(1.0);
    soundOutput -> setNotifyInterval(millisPeriod * 1000);
    qDebug() << "##################### " << soundOutput -> notifyInterval();
    connect(soundOutput, SIGNAL(notify()), this, SLOT(iteration()));
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
            long last_buff_delay = soundOutput -> format().durationForBytes(ar -> size());
            audioIO -> write(*ar);
            mutex -> unlock();

            delete ar;
            msleep(last_buff_delay / 1000);
            return;
        } else { msleep(12); }
    mutex -> unlock();
}

void AudioOutputStream::iteration() {
    MasterClock::instance() -> iterateAudioOutput(millisPeriod);
    qDebug() << "AUDIO " << MasterClock::instance() -> audioOutput();
}
