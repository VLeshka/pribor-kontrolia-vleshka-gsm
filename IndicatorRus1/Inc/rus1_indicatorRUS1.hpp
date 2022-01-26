
// модуль работы с индикатором (LCD-панель + 4 кнопки) ООО "НПО Наука"

#ifndef __rus1_indicatorRUS1__
#define __rus1_indicatorRUS1__

#include "rus1_mainTitle.hpp"

//////////////////// 
// периферия кнопок
////////////////// 

// надо чтобы кнопки были "по убыванию", чтобы запись в LCD_WriteByte была корректна 
// кнопка "вверх"
#define indRus1_ButtonUp BIT3 // 7
// кнопка "вниз"
#define indRus1_ButtonDown BIT2 // 6
// кнопка "влево"
#define indRus1_ButtonEnter BIT1 // 5
// кнопка "вправо"
#define indRus1_ButtonEscape BIT0 // 4

// порт где настраиваются кнопки
#define indRus1_InPort P2IN

// сумма всех кнопок
#define indRus1_ButtonAll (indRus1_ButtonUp | indRus1_ButtonDown | indRus1_ButtonEnter | indRus1_ButtonEscape)

#ifdef use_ButtonsShift

// Shift-кнопки
#define indRus1_ButtonsShift (indRus1_ButtonEnter+indRus1_ButtonEscape)

// "до события": выдерживаемая пауза от дребезга, по истечении которой происходит СОБЫТИЕ нажатия кнопки
// "после события", пост-нажатие: выдерживаемая пауза после СОБЫТИЯ нажатия кнопки

// масска в buttonDelayWaitFlag: нажата ли "до события" Shift-кнопка
#define buttonBounceDelay_Shift ( indRus1_ButtonsShift )

// масска в buttonDelayWaitFlag: нажата ли "после события" Shift-кнопка
#define buttonPostDelay_Shift ( indRus1_ButtonsShift << 8)

// масска в buttonDelayWaitFlag: нажата ли "до события" или "после события" любая Shift-кнопка
#define buttonBounceAndPostDelayMask_Shift ( (indRus1_ButtonsShift << 8) | indRus1_ButtonsShift )

// масска в buttonDelayWaitFlag: нажата ли "до события" или "после события" Shift-кнопка Esc
#define buttonBounceAndPostDelay_Esc ( (indRus1_ButtonEscape << 8) | indRus1_ButtonEscape )

#endif // #ifdef use_ButtonsShift



// индикатор в режиме LCD
#define isIndRus1_RezhimLCD (portLcdDirData & indRus1_ButtonAll)

// ожидаем ли паузу от дребезга кнопки
// после того, как зафиксировали нажатие кнопки, и ждём паузу от дребезга, чтобы произвести событие успешного нажатия кнопки 
#define isWaitForButtonBounceDelay (buttonMask_Pressed & 0x00FF)
// ожидаем ли паузу после события успешного нажатия кнопки 
#define isWaitForButtonPostDelay   (buttonMask_Pressed & 0xFF00)

// нажата ли кнопка "вверх"
#define isButtonUpPressed     (0==(indRus1_InPort & indRus1_ButtonUp))
// нажата ли кнопка "вниз"
#define isButtonDownPressed   (0==(indRus1_InPort & indRus1_ButtonDown))
// нажата ли кнопка "влево"
#define isButtonEnterPressed  (0==(indRus1_InPort & indRus1_ButtonEnter))
// нажата ли кнопка "вправо"
#define isButtonEscapePressed (0==(indRus1_InPort & indRus1_ButtonEscape))





// настраиваем индикатор РУС1 на вывод информации на LCD
#define dIndRus1_SetRezhimLCD {portLcdDirData |= indRus1_ButtonAll; /*Регистр направления PxDIR =1 на выход*/ \
                         portLcdRenData &= (~indRus1_ButtonAll); /*запрещаем подтяжку*/ \
                         portLcdOutData &= (~indRus1_ButtonAll); /*выходной сигнал = 0*/ \
                         /*__delay_cycles(5);*/ } /*пауза от переходного процесса на ножке порта*/

