#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
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

private slots:
    void on_pushButton_SelectPath_clicked();
    void dropEvent(QDropEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void on_horizontalSlider_sliderReleased();
    void blockUI();


    void on_horizontalSlider_valueChanged(int value);

public: signals:
    void computationFinished();

signals:
public slots:
    void onSetImage(QString filePath);
    void displayMainWindow();
    void onUnlockUI();

private:
    Ui::MainWindow *ui;
    Calculator *calculator;
    StartWindow *startWindow;
    void updateStats(long long singularValue);
    QPixmap* image;
    int k;
    void setImage(QPixmap qPixmap);
    long long pixels, optimalization;
    float weigh, svd_weigh, svd_ratio;
    int max_SingularValues;
public:
    void computeRsvd(int k);
};
#endif // MAINWINDOW_H
