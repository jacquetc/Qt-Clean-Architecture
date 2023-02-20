#pragma once

#include "result.h"
#include <QEventLoop>
#include <QFuture>
#include <QFutureWatcher>

class WaitInEventLoop
{

  public:
    template <class U> auto waitInEventLoop(QFuture<Result<U>> &&future)
    {
        QEventLoop wait;
        QFutureWatcher<Result<U>> fw;

        QObject::connect(&fw, &QFutureWatcher<Result<U>>::finished, &wait, &QEventLoop::quit);

        // Connect progressRangeChanged signal to a lambda function that emits a signal with progress info
        QObject::connect(&fw, &QFutureWatcher<Result<U>>::progressRangeChanged, [this](int minimum, int maximum) {
            // emit a signal with progress info
            emit progressChanged(minimum, maximum, 0);
        });

        // Connect progressValueChanged signal to a lambda function that emits a signal with progress info
        QObject::connect(&fw, &QFutureWatcher<Result<U>>::progressValueChanged, [this](int progressValue) {
            // emit a signal with progress info
            emit progressChanged(0, 0, progressValue);
        });

        fw.setFuture(future);
        wait.exec();

        // Disconnect the signals after the event loop has finished
        QObject::disconnect(&fw, &QFutureWatcher<Result<U>>::finished, &wait, &QEventLoop::quit);
        QObject::disconnect(&fw, &QFutureWatcher<Result<U>>::progressRangeChanged, nullptr, nullptr);
        QObject::disconnect(&fw, &QFutureWatcher<Result<U>>::progressValueChanged, nullptr, nullptr);

        return fw.result();
    }

  signals:
    virtual void progressChanged(int minimum, int maximum, int value) = 0;
};
