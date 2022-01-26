// базовые функции, прототипы из языков высокого уровня
// для независимости от стандартных модулей
                 
// здесь подпрограммы, при работе с LCD-экраном использующие только
// стандартные модули из IAR

#define example_base_functions

#include "msp430G2553.h"

#include "rus1_mainTitle.hpp"

#include "example_base_functions.hpp"
#include "rus1_screenLCD_AC-162A.hpp"
#include "rus1_indicatorRUS1.hpp"
#include "rus1_peripherals.hpp"


// возвращает десятичное число из строки, содержащей 16-ричные числа в символьном виде
// шестнадцатеричные числа должны быть заглавными
unsigned long GetDecimalFromHexString(unsigned char* text, unsigned char textCnt)
{
  unsigned long res = 0;
  unsigned char* textEnd = text + textCnt;
  for (; text<textEnd; text++)
  {
    // if ( (*text > 0x2F) & (*text < 0x3A) )
    if (*text > 0x2F)
    {
      if (*text < 0x3A)
        res = res * 16 + (*text - 0x30);
      else
        if (*text > 0x40)
          if (*text < 0x47)
            res = res * 16 + (*text - 55);
      }
  }
  return(res);
}

// переводит в тексте английские и русские маленькие буквы в большие
void TranslateSmallToLarge(unsigned char* text, unsigned char textCnt)
{
  unsigned char* textEnd = text + textCnt;
  for (; text<textEnd; text++)
    if (*text > 0x60)
    {
      if (*text < 0x7B)
        *text = *text - 0x20;
      else
        // русские буквы ё Ё не учитываем - всё равно их нет 
	    // в используемых смс-командах
		// + текст смс на дисплее не показываем
        if (*text >= 0xE0)
          // if (*text <= 0xFF) // the result is allways true
            *text = *text - 0x20;
    }
}

// сравнивает 2 одинаковые по длине строки, равны ли они по содержимому
bool CompareStrings(const unsigned char* string1, const unsigned char* string2, unsigned char stringsCount)
{
  const unsigned char* pEnd = string1 + stringsCount - 1;
  for (; string1 <= pEnd; string1++, string2++)
    if (*string1 != *string2)
      return(false);
  return(true);
}

// возвращает количество цифр в числе, включая знак "минус"
unsigned char GetLengthOfSignedLong(signed long val) // от -2 147 483 648 до 2 147 483 647 = максимум 11 знаков
{
  unsigned char res = 1;
  if (val<0)
  { 
    val = dInvert(val); 
    res++; 
  }
  if (val<10) return(res); res++;
  if (val<100) return(res); res++;
  if (val<1000) return(res); res++;
  if (val<10000) return(res); res++;
  if (val<100000) return(res); res++;
  if (val<1000000) return(res); res++;
  if (val<10000000) return(res); res++;
  if (val<100000000) return(res); res++;
  if (val<1000000000) return(res); res++;
  return(res); 
}

// преобразует число в строку
void LongToStr( signed long val, unsigned char* pStr, unsigned char strCnt )
{
  unsigned char *posAllowEnd = pStr + strCnt; // с какого указателя уже нельзя выводить
  if (0 > val)
  {
    *pStr = '-';
    pStr++;
  }
  pStr = pStr + GetLengthOfSignedLong(val) - 1; // с какого указателя пытаемся выводить
  // с младшей цифры, выводим
  while (val > 9)
  {		
    // деление long на 10 самым быстрым способом
    signed long tmp = ( val >> 1 );
    tmp += (tmp >> 1);
    tmp += (tmp >> 4);
    tmp += (tmp >> 8);
    tmp += (tmp >> 16);
    tmp >>= 3;
    // вычисляем остаток
    // компилятор *10 делает 5ю регистровыми операциями
	unsigned char cifra = val - (tmp * 10); 
    // проверка на кратность 10 
	//  в этом случае остаток = 10, результат занижен на 1
    if (cifra>9)
    {
      cifra -= 10; 
      tmp++;
    }
    val = tmp;
    // заносим в результат
    if (pStr < posAllowEnd)
      *pStr = dLCD_FigureToLcdChar(cifra);
    pStr--;
  } // while (val > 9)
  if (pStr < posAllowEnd)
    *pStr = dLCD_FigureToLcdChar((unsigned char)val);
}

// пауза x мкс с точностью до такта цпу
//////////////////////////////////////////////// 
// для GetCpuFreqMHz МГц
// делаем (GetCpuFreqMHz-1) цикл ожидания с вложенным циклом, затем 1 цикл с вычетом служебных расходов
// первый цикл: (GetCpuFreqMHz-1) цикл
// вложенный цикл: ожидание для заданных мкс для 1МГц
// подготовка цикла 4 такта
// каждый цикл заниммает 7 тактов, при этом перед первым циклом проверка условия = 4 такта лишних
// 1 цикл с вычетом служебных расходов
// подготовка цикла 4 такта
// каждый цикл заниммает 7 тактов, при этом перед первым циклом проверка условия = 4 такта лишних
// подготовка к вызову пп - 2 такта (параметры вызова пп)
// вызов пп - 5 тактов
// возврат из пп - 2 такта
// подготовка цикла (GetCpuFreqMHz-1) 4 такта
// в (GetCpuFreqMHz-1) перед первым циклом проверка условия = 4 такта лишних
/////////////////
// рекомендуется > 25 мкс
void DelayMicroS(unsigned int x)
{ 
  // (GetCpuFreqMHz-1) цикл ожидания
  for (unsigned int g=1; g<(unsigned int)GetCpuFreqMHz; g++)
    for (unsigned int ii=4+4; ii<x; ii+=7); 
  // 1 цикл с вычетом служебных расходов
#ifdef use_WatchDog_for_PUC
  dWatchDogReset;
  for (unsigned int ii=2+5 + 4+4 + 4+4 +2 +5; ii<x; ii+=7);
#else
  for (unsigned int ii=2+5 + 4+4 + 4+4 +2; ii<x; ii+=7);
#endif
}

// пауза x мс
// на основании DelayMicroS
//////////////////////////////////////////////// 
// void DelayMilliS(unsigned int x) { for (unsigned int ii=1; ii<x; ii++) DelayMicroS(1000); }

// пауза x мс с точностью до такта цпу
// на основании DelayMicroS
//////////////////////////////////////////////// 
//  + при старте пп push.w R10 = 3 такта
//  + при старте пп mov.w R12,R13 = 1 такт
//  + при завершении пп pop.w R10 = 2 такта
void DelayMilliS(unsigned int x) 
{ 
  for (unsigned int ii=1; ii<x; ii++)
     DelayMicroS(1000);
  // 1 мс с вычетом служебных расходов
  for (unsigned int g=1; g<(unsigned int)GetCpuFreqMHz; g++)
    for (unsigned int ii=4+4; ii<1000; ii+=6);
#ifdef use_WatchDog_for_PUC
  dWatchDogReset;
  for (unsigned int ii=2+5 + 3+1 + 4+4 + 4+4 + 4+4 + 2 +2 +5; ii<1000; ii+=6);
#else
  for (unsigned int ii=2+5 + 3+1 + 4+4 + 4+4 + 4+4 + 2 +2; ii<1000; ii+=6);
#endif
}
