#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>
#include <QMainWindow>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QScreen>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

signals:
    void setImage(const QString& imagePath);
    void childClosed();

private slots:
    void on_pushButton_clicked();

private:
    Ui::StartWindow *ui;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void closeEvent(QCloseEvent *event);

    bool selectedImage = false;
};

#endif // STARTWINDOW_H
