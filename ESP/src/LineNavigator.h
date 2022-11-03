// LineNavigator.h

#ifndef _LineNavigator_h
#define _LineNavigator_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <RoboApi.h>
#include "MotorManager.h"
#include "CmdManager.h"

using namespace RoboApi;

namespace _ESP {
	class LineNavigator : public virtual TimedObject {
	private:
		IRSensor m_irSensLinks;
		IRSensor m_irSensRechts;
		bool m_bLWhiteLast;
		bool m_bRWhiteLast;
		int m_iSpeed;
		int m_iSpeedRot;
		MotorManager* m_motorMngr;
		virtual void start();
		virtual void stop();
		static void handlerAI(EspCmd cmd, LineNavigator* _this);
		static void handlerStopAI(EspCmd cmd, LineNavigator* _this);
	public:
		LineNavigator(IRSensor irSensLinks, IRSensor irSensRechts);
		~LineNavigator();
		void setSpeed(int iSpeed);
		int getSpeed();
		void setRotationSpeed(int iSpeedRot);
		int getRotationSpeed();
		virtual void update();
	};
}

#endif

