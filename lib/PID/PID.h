#ifndef PID_v1_h
#define PID_v1_h
#define LIBRARY_VERSION	1.2.1

#include <math.h>

class PID
{


  public:

  //Constants used in some of the functions below
  #define AUTOMATIC	1
  #define MANUAL	0
  #define DIRECT  0
  #define REVERSE  1
  #define P_ON_M 0
  #define P_ON_E 1

  	//commonly used functions **************************************************************************
    // De-facto constructor.  Initial tuning parameters are set here as well as sample time.
	void setup(float Kp, float Ki, float Kd, int sampleTime, int POn, int ControllerDirection);  

    float Compute(float inputAngleRev, float setPointAngleRev); // Does a computation. Should be called at the interval
    											// defined when creating the object.

    void SetOutputLimits(float Min, float Max); // * clamps the output to a specific range. 0-255 by default, but
										                      //   it's likely the user will want to change this depending on
										                      //   the application
	

    void init(float input);

  //available but not commonly used functions ********************************************************
    void SetTunings(float, float,       // * While most users will set the tunings once in the 
                    float);         	    //   constructor, this function gives the user the option
                                          //   of changing tunings during runtime for Adaptive control
    void SetTunings(float, float,       // * overload for specifying proportional mode
                    float, int);         	  

	void SetControllerDirection(int);	  // * Sets the Direction, or "Action" of the controller. DIRECT
										  //   means the output will increase when error is positive. REVERSE
										  //   means the opposite.  it's very unlikely that this will be needed
										  //   once it is set in the constructor.
    void SetSampleTime(int);              // * sets the frequency, in Milliseconds, with which 
                                          //   the PID calculation is performed.  default is 100
										  
										  


  //Display functions ****************************************************************
	float GetKp();						  // These functions query the pid for interal values.
	float GetKi();						  //  they were created mainly for the pid front-end,
	float GetKd();						  // where it's important to know what is actually
	int GetDirection();					  //
	
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

