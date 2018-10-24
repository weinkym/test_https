#ifndef ZTESTACTION_H
#define ZTESTACTION_H
#include "crequestaction.h"

class ZTestAction : public CRequestAction
{
    Q_OBJECT

public:
    static ZTestAction *createUploadAction(const QString &filePath);
    static ZTestAction *createUploadWYSAction(const QString &filePath);
    ~ZTestAction();

protected:
    ZTestAction(HttpRequestType type, QObject *parent = 0);
    Operation getOperation();
    QNetworkRequest getRequest() const;
    QByteArray getByteArray() const;
    QHttpMultiPart *getMultiPart();

private:
    QString m_filePath;
};

#endif // ZTESTACTION_H
