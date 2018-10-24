#include <QSize>
#include <QFile>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <ctime>
#include <QCryptographicHash>
#include "crequestaction.h"
#include "crequestaccessmanager.h"
//#include "ccommon.h"
//#include "cljutils.h"


//QString CRequestAction::m_appKey = "d82680c1ca7e435b98a251472261880c";
//QString CRequestAction::m_secretKey = "d7a5fea84fe93a706edb16473cc89ee94dab3719";
//QString CRequestAction::m_appKey = "d842d7d064dd4338b0632472b5a4ddcb";
QString CRequestAction::m_appKey = "77d632c3dbd5421bb13bdbacad0da52a";
//QString CRequestAction::m_secretKey = "2178ffb77f74bfa48b137e6f9aa4179af1d11892";

//QString CRequestAction::m_secretKey = "301c46a6e0b621f99282d1fa585f95870d715113";
QString CRequestAction::m_secretKey = "9ad3c9d917ef370b3b91a52150404202117e1775";
QString CRequestAction::m_zhiboToken = "";

QString CRequestAction::m_boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";


CRequestAction::CRequestAction(HttpRequestType type, QObject *parent) :
    QObject(parent),
    m_networkReply(NULL),
    m_httpTimeout(m_defaultHttpTimeout),
    m_maxRetryTime(m_defaultMaxRetryTime),
    m_curRetryTime(0),
    m_timerTimeout(NULL)
{
    m_replayStatusData.type = type;
    m_replayStatusData.uuid = QUuid::createUuid();
    initTimeoutTimer();
}

CRequestAction::~CRequestAction()
{
    //    C_LOG_DEBUG << QString("http request(type=%1) consume: %2ms")
    //                 .arg(m_type).arg(m_timeTest.elapsed());
    if(m_networkReply)
    {
        delete m_networkReply;
        m_networkReply = NULL;
    }
}

void CRequestAction::trigger()
{
    m_replayStatusData.startTime = QDateTime::currentDateTime();
    Operation operation = getOperation();
    CRequestAccessManager *accessManager = CRequestAccessManager::getInstance();
    switch(operation)
    {
    case PostByteArray:
        accessManager->postByteArray(this);
        break;
    case Get:
        accessManager->get(this);
        break;
    case PatchByteArray:
        accessManager->patchByteArray(this);
        break;
    case PatchMultimedia:
        accessManager->patchMulti(this);
        break;
    case PostWithttachment:
        accessManager->postWithttachment(this);
        break;
    case Delete:
        accessManager->deleteResource(this);
        break;
    case PutByteArray:
        accessManager->putByteArray(this);
        break;
    default:
        C_LOG_ERROR(QString("Bad action with type = [%1]").arg(operation));
        break;
    }
}

// !TODO add by Guilai, 函数名字和trigger 很奇怪，后续调整，数据获取途径可能也可以调整
void CRequestAction::startRequestAction()
{
    trigger();
}

void CRequestAction::setReply(QNetworkReply *reply)
{
    if(reply == NULL)
    {
        C_LOG_ERROR("param reply is NULL");
        return;
    }
    if(m_networkReply)
    {
        disconnect(m_networkReply,SIGNAL(finished()),
                   this,SLOT(onReplyFinished()));
        m_networkReply->deleteLater();
        m_networkReply = NULL;
    }
    m_networkReply = reply;
    connect(m_networkReply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
    connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onReplyError(QNetworkReply::NetworkError)));
    connect(m_networkReply, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onIgnoreSSlErrors(const QList<QSslError>&)));

    m_timerTimeout->setInterval(m_httpTimeout);
    m_timerTimeout->start();
}

int CRequestAction::getType() const
{
    return m_replayStatusData.type;
}

void CRequestAction::initTimeoutTimer()
{
    m_timerTimeout = new QTimer(this);
    m_timerTimeout->setSingleShot(true);
    connect(m_timerTimeout, SIGNAL(timeout()), this, SLOT(onHttpTimeout()));
}

CRequestAction::Operation CRequestAction::getOperation()
{
    return Undefine;
}

QNetworkRequest CRequestAction::getRequest() const
{
    return QNetworkRequest();
}

void CRequestAction::setZhiboToken(const QString &token)
{
    m_zhiboToken = token;
}

QString CRequestAction::getBoundary()
{
    return QString("--%1\r\n").arg(m_boundary);
}

QByteArray CRequestAction::getByteArray() const
{
    return QByteArray();
}

QHttpMultiPart *CRequestAction::getMultiPart()
{
    return NULL;
}

void CRequestAction::onReplyError(QNetworkReply::NetworkError error)
{
    C_LOG_ERROR(QString("Get reply error no = [%1]").arg(error));
    if (error == QNetworkReply::UnknownNetworkError)
    {
        CRequestAccessManager::getInstance()->recoverNetworkAccessible();
    }

    if(m_networkReply == NULL)
    {
        C_LOG_ERROR(QString("Bad network reply."));
        return;
    }
}

