/*
 Name:		ESP.ino
 Created:	10/31/2022 7:38:07 PM
 Author:	alexe
*/

#include <RoboApi.h>

#define IR_SENS_L 1
#define IR_SENS_R 2
#define MOTOR_L_IA 3
#define MOTOR_L_IB 2
#define MOTOR_R_IA 19
#define MOTOR_R_IB 18

RoboApi::IRSensor irSensLinks(IR_SENS_L), irSensRechts(IR_SENS_R);
RoboApi::MotorGroup motor(RoboApi::MotorController(MOTOR_L_IA, MOTOR_L_IB), RoboApi::MotorController(MOTOR_R_IA, MOTOR_R_IB));

bool bLWhiteLast, bRWhiteLast;
// the setup function runs once when you press reset or power the board
void setup() {
}

// the loop function runs over and over again until power down or reset
void loop() {
	
}
