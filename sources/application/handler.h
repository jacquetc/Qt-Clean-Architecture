#pragma once

#include "wait_in_event_loop.h"
#include <QObject>

class Handler : public QObject, public virtual WaitInEventLoop
{
    Q_OBJECT
  public:
    explicit Handler(QObject *parent = nullptr);

  signals:
    void progressChanged(int minimum, int maximum, int progress);
};
