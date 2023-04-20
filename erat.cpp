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
    m_sieve = QVector<bool>(size, true);
    int offset = m_from; // calculate the offset value to adjust index

    QElapsedTimer timer;
    timer.start();

    if (m_storedIteration > 2) {
        // Resume calculation from stored iteration and sieve
        for (int p = 2; p < m_storedIteration; ++p) {
            for (int i = qMax(p * p, ((m_from + p - 1) / p) * p) - offset; i < size; i += p) {
                m_sieve[i] = false;
            }
        }
        // emit progress(m_estimatedCounter += estimatedFirst, estimated);
    }

    // Continue calculation from where it was left off
    int max = qSqrt(m_to);
    for (int p = m_storedIteration > 0 ? m_storedIteration : 2; p <= max; ++p) {
        // Check if the calculation should be paused or stopped
        if (m_stopped) {
            emit progress(0, 100); // Re-set progress
            return;
        }

        if (m_paused) {
            m_storedIteration = p;
            m_storedSieve = m_sieve;
            return;
        }

        for (int i = qMax(p * p, ((m_from + p - 1) / p) * p) - offset; i < size; i += p) {
            m_sieve[i] = false;
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

        emit progress(m_estimatedCounter += estimatedFirst, estimated);
    }

    // Fill QList with primes
    QList<int> primes;
    for (int i = 0; i < size; ++i) {
        if (m_sieve[i]) {
            primes.append(i + offset);
        }
    }

    // Send results via signal
    emit primesReady(primes);

    // Reset values when finished
    if (!m_paused) {
        qDebug("Restarting values");
        m_sieve.fill(true);
        m_storedSieve.fill(true);
        m_storedIteration = m_currentIteration = 2;
        m_estimatedCounter = 0;
    }
}

void Erat::startThread() {
    qDebug() << "Starting the thread calc.";
    m_stopped = false;
    start();
}

void Erat::pause() {
    m_storedIteration = m_currentIteration;
    m_storedSieve = m_sieve;
    m_paused = true;
}

void Erat::resume() {
    m_paused = false;
    qDebug() << "Calculation resumed.";
    if (!m_storedSieve.isEmpty()) {
        m_sieve = m_storedSieve;
        m_currentIteration = m_storedIteration;
        m_storedSieve.clear();
        start();
    }
}

void Erat::stop() {
    m_stopped = true;
    m_sieve.fill(true);
    m_storedSieve.fill(true);
    m_storedIteration = m_currentIteration = 2;
    m_estimatedCounter = 0;
}
