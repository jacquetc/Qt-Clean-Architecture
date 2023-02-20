#ifndef HANDLER_H
#define HANDLER_H

#include "wait_in_event_loop.h"
#include <QObject>

class Handler : public QObject, public WaitInEventLoop
{
    Q_OBJECT
  public:
    explicit Handler(QObject *parent = nullptr);

  signals:
    void progressChanged(int minimum, int maximum, int progress);
};

#endif // HANDLER_H
