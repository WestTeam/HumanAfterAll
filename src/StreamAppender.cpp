// Copyright (c) 2018 All Rights Reserved WestBot

#include <QCoreApplication>
#include <QDateTime>
#include <QMessageLogContext>
#include <QString>
#include <QStringBuilder>

#include <WestBot/HumanAfterAll/StreamAppender.hpp>
#include <WestBot/HumanAfterAll/MessageLogger.hpp>

using namespace WestBot::HumanAfterAll::Logging;

namespace
{
    inline QString GetLogLevel( quint32 type )
    {
        switch( type )
        {
        case MessageLogger::DebugLevel:
            return QStringLiteral( " DEBUG:    " );
        case MessageLogger::InfoLevel:
            return QStringLiteral( " INFO:     " );
        case MessageLogger::WarningLevel:
            return QStringLiteral( " WARNING:  " );
        case MessageLogger::CriticalLevel:
            return QStringLiteral( " CRITICAL: " );
        case MessageLogger::FatalLevel:
            return QStringLiteral( " FATAL:    " );
        default:
            return QStringLiteral( " UNKNOWN:  " );
        }
    }

    const QString LOG_DATE_FORMAT = QStringLiteral( "HH:mm:ss.zzz" );
}

//
// Public methods
//

StreamAppender::StreamAppender( std::ostream& stream, QObject* parent )
    : Appender( parent )
    , _categoryWidth( 0 )
    , _stream( & stream )
{
}

StreamAppender::~StreamAppender()
{
}

void StreamAppender::logEvent(
    QtMsgType type,
    const QDateTime& timestamp,
    const QMessageLogContext& context,
    const QString& msg )
{
    // Update the category size if necessary...
    const QString category = QString::fromLatin1( context.category );
    if( Q_UNLIKELY( category.size() > _categoryWidth ) )
    {
        _categoryWidth = category.size();
    }

    // Generate the date string
    const QString date = timestamp.toString( LOG_DATE_FORMAT );

    // Create the log message using a QStringBuilder (much fast! wow!)
    const QString log =
        date % ' '
        % category.leftJustified( _categoryWidth )
        % GetLogLevel( type )
        % msg;

    // Print the result on stdout.
    *_stream << qUtf8Printable( log ) << std::endl;
}

//
// Protected methods
//

bool StreamAppender::setStream( std::ostream& stream )
{
    if( ! stream.good() )
    {
        return false;
    }
    _stream = & stream;
    return true;
}
