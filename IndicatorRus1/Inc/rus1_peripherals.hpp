
// .h-���� ������ ��� ������ � ����������

#ifndef __rus1_peripherals__
#define __rus1_peripherals__

#include "rus1_screenLCD_AC-162A.hpp"

#include "rus1_mainTitle.hpp"

// ���������� �� shift-������
// #define use_ButtonsShift

// ������� ���, ���
///////////////////////////////////////////////////////

// ���� ����������, �� ������� ��� ������������
#define use_Constant_FreqCPU 

// ������� �� ���������
#define cpuFreqMHzDefault 16



// ���������� 100��, ��� ������ �������� �������
#define redSignal_100ms 1
// ���������� 100��, ��� ������ ������� �������
#define greenSignal_100ms 1



////////////////////////////// 
// LCD-�������
///////////////////////////////////////////////


///����, � �������� ��������� LCD-�������// 
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
///////Port ������������ �����
// data
// ���������� ������ ������ 4 ���� ����� portLcdOutData (7 6 5 4 3 2 1 0)
#define portLcdData (BIT0 + BIT1 + BIT2 + BIT3)
// RS
#define portLcdPinRS BIT4
// E
#define portLcdPinE BIT5



///////////////////////////////////////////
// ������� ������� ��������� LCD-������, ���
#define GetCpuFreq ((unsigned long)GetCpuFreqMHz*1000000)
#define GetFreqLcdMHz GetCpuFreqMHz
#define GetFreqLcd GetCpuFreq




/////////////////////////////////////////////////
// TIMER ��� ������������ LCD-������� (� ��������)
/////////////////////////////////////////////////

// �������� �������, ����� ����������
#define NameTimerForRus1 TIMER0_A0_VECTOR




// ���������� ���������, ���� use_Constant_FreqCPU
#ifdef use_Constant_FreqCPU
  #define valTimerRus1 valTimerRus1_default
  #define f_cpuFreqMHz cpuFreqMHzDefault
#endif

// �������� TACCR0
//  I.
// ��������� ���, ����� ������� ���������� ������� �� ������� ���� ���������� ��� ����������� �� ������� ���
// ������� ���������� 10 ������ � �������, �� 1 ���� ���� �������� (lcdColCount*lcdRowCount) ��������, ��� 1 ������� �������� �� ������
// valTACCR0 �� ���������
// ��� ��� 1���, 10 ������/�, LCD 16*2 ��� ���������� �������� ������� = (uint)2941,1764705882352941176470588235 ������ ���
//  = 1000000/(10*(lcdColCount+1)*lcdRowCount)
#define valTimerRus1_IntervalFor1MHz_mks_asis ( 100000/((lcdColCount+1)*lcdRowCount) )
// II.
// ���������� ��������, ����� (valTimerRus1_IntervalFor1MHz_mks_asis * 16) < 0xFFFF
#if (valTimerRus1_IntervalFor1MHz_mks_asis < (0xFFFF / 16 * 1))
  #define valTimerRus1_IntervalFor1MHz_mks (valTimerRus1_IntervalFor1MHz_mks_asis / 1) // �������� /1
  #define valTimerRus1_Delitel (ID_0)
#elif (valTimerRus1_IntervalFor1MHz_mks_asis < (0xFFFF / 16 * 2))
  #define valTimerRus1_IntervalFor1MHz_mks (valTimerRus1_IntervalFor1MHz_mks_asis / 2) // �������� /2
  #define valTimerRus1_Delitel (ID_1)
#elif (valTimerRus1_IntervalFor1MHz_mks_asis < (0xFFFF / 16 * 4))
  #define valTimerRus1_IntervalFor1MHz_mks (valTimerRus1_IntervalFor1MHz_mks_asis / 4) // �������� /4
  #define valTimerRus1_Delitel (ID_2)
#elif (valTimerRus1_IntervalFor1MHz_mks_asis < (0xFFFF / 16 * 8))
  #define valTimerRus1_IntervalFor1MHz_mks (valTimerRus1_IntervalFor1MHz_mks_asis / 8) // �������� /8
  #define valTimerRus1_Delitel (ID_3)
#endif

// � ������ ������� ��� ��� ���������� �������� �������
#define valTimerRus1_default ( cpuFreqMHzDefault * valTimerRus1_IntervalFor1MHz_mks )


#define GetCpuFreqMHz f_cpuFreqMHz
#define GetCpuFreq ((unsigned long)GetCpuFreqMHz*1000000)

  
  
//////////////////////////////////////////// 
// �������� ������������ ���������� ������� NameTimerForRus1
//////////////////////////////////////////// 
  
  
// ��������� �������
// ������� ��� 1 �:
// � ������ ������ ������ ����������� ������ 2941,1764705882352941176470588235 ���
// ����������� �������� �������� = 100 ��, ��� ���� � ������ ������ ���� = 34,000000000000000000000000000006 ������������ �������
// �������. ����������� = 6*10^-5 %.
// ��� ����� ������, �������. ����������� = (176470588235,29411764705882352938 - 176460000000) = 10588235,29411764705882352938 ���
//  = 10,59 �.
// ������� ������������� �� �������� ���������, ������������� �� ����. ����� ������������ ������� ���-1, �� ��������� ������� �� ����������.
// ������� 2-������� => ���� 192749999,99999999999999999999997 ��� = 192,74999999999999999999999999997 � = 3,2124999999999999999999999999994 ���
// ������� ������������� �� 1 ������
// ����. ����� ������������ ��� 1 ������ = (uint)(60*1000*1000/2941,1764705882352941176470588235) = (uint)20400,000000000000000000000000004.
// ����. ����� ������������ ��� 100 ����������� = (uint)(34,000000000000000000000000000006)

