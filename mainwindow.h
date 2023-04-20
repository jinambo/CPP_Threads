#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include "factorial.h"
#include "erat.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Factorial
    void on_factorialStart_clicked();
    void on_factorialPause_clicked();
    void on_factorialStop_clicked();
    void getResult(unsigned long int result);
    void updateProgressBar(int iteration, int maxIterations);
    void updateTimeUI();
    void updateEstimatedTime(qint64 estimated);

    // Erat
    void on_eratStart_clicked();
    void on_eratStop_clicked();
    void on_eratPause_clicked();
    void getResultPrimes(const QList<int> &primes);
    void updateEratProgressBar(int firstItTime, int allItsTime);
    void updateEratTimeUI();
    void updateEratEstimatedTime(qint64 estimated);

private:
    Ui::MainWindow *ui;

    // Factorial
    Factorial factorialCalculator;
    bool isCalculationPaused = false;
    QTimer realtimeTimer;
    QElapsedTimer timer;
    QTime startTime;
    QTime pauseTime;
    int timerPauseValue = 0;

    // Erat
    Erat eratCalculator;
    bool isEratPaused = false;
    QTimer realtimeEratTimer;
    QElapsedTimer eratTimer;
    QTime startEratTime;
    QTime pauseEratTime;
    int timerEratPauseValue = 0;
};
#endif // MAINWINDOW_H
