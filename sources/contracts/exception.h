#pragma once
#include <QException>

namespace Contracts
{
class Exception : public QException
{
  public:
    void raise() const override
    {
        throw *this;
    }

    Exception *clone() const override
    {
        return new Exception(*this);
    }
};
} // namespace Contracts
