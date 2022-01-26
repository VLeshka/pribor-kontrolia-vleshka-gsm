
// .h-файл модуля для работы с периферией

#ifndef __rus1_peripherals__
#define __rus1_peripherals__

#include "rus1_screenLCD_AC-162A.hpp"

#include "rus1_mainTitle.hpp"

// используем ли shift-кнопки
// #define use_ButtonsShift

// частота ЦПУ, МГц
///////////////////////////////////////////////////////

// если определена, то частота ЦПУ фиксированна
#define use_Constant_FreqCPU 

// частота по умолчанию
#define cpuFreqMHzDefault 16



// количество 100мс, для показа красного сигнала
#define redSignal_100ms 1
// количество 100мс, для показа зелёного сигнала
#define greenSignal_100ms 1



////////////////////////////// 
// LCD-дисплей
///////////////////////////////////////////////


///порт, к которому подключён LCD-дисплей// 
///////////////////////////////////////////
////// Port OUT
// Out
#define portLcdOutData P2OUT
// E
#define portLcdOutE P2OUT
// RS
#define portLcdOutRS P2OUT
///////Port DIR
#define portLcdDirData P2DIR
// E
#define portLcdDirE P2DIR
// RS
#define portLcdDirRS P2DIR
///////Port SEL
#define portLcdSelData P2SEL
// E
#define portLcdSelE P2SEL
// RS
#define portLcdSelRS P2SEL
///////Port IES
#define portLcdIesData P2IES
// E
#define portLcdIesE P2IES
// RS
#define portLcdIesRS P2IES
///////Port REN
#define portLcdRenData P2REN
///////Port IE
#define portLcdIeData P2IE
///////Port IFG
#define portLcdIfgData P2IFG
///////Port используемые ножки
// data
// используем только первые 4 бита порта portLcdOutData (7 6 5 4 3 2 1 0)
#define portLcdData (BIT0 + BIT1 + BIT2 + BIT3)
// RS
#define portLcdPinRS BIT4
// E
#define portLcdPinE BIT5



///////////////////////////////////////////
// частота таймера обработки LCD-модуля, МГц
#define GetCpuFreq ((unsigned long)GetCpuFreqMHz*1000000)
#define GetFreqLcdMHz GetCpuFreqMHz
#define GetFreqLcd GetCpuFreq




/////////////////////////////////////////////////
// TIMER для обслуживания LCD-дисплея (с кнопками)
/////////////////////////////////////////////////

// название таймера, какой используем
#define NameTimerForRus1 TIMER0_A0_VECTOR




// используем константы, если use_Constant_FreqCPU
#ifdef use_Constant_FreqCPU
  #define valTimerRus1 valTimerRus1_default
  #define f_cpuFreqMHz cpuFreqMHzDefault
#endif

// значение TACCR0
//  I.
// вычисляем так, чтобы частота обновления символа на дисплее была константой вне зависимости от частоты цпу
// частота обновления 10 кадров в секунду, за 1 кадр надо обновить (lcdColCount*lcdRowCount) символов, ещё 1 команда перехода на строку
// valTACCR0 по умолчанию
// для цпу 1МГц, 10 кадров/с, LCD 16*2 при отсутствии делителя таймера = (uint)2941,1764705882352941176470588235 тактов мкс
//  = 1000000/(10*(lcdColCount+1)*lcdRowCount)
#define valTimerRus1_IntervalFor1MHz_mks_asis ( 100000/((lcdColCount+1)*lcdRowCount) )
// II.
// выставляем делитель, чтобы (valTimerRus1_IntervalFor1MHz_mks_asis * 16) < 0xFFFF
#if (valTimerRus1_IntervalFor1MHz_mks_asis < (0xFFFF / 16 * 1))
  #define valTimerRus1_IntervalFor1MHz_mks (valTimerRus1_IntervalFor1MHz_mks_asis / 1) // делитель /1
  #define valTimerRus1_Delitel (ID_0)
#elif (valTimerRus1_IntervalFor1MHz_mks_asis < (0xFFFF / 16 * 2))
  #define valTimerRus1_IntervalFor1MHz_mks (valTimerRus1_IntervalFor1MHz_mks_asis / 2) // делитель /2
  #define valTimerRus1_Delitel (ID_1)
#elif (valTimerRus1_IntervalFor1MHz_mks_asis < (0xFFFF / 16 * 4))
  #define valTimerRus1_IntervalFor1MHz_mks (valTimerRus1_IntervalFor1MHz_mks_asis / 4) // делитель /4
  #define valTimerRus1_Delitel (ID_2)
