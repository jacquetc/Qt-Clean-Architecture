#pragma once

#include "presenter_global.h"
#include "result.h"

#include <QObject>

namespace Presenter::System::Private
{

class SKR_PRESENTER_EXPORT SystemSignalBridge : public QObject
{
    Q_OBJECT
  public:
    SystemSignalBridge(QObject *parent) : QObject(parent)
    {
    }

  signals:

    void systemLoaded(Result<void> result);
    void systemSaved(Result<void> result);
};
} // namespace Presenter::System::Private
