// Copyright (c) 2018 All Rights Reserved WestBot

#ifndef WESTBOT_HUMANAFTERALL_LOGGING_LOGEVENT_HPP_
#define WESTBOT_HUMANAFTERALL_LOGGING_LOGEVENT_HPP_

#include <QDateTime>
#include <QMessageLogContext>
#include <QtDebug>
#include <QString>

#include "Defines.hpp"

namespace WestBot {
namespace HumanAfterAll {
namespace Logging {

class LogEvent : public QEvent
{
public:
    LogEvent(
        QtMsgType type,
        const QMessageLogContext& ctx,
        const QString& message )
        : QEvent( static_cast< QEvent::Type >( LogEventType ) )
        , _timestamp( QDateTime::currentDateTimeUtc() )
        , _context( ctx.file, ctx.line, ctx.function, ctx.category )
        , _message( message )
        , _type( type )
    {
    }

    LogEvent( const LogEvent& event )
        : QEvent( static_cast< QEvent::Type >( LogEventType ) )
        , _timestamp( event._timestamp )
        , _context(
            event._context.file,
            event._context.line,
            event._context.function,
            event._context.category )
        , _message( event._message )
        , _type( event._type )
    {
    }

public:
    const QDateTime _timestamp;
    const QMessageLogContext _context;
    const QString _message;
    const QtMsgType _type;
};

}
}
}

#endif // WESTBOT_HUMANAFTERALL_LOGGING_LOGEVENT_HPP_
