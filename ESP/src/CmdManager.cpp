// 
// 
// 

#include "CmdManager.h"

_ESP::CmdManager::CmdManager() : m_cmdQueueStart(0), m_cmdQueueEnd(0), m_cmdQueueLength(0), m_callbackSP(0) 
{
	CmdCallback callback;
	callback.cmdHandler = defaultCmdHandler;
	callback.triggerMask = 0; 
	callback.param = nullptr;
	callback.bMaskNot = true;
	registerCmdCallback(callback);
}

_ESP::EspCmd _ESP::CmdManager::pop()
{
	if (m_cmdQueueStart == m_cmdQueueEnd)
	{
		//TODO: Error message (underflow)
		return EspCmd{ 0 };
	}
	int cmdQueueStart = m_cmdQueueStart;
	m_cmdQueueStart = (m_cmdQueueStart + 1) % MAX_COMMANDS;
	--m_cmdQueueLength;
	return m_cmdQueue[cmdQueueStart];
}

void _ESP::CmdManager::defaultCmdHandler(EspCmd cmd, void* param)
{
}

_ESP::CmdManager* _ESP::CmdManager::instance()
{
	static CmdManager g_cmdManager;
	return &g_cmdManager;
}

void _ESP::CmdManager::flushCmds()
{
	while (m_cmdQueueLength > 0)
	{
		EspCmd cmd = pop();
		m_stateFlags = cmd.flags;
		if (cmd.flags & FL_ACTIVE)
		{
			for (int i = 0; i < m_callbackSP; ++m_callbackSP)
			{
				bool condition = ((!m_callbackStack[i].bMaskNot && ((cmd.flags & m_callbackStack[i].triggerMask) == m_callbackStack[i].triggerMask)) //mask triggered ?
					|| (m_callbackStack[i].bMaskNot && ((cmd.flags & ~m_callbackStack[i].triggerMask) == cmd.flags))) //not mask triggered ?
					&& ((cmd.flags & m_callbackStack[i].triggerMask) != (m_stateFlags & m_callbackStack[i].triggerMask)); //mask changed ?
				if (condition)
				{
					m_callbackStack[i].cmdHandler(cmd, m_callbackStack[i].param);
				}
			}
		}
	}
}

void _ESP::CmdManager::sendCmd(EspCmd cmd)
{
	int cmdQueueEnd = (m_cmdQueueEnd + 1) % MAX_COMMANDS;
	if (cmdQueueEnd == m_cmdQueueStart)
	{
		//TODO: Error message (overflow)
		return;
	}
	m_cmdQueueEnd = cmdQueueEnd;
	m_cmdQueue[m_cmdQueueEnd] = cmd;
	++m_cmdQueueLength;
}

void _ESP::CmdManager::registerCmdCallback(CmdCallback callback)
{
	if (m_callbackSP < COUNT_OF(m_callbackStack))
		m_callbackStack[m_callbackSP++] = callback;
}

void _ESP::CmdManager::unregisterCmdCallback(CmdCallback callback)
{
	for (int i = m_callbackSP - 1; i >= 0; --i)
	{
		if (m_callbackStack[i] == callback)
		{
			--m_callbackSP;
			for (int j = i; j < m_callbackSP; ++j)
			{
				m_callbackStack[j] = m_callbackStack[j + 1]; //Alle Elemente nach links einrücken
			}
		}
	}
}

uint16_t _ESP::CmdManager::stateFlags()
{
	return m_stateFlags;
}
