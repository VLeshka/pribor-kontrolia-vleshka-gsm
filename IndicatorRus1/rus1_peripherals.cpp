// модуль для работы с периферией

#define rus1_peripherals

#include "msp430G2553.h"

/////////////////////////////////////
// здесь подпрограммы, при работе с LCD-экраном 
// использующие только:
//  - стандартные модули из IAR;
//  - модуль base_functions;
/////////////////////////////////////

#include "rus1_mainTitle.hpp"
#include "example_base_functions.hpp"
#include "rus1_screenLCD_AC-162A.hpp"
#include "rus1_indicatorRUS1.hpp"
#include "rus1_peripherals.hpp"
#include "peripherals.hpp"

/////////////////////////////////////////////////
// TIMER обработки LCD-дисплея (далее таймера lcd)
/////////////////////////////////////////////////

// счётчик срабатываний прерывания таймера lcd для получения 1 часа
unsigned char cntTIMER_1hour = 0; 
// счётчик срабатываний прерывания таймера lcd для получения 1 минуты
// ставим за 100 мс до interrupts1min, чтобы после включения прибора через 100мс переинициализировать дисплей
// if (lcd_IsReInitLCD)
unsigned int cntTIMER_1min = (interrupts1min - (unsigned int)(100/2.4));
// счётчик срабатываний прерывания таймера lcd для получения 100 миллисекунд
unsigned char cntTIMER_100ms = 0; 
// счётчик срабатываний 100 миллисекунд для получения 1 секунды
unsigned char cnt100msTo_1s = 0; 

// счётчик срабатываний секунд для сбрасывания режима редактирования при неактивности пользвателя
unsigned char cnt1s_ChangeOff = 0;

#ifdef use_Constant_FreqCPU
#else
  // для перспективы изменения частоты таймера NameTimerForRus1 на лету
  unsigned int valTimerRus1;
// если меняем частоту "на лету"
  unsigned char f_cpuFreqMHz;
#endif


// количество срабатываний в прерывании TIMER0_A0, для выдержки при приёме АТ-пакета
// для стабильного приёма по uart - > 32 миллисекунды, для теста при глючной связи - 16
#ifdef use_Constant_FreqCPU
#else
  unsigned int itterruptsIndRus1_AtPauseDelay = (2*interrupts100millisec); // (0xFF); /*для перспективы изменения частоты таймера на лету*/
#endif


// настройки периферии
bool Rus1_InitOnStart( void )
  {    
// если используется сторожевой таймер, для генерации сигнал сброса PUC
#ifdef use_WatchDog_for_PUC
    // включить
    // источник - SMCLK
    // интервала модуля WDT+: Частота тактового сигнала /32768
    WDTCTL = (WDTPW+ WDTCNTCL); // +WDTIS0+WDTIS1
#endif
    
    // светодиоды    
    dLedsSetup;
    // зажигаем нужные светодиоды
    // dGreenLed_Off;
    // dRedLed_On;   

    // устанавливаем частоту ЦПУ
    Rus1_SetupFreqCPU_MSP430G();
    // анализ на исправность системы
    if ( (!Rus1_CheckGeneralOK()) 
      ) 
    {
      // ошибка при проверке на исправность системы или при установке частоты ЦПУ -> работа прибора невозможна            
#ifndef use_WatchDog_for_PUC
      // заводим watchdog (для получения сигнала PUC) принудительно
      WDTCTL = (WDTPW+ WDTCNTCL);
#endif
      // ждём, пока сработает сторожевой таймер
      // и при этом пытаемся мигать красным светодиодом, если он есть
      while (true)
      { 
        for (unsigned int ii=0; ii<0xFFFF; ii++); 
        dRedLed_Invert; 
      }            
    }
    
    // готовим LCD-дисплей
    Rus1_SetupLCD();  
    // LCD_Clear();
#ifdef use_IndRUS1
    dIndRus1_SetRezhimButton; // рабочий режим - ожидание нажатия кнопки
#endif
    
    // готовим таймер для работы с индикаторной платой РУС-1
    Rus1_SetupTimerLcd(); 
    
    return(true);
  }

   // готовим LCD-дисплей
