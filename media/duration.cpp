#include "duration.h"

QString Duration::fromSeconds(int seconds, bool includeHours) {
    int m = seconds == 0 ? 0 : abs(seconds / 60) % 60;
    int s = seconds == 0 ? 0 : abs(seconds) % 60;
    int h = seconds == 0 ? 0 : abs(seconds / 3600) % 24;

    return formate(h, m, s, includeHours);
}
QString Duration::fromMillis(int millis, bool includeHours) {
    int m = millis == 0 ? 0 : abs(millis / 60000) % 60;
    int s = millis == 0 ? 0 : abs(millis / 1000) % 60;
    int h = millis == 0 ? 0 : abs(millis / 3600000) % 24;

    return formate(h, m, s, includeHours);
}

bool Duration::hasHours(int millis) {
    int h = millis == 0 ? 0 : abs(millis / 3600000) % 24;
    return h > 0;
}

QString Duration::formate(int h, int m, int s, bool includeHours) {
    if (h > 0 || includeHours) {
        return QString().sprintf("%02d:%02d:%02d", h, m, s);
    } else {
        return QString().sprintf("%02d:%02d", m, s);
    }
}
