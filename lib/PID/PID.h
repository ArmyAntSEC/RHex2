#ifndef PID_v1_h
#define PID_v1_h

#include <math.h>

//Constants used in some of the functions below
#define AUTOMATIC	1
#define MANUAL	0
#define DIRECT  0
#define REVERSE  1
#define P_ON_M 0
#define P_ON_E 1

class PID
{
  public:
	void setup(float Kp, float Ki, float Kd, int sampleTime, int POn, int ControllerDirection)
	{
		PID::SetOutputLimits(-250, 250);			
		SampleTime = sampleTime;

		PID::SetControllerDirection(ControllerDirection);
		PID::SetTunings(Kp, Ki, Kd, POn);
	}
  
    float Compute(float inputAngleRev, float setPointAngleRev)
	{
		/*Compute all the working error variables*/
		float error = this->angleDifferenceRev(setPointAngleRev, inputAngleRev);
	
		float dInput = this->angleDifferenceRev(inputAngleRev, lastInput);
		outputSum+= (ki * error);

		/*Add Proportional on Measurement, if P_ON_M is specified*/
		if(!pOnE) outputSum-= kp * dInput;

		if(outputSum > outMax) outputSum= outMax;
		else if(outputSum < outMin) outputSum= outMin;

		/*Add Proportional on Error, if P_ON_E is specified*/
		float output;
		if(pOnE) output = kp * error;
		else output = 0;

		/*Compute Rest of PID Output*/
		output += outputSum - kd * dInput;

		if(output > outMax) output = outMax;
		else if(output < outMin) output = outMin;

		/*Remember some variables for next time*/
		lastInput = inputAngleRev;
		return output;
	}

    void SetOutputLimits(float Min, float Max)
	{
		if(Min >= Max) return;
		outMin = Min;
		outMax = Max;

		if(outputSum > outMax) outputSum= outMax;
		else if(outputSum < outMin) outputSum= outMin;
	}

    void setup(float input)
	{
		outputSum = 0;
		lastInput = input;
		if(outputSum > outMax) outputSum = outMax;
		else if(outputSum < outMin) outputSum = outMin;
	}

    void SetTunings(float Kp, float Ki, float Kd, int POn)
	{
		if (Kp<0 || Ki<0 || Kd<0) return;

		pOn = POn;
		pOnE = POn == P_ON_E;

		dispKp = Kp; dispKi = Ki; dispKd = Kd;

		float SampleTimeInSec = ((float)SampleTime)/1000;
		kp = Kp;
		ki = Ki * SampleTimeInSec;
		kd = Kd / SampleTimeInSec;

		if(controllerDirection ==REVERSE)
		{
			kp = (0 - kp);
			ki = (0 - ki);
			kd = (0 - kd);
		}
	}

    void SetTunings(float Kp, float Ki, float Kd)
	{
		SetTunings(Kp, Ki, Kd, pOn);
	}         	  

	void SetControllerDirection(int Direction)
	{
		if(Direction != controllerDirection)
		{
			kp = (0 - kp);
			ki = (0 - ki);
			kd = (0 - kd);
		}
		controllerDirection = Direction;
	}

    void SetSampleTime(int NewSampleTime)
	{
		if (NewSampleTime > 0)
		{
			float ratio  = (float)NewSampleTime / (float)SampleTime;
			ki *= ratio;
			kd /= ratio;
			SampleTime = (unsigned long)NewSampleTime;
		}
	}

	float getMaxOutput()
	{
		return this->outMax;
	}
	
  private:
	
	float dispKp;				// * we'll hold on to the tuning parameters in user-entered 
	float dispKi;				//   format for display purposes
	float dispKd;				//
    
	float kp;                  // * (P)roportional Tuning Parameter
    float ki;                  // * (I)ntegral Tuning Parameter
    float kd;                  // * (D)erivative Tuning Parameter

	int controllerDirection;
	int pOn;

	float outputSum, lastInput;

	unsigned long SampleTime;
	float outMin, outMax;
	bool pOnE;

	inline float angleDifferenceRev( float angle1Rev, float angle2Rev )
	{
	    float diff = fmod(( angle2Rev - angle1Rev + 0.5 ), 1) - 0.5;
	    return diff < -0.5 ? diff + 1 : diff;
	}

};
#endif

