// модуль по работе с LCD-экраном 
// модуль для подпрограмм, не использующих модули из rus1*.* и не связанных с флагом use_IndRUS1
// тестировался на LCD-экране типа AC-162A

#define rus1_screenLCD_AC_162A

#include "msp430G2553.h"

/////////////////////////////////////
// здесь подпрограммы, при работе с LCD-экраном 
// использующие только:
//  - стандартные модули из IAR;
//  - модуль base_functions;
/////////////////////////////////////

#include "rus1_screenLCD_AC-162A.hpp"
#include "rus1_peripherals.hpp"


// выведенный текст на ЖК индикаторе
unsigned char f_lcdText[lcdRowCount][lcdColCount];

// буфер вывода текста на ЖК индикатор [когда прибор занят]
// нужен чтобы не переключать LCD панель с кнопками в режим LCD, когда символ уже выведен, а ждать нажатия кнопки
unsigned char f_lcdTextBuffer[lcdRowCount][lcdColCount];




// для использования в таймере обновления текста
// для постепенного обновления текста
unsigned char* pLcdText       = &f_lcdText[0][0];       
// буфер для постепенного занесения в pLcdText
// нужен чтобы не переключать в таймере LCD-панель в режим LCD (когда символ уже выведен), а ждать нажатия кнопок
unsigned char* pLcdTextBuffer = &f_lcdTextBuffer[0][0]; 
// последний символ, выведенный на дисплей 
// учитываем автоматический сдвиг символа вправо на дисплее
unsigned char* pLastSimbolLCD = NULL; // чтобы лишний раз не посылать команду позиционирования

///////////////
// подпрограммы
///////////////

// очищает дисплей // для экономии места на флешке
void LCD_Clear() 
{
   for (unsigned char* pTemp = pLcdBufBegin; pTemp<pLcdBufBegin+(lcdColCount*lcdRowCount); pTemp++)
     *pTemp = ' '; 
}
// выводит команду на LCD-дисплей максимально быстро
// последняя пауза не выполняется, т.к. работа с LCD-дисплеем - только в след. прерывание таймера
void LCD_WriteCommandQuickly(unsigned char data) { \
        dLcdRsOff; \
        dDelayTAS; \
        \
        dLcdE_On; \
        __delay_cycles(5); \
        dLcd_SetData(data >> 4); \
        __delay_cycles(4); \
        dLcdE_Off; \
        \
        dDelayToCycleE; \
        \
        dLcdE_On; \
        __delay_cycles(5); \
        dLcd_SetData(data&0x0F); \
        __delay_cycles(4); \
        dLcdE_Off; \
        }

// возвращает код числа в таблице символов LCD-контроллера
// 0x00 - 0x0F совпадает с ANSI-1251
const unsigned char numbersForCharHex[0x10] = 
// 0   1   2   3   4   5   6   7   8   9    10   11   12   13   14   15
{ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 'A', 'B', 'C', 'D', 'E', 'F'};
unsigned char LCD_NumberToCharHex(unsigned char val)
    {
      if (val>0x0F)
        return(0x5F); // символ "_"
      else return(numbersForCharHex[val]);
    }
         

// таблица для быстрого перевода русских букв в кодировку кодовой страницы lcd-дисплея типа AC-162A
// для каждого индикатора таблица кодировки русских букв может отличаться
// 0x5F = '_'
const unsigned char lcd_tab_rus[192+64] = {
// 0   1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,   // 0F
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,   // 1F
 ' ',  '!',  '"',  '#',  '$',  '%',  '&', 0x27,  '(',  ')',  '*',  '+',  ',',  '-',  '.', 0x2F,   // 2F
 '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',   // 3F
 '@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',   // 4F
 'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '[', 0x5C, ']',  '^',  '_',    // 5F
 '`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',   // 6F
 'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,   // 7F
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x7E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,   // 8F
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,   // 9F
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0xA2, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,   // AF
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0xB5, 0x5F, 0x5F, 0xDC, 0x5F, 0x5F, 0x5F, 0x5F,   // BF
 'A', 0xA0,  'B', 0xA1, 0xE0,  'E', 0xA3, 0xA4, 0xA5, 0xA6,  'K', 0xA7,  'M',  'H',  'O', 0xA8,   // CF
 'P',  'C',  'T', 0xA9, 0xAA,  'X', 0xE1, 0xAB, 0xAC, 0xE2, 0xAD, 0xAE,  'b', 0xAF, 0xB0, 0xB1,   // DF
 'a', 0xB2, 0xB3, 0xB4, 0xE3, 0x65, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD,  'o', 0xBE,   // EF
 'p',  'c', 0xBF,  'y', 0xE4,  'x', 0xE5, 0xC0, 0xC1, 0xE6, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7 }; // FF 
