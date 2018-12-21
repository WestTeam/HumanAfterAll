# Copyright (c) 2018 All Rights Reserved WestBot

QT += core
QT -= gui

TARGET = HumanAfterAll

TEMPLATE = lib

INCLUDEPATH += include

SOURCES += src/Appender.cpp \
           src/Category.cpp \
           src/ConsoleAppender.cpp \
           src/FileAppender.cpp \
           src/Handler.cpp \
	       src/HandlerPrivate.cpp \
	       src/StreamAppender.cpp

HEADERS += include/WestBot/HumanAfterAll/Appender.hpp \
           include/WestBot/HumanAfterAll/Category.hpp \
           include/WestBot/HumanAfterAll/ConsoleAppender.hpp \
           include/WestBot/HumanAfterAll/FileAppender.hpp \
           include/WestBot/HumanAfterAll/Handler.hpp \
           include/WestBot/HumanAfterAll/MessageLogger.hpp \
           include/WestBot/HumanAfterAll/StreamAppender.hpp \
           include/Defines.hpp \
           include/HandlerPrivate.hpp \
           include/LogEvent.hpp


DEFINES += WESTBOT_HUMANAFTERALLSHAREDLIB_LIBRARY
