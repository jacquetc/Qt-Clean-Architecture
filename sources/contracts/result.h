#ifndef RESULT_H
#define RESULT_H

#include "error.h"
#include <QMetaType>
#include <QString>

template <typename T> class Result
{

  public:
    explicit Result(const T &value) : m_value(value), m_error(Error())
    {
    }

    explicit Result(const Error &error) : m_error(error)
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
    Q_INVOKABLE Result &operator=(const Result &result);
    bool operator==(const Result &otherResult) const;
    bool operator!=(const Result &otherResult) const;

    bool isOk() const
    {
        return m_error.isNull();
    }
    bool isSuccess() const
    {
        return m_error.isNull();
    }

    bool isError() const
    {
        return !m_error.isNull();
    }

    bool hasError() const
    {
        return !m_error.isNull();
    }

    T value() const
    {
        if (!isOk())
        {
            throw m_error;
        }
        return m_value;
    }

    Error error() const
    {
        return m_error;
    }
    Error::Status getStatus() const;
    void setStatus(const Error::Status &status);

  private:
    T m_value;
    Error m_error;
    Error::Status m_status;
};

#endif // RESULT_H
