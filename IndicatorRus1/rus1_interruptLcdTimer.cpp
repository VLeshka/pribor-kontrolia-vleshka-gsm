//////////////////////////////////////////// 
// модуль для описания прерывания таймера
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
// itterruptsIndRus1_AtPauseDelay и funcsSeries.waitBetween_100ms
//////////////////////////////////////////////////////////////////////// 
// 
// 1. Пакет от модема считается законченным, если выдержано время itterruptsIndRus1_AtPauseDelay после 
//    приёма символа из пакета.
// 2. Между пакетами, отправляемых модему, выдерживается интервал funcsSeries.waitBetween_100ms.
// Какой интервал выдерживается - itterruptsIndRus1_AtPauseDelay или funcsSeries.waitBetween_100ms - определяется 
// по условию if (0 == receiveGsmPackageCount).



/*прерывание по NameTimerForLcd используем для:
  - минимизации использования IndRus1_SetRezhimLCD/IndRus1_SetRezhimButton.
  - работы со светодиодами (если используется LaunchPad MSP-EXP430G2).
  - профилактической переинициализации дисплея каждую минуту.
  - работы с LCD-дисплеем (с отсутствием "мёртвой" паузы при отправке команды или данных на LCD-дисплей). 
  - реализации защиты от "дребезга" кнопок (с отсутствием "мёртвой" паузы).
  - реализации защиты от "долгого" нажатия кнопки (с отсутствием "мёртвой" паузы).
*/
#pragma vector = NameTimerForRus1
__interrupt void NameTimerForRus1_PRE (void)
{    
  /*Замечание по поводу использования сторожевого таймера:
  0xFFFF тактов сторожевого таймера с лихвой хватает на выполнение этого прерывания [при демонстрация 
  наглядной работы мк с индикаторной платой РУС-1] вместе со всеми подпрограмами вывода информации, 
  описанных в модулях vl_TreeStructureGenerator_Functions.*.
  Однако, для повышения надёжности при разработке новых подпрограмм вывода информации,
  перед вызовом подпрограмм вывода информации на экран, сбрасываем сторожевой таймер.
  Новым пользователем следует учитывать, что их подпрограммы вывода информации, должны:
  - или укладываться по тактам,
  - или сбрасывать сторожевой таймер (команда dWatchDogReset).
  В противном случае произойдёт генерация сторожевым таймером сигнала PUC.*/
  
  ADC10CTL0 |= ADC10SC; // измеряем напряжение на нагрузке 220В

  //////////////////////////////////////////////////////////// 
  // ведение счётчиков по времени
  // ставим маску срабатывания наступления временных интервалов
  //////////////////////////////////////////////////////////// 
  // с младшего бита:
  // BIT0 - 10 мс
  // BIT1 - 100 мс
  // BIT2 - 1 с
  // BIT3 - 1 мин
  // BIT4 - 1 час
  // BIT5 - 1 день = 24 часа
  // BIT6 - 1 месяц
  // BIT7 - 1 год
  unsigned char timeMask = 0;
  if (cntTIMER_1min >= interrupts1min) // счётчик минуты // отталкиваемся от 1 минуты
  {
    if ( !GetIsCursorVisiblePosition_Need )
        dSetIsReInitLCD_ON;
    if (cntTIMER_1hour >= interrupts1hour) // счётчик часа
    {
      // наступил новый час
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
    // счётчик 100 мс
    if (cntTIMER_100ms >= interrupts100millisec)
    {
      // настало 100мс
      cntTIMER_100ms = 0;
      
      // счётчик секунды
      if (cnt100msTo_1s >= ms100inMin) // обычный режим
      {
        // настала секунда
        timeMask |= (BIT0 + BIT1 + BIT2);
        cnt100msTo_1s = 0;        

#ifdef use_IndRUS1
//         if ( lcd_Is1s )
        /*
        // счётчик неактивности пользователя, чтобы выходить из режима редактирования, а так возвращать на экран сообщение
        if (cnt1s_ChangeOff) 
            cnt1s_ChangeOff--;
          if (!cnt1s_ChangeOff) // обычный режим
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
                  LCD_SetCursorBlinkOff(); // Выключение индикатора, курсор выключен // выход из редактирования
              }
          }
*/
/*
        // счётчик неактивности пользователя, чтобы выходить из режима редактирования
        if (0 != GetRRP)
        {
          if (cnt1s_ChangeOff) 
            cnt1s_ChangeOff--;
          if (!cnt1s_ChangeOff) // обычный режим
          {
            SetRRP(rrp_0);
            if (GetIsCursorVisible)
              LCD_SetCursorBlinkOff(); // Выключение индикатора, курсор выключен // выход из редактирования
          }
        }
*/
        // счётчик неактивности пользователя, чтобы выходить из режима редактирования
        if (cnt1s_ChangeOff) 
          cnt1s_ChangeOff--;
        if (!cnt1s_ChangeOff) // обычный режим
        {
          SetRRP(rrp_0);
          if (GetIsCursorVisible)
            LCD_SetCursorBlinkOff(); // Выключение индикатора, курсор выключен // выход из редактирования
        }
#endif
        
        // сколько секунд осталось до включения модема, при выполнении команды перезагрузка
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
        
        // счётчик секунд выключенного напряжения для начала рассылки смс        
        if (!isNeedSendSmsOnPower220_Alarm_PowerOff) // если ещё не надо рассылать смс о пропадании напряжения
          if (nastr_IndexSendSmsOnBad220) // если в настройках указано посылать смс при пропадании 220В            
            if (isStartSecondsPowerAlarm) // если отсчитываем время до запуска расылки смс, отсчитываем его
            {
              dDecSecondsPowerAlarm;
              if (!isStartSecondsPowerAlarm)
                dSetNeedSendSmsOnPower220_Alarm_PowerOff_On; // и начинаем рассылку смс
            }
        
        // для тестирования реле
        // dRelePower220_Invert; // каждую секунду вкл/выкл реле
        
      } // if (cnt100msTo_1s >= ms100inMin)
      else    
      {
        timeMask |= (BIT0 + BIT1);
        cnt100msTo_1s++;        
      }

      // начинаем рассылку смс при готовности модема
      if (isNeedSendSmsOnPower220_Alarm_PowerOff)
        if (!isSendSmsToAllPhonesInBook)
          StartSendSmsToAllNumbers();
      
      // режим спецсообщения
      // if (isRezhimSpecMessage)
        if (isRezhimSpecMessage_Time100msShow)
        {
           dDecRezhimSpecMessageTime100msCnt;
           if (!isRezhimSpecMessage_Time100msShow)
           {
             if (isSpecMessage_ChangeToCritical_onZero)
             {
               dBeepSignal_LongBad;
               RezhimSpecMessage_On(errorScreen.word, 0, (BIT7 | BIT5 | BIT4)); // выводим сообщение о критическом событии с В ЛЮБОМ КАТАЛОГЕ
             }
             else
               RezhimSpecMessage_Off();
           }
        }

      // выключение светодиодов
      ////////////////////////////// 
//       if (!isRezhimSpecMessage)
      {
        // красный светодиод
        if (isRedLedSignal)
        {
          dDecRedLedSignalCnt;
          if (!isRedLedSignal)
            dRedLed_Off;
        }
        // зелёный светодиод
        if (isGreenLedSignal)
        {
          dDecGreenLedSignalCnt;
          if (!isGreenLedSignal)
            dGreenLed_Off;
        }
      }
      
      // звуковой сигнал
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
      // обслуживаем модем
      if ( ( 0 == receiveGsmPackageCount ) | ( isWaitAnswer & receiveGsmPackageCount>0 ) )
      {
        dIncFromLastCommand100ms; // инкремент счётчика 100мс после последней команды модему
        if (!isGsmVcc_On) // модем пока не включился
        {
          // модем надо включить
		  if (isModemNeedPowerOn) 
          {
            dGsmVcc_On;
            // ожидаем от модема сообщения "MODEM:STARTUP" 10 секунд
            // сообщение о не найденном модеме
			errorScreen.word = lines_GsmModem_Off; 
            RezhimSpecMessage_On(lines_ModemPowerOn, 100, (BIT6 | BIT5 | BIT0)); // по прошествии 10с выведется "модем не обнаружен"
          }
        }
        else // if (!isGsmVcc_On) // модем пока не включился
        {        
          if (isWaitAnswer)
          {
            // если модем уже не отправляет нужный пакет - значит он завис и его надо перезагружать
            // ожидание от модема истекло
			if (isEndWaitAnswerOfCommand_100ms) 
              RestartModem();
          }
          else // if (isWaitAnswer)        
            if (isExistsNextCommand)
            {
              if (dFuncsSeriesNeedStart)
                // выполнение следующей функции из очереди
			    dFuncsSeriesSendNextCommand; 
              if (isRezhimSpecMessage_OffOnEndFuncsSeries)
                // если надо сбросить спецрежим, сбрасываем его
			    if (!isExistsNextCommand) 
                  dSetRezhimSpecMessage_OffOnEndFuncsSeries_Off;
            }
            else
              if (isSetupOnStartUp)
                if (!isFindOkNumberInBook)
                  // если с модемом не работаем // не рассылаем смс
			      if (!isSendSmsToAllPhonesInBook) 
                    if (isGsmTestCommandNeed)
                    {
                      if (lcd_Is1hour)
                        // проверяем модем, что он не завис
					    dSetFuncsSeries_TestModem_PerHour; 
                      else
                        // проверяем модем, что он не завис
					    dSetFuncsSeries_TestModem_Often; 
                      // чтобы не ждать зря 100мс до следующей команды, посылаем её
					  // dFuncsSeriesSendNextCommand(); 
                    }
        } // if (!isGsmVcc_On) .. else
      } // if (0 == receiveGsmPackageCount)
      
    } // if (cntTIMER_100ms >= interrupts100millisec)
    else
      cntTIMER_100ms++;
  }
    
        
        /////////////////////////////////////////
        // светодиоды
        if ( lcd_Is1s )
        {
          // используем зелёный светодиод как сигнал, что прибор не завис
          if (!isWasTypeSpecMessage_CriticalError)
            if (isSetupOnStartUp)
              GreenLedSignal_ON();
          // критическое событие
          if (isWasTypeSpecMessage_CriticalError)
            // красный светодиод мигает длинными сигналами
            dRedLed_Invert;
        }

#ifdef use_WatchDog_for_PUC
  dWatchDogReset; // сбрасываем сторожевой таймер
#endif

  //////////////////////////////////////// 
  // здесь выходим из прерывания, если надо
  //////////////////////////////////////// 
  // Если что-то пришло в UART, незамедлительно читаем из него.
  // В мк msp430G2553 приоритет вектора прерывания UART меньше, чем у таймеров Timer0 и Timer1.
  if (IFG2 & UCA0RXIFG) return;
  
  //////////////////////////////////////////////////// 
  // обслуживаем кнопки
  /////////////////////
  
#ifdef use_IndRUS1

  // btnPressed - маска нажатых клавиш в течение interruptsLcd_ButtonPostDelay и ожидания_отжатия_кнопок
  // btnPressed - не для редактирования переменных
  unsigned int btnPressed_Now = ((~indRus1_InPort) & indRus1_ButtonAll); // результат uint c очищенным старшим байтом // indRus1_InPort & (~indRus1_ButtonAll)
  
#ifdef use_ButtonsShift
///////////////////////////////////////////////////////////////////////////////////////////////
// Возможна обработка нажатия комбинации нескольких кнопок
// Для этого некоторые кнопки допускают удерживание перед своим "срабатыванием", для комфортного 
// нажатия комбинации кнопок - выполняющит функцию "Shift-кнопки".
// "Shift-кнопки" определяются маской buttonDelayWaitFlagMask_Shift.
// События нажатия "shift" и "не-Shift" кнопок произходят при разных условиях:
//  - "shift-кнопка" - сразу при нажатии кнопки;
//  - "не-Shift-кнопка" - при отпускании кнопки.
//////////////////////////////////////////////// 

  // обработка нажатия "не-shift" кнопки
  if ( 
     ( 0 != btnPressed_Now ) // если нажата какая-нибудь кнопка
     & ( 0 == ( btnPressed_Now & buttonBounceDelay_Shift ) ) // если нажалась не-shift-кнопка
     & ( 0 == buttonMask_Pressed ) // если кнопка нажимается в первый раз
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
          dIndRUS1_PressButtonUp; // обработка успешного события нажатия кнопки "вверх"
          break;
        case indRus1_ButtonDown: 
          dIndRUS1_PressButtonDown; // обработка успешного события нажатия кнопки "вниз"
          break;
        default:
          break;
      } // switch (buttonMask_Pressed)
    buttonMask_Pressed |= btnPressed_Now;
    interruptsIndRus1_ButtonCnt = 0;
  } // if ( btnPressed ) // если нажата к.-н. кнопка
  else // if ( ( 0 != btnPressed ) & ...
    // фиксируем нажатие shift-клавиши
    if ( btnPressed_Now & buttonBounceDelay_Shift )
      buttonMask_Pressed |= btnPressed_Now;    
    else // if ( btnPressed & buttonDelayWaitFlagMask_Shift )
      // Отсчитываем необходимую паузу после события нажатия кнопок.
      // В случае с shift-кнопкой, отсчитываем время для защиты от дребезга (несколько десятков мс).
      // В случае с не-shift кнопки, дополнительно удерживаем паузу защиты от слишком быстрого 
      // последовательного срабатывания кнопки при её удержании (несколько сотен мс).
      if (buttonMask_Pressed)
      {
        dResetCnt1s_ChangeOff;
        interruptsIndRus1_ButtonCnt++;
        if ( 0 == ( buttonMask_Pressed & buttonBounceAndPostDelayMask_Shift ) ) // если не завязаны shift-кнопки
        {
          if (interruptsIndRus1_ButtonCnt >= interruptsLcd_ButtonPostDelay)
            dButtonMask_Pressed_Clear;
        } // if ( 0 == ( btnPressed & buttonDelayWaitFlagMask_Shift ) )
        else
          if (buttonPostDelay_Shift & buttonMask_Pressed) // если "после события" shift-кнопки
          {
            if (interruptsIndRus1_ButtonCnt >= interruptsLcd_ButtonPostDelay)
              dButtonMask_Pressed_Clear;            
          }
          else // значит это "до события" shift-кнопки
            {
              if (isRezhimSpecMessage)
              {
     			// если не "простое" нажатие Esc в корневой папке, выходим из режим6
				// + учитываетсчя, если нажата не только Esc [если нажат Enter]
                if (
                  ( ( 0 == ( buttonBounceDelay_Shift & buttonMask_Pressed) )
                    & (currentTreeNode == &treeNode_Root) ) 
                  | ( (~buttonBounceAndPostDelay_Esc) & buttonMask_Pressed) 
                  )
                  RezhimSpecMessage_Off();
              } // if (isRezhim6)
              else
                // обработка нажатия shift-кнопки [совместно с другой кнопкой]
                switch (buttonMask_Pressed)
                {
                  case indRus1_ButtonEnter: 
                    // обработка успешного события нажатия кнопки "Enter"
					dIndRUS1_PressButtonEnter; 
                    break;
                  case indRus1_ButtonEscape: 
                    // обработка успешного события нажатия кнопки "Esc"
					dIndRUS1_PressButtonEscape; 
                    break;  
                  // возможна обработка нажатия сразу нескольких кнопок
				  // обработка успешного события нажатия кнопок "вверх" + "вниз" + "Enter"
                  case indRus1_ButtonUp + indRus1_ButtonDown + indRus1_ButtonEnter: 
//                    Tree_JumpToRootKatalog(); // Tree_JumpToRootKatalog - только для примера
//                    Rezhim6_On(LCD_ShowMsgDone);
                    break;
                  default:              
                    break;
                } // switch (buttonMask_Pressed)
              buttonMask_Pressed <<= 8;
              interruptsIndRus1_ButtonCnt = 0;
            } // if ( buttonMask_Pressed & buttonDelayWaitFlagMask_Shift ) // если в маске Enter или Esc
          } // if (buttonMask_Pressed)
#else // #ifdef use_ButtonsShift
  if ( 
     ( 0 != btnPressed_Now ) // если нажата какая-нибудь кнопка
     & ( 0 == buttonMask_Pressed ) // если кнопка нажимается в первый раз
     )
  {
    if (isTypeSpecMessage_ChangeOff)
      dResetCnt1s_ChangeOff
    else
      RezhimSpecMessage_Off();
    buttonMask_Pressed = btnPressed_Now;
    interruptsIndRus1_ButtonCnt = 0;
  } // if ( btnPressed ) // если нажата к.-н. кнопка
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
                // обработка успешного события нажатия кнопки "вверх"
				dIndRUS1_PressButtonUp; 
                break;
              case indRus1_ButtonDown: 
                // обработка успешного события нажатия кнопки "вниз"
				dIndRUS1_PressButtonDown; 
                break;
              case indRus1_ButtonEnter: 
                // обработка успешного события нажатия кнопки "Enter"
				dIndRUS1_PressButtonEnter; 
                break;
              case indRus1_ButtonEscape: 
                // обработка успешного события нажатия кнопки "Esc"
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
// обслуживаем LCD-дисплей:
//   - переинициализируем дисплей каждую минуту.
//   - и выводим, если надо:
    //  - или 1 команду, отличной от команды на установку курсора
    //  - или 1 команду на установку курсора dLCD_CursorPosition_Make
    //  - или 1 символ
// стараемся не переводить лишний раз IndRus1_SetRezhimLCD/IndRus1_SetRezhimButton
// после вывода символа, ставим RUS1 в режим кнопок

  // unsigned char simbBuffer = lcd_tab_rus[*pLcdTextBuffer];
  unsigned char simbBuffer = 0; 
  if (*pLcdTextBuffer) 
    simbBuffer = lcd_tab_rus[*pLcdTextBuffer];
  // если надо IndRus1_SetRezhimLCD
  // или найден символ, который надо обновить
  if ( (*pLcdText != simbBuffer) 
#ifndef use_IndRUS1
       // или пора реинициализировать дисплей
	   | lcd_IsReInitLCD               
#else
       // или пора реинициализировать дисплей или команду позиционирования видимого курсора
       | lcd_IsReInitLCD_or_IsCursorVisiblePosition_Need 
#endif // #ifdef use_IndRUS1
       // если надо делать "необычную" команду
	   | lcd_IsCommandON )             
  {
#ifdef use_IndRUS1
    dIndRus1_SetRezhimLCD;
#endif // #ifdef use_IndRUS1

    // реинициализация дисплея каждую 1 минуту
    if (lcd_IsReInitLCD)
    {
      // после реинициализации, курсор на координате (0;0)
      dLcdRsOff;
      pLastSimbolLCD = NULL;

      // перевод дисплея в 4-битный режим
      dLcdE_Off;
      dDelayPWEN;
      dDelayTAS;
      dLcdE_On;
      // Установка типа интерфейса (8 бит) 
	  // (0x03 - старший полубайт команды 0x30) - в начале ВСЕГДА 
	  // переключаем индикатор в 8-ми битный режим работы        
	  dLcd_SetData(0x03); 
      dDelayToCycleE;
      dLcdE_Off;

      // из мануала на LCD-экран типа AC-162A:
      // 0x02, 40мс      // Set interface data length
      // 0x28, 40мс	// Настройка правильного режима ЖКИ, страница 0 
//////// 0x02, 2000мс    // Очистка индикатора
      // 0x0C, 40мс	// Включение индикатора, курсор выключен	
//////// 0x0F, 40мс      // Включение индикатора, курсор включен
      // 0x06, 40мс	// Установка режима ввода данных: сдвигать курсор вправо
      const unsigned char c_tmp[4] = { 0x02, 0x28, 0x0C, 0x06 };
      LCD_CommandsAddToBuf(c_tmp, 4);
            
      dSetIsReInitLCD_OFF;
      
      for (unsigned char* pTemp = pLcdBegin; pTemp<pLcdBegin+(lcdColCount*lcdRowCount); pTemp++)
        *pTemp = 0;
    }
    else
    // выводим символы на дисплей
    // подпрограммы отправки на lcd-дисплей команды разделяются на 2 типа:
    //  - dLCD_CommandMake (команда, отличная от команды позиционирования курсора)
    //  - dLCD_CursorPosition_Make (команда позиционирования курсора, когда курсор включён)
    //  кроме этого, выполняем заместо вывода символа команду позиционирования курсора (при необходимости)
    if (lcd_IsCommandON)
      dLCD_CommandMake
    else // if (lcd_IsCommandON)
      // if (*pLcdText != *pLcdTextBuffer) т.к. надо для сообщения использовать буфер, и 32 байта ОЗУ для него выделять - слишеом много
      if (*pLcdText != simbBuffer)
      {
        // если надо сперва позиционировать курсор,
        if (pLastSimbolLCD != (pLcdTextBuffer)) 
        { // позиционируем
          // оптимизированный под 2-строчный дисплей код
          // выполняем в этом же цикле позиционирование курсора, без LCD_CursorCmdMake
          unsigned char cmd; // т.к. в LCD_WriteCommandQuickly( x ) x используется 2 раза
          if (pLcdTextBuffer < &GetLcdTextBuffer[1][0])
            cmd = 0x80+(pLcdTextBuffer-&GetLcdTextBuffer[0][0]); 
          else
            cmd = (0x80+0x40)+(pLcdTextBuffer-&GetLcdTextBuffer[1][0]);
          LCD_WriteCommandQuickly(cmd);
          pLastSimbolLCD = pLcdTextBuffer;
        }
        // иначе выводим символ
        else // if (pLastSimbolLCD != (pLcdTextBuffer))
        { 
          // оптимизированный под 2-строчный дисплей код      
          *pLcdText = simbBuffer;
          dLCD_WriteCharQuickly(*pLcdText); 
          // если редактируется переменная через дисплей, позиционируем символ
#ifdef use_IndRUS1
          if (GetIsCursorVisible)
          {
            dSetIsCursorVisiblePosition_Need_ON;
            pLastSimbolLCD = NULL;
          }
          else
#endif // #ifdef use_IndRUS1
            // учитываем автоматический сдвиг символа вправо на дисплее
            if (pLcdTextBuffer == &GetLcdTextBuffer[lcdRowCount-1][lcdColCount-1])
            {                 
              pLastSimbolLCD = NULL; 
              // перевод на следующую строку
              pLcdTextBuffer = &GetLcdTextBuffer[0][0]; 
              pLcdText = &GetLcdText[0][0];
            }  
            else
            { 
              if (pLcdTextBuffer == &GetLcdTextBuffer[0][lcdColCount-1])
                // перевод на следующую строку
                pLastSimbolLCD = NULL;
              else
                // перевод на следующий символ
                pLastSimbolLCD++;
              pLcdText++; 
              pLcdTextBuffer++;
            }
        }
      } // if (*pLcdText != *pLcdTextBuffer) 
#ifdef use_IndRUS1
      else // if (lcd_IsCursorVisiblePosition_Need)
        // if (GetIsCursorVisiblePosition_Need) // позиционируем видимый курсор
          // выполнение команды позиционирования курсора
          { 
            LCD_WriteCommandQuickly((0x80+0x40) + GetBaseXCursor - GetLnChange);
            dSetIsCursorVisiblePosition_Need_OFF;
            // вероятнее всего, изменится символ под курсором
            // поэтому, чтобы зря не позиционировать курсор на него, указываем, что 
            // последняя команда позиционирования курсора выполнена как раз туда, где мигающий курсор
            pLastSimbolLCD = (pLcdBufBegin + lcdColCount) + GetBaseXCursor - GetLnChange;
          }
    dIndRus1_SetRezhimButton;
#endif
  } // if ( (*pLcdText != *pLcdTextBuffer) | lcd_IsReInitLCD | ... )
  else
  {
    // переход на следующий символ
    if (pLcdTextBuffer == &GetLcdTextBuffer[lcdRowCount-1][lcdColCount-1])
      { pLcdTextBuffer = &GetLcdTextBuffer[0][0]; pLcdText = &GetLcdText[0][0]; }
    else
      { pLcdText++; pLcdTextBuffer++; }
  }
  
  ///////////////////////////////////////////////////////////////
  // отработка "связки" кнопок с изменением настроек пользователем
#ifdef use_IndRUS1
  
#ifdef use_WatchDog_for_PUC
  dWatchDogReset; // перед вызовом подпрограмм вывода информации на экран, сбрасываем сторожевой таймер
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
    case rrp_0: // обычный режим
      if (lcd_Is100ms)
        // if (NULL != *(currentTreeNode->pFunctionShowInfo)) для экономии места на флешке
          currentTreeNode->pFunctionShowInfo();
      break;

// rrp_2 и rrp_3: отрабатываем изменение GetLnChange с использованием мигающего курсора
// GetLnChange изменяем после отсчёта interruptsLnChange
// надо бОльшую часто времени держать курсор на месте редактирования символа (отсчитываем interruptsChangePerem)
// бОльшую часть времени держим курсор на месте INC/DEC числа, чтобы пользователь не заметил как курсор "прыгает"
// если изменяем число, то сразу даём команду на возврат числа обратно dLCD_CursorCmdAdd
///////////////////////
    case rrp_2: // изменение "вверх" см. t_VLtreeWork 
      if (GetIsCursorVisible) // если происходит редактирование с использованием мигающего курсора
      {
        if (isButtonUpPressed) // если режим 2 и нажата кнопка 1 // подтяжка к питанию
        {
          dResetTimerCounterLnChangeKeyUnPressed;
          dIncTimerCounterLnChangeKeyPressed;
          if (GetTimerCounterLnChangeKeyPressed >= interruptsLcd_LnChange)
          {                     
            dIncLnChange; // потом выводим измененние позиции курсора на дисплее при изменении переменной
            dResetTimerCounterLnChangeKeyPressed;
            dSetIsCursorVisiblePosition_Need_ON;
          }   
          // изменяем переменную, показывая на дисплее изменение
          if (isWaitForButtonBounceDelay) // защита от многократного срабатывания кнопки в секунду
            dIncTimerCounterChangePerem;
          if (GetTimerCounterChangePerem >= interruptsLcd_ChangePerem)
          {
            currentTreeNode->pFunctionShowInfo(); // здесь явно FunctionShowInfo != NULL
            dResetTimerCounterChangePerem;
          }  
        }
        else //  if (isButtonUpPressed) // если режим 2 и нажата кнопка 1 // подтяжка к питанию
        {
          dResetTimerCounterLnChangeKeyPressed;
          dIncTimerCounterLnChangeKeyUnPressed;
          if (GetTimerCounterLnChangeKeyUnPressed >= interruptsLcd_LnChange)
          { 
            dDecLnChange; // потом выводим измененние позиции курсора на дисплее
            dResetTimerCounterLnChangeKeyUnPressed;
            dSetIsCursorVisiblePosition_Need_ON;
          }
          dResetTimerCounterChangePerem; 
        }
      }
      break; // case rrp_2: // изменение "вверх" см. t_VLtreeWork 
    case rrp_3:  // изменение "вниз" см. t_VLtreeWork
      if (GetIsCursorVisible) // если происходит редактирование с использованием мигающего курсора
      {
        if (isButtonDownPressed) // если режим 3 и нажата кнопка 1 // подтяжка к питанию
        {
          dResetTimerCounterLnChangeKeyUnPressed;
          dIncTimerCounterLnChangeKeyPressed;
          if (GetTimerCounterLnChangeKeyPressed >= interruptsLcd_LnChange)
          {                     
            dIncLnChange; // потом выводим измененние позиции курсора на дисплее при изменении переменной
            dResetTimerCounterLnChangeKeyPressed;
          }   
          // изменяем переменную, показывая на дисплее изменение
          if (isWaitForButtonBounceDelay) // защита от многократного срабатывания кнопки в секунду
            dIncTimerCounterChangePerem;
          if (GetTimerCounterChangePerem >= interruptsLcd_ChangePerem)
          {
            currentTreeNode->pFunctionShowInfo(); // здесь явно FunctionShowInfo != NULL
            dResetTimerCounterChangePerem;
          }      
        }
        else //  if (isButtonDownPressed) // если режим 3 и нажата кнопка 1 // подтяжка к питанию
        {
          dResetTimerCounterLnChangeKeyPressed;
          dIncTimerCounterLnChangeKeyUnPressed;
          if (GetTimerCounterLnChangeKeyUnPressed >= interruptsLcd_LnChange)
          { 
            dDecLnChange; // потом выводим измененние позиции курсора на дисплее
            dResetTimerCounterLnChangeKeyUnPressed;
            dSetIsCursorVisiblePosition_Need_ON;
          }
          dResetTimerCounterChangePerem;
        }
      } // if (GetIsCursorVisible) // если происходит редактирование с использованием мигающего курсора
      break; // case rrp_3:  // изменение "вниз" см. t_VLtreeWork
//     case rrp_4: // сообщение во весь экран о выполненной операции
//       break;
//     case rrp_5: // сообщение во весь экран об отменённой операции
//       break;    
    default:
      break;
  } // switch (GetSpecRezhim)
