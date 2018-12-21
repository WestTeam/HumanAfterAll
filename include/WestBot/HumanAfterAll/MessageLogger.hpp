// Copyright (c) 2018 All Rights Reserved WestBot

#ifndef WESTBOT_HUMANAFTERALL_LOGGING_LOGGING_MESSAGELOGGER_HPP_
#define WESTBOT_HUMANAFTERALL_LOGGING_LOGGING_MESSAGELOGGER_HPP_

#include <QDebug>
#include <QLoggingCategory>
#include <QMessageLogContext>
#include <QtGlobal>

namespace WestBot {
namespace HumanAfterAll {
namespace Logging {

class MessageLogger
{
public:
    enum LogLevel
    {
        DebugLevel = QtDebugMsg,
        WarningLevel = QtWarningMsg,
        CriticalLevel = QtCriticalMsg,
        FatalLevel = QtFatalMsg,
        InfoLevel = QtFatalMsg + 1,
        MAX_LEVEL
    };

public:
    inline MessageLogger(
        const char* fileName,
        int fileLine,
        const char* functionName,
        const char* categoryName,
        LogLevel level )
        : _stream( & _message )
        , _context( fileName, fileLine, functionName, categoryName )
        , _level( level )
    {
    }

    inline MessageLogger(
        const char* categoryName,
        LogLevel level )
        : _stream( & _message )
        , _context( nullptr, 0, nullptr, categoryName )
        , _level( level )
    {
    }

    inline ~MessageLogger()
    {
        // Push to Qt log handling infrastructure
        qt_message_output(
            static_cast< QtMsgType >( _level ),
            _context,
            _message );
    }

    inline QDebug& stream()
    {
        return _stream;
    }

private:
    QString _message;
    QDebug _stream;
    QMessageLogContext _context;
    LogLevel _level;
};

}
}
}

#endif // WESTBOT_HUMANAFTERALL_LOGGING_LOGGING_MESSAGELOGGER_HPP_
