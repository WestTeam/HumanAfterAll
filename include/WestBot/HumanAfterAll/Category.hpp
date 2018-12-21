// Copyright (c) 2018 All Rights Reserved WestBot

#ifndef WESTBOT_HUMANAFTERALL_LOGGING_CATEGORY_HPP_
#define WESTBOT_HUMANAFTERALL_LOGGING_CATEGORY_HPP_

#include <QByteArray>

#include "MessageLogger.hpp"

namespace WestBot {
namespace HumanAfterAll {
namespace Logging {

class Category
{
public:
    Category( const char* name );
    ~Category();

    Category &operator()()
    {
        return *this;
    }

    const char* fullName() const;

    const char* name() const;

    void enableDebug( bool on );
    void enableInfo( bool on );
    void enableWarning( bool on );
    void enableCritical( bool on );

    inline void setEnabled( MessageLogger::LogLevel level, bool enable )
    {
        _levels[ level ] = enable;
    }

    inline bool isEnabled( MessageLogger::LogLevel level ) const
    {
        return _levels[ level ];
    }

private:
    const char* _name;
    const QByteArray _shortName;
    bool _levels[ MessageLogger::MAX_LEVEL ];
};

}
}
}

#define HUMANAFTERALL_LOGGING_DECLARE_LOGGING_CATEGORY( name ) \
    extern WestBot::HumanAfterAll::Logging::Category& name();

#define HUMANAFTERALL_LOGGING_CATEGORY( name, string ) \
    WestBot::HumanAfterAll::Logging::Category& name() \
    { \
        static WestBot::HumanAfterAll::Logging::Category category( string ); \
        return category; \
    }

#define tLogLevel( level ) \
    WestBot::HumanAfterAll::Logging::MessageLogger::level

#define tLogEnabled( category, level ) \
    category().isEnabled( tLogLevel( level ) )

#define tLogStream( category, level ) \
    WestBot::HumanAfterAll::Logging::MessageLogger( \
        __FILE__, \
        __LINE__, \
        Q_FUNC_INFO, \
        category().name(), \
        tLogLevel( level ) ).stream()

#define tLog( category, level ) \
    for( bool on = tLogEnabled( category, level ); on; on = false ) \
        tLogStream( category, level )

#define tDebug( category )      tLog( category, DebugLevel )
#define tInfo( category )       tLog( category, InfoLevel )
#define tWarning( category )    tLog( category, WarningLevel )
#define tCritical( category )   tLog( category, CriticalLevel )
#define tFatal( category )      tLog( category, FatalLevel )

#endif // WESTBOT_HUMANAFTERALL_LOGGING_CATEGORY_HPP_
