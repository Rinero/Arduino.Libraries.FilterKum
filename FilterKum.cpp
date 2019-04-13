/*
 * FilterKum.h - is a library designed to filter any incoming data stream.
 * This library includes 7 different types of filters and a function for receiving data from a range meter.
*/

#if ARDUINO < 100
#include "WProgram.h"
#else
#include "Arduino.h"
#endif
#include "FilterKum.h"

FilterKum::FilterKum()
{
}
float FilterKum::Get_Dist(const int Trig, const int Echo, float Temp)
{
  digitalWrite(Trig, HIGH);
  delay(0.01);
  digitalWrite(Trig, LOW);
  float Times = pulseIn(Echo, HIGH);
  float Dist = Times / float(2 / ((331.5 + 0.6 * Temp) / 1000));
  Dist = (Dist < 0) ? 0 : Dist;
  return Dist;
}
float FilterKum::ExpMov_Filt(float Value, float ExpMov_Val)
{
  float Delta = abs(ExpMov_Val - Value);
  float k = (Delta > 1) ? 0.7 : 0.1;
  return Value * k + ExpMov_Val * (1 - k);
}
float FilterKum::Median_Filt(float *A)
{
  float Middle;
  if ((A[0] <= A[1]) && (A[0] <= A[2]))
  {
    Middle = (A[1] <= A[2]) ? A[1] : A[2];
  }
  else
  {
    if ((A[1] <= A[0]) && (A[1] <= A[2]))
    {
      Middle = (A[0] <= A[2]) ? A[0] : A[2];
    }
    else
    {
      Middle = (A[0] <= A[1]) ? A[0] : A[1];
    }
  }
  return Middle;
}
float FilterKum::Often_Filt(float *Buf, int Size, float Range)
{
  float MaxVal = 0;
  float Oft;
  float Var;
  for (byte i = 0; i < Size; i++)
  {
    Var = Buf[i];
    float Count = 0;
    for (byte j = 0; j < Size; j++)
    {
      if (Buf[j] > Var - Range && Buf[j] < Var + Range)
      Count++;
    }
    if (Count > MaxVal)
    {
      MaxVal = Count;
      Oft = Var;
    }
  }
  return Oft;
}
float FilterKum::WeightMov_Filt(float *Value, int Quan)
{
  int k = Quan - 1;
  float Num = 0;
  float Den = 0;
  for (Quan; Quan > 0; k--, Quan--)
  {
    float Comp = Quan * Value[k];
    Num += Comp;
    Den += Quan;
  }
  return Num / Den;
}
float FilterKum::SimpKalm_Filt(float Err_Measure, float Err_Estimate, float Q, float Value)
{
  float Last_Estimate;
  float Kalman_Gain = Err_Estimate / (Err_Estimate + Err_Measure);
  float Current_Estimate = Last_Estimate + Kalman_Gain * (Value * 2 - Last_Estimate);
  Err_Estimate = (1.0 - Kalman_Gain) * Err_Estimate + fabs(Last_Estimate - Current_Estimate) * Q;
  Last_Estimate = Current_Estimate;
  return Current_Estimate;
}
float FilterKum::AbKalm_Filt(float Dt, float Sigma_Process, float Sigma_Noise, float Value)
{
  float Xk_1;
  float Vk_1;
  float Lambda = (float)Sigma_Process * Dt * Dt / Sigma_Noise;
  float r = (4 + Lambda - (float)sqrt(8 * Lambda + Lambda * Lambda)) / 4;
  float a = (float)1 - r * r;
  float b = (float)2 * (2 - a) - 4 * (float)sqrt(1 - a);
  float Xm = Value * 2;
  float Xk = Xk_1 + ((float) Vk_1 * Dt );
  float Vk = Vk_1;
  float Rk = Xm - Xk;
  Xk += (float) a * Rk;
  Vk += (float) (b * Rk) / Dt;
  Xk_1 = Xk;
  Vk_1 = Vk;
  return Xk_1;
}
float FilterKum::Kalman_Filt(float Value, float Mist, float React)
{
  float Pc = 0.0;
  float G = 0.0;
  float P = 1.0;
  float Xp = 0.0;
  float Zp = 0.0;
  float Xe = 0.0;
  Pc = P + React;
  G = Pc / (Pc + Mist);
  P = (1 - G) * Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G * Value + (1 - G) * Xe;
  return Xe;
}