// ������ ��� �������� ����������

#define interrupts

#include "msp430G2553.h"

#include "gsm_mainTitle.hpp"
#include "rus1_mainTitle.hpp"

#include "gsm.hpp"
#include "example_options.hpp"
#include "peripherals.hpp"
#include "rus1_peripherals.hpp"
#include "rus1.hpp"
#include "example_base_strings.hpp"
#include "example_base_functions.hpp"
#include "gsmUartCommands.hpp"



#pragma vector = USCIAB0RX_VECTOR 
__interrupt void USCIAB0RX_VECTOR_PRE (void)
{    
// ���� ���������� UCAxRXIFG ��������������� ������ ��� ��� ����� ������� � �������� ��� � 
// ������� UCAxRXBUF. 
// ���� UCAxRXIFG ������������� ������������ ��� ������ �������� UCAxRXBUF.
  if (&receiveGsmPackage[receiveGsmPackageMax-1] >= pReceiveGsmPackageActive)
  {
	*pReceiveGsmPackageActive = UCA0RXBUF; 
    pReceiveGsmPackageActive++;
  }
  else
  {
	unsigned char c_tmp = UCA0RXBUF; 
    // ���� ����� ����������, ���������, ��� �� ���� ������� "\r\nOK\r\n" - �� ��� � ���� ������ �� ���������
	dSetWaitAnswer_Off; 
  }
  
  GreenLedSignal_ON();
  
  if (isWaitAnswer)
    {
      // ���� ��������� ������� "\r\nOK\r\n", �� ���������� ���� isWaitAnswer � �� ������ ������ ��� ������ itterruptsIndRus1_AtPauseDelay
      unsigned char* pLast = (pReceiveGsmPackageActive-1);
      if (receiveGsmPackageCount > 5)
        if (IsSimbolLineBreak(*pLast--))
        {
          if (CompareStrings(--pLast,str_OK,str_OK_Cnt))
              if (IsSimbolLineBreak(*(--pLast)))
              {
                modemAnswerCnt = 0;
                dSetWaitAnswer_Off;
              }
          pLast -= 4;
          if (CompareStrings(pLast,str_ERROR,str_ERROR_Cnt))
            // if (IsSimbolLineBreak(*(--pLast)))
              {
                // dSetWaitAnswer_Off;
                modemAnswerCnt++; // ��������� �������� ������
                if (modemAnswerCnt >= modemAnswerCntMax)
                {
                  // ������ ���� ���-����� �����������, �� ������������� �����
                  if (CompareStrings(&receiveGsmPackage[0], strCmdCpbs_SimCard, strCmdCpbs_SimCardCnt))
                  {
                    errorScreen.word = lines_sim_Card_Off;
                    dResetRezhimSpecMessageTime100msCnt;
                  }
                  else
                    RestartModem();                  
                }
                else
                  dResumeLastCommandFuncsSeries;
              }
        }
    }
}

/*
������ ��� ��� ���������� ���������� �� ����� < secondsPowerAlarmMax:
���������� 220� � 15� ���� ���� ������ 250�
���� ��������
������100,0

������ ��� ��� ���������� ���������� �� ����� > secondsPowerAlarmMax:
���������� ���� ������ 250�
���� ���������
���������� 220�
������100,0

������ ��� ��� �������������� ���������� ����� �����������, > secondsPowerAlarmMax, ���������� �������������:
���������� ���������������
���� ��������
���������� 220�
������100,0
*/
#pragma vector = ADC10_VECTOR 
__interrupt void ADC10_VECTOR_PRE (void)
{
  signed int res = ADC10MEM;  
  dVal220add( res );
  unsigned int now220 = GetVal220();    
  // ������� 220�
  if (isStartPowerAlarmBounceDelay)
  {
    dDecPowerAlarmBounceDelay;
    if (!isStartPowerAlarmBounceDelay)
    {
      // ����� �� ��������� �������� ���������� � sds, ����� � ������� ������ SdsDataUpdate() ��� �� ������ ����������
      if (isRelePower220_On) // ���� ��� �� ��������� - ������, ������������ ��������� ����������
      {
        if ( (now220 < nastr_DataMin220) & (0 != now220) )
        {
          sds.val220 = now220;
          sds.is200wasUp = false;        
          dBeepSignal_LongBad;
          dRelePower220_Off;
          dSetPower220_Alarm_PowerOff_On; // "�������� ��������� �������� ��-�� ������ ��� ���������� �� ������� �����������"        
          RezhimSpecMessage_On(lines_Power220_Alarm_PowerOff_Lower, 0, BIT2 | BIT5);      
        }
        else
          dStopSecondsPowerAlarm;
      }
      else // if (isRelePower220_On)
      {
        // ���� ���������� � �����, �� ������������� �������
		if (now220 < (nastr_DataMax220 - val220ustavka) ) 
        {
          RelePower220_On();
          // "�������� ��������� �������� ��-�� ������ ��� ���������� �� ������� �����������"
		  dSetPower220_Alarm_PowerOff_Off; 
          // ���������� �������� ������
		  dResetBeepMasIndex; 
          // ��������� ����� ������ � ���������� ����������
		  dStopSecondsPowerAlarm; 
        }
        else
        {
          // ����� ������ ������ ���� ��������� ��� � �������; ���������� ������� � ��� ��������
		  if (now220 <= nastr_DataMax220) 
            now220 = nastr_DataMax220+1;
          sds.val220 = now220;
          sds.is200wasUp = true;      
        }
      }          
    } // if (isStartPowerAlarmBounceDelay)
    return;
  }
  
  // ���� ��� �� ��������� �������
  if (!isPower220_Alarm_PowerOff)
  {
    if (res > (adc220_max/*+adc220ustavka*/))
    {
      dRelePower220_Off;
      // "�������� ��������� �������� ��-�� ������ ��� ���������� �� ������� �����������"
	  dSetPower220_Alarm_PowerOff_On; 
      dBeepSignal_LongBad;
      // ������ ����� ������ � ���������� ����������
	  dStartSecondsPowerAlarm; 
      // ���� �������� 220�
	  dStartPowerAlarmBounceDelay; 
      RezhimSpecMessage_On(lines_Power220_Alarm_PowerOff_Upper, 0, BIT2 | BIT5);
    }
    else
      if (res < adc220_min)
      {
        // ������ ����� ������ � ���������� ����������
		dStartSecondsPowerAlarm; 
        // ���� �������� 220�
		dStartPowerAlarmBounceDelay; 
      }
  }
  // �����, ���� ������� ��� ���������
  else // if (!isPower220_Alarm_PowerOff)
  {
  if ( (now220 < (nastr_DataMax220-val220ustavka)) &
    (now220 > (nastr_DataMin220+val220ustavka)) )
    {
      dDecPowerAlarmNormDelay;
      if (!isStartPowerAlarmNormDelay)
      {
      // ���� ���� ��������� ��������� � ��������� �������������, ���������� ���
      if (bookNumbersCount) // ���� ���� ���� ����������
        if (nastr_IndexSendSmsOnBad220)
        {
          sds.val220 = now220;
          dSetNeedSendSmsOnPower220_Alarm_PowerOff_On;
        }
      RelePower220_On();
      dBeepSignal_LongOk;
      dSetPower220_Alarm_PowerOff_Off;
      RezhimSpecMessage_On(lines_Power220_Norm, 100, BIT0 | BIT5);
      }
    }
    else
      dStartPowerAlarmNormDelay;
  }
}