// Copyright (c) 2018 All Rights Reserved WestBot

#ifndef WESTBOT_HUMANAFTERALL_LOGGING_HANDLER_HPP_
#define WESTBOT_HUMANAFTERALL_LOGGING_HANDLER_HPP_

#include <QCoreApplication>
#include <QLoggingCategory>
#include <QObject>

namespace WestBot {
namespace HumanAfterAll {
namespace Logging {

class Appender;
class HandlerPrivate;
class Category;

/*
 * The Handler installs itself as the Qt logging backend. It is completely
 * thread-safe. However, it must be installed before any thread actually tries
 * to log data.
 */
class Handler : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QStringList filterRules READ filterRules WRITE setFilterRules )

public:
    Handler( QCoreApplication* app );

    void addAppender( Appender* appender );

    void removeAppender( Appender* appender );

    const QStringList& filterRules() const;
    void setFilterRules( const QStringList& rules );

    const QList< Appender* >& appenders() const;

    const QList< Category* >& categories() const;

    bool criticalLevelIsEnabled() const;
    void setEnableCriticalLevel( bool enabled );

    bool debugLevelIsEnabled() const;
    void setEnableDebugLevel( bool enabled );

    bool infoLevelIsEnabled() const;
    void setEnableInfoLevel( bool enabled );

    bool warningLevelIsEnabled() const;
    void setEnableWarningLevel( bool enabled );

    bool isUseShortNamesEnabled() const;
    void setUseShortNamesEnabled( bool enable );

private:
    HandlerPrivate* _dptr;
};

}
}
}

#endif // WESTBOT_HUMANAFTERALL_LOGGING_HANDLER_HPP_
