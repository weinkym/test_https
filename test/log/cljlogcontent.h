#ifndef CLJLOGCONTENT_H
#define CLJLOGCONTENT_H
#include <QLine>
#include <QLineF>
#include <QTime>
#include <QDateTime>
#include <QSize>
#include <QSizeF>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QString>

template<typename T>
class CLJLogCreate
{
public:
    static QString createValue(const T &value)
    {
      return QString("%1").arg(value);
    };

private:

};

template<>
class CLJLogCreate<QLine>
{
public:
    static QString createValue(const QLine &value)
    {
        return QString("(pos1=%1,%2,pos2=%3,%4)").arg(value.x1()).arg(value.y1()).arg(value.x2()).arg(value.y2());
    };

private:
};

template<>
class CLJLogCreate<QLineF>
{
public:
    static QString createValue(const QLineF &value)
    {
        return QString("(pos1=%1,%2,pos2=%3,%4)").arg(value.x1()).arg(value.y1()).arg(value.x2()).arg(value.y2());
    };

private:
};

template<>
class CLJLogCreate<QSize>
{
public:
    static QString createValue(const QSize &value)
    {
        return QString("(w=%1,h=%2)").arg(value.width()).arg(value.height());
    };

private:
};

template<>
class CLJLogCreate<QSizeF>
{
public:
    static QString createValue(const QSizeF &value)
    {
        return QString("(w=%1,h=%2)").arg(value.width()).arg(value.height());
    };

private:
};

template<>
class CLJLogCreate<QRect>
{
public:
    static QString createValue(const QRect &value)
    {
        return QString("(x=%1,y=%2,w=%3,h=%4)").arg(value.x()).arg(value.y()).arg(value.width()).arg(value.height());
    };

private:
};

template<>
class CLJLogCreate<QRectF>
{
public:
    static QString createValue(const QRectF &value)
    {
        return QString("(x=%1,y=%2,w=%3,h=%4)").arg(value.x()).arg(value.y()).arg(value.width()).arg(value.height());
    };

private:
};

template<>
class CLJLogCreate<QTime>
{
public:
    static QString createValue(const QTime &value)
    {
        return QString("(%1)").arg(value.toString("hh:mm:ss"));
    };

private:
};

template<>
class CLJLogCreate<QDateTime>
{
public:
    static QString createValue(const QDateTime &value)
    {
        return QString("(%1)").arg(value.toString("yyyy-MM-dd hh:mm:ss"));
    };

private:
};

template<>
class CLJLogCreate<bool>
{
public:
    static QString createValue(const bool &value)
    {
        return QString("(%1)").arg(value ? "T":"F");
    };

private:
};

template<>
class CLJLogCreate<QByteArray>
{
public:
    static QString createValue(const QByteArray &value)
    {
        return QString(value);
    };

private:
};

template<typename T>
QString createLogContent(const T& data)
{
    return CLJLogCreate<T>::createValue(data);
}

#endif // CLJLOGCONTENT_H
