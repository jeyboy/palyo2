#ifndef MEMORY_PROFILER_H
#define MEMORY_PROFILER_H

#include <QDebug>
#include <QHash>

#include <typeinfo>
#include <new>                  // size_t/std::bad_alloc

#if defined(__unix__) || defined(__unix) || \
        (defined(__APPLE__) && defined(__MACH__))
#include <alloca.h>             // alloca
#endif

#ifdef _WIN32
#include <malloc.h>             // alloca
#endif

//#include <stdio.h>              // FILE

struct MemoryProfilePointer;

class MemoryProfileRecorder {
public:
    static MemoryProfileRecorder * instance();
    static void close() {
        delete self;
    }

    void proceed(void * p, MemoryProfilePointer * pointer, int size);
private:
    MemoryProfileRecorder() : memory_usage(0) {}

    int memory_usage;
    QHash<void *, MemoryProfilePointer *> pointers;
    static MemoryProfileRecorder * self;
};

struct MemoryProfilePointer {
    MemoryProfilePointer(const char *file, int lineNo, bool creation)
        : mFile(file), mLineNo(lineNo), mCreation(creation)
    {
    }

    template <class T>
    T * operator << (T * t) const {
        if (typeid(T) != typeid(MemoryProfileRecorder))
            MemoryProfileRecorder::instance() -> proceed(t, const_cast<MemoryProfilePointer *>(this), sizeof(t) * (mCreation ? 1 : -1));

        return t;
    }

//    void operator ->* (void) const {
//        MemoryProfileRecorder::instance() -> proceed(t, const_cast<MemoryProfilePointer *>(this), sizeof(t) * (mCreation ? 1 : -1));
//    }

    const char *mFile;
    const int mLineNo;
    const bool mCreation;
};

#define new MemoryProfilePointer(__FILE__,__LINE__, true) << new
//#define delete new MemoryProfilePointer(__FILE__,__LINE__, false) ->* delete

#endif // MEMORY_PROFILER_H

//void * operator new(unsigned int size) throw (std::bad_alloc)
//{
//    void *ptr = (void *)malloc(size);
//    Pointers::instance()->append(ptr);
//    return(ptr);
//};

//void operator delete(void *p) throw()
//{
//    Pointers::instance()->removeAll(p);
//    free(p);
//};
