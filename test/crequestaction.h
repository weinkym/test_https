/*!
 * 版权 (C) 2015 Bitbrothers Science and Technology Co., Ltd.
 *
 * 版权内容见：Bitbrothers Science and Technology Co., Ltd
 *
 * \file crequestaction.h
 * \brief 本文件属于starfish平台的网络通信模块
 * \author CaiWancang <cwc1987@163.com>
 * \date 2015-2-1
 *
 * 主要作用：
 * 本文件是通信模块动作抽象类，抽象类通信动作的通用部分与接口。
 *
 * 修改者: CaiWancang <cwc1987@163.com>
 * 修改时间: 2015-2-1
 *
 * 修改者: fenggl <fenggl@yunxuetang.cn>
 * 修改时间: 2016-5-25
 */
#ifndef _CREQUESTACTION_H_
#define _CREQUESTACTION_H_

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariantMap>
#include <QUuid>
#include <QTimer>
#include "cpublicdefine.h"
#include "clogsetting.h"

/*! \brief 实现了CRequestAction类,用于网络模块通信动作基类。
 *类主要用于抽象通信动作的接口和共同实现
 */

class CRequestAction : public QObject
{
    Q_OBJECT
public:
    /*! \brief 枚举, 定义Http协议操作
     */
    enum Operation
    {
        Undefine = 0, /*!< 未定义操作 */
        PostByteArray, /*!< 发送文本信息 */
        PostWithttachment, /*!< 发送QHttpMultiPart */
        Get, /*!< 请求数据 */
        Delete, /*!< 删除数据 */
        PatchByteArray, /*!< 更新文本数据 */
        PatchMultimedia,/*!< 更新多媒体数据 */
        PutByteArray
    };

    //! Http返回的状态码
    enum HttpStatesCode
    {
        OK = 200,             /*!< 成功 */
        PartialContent = 206,  /*!< 断点续传 */
        NotChanged = 304     /*!< 所请求资源无变化 */
    };

    /*! \brief 构造函数
     *  \param parent 父对象指针
     */
    explicit CRequestAction(HttpRequestType type, QObject *parent = 0);

    /*! \brief 析构函数
     */
    virtual ~CRequestAction();

    /*! \brief 触发请求动作
     */
    void startRequestAction();

    /*! \brief 触发网络通信动作
     */
    void trigger();

    /*! \brief 获取当前动作类型
     *  \return 返回动作类型
     */
    int getType() const;

    /*! \brief 获取当前动作使用的Http请求头
     *  \return 返回Http请求头
     */
    virtual QNetworkRequest getRequest() const;
    QUuid getUuid() { return m_replayStatusData.uuid; }

    static void setZhiboToken(const QString &token);
    static QString getBoundary();

protected:
    /*! \brief 获取当动作使用的Http协议类型
     *  \return 返回Http协议类型
     */
    virtual Operation getOperation();

    /*! \brief 获取当前动作Http post的字符串数据
     *  \return 返回动作Http post的字符串数据
     */
    virtual QByteArray getByteArray() const;

    /*! \brief 获取当前动作Http post的多媒体数据
     *  \return 返回动作Http post的多媒体数据
     */
    virtual QHttpMultiPart *getMultiPart();

    /*! \brief 创建Http请求头
     *  \param isOrgApi 是否是组织相关的api请求
     *  \param lastHeader 请求头地址
     */
    QNetworkRequest createRequest(const QString &lastHeader, bool needVerify = false) const;

private:
    void setReply(QNetworkReply *reply);
    void initTimeoutTimer();

protected slots:
    void onReplyFinished();
    virtual void onReplyError(QNetworkReply::NetworkError error);
    void onIgnoreSSlErrors(QList<QSslError> sslErrors);
    void onHttpTimeout();

signals:
    void sigRequestFinished(const CPB::RequestReplyData &replayStatusData,const QVariant &replyData);

protected:
    friend class CRequestAccessManager;
    QNetworkReply *m_networkReply; /*!< 用来访问网络请求结果的指针 */

    int m_httpTimeout;  /*!< http超时毫秒 */
    int m_maxRetryTime;  /*!< 网络错误引发的网络命令重试最大次数 */
    int m_curRetryTime /*!< 当前重试次数 */;
    QTimer *m_timerTimeout; /*!< 网络请求超时的timer */

    CPB::RequestReplyData m_replayStatusData;
    static const int m_defaultHttpTimeout = 30000;  /*!< 默认http超时毫秒 */
    static const int m_defaultMaxRetryTime = 0; /*!< 默认网络错误引发的网络命令重试最大次数 */
    static QString m_secretKey;
    static QString m_appKey;
    static QString m_zhiboToken;
    static QString m_boundary;

};

#endif // _CREQUESTACTION_H_
