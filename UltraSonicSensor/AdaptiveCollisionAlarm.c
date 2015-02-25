#pragma config(Sensor, S1,     touchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro)
#pragma config(Sensor, S3,     colorSensor,    sensorEV3_Color)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,           ,             tmotorEV3_Large, openLoop)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorD,           ,             tmotorEV3_Large, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int t_sound = 0;

task collisionAlarm()
{
		bool beep = true;
		setSoundVolume(1);
		while(true)
		{
			switch(t_sound)
			{
				case 0:
				clearSounds();
				break;

				case 100:
				if(bSoundQueueAvailable)
					playTone(1000, 100);
				break;

				default:
				clearSounds();
				if(beep){
					playTone(1000, t_sound);
					while(bSoundActive);
				}
				else
				{
					wait10Msec(t_sound);
				}
			}
		}
}

void calculateHazard(float distance)
{
	/*
	Calculation of hazardness must end up with an integer value [0,100].
	0 denotes not hazardous.
	100 denotes very hazardous.
	*/
	if(distance > 15.0)
	{
		t_sound = 0;
		return;
	}

	if(distance < 5.0)
	{
		t_sound = 100;
		return;
	}

	t_sound = (10 * distance + 50.0)*0.5*0.1;

}

task main()
{
	startTask(collisionAlarm);

	float distance;
	while(true)
	{
		/*
		Measuring distance to front object in cm unit.
		*/
		distance = getUSDistance(sonarSensor);
		calculateHazard(distance);
	}

}
