// ������ ������������������ ��������� �������

#ifndef __peripherals__
#define __peripherals__

#include "gsm_mainTitle.hpp"
#include "rus1_mainTitle.hpp"

// ������� SMCLK
// #define f_SmclkFreq 16000000


// ������� ������������
struct t_LedSignal
{
  // ���������� 100ms, ��� �������� �������� �������
  unsigned int f_RedSignal_100msCnt;
  // ���������� 100ms, ��� �������� ������� �������
  unsigned int f_GreenSignal_100msCnt;
};

// ������� �� ���������� 220�
#define interruptsLcd_PowerAlarmBounceDelay_Default ((unsigned int)(interrupts100millisec))
struct t_Power220_Alarm
{
  // �������, ������ // ������� ������ �������� �� ����, ����� ������� ��� "���������� ���������" ������� "���������� �������� ����������� �� �� ������"
  unsigned int f_secondsPowerAlarm;
  // �������, 4�� // ������� ���������� ������� LCD ���������� ��� ������ �� �������� 220�
  unsigned int f_interruptsLcd_PowerAlarmBounceDelay;
  // �������, 4�� // ������� ���������� ������� LCD ���������� ��� ����, ����� ���������, ��� ��������������� 220�
  unsigned int f_interruptsLcd_PowerAlarmNormDelay;
};

//////////////////////////////// 
// �������� uart
#define uartSpeed 115200
// #define uartSpeed 57600
// #define uartSpeed 38400
// #define uartSpeed 19200
// #define uartSpeed 9600

// ������������� // ��� ���������
/////////////////////

// ����� receiveGsmPackageCount
#define dResetReceiveGsmPackageCount { pReceiveGsmPackageActive = &receiveGsmPackage[0]; }

// ������� gsm-������
  #define dGsmVcc_Bit BIT0
  #define isGsmVcc_On ( 0 != (P1OUT & dGsmVcc_Bit) )
  #define dGsmVcc_Setup { P1SEL &= ~(dGsmVcc_Bit); \
                        P1SEL2 &= ~(dGsmVcc_Bit); \
                        P1DIR |= dGsmVcc_Bit; \
                        dGsmVcc_Off; }
  #define dGsmVcc_On { P1OUT |= dGsmVcc_Bit; }
  #define dGsmVcc_Off { P1OUT &= ~dGsmVcc_Bit; }
  #define dGsmVcc_Invert { P1OUT ^= dGsmVcc_Bit; }


// ���������� 220�
///////////////////////

// power220_Alarm.f_secondsPowerAlarm
#define secondsPowerAlarmMax 15
#define isStartSecondsPowerAlarm ( 0 != power220_Alarm.f_secondsPowerAlarm )
#define dDecSecondsPowerAlarm { if (power220_Alarm.f_secondsPowerAlarm) power220_Alarm.f_secondsPowerAlarm--; }
#define dStartSecondsPowerAlarm { power220_Alarm.f_secondsPowerAlarm = secondsPowerAlarmMax; }
#define dStopSecondsPowerAlarm { power220_Alarm.f_secondsPowerAlarm = 0; }
#define secondsPowerAlarmLeft ( secondsPowerAlarmMax - power220_Alarm.f_secondsPowerAlarm )
// power220_Alarm.f_interruptsLcd_PowerAlarmBounceDelay
#define f_interruptsLcd_PowerAlarmBounceDelayMax (interrupts100millisec * 10)
#define dStartPowerAlarmBounceDelay { power220_Alarm.f_interruptsLcd_PowerAlarmBounceDelay = f_interruptsLcd_PowerAlarmBounceDelayMax; }
#define dDecPowerAlarmBounceDelay { if (power220_Alarm.f_interruptsLcd_PowerAlarmBounceDelay) power220_Alarm.f_interruptsLcd_PowerAlarmBounceDelay--; }
#define isStartPowerAlarmBounceDelay ( 0 != power220_Alarm.f_interruptsLcd_PowerAlarmBounceDelay )
// power220_Alarm.f_interruptsLcd_PowerAlarmNormDelay
#define f_interruptsLcd_PowerAlarmNormDelayMax (interrupts100millisec * 10)
#define dStartPowerAlarmNormDelay { power220_Alarm.f_interruptsLcd_PowerAlarmNormDelay = f_interruptsLcd_PowerAlarmNormDelayMax; }
#define dDecPowerAlarmNormDelay { if (power220_Alarm.f_interruptsLcd_PowerAlarmNormDelay) power220_Alarm.f_interruptsLcd_PowerAlarmNormDelay--; }
#define isStartPowerAlarmNormDelay ( 0 != power220_Alarm.f_interruptsLcd_PowerAlarmNormDelay )

