// Copyright (c) 2018 All Rights Reserved WestBot

#ifndef WESTBOT_HUMANAFTERALL_LOGGING_CONSOLEAPPENDER_HPP_
#define WESTBOT_HUMANAFTERALL_LOGGING_CONSOLEAPPENDER_HPP_

#include "StreamAppender.hpp"

namespace WestBot {
namespace HumanAfterAll {
namespace Logging {

class ConsoleAppender : public StreamAppender
{
public:
    ConsoleAppender( QObject* parent = nullptr );
};

}
}
}

#endif // WESTBOT_HUMANAFTERALL_LOGGING_CONSOLEAPPENDER_HPP_
