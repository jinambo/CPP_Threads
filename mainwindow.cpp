#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Connect the signals for factorial
    connect(&factorialCalculator, &Factorial::resultReady, this, &MainWindow::getResult);
    connect(&factorialCalculator, &Factorial::progress, this, &MainWindow::updateProgressBar);
    connect(&factorialCalculator, &Factorial::estimatedTime, this, &MainWindow::updateEstimatedTime);
    connect(&realtimeTimer, &QTimer::timeout, this, &MainWindow::updateTimeUI);

    // Connect the signals for erat
    connect(&eratCalculator, &Erat::primesReady, this, &MainWindow::getResultPrimes);
    connect(&eratCalculator, &Erat::progress, this, &MainWindow::updateEratProgressBar);
    connect(&eratCalculator, &Erat::estimatedTime, this, &MainWindow::updateEratEstimatedTime);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_factorialStart_clicked() {
    QString inputText = ui->factorialIn->text();
    bool ok;
    int inputVal = inputText.toInt(&ok);

    // Check if the conversion was successful
    if (ok) {
        factorialCalculator.setNumber(inputVal);

        // Start realtime timer
        realtimeTimer.start();

        // Store the start time
        startTime = QTime::currentTime();

        // Check if the calculation is paused
        if (isCalculationPaused) {
            factorialCalculator.resume();

            factorialCalculator.resume();
            isCalculationPaused = false;

            // startTime - pauseTime
            timerPauseValue += pauseTime.msecsTo(startTime);

            qDebug() << QString("Calculation resumed. Pause time: %1").arg(timerPauseValue);
        } else {
            // Start the timer
            startTime = QTime::currentTime(); // Store the start time
            timer.start();
            realtimeTimer.start();

            // Start thread calc
            factorialCalculator.startThread();
            qDebug() << "Calculation started.";
        }
    }
}

void MainWindow::on_factorialPause_clicked() {
    factorialCalculator.pause();
    isCalculationPaused = true;
    ui->factorialStart->setText("Resume");

    // Store the current time
    pauseTime = QTime::currentTime();
    realtimeTimer.stop();

    qDebug() << "Calculation paused.";
}

void MainWindow::on_factorialStop_clicked() {
    factorialCalculator.stop();
    ui->factorialStart->setText("Start");
    timerPauseValue = 0;
}

void MainWindow::getResult(unsigned long int result) {
    // Calculate the elapsed time
    int elapsed = timer.elapsed() - timerPauseValue;

    // Display the result and elapsed time
    ui->factorialOut->setText(QString("%1").arg(result));

    qDebug() << result;
    qDebug() << QString("Elapsed time: %1 ms").arg(elapsed);

    // Start realtime timer
    realtimeTimer.stop();
}

void MainWindow::updateProgressBar(int iteration, int maxIterations) {
    double currentProgress = static_cast<double>(iteration) / maxIterations * 100;
    ui->factorialProgress->setValue(currentProgress);
    qDebug() << QString("Calculating progress: %1").arg(currentProgress);
}

void MainWindow::updateTimeUI() {
    int elapsed = timer.elapsed() - timerPauseValue;
    ui->factorialTime->setText(QString("%1 ms").arg(elapsed));
}

void MainWindow::updateEstimatedTime(qint64 estimated) {
    ui->factorialEstimated->setText(QString("%1 ms").arg(estimated));
}

void MainWindow::on_eratStart_clicked() {
    QString inputTextFrom = ui->eratInFrom->text();
    QString inputTextTo = ui->eratInTo->text();

    bool okFrom, okTo;
    int inputFromVal = inputTextFrom.toInt(&okFrom);
    int inputToVal = inputTextTo.toInt(&okTo);

    // Check if the conversions were successful
    if (okFrom && okTo) {
        eratCalculator.setFrom(inputFromVal);
        eratCalculator.setTo(inputToVal);

        // Start timer
        eratTimer.start();

        // Start calculation
        eratCalculator.start();
    }
}

void MainWindow::getResultPrimes(const QList<int> &primes) {
    // Calculate the elapsed time
    int elapsed = eratTimer.elapsed();
    ui->eratTime->setText(QString("%1 ms").arg(elapsed));

    QStringList primeStrings;
    for (int i = 0; i < primes.size(); ++i) {
        primeStrings.append(QString::number(primes.at(i)));
    }

    ui->eratOut->setText(primeStrings.join(", "));
}

void MainWindow::updateEratProgressBar(int firstItTime, int allItsTime) {
    double currentProgress = static_cast<double>(firstItTime) / allItsTime * 100;
    ui->eratProgress->setValue(currentProgress);
    qDebug() << QString("Calculating progress: %1").arg(currentProgress);
}

void MainWindow::updateEratEstimatedTime(qint64 estimated) {
    ui->eratEstimated->setText(QString("%1 ms").arg(estimated));
}

