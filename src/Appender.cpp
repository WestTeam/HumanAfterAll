// Copyright (c) 2018 All Rights Reserved WestBot

#include <WestBot/HumanAfterAll/Appender.hpp>

#include <LogEvent.hpp>

using namespace WestBot::HumanAfterAll::Logging;

Appender::Appender( QObject* parent )
    : QObject( parent )
{
}

void Appender::customEvent( QEvent* e )
{
    LogEvent* event = static_cast< LogEvent* >( e );
    logEvent(
        event->_type,
        event->_timestamp,
        event->_context,
        event->_message );
}