#else
    if (lcd_Is100ms) // обычный режим
        if (isRezhimSpecMessage)
        { /*if (NULL != vlTree.f_rezhimSpecMessage) для экономии места */ vlTree.f_rezhimSpecMessage(); }
        else
          Show_Root();
#endif // #ifdef use_IndRUS1  

  
  
// Сперва ждём от модема сообщение 'MODEM:STARTUP', этим самым определяем, что он включился.
// Потом 10 раз отправлям модему команду 'AT+CPBS="SM"' с интервалом 0.5 секунд, если все они не вернули "OK" -
// значит сим-карты нет. Потом отправляем модему команду 'AT+CPBS?', этим самым узнаём количество номеров в 
// телефонной книге сим-карты. Затем ждём регистрации в GSM-сети, после чего полностью настраиваем модем.
  
  if (receiveGsmPackageCount>0) // если "собираем" пакет по UART
  {
    if (!isWaitAnswer)
      dIncTimerCounter_AtPauseDelay;
    // Простые сообщения (не ответы на команды) идут без словосочетания "\r\nOK\r\n".
    // Если не ждём ответа от модема: необходима пауза после приёма последнего символа по Uart = пришёл пакет по Uart.
    // Если ждём ответ от модема: обрабатывать не по паузе, а по принятию словосочетания "\r\nOK\r\n" и паузы после неё.
    if (GetTimerCounter_AtPauseDelay >= itterruptsIndRus1_AtPauseDelay)
    {
      // Парсим в switch ответ и затем сбрасываем isWaitAnswer.
      // Если надо обязательное принятие от модема "\r\nOK\r\n", отправляем команды модему только через SetFuncsSeries, учитывая 
      // чтобы предыдущая SetFuncsSeries закончилась полностью
      switch (IndexReservedMsg()) // индекс после IndexReservedMsg выдаёт только проверенный на ошибки пакет
      {
        // модем отрапортовал об успешном включении
		case msgGsm_ModemStartUp: 
          // ставим флаг "модем только что стартовал и требует настройки"          
		  dSetModemStartUp_On; 
          // теперь сообщение уберём только вручную
		  SetRezhimSpecMessage_Show100ms(0); 
          dSetFuncsSeries_CmdCpbs_SimCard;
          // если нет сим-карты, модем никогда не вернёт 0 на команду msgGsm_Status                    
		  dSetModemReady_On; 
          break;
        
		// указываем записывать на сим-карту, этим самым узнаём, имеется ли сим-карта в модеме
		case msgGsm_CmdCpbs_SimCard: 
          // нет GSM-сети
		  errorScreen.word = line_ModemPbReady_Error; 
          // через 25 секунд пишем, что GSM-сеть не обнаружена
		  SetRezhimSpecMessage_Show100ms(250); 
          // ставим на ожидание от модема сообщения "+PBREADY"
          // RezhimSpecMessage_On(lines_ModemPbReady_Wait, gsmWaitAnswer_100ms, (BIT0 | BIT6));
          break;
        
		// если пришло сообщение +PBREADY
		case msgGsm_PbReady: 
          // ставим флаг "модем успешно зарегистрировался в GSM-сети"
		  dSetPbReady_On;      
          // ставим выполнение очереди команд после dSetPbReady_On          
		  dSetFuncsSeries_OnPbReady; 
          // после выполнения dSetFuncsSeries_OnPbReady, 
          // регулярно опрашиваем модем тестовой командой (очередь funcsSendToUart_TestCnt), при неполучении 
          // ответа на которую перезагружаем модем с дальнейшей его настройкой заного
          // выводим сообщение о настройке модема
		  RezhimSpecMessage_On(lines_ModemSetupModem, 0, (BIT0 | BIT5 | BIT6)); 
          break;
        
		// информация об операторе gsm // используем так же для определения, что GSM-сеть отсутствует
		case msgGsm_InfoOperator: 
          // на практике: если модем хоть 1 раз получит информацию об операторе gsm, 
		  // всегда будет возвращать информацию нормально до его перезагрузки
          ParseReservedOperatorGsm();
          break;
        case msgGsm_Ussd: // ответ на ussd-запрос
          ParseAnswerUssd(); // парсинг ussd-запроса // на баланс симки
      //     if (isSetupOnStartUp)
        //     RezhimSpecMessage_Off;
          break;
        
		// если пришло сообщение о поступлении звонка
		case msgGsm_Ring: 
          ParseReservedRing();
          // пауза после ответа модема выдержана, можно работать с модемом
		  StartFindPhoneNumberOk(); 
          break;
        
		// если пришло сообщение о поступлении смс
		case msgGsm_Sms: 
          // парсинг смс на: номер, время и текст
		  ParseReservedSms(); 
          // пауза после ответа модема выдержана, можно работать с модемом
		  StartFindPhoneNumberOk(); 
          break;
        
		// выбранный телефонный номер с телефонной книги
		case msgGsm_PhoneNumber: 
          ParseAnswerPhoneNumber();          
          break;
        
		// первая часть отправки смс 
		// отправляем только в режиме PDU
		case msgGsm_Smgs: 
          ParseAnswerCmgs();
          // чтобы не отправляло команду проверки готовности модему - тут её отправлять нельзя
		  dSetModemReady_On; 
          break;
        
		// закончили отправлять смс
		case msgGsm_SmsSendComplete: 
          dSetSendSmsComplete_Off;
          // если отправили не все смски "длинной" смс
		  if (sds.smsPartIndex != sds.smsPartsCount) 
          {
            // счётчик отправленных смс при отправке "длинных" смс
			sds.smsPartIndex++; 
            dSetFuncsSeries_SendSmsToNumberFromPhoneBook;
          }
          else
          {
            // если надо кому-то ещё отправить смс
			if ( (isSendSmsToAllPhonesInBook) & (bookNumbersNeedGetCount) ) 
              // следующий номер из книги
		      dSetFuncsSeries_FindNextNumber; 
            else 
            {
			  // заканчиваем рассылку смс
              dSetSendSmsToAllPhonesInBook_Off;
              dSetNeedSendSmsOnPower220_Alarm_PowerOff_Off;
              dStopSecondsPowerAlarm;
              dSetFuncsSeries_TestModem_PerHour; // после того, как отправили смс, баланс изменился - обновляем его
            }
          }
          break;
        
		/// уровень сигнала
		case msgGsm_Level:
          ParseReservedLevelGsm();
          break;
        
		// информация о телефонной книге 
		// количество номеров 
		// используем как флаг, что модем полностью настроен
		case msgGsm_BookInfo: 
          dParseAnswerBookInfo();
          break;
		  
        case msgGsm_Status:
          // весь парсинг заключается в проверке нуля в ответе
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