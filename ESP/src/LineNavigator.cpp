// 
// 
// 

#include "LineNavigator.h"

_ESP::LineNavigator::LineNavigator(IRSensor irSensLinks, IRSensor irSensRechts) : m_irSensLinks(irSensLinks), m_irSensRechts(irSensRechts), m_motorMngr(MotorManager::instance())
{
	setSpeed(100);
	setRotationSpeed(getSpeed() / 2);
	CmdCallback callback;
	callback.cmdHandler = (cmdHandler_fn)handlerAI;
	callback.param = this;
	callback.triggerMask = FL_AI;
	callback.bMaskNot = false;
	CmdManager::instance()->registerCmdCallback(callback);
	callback.cmdHandler = (cmdHandler_fn)handlerStopAI;
	callback.bMaskNot = true;
	CmdManager::instance()->registerCmdCallback(callback);
}

_ESP::LineNavigator::~LineNavigator()
{
	CmdCallback callback;
	callback.cmdHandler = (cmdHandler_fn)handlerAI;
	callback.param = this;
	callback.triggerMask = FL_AI;
	callback.bMaskNot = false;
	CmdManager::instance()->unregisterCmdCallback(callback);
	callback.cmdHandler = (cmdHandler_fn)handlerStopAI;
	callback.bMaskNot = true;
	CmdManager::instance()->unregisterCmdCallback(callback);
}

void _ESP::LineNavigator::setSpeed(int iSpeed)
{
	m_iSpeed = iSpeed;
}

int _ESP::LineNavigator::getSpeed()
{
	return m_iSpeed;
}

void _ESP::LineNavigator::setRotationSpeed(int iSpeedRot)
{
	m_iSpeedRot = iSpeedRot;
}

int _ESP::LineNavigator::getRotationSpeed()
{
	return m_iSpeedRot;
}

void _ESP::LineNavigator::start()
{
	if (!isStopped())
		return;
	m_bLWhiteLast = m_irSensLinks.isWhite();
	m_bRWhiteLast = m_irSensRechts.isWhite();
	TimedObject::start();
}

void _ESP::LineNavigator::stop()
{
	if (isStopped())
		return;
	TimedObject::stop();
	m_motorMngr->createMove(0, 0, true, SysCmd);
}

void _ESP::LineNavigator::handlerAI(EspCmd cmd, LineNavigator* _this)
{
	_this->start();
}

void _ESP::LineNavigator::handlerStopAI(EspCmd cmd, LineNavigator* _this)
{
	_this->stop();
	//TODO: sendCmd FL_USER maybe global handler manager for both FL_USER AND FL_AI is better
}

void _ESP::LineNavigator::update()
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
			m_motorMngr->createMove(0, getRotationSpeed(), false, SysCmd); //linker sensor auf schwarz -> nach links
		}
		else if (!bRWhite)
		{
			m_motorMngr->createMove(0, -getRotationSpeed(), false, SysCmd); //rechter sensor auf schwarz -> nach rechts
		}
		else
		{
			m_motorMngr->createMove(getSpeed(), 0, false, SysCmd);
		}
	}
}
