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
    connect(&realtimeEratTimer, &QTimer::timeout, this, &MainWindow::updateEratTimeUI);

    // Get main thread ID
    qInfo() << "Main thread ID: " << QThread::currentThreadId();
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
            //factorialCalculator.resume();
            isCalculationPaused = false;

            // startTime - pauseTime
            timerPauseValue += pauseTime.msecsTo(startTime);

            qDebug() << QString("Calculation resumed. Pause time: %1").arg(timerPauseValue);
        } else {
            // Start the timer
            // startTime = QTime::currentTime(); // Store the start time
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
    realtimeTimer.stop();

    // Reset GUI values
    ui->factorialStart->setText("Start");
    ui->factorialEstimated->setText(QString("0 ms"));
    ui->factorialTime->setText(QString("0 ms"));
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
    timerPauseValue = 0;
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

        // Start realtime timer
        realtimeEratTimer.start();

        // Store the start time
        startEratTime = QTime::currentTime();

        // Check if the calculation is paused
        if (isEratPaused) {
            eratCalculator.resume();
            isEratPaused = false;

            // startTime - pauseTime
            timerEratPauseValue += pauseEratTime.msecsTo(startEratTime);

            qDebug() << QString("Calculation resumed. Pause time: %1").arg(timerEratPauseValue);
        } else {
            // Start the timer
            // startEratTime = QTime::currentTime(); // Store the start time
            eratTimer.start();
            realtimeEratTimer.start();

            // Start thread calc
            eratCalculator.startThread();
            qDebug() << "Calculation started.";
        }
    }
}

void MainWindow::getResultPrimes(const QList<int> &primes) {
    // Calculate the elapsed time
    int elapsed = eratTimer.elapsed() - timerEratPauseValue;
    ui->eratTime->setText(QString("%1 ms").arg(elapsed));

    QStringList primeStrings;
    for (int i = 0; i < primes.size(); ++i) {
        primeStrings.append(QString::number(primes.at(i)));
    }

    ui->eratOut->setText(primeStrings.join(", "));

    // Start realtime timer
    realtimeEratTimer.stop();

    timerEratPauseValue = 0;
}

void MainWindow::updateEratProgressBar(int firstItTime, int allItsTime) {
    double currentProgress = static_cast<double>(firstItTime) / allItsTime * 100;
    ui->eratProgress->setValue(currentProgress);
    qDebug() << QString("Calculating progress: %1").arg(currentProgress);
}

void MainWindow::updateEratTimeUI() {
    int elapsed = eratTimer.elapsed() - timerEratPauseValue;
    ui->eratTime->setText(QString("%1 ms").arg(elapsed));
}

void MainWindow::updateEratEstimatedTime(qint64 estimated) {
    ui->eratEstimated->setText(QString("%1 ms").arg(estimated));
}

void MainWindow::on_eratStop_clicked() {
    eratCalculator.stop();
    realtimeEratTimer.stop();

    //  Re-set values
    ui->factorialStart->setText("Start");
    ui->eratEstimated->setText(QString("0 ms"));
    ui->eratTime->setText(QString("0 ms"));
    timerEratPauseValue = 0;
}


void MainWindow::on_eratPause_clicked() {
    eratCalculator.pause();
    isEratPaused = true;
    ui->eratStart->setText("Resume");

    // Store the current time
    pauseEratTime = QTime::currentTime();
    realtimeEratTimer.stop();

    qDebug() << "Calculation paused.";
}

