// ������ ��� ������ � ����������

#define rus1_peripherals

#include "msp430G2553.h"

/////////////////////////////////////
// ����� ������������, ��� ������ � LCD-������� 
// ������������ ������:
//  - ����������� ������ �� IAR;
//  - ������ base_functions;
/////////////////////////////////////

#include "rus1_mainTitle.hpp"
#include "example_base_functions.hpp"
#include "rus1_screenLCD_AC-162A.hpp"
#include "rus1_indicatorRUS1.hpp"
#include "rus1_peripherals.hpp"
#include "peripherals.hpp"

/////////////////////////////////////////////////
// TIMER ��������� LCD-������� (����� ������� lcd)
/////////////////////////////////////////////////

// ������� ������������ ���������� ������� lcd ��� ��������� 1 ����
unsigned char cntTIMER_1hour = 0; 
// ������� ������������ ���������� ������� lcd ��� ��������� 1 ������
// ������ �� 100 �� �� interrupts1min, ����� ����� ��������� ������� ����� 100�� �������������������� �������
// if (lcd_IsReInitLCD)
unsigned int cntTIMER_1min = (interrupts1min - (unsigned int)(100/2.4));
// ������� ������������ ���������� ������� lcd ��� ��������� 100 �����������
unsigned char cntTIMER_100ms = 0; 
// ������� ������������ 100 ����������� ��� ��������� 1 �������
unsigned char cnt100msTo_1s = 0; 

// ������� ������������ ������ ��� ����������� ������ �������������� ��� ������������ �����������
unsigned char cnt1s_ChangeOff = 0;

#ifdef use_Constant_FreqCPU
#else
  // ��� ����������� ��������� ������� ������� NameTimerForRus1 �� ����
  unsigned int valTimerRus1;
// ���� ������ ������� "�� ����"
  unsigned char f_cpuFreqMHz;
#endif


// ���������� ������������ � ���������� TIMER0_A0, ��� �������� ��� ����� ��-������
// ��� ����������� ����� �� uart - > 32 ������������, ��� ����� ��� ������� ����� - 16
#ifdef use_Constant_FreqCPU
#else
  unsigned int itterruptsIndRus1_AtPauseDelay = (2*interrupts100millisec); // (0xFF); /*��� ����������� ��������� ������� ������� �� ����*/
#endif


// ��������� ���������
bool Rus1_InitOnStart( void )
  {    
// ���� ������������ ���������� ������, ��� ��������� ������ ������ PUC
#ifdef use_WatchDog_for_PUC
    // ��������
    // �������� - SMCLK
    // ��������� ������ WDT+: ������� ��������� ������� /32768
    WDTCTL = (WDTPW+ WDTCNTCL); // +WDTIS0+WDTIS1
#endif
    
    // ����������    
    dLedsSetup;
    // �������� ������ ����������
    // dGreenLed_Off;
    // dRedLed_On;   

    // ������������� ������� ���
    Rus1_SetupFreqCPU_MSP430G();
    // ������ �� ����������� �������
    if ( (!Rus1_CheckGeneralOK()) 
      ) 
    {
      // ������ ��� �������� �� ����������� ������� ��� ��� ��������� ������� ��� -> ������ ������� ����������            
#ifndef use_WatchDog_for_PUC
      // ������� watchdog (��� ��������� ������� PUC) �������������
      WDTCTL = (WDTPW+ WDTCNTCL);
#endif
      // ���, ���� ��������� ���������� ������
      // � ��� ���� �������� ������ ������� �����������, ���� �� ����
      while (true)
      { 
        for (unsigned int ii=0; ii<0xFFFF; ii++); 
        dRedLed_Invert; 
      }            
    }
    
    // ������� LCD-�������
    Rus1_SetupLCD();  
    // LCD_Clear();
#ifdef use_IndRUS1
    dIndRus1_SetRezhimButton; // ������� ����� - �������� ������� ������
#endif
    
    // ������� ������ ��� ������ � ������������ ������ ���-1
    Rus1_SetupTimerLcd(); 
    
    return(true);
  }

   // ������� LCD-�������
void Rus1_SetupLCD( void )
  {
    // Dir
    portLcdDirData |= (portLcdData + portLcdPinRS + portLcdPinE);  // �������������� ������ 0 - 3 (4 ���� ����� �����-������), 4, 5
    // Out
    portLcdOutData &= ~(portLcdData + portLcdPinRS + portLcdPinE);  // ������ ��� ����
    // ��������
    portLcdRenData &= (~(portLcdData + portLcdPinRS + portLcdPinE)); /*��������� ��������*/ \
    // ����������
    portLcdIeData &= (~(portLcdData + portLcdPinRS + portLcdPinE)); /*��������� ����������*/ \
  }
    
  // ������ ��� �� �������, ���, � ����������� �� ��� ������� � ���� �������� 
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
  Rus1_SetupTimerLcd(); /*��������������� ������*/
}
#else
bool Rus1_SetupFreqCPU_MSP430G(unsigned char val) // ������������ �������� 1 8 12 16���
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
      default: /*���� �������� �� ��������������, ������ �� 1 ��� � ���������� false*/
        BCSCTL1 = CALBC1_1MHZ;
        DCOCTL = CALDCO_1MHZ;  
        SetFreqCpuMHz(1); /*��� ����������� ��������� ������� ��� �� ����*/
        return(false);
        break;
    }
    SetFreqCpuMHz(val); /*��� ����������� ��������� ������� ��� �� ����*/
    Rus1_SetupTimerLcd(); /*��������������� ������*/
    return(true);
  }
#endif

  // ������ ������� �� ����� �������� �� ������ �� �������
void Rus1_SetupTimerLcd()
// #define Rus1_SetupTimerLcd()
  { \
    /*������� ������ ���1*/ \
    TACCTL0 = CCIE; /*���������� ���������� �������/��������� ��������-������� A *//* Timer0_A3 Capture/Compare Control 0 */ \
    TACCR0 = (unsigned int)valTimerRus1; \
    TA0CTL = TASSEL_2 \
             + MC_1 \
             + valTimerRus1_Delitel \
             ; \
  }
  
  // ������ ������ �� ����������� �������
bool Rus1_CheckGeneralOK( void )
  {    
    /////////////////////
    // ���� ���������� XT2
    /////////////////////
    
    // ���� ��������� ���� MSP430F5529
    // ������ ������ UCSCTL7:
    // XT2OFFG - XT2 oscillator fault flag
    
    // ���� ��������� ���� MSP430430G2
    // ������ ������ XT2OF �������� BCSCTL3:
    
    return(true);
  }
  