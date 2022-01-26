// модуль специализированной настройки системы

#ifndef __peripherals__
#define __peripherals__

#include "gsm_mainTitle.hpp"
#include "rus1_mainTitle.hpp"

// частота SMCLK
// #define f_SmclkFreq 16000000


// сигналы светодиодами
struct t_LedSignal
{
  // количество 100ms, для задержки красного сигнала
  unsigned int f_RedSignal_100msCnt;
  // количество 100ms, для задержки зелёного сигнала
  unsigned int f_GreenSignal_100msCnt;
};

// событие на отключение 220В
#define interruptsLcd_PowerAlarmBounceDelay_Default ((unsigned int)(interrupts100millisec))
struct t_Power220_Alarm
{
  // счётчик, секунд // сколько секунд осталось до того, чтобы послать смс "Напряжение выключено" заместо "Напряжение временно отключалось на хх секунд"
  unsigned int f_secondsPowerAlarm;
  // счётчик, 4мс // сколько прерываний таймера LCD пропускаем для защиты от дребезга 220В
  unsigned int f_interruptsLcd_PowerAlarmBounceDelay;
  // счётчик, 4мс // сколько прерываний таймера LCD пропускаем для того, чтобы убедиться, что нормализовалось 220В
  unsigned int f_interruptsLcd_PowerAlarmNormDelay;
};

//////////////////////////////// 
// скорость uart
#define uartSpeed 115200
// #define uartSpeed 57600
// #define uartSpeed 38400
// #define uartSpeed 19200
// #define uartSpeed 9600

// псевдофункции // для ускорения
/////////////////////

// ресет receiveGsmPackageCount
#define dResetReceiveGsmPackageCount { pReceiveGsmPackageActive = &receiveGsmPackage[0]; }

// питание gsm-модуля
  #define dGsmVcc_Bit BIT0
  #define isGsmVcc_On ( 0 != (P1OUT & dGsmVcc_Bit) )
  #define dGsmVcc_Setup { P1SEL &= ~(dGsmVcc_Bit); \
                        P1SEL2 &= ~(dGsmVcc_Bit); \
                        P1DIR |= dGsmVcc_Bit; \
                        dGsmVcc_Off; }
  #define dGsmVcc_On { P1OUT |= dGsmVcc_Bit; }
  #define dGsmVcc_Off { P1OUT &= ~dGsmVcc_Bit; }
  #define dGsmVcc_Invert { P1OUT ^= dGsmVcc_Bit; }


// напряжение 220В
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

// реле 220В
#define dRelePower220_Bit BIT3
#define isRelePower220_On ( 0 == (P1OUT & dRelePower220_Bit) )
#define dRelePower220_Setup { P1SEL &= ~(dRelePower220_Bit); \
                        P1SEL2 &= ~(dRelePower220_Bit); \
                        P1DIR |= dRelePower220_Bit; \
                        dRelePower220_Off; }
#define dRelePower220_Invert { P1OUT ^= dRelePower220_Bit; }

// звуковой сигнал
#define dBeep_Bit BIT5
#define dBeep_Setup { P1SEL &= ~(dBeep_Bit); \
                        P1SEL2 &= ~(dBeep_Bit); \
                        P1DIR |= dBeep_Bit; \
                        dBeep_Off; }
#define dBeep_On { if (nastr_IndexBeepOff) P1OUT |= dBeep_Bit; }
#define dBeep_Off { P1OUT &= ~dBeep_Bit; }


// готовим uart
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


// готовим ножку включения/выключения питания для gsm-модуля
// void Syst_SetupPowerGsm()
#define dSyst_SetupPowerGsm \
{ \
  /*готовим питание для gsm*/ \
  dGsmVcc_Setup; \
  dGsmVcc_Off; \
}

// готовим ножку включения/выключения питания для розетки 220В
// void Syst_SetupPower220()
#define dSyst_SetupPower220 \
{ \
  /*готовим питание для gsm*/ \
  dRelePower220_Setup; \
  RelePower220_On(); \
}

// готовим ножку измерения напряжения в розетке 220В
// void Syst_SetupMeasure220()
#define dSyst_SetupMeasure220 \
{ \
  ADC10CTL0 = ADC10SHT0 + ADC10ON + REF2_5V + REFON + SREF0 + ADC10SR + ADC10IE; /*с прерыванием*/ \
  ADC10CTL1 = INCH2; \
  ADC10AE0 = BIT4; \
  ADC10CTL0 |= ENC; \
}


// настройки периферии
// #ifdef use_Constant_FreqCPU
// void Syst_InitOnStart()
#define Syst_InitOnStart() \
  { \
    Rus1_InitOnStart(); \
    /*готовим uart*/ \
    Syst_SetupUart(); \
    /*готовим ножку включения/выключения питания для gsm-модуля*/ \
    dSyst_SetupPowerGsm; \
    /*готовим ножку включения/выключения питания для розетки 220В*/ \
    dSyst_SetupPower220; \
    /*готовим ножку для измерения напряжения в розетке 220В*/ \
    dSyst_SetupMeasure220; \
    /*готовим ножку подачи звукового сигнала*/ \
    dBeep_Setup; \
  }
