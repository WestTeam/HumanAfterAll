// Copyright (c) 2018 All Rights Reserved WestBot

#ifndef WESTBOT_HUMANAFTERALL_LOGGING_DEFINES_HPP_
#define WESTBOT_HUMANAFTERALL_LOGGING_DEFINES_HPP_

#include <QEvent>
#include <QString>

#include <WestBot/HumanAfterAll/Category.hpp>

#define DEFINE_STR( name, value ) const QString name = QStringLiteral( value )

namespace WestBot {
namespace HumanAfterAll {
namespace Logging {

enum HumanAfterAllEvents
{
    LogEventType = QEvent::User,
    AddCategoryEventType,
    RemCategoryEventType,
    AddAppenderEventType,
    RemAppenderEventType
};

}
}
}

Q_DECLARE_LOGGING_CATEGORY( LogCategory ) // Defined in HandlerPrivate.cpp

#endif // WESTBOT_HUMANAFTERALL_LOGGING_DEFINES_HPP_
