#ifndef ERROR_H
#define ERROR_H

#include "QtCore/qobject.h"
#include <QList>
#include <QString>

class Error
{

    Q_GADGET
  public:
    enum Status
    {
        Ok,
        Warning,
        Critical,
        Fatal
    };
    Q_ENUM(Status)

    //--------------------------------------------------------------

    explicit Error(const QObject *object, const Error::Status &status, const QString &code)
        : m_status(status), m_code(code)
    {
        m_className = object->metaObject()->className();
    }

    //--------------------------------------------------------------
    explicit Error(const QObject *object, const Error::Status &status, const QString &code, const QString &message)
        : m_status(status), m_code(code), m_message(message)
    {
        m_className = object->metaObject()->className();
    }

    //--------------------------------------------------------------
    explicit Error(const QObject *object, const Error::Status &status, const QString &code, const QString &message,
                   const QString data)
        : m_status(status), m_code(code), m_message(message), m_data(data)
    {
        m_className = object->metaObject()->className();
    }

    //--------------------------------------------------------------
    explicit Error(const QString &className, const Error::Status &status, const QString &code)
        : m_className(className), m_status(status), m_code(code)
    {
    }

    //--------------------------------------------------------------
    explicit Error(const QString &className, const Error::Status &status, const QString &code, const QString &message)
        : m_className(className), m_status(status), m_code(code), m_message(message)
    {
    }

    //--------------------------------------------------------------
    explicit Error(const QString &className, const Error::Status &status, const QString &code, const QString &message,
                   const QString data)
        : m_className(className), m_status(status), m_code(code), m_message(message), m_data(data)
    {
    }

    //--------------------------------------------------------------
    explicit Error()
    {
        m_status = Status::Ok;
    }

    //--------------------------------------------------------------
    Error(const Error &other)
        : m_status(other.m_status), m_className(other.m_className), m_code(other.m_code), m_message(other.m_message),
          m_data(other.m_data)
    {
    }

    //--------------------------------------------------------------
    QString message() const
    {
        return m_message;
    }

    //--------------------------------------------------------------
    QString data() const
    {
        return m_data;
    }
    //--------------------------------------------------------------
    bool isNull() const
    {
        return m_status == Status::Ok;
    }

    //--------------------------------------------------------------
    Error::Status getStatus() const
    {
        return m_status;
    }

    //--------------------------------------------------------------
    void setStatus(const Error::Status &status)
    {
        m_status = status;
    }

    //--------------------------------------------------------------

  private:
    QString m_code;
    QString m_message;
    QString m_data;
    QString m_className;
    Error::Status m_status;
};

#endif // ERROR_H
