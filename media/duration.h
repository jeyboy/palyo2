#ifndef DURATION_H
#define DURATION_H

#include <QString>
//#include <QStringList>

#include "misc/format.h"

class Duration {
public:
    static QString fromSeconds(int seconds, bool includeHours = false);
    static QString fromMillis(int millis, bool includeHours = false);

    static bool hasHours(int millis);
//    static int parse(QString str);
};


#endif // DURATION_H
