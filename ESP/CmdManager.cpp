// 
// 
// 

#include "CmdManager.h"

ESP::CmdManager::CmdManager() : m_cmdQueueStart(0), m_cmdQueueEnd(0), m_cmdQueueLength(0) {}

ESP::EspCmd ESP::CmdManager::pop()
{
	if (m_cmdQueueStart == m_cmdQueueEnd)
	{
		//TODO: Error message (underflow)
		return;
	}
	int cmdQueueStart = m_cmdQueueStart;
	m_cmdQueueStart = (m_cmdQueueStart + 1) % MAX_COMMANDS;
	--m_cmdQueueLength;
	return m_cmdQueue[cmdQueueStart];
}

ESP::CmdManager* ESP::CmdManager::instance()
{
	static CmdManager g_cmdManager;
	return &g_cmdManager;
}

void ESP::CmdManager::flushCmds()
{
	while (m_cmdQueueLength > 0)
	{
		EspCmd cmd = pop();
		cmd.flags
	}
}

void ESP::CmdManager::sendCmd(EspCmd cmd)
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
