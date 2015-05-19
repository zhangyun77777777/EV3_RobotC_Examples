#pragma config(Sensor, S1,     touchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro)
#pragma config(Sensor, S3,     colorSensor,    sensorEV3_Color)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


/*****************************************Button*********************************************/
void button_blocking(const char button)
{
	while(!getButtonPress(button));
	while(getButtonPress(button));
}

int get_pressed_button()
{
	if(getButtonPress(buttonLeft)){
		while(getButtonPress(buttonLeft));
		return buttonLeft;
	}
	if(getButtonPress(buttonRight)){
		while(getButtonPress(buttonRight));
		return buttonRight;
	}
	if(getButtonPress(buttonEnter)){
		while(getButtonPress(buttonEnter));
		return buttonEnter;
	}
	if(getButtonPress(buttonUp)){
		while(getButtonPress(buttonUp));
		return buttonUp;
	}
	if(getButtonPress(buttonDown)){
		while(getButtonPress(buttonDown));
		return buttonDown;
	}

	return buttonNone;
}

/*****************************************Miscellaneous*********************************************/
void wait_start_signal(void)
{
	playSoundFile("Ready");
	eraseDisplay();
	displayBigTextLine(0,"press ENTER");
	displayBigTextLine(2,"   to start");
	button_blocking(buttonEnter);
}

/*****************************************Motion*********************************************/
void stop_left_right_motor()
{
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor, 0);
}

unsigned int motor_stop_duration = 0;
task task_left_right_motor_stop()
{
	clearTimer(T4);
	while(time1[T4] < motor_stop_duration)
	{
	}

	stop_left_right_motor();
}

void steering_robot(int steering, int motor_power, unsigned int duration_ms=0, TMotorBrakeModes brake=motorBrake)
{
	stopTask(task_left_right_motor_stop);

	if(getMotorBrakeMode(leftMotor)!=motorBrake)
		setMotorBrakeMode(leftMotor, motorBrake);
	if(getMotorBrakeMode(rightMotor)!=motorBrake)
		setMotorBrakeMode(rightMotor, motorBrake);

	int left_motor_power, right_motor_power;

	if(abs(motor_power) > 100)
		motor_power = sgn(motor_power)*100;
	if(abs(steering) > 100)
		steering = sgn(steering)*100;	//limitting steering within -100~100

	if(steering > 0)
	{
		left_motor_power = motor_power;
		right_motor_power = motor_power * (1 - steering*2/100.0);
	}
	else if(steering < 0)
	{
		left_motor_power = motor_power * (1 + steering*2/100.0);
		right_motor_power = motor_power;
	}
	else
	{
		left_motor_power = motor_power;
		right_motor_power = motor_power;
	}

	setMotorSpeed(leftMotor, left_motor_power);
	setMotorSpeed(rightMotor, right_motor_power);

	motor_stop_duration = duration_ms;
	if(duration_ms > 0)
	{
		startTask(task_left_right_motor_stop);
	}
}


/*****************************************PID gain tunning interface*********************************************/

#define BUTTON_P_GAIN buttonLeft
#define BUTTON_I_GAIN buttonUP
#define BUTTON_D_GAIN buttonRight
#define BUTTON_DONE		buttonEnter

#define BUTTON_GAIN_UP buttonUp
#define BUTTON_GAIN_DOWN buttonDown
#define BUTTON_RESOL_UP	buttonLeft
#define BUTTON_RESOL_DOWN	buttonRight


#define RESOLUTION_STEP_SIZE 6
const float RESOLUTION_STEPS[RESOLUTION_STEP_SIZE] = {0.001, 0.01, 0.1, 1.0, 10.0, 100.0};

typedef struct PID_gains{
	float P;
	float I;
	float D;

	int resolution_idx;
} PID_gains;

void display_PID_gains(const PID_gains* gains)
{
	eraseDisplay();
	displayBigTextLine(0,"P : %.3f",gains->P);
	displayBigTextLine(2,"I : %.3f",gains->I);
	displayBigTextLine(4,"D : %.3f",gains->D);
	displayBigTextLine(6,"resol :  %.3f",RESOLUTION_STEPS[gains->resolution_idx]);
	displayBigTextLine(9,"UP/DOWN->PID");
	displayBigTextLine(11,"LEFT/RIGHT->resol");
	displayBigTextLine(13,"Enter to exit");
}

void display_PID_help(void)
{
	eraseDisplay();
	displayCenteredBigTextLine(2,"P : Left");
	displayCenteredBigTextLine(5,"I : Up");
	displayCenteredBigTextLine(8,"D : Right");
}

