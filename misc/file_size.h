#ifndef FILE_SIZE_H
#define FILE_SIZE_H

#include <QObject>

class FileSize {
public:
    static QString toUnits(long long val);
};


#endif // FILE_SIZE_H
