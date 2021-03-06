#pragma config(Sensor, S1,     touchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro)
#pragma config(Sensor, S3,     colorSensor,    sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,           ,             tmotorEV3_Large, openLoop)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorD,           ,             tmotorEV3_Large, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
This demonstration is about how to control motor and contains
	giving relative motion
	giving absolute motion
	getting encoder value
	spinning robot
	braking mode.

You can run each demonstration by activating '#define' line.
*/

//#define DEMO_RELATIVE_TARGET
//#define DEMO_ABSOLUTE_TARGET
//#define DEMO_GET_MOTOR_VALUE
//#define DEMO_SPINNING_ROBOT
//#define DEMO_BRACKING_MODE

task main()
{
	//Speed : -100 ~ 100

#ifdef DEMO_RELATIVE_TARGET
	/*
	Rotate motor with relative target position and speed.
	'relative target' indicates how much the motor will rotate with respect to the current motor position.
	You must give enough time for the motor reaching the target position.
	*/
	moveMotorTarget(leftMotor, 90, 20);
	waitUntilMotorStop(leftMotor);
	//sleep(2000);

	moveMotorTarget(leftMotor, 90, 20);
	waitUntilMotorStop(leftMotor);
	//sleep(2000);


	/*
	It seems that rotational direction is only depending on the sign of speed.
	Looking at 3rd case, position value is negative but it moves forward.
	*/
	//Forward
	moveMotorTarget(leftMotor, 50, 50);
	waitUntilMotorStop(leftMotor);
	wait1Msec(1000);
	//Backward
	moveMotorTarget(leftMotor, 50, -50);
	waitUntilMotorStop(leftMotor);
	wait1Msec(1000);
	//Forward
	moveMotorTarget(leftMotor, -50, 50);
	waitUntilMotorStop(leftMotor);
	wait1Msec(1000);
	//Backward
	moveMotorTarget(leftMotor, -50, -50);
	waitUntilMotorStop(leftMotor);
	wait1Msec(1000);

#endif

#ifdef DEMO_ABSOLUTE_TARGET
	/*
	Rotate Motor with Absolute Target Position and Speed.
	'absolute position' means how much the motor has been rotated from the start position(0 deg).
	Note that 90deg and -270deg are different target.
	If you relative command of 90deg and repeat this 4 times, motor will end up with 360deg in absolute position.
	Also you must give enough time for the motor reaching the target position.
	*/

	moveMotorTarget(leftMotor, 90, 20);
	waitUntilMotorStop(leftMotor);

	moveMotorTarget(leftMotor, 90, 20);
	waitUntilMotorStop(leftMotor);

	moveMotorTarget(leftMotor, 90, 20);
	waitUntilMotorStop(leftMotor);

	moveMotorTarget(leftMotor, 90, 20);
	waitUntilMotorStop(leftMotor);

	setMotorTarget(leftMotor, 0, 20);

	//sleep(2000);
	waitUntilMotorStop(leftMotor);	//!!!!!!Important.

#endif

#ifdef DEMO_GET_MOTOR_VALUE
/*

*/
	moveMotorTarget(leftMotor, 90, 20);
	while(getMotorRunning(leftMotor))
	{
		writeDebugStreamLine("%10f %10f",getMotorEncoder(leftMotor), getMotorTarget(leftMotor));
		wait1Msec(100);
	}

	moveMotorTarget(leftMotor, 45, 20);
	while(getMotorRunning(leftMotor))
	{
		writeDebugStreamLine("%10f %10f",getMotorEncoder(leftMotor), getMotorTarget(leftMotor));
		wait1Msec(100);
	}

	setMotorTarget(leftMotor, 0, 20);
	while(getMotorRunning(leftMotor))
	{
		writeDebugStreamLine("%10f %10f",getMotorEncoder(leftMotor), getMotorTarget(leftMotor));
		wait1Msec(100);
	}

	//wait1Msec(100);

#endif

#ifdef DEMO_SPINNING_ROBOT
	/*
	Spinning Robot
	You can make the robot spin by applying different speeds for each wheels.
	In this demo, we will rotate left and right motors in the speed of 20 and -20 repectively for 2000ms.
	*/
	setMotorSpeed(leftMotor,50);
	setMotorSpeed(rightMotor,-50);
	wait1Msec(2000);			//This is mandatory.
#endif

#ifdef DEMO_BRACKING_MODE
	/*
	Braking Mode
	Mode of motorCoast make the motor coast(or glide) until it stops.
	The difference can be seen more conspicuously in high speed.
	*/
	setMotorBrakeMode(leftMotor, motorBrake);
	setMotorBrakeMode(rightMotor, motorBrake);
	forward(1,rotations, 100);

	sleep(1000);

	setMotorBrakeMode(leftMotor, motorCoast);
	setMotorBrakeMode(rightMotor, motorCoast);
	forward(1,rotations, 100);
#endif


}
