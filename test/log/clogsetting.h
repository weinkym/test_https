#ifndef CLOGSETTING_H
#define CLOGSETTING_H
#include <QDebug>
#include <QTime>
#include <QThread>
#include "cljlogcontent.h"
#include "cljlogdefines.h"

QString adjuestContent(const QString &log);

void lj_out_log(C_LOG_TYPE type,C_LOG_LEVEL level,const QString &lineString,
                const QString &content,const QString &funString,const QString &fileString);

#define C_LOG_FIELD_DATE QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss:zzz: ")
#define C_LOG_FIELD_FILE (QString("[FILE=%1]").arg(QString(__FILE__)))
#define C_LOG_FIELD_FUN (QString("[FUN=%1]").arg(QString(Q_FUNC_INFO)))
#define C_LOG_FIELD_LINE (QString("[LINE=%1]").arg(QString("0000%1").arg(__LINE__).right(4)))


#define C_LOG_P(prefix,value) (QString("%1 ").arg(prefix)+QString(#value"=%1").arg(createLogContent(value)))
#define C_LOG_P2(prefix,value,value2) (QString("%1 ").arg(prefix)+QString(#value"=%1").arg(createLogContent(value))+QString(", "#value2"=%1").arg(createLogContent(value2)))
#define C_LOG_P3(prefix,value,value2,value3) (QString("%1 ").arg(prefix)+QString(#value"=%1").arg(createLogContent(value))+QString(", "#value2"=%1").arg(createLogContent(value2))+QString(", "#value3"=%1").arg(createLogContent(value3)))

class CLogFuncHelper
{
public:
    CLogFuncHelper(const QString &line,const QString &fun,const QString &file);
    ~CLogFuncHelper();
private:
    QString m_lineString;
    QString m_funString;
    QString m_fileString;
    QString m_enterLog;
    QString m_leaveLog;
    QTime m_time;
};

#define C_LOG_FUNCTION CLogFuncHelper logFuncHelper(C_LOG_FIELD_LINE,C_LOG_FIELD_FUN,C_LOG_FIELD_FILE)

#define C_LOG_OUTPUT(type,level,log) lj_out_log(type,level,C_LOG_FIELD_LINE,log,C_LOG_FIELD_FUN,C_LOG_FIELD_FILE)

#define C_LOG_NORMAL_LEVEL(level,log) C_LOG_OUTPUT(C_LOG_TYPE_NORMAL,level,log)

#define C_LOG_TEST_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG_LEVEL_DEBUG,log)

#define C_LOG_DEBUG_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG_LEVEL_DEBUG,log)
#define C_LOG_INFO_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG_LEVEL_INFO,log)
#define C_LOG_WARNING_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG_LEVEL_WARNING,log)
#define C_LOG_ERROR_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG_LEVEL_ERROR,log)
#define C_LOG_FATAL_TYPE(type,log) C_LOG_OUTPUT(type,C_LOG_LEVEL_FATAL,log)

#define C_LOG_TEST(log) C_LOG_TEST_TYPE(C_LOG_TYPE_NORMAL,log)
#define C_LOG_DEBUG(log) C_LOG_DEBUG_TYPE(C_LOG_TYPE_NORMAL,log)
#define C_LOG_INFO(log) C_LOG_INFO_TYPE(C_LOG_TYPE_NORMAL,log)
#define C_LOG_WARNING(log) C_LOG_WARNING_TYPE(C_LOG_TYPE_NORMAL,log)
#define C_LOG_ERROR(log) C_LOG_ERROR_TYPE(C_LOG_TYPE_NORMAL,log)
#define C_LOG_FATAL(log) C_LOG_FATAL_TYPE(C_LOG_TYPE_NORMAL,log)

#define C_LOG_TEST_TT C_LOG_TEST(QString("TTTTTTTTTTTTTTTV"));

#define C_BOOL_VALUE_TO_STRING(value) (value ? "TRUE":"FALSE")

#define C_LOG_VALUE(level,prefix,value) C_LOG_NORMAL_LEVEL(level,C_LOG_P(prefix,value))
#define C_LOG_VALUE2(level,prefix,value,value2) C_LOG_NORMAL_LEVEL(level,C_LOG_P2(prefix,value,value2))
#define C_LOG_VALUE3(level,prefix,value,value2,value3) C_LOG_NORMAL_LEVEL(level,C_LOG_P3(prefix,value,value2,value3))

#endif // CLOGSETTING_H
