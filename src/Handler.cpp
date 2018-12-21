// Copyright (c) 2018 All Rights Reserved WestBot

#include <QCoreApplication>
#include <QtDebug>
#include <QThread>

#include <WestBot/HumanAfterAll/Handler.hpp>

#include <HandlerPrivate.hpp>

using namespace WestBot::HumanAfterAll::Logging;

Handler::Handler( QCoreApplication* app )
    : QObject( app )
    , _dptr( new HandlerPrivate( this, app ) )
{
}

void Handler::addAppender( Appender* appender )
{
    _dptr->addAppender( appender );
}

void Handler::removeAppender( Appender* appender )
{
    _dptr->removeAppender( appender );
}

const QList< Appender* >& Handler::appenders() const
{
    return _dptr->_appenders;
}

const QList< Category* >& Handler::categories() const
{
    return _dptr->_categories;
}

const QStringList& Handler::filterRules() const
{
    return _dptr->_filterRules;
}

void Handler::setFilterRules( const QStringList& rules )
{
    _dptr->setFilterRules( rules );
}

bool Handler::criticalLevelIsEnabled() const
{
    return _dptr->globalLoggingLevel( MessageLogger::CriticalLevel );
}

void Handler::setEnableCriticalLevel( bool enabled )
{
    _dptr->setGlobalLoggingLevel( MessageLogger::CriticalLevel, enabled );
}

bool Handler::debugLevelIsEnabled() const
{
    return _dptr->globalLoggingLevel( MessageLogger::DebugLevel );
}

void Handler::setEnableDebugLevel( bool enabled )
{
    _dptr->setGlobalLoggingLevel( MessageLogger::DebugLevel, enabled );
}

bool Handler::infoLevelIsEnabled() const
{
    return _dptr->globalLoggingLevel( MessageLogger::InfoLevel );
}

void Handler::setEnableInfoLevel( bool enabled )
{
    _dptr->setGlobalLoggingLevel( MessageLogger::InfoLevel, enabled );
}

bool Handler::warningLevelIsEnabled() const
{
    return _dptr->globalLoggingLevel( MessageLogger::WarningLevel );
}

void Handler::setEnableWarningLevel( bool enabled )
{
    _dptr->setGlobalLoggingLevel( MessageLogger::WarningLevel, enabled );
}

bool Handler::isUseShortNamesEnabled() const
{
    return _dptr->isUseShortNamesEnabled();
}

void Handler::setUseShortNamesEnabled( bool enable )
{
    return _dptr->setUseShortNamesEnabled( enable );
}
