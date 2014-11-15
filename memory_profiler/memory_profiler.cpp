#include "memory_profiler.h"

MemoryProfileRecorder * MemoryProfileRecorder::self = 0;

MemoryProfileRecorder * MemoryProfileRecorder::instance() {
    if (!self)
        self = ::new MemoryProfileRecorder();

    return self;
}

void MemoryProfileRecorder::proceed(void * p, MemoryProfilePointer * pointer, int size) {
    QString info = QString(pointer -> mFile) + "  " + QString(pointer -> mLineNo);

    if (pointers.contains(p)) {
        free(pointers.take(p));
        memory_usage += size;
        free(pointer);
    } else {
        pointers.insert(p, pointer);
        memory_usage += size;
    }

    qDebug() << "!!!! " << info << " : " << memory_usage;
}

