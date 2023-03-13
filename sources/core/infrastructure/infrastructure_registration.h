#pragma once

#include "infrastructure_global.h"
#include <QObject>

class SKRINFRASTRUCTUREEXPORT InfrastructureRegistration : public QObject
{
    Q_OBJECT
  public:
    explicit InfrastructureRegistration(QObject *parent);

  signals:
  private:
};