void CRequestAction::onHttpTimeout()
{
    C_LOG_FUNCTION;
    if(m_networkReply == NULL)
    {
        C_LOG_ERROR(QString("Http timeout, bad network reply"));
        return;
    }
    m_networkReply->abort();
}

void CRequestAction::onIgnoreSSlErrors(QList<QSslError> sslErrors)
{
    for (int i = 0; i < sslErrors.size(); ++i)
    {
        C_LOG_ERROR(QString("Get ssl error [%1]").arg(sslErrors[i].errorString()));
    }

    m_networkReply->ignoreSslErrors(sslErrors);
}

void CRequestAction::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply == NULL)
    {
        C_LOG_ERROR(QString("Get bad QNetworkReply = NLL"));
        return;
    }

    if(m_networkReply != reply)
    {
        //该网络请求已过期
        C_LOG_WARNING(QString("This network request is overdue, action type:%1").arg(m_replayStatusData.type));
        return;
    }
    m_timerTimeout->stop();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    m_replayStatusData.statusCode = statusCode;

    m_replayStatusData.endTime = QDateTime::currentDateTime();
    m_replayStatusData.networkErrorCode = reply->error();
    int consuming = m_replayStatusData.endTime.time().msecsTo(m_replayStatusData.startTime.time());
    consuming = qAbs(consuming);
    QString urlString = reply->request().url().toString();
    QString zhiboToken(reply->request().rawHeader("zhiboToken"));

    QString postArray(this->getByteArray());
    QByteArray returnArray = reply->readAll();

    C_LOG_INFO_TYPE(C_LOG_TYPE_API_DATA,QString("postArray=%1").arg(QString(postArray)));
    C_LOG_INFO_TYPE(C_LOG_TYPE_API_DATA,QString("returnArray=%1").arg(QString(returnArray)));

    C_LOG_INFO_TYPE(C_LOG_TYPE_API_DATA,C_LOG_P("onReplyFinished",postArray));
    C_LOG_INFO_TYPE(C_LOG_TYPE_API_DATA,C_LOG_P("onReplyFinished",returnArray));

    C_LOG_INFO_TYPE(C_LOG_TYPE_API_DATA,C_LOG_P3("onReplyFinished",urlString,zhiboToken,m_replayStatusData.uuid.toString()));
    C_LOG_INFO_TYPE(C_LOG_TYPE_API_DATA,C_LOG_P3("onReplyFinished",m_replayStatusData.type,statusCode,consuming));
    C_LOG_INFO_TYPE(C_LOG_TYPE_API_DATA,C_LOG_P2("onReplyFinished",returnArray.size(),m_replayStatusData.networkErrorCode));

    if(m_replayStatusData.networkErrorCode == QNetworkReply::NoError)
    {
        emit sigRequestFinished(m_replayStatusData,returnArray);
    }
    else
    {
        C_LOG_ERROR_TYPE(C_LOG_TYPE_API_ERROR_DATA,C_LOG_P3("onReplyFinished",m_replayStatusData.type,m_replayStatusData.networkErrorCode,urlString));
        if(m_curRetryTime < m_maxRetryTime)
        {
            C_LOG_WARNING_TYPE(C_LOG_TYPE_API_ERROR_DATA,C_LOG_P2("Action trigger again",m_replayStatusData.type,m_curRetryTime));
            m_curRetryTime++;
            trigger();
            return;
        }
        else
        {
            emit sigRequestFinished(m_replayStatusData,returnArray);
        }
    }
    this->deleteLater();
}

QNetworkRequest CRequestAction::createRequest(const QString &lastHeader, bool needVerify) const
{
    C_LOG_FUNCTION;
    QString urlPrefix = "https://sandbox.ppj.io/api";


    QUrl url(urlPrefix + lastHeader);

    QNetworkRequest request(url);
//    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("cache-control", "no-cache");
//    request.setRawHeader(QString("Content-Type").toLatin1(),QString("multipart/form-data;boundary="+m_boundary).toLatin1());
    //    request.setRawHeader("Content-Type", "application/zip");
//    request.setHeader(QNetworkRequest::ContentTypeHeader,"multipart/form-data;boundary="+m_boundary);
    request.setRawHeader("X-PPJ-Credential", "YWotn1hoTbo1UZJ2oCF4kKsX");
//    request.setRawHeader("zhiboToken", m_zhiboToken.toLatin1());
    //curl -i -H "X-PPJ-Credential:YWotn1hoTbo1UZJ2oCF4kKsX" -H "Content-Type:multipart/form-data" https://sandbox.ppj.io/api/jobs -XPOST -F 'file_source=@test03.pptx'
    return request;
}
