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
		EspCmd m_cmdQueue[MAX_COMMANDS] = { 0 };
		int m_cmdQueueStart;
		int m_cmdQueueEnd;
		int m_cmdQueueLength;
		EspCmd pop();
		void handleCommmand();
	public:
		CmdManager(CmdManager const&) = delete;
		void operator=(CmdManager const&) = delete;
		CmdManager* instance();
		void flushCmds();
		void sendCmd(EspCmd cmd);
	};
}

#endif