// настраиваем индикатор РУС1 на ожидание прерываний от кнопок
#define dIndRus1_SetRezhimButton {portLcdDirData &= (~indRus1_ButtonAll); /*Регистр направления PxDIR // =0 на вход*/ \
                         portLcdRenData |= indRus1_ButtonAll; /*разрешаем подтяжку*/ \
                         portLcdOutData |= indRus1_ButtonAll; } /*подтяжка выводов к питанию*/

// очистка маски нажатых кнопок
#define dButtonMask_Pressed_Clear   { buttonMask_Pressed = 0x00; }


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// подпрограммы обработки нажатия кнопок
// вызываются только по 1 разу, поэтому переведены в define
// используем только когда спецрежим 6 не включён
// спецрежим 6 сбрасывается при нажатии любой кнопки (см. rus1_interruptTimer.cpp -> обслуживаем кнопки)
////////////////////////////////////////

// кнопка "вверх"
// #pragma inline=forced //  требуется Optimization не ниже medium, но при такой оптимизации
// нельзя упаковывать в памяти константы и теряется переносимость кода
// void IndRUS1_PressButtonUp( void )
#define dIndRUS1_PressButtonUp \
  { \
    switch (GetRRP) \
      { \
        case rrp_0: /* инфо показывает другая функция (об ошибке, заставка)*/ \
          Tree_PreviousTreeNode(); \
          break; \
        case rrp_1: /* нет специального режима (обычная работа)*/ \
        case rrp_2: /* редактирование переменной (изменение "вверх")*/ \
        case rrp_3: /* редактирование переменной (изменение "вниз")*/ \
          SetRRP(rrp_2); \
          currentTreeNode->pFunctionShowInfo(); /*здесь явно FunctionShowInfo != NULL*/ \
          break; \
        case rrp_4: /* после вывода сообщения об успешно выполненной операции*/ \
        case rrp_5: /* после вывода сообщения об отменённой операции*/ \
          dResetIndexShowGoString; \
          currentTreeNode->pFunctionShowInfo(); \
          SetRRP(rrp_0); \
          break; \
        default: \
          break; \
      } \
  }
  
  // кнопка "вниз"
// void IndRUS1_PressButtonDown( void )
#define dIndRUS1_PressButtonDown \
  { \
    switch (GetRRP) \
      { \
        case rrp_0: /* инфо показывает другая функция (об ошибке, заставка)*/ \
          Tree_NextTreeNode(); \
          break; \
        case rrp_1: /* нет специального режима (обычная работа)*/ \
        case rrp_2: /* редактирование переменной (изменение "вверх")*/ \
        case rrp_3: /* редактирование переменной (изменение "вниз")*/ \
          SetRRP(rrp_3); \
          currentTreeNode->pFunctionShowInfo(); /*здесь явно FunctionShowInfo != NULL*/ \
          break; \
        case rrp_4: /* после вывода сообщения об успешно выполненной операции*/ \
        case rrp_5: /* после вывода сообщения об отменённой операции*/ \
          dResetIndexShowGoString; \
          currentTreeNode->pFunctionShowInfo(); \
          SetRRP(rrp_0); \
          break; \
        default: \
          break; \
      } \
  }

  // кнопка "Enter"
