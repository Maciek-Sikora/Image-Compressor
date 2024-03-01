#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QScreen>
#include <QObject>
#include <QGuiApplication>
#include <QtConcurrent>
#include <QFuture>
#include <QThread>
#include <QString>
#include "calculator.h"
#include "startwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void computationFinished();

public slots:
    void onSetImage(QString filePath);
    void displayMainWindow();
    void onUnlockUI();

private slots:
    void on_pushButton_SelectPath_clicked();
    void dropEvent(QDropEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void on_horizontalSlider_sliderReleased();
    void blockUI();
    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_SaveImage_clicked();

private:
    Ui::MainWindow *ui;
    Calculator *calculator;
    StartWindow *startWindow;
    QPixmap *image;
    QPixmap *reconstructedImage;
    int k;
    QFutureWatcher<void> *watcher;
    void setImage(QPixmap& img);
    bool checkPixmap(const QPixmap &qPixmap);
    void updateStats(long long singularValue);

    long long pixels;
    long long optimization;
    float weigh;
    float svd_weigh;
    float svd_ratio;
    int max_SingularValues;
    void computeRsvd(int k);

};
#endif // MAINWINDOW_H