// ����������:
// ������� �� (����� ������������ ��� 1 ������) / (����� ������������ ��� 1 ������������) = 0 
//  => � ������ ������ ��� �������� ������ 100 ��, �� ����� ������������� �� 1 ������ ��� ���������������

// ���������� ������������ � ���������� ������� ���1, ������ 1 ������
//////////////////////////////////////////////////////////////////// 
#define interrupts1min        ((unsigned int)( (unsigned long)60*1000*1000/(valTimerRus1_IntervalFor1MHz_mks) ))
  
// ���������� 100�� � 1 �������
#define ms100inMin 10

// ���������� ����� � 1 ����
#define interrupts1hour 60

// ���������� ������������ � ���������� ������� ���1, ������ 100 ����������� = 34,000000000000000000000000000006
#define interrupts100millisec ((unsigned char)( interrupts1min/(60*10) ))

// ������� ������ ��� ����������� ������ �������������� ��� ������������ �����������
#define cnt11s_ChangeOff_Max 5// 60
 
  
// ������ � ���������� �� �������
//////////////////////////////// 
  
// ���������� ������������ ���������� ������� ���1, ����� ������� �������� ����������
// 100��
// GetFreqCPU - 1000��
// interruptsChangePerem �� ���������
#define interruptsLcd_ChangePerem_Default interrupts100millisec
#define interruptsLcd_ChangePerem         interruptsLcd_ChangePerem_Default

// ���������� ������������ � ���������� ������� ���1, ����� ������� �������� vlTree.f_lev10
// �������� 1 c������
// interruptsLnChange �� ���������
#define interruptsLcd_LnChange_Default    ((unsigned int)( interrupts1min/60 ))
#define interruptsLcd_LnChange            interruptsLcd_LnChange_Default
  
// ��� ��-shift ������
// �������� ��� ������������ ������� ������� ������
// ��� ������ �� ��������
// ���������� ������������ � ���������� ������� ���1 
// �������� 30 ��
// interruptsLcd_ButtonBounceDelay �� ���������
// ���. bounce - �������
#ifdef use_ButtonsShift
  #define interruptsLcd_ButtonBounceDelay_Default ((unsigned int)(interrupts100millisec/3))
  #define interruptsLcd_ButtonBounceDelay         interruptsLcd_ButtonBounceDelay_Default
#else
  #define interruptsLcd_ButtonBounceDelay_Default ((unsigned int)(interrupts100millisec/3))
  #define interruptsLcd_ButtonBounceDelay         interruptsLcd_ButtonBounceDelay_Default
#endif

// ��� shift ������
// �������� ��� ������������ ������� ������� ������
// ��� ������ �� ������� �������� ����������������� ������������ ������ ��� � ���������
// ���������� ������������ � ���������� ������� ���1 
// �������� 30 ��
// interruptsLcd_ButtonPostDelay �� ���������
#define interruptsLcd_ButtonPostDelay_Default ((unsigned int)(3*interrupts100millisec))
#define interruptsLcd_ButtonPostDelay         interruptsLcd_ButtonPostDelay_Default

#define dResetCnt1s_ChangeOff { cnt1s_ChangeOff = cnt11s_ChangeOff_Max; }
#define dStopCnt1s_ChangeOff { cnt1s_ChangeOff = 0; }

///////////////////////////
// ������ � ����������� f_
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

// ������� ������������ ���������� ������� lcd ��� ��������� 1 ����
extern unsigned char cntTIMER_1hour;
// ������� ������������ ���������� ������� lcd ��� ��������� 1 ������
// ������ �� 100 �� �� interrupts1min, ����� ����� ��������� ������� ����� 100�� ���������������� �������
// if (lcd_IsReInitLCD)
extern unsigned int cntTIMER_1min;
// ������� ������������ ���������� ������� lcd ��� ��������� 100 �����������
extern unsigned char cntTIMER_100ms;
// ������� ������������ 100 ����������� ��� ��������� 1 �������
extern unsigned char cnt100msTo_1s;

// ������� ������������ ������ ��� ����������� ������ �������������� ��� ������������ �����������
extern unsigned char cnt1s_ChangeOff;

#ifdef use_Constant_FreqCPU
#else
  // ��� ����������� ��������� ������� ������� NameTimerForRus1 �� ����
  extern unsigned int valTimerRus1;
// ���� ������ ������� "�� ����"
  extern unsigned char f_cpuFreqMHz;
#endif


// ���������� ������������ � ���������� TIMER0_A0, ��� �������� ��� ����� ��-������
// ��� ����������� ����� �� uart - > 32 ������������, ��� ����� ��� ������� ����� - 16
#ifdef use_Constant_FreqCPU
  #define itterruptsIndRus1_AtPauseDelay (3*interrupts100millisec)
#else
  extern unsigned int itterruptsIndRus1_AtPauseDelay; // (0xFF); /*��� ����������� ��������� ������� ������� �� ����*/
#endif
  
  
  
//////////////////////////////// 
// ��������� �������
//////////////////////////////// 


// ��������� ��������� (�������)
bool Rus1_InitOnStart( void );

// ������� LCD-�������
void Rus1_SetupLCD( void );
   
// ������ ������� TimerA_0
void Rus1_SetupTimerLcd();
  
  // ������ ������ �� ����������� �������
bool Rus1_CheckGeneralOK( void );

#ifdef use_Constant_FreqCPU
  void Rus1_SetupFreqCPU_MSP430G();
#else
  void Rus1_SetupFreqCPU_MSP430G( unsigned char val = cpuFreqMHzDefault ); // ������������ �������� 1 8 12 16���
#endif

#endif // #ifndef __rus1_peripherals__