#pragma once

#include "dto/system/load_system_dto.h"
#include <QObject>

using namespace Contracts::DTO::System;

class SkribLoader : public QObject
{
    Q_OBJECT
  public:
    explicit SkribLoader(QObject *parent = nullptr);

    void load(const LoadSystemDTO &dto);

  signals:
};
