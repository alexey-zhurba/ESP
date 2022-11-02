// CmdManager.h

#ifndef _CMDMANAGER_h
#define _CMDMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <EspCmd.h>

#define MAX_COMMANDS 100

namespace ESP {
	class CmdManager {
	private:
		CmdManager();
		EspCmd m_cmdQueue[MAX_COMMANDS];
		int m_cmdQueueStart;
		int m_cmdQueueEnd;
		EspCmd pop();
	public:
		CmdManager(CmdManager const&) = delete;
		void operator=(CmdManager const&) = delete;
		CmdManager* instance();
		void update();
		void sendCmd(EspCmd cmd);
	};
}

#endif

