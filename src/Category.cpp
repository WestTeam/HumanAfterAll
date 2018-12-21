// Copyright (c) 2018 All Rights Reserved WestBot

#include <QChar>

#include <WestBot/HumanAfterAll/Category.hpp>

#include <HandlerPrivate.hpp>

using namespace WestBot::HumanAfterAll::Logging;

namespace
{
    QByteArray GetShortName( const char* name )
    {
        QByteArray result;

        result.reserve( 32 );

        int shortenedSections = 2; // Because!

        for( const char* c = name; '\0' != *c; ++c )
        {
            const QChar qc( *c );

            if( qc.isPunct() && 0 != shortenedSections )
            {
                shortenedSections--;
            }
            else if( 0 != shortenedSections && ! qc.isUpper() )
            {
                continue;
            }

            result.append( qc.toLatin1() );
        }

        result.squeeze();

        return result;
    }
}

Category::Category( const char* name )
    : _name( name )
    , _shortName( GetShortName( name ) )
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

    HandlerPrivate::RegisterCategory( this );
}

Category::~Category()
{
    HandlerPrivate::UnregisterCategory( this );
}

const char* Category::fullName() const
{
    return _name;
}

const char* Category::name() const
{
    return HandlerPrivate::UseShortNames() ? _shortName.constData() : _name;
}

void Category::enableDebug( bool on )
{
    _levels[ MessageLogger::DebugLevel ] = on;
}

void Category::enableInfo( bool on )
{
    _levels[ MessageLogger::InfoLevel ] = on;
}

void Category::enableWarning( bool on )
{
    _levels[ MessageLogger::WarningLevel ] = on;
}

void Category::enableCritical( bool on )
{
    _levels[ MessageLogger::CriticalLevel ] = on;
}
