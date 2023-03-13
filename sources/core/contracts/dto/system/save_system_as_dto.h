#pragma once

#include "contracts_global.h"
#include <QObject>
#include <QUrl>

namespace Contracts::DTO::System
{
class SKRCONTRACTSEXPORT SaveSystemAsDTO
{
    Q_GADGET

    Q_PROPERTY(QUrl fileName MEMBER m_fileName)
  public:
    QUrl fileName() const
    {
        return m_fileName;
    }

    void setFileName(const QUrl &fileName)
    {
        m_fileName = fileName;
    }

  private:
    QUrl m_fileName;
};
} // namespace Contracts::DTO::System
