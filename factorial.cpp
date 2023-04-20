#include "factorial.h"
#include <QDebug>
#include <QElapsedTimer>

Factorial::Factorial(QObject *parent)
    : QThread{parent} {}

void Factorial::setNumber(int number) {
    m_number = number;
}

void Factorial::run() {
    QElapsedTimer timer;
    timer.start();

    // Factorial calculation
    for (int i = iteration; i <= m_number; ++i) {
        // Check if the calculation should be paused or stopped
        if (m_stopped) {
            emit progress(0, m_number); // Re-set progress
            return;
        }

        if (m_paused) {
            iteration = i;
            return;
        }

        result *= i;
        emit progress(i, m_number);

        // Add a delay between iterations to slow down the calculation
        QThread::msleep(500);

        if (i == 1) {
            qint64 estimated = timer.elapsed() * m_number;
            emit estimatedTime(estimated);
            qDebug() << "Estimated time for" << m_number << "iterations: " << estimated << "ms";
        }
    }

    // Send result using signal
    emit resultReady(result);

    // Reset values when finished
    if (!m_paused) {
        qDebug("Restarting values");
        result = 1;
        iteration = 1;
    }
}

void Factorial::startThread() {
    qDebug() << "Starting the thread calc.";
    m_stopped = false;
    start();
}

void Factorial::pause() {
    m_paused = true;
}

void Factorial::stop() {
    m_stopped = true;
    result = 1;
    iteration = 1;
}

void Factorial::resume() {
    m_paused = false;
    qDebug() << "Calculation resumed.";
    if (result != 1) {
        // If the calculation was paused, continue from the last saved result
        start();
    }
}