// #endif


  #define dGreenLed_Bit BIT6
  #define dRedLed_Bit BIT7
  #define dLedsSetup { P2SEL &= ~(dGreenLed_Bit + dRedLed_Bit); \
                            P2SEL &= ~(dGreenLed_Bit + dRedLed_Bit); \
                            P2DIR |= (dGreenLed_Bit + dRedLed_Bit); }
// зелёный светодиод
  #define isGreenLed_On   ( P2OUT | dGreenLed_Bit )
  #define dGreenLed_On { P2OUT |= dGreenLed_Bit; }
  #define dGreenLed_Off { P2OUT &= ~dGreenLed_Bit; }
  #define dGreenLed_Invert { P2OUT ^= (dGreenLed_Bit); }
// красный светодиод
  #define isRedLed_On   ( P2OUT | dRedLed_Bit )
  #define dRedLed_On   { P2OUT |= dRedLed_Bit;  }
  #define dRedLed_Off   { P2OUT &= ~dRedLed_Bit; }
  #define dRedLed_Invert   { P2OUT ^= (dRedLed_Bit); }
// сразу красный + зелёный светодиоды // для экономии места
  #define isRedOrGreenLed_On   ( P2OUT | (dRedLed_Bit | dGreenLed_Bit) )
  #define dRedAndGreenLed_On   { P2OUT |= (dRedLed_Bit | dGreenLed_Bit);  }
  #define dRedAndGreenLed_Off   { P2OUT &= ~(dRedLed_Bit | dGreenLed_Bit); }
  #define dRedAndGreenLed_Invert   { P2OUT ^= (dRedLed_Bit | dGreenLed_Bit); }

// красный сигнал
//////////////// 
// счётчик красного сигнала работает?
#define isRedLedSignal ( 0 != led.f_RedSignal_100msCnt )
// декремент счётчика
#define dDecRedLedSignalCnt { if (led.f_RedSignal_100msCnt) led.f_RedSignal_100msCnt--; }
// включить красный светодиод для подачи сигнала
#define dRedLedSignal_ON { dRedLed_On; if (led.f_RedSignal_100msCnt < redSignal_100ms) led.f_RedSignal_100msCnt = redSignal_100ms; }
// включить красный светодиод на заданное количество 100мс
#define dRedLed_100msOn(x) { dRedLed_On; if (led.f_RedSignal_100msCnt < x) led.f_RedSignal_100msCnt = x; }
// включить красный светодиод и установить у него заданное количество 100мс в любом случае
#define dRedLedMust_100msOn(x) { dRedLed_On; led.f_RedSignal_100msCnt = x; }
// сигнал светодиодом бесконечный
#define dRedSignal_0 { led.f_RedSignal_100msCnt = 0; }

// зелёный сигнал
//////////////// 
#define getGreenLedSignalCnt led.f_GreenSignal_100msCnt
// счётчик зелёного сигнала работает?
#define isGreenLedSignal ( 0 != led.f_GreenSignal_100msCnt )
// декремент счётчика
#define dDecGreenLedSignalCnt { if (led.f_GreenSignal_100msCnt) led.f_GreenSignal_100msCnt--; }
// включить зелёный светодиод на заданное количество 100мс
#define dGreenLed_100msOn(x) { dGreenLed_On; if (led.f_GreenSignal_100msCnt < x) led.f_GreenSignal_100msCnt = x; }
// включить зелёный светодиод и установить у него заданное количество 100мс в любом случае
#define dGreenLedMust_100msOn(x) { dGreenLed_On; led.f_GreenSignal_100msCnt = x; }
// сигнал светодиодом бесконечный
#define dGreenSignal_0 { led.f_GreenSignal_100msCnt = 0; }


////////////////////// 
// extern
//////////////////// 

// счётчик // сколько секунд осталось до включения модема, при рестарте
extern unsigned long restart220_seconds;

// сигналы светодиодами
extern t_LedSignal led;

// событие на отключение 220В
extern t_Power220_Alarm power220_Alarm;




//////////////////////////////// 
// прототипы функций
//////////////////////////////// 


// включить зелёный светодиод для подачи сигнала
void GreenLedSignal_ON();

// готовим ножку включения/выключения питания для gsm-модуля
void Syst_SetupPowerGsm();

// готовим ножку включения/выключения питания для розетки 220В
// void Syst_SetupPower220();
/*
#ifdef use_Constant_FreqCPU
void Syst_SetupUart();
#else // #ifdef use_Constant_FreqCPU
// готовим uart
//                   скорость, бит/с      UCOS16
bool Syst_SetupUart(unsigned long speed, bool ucos16);
#endif

// настройки периферии (системы)
#ifndef use_Constant_FreqCPU
void Syst_InitOnStart();
#endif
*/

#endif // #ifndef __peripherals__