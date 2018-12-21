// Copyright (c) 2018 All Rights Reserved WestBot

#ifndef WESTBOT_HUMANAFTERALL_LOGGING_STREAMAPPENDER_HPP_
#define WESTBOT_HUMANAFTERALL_LOGGING_STREAMAPPENDER_HPP_

#include <iostream>

#include "Appender.hpp"

namespace WestBot {
namespace HumanAfterAll {
namespace Logging {

class StreamAppender : public Appender
{
public:
    StreamAppender( std::ostream& stream, QObject* parent = nullptr );
    virtual ~StreamAppender();

    void logEvent(
        QtMsgType type,
        const QDateTime& timestamp,
        const QMessageLogContext& context,
        const QString& message ) override;

protected:
    bool setStream( std::ostream& stream );

private:
    int _categoryWidth;
    std::ostream* _stream;
};

}
}
}

#endif // WESTBOT_HUMANAFTERALL_LOGGING_STREAMAPPENDER_HPP_
