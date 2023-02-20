#pragma once
#include "error.h"
#include <QMetaType>
#include <QString>

template <typename T> class Result
{

  public:
    explicit Result(const T &value) : m_value(value), m_error(Error())
    {
        m_error.setStatus(Error::Ok);
    }

    explicit Result(const Error &error) : m_error(error)
    {
    }

    explicit Result() : m_error(Error())
    {
    }

    bool operator!() const
    {
        return !isOk();
    }
    operator bool() const
    {
        return isOk();
    }
    Q_INVOKABLE Result &operator=(const Result &result)
    {
        if (Q_LIKELY(&result != this))
        {
            m_status = result.m_status;
            m_value = result.m_value;
        }

        //    if (result.getStatus() == Error::Critical || result.getStatus() == Error::Fatal)
        //    {
        //        qFatal("");
        //    }

        return *this;
    }
    bool operator==(const Result &otherResult) const
    {
        return m_status == otherResult.getStatus() && m_value == otherResult.m_value;
    }
    bool operator!=(const Result &otherResult) const
    {
        return m_status != otherResult.getStatus() || m_value != otherResult.m_value;
    }

    bool isOk() const
    {
        return m_error.isOk();
    }
    bool isSuccess() const
    {
        return m_error.isOk();
    }

    bool isError() const
    {
        return !m_error.isOk();
    }

    bool hasError() const
    {
        return !m_error.isOk();
    }

    bool isEmpty() const
    {
        return m_error.isEmpty();
    }

    T value() const
    {
        if (!isOk())
        {
            qCritical("Result in error while calling value()");
            throw m_error;
        }
        return m_value;
    }

    Error error() const
    {
        return m_error;
    }
    Error::Status getStatus() const
    {
        return m_status;
    }
    void setStatus(const Error::Status &status)
    {
        m_status = status;
    }

  private:
    T m_value;
    Error m_error;
    Error::Status m_status;
};
