#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ztestaction.h"
#include "clogsetting.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    C_LOG_FUNCTION;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    C_LOG_FUNCTION;
//    ZTestAction *action = ZTestAction::createUploadAction("/Users/miaozw/Documents/test03.pptx");
    ZTestAction *action = ZTestAction::createUploadWYSAction("/Users/miaozw/Documents/test03.pptx");
    connect(action, SIGNAL(sigRequestFinished(CPB::RequestReplyData,QVariant)), this, SLOT(onRequestFinished(CPB::RequestReplyData,QVariant)));
    action->trigger();
}

void MainWindow::onRequestFinished(const CPB::RequestReplyData &replayStatusData, const QVariant &replyData)
{
    C_LOG_FUNCTION;
}
