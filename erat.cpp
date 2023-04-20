#include "erat.h"
#include <QDebug>

Erat::Erat(QObject *parent)
    : QThread{parent} {

}

void Erat::setFrom(int from) {
    m_from = from;
}

void Erat::setTo(int to) {
    m_to = to;
}

void Erat::run() {
    int size = m_to - m_from + 1;  // calculate size of the sieve
    QVector<bool> sieve(size, true); // initialize the sieve with true values
    int offset = m_from; // calculate the offset value to adjust index

    QElapsedTimer timer;
    timer.start();


    int estimatedCounter = 0;

    // Provedení výpočtu Eratosthenova síta
    int max = qSqrt(m_to);
    for (int p = 2; p <= max; ++p) {
        for (int i = qMax(p * p, ((m_from + p - 1) / p) * p) - offset; i < size; i += p) {
            sieve[i] = false;
        }

        // Add a delay between iterations to slow down the calculation
        QThread::msleep(500);

        // Calculate estimated time after one iteration
        if (p == 2) {
            estimatedFirst = timer.elapsed();
            estimated = estimatedFirst * (max - 1);
            emit estimatedTime(estimated);
            qDebug() << "Estimated time for finding primes from" << m_from << "to" << m_to << ":" << estimated << "ms";
        }

        emit progress(estimatedCounter += estimatedFirst, estimated);
    }

    // Sběr výsledků (prvočísel) do seznamu
    QList<int> primes;
    for (int i = 0; i < size; ++i) {
        if (sieve[i]) {
            primes.append(i + offset);
        }
    }

    // Odeslání výsledků pomocí signálu
    emit primesReady(primes);
    qDebug() << "Primes found from" << m_from << "to" << m_to << ":" << primes;
}

