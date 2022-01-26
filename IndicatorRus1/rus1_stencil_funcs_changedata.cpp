
// модуль функций-"трафаретов" изменения данных

#define rus1_stencil_funcs_changedata

#include "msp430G2553.h"
#include "rus1.hpp"   // псевдофункции
#include "rus1_screenLCD_AC-162A.hpp"
#include "example_base_functions.hpp"
#include "rus1_stencil_funcs_changedata.hpp"
#include "example_base_strings.hpp"
#include "example_options.hpp"

/////////////////////////
// изменение данных
// упрощённый код, под 2-строчный дисплей
/////////////////////

// Изменение на геометрически изменяющееся значение (1, 10, 100, ...), и отображение новых данных на экране.
// Порядок значения указывается положением мигающего курсора. Положение курсора изменяется длинельностью нажатия 
// кнопок "вверх" и "вниз".
// В прерывании таймера изменяется порядок числа GetLnChange, следя за длительности нажатия клавиш по времени.
void Data_ChangeWithCursor() // строка заголовка
{
  // if (NULL != currentTreeNode->pDataForChange)
  if (isNeedSaveData) // если пора применить изменённые данные
  {
    t_dataForChangeWithCursor* pData = (t_dataForChangeWithCursor*)currentTreeNode->pDataForChange;
    // принимаем данные
    SaveData(pData->data);
    // сбрасываем "флаг" надобности применения изменённых данных
    dSetNeedSaveData_Off;
  }
  else // if (isNeedSaveData)
  {
    t_dataForChangeWithCursor* pData = (t_dataForChangeWithCursor*)currentTreeNode->pDataForChange;
    switch (GetRRP)
    {
      case rrp_0: //  нет специального режима (обычная работа)
        // заголовок
        LCD_ShowGoString(pData->strZagolovok, pData->strZagolovokCnt,0);
        LCD_ClearCharsToEndFromRow0(pData->strZagolovokCnt);
        // в строке для данных выводим строку перед значением
        LCD_ShowString(pData->strBefore, pData->strBeforeCnt, 0, 1);
        // готовимся к режиму редактирования
        dResetLnChange;
        dResetDeltaChange;
        break;
      case rrp_1: //  редактирование переменной (и пока не произошло изменения)
        // заголовок
        LCD_ShowString(lineLCD_Enter, lcdColCount, 0, 0);
        // готовимся к началу работы по редактированию переменной с использованием мигающего курсора экрана
        // при этом мигающий курсор указывает на символ, изменяемый пользователем клавишей "вверх/вниз"        
        dSetIsCursorVisiblePosition_Need_ON;
        dLCD_SetCursorBlinkOn;        
        break;
      case rrp_2: //  редактирование переменной (изменение "вверх")
        // вычисляем новое значение (вывод нового значения - после switch)
        if ( ( ((signed long)(*pData->data) + (signed long)GetDeltaChange) + (signed long)dPow10(GetLnChange)) > (signed long)pData->dataMax )
          GetDeltaChange = (signed long)pData->dataMax - (signed long)(*pData->data);
        else
          GetDeltaChange += dPow10(GetLnChange);
        break;
      case rrp_3: //  редактирование переменной (изменение "вниз")
        // вычисляем новое значение (вывод нового значения - после switch)
        if ( ( ((signed long)(*pData->data) + (signed long)GetDeltaChange) - (signed long)dPow10(GetLnChange)) < (signed long)pData->dataMin )
          GetDeltaChange = (signed long)pData->dataMin - (signed long)(*pData->data);
        else
          GetDeltaChange -= dPow10(GetLnChange);
        break;
      // case rrp_4: //  после вывода сообщения об успешно выполненной операции // висит сообщение!
        // мигающий курсор экрана убирают в обработчике нажатия Enter/Escape
      // case rrp_5: //  после сообщения об отменённой операции        
      //   return;
    default:
      return;
    }
    signed long new_data = (*((signed long*)(pData->data))) + GetDeltaChange;
    // учитываем возможное изменение длины данных при использовании мигающего курсора экрана
    unsigned char maxLn = GetLengthOfSignedLong(new_data)-1; // ln отсчёт с нуля
    dSetLnChangeMax(maxLn);
    // (базовая_координата_курсора_Х) = (отступ) + (длина_данных)
    unsigned int baseCoordX = pData->strBeforeCnt + maxLn;
    dSetBaseXCursor(baseCoordX);    
    // выводим новое значение на экран
    LCD_ShowLong(new_data, pData->strBeforeCnt,1);
    // и строку после этого значения
    LCD_ShowString(pData->strAfter, pData->strAfterCnt, ++baseCoordX, 1);
    LCD_ClearCharsToEndFromRow1(baseCoordX + pData->strAfterCnt);
    // символ "ввод"
    if (rrp_0 == GetRRP)
      GetCharInRow1[lcdColCount-1] = cursorEnter;
    
#ifdef use_WatchDog_for_PUC
    // перед вызовом подпрограмм вывода информации на экран, сбрасываем сторожевой таймер
	dWatchDogReset; 
#endif
  } // if (isNeedSaveData) .. else
}