void Rus1_SetupLCD( void )
  {
    // Dir
    portLcdDirData |= (portLcdData + portLcdPinRS + portLcdPinE);  // инициализируем выходы 0 - 3 (4 бита линии ввода-вывода), 4, 5
    // Out
    portLcdOutData &= ~(portLcdData + portLcdPinRS + portLcdPinE);  // ставим там нули
    // подтяжку
    portLcdRenData &= (~(portLcdData + portLcdPinRS + portLcdPinE)); /*запрещаем подтяжку*/ \
    // прерывания
    portLcdIeData &= (~(portLcdData + portLcdPinRS + portLcdPinE)); /*запрещаем прерывание*/ \
  }
    
  // ставит ЦПУ на частоту, МГц, и настраивает всё что связано с этой частотой 
#ifdef use_Constant_FreqCPU
void Rus1_SetupFreqCPU_MSP430G()
{
#if (GetCpuFreqMHz == 16)
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL = CALDCO_16MHZ; 
#elif (GetCpuFreqMHz == 12)
  BCSCTL1 = CALBC1_12MHZ;
  DCOCTL = CALDCO_12MHZ;  
#elif (GetCpuFreqMHz == 8)
  BCSCTL1 = CALBC1_8MHZ;
  DCOCTL = CALDCO_8MHZ; 
#elif (GetCpuFreqMHz == 1)
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;  
#endif
  Rus1_SetupTimerLcd(); /*перенастраиваем таймер*/
}
#else
bool Rus1_SetupFreqCPU_MSP430G(unsigned char val) // поддерживает значения 1 8 12 16МГц
  {
    switch (val)
    {
      case 1:         
        BCSCTL1 = CALBC1_1MHZ;
        DCOCTL = CALDCO_1MHZ;  
        break;
      case 8:      
        BCSCTL1 = CALBC1_8MHZ;
        DCOCTL = CALDCO_8MHZ;  
        break;
      case 12:      
        BCSCTL1 = CALBC1_12MHZ;
        DCOCTL = CALDCO_12MHZ;  
        break;
      case 16:      
        BCSCTL1 = CALBC1_16MHZ;
        DCOCTL = CALDCO_16MHZ;  
        break;
      default: /*если значение не поддерживается, ставим на 1 МГц и возаращаем false*/
        BCSCTL1 = CALBC1_1MHZ;
        DCOCTL = CALDCO_1MHZ;  
        SetFreqCpuMHz(1); /*для перспективы изменения частоты ЦПУ на лету*/
        return(false);
        break;
    }
    SetFreqCpuMHz(val); /*для перспективы изменения частоты ЦПУ на лету*/
    Rus1_SetupTimerLcd(); /*перенастраиваем таймер*/
    return(true);
  }
#endif

  // запуск таймера на вывод символов из буфера на дисплей
void Rus1_SetupTimerLcd()
// #define Rus1_SetupTimerLcd()
  { \
    /*готовим таймер Рус1*/ \
    TACCTL0 = CCIE; /*Разрешение прерывания захвата/сравнения счётчика-таймера A *//* Timer0_A3 Capture/Compare Control 0 */ \
    TACCR0 = (unsigned int)valTimerRus1; \
    TA0CTL = TASSEL_2 \
             + MC_1 \
             + valTimerRus1_Delitel \
             ; \
  }
  
  // делает анализ на исправность системы
bool Rus1_CheckGeneralOK( void )
  {    
    /////////////////////
    // если используем XT2
    /////////////////////
    
    // если семейство типа MSP430F5529
    // делаем анализ UCSCTL7:
    // XT2OFFG - XT2 oscillator fault flag
    
    // если семейство типа MSP430430G2
    // делаем анализ XT2OF регистра BCSCTL3:
    
    return(true);
  }
  