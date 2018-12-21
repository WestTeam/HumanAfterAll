// Copyright (c) 2018 All Rights Reserved WestBot

#ifndef WESTBOT_HUMANAFTERALL_LOGGING_APPENDER_HPP_
#define WESTBOT_HUMANAFTERALL_LOGGING_APPENDER_HPP_

#include <QMessageLogContext>
#include <QObject>

class QDateTime;
class QString;

namespace WestBot {
namespace HumanAfterAll {
namespace Logging {

class Appender : public QObject
{
public:
    Appender( QObject* parent = nullptr );

    virtual void logEvent(
        QtMsgType type,
        const QDateTime& timestamp,
        const QMessageLogContext& context,
        const QString& message ) = 0;

protected:
    void customEvent( QEvent* e ) override;
};

}
}
}

#endif // WESTBOT_HUMANAFTERALL_LOGGING_APPENDER_HPP_
