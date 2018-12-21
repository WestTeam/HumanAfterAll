// Copyright (c) 2018 All Rights Reserved WestBot

#include <assert.h>

#include <QCoreApplication>
#include <QDateTime>
#include <QEvent>
#include <QRegExp>
#include <QtDebug>
#include <QThread>
#include <QTimer>

#include <WestBot/HumanAfterAll/Appender.hpp>
#include <WestBot/HumanAfterAll/Category.hpp>
#include <WestBot/HumanAfterAll/Handler.hpp>

#include <Defines.hpp>
#include <HandlerPrivate.hpp>
#include <LogEvent.hpp>

using namespace WestBot::HumanAfterAll::Logging;

// Declared in Defines.hpp
Q_LOGGING_CATEGORY( LogCategory, "WestBot.HumanAfterAll.Logging" )

namespace
{
    DEFINE_STR(
        FILTER_RULE_REGEXP,
        "(debug|info|warning|critical):(.+)\\s*=\\s*(0|1|on|off|true|false)" );

    DEFINE_STR( DEBUG_LEVEL, "debug" );
    DEFINE_STR( INFO_LEVEL, "info" );
    DEFINE_STR( WARNING_LEVEL, "warning" );
    DEFINE_STR( CRITICAL_LEVEL, "critical" );

    DEFINE_STR( ONE_VALUE, "1" );
    DEFINE_STR( ON_VALUE, "on" );
    DEFINE_STR( TRUE_VALUE, "true" );

    template< typename T, int eventType >
    class CustomEvent : public QEvent
    {
    public:
        inline CustomEvent( T* value )
            : QEvent( static_cast< QEvent::Type >( eventType ) )
            , _value( value )
        {
        }

        inline T* value() const
        {
            return _value;
        }

    private:
        T* _value;
    };

    using AddCategoryEvent = CustomEvent< Category, AddCategoryEventType >;
    using RemCategoryEvent = CustomEvent< Category, RemCategoryEventType >;
    using AddAppenderEvent = CustomEvent< Appender, AddAppenderEventType >;
    using RemAppenderEvent = CustomEvent< Appender, RemAppenderEventType >;

    MessageLogger::LogLevel ParseLogLevel( const QString& level )
    {
        if( DEBUG_LEVEL == level )
        {
            return MessageLogger::DebugLevel;
        }

        if( INFO_LEVEL == level )
        {
            return MessageLogger::InfoLevel;
        }

        if( WARNING_LEVEL == level )
        {
            return MessageLogger::WarningLevel;
        }

        if( CRITICAL_LEVEL == level )
        {
            return MessageLogger::CriticalLevel;
        }

        return MessageLogger::MAX_LEVEL;
    }

    bool ParseRuleValue( const QString& value )
    {
        return
            ( ONE_VALUE == value ) ||
            ( ON_VALUE == value ) ||
            ( TRUE_VALUE == value );
    }

    void ApplyFilter(
        Category* cat,
        MessageLogger::LogLevel logLevel,
        const QRegExp& category,
        bool enable )
    {
        const QString categoryName = QString( cat->fullName() );
        if( category.exactMatch( categoryName ) )
        {
            cat->setEnabled( logLevel, enable );
        }
    }
}

HandlerPrivate::HandlerPrivate( Handler* handler, QCoreApplication* app )
    : QObject( handler )
    , _logHandler( handler )
    , _handler( nullptr )
    , _useShortNames( true )
{
    // Default debug levels...
#ifdef QT_NO_DEBUG
    _levels[ MessageLogger::DebugLevel ] = false;
#else
    _levels[ MessageLogger::DebugLevel ] = true;
#endif
    _levels[ MessageLogger::InfoLevel ] = true;
    _levels[ MessageLogger::WarningLevel ] = true;
    _levels[ MessageLogger::CriticalLevel ] = true;
    _levels[ MessageLogger::FatalLevel ] = true;

    if( QThread::currentThread() != app->thread() )
    {
        qFatal( "WestBot::HumanAfterAll::Logging::Handler "
                "MUST be created in the main() application thread" );
    }
    else if( ! _Instance.testAndSetAcquire( nullptr, this ) )
    {
        qFatal( "There can only be one WestBot::HumanAfterAll::Logging::Handler "
                "instance" );
    }
}