void resolution_up_down(PID_gains* gains, bool up)
{
	if(up)
	{
		gains->resolution_idx++;
		if(gains->resolution_idx >= RESOLUTION_STEP_SIZE)
		{
			gains->resolution_idx = RESOLUTION_STEP_SIZE-1;
		}
	}
	else
	{
		gains->resolution_idx--;
		if(gains->resolution_idx < 0)
		{
			gains->resolution_idx = 0;
		}
	}
}

void tunning_P_gain(PID_gains* gains)
{
	bool updated = true;
	bool keep_tunning = true;
	int btn = buttonNone;
	while(keep_tunning){
		btn = get_pressed_button();
		switch(btn)
		{
			case BUTTON_GAIN_UP:
			gains->P += RESOLUTION_STEPS[gains->resolution_idx];
			updated = true;
			break;

			case BUTTON_GAIN_DOWN:
			gains->P -= RESOLUTION_STEPS[gains->resolution_idx];
			updated = true;
			break;

			case BUTTON_RESOL_UP:
			resolution_up_down(gains, true);
			updated = true;
			break;

			case BUTTON_RESOL_DOWN:
			resolution_up_down(gains, false);
			updated = true;
			break;

			case BUTTON_DONE:
			keep_tunning = false;
			break;

			default:
			break;
		}

		if(updated)
		{
			display_PID_gains(gains);
			updated = false;
		}
	}
}

void tunning_I_gain(PID_gains* gains)
{
	bool updated = true;
	bool keep_tunning = true;
	int btn = buttonNone;
	while(keep_tunning){
		btn = get_pressed_button();
		switch(btn)
		{
			case BUTTON_GAIN_UP:
			gains->I += RESOLUTION_STEPS[gains->resolution_idx];
			updated = true;
			break;

			case BUTTON_GAIN_DOWN:
			gains->I -= RESOLUTION_STEPS[gains->resolution_idx];
			updated = true;
			break;

			case BUTTON_RESOL_UP:
			resolution_up_down(gains, true);
			updated = true;
			break;

			case BUTTON_RESOL_DOWN:
			resolution_up_down(gains, false);
			updated = true;
			break;

			case BUTTON_DONE:
			keep_tunning = false;
			break;

			default:
			break;
		}

		if(updated)
		{
			display_PID_gains(gains);
			updated = false;
		}
	}
}

void tunning_D_gain(PID_gains* gains)
{
	bool updated = true;
	bool keep_tunning = true;
	int btn = buttonNone;
	while(keep_tunning){
		btn = get_pressed_button();
		switch(btn)
		{
			case BUTTON_GAIN_UP:
			gains->D += RESOLUTION_STEPS[gains->resolution_idx];
			updated = true;
			break;

			case BUTTON_GAIN_DOWN:
			gains->D -= RESOLUTION_STEPS[gains->resolution_idx];
			updated = true;
			break;

			case BUTTON_RESOL_UP:
			resolution_up_down(gains, true);
			updated = true;
			break;

			case BUTTON_RESOL_DOWN:
			resolution_up_down(gains, false);
			updated = true;
			break;

			case BUTTON_DONE:
			keep_tunning = false;
			break;

			default:
			break;
		}

		if(updated)
		{
			display_PID_gains(gains);
			updated = false;
		}
	}
}

void gain_tuning(PID_gains* gains)
{
	bool updated = true;
	bool keep_tunning = true;
	int btn = buttonNone;
	while(keep_tunning)
	{
		btn = get_pressed_button();
		switch(btn)
		{
			case BUTTON_P_GAIN:
			tunning_P_gain(gains);
			updated = true;
			break;

			case BUTTON_I_GAIN:
			tunning_I_gain(gains);
			updated = true;
			break;

			case BUTTON_D_GAIN:
			tunning_D_gain(gains);
			updated = true;
			break;

			case BUTTON_DONE:
			keep_tunning = false;
			break;

			default:
			break;
		}

		if(updated)
		{
			display_PID_help();
			updated = false;
		}
	}
}

/*****************************************Save/Read Parameters*********************************************/
bool save_parameters(const float* p_gain, const float* i_gain, const float* d_gain, const int* motor_power)
{
	long file = fileOpenWrite("line_tracer_params.param");
	bool good = true;
	good &= fileWriteFloat(file, *p_gain);
	good &= fileWriteFloat(file, *i_gain);
	good &= fileWriteFloat(file, *d_gain);
	good &= fileWriteFloat(file, *motor_power);

	good &= fileClose(file);

	return good;
}

