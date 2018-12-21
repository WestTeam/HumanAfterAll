// Copyright (c) 2018 All Rights Reserved WestBot

#ifndef WESTBOT_HUMANAFTERALL_LOGGING_HANDLER_PRIVATE_HPP_
#define WESTBOT_HUMANAFTERALL_LOGGING_HANDLER_PRIVATE_HPP_

#include <atomic>

#include <QAtomicPointer>
#include <QByteArray>
#include <QCoreApplication>
#include <QList>
#include <QLoggingCategory>
#include <QMultiHash>
#include <QObject>
#include <QRegExp>

#include <WestBot/HumanAfterAll/MessageLogger.hpp>

namespace WestBot {
namespace HumanAfterAll {
namespace Logging {

class Appender;
class Category;
class Handler;

class HandlerPrivate : public QObject
{
    friend class Handler;

public:
    HandlerPrivate( Handler* handler, QCoreApplication* app );
    ~HandlerPrivate() override;

    void addAppender( Appender* appender );
    void removeAppender( Appender* appender );

    void setFilterRules( const QStringList& rules );

    bool globalLoggingLevel( MessageLogger::LogLevel level ) const;
    void setGlobalLoggingLevel( MessageLogger::LogLevel level, bool enable );

    bool isUseShortNamesEnabled() const;
    void setUseShortNamesEnabled( bool enable );

    static void RegisterCategory( Category* category );
    static void UnregisterCategory( Category* category );

    static bool UseShortNames();

protected:
    void customEvent( QEvent* e ) override;

private:
    void installHandler();
    void uninstallHandler();
    static void MessageHandler(
        QtMsgType type,
        const QMessageLogContext& context,
        const QString& message );

private:
    Handler* _logHandler;
    QtMessageHandler _handler;

    QList< Appender* > _appenders;

    QList< Category* > _categories;

    struct Filter
    {
        MessageLogger::LogLevel level;
        QRegExp category;
        bool enabled;
    };
    QVector< Filter > _filters;
    QStringList _filterRules;

    bool _levels[ MessageLogger::MAX_LEVEL ];

    std::atomic< bool > _useShortNames;

    static QAtomicPointer< HandlerPrivate > _Instance;
};

}
}
}

#endif // WESTBOT_HUMANAFTERALL_LOGGING_HANDLER_PRIVATE_HPP_
