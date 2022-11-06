// PinDef.h

#ifndef _PinDef_h
#define _PinDef_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define MOTOR_L_IA 2
#define MOTOR_L_IB 3
#define MOTOR_R_IA 18
#define MOTOR_R_IB 19

#define IR_SENS_L A0
#define IR_SENS_R A1


#endif

