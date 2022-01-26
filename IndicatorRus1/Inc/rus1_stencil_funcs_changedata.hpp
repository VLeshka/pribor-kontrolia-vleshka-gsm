
// .h-файл модуля функций-"трафаретов" изменения данных

#ifndef __rus1_stencil_funcs_changedata__
#define __rus1_stencil_funcs_changedata__

#include "rus1_mainTitle.hpp"

// для void Data_ChangeWithCursor(); 
struct t_dataForChangeWithCursor
{
  // значение настройки, которую изменяем
  signed long* data; 
  // минимально допустимое значение настройки
  const unsigned long  dataMin; 
  // максимально допустимое значение настройки
  const unsigned long  dataMax; 
  // строка перед выводом нового значения (он же служит отступом)
  const unsigned char* strBefore; 
  // строка после вывода нового значения
  const unsigned char* strAfter; 
  // строка заголовка
  const unsigned char* strZagolovok; 
  // размер 
  // строка перед выводом нового значения (он же служит отступом)
  const unsigned char strBeforeCnt; 
  // размер 
  // строка после вывода нового значения
  const unsigned char strAfterCnt; 
  // размер 
  // строка заголовка
  const unsigned char strZagolovokCnt; 
};

// для void Data_ChangeIndex_ArrayStr();
struct t_dataForChangeIndex_ArrayStr
{
  // значение настройки, которую изменяем [индекс]
  signed long* dataIndex; 
  // массив указателей на строки размера lcdColCount
  const unsigned char** dataMas; 
  const unsigned char* strZagolovok; 
  // размерность 
  // массив допустимых значений
  const unsigned char dataMasCount; 
  // строка заголовка
  const unsigned char strZagolovokCnt; 
};



//////////////////////////////////// 
// при использовании индикатора РУС-1 
// изменение данных
/////////////////////

// Изменение на геометрически изменяющееся значение (1, 10, 100, ...), и отображение новых данных на экране.
// Порядок значения указывается положением мигающего курсора. Положение курсора изменяется длинельностью нажатия 
// кнопок "вверх" и "вниз".
// В прерывании таймера изменяется порядок числа GetLnChange, следя за длительности нажатия клавиш по времени.
void Data_ChangeWithCursor(); 

// Изменение по массиву допустимых значений (строки), и отображение новых данных на экране.
// Сохраняет или не сохраняет настройки, в зависимости от выбора пользователя.
void Data_ChangeIndex_ArrayStr();
/*
// Изменение данных, используя "готовый фиксированный GetLnChange->GetDeltaChange, и отображение новых данных на экране.
// Сохраняет или не сохраняет настройки, в зависимости от выбора пользователя.
void Data_ChangeFixGetLnChange(signed long data, // значение настройки, которую изменяем
                               unsigned int lnChange, // какое число, с младшего, изменяем, нумерация с нуля
                               const unsigned long  dataMin, // минимально допустимое значение настройки
                               const unsigned long  dataMax, // максимально допустимое значение настройки
                               const unsigned char* strBefore, const unsigned char strBeforeCnt, // строка перед выводом нового значения (он же служит отступом)
                               const unsigned char* strAfter,  const unsigned char strAfterCnt, // строка после вывода нового значения
                               const unsigned char* strZagolovok, const unsigned char strZagolovokCnt); // строка заголовка
*/
/*
// Изменение по массиву допустимых значений (числа), и отображение новых данных на экране.
// Сохраняет или не сохраняет настройки, в зависимости от выбора пользователя.
// GetDeltaChange - разница между старым индексом массива и новым
void Data_ChangeIndex_ArrayLong(unsigned char dataIndex, // значение настройки, которую изменяем [индекс]
                               const unsigned long* dataMas, // массив допустимых значений
                               const unsigned int   dataMasCount, // размерность // массив допустимых значений
                               const unsigned char* strBefore, const unsigned char strBeforeCnt, // строка перед выводом нового значения (он же служит отступом)
                               const unsigned char* strAfter,  const unsigned char strAfterCnt, // строка после вывода нового значения
                               const unsigned char* strZagolovok, const unsigned char strZagolovokCnt); // строка заголовка
*/

#endif // #ifndef __rus1_stencil_funcs_changedata__