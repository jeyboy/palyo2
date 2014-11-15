#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#ifdef Q_OS_WIN
    #include <windows.h>
    #include <psapi.h>

    #pragma comment(lib, "user32.lib")
    static bool * screensaverState;
#elif Q_OS_LYNUX
    #include <QProcess>
#endif


static QString procMemoryUsage() {
    #ifdef Q_OS_WIN
        PROCESS_MEMORY_COUNTERS_EX pmc;
        pmc.cb = sizeof(pmc);
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        return QString::number(pmc.PrivateUsage);
    #else
        return QString("pont");
    #endif
}

static QString memoryUsage() {
    #ifdef Q_OS_WIN
        qDebug() << "*** WIN memory usage";

        MEMORYSTATUSEX memory_status;
        ZeroMemory(&memory_status, sizeof(MEMORYSTATUSEX));
        memory_status.dwLength = sizeof(MEMORYSTATUSEX);
        if (GlobalMemoryStatusEx(&memory_status)) {
            return QString("RAM: %1 MB").arg(memory_status.ullTotalPhys / (1024 * 1024));
        } else {
            return QString("Unknown RAM");
        }

    #elif Q_OS_MAC
        qDebug() << "*** MAC memory usage";

        QProcess p;
        p.start("sysctl", QStringList() << "kern.version" << "hw.physmem");
        p.waitForFinished();
        QString system_info = p.readAllStandardOutput();
        p.close();

    #elif Q_OS_LYNUX
        qDebug() << "*** LYNUX memory usage";

        QProcess p;
        p.start("awk", QStringList() << "/MemTotal/ { print $2 }" << "/proc/meminfo");
        p.waitForFinished();
        QString memory = p.readAllStandardOutput();
        system_info.append(QString("; RAM: %1 MB").arg(memory.toLong() / 1024));
        p.close();
    #else
        qDebug() << "*** This sistem did not support for memory usage monitoring";
    #endif
}

static void restoreScreenSaver() {
    #ifdef Q_OS_WIN
        qDebug() << "??? WIN screensaver restoring";
        if (!SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, *screensaverState, NULL, SPIF_SENDWININICHANGE))
            qDebug() << "!!!! WIN fail to change screensaver";
    #elif Q_OS_MAC
        qDebug() << "??? MAC screensaver restoring";
        qDebug() << "!!!! Mac sistem did not support for screensaver restoring";
    #elif Q_OS_LYNUX
        qDebug() << "??? LYNUX screensaver restoring";
        QProcess proc;

        proc.start("/bin/sh", QStringList() << "-c" << "xset +dpms; xset s on", QIODevice::ReadOnly);
        proc.waitForFinished();

        QString stdError = proc.readAllStandardError();
        if (!stdError.isEmpty())
           qDebug() << "!!!! Error occured on screensaver restoring: " << stdError;
    #else
        qDebug() << "??? OTHER screensaver disabling";
        qDebug() << "!!!! This sistem did not support for screensaver restoring";
    #endif
}

static void offScreenSaver() {
    #ifdef Q_OS_WIN
        qDebug() << "??? WIN screensaver disabling";
        SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &screensaverState, 0);

        if (!SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE , NULL, SPIF_SENDWININICHANGE))
            qDebug() << "!!!! WIN fail to off screensaver";
    #elif Q_OS_MAC
        qDebug() << "??? MAC screensaver disabling";
        qDebug() << "!!!! Mac sistem did not support for screensaver disabling";
    #elif Q_OS_LYNUX
        qDebug() << "??? LYNUX screensaver disabling";
        QProcess proc;

        proc.start("/bin/sh", QStringList() << "-c" << "xset -dpms; xset s noblank; xset s off", QIODevice::ReadOnly);
        proc.waitForFinished();

        QString stdError = proc.readAllStandardError();
        if (!stdError.isEmpty())
           qDebug() << "!!!! Error occured on screensaver disabling: " << stdError;
    #else
        qDebug() << "??? OTHER screensaver disabling";
        qDebug() << "!!!! This sistem did not support for screensaver disabling";
    #endif
}

//static void revertScreenSaver() {

//}


//#include <QLibrary>



//void timerDone(){
//QLibrary library("kernel32.dll");
//if (!library.load())
//qFatal("Library loading failed.");
//typedef uint (*SetThreadExecutionState)(uint esFlag);
//SetThreadExecutionState stes = (SetThreadExecutionState)library.resolve("SetThreadExecutionState");
//stes(0x00000002);
//library.unload();
//}


//#include <Windows.h>
//void DisableScreenSaver()
//{
//SetThreadExecutionState(EXECUTION_STATE.ES_DISPLAY_REQUIRED | EXECUTION_STATE.ES_CONTINUOUS);
//}
//void EnableScreenSaver()
//{
//SetThreadExecutionState(EXECUTION_STATE.ES_CONTINUOUS);
//}


#endif // SYSTEM_UTILS_H
