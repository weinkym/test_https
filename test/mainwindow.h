#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cpublicdefine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void onRequestFinished(const CPB::RequestReplyData &replayStatusData,const QVariant &replyData);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
