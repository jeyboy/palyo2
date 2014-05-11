#include "file_size.h"

QString FileSize::toUnits(long long val) {
    QString unitList[] = {"B", "Kb", "Mb", "Gb", "Tb", "Pb"};
    int size = sizeof(unitList) / sizeof(unitList[0]);

    int loop1 = 0;
    double dVal = val;

    for (; loop1 < size && dVal > 1023; loop1++) {
        dVal /= 1024.0;
    }

    return QString().sprintf("%03d ", dVal) + unitList[loop1];
}
