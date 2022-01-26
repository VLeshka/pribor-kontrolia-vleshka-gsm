//////////////////////////////////////////// 
// ������ ��� �������� ���������� �������
//////////////////////////////////////////// 

#define rus1_interruptLcdTimer

#include "msp430G2553.h"

#include "rus1_mainTitle.hpp"

#include "rus1_interruptLcdTimer.hpp"
#include "example_base_functions.hpp"
#include "vl_TreeStructureGenerator_Structure.hpp"
#include "rus1_indicatorRUS1.hpp"
#include "rus1_screenLCD_AC-162A.hpp"
#include "rus1_peripherals.hpp"
#include "peripherals.hpp"
#include "gsm.hpp"
#include "gsmUartCommands.hpp"
#include "example_base_strings.hpp"
#include "rus1_treenode_go.hpp"



//////////////////////////////////////////////////////////////////////// 
// itterruptsIndRus1_AtPauseDelay � funcsSeries.waitBetween_100ms
//////////////////////////////////////////////////////////////////////// 
// 
// 1. ����� �� ������ ��������� �����������, ���� ��������� ����� itterruptsIndRus1_AtPauseDelay ����� 
//    ����� ������� �� ������.
// 2. ����� ��������, ������������ ������, ������������� �������� funcsSeries.waitBetween_100ms.
// ����� �������� ������������� - itterruptsIndRus1_AtPauseDelay ��� funcsSeries.waitBetween_100ms - ������������ 
// �� ������� if (0 == receiveGsmPackageCount).