HandlerPrivate::~HandlerPrivate()
{
    uninstallHandler();
    bool clean = _Instance.testAndSetAcquire( this, nullptr );
    Q_ASSERT( clean );
    Q_UNUSED( clean );
}

void HandlerPrivate::addAppender( Appender* appender )
{
    if( QThread::currentThread() == thread() )
    {
        AddAppenderEvent event( appender );
        QCoreApplication::sendEvent( this, & event );
    }
    else
    {
        auto* event = new AddAppenderEvent( appender );
        QCoreApplication::postEvent( this, event );
    }
}

void HandlerPrivate::removeAppender( Appender* appender )
{
    if( QThread::currentThread() == thread() )
    {
        RemAppenderEvent event( appender );
        QCoreApplication::sendEvent( this, & event );
    }
    else
    {
        auto* event = new RemAppenderEvent( appender );
        QCoreApplication::postEvent( this, event );
    }
}

void HandlerPrivate::setFilterRules( const QStringList& rules )
{
    // Clear the rules...
    _filterRules.clear();

    QRegExp regexp( FILTER_RULE_REGEXP, Qt::CaseInsensitive );

    for( const QString& str : rules )
    {
        const QString rule = str.toLower();

        if( ! regexp.exactMatch( rule ) )
        {
            qCWarning(
                LogCategory(),
                "Invalid filter rule syntax: %s",
                qPrintable( rule ) );
            continue;
        }

        Q_ASSERT( regexp.captureCount() == 3 );

        const QString level = regexp.cap( 1 );
        const QRegExp category =
            QRegExp( regexp.cap( 2 ), Qt::CaseInsensitive, QRegExp::Wildcard );
        const QString value = regexp.cap( 3 );

        MessageLogger::LogLevel logLevel = ParseLogLevel( level );
        if( MessageLogger::MAX_LEVEL == logLevel )
        {
            qCWarning(
                LogCategory(),
                "Invalid log level in rule: %s",
                qPrintable( rule ) );
            continue;
        }

        bool enable = ParseRuleValue( value );

        // Store the valid filter rule
        _filterRules.push_back( rule );

        // Store the filter
        _filters.push_back( { logLevel, category, enable } );

        for( Category* cat : _categories )
        {
            ApplyFilter( cat, logLevel, category, enable );
        }
    }
}

bool HandlerPrivate::globalLoggingLevel( MessageLogger::LogLevel level ) const
{
    return _levels[ level ];
}

void HandlerPrivate::setGlobalLoggingLevel(
    MessageLogger::LogLevel level,
    bool enable )
{
    _levels[ level ] = enable;

    for( auto category : _categories )
    {
        category->setEnabled( level, enable );

        for( const Filter& filter : _filters )
        {
            ApplyFilter(
                category,
                filter.level,
                filter.category,
                filter.enabled );
        }
    }
}

bool HandlerPrivate::isUseShortNamesEnabled() const
{
    return _useShortNames.load( std::memory_order_relaxed );
}

void HandlerPrivate::setUseShortNamesEnabled( bool enable )
{
    _useShortNames.store( enable, std::memory_order_relaxed );
}

void HandlerPrivate::RegisterCategory( Category* category )
{
    HandlerPrivate* instance = _Instance.load();

    if( nullptr == instance )
    {
        qCWarning(
            LogCategory(),
            "Can't register logging category: %s without a "
                "WestBot::HumanAfterAll::Logging::Handler instance.",
            qPrintable( category->name() ) );
        return;
    }

    if( QThread::currentThread() == instance->thread() )
    {
        AddCategoryEvent event( category );
        QCoreApplication::sendEvent( instance, & event );
    }
    else
    {
        auto* event = new AddCategoryEvent( category );
        QCoreApplication::postEvent( instance, event );
    }
}

