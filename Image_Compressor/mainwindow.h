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



signals:
public slots:
    void onSetImage(QPixmap qPixmap);
    void onSetImage(QString filePath);
    void displayMainWindow();

private:
    Ui::MainWindow *ui;
    Calculator *calculator;
    StartWindow *startWindow;
};
#endif // MAINWINDOW_H