/*���������� �� NameTimerForLcd ���������� ���:
  - ����������� ������������� IndRus1_SetRezhimLCD/IndRus1_SetRezhimButton.
  - ������ �� ������������ (���� ������������ LaunchPad MSP-EXP430G2).
  - ���������������� ����������������� ������� ������ ������.
  - ������ � LCD-�������� (� ����������� "������" ����� ��� �������� ������� ��� ������ �� LCD-�������). 
  - ���������� ������ �� "��������" ������ (� ����������� "������" �����).
  - ���������� ������ �� "�������" ������� ������ (� ����������� "������" �����).
*/
#pragma vector = NameTimerForRus1
__interrupt void NameTimerForRus1_PRE (void)
{    
  /*��������� �� ������ ������������� ����������� �������:
  0xFFFF ������ ����������� ������� � ������ ������� �� ���������� ����� ���������� [��� ������������ 
  ��������� ������ �� � ������������ ������ ���-1] ������ �� ����� ������������� ������ ����������, 
  ��������� � ������� vl_TreeStructureGenerator_Functions.*.
  ������, ��� ��������� ��������� ��� ���������� ����� ����������� ������ ����������,
  ����� ������� ����������� ������ ���������� �� �����, ���������� ���������� ������.
  ����� ������������� ������� ���������, ��� �� ������������ ������ ����������, ������:
  - ��� ������������ �� ������,
  - ��� ���������� ���������� ������ (������� dWatchDogReset).
  � ��������� ������ ��������� ��������� ���������� �������� ������� PUC.*/
  
  ADC10CTL0 |= ADC10SC; // �������� ���������� �� �������� 220�

  //////////////////////////////////////////////////////////// 
  // ������� ��������� �� �������
  // ������ ����� ������������ ����������� ��������� ����������
  //////////////////////////////////////////////////////////// 
  // � �������� ����:
  // BIT0 - 10 ��
  // BIT1 - 100 ��
  // BIT2 - 1 �
  // BIT3 - 1 ���
  // BIT4 - 1 ���
  // BIT5 - 1 ���� = 24 ����
  // BIT6 - 1 �����
  // BIT7 - 1 ���
  unsigned char timeMask = 0;
  if (cntTIMER_1min >= interrupts1min) // ������� ������ // ������������� �� 1 ������
  {
    if ( !GetIsCursorVisiblePosition_Need )
        dSetIsReInitLCD_ON;
    if (cntTIMER_1hour >= interrupts1hour) // ������� ����
    {
      // �������� ����� ���
      cntTIMER_1hour = 0;
      timeMask |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    }
    else
    {
      cntTIMER_1hour++;
      timeMask |= (BIT0 + BIT1 + BIT2 + BIT3);
    }
    cntTIMER_1min = 0;
    cnt100msTo_1s = 0;
    cntTIMER_100ms = 0;
  }
  else
  {
    cntTIMER_1min++;
    // ������� 100 ��
    if (cntTIMER_100ms >= interrupts100millisec)
    {
      // ������� 100��
      cntTIMER_100ms = 0;
      
      // ������� �������
      if (cnt100msTo_1s >= ms100inMin) // ������� �����
      {
        // ������� �������
        timeMask |= (BIT0 + BIT1 + BIT2);
        cnt100msTo_1s = 0;        

#ifdef use_IndRUS1
//         if ( lcd_Is1s )
        /*
        // ������� ������������ ������������, ����� �������� �� ������ ��������������, � ��� ���������� �� ����� ���������
        if (cnt1s_ChangeOff) 
            cnt1s_ChangeOff--;
          if (!cnt1s_ChangeOff) // ������� �����
          {
            if (isRedLed_On)
            // if (isPower220_Alarm_PowerOff)
            {
              const unsigned char* strMessage;
              if (!sds.is200wasUp)
                strMessage = lines_Power220_Alarm_PowerOff_Lower;
              else
                strMessage = lines_Power220_Alarm_PowerOff_Upper;
              RezhimSpecMessage_On(strMessage, 0, 0);
            }
            else
              if (0 != GetRRP)
              {
                SetRRP(rrp_0);
                if (GetIsCursorVisible)
                  LCD_SetCursorBlinkOff(); // ���������� ����������, ������ �������� // ����� �� ��������������
              }
          }
*/
/*
        // ������� ������������ ������������, ����� �������� �� ������ ��������������
        if (0 != GetRRP)
        {
          if (cnt1s_ChangeOff) 
            cnt1s_ChangeOff--;
          if (!cnt1s_ChangeOff) // ������� �����
          {
            SetRRP(rrp_0);
            if (GetIsCursorVisible)
              LCD_SetCursorBlinkOff(); // ���������� ����������, ������ �������� // ����� �� ��������������
          }
        }
*/
        // ������� ������������ ������������, ����� �������� �� ������ ��������������
        if (cnt1s_ChangeOff) 
          cnt1s_ChangeOff--;
        if (!cnt1s_ChangeOff) // ������� �����
        {
          SetRRP(rrp_0);
          if (GetIsCursorVisible)
            LCD_SetCursorBlinkOff(); // ���������� ����������, ������ �������� // ����� �� ��������������
        }
#endif
        
        // ������� ������ �������� �� ��������� ������, ��� ���������� ������� ������������
        if (isPower220_PauseThenOn)
        {
          if (restart220_seconds)
            restart220_seconds--;
          if (!restart220_seconds)
          {
            dSetPower220_OffThenOn_Off;
            RelePower220_On();
            // RezhimSpecMessage_Off();
          }
        }
        
        // ������� ������ ������������ ���������� ��� ������ �������� ���        
        if (!isNeedSendSmsOnPower220_Alarm_PowerOff) // ���� ��� �� ���� ��������� ��� � ���������� ����������
          if (nastr_IndexSendSmsOnBad220) // ���� � ���������� ������� �������� ��� ��� ���������� 220�            
            if (isStartSecondsPowerAlarm) // ���� ����������� ����� �� ������� ������� ���, ����������� ���
            {
              dDecSecondsPowerAlarm;
              if (!isStartSecondsPowerAlarm)
                dSetNeedSendSmsOnPower220_Alarm_PowerOff_On; // � �������� �������� ���
            }
        
        // ��� ������������ ����
        // dRelePower220_Invert; // ������ ������� ���/���� ����
        
      } // if (cnt100msTo_1s >= ms100inMin)
      else    
      {
        timeMask |= (BIT0 + BIT1);
        cnt100msTo_1s++;        
      }

      // �������� �������� ��� ��� ���������� ������
      if (isNeedSendSmsOnPower220_Alarm_PowerOff)
        if (!isSendSmsToAllPhonesInBook)
          StartSendSmsToAllNumbers();
      
      // ����� �������������
      // if (isRezhimSpecMessage)
        if (isRezhimSpecMessage_Time100msShow)
        {
           dDecRezhimSpecMessageTime100msCnt;
           if (!isRezhimSpecMessage_Time100msShow)
           {
             if (isSpecMessage_ChangeToCritical_onZero)
             {
               dBeepSignal_LongBad;
               RezhimSpecMessage_On(errorScreen.word, 0, (BIT7 | BIT5 | BIT4)); // ������� ��������� � ����������� ������� � � ����� ��������
             }
             else
               RezhimSpecMessage_Off();
           }
        }

      // ���������� �����������
      ////////////////////////////// 
//       if (!isRezhimSpecMessage)
      {
        // ������� ���������
        if (isRedLedSignal)
        {
          dDecRedLedSignalCnt;
          if (!isRedLedSignal)
            dRedLed_Off;
        }
        // ������ ���������
        if (isGreenLedSignal)
        {
          dDecGreenLedSignalCnt;
          if (!isGreenLedSignal)
            dGreenLed_Off;
        }
      }
      
      // �������� ������
      if (isBeepMasSignal)
      {
        dDecBeepSignal_100msCnt_Wait;
        if (!isBeepSignal_100msCnt_Wait)
        {
          if (isEndBeepMasIndex)
          {
            dBeep_Off;
            dResetBeepMasIndex;
          }
          else
          {
            dSetBeepDelay100msIndex_Next;
            if ( beepDelay100msIndex & 0x01)
              dBeep_Off
            else
              dBeep_On;
          }
        }
      }
      
      // GSM
      ////////////////// 
      // ����������� �����
      if ( ( 0 == receiveGsmPackageCount ) | ( isWaitAnswer & receiveGsmPackageCount>0 ) )
      {
        dIncFromLastCommand100ms; // ��������� �������� 100�� ����� ��������� ������� ������
        if (!isGsmVcc_On) // ����� ���� �� ���������
        {
          // ����� ���� ��������
		  if (isModemNeedPowerOn) 
          {
            dGsmVcc_On;
            // ������� �� ������ ��������� "MODEM:STARTUP" 10 ������
            // ��������� � �� ��������� ������
			errorScreen.word = lines_GsmModem_Off; 
            RezhimSpecMessage_On(lines_ModemPowerOn, 100, (BIT6 | BIT5 | BIT0)); // �� ���������� 10� ��������� "����� �� ���������"
          }
        }
        else // if (!isGsmVcc_On) // ����� ���� �� ���������
        {        
          if (isWaitAnswer)
          {
            // ���� ����� ��� �� ���������� ������ ����� - ������ �� ����� � ��� ���� �������������
            // �������� �� ������ �������
			if (isEndWaitAnswerOfCommand_100ms) 
              RestartModem();
          }
          else // if (isWaitAnswer)        
            if (isExistsNextCommand)
            {
              if (dFuncsSeriesNeedStart)
                // ���������� ��������� ������� �� �������
			    dFuncsSeriesSendNextCommand; 
              if (isRezhimSpecMessage_OffOnEndFuncsSeries)
                // ���� ���� �������� ���������, ���������� ���
			    if (!isExistsNextCommand) 
                  dSetRezhimSpecMessage_OffOnEndFuncsSeries_Off;
            }
            else
              if (isSetupOnStartUp)
                if (!isFindOkNumberInBook)
                  // ���� � ������� �� �������� // �� ��������� ���
			      if (!isSendSmsToAllPhonesInBook) 
                    if (isGsmTestCommandNeed)
                    {
                      if (lcd_Is1hour)
                        // ��������� �����, ��� �� �� �����
					    dSetFuncsSeries_TestModem_PerHour; 
                      else
                        // ��������� �����, ��� �� �� �����
					    dSetFuncsSeries_TestModem_Often; 
                      // ����� �� ����� ��� 100�� �� ��������� �������, �������� �
					  // dFuncsSeriesSendNextCommand(); 
                    }
        } // if (!isGsmVcc_On) .. else
      } // if (0 == receiveGsmPackageCount)
      
    } // if (cntTIMER_100ms >= interrupts100millisec)
    else
      cntTIMER_100ms++;
  }
    
        
        /////////////////////////////////////////
        // ����������
        if ( lcd_Is1s )
        {
          // ���������� ������ ��������� ��� ������, ��� ������ �� �����
          if (!isWasTypeSpecMessage_CriticalError)
            if (isSetupOnStartUp)
              GreenLedSignal_ON();
          // ����������� �������
          if (isWasTypeSpecMessage_CriticalError)
            // ������� ��������� ������ �������� ���������
            dRedLed_Invert;
        }

#ifdef use_WatchDog_for_PUC
  dWatchDogReset; // ���������� ���������� ������
#endif

  //////////////////////////////////////// 
  // ����� ������� �� ����������, ���� ����
  //////////////////////////////////////// 
  // ���� ���-�� ������ � UART, ��������������� ������ �� ����.
  // � �� msp430G2553 ��������� ������� ���������� UART ������, ��� � �������� Timer0 � Timer1.
  if (IFG2 & UCA0RXIFG) return;
  
  //////////////////////////////////////////////////// 
  // ����������� ������
  /////////////////////
  
#ifdef use_IndRUS1

  // btnPressed - ����� ������� ������ � ������� interruptsLcd_ButtonPostDelay � ��������_�������_������
  // btnPressed - �� ��� �������������� ����������
  unsigned int btnPressed_Now = ((~indRus1_InPort) & indRus1_ButtonAll); // ��������� uint c ��������� ������� ������ // indRus1_InPort & (~indRus1_ButtonAll)
  
#ifdef use_ButtonsShift
///////////////////////////////////////////////////////////////////////////////////////////////
// �������� ��������� ������� ���������� ���������� ������
// ��� ����� ��������� ������ ��������� ����������� ����� ����� "�������������", ��� ����������� 
// ������� ���������� ������ - ����������� ������� "Shift-������".
// "Shift-������" ������������ ������ buttonDelayWaitFlagMask_Shift.
// ������� ������� "shift" � "��-Shift" ������ ���������� ��� ������ ��������:
//  - "shift-������" - ����� ��� ������� ������;
//  - "��-Shift-������" - ��� ���������� ������.
//////////////////////////////////////////////// 

  // ��������� ������� "��-shift" ������
  if ( 
     ( 0 != btnPressed_Now ) // ���� ������ �����-������ ������
     & ( 0 == ( btnPressed_Now & buttonBounceDelay_Shift ) ) // ���� �������� ��-shift-������
     & ( 0 == buttonMask_Pressed ) // ���� ������ ���������� � ������ ���
     )
  {
    if (isTypeSpecMessage_ChangeOff)
      dResetCnt1s_ChangeOff;
    if (isRezhimSpecMessage)
      RezhimSpecMessage_Off();
    else
      switch (btnPressed_Now)
      {
        case indRus1_ButtonUp: 
          dIndRUS1_PressButtonUp; // ��������� ��������� ������� ������� ������ "�����"
          break;
        case indRus1_ButtonDown: 
          dIndRUS1_PressButtonDown; // ��������� ��������� ������� ������� ������ "����"
          break;
        default:
          break;
      } // switch (buttonMask_Pressed)
    buttonMask_Pressed |= btnPressed_Now;
    interruptsIndRus1_ButtonCnt = 0;
  } // if ( btnPressed ) // ���� ������ �.-�. ������
  else // if ( ( 0 != btnPressed ) & ...
    // ��������� ������� shift-�������
    if ( btnPressed_Now & buttonBounceDelay_Shift )
      buttonMask_Pressed |= btnPressed_Now;    
    else // if ( btnPressed & buttonDelayWaitFlagMask_Shift )
      // ����������� ����������� ����� ����� ������� ������� ������.
      // � ������ � shift-�������, ����������� ����� ��� ������ �� �������� (��������� �������� ��).
      // � ������ � ��-shift ������, ������������� ���������� ����� ������ �� ������� �������� 
      // ����������������� ������������ ������ ��� � ��������� (��������� ����� ��).
      if (buttonMask_Pressed)
      {
        dResetCnt1s_ChangeOff;
        interruptsIndRus1_ButtonCnt++;
        if ( 0 == ( buttonMask_Pressed & buttonBounceAndPostDelayMask_Shift ) ) // ���� �� �������� shift-������
        {
          if (interruptsIndRus1_ButtonCnt >= interruptsLcd_ButtonPostDelay)
            dButtonMask_Pressed_Clear;
        } // if ( 0 == ( btnPressed & buttonDelayWaitFlagMask_Shift ) )
        else
          if (buttonPostDelay_Shift & buttonMask_Pressed) // ���� "����� �������" shift-������
          {
            if (interruptsIndRus1_ButtonCnt >= interruptsLcd_ButtonPostDelay)
              dButtonMask_Pressed_Clear;            
          }
          else // ������ ��� "�� �������" shift-������
            {
              if (isRezhimSpecMessage)
              {
     			// ���� �� "�������" ������� Esc � �������� �����, ������� �� �����6
				// + ������������, ���� ������ �� ������ Esc [���� ����� Enter]
                if (
                  ( ( 0 == ( buttonBounceDelay_Shift & buttonMask_Pressed) )
                    & (currentTreeNode == &treeNode_Root) ) 
                  | ( (~buttonBounceAndPostDelay_Esc) & buttonMask_Pressed) 
                  )
                  RezhimSpecMessage_Off();
              } // if (isRezhim6)
              else
                // ��������� ������� shift-������ [��������� � ������ �������]
                switch (buttonMask_Pressed)
                {
                  case indRus1_ButtonEnter: 
                    // ��������� ��������� ������� ������� ������ "Enter"
					dIndRUS1_PressButtonEnter; 
                    break;
                  case indRus1_ButtonEscape: 
                    // ��������� ��������� ������� ������� ������ "Esc"
					dIndRUS1_PressButtonEscape; 
                    break;  
                  // �������� ��������� ������� ����� ���������� ������
				  // ��������� ��������� ������� ������� ������ "�����" + "����" + "Enter"
                  case indRus1_ButtonUp + indRus1_ButtonDown + indRus1_ButtonEnter: 
//                    Tree_JumpToRootKatalog(); // Tree_JumpToRootKatalog - ������ ��� �������
//                    Rezhim6_On(LCD_ShowMsgDone);
                    break;
                  default:              
                    break;
                } // switch (buttonMask_Pressed)
              buttonMask_Pressed <<= 8;
              interruptsIndRus1_ButtonCnt = 0;
            } // if ( buttonMask_Pressed & buttonDelayWaitFlagMask_Shift ) // ���� � ����� Enter ��� Esc
          } // if (buttonMask_Pressed)
#else // #ifdef use_ButtonsShift
  if ( 
     ( 0 != btnPressed_Now ) // ���� ������ �����-������ ������
     & ( 0 == buttonMask_Pressed ) // ���� ������ ���������� � ������ ���
     )
  {
    if (isTypeSpecMessage_ChangeOff)
      dResetCnt1s_ChangeOff
    else
      RezhimSpecMessage_Off();
    buttonMask_Pressed = btnPressed_Now;
    interruptsIndRus1_ButtonCnt = 0;
  } // if ( btnPressed ) // ���� ������ �.-�. ������
  else // if ( ( 0 != btnPressed ) & ...
      if (buttonMask_Pressed)
      {
        if (isTypeSpecMessage_ChangeOff)
          dResetCnt1s_ChangeOff
        else
          RezhimSpecMessage_Off();
        interruptsIndRus1_ButtonCnt++;
        if (buttonMask_Pressed > 0xFF)
        {
          if (interruptsIndRus1_ButtonCnt >= interruptsLcd_ButtonPostDelay)
            dButtonMask_Pressed_Clear;
        }
        else
        if (interruptsIndRus1_ButtonCnt >= interruptsLcd_ButtonBounceDelay)
        {          
          if (isRezhimSpecMessage)
            RezhimSpecMessage_Off(); // dResetRezhimSpecMessageTime100msCnt
          else
            switch (buttonMask_Pressed)
            {
              case indRus1_ButtonUp: 
                // ��������� ��������� ������� ������� ������ "�����"
				dIndRUS1_PressButtonUp; 
                break;
              case indRus1_ButtonDown: 
                // ��������� ��������� ������� ������� ������ "����"
				dIndRUS1_PressButtonDown; 
                break;
              case indRus1_ButtonEnter: 
                // ��������� ��������� ������� ������� ������ "Enter"
				dIndRUS1_PressButtonEnter; 
                break;
              case indRus1_ButtonEscape: 
                // ��������� ��������� ������� ������� ������ "Esc"
				dIndRUS1_PressButtonEscape; 
                break;
              default:
                break;
            } // switch (buttonMask_Pressed)
          buttonMask_Pressed <<= 8;
        }
      } // if (buttonMask_Pressed)
#endif // #ifdef use_ButtonsShift .. #else
#endif // #ifdef use_IndRUS1

/////////////////////////////////////////////////////
// ����������� LCD-�������:
//   - ������������������ ������� ������ ������.
//   - � �������, ���� ����:
    //  - ��� 1 �������, �������� �� ������� �� ��������� �������
    //  - ��� 1 ������� �� ��������� ������� dLCD_CursorPosition_Make
    //  - ��� 1 ������
// ��������� �� ���������� ������ ��� IndRus1_SetRezhimLCD/IndRus1_SetRezhimButton
// ����� ������ �������, ������ RUS1 � ����� ������

  // unsigned char simbBuffer = lcd_tab_rus[*pLcdTextBuffer];
  unsigned char simbBuffer = 0; 
  if (*pLcdTextBuffer) 
    simbBuffer = lcd_tab_rus[*pLcdTextBuffer];
  // ���� ���� IndRus1_SetRezhimLCD
  // ��� ������ ������, ������� ���� ��������
  if ( (*pLcdText != simbBuffer) 
#ifndef use_IndRUS1
       // ��� ���� ������������������ �������
	   | lcd_IsReInitLCD               
#else
       // ��� ���� ������������������ ������� ��� ������� ���������������� �������� �������
       | lcd_IsReInitLCD_or_IsCursorVisiblePosition_Need 
#endif // #ifdef use_IndRUS1
       // ���� ���� ������ "���������" �������
	   | lcd_IsCommandON )             
  {
#ifdef use_IndRUS1
    dIndRus1_SetRezhimLCD;
#endif // #ifdef use_IndRUS1

    // ��������������� ������� ������ 1 ������
    if (lcd_IsReInitLCD)
    {
      // ����� ���������������, ������ �� ���������� (0;0)
      dLcdRsOff;
      pLastSimbolLCD = NULL;

      // ������� ������� � 4-������ �����
      dLcdE_Off;
      dDelayPWEN;
      dDelayTAS;
      dLcdE_On;
      // ��������� ���� ���������� (8 ���) 
	  // (0x03 - ������� �������� ������� 0x30) - � ������ ������ 
	  // ����������� ��������� � 8-�� ������ ����� ������        
	  dLcd_SetData(0x03); 
      dDelayToCycleE;
      dLcdE_Off;

      // �� ������� �� LCD-����� ���� AC-162A:
      // 0x02, 40��      // Set interface data length
      // 0x28, 40��	// ��������� ����������� ������ ���, �������� 0 
//////// 0x02, 2000��    // ������� ����������
      // 0x0C, 40��	// ��������� ����������, ������ ��������	
//////// 0x0F, 40��      // ��������� ����������, ������ �������
      // 0x06, 40��	// ��������� ������ ����� ������: �������� ������ ������
      const unsigned char c_tmp[4] = { 0x02, 0x28, 0x0C, 0x06 };
      LCD_CommandsAddToBuf(c_tmp, 4);
            
      dSetIsReInitLCD_OFF;
      
      for (unsigned char* pTemp = pLcdBegin; pTemp<pLcdBegin+(lcdColCount*lcdRowCount); pTemp++)
        *pTemp = 0;
    }
    else
    // ������� ������� �� �������
    // ������������ �������� �� lcd-������� ������� ����������� �� 2 ����:
    //  - dLCD_CommandMake (�������, �������� �� ������� ���������������� �������)
    //  - dLCD_CursorPosition_Make (������� ���������������� �������, ����� ������ �������)
    //  ����� �����, ��������� ������� ������ ������� ������� ���������������� ������� (��� �������������)
    if (lcd_IsCommandON)
      dLCD_CommandMake
    else // if (lcd_IsCommandON)
      // if (*pLcdText != *pLcdTextBuffer) �.�. ���� ��� ��������� ������������ �����, � 32 ����� ��� ��� ���� �������� - ������� �����
      if (*pLcdText != simbBuffer)
      {
        // ���� ���� ������ ��������������� ������,
        if (pLastSimbolLCD != (pLcdTextBuffer)) 
        { // �������������
          // ���������������� ��� 2-�������� ������� ���
          // ��������� � ���� �� ����� ���������������� �������, ��� LCD_CursorCmdMake
          unsigned char cmd; // �.�. � LCD_WriteCommandQuickly( x ) x ������������ 2 ����
          if (pLcdTextBuffer < &GetLcdTextBuffer[1][0])
            cmd = 0x80+(pLcdTextBuffer-&GetLcdTextBuffer[0][0]); 
          else
            cmd = (0x80+0x40)+(pLcdTextBuffer-&GetLcdTextBuffer[1][0]);
          LCD_WriteCommandQuickly(cmd);
          pLastSimbolLCD = pLcdTextBuffer;
        }
        // ����� ������� ������
        else // if (pLastSimbolLCD != (pLcdTextBuffer))
        { 
          // ���������������� ��� 2-�������� ������� ���      
          *pLcdText = simbBuffer;
          dLCD_WriteCharQuickly(*pLcdText); 
          // ���� ������������� ���������� ����� �������, ������������� ������
#ifdef use_IndRUS1
          if (GetIsCursorVisible)
          {
            dSetIsCursorVisiblePosition_Need_ON;
            pLastSimbolLCD = NULL;
          }
          else
#endif // #ifdef use_IndRUS1
            // ��������� �������������� ����� ������� ������ �� �������
            if (pLcdTextBuffer == &GetLcdTextBuffer[lcdRowCount-1][lcdColCount-1])
            {                 
              pLastSimbolLCD = NULL; 
              // ������� �� ��������� ������
              pLcdTextBuffer = &GetLcdTextBuffer[0][0]; 
              pLcdText = &GetLcdText[0][0];
            }  
            else
            { 
              if (pLcdTextBuffer == &GetLcdTextBuffer[0][lcdColCount-1])
                // ������� �� ��������� ������
                pLastSimbolLCD = NULL;
              else
                // ������� �� ��������� ������
                pLastSimbolLCD++;
              pLcdText++; 
              pLcdTextBuffer++;
            }
        }
      } // if (*pLcdText != *pLcdTextBuffer) 
#ifdef use_IndRUS1
      else // if (lcd_IsCursorVisiblePosition_Need)
        // if (GetIsCursorVisiblePosition_Need) // ������������� ������� ������
          // ���������� ������� ���������������� �������
          { 
            LCD_WriteCommandQuickly((0x80+0x40) + GetBaseXCursor - GetLnChange);
            dSetIsCursorVisiblePosition_Need_OFF;
            // ��������� �����, ��������� ������ ��� ��������
            // �������, ����� ��� �� ��������������� ������ �� ����, ���������, ��� 
            // ��������� ������� ���������������� ������� ��������� ��� ��� ����, ��� �������� ������
            pLastSimbolLCD = (pLcdBufBegin + lcdColCount) + GetBaseXCursor - GetLnChange;
          }
    dIndRus1_SetRezhimButton;
#endif
  } // if ( (*pLcdText != *pLcdTextBuffer) | lcd_IsReInitLCD | ... )
  else
  {
    // ������� �� ��������� ������
    if (pLcdTextBuffer == &GetLcdTextBuffer[lcdRowCount-1][lcdColCount-1])
      { pLcdTextBuffer = &GetLcdTextBuffer[0][0]; pLcdText = &GetLcdText[0][0]; }
    else
      { pLcdText++; pLcdTextBuffer++; }
  }
  
  ///////////////////////////////////////////////////////////////
  // ��������� "������" ������ � ���������� �������� �������������
#ifdef use_IndRUS1
  
#ifdef use_WatchDog_for_PUC
  dWatchDogReset; // ����� ������� ����������� ������ ���������� �� �����, ���������� ���������� ������
#endif
  if ( (isRezhimSpecMessage) & (!cnt1s_ChangeOff) )
 //  if (isRezhimSpecMessage)
  {
    if (lcd_Is100ms)
      ShowCriticalMessage();
  }
  else
  switch (GetRRP)
  {
    case rrp_0: // ������� �����
      if (lcd_Is100ms)
        // if (NULL != *(currentTreeNode->pFunctionShowInfo)) ��� �������� ����� �� ������
          currentTreeNode->pFunctionShowInfo();
      break;

// rrp_2 � rrp_3: ������������ ��������� GetLnChange � �������������� ��������� �������
// GetLnChange �������� ����� ������� interruptsLnChange
// ���� ������� ����� ������� ������� ������ �� ����� �������������� ������� (����������� interruptsChangePerem)
// ������� ����� ������� ������ ������ �� ����� INC/DEC �����, ����� ������������ �� ������� ��� ������ "�������"
// ���� �������� �����, �� ����� ��� ������� �� ������� ����� ������� dLCD_CursorCmdAdd
///////////////////////
    case rrp_2: // ��������� "�����" ��. t_VLtreeWork 
      if (GetIsCursorVisible) // ���� ���������� �������������� � �������������� ��������� �������
      {
        if (isButtonUpPressed) // ���� ����� 2 � ������ ������ 1 // �������� � �������
        {
          dResetTimerCounterLnChangeKeyUnPressed;
          dIncTimerCounterLnChangeKeyPressed;
          if (GetTimerCounterLnChangeKeyPressed >= interruptsLcd_LnChange)
          {                     
            dIncLnChange; // ����� ������� ���������� ������� ������� �� ������� ��� ��������� ����������
            dResetTimerCounterLnChangeKeyPressed;
            dSetIsCursorVisiblePosition_Need_ON;
          }   
          // �������� ����������, ��������� �� ������� ���������
          if (isWaitForButtonBounceDelay) // ������ �� ������������� ������������ ������ � �������
            dIncTimerCounterChangePerem;
          if (GetTimerCounterChangePerem >= interruptsLcd_ChangePerem)
          {
            currentTreeNode->pFunctionShowInfo(); // ����� ���� FunctionShowInfo != NULL
            dResetTimerCounterChangePerem;
          }  
        }
        else //  if (isButtonUpPressed) // ���� ����� 2 � ������ ������ 1 // �������� � �������
        {
          dResetTimerCounterLnChangeKeyPressed;
          dIncTimerCounterLnChangeKeyUnPressed;
          if (GetTimerCounterLnChangeKeyUnPressed >= interruptsLcd_LnChange)
          { 
            dDecLnChange; // ����� ������� ���������� ������� ������� �� �������
            dResetTimerCounterLnChangeKeyUnPressed;
            dSetIsCursorVisiblePosition_Need_ON;
          }
          dResetTimerCounterChangePerem; 
        }
      }
      break; // case rrp_2: // ��������� "�����" ��. t_VLtreeWork 
    case rrp_3:  // ��������� "����" ��. t_VLtreeWork
      if (GetIsCursorVisible) // ���� ���������� �������������� � �������������� ��������� �������
      {
        if (isButtonDownPressed) // ���� ����� 3 � ������ ������ 1 // �������� � �������
        {
          dResetTimerCounterLnChangeKeyUnPressed;
          dIncTimerCounterLnChangeKeyPressed;
          if (GetTimerCounterLnChangeKeyPressed >= interruptsLcd_LnChange)
          {                     
            dIncLnChange; // ����� ������� ���������� ������� ������� �� ������� ��� ��������� ����������
            dResetTimerCounterLnChangeKeyPressed;
          }   
          // �������� ����������, ��������� �� ������� ���������
          if (isWaitForButtonBounceDelay) // ������ �� ������������� ������������ ������ � �������
            dIncTimerCounterChangePerem;
          if (GetTimerCounterChangePerem >= interruptsLcd_ChangePerem)
          {
            currentTreeNode->pFunctionShowInfo(); // ����� ���� FunctionShowInfo != NULL
            dResetTimerCounterChangePerem;
          }      
        }
        else //  if (isButtonDownPressed) // ���� ����� 3 � ������ ������ 1 // �������� � �������
        {
          dResetTimerCounterLnChangeKeyPressed;
          dIncTimerCounterLnChangeKeyUnPressed;
          if (GetTimerCounterLnChangeKeyUnPressed >= interruptsLcd_LnChange)
          { 
            dDecLnChange; // ����� ������� ���������� ������� ������� �� �������
            dResetTimerCounterLnChangeKeyUnPressed;
            dSetIsCursorVisiblePosition_Need_ON;
          }
          dResetTimerCounterChangePerem;
        }
      } // if (GetIsCursorVisible) // ���� ���������� �������������� � �������������� ��������� �������
      break; // case rrp_3:  // ��������� "����" ��. t_VLtreeWork
//     case rrp_4: // ��������� �� ���� ����� � ����������� ��������
//       break;
//     case rrp_5: // ��������� �� ���� ����� �� ��������� ��������
//       break;    
    default:
      break;
  } // switch (GetSpecRezhim)
#else
    if (lcd_Is100ms) // ������� �����
        if (isRezhimSpecMessage)
        { /*if (NULL != vlTree.f_rezhimSpecMessage) ��� �������� ����� */ vlTree.f_rezhimSpecMessage(); }
        else
          Show_Root();
#endif // #ifdef use_IndRUS1  

  
  
// ������ ��� �� ������ ��������� 'MODEM:STARTUP', ���� ����� ����������, ��� �� ���������.
// ����� 10 ��� ��������� ������ ������� 'AT+CPBS="SM"' � ���������� 0.5 ������, ���� ��� ��� �� ������� "OK" -
// ������ ���-����� ���. ����� ���������� ������ ������� 'AT+CPBS?', ���� ����� ����� ���������� ������� � 
// ���������� ����� ���-�����. ����� ��� ����������� � GSM-����, ����� ���� ��������� ����������� �����.
  
  if (receiveGsmPackageCount>0) // ���� "��������" ����� �� UART
  {
    if (!isWaitAnswer)
      dIncTimerCounter_AtPauseDelay;
    // ������� ��������� (�� ������ �� �������) ���� ��� �������������� "\r\nOK\r\n".
    // ���� �� ��� ������ �� ������: ���������� ����� ����� ����� ���������� ������� �� Uart = ������ ����� �� Uart.
    // ���� ��� ����� �� ������: ������������ �� �� �����, � �� �������� �������������� "\r\nOK\r\n" � ����� ����� ��.
    if (GetTimerCounter_AtPauseDelay >= itterruptsIndRus1_AtPauseDelay)
    {
      // ������ � switch ����� � ����� ���������� isWaitAnswer.
      // ���� ���� ������������ �������� �� ������ "\r\nOK\r\n", ���������� ������� ������ ������ ����� SetFuncsSeries, �������� 
      // ����� ���������� SetFuncsSeries ����������� ���������
      switch (IndexReservedMsg()) // ������ ����� IndexReservedMsg ����� ������ ����������� �� ������ �����
      {
        // ����� ������������ �� �������� ���������
		case msgGsm_ModemStartUp: 
          // ������ ���� "����� ������ ��� ��������� � ������� ���������"          
		  dSetModemStartUp_On; 
          // ������ ��������� ����� ������ �������
		  SetRezhimSpecMessage_Show100ms(0); 
          dSetFuncsSeries_CmdCpbs_SimCard;
          // ���� ��� ���-�����, ����� ������� �� ����� 0 �� ������� msgGsm_Status                    
		  dSetModemReady_On; 
          break;
        
		// ��������� ���������� �� ���-�����, ���� ����� �����, ������� �� ���-����� � ������
		case msgGsm_CmdCpbs_SimCard: 
          // ��� GSM-����
		  errorScreen.word = line_ModemPbReady_Error; 
          // ����� 25 ������ �����, ��� GSM-���� �� ����������
		  SetRezhimSpecMessage_Show100ms(250); 
          // ������ �� �������� �� ������ ��������� "+PBREADY"
          // RezhimSpecMessage_On(lines_ModemPbReady_Wait, gsmWaitAnswer_100ms, (BIT0 | BIT6));
          break;
        
		// ���� ������ ��������� +PBREADY
		case msgGsm_PbReady: 
          // ������ ���� "����� ������� ����������������� � GSM-����"
		  dSetPbReady_On;      
          // ������ ���������� ������� ������ ����� dSetPbReady_On          
		  dSetFuncsSeries_OnPbReady; 
          // ����� ���������� dSetFuncsSeries_OnPbReady, 
          // ��������� ���������� ����� �������� �������� (������� funcsSendToUart_TestCnt), ��� ����������� 
          // ������ �� ������� ������������� ����� � ���������� ��� ���������� ������
          // ������� ��������� � ��������� ������
		  RezhimSpecMessage_On(lines_ModemSetupModem, 0, (BIT0 | BIT5 | BIT6)); 
          break;
        
		// ���������� �� ��������� gsm // ���������� ��� �� ��� �����������, ��� GSM-���� �����������
		case msgGsm_InfoOperator: 
          // �� ��������: ���� ����� ���� 1 ��� ������� ���������� �� ��������� gsm, 
		  // ������ ����� ���������� ���������� ��������� �� ��� ������������
          ParseReservedOperatorGsm();
          break;
        case msgGsm_Ussd: // ����� �� ussd-������
          ParseAnswerUssd(); // ������� ussd-������� // �� ������ �����
      //     if (isSetupOnStartUp)
        //     RezhimSpecMessage_Off;
          break;
        
		// ���� ������ ��������� � ����������� ������
		case msgGsm_Ring: 
          ParseReservedRing();
          // ����� ����� ������ ������ ���������, ����� �������� � �������
		  StartFindPhoneNumberOk(); 
          break;
        
		// ���� ������ ��������� � ����������� ���
		case msgGsm_Sms: 
          // ������� ��� ��: �����, ����� � �����
		  ParseReservedSms(); 
          // ����� ����� ������ ������ ���������, ����� �������� � �������
		  StartFindPhoneNumberOk(); 
          break;
        
		// ��������� ���������� ����� � ���������� �����
		case msgGsm_PhoneNumber: 
          ParseAnswerPhoneNumber();          
          break;
        
		// ������ ����� �������� ��� 
		// ���������� ������ � ������ PDU
		case msgGsm_Smgs: 
          ParseAnswerCmgs();
          // ����� �� ���������� ������� �������� ���������� ������ - ��� � ���������� ������
		  dSetModemReady_On; 
          break;
        
		// ��������� ���������� ���
		case msgGsm_SmsSendComplete: 
          dSetSendSmsComplete_Off;
          // ���� ��������� �� ��� ����� "�������" ���
		  if (sds.smsPartIndex != sds.smsPartsCount) 
          {
            // ������� ������������ ��� ��� �������� "�������" ���
			sds.smsPartIndex++; 
            dSetFuncsSeries_SendSmsToNumberFromPhoneBook;
          }
          else
          {
            // ���� ���� ����-�� ��� ��������� ���
			if ( (isSendSmsToAllPhonesInBook) & (bookNumbersNeedGetCount) ) 
              // ��������� ����� �� �����
		      dSetFuncsSeries_FindNextNumber; 
            else 
            {
			  // ����������� �������� ���
              dSetSendSmsToAllPhonesInBook_Off;
              dSetNeedSendSmsOnPower220_Alarm_PowerOff_Off;
              dStopSecondsPowerAlarm;
              dSetFuncsSeries_TestModem_PerHour; // ����� ����, ��� ��������� ���, ������ ��������� - ��������� ���
            }
          }
          break;
        
		/// ������� �������
		case msgGsm_Level:
          ParseReservedLevelGsm();
          break;
        
		// ���������� � ���������� ����� 
		// ���������� ������� 
		// ���������� ��� ����, ��� ����� ��������� ��������
		case msgGsm_BookInfo: 
          dParseAnswerBookInfo();
          break;
		  
        case msgGsm_Status:
          // ���� ������� ����������� � �������� ���� � ������
		  if ('0' == *answerEffectiveDataBegin) 
            dSetModemReady_On;
          break;
		  
        default:
          // RezhimSpecMessage_On(Show_Error_Gsm);
          break;
      } // switch (IndexReservedMsg())
	  
      dResetReceiveGsmPackageCount;
      dResetTimerCounter_AtPauseDelay;
      dSetWaitAnswer_Off;
      dResetFromLastCommand100ms;
    } // if (GetTimerCounter_AtPauseDelay >= itterruptsIndRus1_AtPauseDelay)
  } // if (receiveGsmPackageCount>0)

} // #pragma vector =