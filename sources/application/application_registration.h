#ifndef APPLICATIONREGISTRATION_H
#define APPLICATIONREGISTRATION_H

#include "application_global.h"
#include <QObject>

class SKRAPPLICATIONEXPORT ApplicationRegistration : public QObject
{
    Q_OBJECT
  public:
    explicit ApplicationRegistration(QObject *parent = nullptr);

  signals:
  private:
};

#endif // APPLICATIONREGISTRATION_H