// выводит строку в координатах unsigned char xCoord, unsigned char yCoord
// если выходит за ПОСЛЕДНЮЮ строку - обрезается, иначе переносится на другую строку
// упрощённый код, под 2-строчный дисплей
void LCD_ShowString(unsigned char const* dataVal, unsigned char val_len, unsigned char xCoord, unsigned char yCoord)
{
  // если выходит за ПОСЛЕДНЮЮ строку - обрезается, иначе переносится на другую строку
  unsigned char* dataXCoordBegin = pLcdBufBegin + xCoord;
  unsigned char* dataXCoordEnd = dataXCoordBegin + val_len;  
  // выводит строку в координатах unsigned char xCoord, unsigned char yCoord;
  if (yCoord) 
  {
    dataXCoordBegin += lcdColCount;
    dataXCoordEnd += lcdColCount;
  }
  if (dataXCoordEnd > (pLcdBufEnd+1)) 
    dataXCoordEnd = (pLcdBufEnd+1);
  for (unsigned char* index=dataXCoordBegin;index<dataXCoordEnd;index++, dataVal++)
  //   *index = lcd_tab_rus[*dataVal]; т.к. надо для сообщения использовать буфер, и 32 байта ОЗУ для него выделять - слишеом много
    *index = *dataVal;
}
/*
void LCD_ShowString_(unsigned char const* dataVal, unsigned char val_len, unsigned char* dataXCoordBegin)
{
  // если выходит за ПОСЛЕДНЮЮ строку - обрезается, иначе переносится на другую строку
  unsigned char* dataXCoordEnd = dataXCoordBegin + val_len;  
  // выводит строку в координатах unsigned char xCoord, unsigned char yCoord;
  if (dataXCoordEnd > (pLcdBufEnd+1)) 
    dataXCoordEnd = (pLcdBufEnd+1);
  for (unsigned char* index=dataXCoordBegin;index<dataXCoordEnd;index++, dataVal++)
    *index = lcd_tab_rus[*dataVal];
}
*/
/*
// для уменьшения размера кода
// полностью обновляет экран, выводя строку размера [lcdColCount * lcdRowCount]
// строка обязательно должна быть размера [lcdColCount * lcdRowCount] !
void LCD_FullUpdateFromString( const unsigned char* dataVal )
{
  for (unsigned char* index=pLcdBufBegin;index<pLcdBufEnd;index++, dataVal++)
    *index = lcd_tab_rus[*dataVal];
}
*/
/*
    // выводит float со знаком в заданные координаты; если выходит за экран - обрезается
    //               значение    число цифр в дроб. части  координата Х          координата Y          надо ли чистить до конца строки
    // упрощённый код, под 2-строчный дисплей
void LCD_ShowFloat( float fval, unsigned char frac_Count, unsigned char xCoord, unsigned char yCoord, bool isClearToEndOfLine )
    {
      // выводим целую часть
      signed long int_val = (signed long)fval;
      LCD_ShowLong( int_val, xCoord, yCoord, (frac_Count==0) & isClearToEndOfLine );
      // выводим дробную часть
      if (frac_Count>0)
      {
        // выводим точку
        unsigned char posXNext = xCoord + GetLengthOfSignedLong(int_val);
        unsigned char* dotPoint = pLcdBufBegin + posXNext;
        if (yCoord) dotPoint += lcdColCount;
        *dotPoint = '.';
        posXNext++;
        // выводим дробную часть
        signed long int_frac = dRound( (fval - int_val)*dPow10(frac_Count) );
        LCD_ShowLong( int_frac, posXNext, yCoord, isClearToEndOfLine );
      }
    }    
*/