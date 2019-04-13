#include <FilterKum.h>
FilterKum FilterKum;
float ExpMov_Val;	// Глобальная переменная для Экспоненциального фильтра
float A[3];
float Ar[3];
float Arr[5];
int a = sizeof(A) / sizeof(A[0]);	// Получение количества элеменитов массива
int b = sizeof(Ar) / sizeof(Ar[0]);
int c = sizeof(Arr) / sizeof(Arr[0]);
float Dist_Val;	// Глобальная переменная для первичного измерения
const int Ttig = 2;	// Пины ультразвукового дальномера
const int Echo = 3;
float Temperature = 20;	// Температура окружающей среды
int Range = 2;
float Mist = 0.001;
float React = 0.05;
float Delta = 0.08;
float Sigma = 40;
float Noise = 1;
float Measure = 40;
float Estimate = 40;
float Coef_Q = 0.5;
int Delay = 500;
void setup()
{
  Serial.begin (9600);
  pinMode(Ttig , OUTPUT);
  pinMode(Echo, INPUT);
}
void loop()
{
  for (byte z = 0; z < c; z++)
  {
    for (byte y = 0; y < b; y++)
    {
      for (byte x = 0; x < a; x++)
      {
        Dist_Val = FilterKum.Get_Dist(Ttig, Echo, Temperature);	// Вызываем функцию измерения расстояния дальномером
        float ExpMov_Val = FilterKum.ExpMov_Filt(Dist_Val, ExpMov_Val);	// Вызываем функцию Экспоненциального фильтра, тут ExpMov_Val - старое значение фильтрации
        A[x] = ExpMov_Val;
      }
      float Mid = FilterKum.Median_Filt(A);	// Вызываем функцию Медианного фильтра
      Ar[y] = Mid;
    }
    float Often_Val = FilterKum.Often_Filt(Ar, b, Range);	// Вызываем функцию Частого фильтра, b - число семплов для фильтрации, Range - точность (чем ниже - тем точнее)
    Arr[z] = Often_Val;
  }
  float WeightMov_Val = FilterKum.WeightMov_Filt(Arr, c);	// Вызываем функцию Весового фильтра, c - число семплов для фильтрации
  float SimpKalm_Val = FilterKum.SimpKalm_Filt(Measure, Estimate, Coef_Q, WeightMov_Val);	// Вызываем функцию фильтра Простого Калмана
  float AbKalm_Val = FilterKum.AbKalm_Filt(Delta, Sigma, Noise, SimpKalm_Val);	// Вызываем функцию Альфа-Бетта фильтра
  float Kalman_Val = FilterKum.Kalman_Filt(AbKalm_Val, Mist, React);	// Вызываем функцию фильтра Калмана, тут Mist - ошибка фильтра Калмана, React - скорость реакции фильтра Калмана
  Serial.println(Dist_Val);
  Serial.println(Kalman_Val);
  Serial.println("**********************************************");
  delay(Delay);
}