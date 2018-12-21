// Copyright (c) 2018 All Rights Reserved WestBot

#include <cstdlib>
#include <iostream>

#include <QCoreApplication>
#include <QStandardPaths>
#include <QStringBuilder>

#include <WestBot/HumanAfterAll/FileAppender.hpp>

using namespace WestBot::HumanAfterAll::Logging;

//
// Public methods
//

FileAppender::FileAppender( QObject* parent )
    : StreamAppender( std::clog, parent )
{
    const QString fileName =
        qApp->applicationName().toLower() % "." %
        QString::number( qApp->applicationPid() ) %
        ".log";

    init( fileName );
}

FileAppender::FileAppender( const QString& fileName, QObject* parent )
    : StreamAppender( std::clog, parent )
{
    init( fileName );
}

//
// Private methods
//

void FileAppender::init( const QString& fileName )
{
    _file.open(
        qPrintable( fileName ),
        std::ofstream::out | std::ofstream::trunc );

    if( ! StreamAppender::setStream( _file ) )
    {
        Q_ASSERT( ! "can't initialize stream appender" );
        abort();
    }
}
