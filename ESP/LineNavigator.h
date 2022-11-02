// LineNavigator.h

#ifndef _LineNavigator_h
#define _LineNavigator_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <RoboApi.h>

using namespace RoboApi;

namespace ESP {
	class LineNavigator : public virtual TimedObject {
	private:
		IRSensor m_irSensLinks;
		IRSensor m_irSensRechts;
		MotorGroup m_motorGroup;
		bool m_bLWhiteLast;
		bool m_bRWhiteLast;
		int m_iSpeed;
		int m_iSpeedRot;
	public:
		LineNavigator(IRSensor irSensLinks, IRSensor irSensRechts, MotorGroup motorGroup);
		void setSpeed(int iSpeed);
		int getSpeed();
		void setRotationSpeed(int iSpeedRot);
		int getRotationSpeed();
		virtual void start();
		virtual void stop();
		virtual void update();
	};
}

#endif

