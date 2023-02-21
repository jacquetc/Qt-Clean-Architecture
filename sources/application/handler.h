#pragma once

#include "application_global.h"
#include "wait_in_event_loop.h"
#include <QObject>

namespace Application
{
class SKRAPPLICATIONEXPORT Handler : public QObject, public virtual WaitInEventLoop
{
    Q_OBJECT
  public:
    explicit Handler(QObject *parent = nullptr);

  signals:
    void progressChanged(int minimum, int maximum, int progress);
};
} // namespace Application
