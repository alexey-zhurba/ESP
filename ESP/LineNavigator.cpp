// 
// 
// 

#include "LineNavigator.h"

ESP::LineNavigator::LineNavigator(IRSensor irSensLinks, IRSensor irSensRechts, MotorGroup motorGroup) : m_irSensLinks(irSensLinks), m_irSensRechts(irSensRechts), m_motorGroup(motorGroup)
{
	setSpeed(100);
	setRotationSpeed(getSpeed() / 2);
}

void ESP::LineNavigator::setSpeed(int iSpeed)
{
	m_iSpeed = iSpeed;
}

int ESP::LineNavigator::getSpeed()
{
	return m_iSpeed;
}

void ESP::LineNavigator::setRotationSpeed(int iSpeedRot)
{
	m_iSpeedRot = iSpeedRot;
}

int ESP::LineNavigator::getRotationSpeed()
{
	return m_iSpeedRot;
}

void ESP::LineNavigator::start()
{
	m_bLWhiteLast = m_irSensLinks.isWhite();
	m_bRWhiteLast = m_irSensRechts.isWhite();
	TimedObject::start();
}

void ESP::LineNavigator::stop()
{
	TimedObject::stop();
	m_motorGroup.stop();
}

void ESP::LineNavigator::update()
{
	bool bLWhite = m_irSensLinks.isWhite();
	bool bRWhite = m_irSensRechts.isWhite();

	if (bLWhite != m_bLWhiteLast
		|| bRWhite != m_bRWhiteLast)
	{
		m_bLWhiteLast = bLWhite;
		m_bRWhiteLast = bRWhite;

		if (!bLWhite)
		{
			motor.links(getRotationSpeed()); //linker sensor auf schwarz -> nach links
		}
		else if (!bRWhite)
		{
			motor.rechts(getRotationSpeed()); //rechter sensor auf schwarz -> nach rechts
		}
		else
		{
			motor.vorwaerts(getSpeed());
		}
	}
}
