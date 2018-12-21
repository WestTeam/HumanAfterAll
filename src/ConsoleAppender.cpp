// Copyright (c) 2018 All Rights Reserved WestBot

#include <iostream>

#include <WestBot/HumanAfterAll/ConsoleAppender.hpp>

using namespace WestBot::HumanAfterAll::Logging;

ConsoleAppender::ConsoleAppender( QObject* parent )
    : StreamAppender( std::cout, parent )
{
}
