#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>
#include <QMainWindow>

namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT




public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::StartWindow *ui;
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent* e);

public:
signals:
    void childSignal(const QString& imagePath);
};

#endif // STARTWINDOW_H
