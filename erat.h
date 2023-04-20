#ifndef ERAT_H
#define ERAT_H

#include <QThread>
#include <QVector>

class Erat : public QThread
{
    Q_OBJECT

public:
    explicit Erat(QObject *parent = nullptr);
    void run() override;
    void setFrom(int from);
    void setTo(int to);
    void startThread();
    void pause();
    void resume();
    void stop();

signals:
    void primesReady(const QList<int> &primes);
    void progress(int firstItTime, int allItsTime);
    void estimatedTime(qint64 estimated);

private:
    int m_from;
    int m_to;
    QVector<bool> m_sieve;
    qint64 estimatedFirst;
    qint64 estimated;
    QVector<bool> m_storedSieve;
    int m_estimatedCounter = 0;
    int m_currentIteration = 2;
    int m_storedIteration = 2;
    bool m_stopped;
    bool m_paused;
};

#endif // ERAT_H