void HandlerPrivate::UnregisterCategory( Category* category )
{
    HandlerPrivate* instance = _Instance.load();

    if( nullptr == instance )
    {
        return;
    }

    if( QThread::currentThread() == instance->thread() )
    {
        RemCategoryEvent event( category );
        QCoreApplication::sendEvent( instance, & event );
    }
    else
    {
        auto* event = new RemCategoryEvent( category );
        QCoreApplication::postEvent( instance, event );
    }
}

bool HandlerPrivate::UseShortNames()
{
    HandlerPrivate* instance = _Instance.load();

    if( nullptr == instance )
    {
        return false;
    }

    return instance->isUseShortNamesEnabled();
}

void HandlerPrivate::customEvent( QEvent* event )
{
    LogEvent* logEvent = nullptr;
    AddCategoryEvent* addCategoryEvent = nullptr;
    RemCategoryEvent* remCategoryEvent = nullptr;
    AddAppenderEvent* addAppenderEvent = nullptr;
    RemAppenderEvent* remAppenderEvent = nullptr;

    switch( static_cast< int >( event->type() ) )
    {
    case LogEventType:
        logEvent = static_cast< LogEvent* >( event );
        // Push the message to all appenders
        for( Appender* appender : _appenders )
        {
            if( QThread::currentThread() == appender->thread() )
            {
                QCoreApplication::sendEvent( appender, logEvent );
            }
            else
            {
                auto* le = new LogEvent( *logEvent );
                QCoreApplication::postEvent( appender, le );
            }
        }
        break;

    case AddCategoryEventType:
    {
        addCategoryEvent = static_cast< AddCategoryEvent* >( event );
        addCategoryEvent->accept();

        auto category = addCategoryEvent->value();
        _categories.push_back( category );

        for( uint i = 0; i < MessageLogger::MAX_LEVEL; ++i )
        {
            const auto level = static_cast< MessageLogger::LogLevel >( i );
            category->setEnabled( level, _levels[ level ] );
        }

        for( const Filter& filter : _filters )
        {
            ApplyFilter(
                category,
                filter.level,
                filter.category,
                filter.enabled );
        }
        break;
    }
    case RemCategoryEventType:
        remCategoryEvent = static_cast< RemCategoryEvent* >( event );
        remCategoryEvent->accept();
        _categories.removeOne( remCategoryEvent->value() );
        break;

    case AddAppenderEventType:
        addAppenderEvent = static_cast< AddAppenderEvent* >( event );
        if( _appenders.empty() )
        {
            // First appender, install the handler
            installHandler();
        }
        _appenders.push_back( addAppenderEvent->value() );
        break;

    case RemAppenderEventType:
        remAppenderEvent = static_cast< RemAppenderEvent* >( event );
        _appenders.removeOne( remAppenderEvent->value() );
        if( _appenders.empty() )
        {
            // No more appenders, uninstall the handler
            uninstallHandler();
        }
        break;
    }
}

void HandlerPrivate::installHandler()
{
    // Install the event handler
    if( _handler != MessageHandler )
    {
        // Save the previous handler
        _handler = qInstallMessageHandler( & HandlerPrivate::MessageHandler );
    }
}

void HandlerPrivate::uninstallHandler()
{
    // Restore the previous handler
    if( _handler != MessageHandler )
    {
        qInstallMessageHandler( _handler );
    }
}

void HandlerPrivate::MessageHandler(
    QtMsgType type,
    const QMessageLogContext& context,
    const QString& message )
{
    HandlerPrivate* instance = _Instance.load();

    if( QThread::currentThread() == instance->thread() )
    {
        LogEvent le( type, context, message );
        QCoreApplication::sendEvent( instance, & le );
    }
    else
    {
        auto* le = new LogEvent( type, context, message );
        QCoreApplication::postEvent( instance, le );
    }
}

QAtomicPointer< HandlerPrivate > HandlerPrivate::_Instance = nullptr;