// Изменение по массиву допустимых значений (строки), и отображение новых данных на экране.
// Сохраняет или не сохраняет настройки, в зависимости от выбора пользователя.
void Data_ChangeIndex_ArrayStr() // строка заголовка
{
  // if (NULL != currentTreeNode->pDataForChange)
  // если пора применить изменённые данные
  if (isNeedSaveData) 
  {
    t_dataForChangeIndex_ArrayStr* pData = (t_dataForChangeIndex_ArrayStr*)currentTreeNode->pDataForChange;
    // принимаем данные
    SaveData(pData->dataIndex);
    // сбрасываем "флаг" надобности применения изменённых данных
    dSetNeedSaveData_Off;
  }
  else // if (isNeedSaveData)
  {
    t_dataForChangeIndex_ArrayStr* pData = (t_dataForChangeIndex_ArrayStr*)currentTreeNode->pDataForChange;
    switch (GetRRP)
    {
      //  нет специального режима (обычная работа)
	  case rrp_0: 
        // заголовок
        LCD_ShowGoString(pData->strZagolovok, pData->strZagolovokCnt,0);
        LCD_ClearCharsToEndFromRow0(pData->strZagolovokCnt);
        // готовимся к режиму редактирования
        dResetDeltaChange;
        break;
      
	  //  редактирование переменной (и пока не произошло изменения)
	  case rrp_1: 
        // заголовок
        LCD_ShowString(lineLCD_Check, lcdColCount, 0, 0);
        break;
      
	  //  редактирование переменной (изменение "вверх")
	  case rrp_2: 
        // вычисляем новое значение (вывод нового значения - после switch)
        if ( ( (*((signed long*)(pData->dataIndex))) + (signed long)GetDeltaChange) < (signed long)(pData->dataMasCount-1) )
          GetDeltaChange++;
        break;
      case rrp_3: //  редактирование переменной (изменение "вниз")
        // вычисляем новое значение (вывод нового значения - после switch)
        if ( ( (*((signed long*)(pData->dataIndex))) + (signed long)GetDeltaChange) > (signed long)0 )
          GetDeltaChange--;
        break;
		
      // case rrp_4: //  после вывода сообщения об успешно выполненной операции // висит сообщение!
      // case rrp_5: //  после сообщения об отменённой операции        
      //   return;
	  
      default:
        return;
    }
	
    // выводим новое значение на экран
    LCD_ShowString(pData->dataMas[*pData->dataIndex + GetDeltaChange], lcdColCount, 0, 1);
    // символ "ввод"
    if (rrp_0 == GetRRP)
      GetCharInRow1[lcdColCount-1] = cursorEnter;

#ifdef use_WatchDog_for_PUC
    // перед вызовом подпрограмм вывода информации на экран, сбрасываем сторожевой таймер
	dWatchDogReset; 
#endif
  } // if (isNeedSaveData) .. else
}
/*
// Изменение данных, используя "готовый фиксированный GetLnChange->GetDeltaChange, и отображение новых данных на экране.
// Сохраняет или не сохраняет настройки, в зависимости от выбора пользователя.
void Data_ChangeFixGetLnChange(signed long data, // значение настройки, которую изменяем
                               unsigned int lnChange, // какое число, с младшего, изменяем, нумерация с нуля
                               const unsigned long  dataMin, // минимально допустимое значение настройки
                               const unsigned long  dataMax, // максимально допустимое значение настройки
                               const unsigned char* strBefore, const unsigned char strBeforeCnt, // строка перед выводом нового значения (он же служит отступом)
                               const unsigned char* strAfter,  const unsigned char strAfterCnt, // строка после вывода нового значения
                               const unsigned char* strZagolovok, const unsigned char strZagolovokCnt) // строка заголовка
{    
    switch (GetRRP)
    {
      case rrp_0: //  нет специального режима (обычная работа)
        // заголовок
        LCD_ShowGoString(strZagolovok, strZagolovokCnt,0);
        LCD_ClearCharsToEndFromRow0(strZagolovokCnt);
        // в строке для данных выводим строку перед значением
        LCD_ShowString(strBefore, strBeforeCnt, 0, 1);
        // готовимся к режиму редактирования
        dResetDeltaChange;
        break;
      case rrp_1: //  редактирование переменной (и пока не произошло изменения)
        // заголовок
        LCD_ShowString(lineLCD_Enter, lcdColCount,0,0);
        break;
      case rrp_2: //  редактирование переменной (изменение "вверх")
        // вычисляем новое значение (вывод нового значения - после switch)
        if ( ( ((signed long)data + (signed long)GetDeltaChange) + (signed long)dPow10(lnChange)) > (signed long)dataMax )
          GetDeltaChange = (signed long)dataMax - (signed long)data;
        else
          GetDeltaChange += dPow10(lnChange);
        break;
      case rrp_3: //  редактирование переменной (изменение "вниз")
        // вычисляем новое значение (вывод нового значения - после switch)
        if ( ( ((signed long)data + (signed long)GetDeltaChange) - (signed long)dPow10(lnChange)) < (signed long)dataMin )
          GetDeltaChange = (signed long)dataMin - (signed long)data;
        else
          GetDeltaChange -= dPow10(lnChange);
        break;
      // case rrp_4: //  после вывода сообщения об успешно выполненной операции // висит сообщение!
      // case rrp_5: //  после сообщения об отменённой операции        
      //   return;
    default:
      return;
    }
    signed long new_data = data + GetDeltaChange;
    // учитываем возможное изменение длины данных при использовании мигающего курсора экрана
    unsigned char maxLn = GetLengthOfSignedLong(new_data)-1; // ln отсчёт с нуля
    dSetLnChangeMax(maxLn);
    // (базовая_координата_курсора_Х) = (отступ) + (длина_данных)
    unsigned int baseCoordX = strBeforeCnt + maxLn;
    // выводим новое значение на экран
    LCD_ShowLong(new_data, strBeforeCnt,1);
    // и строку после этого значения
    LCD_ShowString(strAfter, strAfterCnt, ++baseCoordX, 1);
    LCD_ClearCharsToEndFromRow1(baseCoordX + strAfterCnt);
    // символ "ввод"
    if (rrp_0 == GetRRP)
      GetCharInRow1[lcdColCount-1] = cursorEnter;
    
#ifdef use_WatchDog_for_PUC
    dWatchDogReset; // перед вызовом подпрограмм вывода информации на экран, сбрасываем сторожевой таймер
#endif
}

// Изменение по массиву допустимых значений (числа), и отображение новых данных на экране.
// Сохраняет или не сохраняет настройки, в зависимости от выбора пользователя.
// GetDeltaChange - разница между старым индексом массива и новым
void Data_ChangeIndex_ArrayLong(unsigned char dataIndex, // значение настройки, которую изменяем [индекс]
                              const unsigned long* dataMas, // массив допустимых значений
                              const unsigned int   dataMasCount, // размерность // массив допустимых значений
                              const unsigned char* strBefore, const unsigned char strBeforeCnt, // строка перед выводом нового значения (он же служит отступом)
                              const unsigned char* strAfter,  const unsigned char strAfterCnt, // строка после вывода нового значения
                              const unsigned char* strZagolovok, const unsigned char strZagolovokCnt) // строка заголовка
{    
    switch (GetRRP)
    {
      case rrp_0: //  нет специального режима (обычная работа)
        // заголовок
        LCD_ShowGoString(strZagolovok, strZagolovokCnt,0);
        LCD_ClearCharsToEndFromRow0(strZagolovokCnt);
        // в строке для данных выводим строку перед значением
        LCD_ShowString(strBefore, strBeforeCnt, 0, 1);  
        // готовимся к режиму редактирования
        dResetDeltaChange;
        break;
      case rrp_1: //  редактирование переменной (и пока не произошло изменения)
        // заголовок
        LCD_ShowString(lineLCD_Check, lcdColCount,0,0);
        break;
      case rrp_2: //  редактирование переменной (изменение "вверх")
        // вычисляем новое значение (вывод нового значения - после switch)
        if ( ((signed int)dataIndex + (signed int)GetDeltaChange) < (signed int)(dataMasCount-1) )
          GetDeltaChange++;
        break;
      case rrp_3: //  редактирование переменной (изменение "вниз")
        // вычисляем новое значение (вывод нового значения - после switch)
        if ( ((signed int)dataIndex + (signed int)GetDeltaChange) > (signed int)0 )
          GetDeltaChange--;
        break;
      // case rrp_4: //  после вывода сообщения об успешно выполненной операции // висит сообщение!
      // case rrp_5: //  после сообщения об отменённой операции        
      //   return;
    default:
      return;
    }
    signed long new_data = *(dataMas + (dataIndex + GetDeltaChange));
    // учитываем возможное изменение длины данных
    // (базовая_координата_курсора_Х) = (отступ) + (длина_данных)
    unsigned int baseCoordX = strBeforeCnt-1 + GetLengthOfSignedLong(new_data);
    // выводим новое значение на экран
    LCD_ShowLong(new_data, strBeforeCnt,1);
    // и строку после этого значения
    LCD_ShowString(strAfter, strAfterCnt, ++baseCoordX, 1);
    LCD_ClearCharsToEndFromRow1(baseCoordX + strAfterCnt);
    // символ "ввод"
    if (rrp_0 == GetRRP)
      GetCharInRow1[lcdColCount-1] = cursorEnter;
    
#ifdef use_WatchDog_for_PUC
    dWatchDogReset; // перед вызовом подпрограмм вывода информации на экран, сбрасываем сторожевой таймер
#endif
}
*/