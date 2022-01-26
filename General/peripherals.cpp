// ������ ��� ������ � ����������

#define peripherals

// ����� ������ �������� ������������, ������������ ������:
//  - ����������� ������ �� IAR;
//  - ������ base_functions;

#include "msp430G2553.h"

#include "peripherals.hpp"
#include "rus1_peripherals.hpp"
#include "rus1.hpp"

//////////////////////////////// 
// �������� uart
#define uartSpeed 115200
// #define uartSpeed 57600
// #define uartSpeed 38400
// #define uartSpeed 19200
// #define uartSpeed 9600

// ������� // ������� ������ �������� �� ��������� ������, ��� ���������� ������� ������������
unsigned long restart220_seconds;

// ������� ������������
t_LedSignal led = { 0, 0 };

// ������� �� ���������� 220�
t_Power220_Alarm power220_Alarm = { 0, 0 };


// �������� ������ ��������� ��� ������ �������
// #define dGreenLedSignal_ON { dGreenLed_On; if (f_GreenSignal_100msCnt < greenSignal_100ms) f_GreenSignal_100msCnt = greenSignal_100ms; }
void GreenLedSignal_ON() 
{ 
  if (!isRezhimSpecMessage)
  {
    dGreenLed_On; 
    if (getGreenLedSignalCnt < greenSignal_100ms)
      getGreenLedSignalCnt = greenSignal_100ms;
  }
}

/*
// ��������� ���������
#ifndef use_Constant_FreqCPU
void Syst_InitOnStart() \
  { \
    Rus1_InitOnStart(); \
    Syst_SetupUart(uartSpeed); \
    dSyst_SetupPowerGsm; \
    dSyst_SetupPower220; \
    dSyst_SetupMeasure220; \
    dBeep_Setup; \
  }
#endif // #ifdef use_Constant_FreqCPU
*/