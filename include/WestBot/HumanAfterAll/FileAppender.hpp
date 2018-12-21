// Copyright (c) 2018 All Rights Reserved WestBot

#ifndef WESTBOT_HUMANAFTERALL_LOGGING_FILEAPPENDER_HPP_
#define WESTBOT_HUMANAFTERALL_LOGGING_FILEAPPENDER_HPP_

#include <fstream>

#include "StreamAppender.hpp"

// Forward declaration
class QString;

namespace WestBot {
namespace HumanAfterAll {
namespace Logging {

class FileAppender : public StreamAppender
{
public:
    FileAppender( QObject* parent = nullptr );
    FileAppender( const QString& fileName, QObject* parent = nullptr );

private:
    void init( const QString& fileName );

    std::ofstream _file;
};

}
}
}

#endif // WESTBOT_HUMANAFTERALL_LOGGING_FILEAPPENDER_HPP_