#elif (valTimerRus1_IntervalFor1MHz_mks_asis < (0xFFFF / 16 * 8))
  #define valTimerRus1_IntervalFor1MHz_mks (valTimerRus1_IntervalFor1MHz_mks_asis / 8) // делитель /8
  #define valTimerRus1_Delitel (ID_3)
#endif

// с учётом частоты цпу при отсутствии делителя таймера
#define valTimerRus1_default ( cpuFreqMHzDefault * valTimerRus1_IntervalFor1MHz_mks )


#define GetCpuFreqMHz f_cpuFreqMHz
#define GetCpuFreq ((unsigned long)GetCpuFreqMHz*1000000)

  
  
//////////////////////////////////////////// 
// счётчики срабатываний прерывания таймера NameTimerForRus1
//////////////////////////////////////////// 
  
  
// иммтиация времени
// рассчёт для 1 с:
// в идеале таймер должен срабатывать каждые 2941,1764705882352941176470588235 мкс
// минимальный интервал отссчёта = 100 мс, при этом в идеале должно быть = 34,000000000000000000000000000006 срабатываний таймера
// относит. погрешность = 6*10^-5 %.
// при одной минуте, абсолют. погрешность = (176470588235,29411764705882352938 - 176460000000) = 10588235,29411764705882352938 мкс
//  = 10,59 с.
// поэтому отталкиваемся от большого интервала, рассчитанного на фикс. число срабатывания таймера Рус-1, по истечении которой всё сбрасываем.
// счётчик 2-байтный => макс 192749999,99999999999999999999997 мкс = 192,74999999999999999999999999997 с = 3,2124999999999999999999999999994 мин
// поэтому отталкиваемся от 1 минуты
// фикс. число срабатывания для 1 минуты = (uint)(60*1000*1000/2941,1764705882352941176470588235) = (uint)20400,000000000000000000000000004.
// фикс. число срабатывания для 100 миллисекунд = (uint)(34,000000000000000000000000000006)

// примечание:
// остаток от (число срабатывания для 1 минуты) / (число срабатывания для 1 миллисекунды) = 0 
//  => в ДАННОМ случае для точности хватит 100 мс, но будем отталкиваться от 1 минуты для универсальности

// количество срабатываний в прерывании таймера Рус1, равное 1 минуте
//////////////////////////////////////////////////////////////////// 
#define interrupts1min        ((unsigned int)( (unsigned long)60*1000*1000/(valTimerRus1_IntervalFor1MHz_mks) ))
  
// количество 100мс в 1 секунде
#define ms100inMin 10

// количество минут в 1 часе
#define interrupts1hour 60

// количество срабатываний в прерывании таймера Рус1, равное 100 миллисекунд = 34,000000000000000000000000000006
#define interrupts100millisec ((unsigned char)( interrupts1min/(60*10) ))

// сколько секунд для сбрасывания режима редактирования при неактивности пользвателя
#define cnt11s_ChangeOff_Max 5// 60
 
  
// работа с периферией по таймеру
//////////////////////////////// 
  
// количество срабатываний прерывания таймера Рус1, после которых изменяем переменную
// 100мс
// GetFreqCPU - 1000мс
// interruptsChangePerem по умолчанию
#define interruptsLcd_ChangePerem_Default interrupts100millisec
#define interruptsLcd_ChangePerem         interruptsLcd_ChangePerem_Default

// количество срабатываний в прерывании таймера Рус1, после которых изменяем vlTree.f_lev10
// интервал 1 cекунда
// interruptsLnChange по умолчанию
#define interruptsLcd_LnChange_Default    ((unsigned int)( interrupts1min/60 ))
#define interruptsLcd_LnChange            interruptsLcd_LnChange_Default
  
// для не-shift кнопок
// выдержка при срабатывании события нажатия кнопок
// для защиты от дребезга
// количество срабатываний в прерывании таймера Рус1 
// интервал 30 мс
// interruptsLcd_ButtonBounceDelay по умолчанию
// анг. bounce - дребезг
#ifdef use_ButtonsShift
  #define interruptsLcd_ButtonBounceDelay_Default ((unsigned int)(interrupts100millisec/3))
  #define interruptsLcd_ButtonBounceDelay         interruptsLcd_ButtonBounceDelay_Default
#else
  #define interruptsLcd_ButtonBounceDelay_Default ((unsigned int)(interrupts100millisec/3))
  #define interruptsLcd_ButtonBounceDelay         interruptsLcd_ButtonBounceDelay_Default
#endif

