#include "clogsetting.h"
#include <QThread>
//#include "obs.h"
//#include "util/base.h"

#ifdef Q_OS_WIN
#define C_LOG_FIELD_THREAD_ID (QString("[TID=%1]").arg(QString("000000%1").arg((long long)QThread::currentThreadId()).right(6)))
#else
#define C_LOG_FIELD_THREAD_ID (QString("[TID=%1]").arg(QString("00000000000000000000%1").arg((long long)QThread::currentThreadId()).right(16)))
#endif

CLogFuncHelper::CLogFuncHelper(const QString &line, const QString &fun, const QString &file)
    :m_lineString(line)
    ,m_funString(fun)
    ,m_fileString(file)
{

    lj_out_log(C_LOG_TYPE_FUN_RUN,C_LOG_LEVEL_DEBUG,m_lineString,"ENTER",m_funString,m_fileString);
}

CLogFuncHelper::~CLogFuncHelper()
{
    QString content = QString("LEAVE=%1ms").arg(m_time.msecsTo(QTime::currentTime()));
    lj_out_log(C_LOG_TYPE_FUN_RUN,C_LOG_LEVEL_DEBUG,m_lineString,content,m_funString,m_fileString);
}

int s_number_d = 3;
int s_lowercase_d = 7;
int s_capital_d = 16;

QString decryption(const QString &log)
{
    QByteArray dataLog = log.toLatin1();
    for(int i = 0; i <dataLog.size();++i)
    {
        char c = dataLog[i];
        if(c >= '0' && c <= '9')
        {
            dataLog[i] = '0'+(dataLog[i]-'0'+10-s_number_d) % 10;
        }
        else if(c >= 'a' && c <= 'z')
        {
            dataLog[i] = 'a'+(dataLog[i]-'a'+26-s_lowercase_d) % 26;
        }
        else if(c >= 'A' && c <= 'Z')
        {
            dataLog[i] = 'A'+(dataLog[i]-'A'+26-s_capital_d) % 26;
        }
    }
    QString res(dataLog);
    return res;
}

QString adjuestContent(const QString &log)
{
    QByteArray dataLog = log.toLatin1().toBase64(QByteArray::Base64Encoding);
    for(int i = 0; i <dataLog.size();++i)
    {
        char c = dataLog[i];
        if(c >= '0' && c <= '9')
        {
            dataLog[i] = '0'+(dataLog[i]-'0'+s_number_d) % 10;
        }
        else if(c >= 'a' && c <= 'z')
        {
            dataLog[i] = 'a'+(dataLog[i]-'a'+s_lowercase_d) % 26;
        }
        else if(c >= 'A' && c <= 'Z')
        {
            dataLog[i] = 'A'+(dataLog[i]-'A'+s_capital_d) % 26;
        }
    }
    QString res(dataLog);
    decryption(res);
    return res;
}

void lj_out_log(C_LOG_TYPE type, C_LOG_LEVEL level, const QString &lineString, const QString &content, const QString &funString, const QString &fileString)
{
//    bool encrypted = lj_obs_get_log_encrypted();
    bool encrypted = false;
    QStringList logList;
    if(!encrypted)
    {
        logList.append(C_LOG_FIELD_DATE);
    }
    logList.append(QString("[LEVEL=%1]").arg(level));
    logList.append(C_LOG_FIELD_THREAD_ID);
    logList.append(lineString);
    logList.append(QString("[TYPE=%1]").arg(type));
    logList.append(QString("[CONTENT=%1]").arg(encrypted ? adjuestContent(content) : content));
    logList.append(funString);
    logList.append(fileString);
    QString log = logList.join("");
//    blog(LOG_INFO,"%s",(log.toLocal8Bit().data()));

    qDebug()<<log;

#ifdef Q_OS_WIN
    if(!encrypted)
    {
        qDebug()<<log;
    }
#endif
}