bool read_params(float* p_gain, float* i_gain, float* d_gain, int* motor_power)
{
	float motor_power_f = 0.0;
	long file = fileOpenRead("line_tracer_params.param");
	bool good = true;
	good &= fileReadFloat(file,p_gain);
	good &= fileReadFloat(file,i_gain);
	good &= fileReadFloat(file,d_gain);
	good &= fileReadFloat(file,&motor_power_f);
	*motor_power = (int)motor_power_f;

	good &= fileClose(file);

	return good;
}

/*****************************************Main function*********************************************/

void display_help(const int *motor_power, const float *P_gain, const float *I_gain, const float *D_gain)
{
	eraseDisplay();
	displayBigTextLine(0,"Enter:gain tune");
	displayBigTextLine(2,"Up   :Speed Up");
	displayBigTextLine(4,"Down :Speed Down");
	displayBigTextLine(6,"power : %d", *motor_power);
	displayBigTextLine(8,"P : %.4f", *P_gain);
	displayBigTextLine(10,"I : %.4f", *I_gain);
	displayBigTextLine(12,"D : %.4f", *D_gain);
	displayTextLine(14,"keep pressing LEFT ");
	displayTextLine(15,"  to save current params" );
}

task main()
{
	unsigned short reflected_middle = 45;
	//unsigned short reflected_black, reflected_white;
	//getColorReflected(colorSensor);
	//wait1Msec(500);

	//playSoundFile("Black");
	//button_blocking(buttonEnter);
	//reflected_black = getColorReflected(colorSensor);

	//playSoundFile("White");
	//button_blocking(buttonEnter);
	//reflected_white = getColorReflected(colorSensor);

	//reflected_middle = (reflected_black + reflected_white) / 2;
	////displayCenteredBigTextLine(0,"%d",reflected_middle);

	wait_start_signal();

	int motor_power = 50;
	int steering = 0;
	int error=0;
	int last_error=0;
	int error_integration=0;

	float P_gain = 0.4;
	float I_gain = 0.05;
	float D_gain = 0.9;

	//try reading lastly saved parameters
	read_params(&P_gain, &I_gain, &D_gain, &motor_power);

	unsigned int reflected_value;
	PID_gains pid_gains;

	bool display_updated = true;
	display_help(&motor_power, &P_gain, &I_gain, &D_gain);
	while(true)
	{
		/********************************Line Tracer********************************/
		reflected_value = getColorReflected(colorSensor);
		error = (reflected_value - reflected_middle);
		steering = P_gain*error + D_gain*(error - last_error) + I_gain*error_integration;
		error_integration += error;
		last_error = error;

		steering_robot(steering, motor_power);

		/********************************Interface for Manipulating Parameters********************************/
		if(getButtonPress(buttonEnter))
		{
			stop_left_right_motor();
			pid_gains.P = P_gain;
			pid_gains.I = I_gain;
			pid_gains.D = D_gain;
			pid_gains.resolution_idx = 2;
			while(getButtonPress(buttonEnter));
			gain_tuning(&pid_gains);

			P_gain = pid_gains.P;
			I_gain = pid_gains.I;
			D_gain = pid_gains.D;

			wait_start_signal();
			display_updated = true;
		}

		if(getButtonPress(buttonUp))
		{
			motor_power += 5;
			if(motor_power > 100)
				motor_power = 100;
			display_updated = true;
			while(getButtonPress(buttonUp));
		}

		if(getButtonPress(buttonDown))
		{
			motor_power -= 5;
			if(motor_power < -100)
				motor_power = -100;
			display_updated = true;
			while(getButtonPress(buttonDown));
		}

		if(getButtonPress(buttonLeft))
		{
			clearTimer(T3);
			while(getButtonPress(buttonLeft) && (time1[T3]<1500))
			{
			}
			if(time1[T3] >= 1500)
			{
				eraseDisplay();
				displayBigTextLine(2,"Saving Parameters...");
				save_parameters(&P_gain, &I_gain, &D_gain, &motor_power);
				wait1Msec(800);
				displayBigTextLine(5,"..........Done");
				wait1Msec(800);
			}
			while(getButtonPress(buttonLeft));
			display_updated = true;
		}

		if(display_updated)
		{
			display_help(&motor_power, &P_gain, &I_gain, &D_gain);
			display_updated = false;
		}
	}
}