// для shift кнопок
// выдержка при срабатывании события нажатия кнопок
// для защиты от слишком быстрого последовательного срабатывания кнопки при её удержании
// количество срабатываний в прерывании таймера Рус1 
// интервал 30 мс
// interruptsLcd_ButtonPostDelay по умолчанию
#define interruptsLcd_ButtonPostDelay_Default ((unsigned int)(3*interrupts100millisec))
#define interruptsLcd_ButtonPostDelay         interruptsLcd_ButtonPostDelay_Default

#define dResetCnt1s_ChangeOff { cnt1s_ChangeOff = cnt11s_ChangeOff_Max; }
#define dStopCnt1s_ChangeOff { cnt1s_ChangeOff = 0; }

///////////////////////////
// работа с переменными f_
///////////////////////////
  
// f_timer1_A0counterChangePerem
#define GetTimerCounterChangePerem    f_timerCounterChangePerem
#define dIncTimerCounterChangePerem   { f_timerCounterChangePerem++; }
#define dResetTimerCounterChangePerem { f_timerCounterChangePerem=0; }
// f_timer1_A0counterLnChangeKeyPressed
#define GetTimerCounterLnChangeKeyPressed    f_timerCounterLnChangeKeyPressed
#define dIncTimerCounterLnChangeKeyPressed   { f_timerCounterLnChangeKeyPressed++; }
#define dResetTimerCounterLnChangeKeyPressed { f_timerCounterLnChangeKeyPressed = 0; }
// f_timer1_A0counterDeltaChangeKeyUnPressed
#define GetTimerCounterLnChangeKeyUnPressed    f_timerCounterLnChangeKeyUnPressed
#define dIncTimerCounterLnChangeKeyUnPressed   { f_timerCounterLnChangeKeyUnPressed++; }
#define dResetTimerCounterLnChangeKeyUnPressed { f_timerCounterLnChangeKeyUnPressed=0; }

#ifdef use_Constant_FreqCPU
  #define SetFreqCpuMHz( x )
#else
  #define SetFreqCpuMHz(val) { f_cpuFreqMHz = val; valTimerRus1 = (f_cpuFreqMHz * valTimerRus1_IntervalFor1MHz_mks); }
#endif


/////////////////////
// extern
///////////////

// счётчик срабатываний прерывания таймера lcd для получения 1 часа
extern unsigned char cntTIMER_1hour;
// счётчик срабатываний прерывания таймера lcd для получения 1 минуты
// ставим за 100 мс до interrupts1min, чтобы после включения прибора через 100мс инициализировать дисплей
// if (lcd_IsReInitLCD)
extern unsigned int cntTIMER_1min;
// счётчик срабатываний прерывания таймера lcd для получения 100 миллисекунд
extern unsigned char cntTIMER_100ms;
// счётчик срабатываний 100 миллисекунд для получения 1 секунды
extern unsigned char cnt100msTo_1s;

// счётчик срабатываний секунд для сбрасывания режима редактирования при неактивности пользвателя
extern unsigned char cnt1s_ChangeOff;

#ifdef use_Constant_FreqCPU
#else
  // для перспективы изменения частоты таймера NameTimerForRus1 на лету
  extern unsigned int valTimerRus1;
// если меняем частоту "на лету"
  extern unsigned char f_cpuFreqMHz;
#endif


// количество срабатываний в прерывании TIMER0_A0, для выдержки при приёме АТ-пакета
// для стабильного приёма по uart - > 32 миллисекунды, для теста при глючной связи - 16
#ifdef use_Constant_FreqCPU
  #define itterruptsIndRus1_AtPauseDelay (3*interrupts100millisec)
#else
  extern unsigned int itterruptsIndRus1_AtPauseDelay; // (0xFF); /*для перспективы изменения частоты таймера на лету*/
#endif
  
  
  
//////////////////////////////// 
// прототипы функций
//////////////////////////////// 


// настройки периферии (системы)
bool Rus1_InitOnStart( void );

// готовим LCD-дисплей
void Rus1_SetupLCD( void );
   
// запуск таймера TimerA_0
void Rus1_SetupTimerLcd();
  
  // делает анализ на исправность системы
bool Rus1_CheckGeneralOK( void );

#ifdef use_Constant_FreqCPU
  void Rus1_SetupFreqCPU_MSP430G();
#else
  void Rus1_SetupFreqCPU_MSP430G( unsigned char val = cpuFreqMHzDefault ); // поддерживает значения 1 8 12 16МГц
#endif

#endif // #ifndef __rus1_peripherals__