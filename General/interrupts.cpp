// модуль для описаний прерываний

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
// флаг прерывания UCAxRXIFG устанавливается каждый раз при приёме символа и загрузке его в 
// регистр UCAxRXBUF. 
// Флаг UCAxRXIFG автоматически сбрасывается при чтении регистра UCAxRXBUF.
  if (&receiveGsmPackage[receiveGsmPackageMax-1] >= pReceiveGsmPackageActive)
  {
	*pReceiveGsmPackageActive = UCA0RXBUF; 
    pReceiveGsmPackageActive++;
  }
  else
  {
	unsigned char c_tmp = UCA0RXBUF; 
    // если буфер переполнен, указываем, что не надо ожидать "\r\nOK\r\n" - он уже в этом пакете не уместится
	dSetWaitAnswer_Off; 
  }
  
  GreenLedSignal_ON();
  
  if (isWaitAnswer)
    {
      // если последние символы "\r\nOK\r\n", то сбрасываем флаг isWaitAnswer и на всякий случай ждём период itterruptsIndRus1_AtPauseDelay
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
                modemAnswerCnt++; // инкремент счётчика ошибок
                if (modemAnswerCnt >= modemAnswerCntMax)
                {
                  // только если сим-карта отсутствует, не перезагружаем модем
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
пример смс при отключении напряжения на время < secondsPowerAlarmMax:
Напряжение 220В и 15с было выше порога 250В
Реле включено
Баланс100,0

пример смс при отключении напряжения на время > secondsPowerAlarmMax:
Напряжение выше порога 250В
Реле выключено
Напряжение 220В
Баланс100,0

пример смс при восстановлении напряжения после длительного, > secondsPowerAlarmMax, отключения электричества:
Напряжение нормализовалось
Реле включено
Напряжение 220В
Баланс100,0
*/
#pragma vector = ADC10_VECTOR 
__interrupt void ADC10_VECTOR_PRE (void)
{
  signed int res = ADC10MEM;  
  dVal220add( res );
  unsigned int now220 = GetVal220();    
  // дребезг 220В
  if (isStartPowerAlarmBounceDelay)
  {
    dDecPowerAlarmBounceDelay;
    if (!isStartPowerAlarmBounceDelay)
    {
      // сразу же заполняем значение напряжения в sds, чтобы к моменту вызова SdsDataUpdate() оно не успело измениться
      if (isRelePower220_On) // если ещё не отключили - значит, отрабатываем понижение напряжения
      {
        if ( (now220 < nastr_DataMin220) & (0 != now220) )
        {
          sds.val220 = now220;
          sds.is200wasUp = false;        
          dBeepSignal_LongBad;
          dRelePower220_Off;
          dSetPower220_Alarm_PowerOff_On; // "аварийно выключили нагрузку из-за выхода там напряжения за пределы допустимого"        
          RezhimSpecMessage_On(lines_Power220_Alarm_PowerOff_Lower, 0, BIT2 | BIT5);      
        }
        else
          dStopSecondsPowerAlarm;
      }
      else // if (isRelePower220_On)
      {
        // если напряжение в норме, то останавливаем тревогу
		if (now220 < (nastr_DataMax220 - val220ustavka) ) 
        {
          RelePower220_On();
          // "аварийно выключили нагрузку из-за выхода там напряжения за пределы допустимого"
		  dSetPower220_Alarm_PowerOff_Off; 
          // остановить звуковой сигнал
		  dResetBeepMasIndex; 
          // закончить отчёт секунд о пропадании напряжения
		  dStopSecondsPowerAlarm; 
        }
        else
        {
          // очень опасно дёргать реле несколько раз в секунду; используем уставку в абс значении
		  if (now220 <= nastr_DataMax220) 
            now220 = nastr_DataMax220+1;
          sds.val220 = now220;
          sds.is200wasUp = true;      
        }
      }          
    } // if (isStartPowerAlarmBounceDelay)
    return;
  }
  
  // если ещё не затрубили тревогу
  if (!isPower220_Alarm_PowerOff)
  {
    if (res > (adc220_max/*+adc220ustavka*/))
    {
      dRelePower220_Off;
      // "аварийно выключили нагрузку из-за выхода там напряжения за пределы допустимого"
	  dSetPower220_Alarm_PowerOff_On; 
      dBeepSignal_LongBad;
      // начать отчёт секунд о пропадании напряжения
	  dStartSecondsPowerAlarm; 
      // учёт дребезга 220В
	  dStartPowerAlarmBounceDelay; 
      RezhimSpecMessage_On(lines_Power220_Alarm_PowerOff_Upper, 0, BIT2 | BIT5);
    }
    else
      if (res < adc220_min)
      {
        // начать отчёт секунд о пропадании напряжения
		dStartSecondsPowerAlarm; 
        // учёт дребезга 220В
		dStartPowerAlarmBounceDelay; 
      }
  }
  // иначе, если тревога уже объявлена
  else // if (!isPower220_Alarm_PowerOff)
  {
  if ( (now220 < (nastr_DataMax220-val220ustavka)) &
    (now220 > (nastr_DataMin220+val220ustavka)) )
    {
      dDecPowerAlarmNormDelay;
      if (!isStartPowerAlarmNormDelay)
      {
      // если надо отправить сообщение о включении электричества, отправляем его
      if (bookNumbersCount) // если есть кому отправлять
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