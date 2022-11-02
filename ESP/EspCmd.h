// ESPCmd.h

#ifndef _ESPCMD_h
#define _ESPCMD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define FL_ACTIVE (1 << 0)
#define FL_MOVE (1 << 1)

namespace ESP {
	enum CmdOrigin {
		SysCmd = 0, UsrCmd = 1
	};

	struct EspCmd {
		uint16_t flags; //OR'd cmd flags
		CmdOrigin origin;
		struct move_t{
			int speed_fwd;
			int speed_bwd;
			int speed_left;
			int speed_right;
		} move;
	};


}
#endif

