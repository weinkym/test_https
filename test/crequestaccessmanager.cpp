#include "crequestaccessmanager.h"
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QThread>
#include <QTimer>
//#include "ccommon.h"
//#include "cljutils.h"

CRequestAccessManager *CRequestAccessManager::m_instance = NULL;
CRequestAccessManager *CRequestAccessManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CRequestAccessManager();
    }
    return m_instance;
}

CRequestAccessManager::CRequestAccessManager(QObject *parent) :
    QObject(parent),
    m_access(NULL)
{
    m_access = new QNetworkAccessManager();
    connect(m_access, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(onIgnoreSSlErrors(QNetworkReply*,QList<QSslError>)));
    connect(m_access, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestFinished(QNetworkReply*)));
}

// !TODO add by Guilai; 函数可能返回错误，上层逻辑需要处理这个错误信息，不能返回void
bool CRequestAccessManager::setCookie(const QVariant &cookieListVariant, const QString &domain)
{
    bool ret = false;
    QList<QNetworkCookie> cookieList = cookieListVariant.value<QList<QNetworkCookie> >();
    if(!cookieList.isEmpty())
    {
        QNetworkCookieJar *cookieJar = m_access->cookieJar();
        if(cookieJar != NULL)
        {
            foreach(QNetworkCookie cookie, cookieList)
            {
                cookie.setDomain(domain);
                if (cookieJar->updateCookie(cookie) == false)
                {
                    C_LOG_ERROR(QString("update cookie failed. The cookie is not effect."));
                    ret = false;
                    break;
                }
            }
            ret = true;
        }
        else
        {
            cookieJar = new QNetworkCookieJar();
            foreach(QNetworkCookie cookie, cookieList)
            {
                cookie.setDomain(domain);
                if (cookieJar->insertCookie(cookie) == false)
                {
                    C_LOG_ERROR(QString("Insert cookie failed. The cookie is not effect."));
                    ret = false;
                    break;
                }
            }

            m_access->setCookieJar(cookieJar);
            ret = true;
        }
    }
    else
    {
        C_LOG_ERROR(QString("Set bad cookie list. Cookie list are empty."));
        ret = false;
    }

    return ret;
}

void CRequestAccessManager::onRequestFinished(QNetworkReply *reply)
{
    Q_UNUSED(reply);
}

void CRequestAccessManager::onIgnoreSSlErrors(QNetworkReply* reply, QList<QSslError> sslErrors)
{
    for (int i = 0; i < sslErrors.size(); ++i)
    {
        C_LOG_ERROR(QString("Get ssl error [%1]").arg(sslErrors[i].errorString()));
    }
    reply->ignoreSslErrors(sslErrors);
}

void CRequestAccessManager::postByteArray(CRequestAction *action)
{
    C_LOG_FUNCTION;
    if(action == NULL)
    {
        C_LOG_WARNING(QString("action is null"));
        return;
    }
    QByteArray array = action->getByteArray();
    QNetworkRequest request = action->getRequest();
    request.setRawHeader(QString("Content-Length").toLatin1(),QString::number(array.length()).toLatin1());

    QString logInfo = QString("request.url().toString()=%1").arg(request.url().toString());
    C_LOG_TEST(logInfo);
    C_LOG_TEST(QString("post array=%1").arg(QString(array)));
    QNetworkReply *reply = m_access->post(request, array);
    action->setReply(reply);
}

void CRequestAccessManager::putByteArray(CRequestAction *action)
{
    C_LOG_FUNCTION;
    if(action == NULL)
    {
        C_LOG_WARNING(QString("action is null"));
        return;
    }
    QByteArray array = action->getByteArray();
    QNetworkRequest request = action->getRequest();

    QString logInfo = QString("[Http Request][PUT]url: ") + request.url().toString();
    C_LOG_TEST(logInfo);
    QNetworkReply *reply = m_access->put(request, array);
    action->setReply(reply);
}

void CRequestAccessManager::postWithttachment(CRequestAction *action)
{
    C_LOG_FUNCTION;
    if(action == NULL)
    {
        C_LOG_WARNING(QString("action is null"));
        return;
    }
    QNetworkRequest request = action->getRequest();
    QHttpMultiPart *multiPart = action->getMultiPart();

    C_LOG_TEST(QString("[Http Request][Post]post url: %1").arg(request.url().toString()));
    QNetworkReply *reply = m_access->post(request,multiPart);

    action->setReply(reply);
}

void CRequestAccessManager::get(CRequestAction *action)
{
    C_LOG_FUNCTION;
    if(action == NULL)
    {
        C_LOG_WARNING(QString("action is null"));
        return;
    }
    QNetworkRequest request = action->getRequest();

    QString url = request.url().toString();
    if(!url.startsWith("http"))
    {
        QString typeString = QString::number(action->getType());
        C_LOG_ERROR(QString("[Http Request][Get]url not start with http, url is:%1,action type:%2,className:%3")
                  .arg(url).arg(typeString).arg(action->metaObject()->className()));
        return;
    }
    C_LOG_TEST(QString("[Http Request][Get]get url: %1").arg(request.url().toString()));
    QNetworkReply *reply = m_access->get(request);
    action->setReply(reply);
}

void CRequestAccessManager::patchByteArray(CRequestAction *action)
{
    C_LOG_FUNCTION;
    if(action == NULL)
    {
        C_LOG_WARNING(QString("action is null"));
        return;
    }
    QNetworkRequest request = action->getRequest();
    QByteArray array = action->getByteArray();

    QBuffer *buffer = new QBuffer;
    buffer->setData(array);

    QString patchString = array;
    C_LOG_TEST(QString("[Http Request][Patch]array:%1  url:%2").arg(patchString).arg( request.url().toString()));
    QNetworkReply *reply = m_access->sendCustomRequest(request, QByteArray("PATCH"), buffer);
    action->setReply(reply);
    buffer->setParent(reply);
}

void CRequestAccessManager::patchMulti(CRequestAction *action)
{
    if(action == NULL)
    {
        C_LOG_WARNING(QString("action is null"));
        return;
    }
    QNetworkRequest request = action->getRequest();
    QByteArray array = action->getByteArray();

    QBuffer *buffer = new QBuffer;
    buffer->setData(array);

    C_LOG_TEST(QString("[Http Request][Patch]patchMulti url: ") + request.url().toString());
    QNetworkReply *reply = m_access->sendCustomRequest(request, QByteArray("PATCH"), buffer);
    action->setReply(reply);
    buffer->setParent(reply);
}

void CRequestAccessManager::deleteResource(CRequestAction *action)
{
    if(action == NULL)
    {
        C_LOG_ERROR(QString("action is null"));
        return;
    }
    QNetworkRequest request = action->getRequest();
    C_LOG_TEST(QString("[Http Request][Delete]url: ") + request.url().toString());
    QNetworkReply *reply = m_access->deleteResource(request);
    action->setReply(reply);
}

// !TODO add by Guilai. QNetworkAccessManager have bug, so call the function when get network error = 99;
void CRequestAccessManager::recoverNetworkAccessible()
{
    m_access->setNetworkAccessible(QNetworkAccessManager::NotAccessible);
    m_access->clearAccessCache();
    m_access->setNetworkAccessible(QNetworkAccessManager::Accessible);
}
