/*!
 * 版权 (C) 2015 Bitbrothers Science and Technology Co., Ltd.
 *
 * 版权内容见：Bitbrothers Science and Technology Co., Ltd
 *
 * \file networkaccess.h
 * \brief 本文件属于starfish平台的网络通信模块
 * \author CaiWancang <cwc1987@163.com>
 * \date 2015-2-1
 *
 * 主要作用：
 * 本文件是网络通信信息管理,实现了Http信息的发送与接收,通信消息的重发及错误处理等。
 *
 * 修改者: Guilai <guilai.feng@hotmail.com>
 * 修改时间: 2015-6-16
 *
 * 修改者: Guilai <fenggl@yunxuetang.cn>
 * 修改时间: 2016-5-25
 * 修改内容: 添加错误处理
 */

#ifndef _CREQUESTACCESSMANAGER_H_
#define _CREQUESTACCESSMANAGER_H_

#include <QObject>
#include <QNetworkAccessManager>
#include <QBuffer>
#include "crequestaction.h"


/*! \brief 实现了NetworkAccess类,用于网络模块动作的触发和响应
 *类主要用于从RequestAction中获取网络通信信息并使用Http协议发送/接收
 */
class CRequestAccessManager : public QObject
{
    Q_OBJECT
public:
    /*! \brief 静态函数,用于NetworkAccess单例的实现
     *  \return 返回NetworkAccess的唯一实例
     */
    static CRequestAccessManager *getInstance();

    /*! \brief 获取NetworkAccess使用的QNetworkAccessManager
     *  \return 返回唯一实例使用的QNetworkAccessManager
     */
    QNetworkAccessManager* getAccessMgr() { return m_access; }

    /*! \brief 设置cookie
     *  \param cookieList 待设置的cookie list
     *  \param domain cookie相关联的domain
     *  \return ture, 设置cookie生效； false， 设置cookie失效
     */
    bool setCookie(const QVariant &cookieListVariant, const QString &domain);

    /*! \brief 使用Http协议post发送文本数据
     *  \param action 网络通信动作
     */
    void postByteArray(CRequestAction *action);

    void putByteArray(CRequestAction *action);

    void postWithttachment(CRequestAction *action);

    /*! \brief 使用Http协议get请求数据
     *  \param action 网络通信动作
     */
    void get(CRequestAction *action);

    /*! \brief 使用Http协议patch更新文本数据
     *  \param action 网络通信动作
     */
    void patchByteArray(CRequestAction *action);

    /*! \brief 使用Http协议patch更新多媒体数据
     *  \param action 网络通信动作
     */
    void patchMulti(CRequestAction *action);

    void deleteResource(CRequestAction *action);

    /*! \brief 重置网络访问状态。解决QNetworkAccessManager 的bug。bug 被fix 之后可以去掉此函数;
     */
    void recoverNetworkAccessible();

private slots:
    /*! \brief 私有槽函数，忽略ssl错误
     *  \param reply network reply
     *  \param sslErrors 返回的ssl 错误
     */
    void onIgnoreSSlErrors(QNetworkReply* reply, QList<QSslError> sslErrors);

    /*! \brief 私有槽函数，网络请求结束回收资源
     *  \param reply 请求对应的reply
     */
    void onRequestFinished(QNetworkReply *reply);

protected:
    /*! \brief 构造函数
     *  \param parent 父对象指针
     */
    explicit CRequestAccessManager(QObject *parent = 0);

private:
    QNetworkAccessManager *m_access;/*!< 网络通信管理指针 */
    static CRequestAccessManager *m_instance;/*!< 唯一实例*/
};

#endif // _CREQUESTACCESSMANAGER_H_
