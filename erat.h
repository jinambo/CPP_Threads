#ifndef ERAT_H
#define ERAT_H

#include <QThread>
#include <QBitArray>

class Erat : public QThread
{
    Q_OBJECT

public:
    explicit Erat(QObject *parent = nullptr);
    void run() override;
    void setFrom(int from);
    void setTo(int to);

signals:
    void primesReady(const QList<int> &primes);
    void progress(int firstItTime, int allItsTime);
    void estimatedTime(qint64 estimated);

private:
    int m_from;
    int m_to;
    int m_limit;
    QBitArray m_sieve;
    qint64 estimatedFirst;
    qint64 estimated;
};

#endif // ERAT_H
