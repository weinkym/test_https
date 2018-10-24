#include "ztestaction.h"
#include <QFile>
#include <QHttpMultiPart>

ZTestAction *ZTestAction::createUploadAction(const QString &filePath)
{
    ZTestAction *action = new ZTestAction(TYPE_REQUEST_UPLOAD);
    action->m_filePath = filePath;
    return action;
}

ZTestAction *ZTestAction::createUploadWYSAction(const QString &filePath)
{
    ZTestAction *action = new ZTestAction(TYPE_REQUEST_UPLOAD_WYS);
    action->m_filePath = filePath;
    return action;
}

ZTestAction::ZTestAction(HttpRequestType type, QObject *parent)
    :CRequestAction(type,parent)
{

}

CRequestAction::Operation ZTestAction::getOperation()
{
    switch (m_replayStatusData.type)
    {
    case TYPE_REQUEST_UPLOAD:
//        return PostByteArray;
        return PostWithttachment;
        break;
    case TYPE_REQUEST_UPLOAD_WYS:
//        return PostByteArray;
        return PostWithttachment;
        break;
    case TYPE_REQUEST_DOWNLOAD:
    case TYPE_REQUEST_CHECK:
        return Get;
        break;
    default:
        break;
    }
    return Get;
}

QNetworkRequest ZTestAction::getRequest() const
{


    if(getType() == TYPE_REQUEST_UPLOAD)
    {
        QString urlPrefix = "https://sandbox.ppj.io/api";
        QString lastHeader = "/jobs";
        QUrl url(urlPrefix + lastHeader);
        QNetworkRequest request(url);
        request.setRawHeader("cache-control", "no-cache");
        request.setRawHeader("X-PPJ-Credential", "YWotn1hoTbo1UZJ2oCF4kKsX");
        return request;
    }
    if(getType() == TYPE_REQUEST_UPLOAD_WYS)
    {
        QString urlPrefix = "http://ts.whytouch.com/mass_up.php?token=89301494561fad6b6388146e2c6f7cd5&mode=0&aut=mzw";
        QString lastHeader = QString("&fn=%1.pptx").arg(QTime::currentTime().toString("hhmmss"));
        QUrl url(urlPrefix + lastHeader);
        QNetworkRequest request(url);
        return request;
    }
    return QNetworkRequest();
}

QByteArray ZTestAction::getByteArray() const
{
    QByteArray retValue;
    if(m_replayStatusData.type == TYPE_REQUEST_UPLOAD)
    {
        QFile file(m_filePath);
        if(file.open(QIODevice::ReadOnly))
        {

            retValue.append(getBoundary());
//            QString lineString = QString("Content-Disposition: form-data;name=\"")+QString("file_source")+QString("\";file_source=\"")+QString("test03.pptx")+QString("\"")+QString("\r\n");
//            QString lineString = QString("Content-Disposition: form-data; name=\"file_source\"; filename=\"test03.pptx\"")+QString("\r\nContent-Type: application/vnd.openxmlformats-officedocument.presentationml.presentation\r\n");
            QString lineString = QString("Content-Disposition: form-data; name=\"file_source\"; filename=\"test03.pptx\"");
           retValue.append(lineString);
           retValue.append("\r\n");
            C_LOG_INFO(lineString);
//            retValue.append("\r\n");
            retValue.append(file.readAll());
            retValue.append("\r\n");
//            retValue.append("\r\n");
//            retValue.append("\r\n");
            retValue.append(getBoundary()+"--");

        }
        else
        {
            C_LOG_INFO(QString("open file is error = %1").arg(m_filePath));
        }
    }
    return retValue;
}

QHttpMultiPart *ZTestAction::getMultiPart()
{
    if(m_replayStatusData.type == TYPE_REQUEST_UPLOAD)
    {
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/vnd.openxmlformats-officedocument.presentationml.presentation"));
        //    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file_source\"; filename=\"test03.pptx\""));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file_source\"; filename=\"test.pptx\""));
        QFile *file = new QFile(m_filePath);
        bool ok = file->open(QIODevice::ReadOnly);
        C_LOG_VALUE(C_LOG_LEVEL_INFO,"====getMultiPart",ok);
        imagePart.setBodyDevice(file);
        file->setParent(multiPart);
        multiPart->append(imagePart);
        multiPart->setBoundary(m_boundary.toLatin1());
        return multiPart;
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/vnd.openxmlformats-officedocument.presentationml.presentation"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"doc\"; filename=\"test.pptx\""));
    QFile *file = new QFile(m_filePath);
    bool ok = file->open(QIODevice::ReadOnly);
    C_LOG_VALUE(C_LOG_LEVEL_INFO,"====getMultiPart",ok);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);
    multiPart->setBoundary(m_boundary.toLatin1());
    return multiPart;
}

ZTestAction::~ZTestAction()
{

}
