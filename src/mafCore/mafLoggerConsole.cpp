/*
 *  mafLoggerConsole.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 17/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLoggerConsole.h"
#include <iostream>

using namespace mafCore;

mafLoggerConsole::mafLoggerConsole(const QString code_location) : mafLogger(code_location) {
}

mafLoggerConsole::~mafLoggerConsole() {
}

void mafLoggerConsole::loggedMessage(const QtMsgType type, const QString &msg) {
	QString dt(QDateTime::currentDateTime().toString(mafDateTimeLogFormat));
    printf("%s ---> " , dt.toLatin1());
    switch (type) {
    case QtDebugMsg:
        if(logMode() == mafLogModeTestSuite) {
			printf("%s %s\n", TEST_SUITE_LOG_PREFIX, msg.toLatin1());
        } else {
			printf("Debug: %s\n", msg.toLatin1());
        }
        break;
    case QtWarningMsg:
        if(logMode() == mafLogModeTestSuite) {
			printf("%s %s\n", TEST_SUITE_LOG_PREFIX, msg.toLatin1());
        } else {
			printf("Warning: %s\n", msg.toLatin1());
        }
        break;
    case QtCriticalMsg:
		printf("Critical: %s\n", msg.toLatin1());
        break;
    case QtFatalMsg:
		printf("Fatal: %s\n", msg.toLatin1());
        //abort();
        break;
    }
}
