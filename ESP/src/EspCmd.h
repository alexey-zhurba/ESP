// _ESPCmd.h

#ifndef _ESPCMD_h
#define _ESPCMD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define FL_ACTIVE (1u << 0u)
#define FL_MOVE (1u << 1u)
#define FL_AI (1u << 2u)

namespace _ESP {
	enum CmdOrigin {
		SysCmd = 0, UsrCmd = 1
	};

	struct EspCmd {
		uint16_t flags; //OR'd cmd flags
		CmdOrigin origin;
		struct move_t{
			int forward;
			int left;
		} move;
	};


}
#endif

