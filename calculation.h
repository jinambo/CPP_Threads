#ifndef CALCULATION_H
#define CALCULATION_H

#include <QThread>

class Calculation : public QThread
{
public:
    explicit Calculation(QObject *parent = nullptr);
};

#endif // CALCULATION_H
