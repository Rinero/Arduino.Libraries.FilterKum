/*
 * FilterKum.h - is a library designed to filter any incoming data stream.
 * This library includes 7 different types of filters and a function for receiving data from a range meter.
*/

#ifndef FilterKum_h
#define FilterKum_h

#if ARDUINO < 100
#include "WProgram.h"
#else
#include "Arduino.h"
#endif

class FilterKum
{
  public:
    FilterKum();
	float Get_Dist(const int Trig, const int Echo, float Temp);
	float ExpMov_Filt(float Value, float ExpMov_Val);
	float Median_Filt(float *A);
	float Often_Filt(float *Buf, int Size, float Range);
	float WeightMov_Filt(float *Value, int Quan);
	float SimpKalm_Filt(float Err_Measure, float Err_Estimate, float Q, float Value);
	float AbKalm_Filt(float Dt, float Sigma_Process, float Sigma_Noise, float Value);
	float Kalman_Filt(float Value, float Mist, float React);
  private:
};

#endif
