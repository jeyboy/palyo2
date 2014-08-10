#ifndef MASTER_CLOCK_H
#define MASTER_CLOCK_H

#include <QTimer>

class MasterClock : public QTimer {
    Q_OBJECT
public:
    void updateSpeed(float scale);
};

#endif // MASTER_CLOCK_H
