#include "portaudio_output_stream.h"

static int callback(const void * /*inputBuffer*/,
                    void * outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo * /*timeInfo*/,
                    PaStreamCallbackFlags /*statusFlags*/,
                    void * userData) {

    PortAudioOutputStream * stream = (PortAudioOutputStream *)userData;
    stream -> fillBuffer(outputBuffer, stream -> bytesPerBuffer(framesPerBuffer));

    if (stream -> dataLoadingFinished())
        return paComplete;
    else return paContinue;
}

PortAudioOutputStream::PortAudioOutputStream(QObject * parent, AVFormatContext * context, MasterClock * clock, QSemaphore * sema, int streamIndex, Priority priority)
    : AudioStream(parent, context, clock, sema, streamIndex, priority)
    , initialized(false)
    , outputParameters(new PaStreamParameters) {

    if (valid) {
        init();
        open();
    }
}

PortAudioOutputStream::~PortAudioOutputStream() {
    qDebug() << "Audio output";
    close();

    if (initialized)
        Pa_Terminate(); //Do NOT call this if init failed. See document
    if (outputParameters) {
        delete outputParameters;
        outputParameters = 0;
    }
}


//long gain = 0x4000; // equivalent to 0.5 in 1.15 fixed point format

//long sample = *data++;

//long result = (gain * sample) > 15; // fixed point multiple

//*out++ = (short) result;

uint PortAudioOutputStream::getVolume() const { return 0;/*output -> volume() * 1000;*/ }
void PortAudioOutputStream::setVolume(uint val) { /*output -> setVolume((qreal)val / 1000);*/ }
bool PortAudioOutputStream::deviceInAction() { return Pa_IsStreamActive(stream); }

void PortAudioOutputStream::suspendStream() { // portaudio did not have suspend functional :(
    AudioStream::suspendStream();
}
void PortAudioOutputStream::resumeStream() { // portaudio did not have resume functional :(
    AudioStream::resumeStream();
}

bool PortAudioOutputStream::init() {
    PaError err = paNoError;
    if ((err = Pa_Initialize()) != paNoError) {
        qWarning("Error when init portaudio: %s", Pa_GetErrorText(err));
        return false;
    }

//    int numDevices = Pa_GetDeviceCount();
//    for (int i = 0 ; i < numDevices ; ++i) {
//        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
//        if (deviceInfo) {
//            const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo -> hostApi);
//            QString name = QString(hostApiInfo -> name) + ": " + QString::fromLocal8Bit(deviceInfo -> name);
//            qDebug("audio device %d: %s", i, name.toUtf8().constData());
//            qDebug("max in/out channels: %d/%d", deviceInfo -> maxInputChannels, deviceInfo -> maxOutputChannels);
//        }
//    }

    memset(outputParameters, 0, sizeof(PaStreamParameters));
    outputParameters -> device = Pa_GetDefaultOutputDevice();
    if (outputParameters -> device == paNoDevice) {
        qWarning("PortAudio get device error!");
        return false;
    }

    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(outputParameters -> device);
//    max_channels = deviceInfo -> maxOutputChannels;
    qDebug("DEFAULT max in/out channels: %d/%d", deviceInfo -> maxInputChannels, deviceInfo -> maxOutputChannels);
    qDebug("audio device: %s", QString::fromLocal8Bit(Pa_GetDeviceInfo(outputParameters -> device) -> name).toUtf8().constData());
    outputParameters -> hostApiSpecificStreamInfo = NULL;
    outputParameters -> suggestedLatency = Pa_GetDeviceInfo(outputParameters -> device) -> defaultHighOutputLatency;
    outputParameters -> channelCount = format.channelCount();
    outputParameters -> sampleFormat = paSampleFormat();

    initialized = true;
    return true;
}

bool PortAudioOutputStream::open() {
    PaError err = Pa_OpenStream(&stream, NULL, outputParameters, format.sampleRate(), 0, paNoFlag, callback, this);
    if (err == paNoError) {
        outputLatency = Pa_GetStreamInfo(stream) -> outputLatency;
        Pa_StartStream(stream);
    } else {
        qWarning("Open portaudio stream error: %s", Pa_GetErrorText(err));
    }
    return err == paNoError;
}

bool PortAudioOutputStream::close() {
    PaError err = paNoError;
    if (!stream) { return true; }
    err = Pa_StopStream(stream); //may be already stopped: paStreamIsStopped
    if (err != paNoError) {
        qWarning("Stop portaudio stream error: %s", Pa_GetErrorText(err));
        //return err == paStreamIsStopped;
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        qWarning("Close portaudio stream error: %s", Pa_GetErrorText(err));
        return false;
    }

    stream = NULL;
    return true;
}

int PortAudioOutputStream::paSampleFormat() {
    if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::UnSignedInt)
        return paUInt8;
    else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::SignedInt)
        return paInt16;
    else if (format.sampleSize() == 32 && format.sampleType() == QAudioFormat::SignedInt)
        return paInt32;
    else if (format.sampleType() == QAudioFormat::Float)
        return paFloat32;
    else
        return paCustomFormat;
}

////void PortAudioOutputStream::routine() {
////    mutex -> lock();
////    if (audioBuffers.isEmpty()) {
////        mutex -> unlock();
////        return;
////    }
////    QByteArray ar = audioBuffers.takeFirst();
////    mutex -> unlock();

////    if (Pa_IsStreamStopped(stream))
////        Pa_StartStream(stream);
//////#if KNOW_WHY
//////#ifndef Q_OS_MAC //?
//////    int diff = Pa_GetStreamWriteAvailable(stream) - outputLatency * format -> sampleRate();
//////    if (diff > 0) {
//////        int newsize = diff * format -> channelCount() * sizeof(float);
//////        static char *a = new char[newsize];
//////        memset(a, 0, newsize);
//////        Pa_WriteStream(stream, a, diff);
//////    }
//////#endif
//////#endif //KNOW_WHY

////    double last_buff_delay = ((double)ar.size()) / bytes_per_sec;
////    MasterClock::instance() -> iterateAudioOutput(last_buff_delay);

////    PaError err = Pa_WriteStream(stream, ar.constData(), ar.size() / format -> channelCount() / (format -> sampleSize() / 8));
////    if (err == paUnanticipatedHostError) {
////        qWarning("Write portaudio stream error: %s", Pa_GetErrorText(err));
////    }

////    msleep(last_buff_delay * 100);
////}
