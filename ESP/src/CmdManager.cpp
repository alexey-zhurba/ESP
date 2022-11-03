// 
// 
// 

#include "CmdManager.h"

_ESP::CmdManager::CmdManager() : m_cmdQueueStart(-1), m_cmdQueueEnd(-1), m_cmdQueueLength(0), m_callbackSP(0), m_stateFlags(0)
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
	PRINT_ENTER_FUNC();
	if (m_cmdQueueStart == m_cmdQueueEnd)
	{
		//TODO: Error message (underflow)
		PRINT_EXIT_FUNC();
		return EspCmd{ 0 };
	}
	if (m_cmdQueueStart == -1)
	{
		m_cmdQueueStart = 0;
	}
	int cmdQueueStart = m_cmdQueueStart;
	m_cmdQueueStart = (m_cmdQueueStart + 1) % MAX_COMMANDS;
	--m_cmdQueueLength;
	DEBUG_PRINTLN("popping cmd");
	DEBUG_PRINT("New m_cmdQueueStart ");
	DEBUG_PRINTLN(m_cmdQueueStart);
	PRINT_EXIT_FUNC();
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
	PRINT_ENTER_FUNC();
	DEBUG_PRINT("Flushing ");
	DEBUG_PRINT(m_cmdQueueLength);
	DEBUG_PRINTLN(" commands.");
	while (m_cmdQueueLength > 0)
	{
		EspCmd cmd = pop();
		DEBUG_PRINT("Popped command. flags: ");
		DEBUG_PRINTLN(cmd.flags);
		if (cmd.flags & FL_ACTIVE)
		{
			for (int i = 0; i < m_callbackSP; ++i)
			{
				bool condition = ((!m_callbackStack[i].bMaskNot && ((cmd.flags & m_callbackStack[i].triggerMask) == m_callbackStack[i].triggerMask)) //mask triggered ?
					|| (m_callbackStack[i].bMaskNot && ((cmd.flags & ~m_callbackStack[i].triggerMask) == cmd.flags))) //not mask triggered ?
					&& ((cmd.flags & m_callbackStack[i].triggerMask) != (m_stateFlags & m_callbackStack[i].triggerMask)); //mask changed ?
				if (condition)
				{
					DEBUG_PRINT("Calling cmd handler @ ");
					DEBUG_PRINT((__uintptr_t)m_callbackStack[i].cmdHandler);
					DEBUG_PRINT(" with param: ");
					DEBUG_PRINTLN((__uintptr_t)m_callbackStack[i].param);
					m_callbackStack[i].cmdHandler(cmd, m_callbackStack[i].param);
				}
			}
		}
		m_stateFlags = cmd.flags;
	}
	PRINT_EXIT_FUNC();
}

void _ESP::CmdManager::sendCmd(EspCmd cmd)
{
	PRINT_ENTER_FUNC();
	int cmdQueueEnd = (m_cmdQueueEnd + 1) % MAX_COMMANDS;
	if (cmdQueueEnd == m_cmdQueueStart)
	{
		PRINT_EXIT_FUNC();
		//TODO: Error message (overflow)
		return;
	}
	m_cmdQueueEnd = cmdQueueEnd;
	m_cmdQueue[m_cmdQueueEnd] = cmd;
	++m_cmdQueueLength;
	DEBUG_PRINTLN("sending cmd");
	DEBUG_PRINT("flags: ");
	DEBUG_PRINTLN(cmd.flags);
	DEBUG_PRINT("New m_cmdQueueEnd: ");
	DEBUG_PRINTLN(m_cmdQueueEnd);
	PRINT_EXIT_FUNC();
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
