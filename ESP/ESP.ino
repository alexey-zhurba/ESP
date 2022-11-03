/*
 Name:		_ESP.ino
 Created:	10/31/2022 7:38:07 PM
 Author:	alexe
*/

#include "src\CmdManager.h"
#include "src\EspCmd.h"
#include <RoboApi.h>
#include "src\LineNavigator.h"

#define IR_SENS_L 1
#define IR_SENS_R 2

RoboApi::IRSensor irSensLinks(IR_SENS_L), irSensRechts(IR_SENS_R);

_ESP::LineNavigator lineNav(irSensLinks, irSensRechts);
// the setup function runs once when you press reset or power the board

void setup() {
	_ESP::EspCmd cmd{ 0 };
	cmd.flags = FL_ACTIVE; //start robot line navigator
	cmd.origin = _ESP::SysCmd;
	_ESP::CmdManager::instance()->sendCmd(cmd);
	_ESP::MotorManager::instance()->createMove(255, 0, false);
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	_ESP::CmdManager::instance()->flushCmds();
	RoboApi::TimedObjectManager::instance()->update();
}
