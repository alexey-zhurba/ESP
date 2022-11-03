/*
 Name:		ESP.ino
 Created:	10/31/2022 7:38:07 PM
 Author:	alexe
*/

#include "CmdManager.h"
#include "ESPCmd.h"
#include <RoboApi.h>
#include <LineNavigator.h>

#define IR_SENS_L 1
#define IR_SENS_R 2

RoboApi::IRSensor irSensLinks(IR_SENS_L), irSensRechts(IR_SENS_R);

ESP::LineNavigator lineNav(irSensLinks, irSensRechts);
// the setup function runs once when you press reset or power the board

void setup() {
	ESP::EspCmd cmd{ 0 };
	cmd.flags = FL_ACTIVE | FL_AI; //start robot line navigator
	cmd.origin = ESP::SysCmd;
	ESP::CmdManager::instance()->sendCmd(cmd);
}

// the loop function runs over and over again until power down or reset
void loop() {
	ESP::CmdManager::instance()->flushCmds();
	RoboApi::TimedObjectManager::instance()->update();
}