// void IndRUS1_PressButtonEnter( void )
#define dIndRUS1_PressButtonEnter \
  { \
    switch (GetRRP) \
      { \
        case rrp_0: /* нет специального режима (обычная работа)*/ \
          if (isCurrentTreeNode_ChangableData) \
          { \
            SetRRP(rrp_1); \
            currentTreeNode->pFunctionShowInfo(); \
          } \
          else \
            Tree_EnterChildTreeNode(); \
          break; \
        case rrp_1: /* редактирование переменной (и пока не произошло изменения)*/ \
        case rrp_2: /* редактирование переменной (изменение "вверх")*/ \
        case rrp_3: /* редактирование переменной (изменение "вниз")*/ \
          if (GetIsCursorVisible) \
            LCD_SetCursorBlinkOff(); /*Выключение индикатора, курсор выключен *//*выход из редактирования*/ \
          SetRRP(rrp_4); \
          dSetIsReInitLCD_ON; /*когда завершаем интенсивную работу с мигающим курсором, лучше переинициализировать дисплей*/ \
          LCD_ShowString(lineVupolnenoOk, lcdColCount*lcdRowCount, 0, 0); \
          currentTreeNode->pFunctionShowInfo(); /*сохранение данных*/ \
          break; \
        case rrp_4: /* после вывода сообщения об успешно выполненной операции*/ \
        case rrp_5: /* после вывода сообщения об отменённой операции*/ \
          dResetIndexShowGoString; \
          currentTreeNode->pFunctionShowInfo(); \
          SetRRP(rrp_0); \
          break; \
        default: \
          break; \
      } \
  }

  // кнопка "Cancel"
// void IndRUS1_PressButtonEscape( void )
#define dIndRUS1_PressButtonEscape \
  { \
    switch (GetRRP) \
      { \
        case rrp_0: /* нет специального режима (обычная работа)*/ \
          Tree_OutUpTreeNode(); \
          break; \
        case rrp_1: /* редактирование переменной (и пока не произошло изменения)*/ \
        case rrp_2: /* редактирование переменной (изменение "вверх")*/ \
        case rrp_3: /* редактирование переменной (изменение "вниз")*/ \
          if (GetIsCursorVisible) \
            LCD_SetCursorBlinkOff(); /*Выключение индикатора, курсор выключен *//*выход из редактирования*/ \
          SetRRP(rrp_5); \
          dSetIsReInitLCD_ON; /*когда завершаем интенсивную работу с мигающим курсором, лучше переинициализировать дисплей*/ \
          LCD_ShowString(lineCancelOk, lcdColCount*lcdRowCount, 0, 0); \
          break; \
        case rrp_4: /* после вывода сообщения об успешно выполненной операции*/ \
        case rrp_5: /* после вывода сообщения об отменённой операции*/ \
          dResetIndexShowGoString; \
          currentTreeNode->pFunctionShowInfo(); /*здесь явно FunctionShowInfo != NULL*/ \
          SetRRP(rrp_0); \
          break; \
        default: \
          break; \
      } \
  }


/////////////
// extern
//////////// 

// маска, показывающая, что обрабатывается нажатие кнопки
// когда != 0, обрабатывается кнопка
// биты от BIT0 до BIT7 совпадают с реальными ножками indRus1_InPort, показывают что
//    произошло нажатие кнопки, и происходит ожидание паузы для защиты от дребезга
// биты от BIT8 до BIT15 (= "реальные ножки indRus1_InPort" << 8) - успешно прождали паузу от дребезга,  
//    определили событие нажатия кнопки, и ожидаем паузу после события нажатия кнопки
extern unsigned int buttonMask_Pressed;

// счётчик прерываний таймера у кнопки
extern unsigned int interruptsIndRus1_ButtonCnt;

// счётчик срабатывания таймера lcd для изменения переменной [через дисплей]
extern unsigned int f_timerCounterChangePerem;
// счётчик срабатывания таймера lcd для изменения deltaChange, при нажатой кнопке
extern unsigned int f_timerCounterLnChangeKeyPressed;
// счётчик срабатывания таймера lcd для изменения deltaChange, при ненажатой кнопке
extern unsigned int f_timerCounterLnChangeKeyUnPressed;



///////////////////////////
// прототипы подпрограмм
///////////////////////////

// настраиваем индикатор РУС1 на вывод информации на LCD
void IndRus1_SetRezhimLCD();

// настраиваем индикатор РУС1 на ожидание прерываний от кнопок
void IndRus1_SetRezhimButton();

#endif // #ifndef __rus1_indicatorRUS1__