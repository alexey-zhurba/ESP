// CmdManager.h

#ifndef _CMDMANAGER_h
#define _CMDMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "EspCmd.h"
#include <RoboApi.h>

#define MAX_COMMANDS 100

namespace _ESP {
	typedef void(*cmdHandler_fn)(EspCmd cmd, void* param);
	struct CmdCallback {
		cmdHandler_fn cmdHandler;
		uint16_t triggerMask;
		bool bMaskNot;
		void* param;
		inline bool operator==(CmdCallback const& callback)
		{
			return cmdHandler == callback.cmdHandler
				&& triggerMask == callback.triggerMask
				&& param == callback.param
				&& bMaskNot == callback.bMaskNot;
		}
	};
	class CmdManager {
	private:
		CmdManager();
		uint16_t m_stateFlags;
		EspCmd m_cmdQueue[MAX_COMMANDS] = { 0 };
		int m_cmdQueueStart;
		int m_cmdQueueEnd;
		int m_cmdQueueLength;
		CmdCallback m_callbackStack[sizeof(CmdCallback::triggerMask)*8] = {0};
		int m_callbackSP;
		EspCmd pop();
		static void defaultCmdHandler(EspCmd cmd, void* param);
	public:
		CmdManager(CmdManager const&) = delete;
		void operator=(CmdManager const&) = delete;
		static CmdManager* instance();
		void flushCmds();
		void sendCmd(EspCmd cmd);
		void registerCmdCallback(CmdCallback callback);
		void unregisterCmdCallback(CmdCallback callback);
		uint16_t stateFlags();
	};
}

#endif

