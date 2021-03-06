#pragma config(Sensor, S1,     touchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro)
#pragma config(Sensor, S3,     colorSensor,    sensorEV3_Color)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,           ,             tmotorEV3_Large, openLoop)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorD,           ,             tmotorEV3_Large, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define DO 1047
#define RE 1175
#define MI 1319

task main()
{
	clearDebugStream();
	int music[] = {
		MI,RE,DO,RE,MI,MI,MI,
		RE,RE,RE,MI,MI,MI,MI,
		RE,DO,RE,MI,MI,MI,RE,
		RE,MI,RE,DO};

	int n_notes = sizeof(music)/sizeof(int);
	for(int i=0;i<n_notes;i++)
	{
		playTone(music[i],20);
		while(bSoundActive)
		{
		}
	}

}
