#ifndef FACTORIAL_H
#define FACTORIAL_H

#include <QThread>

class Factorial : public QThread
{
    Q_OBJECT

public:
    explicit Factorial(QObject *parent = nullptr);
    void run() override;
    void setNumber(int number);
    void startThread();
    void pause();
    void resume();
    void stop();

signals:
    void resultReady(unsigned long int result);
    void progress(int iteration, int maxIterations);
    void estimatedTime(qint64 estimated);

private:
    int m_number;
    unsigned long int result = 1;
    int iteration = 1;
    bool m_stopped = false;
    bool m_paused = false;
};

#endif // FACTORIAL_H