// ���� 220�
#define dRelePower220_Bit BIT3
#define isRelePower220_On ( 0 == (P1OUT & dRelePower220_Bit) )
#define dRelePower220_Setup { P1SEL &= ~(dRelePower220_Bit); \
                        P1SEL2 &= ~(dRelePower220_Bit); \
                        P1DIR |= dRelePower220_Bit; \
                        dRelePower220_Off; }
#define dRelePower220_Invert { P1OUT ^= dRelePower220_Bit; }

// �������� ������
#define dBeep_Bit BIT5
#define dBeep_Setup { P1SEL &= ~(dBeep_Bit); \
                        P1SEL2 &= ~(dBeep_Bit); \
                        P1DIR |= dBeep_Bit; \
                        dBeep_Off; }
#define dBeep_On { if (nastr_IndexBeepOff) P1OUT |= dBeep_Bit; }
#define dBeep_Off { P1OUT &= ~dBeep_Bit; }


// ������� uart
//  NOTE: Initializing or Re-Configuring the USCI Module
//  The recommended USCI initialization/re-configuration process is:
//  1. Set UCSWRST (BIS.B #UCSWRST,&UCAxCTL1)
//  2. Initialize all USCI registers with UCSWRST = 1 (including UCAxCTL1)
//  3. Configure ports.
//  4. Clear UCSWRST via software (BIC.B #UCSWRST,&UCAxCTL1)
//  5. Enable interrupts (optional) via UCAxRXIE and/or UCAxTXIE
#define Syst_SetupUart() { \
  UCA0CTL1 |= UCSWRST; \
  UCA0CTL1 |= UCSSEL_2; /* SMCLK */ \
  UCA0BR0 = 0x8A; \
  UCA0BR1 = 0x00; \
  UCA0MCTL = 0x0E; \
  P1SEL = BIT1 + BIT2 ; /* P1.1 = RXD, P1.2=TXD*/ \
  P1SEL2 = BIT1 + BIT2 ; /* P1.1 = RXD, P1.2=TXD*/ \
  UCA0CTL1 &= ~UCSWRST; /*Initialize USCI state machine*/ \
  IE2 |= UCA0RXIE; /* Enable USCI_A0 RX interrupt */ \
  }


// ������� ����� ���������/���������� ������� ��� gsm-������
// void Syst_SetupPowerGsm()
#define dSyst_SetupPowerGsm \
{ \
  /*������� ������� ��� gsm*/ \
  dGsmVcc_Setup; \
  dGsmVcc_Off; \
}

// ������� ����� ���������/���������� ������� ��� ������� 220�
// void Syst_SetupPower220()
#define dSyst_SetupPower220 \
{ \
  /*������� ������� ��� gsm*/ \
  dRelePower220_Setup; \
  RelePower220_On(); \
}

// ������� ����� ��������� ���������� � ������� 220�
// void Syst_SetupMeasure220()
#define dSyst_SetupMeasure220 \
{ \
  ADC10CTL0 = ADC10SHT0 + ADC10ON + REF2_5V + REFON + SREF0 + ADC10SR + ADC10IE; /*� �����������*/ \
  ADC10CTL1 = INCH2; \
  ADC10AE0 = BIT4; \
  ADC10CTL0 |= ENC; \
}


// ��������� ���������
// #ifdef use_Constant_FreqCPU
// void Syst_InitOnStart()
#define Syst_InitOnStart() \
  { \
    Rus1_InitOnStart(); \
    /*������� uart*/ \
    Syst_SetupUart(); \
    /*������� ����� ���������/���������� ������� ��� gsm-������*/ \
    dSyst_SetupPowerGsm; \
    /*������� ����� ���������/���������� ������� ��� ������� 220�*/ \
    dSyst_SetupPower220; \
    /*������� ����� ��� ��������� ���������� � ������� 220�*/ \
    dSyst_SetupMeasure220; \
    /*������� ����� ������ ��������� �������*/ \
    dBeep_Setup; \
  }
// #endif


  #define dGreenLed_Bit BIT6
  #define dRedLed_Bit BIT7
  #define dLedsSetup { P2SEL &= ~(dGreenLed_Bit + dRedLed_Bit); \
                            P2SEL &= ~(dGreenLed_Bit + dRedLed_Bit); \
                            P2DIR |= (dGreenLed_Bit + dRedLed_Bit); }
