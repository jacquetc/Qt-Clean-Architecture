#include "result.h"

template <typename T> Result<T> &Result<T>::operator=(const Result &result)
{
    if (Q_LIKELY(&result != this))
    {
        m_status = result.m_status;
        m_value == result.m_value;
    }

    //    if (result.getStatus() == Error::Critical || result.getStatus() == Error::Fatal)
    //    {
    //        qFatal("");
    //    }

    return *this;
}

template <typename T> bool Result<T>::operator==(const Result &otherResult) const
{
    return m_status == otherResult.getStatus() && m_value == otherResult.m_value;
}

template <typename T> bool Result<T>::operator!=(const Result &otherResult) const
{
    return m_status != otherResult.getStatus() || m_value != otherResult.m_value;
}

template <typename T> Error::Status Result<T>::getStatus() const
{
    return m_status;
}

template <typename T> void Result<T>::setStatus(const Error::Status &status)
{
    m_status = status;
}
