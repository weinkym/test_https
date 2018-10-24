#include "mainwindow.h"
#include <QApplication>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <QFile>
#include <QWebEngineView>
#include <QWebEngineSettings>

void handler(int sig)
{
    printf("sig:%d\n",sig);
}

void test()
{
    QFile file("/Users/miaozw/work/ljlive/ljobs/releasenote.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QString startKey = "EGWIN";
    QString allKey = "EGALL";
    QStringList contentList;
    while(!file.atEnd())
    {
        QString line = file.readLine();
        if(line.startsWith(startKey))
        {
            QString note = line.right(line.length() - startKey.length());
            qDebug()<<note;
            contentList.append(note);
        }
        if(line.startsWith(allKey))
        {
            QString note = line.right(line.length() - allKey.length());
            qDebug()<<note;
            contentList.append(note);
        }
    }

}

int main(int argc,char *argv[])
{
//    test();
//    return -1;
//    printf("pid:%d\n",getpid());
//    signal(SIGUSR1,handler);
//    signal(SIGUSR2,handler);
//    signal(SIGINT,handler);

//    sigset_t new_set,old_set;

//    sigemptyset(&new_set);
//    sigaddset(&new_set,SIGINT);
//    sigaddset(&new_set,SIGUSR1);

//    sigprocmask(SIG_SETMASK,&new_set,&old_set);

//    sigset_t pendmask;
//    sigemptyset(&pendmask);
//    sigaddset(&pendmask,SIGUSR1);
//    sigaddset(&pendmask,SIGUSR2);

//    sigsuspend(&pendmask);//block

//    printf("sigsuspend finish\n");
//    while(1) ;
//    return 1;
    QApplication a(argc,argv);
    QWebEngineView w;
    w.settings()->setAttribute(QWebEngineSettings::AutoLoadImages,true);
    w.settings()->setAttribute(QWebEngineSettings::JavascriptEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows,true);
    w.settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard,true);
    w.settings()->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain,true);
    w.settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls,true);
    w.settings()->setAttribute(QWebEngineSettings::XSSAuditingEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::SpatialNavigationEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls,true);
    w.settings()->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::PluginsEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::AutoLoadImages,true);

    w.settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::WebGLEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled,true);
    w.settings()->setAttribute(QWebEngineSettings::AutoLoadIconsForPage,true);
    w.settings()->setAttribute(QWebEngineSettings::TouchIconsEnabled,true);



//    w.load(QUrl("https://webclasstest.bebolive.com/webclass/whiteboard/init?webclass=XJx99lMkHh1BITfyLwH9BQwDNjMMM%2F8ICDEzMP0uBv8GBQYxOAo19P72RHFsRW0aEPAN8WFuPkw3PjlRYktCVzIuQiAlZyRR"));
//    w.load(QUrl("https://s6.aconvert.com/convert/p3r68-cdx67/cbzgp-i6iub.html"));
//    w.load(QUrl("http://h5.ppj.io/SEwdis3a/index.html?v=2"));
//    w.setMinimumSize(1280,720);
//    w.show();

    MainWindow mw;
    mw.show();
    return a.exec();
}
