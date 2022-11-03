// MotorManager.h

#ifndef _MotorManager_h
#define _MotorManager_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <RoboApi.h>
#include "CmdManager.h"
#include "PinDef.h"

namespace _ESP {
	class MotorManager {
	private:
		RoboApi::MotorGroup m_motor;
		MotorManager();
		static void handlerMove(EspCmd cmd, MotorManager* _this);
		static void handlerStop(EspCmd cmd, MotorManager* _this);
	public:
		MotorManager(MotorManager const&) = delete;
		void operator=(MotorManager const&) = delete;
		static MotorManager* instance();
		void createMove(int forward, int left, bool bStop);
	};
}

#endif

