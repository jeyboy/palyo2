#ifndef DURATION_H
#define DURATION_H

#include <QString>

class Duration {
public:
    static QString fromSeconds(int seconds, bool includeHours = false);
    static QString fromMillis(int millis, bool includeHours = false);

    static bool hasHours(int millis);
protected:
    static QString formate(int h, int m, int s, bool includeHours);
};


#endif // DURATION_H