// ������ ���������
  #define isGreenLed_On   ( P2OUT | dGreenLed_Bit )
  #define dGreenLed_On { P2OUT |= dGreenLed_Bit; }
  #define dGreenLed_Off { P2OUT &= ~dGreenLed_Bit; }
  #define dGreenLed_Invert { P2OUT ^= (dGreenLed_Bit); }
// ������� ���������
  #define isRedLed_On   ( P2OUT | dRedLed_Bit )
  #define dRedLed_On   { P2OUT |= dRedLed_Bit;  }
  #define dRedLed_Off   { P2OUT &= ~dRedLed_Bit; }
  #define dRedLed_Invert   { P2OUT ^= (dRedLed_Bit); }
// ����� ������� + ������ ���������� // ��� �������� �����
  #define isRedOrGreenLed_On   ( P2OUT | (dRedLed_Bit | dGreenLed_Bit) )
  #define dRedAndGreenLed_On   { P2OUT |= (dRedLed_Bit | dGreenLed_Bit);  }
  #define dRedAndGreenLed_Off   { P2OUT &= ~(dRedLed_Bit | dGreenLed_Bit); }
  #define dRedAndGreenLed_Invert   { P2OUT ^= (dRedLed_Bit | dGreenLed_Bit); }

// ������� ������
//////////////// 
// ������� �������� ������� ��������?
#define isRedLedSignal ( 0 != led.f_RedSignal_100msCnt )
// ��������� ��������
#define dDecRedLedSignalCnt { if (led.f_RedSignal_100msCnt) led.f_RedSignal_100msCnt--; }
// �������� ������� ��������� ��� ������ �������
#define dRedLedSignal_ON { dRedLed_On; if (led.f_RedSignal_100msCnt < redSignal_100ms) led.f_RedSignal_100msCnt = redSignal_100ms; }
// �������� ������� ��������� �� �������� ���������� 100��
#define dRedLed_100msOn(x) { dRedLed_On; if (led.f_RedSignal_100msCnt < x) led.f_RedSignal_100msCnt = x; }
// �������� ������� ��������� � ���������� � ���� �������� ���������� 100�� � ����� ������
#define dRedLedMust_100msOn(x) { dRedLed_On; led.f_RedSignal_100msCnt = x; }
// ������ ����������� �����������
#define dRedSignal_0 { led.f_RedSignal_100msCnt = 0; }

// ������ ������
//////////////// 
#define getGreenLedSignalCnt led.f_GreenSignal_100msCnt
// ������� ������� ������� ��������?
#define isGreenLedSignal ( 0 != led.f_GreenSignal_100msCnt )
// ��������� ��������
#define dDecGreenLedSignalCnt { if (led.f_GreenSignal_100msCnt) led.f_GreenSignal_100msCnt--; }
// �������� ������ ��������� �� �������� ���������� 100��
#define dGreenLed_100msOn(x) { dGreenLed_On; if (led.f_GreenSignal_100msCnt < x) led.f_GreenSignal_100msCnt = x; }
// �������� ������ ��������� � ���������� � ���� �������� ���������� 100�� � ����� ������
#define dGreenLedMust_100msOn(x) { dGreenLed_On; led.f_GreenSignal_100msCnt = x; }
// ������ ����������� �����������
#define dGreenSignal_0 { led.f_GreenSignal_100msCnt = 0; }


////////////////////// 
// extern
//////////////////// 

// ������� // ������� ������ �������� �� ��������� ������, ��� ��������
extern unsigned long restart220_seconds;

// ������� ������������
extern t_LedSignal led;

// ������� �� ���������� 220�
extern t_Power220_Alarm power220_Alarm;




//////////////////////////////// 
// ��������� �������
//////////////////////////////// 


// �������� ������ ��������� ��� ������ �������
void GreenLedSignal_ON();

// ������� ����� ���������/���������� ������� ��� gsm-������
void Syst_SetupPowerGsm();

// ������� ����� ���������/���������� ������� ��� ������� 220�
// void Syst_SetupPower220();
/*
#ifdef use_Constant_FreqCPU
void Syst_SetupUart();
#else // #ifdef use_Constant_FreqCPU
// ������� uart
//                   ��������, ���/�      UCOS16
bool Syst_SetupUart(unsigned long speed, bool ucos16);
#endif

// ��������� ��������� (�������)
#ifndef use_Constant_FreqCPU
void Syst_InitOnStart();
#endif
*/

#endif // #ifndef __peripherals__