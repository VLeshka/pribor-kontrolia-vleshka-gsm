
// .h-файл модуля по работе с LCD-экраном 

#ifndef __rus1_screenLCD_AC_162A__
#define __rus1_screenLCD_AC_162A__

#include "rus1_mainTitle.hpp"

#include "example_base_functions.hpp"

// для использования в таймере постепенного обновления текста
extern unsigned char* pLcdText;       
// буфер для постепенного занесения в pLcdText
// нужен чтобы не переключать в таймере LCD-панель в режим LCD (когда символ уже выведен), а ждать нажатия кнопок
extern unsigned char* pLcdTextBuffer; 
// последний символ, выведенный на дисплей 
// учитываем автоматический сдвиг символа вправо на дисплее
extern unsigned char* pLastSimbolLCD; // чтобы лишний раз не посылать команду позиционирования

// таблица для быстрого перевода русских букв в кодировку кодовой страницы lcd-дисплея типа AC-162A
// для каждого индикатора таблица кодировки русских букв может отличаться
// 0x5F = '_'
extern const unsigned char lcd_tab_rus[192+64];

// количество строк и столбцов дисплея

// Код в модулях rus1_*.* создан под 2-строчный дисплей (концепция при отображении переменной через lcd-дисплей: 
// первая строка - заголовок переменной, вторая строка - её значение). 
// Из настроек lcd-экрана, свободно можно изменять максимальное количество столбцов дисплея.

#define	lcdRowCount	2	// Максимальное количество строк индикатора
#define	lcdColCount	16	// Максимальное количество символов в строках индикатора




// параметры задержек при работе с lcd-дисплеем
#define	lcdCmdWriteDataToRamDelay 40  // задержка при выполнении Write Data to RAM для команды, мкс
#define	lcdDataWriteDelay         40  // задержка при выполнении Write Data to RAM для символов, мкс

// f_lcdText
#define GetLcdText f_lcdText
#define pLcdBegin (&f_lcdText[0][0])

// f_lcdTextBuffer
#define GetLcdTextBuffer f_lcdTextBuffer
// символ с координатами х в строке 0
#define GetCharInRow0 f_lcdTextBuffer[0]
// символ с координатами х в строке 1
#define GetCharInRow1 f_lcdTextBuffer[1]
#define pLcdBufBegin (&f_lcdTextBuffer[0][0])
#define pLcdBufEnd (&f_lcdTextBuffer[lcdRowCount-1][lcdColCount-1])



/////////////////////////
// управление LCD-дисплеем
/////////////////////////

// сигнал на линии Е
#define dLcdE_On {portLcdOutE |= portLcdPinE;}
#define dLcdE_Off {portLcdOutE &= ~ portLcdPinE;}

// выводит 4 бита 8-битного символа // используем только первые 4 бита порта (7 6 5 4 3 2 1 0)
#define dLcd_SetData(x) {portLcdOutData = ((portLcdOutData & 0xF0) | (x & 0x0F));}

// сигнал на линии RS
#define dLcdRS(x) {if (0==x) portLcdOutRS &= ~ portLcdPinRS; \
      else portLcdOutRS |= portLcdPinRS;}
#define dLcdRsOff {portLcdOutRS &= ~ portLcdPinRS;}
#define dLcdRsOn  {portLcdOutRS |= portLcdPinRS;}
#define dLcdRsInvert  {portLcdOutRS ^= portLcdPinRS;}



/////////////////////////////////////////
// для дефайнов для работы с типами данных
/////////////////////////////////////////

// f_lcdTextBuffer
#define pLcdTextBuffer_Reset { \
   for (unsigned char* pChar=pLcdBufBegin; pChar<(pLcdBufBegin + (lcdColCount*lcdRowCount)); pChar++) \
        *pChar = 0; }


// f_lcdText
#define dLcdText_Reset { \
   for (unsigned char* pChar=pLcdBegin; pChar<(pLcdBegin + (lcdColCount*lcdRowCount)); pChar++) \
        *pChar = 0; }



// переводит число в кодировку дисплея
#define dLCD_FigureToLcdChar( val ) (val + 0x30)

// очищает дисплей
// #define dLCD_Clear { for (unsigned char* pTemp = pLcdBufBegin; pTemp<pLcdBufBegin+(lcdColCount*lcdRowCount); pTemp++) *pTemp = ' '; }

// вставляет число в позицию х строки 1 (нумерация с нуля)
#define dInsertInRow1toX(x, s) {*(pLcdBufBegin + lcdColCount + x) = s; }








////////////////////////////////////////////////////////////////////////////// 
// паузы, нс, для обслуживания TIMING CHARACTERISTICS при работе с lcd-дисплеем
////////////////////////////////////////////////////////////////////////////// 


// пауза tAH = 20 нс
// для 16МГц количество тактов: в 1 мкс 16 тактов, в 1 такте 62,5 нс // меньше невозможно
#define dDelayTAH { __no_operation(); }

// если используем постоянную частоту, то можно рассчитать с точностью по тактам
// если используем переменную частоту, то такты лучше не рассчитывать, т.к. на алгоритм 
//      рассчёта уходит слишком много времени
#ifdef use_Constant_FreqCPU

// пауза tAS = 60 нс
// для 16МГц количество тактов: в 1 мкс 16 тактов, в 1 такте 62,5 нс // меньше невозможно
#if (GetCpuFreqMHz > 16)
  #define dDelayTAS { __delay_cycles(2); }
#else
  #define dDelayTAS { __no_operation(); }
#endif



// пауза PWEH = 450 нс
// 
// для 16МГц количество тактов: в 1 мкс 16 тактов, в 1 такте 62,5 нс // меньше невозможно
// для 16МГц 450/62.5 = 7.2 такта => пауза 8 тактов
// для 1Мгц в 1 такте 1 мкс = 1000 нс 450/1000 = 0.45 такта => пауза 1 такт
///////////////////////////////////////////////////////////////////////////////////////

// для 16МГц 7.20 тактов
// для 15МГц 6.75 тактов
// для 14МГц 6.30 тактов
// для 13МГц 5.85 тактов
// для 12МГц 5.40 тактов
// для 11МГц 4.95 тактов
// для 10МГц 4.50 тактов
// для 9МГц 4.05 тактов
// для 8МГц 3.60 тактов
// для 7МГц 3.15 тактов
// для 6МГц 2.70 тактов
// для 5МГц 2.25 тактов
// для 4МГц 1.80 тактов
// для 3МГц 1.35 тактов
// для 2МГц 0.90 тактов
// для 1МГц 0.45 тактов
#ifdef use_WatchDog_for_PUC

#if (GetCpuFreqMHz>15)
#define dDelayPWEN { __delay_cycles(3); \
                     dWatchDogReset; } // 8 тактов
#elif (GetCpuFreqMHz>13)
#define dDelayPWEN { __delay_cycles(2); \
                     dWatchDogReset; } // 7 тактов
#elif (GetCpuFreqMHz>11)
#define dDelayPWEN { __no_operation(); \
                     dWatchDogReset; } // 6 тактов
#elif (GetCpuFreqMHz>8)
#define dDelayPWEN { dWatchDogReset; } // 5 тактов
#elif (GetCpuFreqMHz>6)
#define dDelayPWEN { __delay_cycles(4); } // 4 такта
#elif (GetCpuFreqMHz>4)
#define dDelayPWEN { __delay_cycles(3); } // 3 такта
#elif (GetCpuFreqMHz>2)
#define dDelayPWEN { __delay_cycles(2); } // 2 такта
#else
#define dDelayPWEN { __no_operation(); } // 1 такт
#endif

#else // #ifdef use_WatchDog_for_PUC

#if (GetCpuFreqMHz>15)
#define dDelayPWEN { __delay_cycles(8); } // 8 тактов
#elif (GetCpuFreqMHz>13)
#define dDelayPWEN { __delay_cycles(7); } // 7 тактов
#elif (GetCpuFreqMHz>11)
#define dDelayPWEN { __delay_cycles(6); } // 6 тактов
#elif (GetCpuFreqMHz>8)
#define dDelayPWEN { __delay_cycles(5); } // 5 тактов
#elif (GetCpuFreqMHz>6)
#define dDelayPWEN { __delay_cycles(4); } // 4 такта
#elif (GetCpuFreqMHz>4)
#define dDelayPWEN { __delay_cycles(3); } // 3 такта
#elif (GetCpuFreqMHz>2)
#define dDelayPWEN { __delay_cycles(2); } // 2 такта
#else
#define dDelayPWEN { __no_operation(); } // 1 такт
#endif

#endif // #ifdef use_WatchDog_for_PUC .. #else

// пауза TcycE
// чтобы цикл канала Е дотянуть до 1000нс, это = 520 нс
// для 16МГц количество тактов: в 1 мкс 16 тактов, в 1 такте 62,5 нс // меньше невозможно
// для 16МГц 520/62.5 = 8.32 такта => делаем паузу 9 тактов
// для 1Мгц в 1 такте 1 мкс = 1000 нс 520/1000 = 0.52 такта => пауза 1 такт
///////////////////////////////////////////////////////////////////////////////////////

// для 16МГц 8.32 тактов
// для 15МГц 7.80 тактов
// для 14МГц 7.28 тактов
// для 13МГц 6.76 тактов
// для 12МГц 6.24 тактов
// для 11МГц 5.72 тактов
// для 10МГц 5.20 тактов
// для 9МГц 4.68 тактов
// для 8МГц 4.16 тактов
// для 7МГц 3.64 тактов
// для 6МГц 3.12 тактов
// для 5МГц 2.60 тактов
// для 4МГц 2.08 тактов
// для 3МГц 1.56 тактов
// для 2МГц 1.04 тактов
// для 1МГц 0.52 тактов
#ifdef use_WatchDog_for_PUC

#if (GetCpuFreqMHz>15)
#define dDelayToCycleE { __delay_cycles(4); \
                         dWatchDogReset; }     // 9 тактов
#elif (GetCpuFreqMHz>13)
#define dDelayToCycleE { __delay_cycles(3); \
                         dWatchDogReset; }     // 8 тактов
#elif (GetCpuFreqMHz>11)
#define dDelayToCycleE { __delay_cycles(2); \
                         dWatchDogReset; }     // 7 тактов
#elif (GetCpuFreqMHz>9)
#define dDelayToCycleE { __no_operation(); \
                         dWatchDogReset; }     // 6 тактов
#elif (GetCpuFreqMHz>7)
#define dDelayToCycleE { dWatchDogReset; }      // 5 тактов
#elif (GetCpuFreqMHz>5)
#define dDelayToCycleE { __delay_cycles(4); } // 4 такта
#elif (GetCpuFreqMHz>3)
#define dDelayToCycleE { __delay_cycles(3); } // 3 такта
#elif (GetCpuFreqMHz>1)
#define dDelayToCycleE { __delay_cycles(2); } // 2 такта
#else
#define dDelayToCycleE { __no_operation(); }  // 1 такт
#endif

#else // #ifdef use_WatchDog_for_PUC

#if (GetCpuFreqMHz>15)
#define dDelayToCycleE { __delay_cycles(9); } // 9 тактов
#elif (GetCpuFreqMHz>13)
#define dDelayToCycleE { __delay_cycles(8); } // 8 тактов
#elif (GetCpuFreqMHz>11)
#define dDelayToCycleE { __delay_cycles(7); } // 7 тактов
#elif (GetCpuFreqMHz>9)
#define dDelayToCycleE { __delay_cycles(6); } // 6 тактов
#elif (GetCpuFreqMHz>7)
#define dDelayToCycleE { __delay_cycles(5); } // 5 тактов
#elif (GetCpuFreqMHz>5)
#define dDelayToCycleE { __delay_cycles(4); } // 4 такта
#elif (GetCpuFreqMHz>3)
#define dDelayToCycleE { __delay_cycles(3); } // 3 такта
#elif (GetCpuFreqMHz>1)
#define dDelayToCycleE { __delay_cycles(2); } // 2 такта
#else
#define dDelayToCycleE { __no_operation(); }  // 1 такт
#endif

#endif // #ifdef use_WatchDog_for_PUC .. #else

#else // #iddef use_Constant_FreqCPU

// пауза tAS = 60 нс
// для 16МГц количество тактов: в 1 мкс 16 тактов, в 1 такте 62,5 нс // меньше невозможно
#define dDelayTAS { __delay_cycles(1); }



// пауза PWEH = 450 нс
// 
// для 16МГц количество тактов: в 1 мкс 16 тактов, в 1 такте 62,5 нс // меньше невозможно
// для 16МГц 450/62.5 = 7.2 такта => пауза 8 тактов
#ifdef use_WatchDog_for_PUC
#define dDelayPWEN { __delay_cycles(3); \
                     dWatchDogReset; } // 8 тактов
#else // #ifdef use_WatchDog_for_PUC
#define dDelayPWEN { __delay_cycles(8); } // 8 тактов
#endif // #ifdef use_WatchDog_for_PUC .. #else

// пауза TcycE
// чтобы цикл канала Е дотянуть до 1000нс, это = 520 нс
// для 16МГц количество тактов: в 1 мкс 16 тактов, в 1 такте 62,5 нс // меньше невозможно
// для 16МГц 520/62.5 = 8.32 такта => делаем паузу 9 тактов
#ifdef use_WatchDog_for_PUC
#define dDelayToCycleE { __delay_cycles(4); \
                         dWatchDogReset; }     // 9 тактов
#else // #ifdef use_WatchDog_for_PUC
#define dDelayToCycleE { __delay_cycles(9); } // 9 тактов
#endif

#endif // #ifdef use_Constant_FreqCPU

// пауза перед первым включением дисплея
// по документации: Wait for more than 30ms after Vdd rises to 4.5V
// на практике: для 3.3В 30мс мало, хватает 100мс.
#define dDelayBeforeFirstStart { DelayMilliS(100); }

// выводит символ на LCD-дисплей максимально быстро
// последняя пауза не выполняется, т.к. работа с LCD-дисплеем - только в след. прерывание таймера
#define dLCD_WriteCharQuickly(data) { \
        dLcdRsOn; \
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

// даёт команду Write Data To Ram
// даёт команду на дисплей
#define dLCD_Cmd( x, y ) { dLCD_WriteCommandQuickly(x); DelayMicroS(y); }


// возвращает код числа в таблице символов LCD-контроллера
#define dLCD_NumberToCharHex( val ) (numbersForCharHex[val])

#define LCD_ShowLong( val, xCoord, yCoord ) \
  LongToStr(val, &GetLcdTextBuffer[yCoord][xCoord], lcdColCount-xCoord)


//////// 
// extern
////////     

// выведенный текст на ЖК индикаторе
extern unsigned char f_lcdText[lcdRowCount][lcdColCount];

// буфер вывода текста на ЖК индикатор [когда прибор занят]
// нужен чтобы не переключать LCD панель с кнопками в режим LCD, когда символ уже выведен, а ждать нажатия кнопки
extern unsigned char f_lcdTextBuffer[lcdRowCount][lcdColCount];



// очищает дисплей // для экономии места на флешке
void LCD_Clear();

// выводит команду на LCD-дисплей максимально быстро
// последняя пауза не выполняется, т.к. работа с LCD-дисплеем - только в след. прерывание таймера
void LCD_WriteCommandQuickly(unsigned char data);

    // возвращает код числа в таблице символов LCD-контроллера
    // 0x00 - 0x0F совпадает с ANSI-1251
unsigned char LCD_NumberToCharHex(unsigned char val);
    
// выводит строку в координатах unsigned char xCoord, unsigned char yCoord
// если выходит за ПОСЛЕДНЮЮ строку - обрезается, иначе переносится на другую строку
// упрощённый код, под 2-строчный дисплей
void LCD_ShowString(unsigned char const* dataVal, unsigned char val_len, unsigned char xCoord = 0, unsigned char yCoord = 0);
// void LCD_ShowString_(unsigned char const* dataVal, unsigned char val_len, unsigned char* dataXCoordBegin);
// #define LCD_ShowString0(p,pl,x) LCD_ShowString_(p,pl,(pLcdBufBegin+x))
// #define LCD_ShowString1(p,pl,x) LCD_ShowString_(p,pl,(pLcdBufBegin+x+lcdColCount))
// #define LCD_ShowString(p,pl,x,y) LCD_ShowString_(p,pl,(pLcdBufBegin+x+(y*lcdColCount)))


    // выводит float со знаком в заданные координаты; если выходит за строку - обрезается
    //                   значение    число цифр в дроб. части  координата Х          координата Y          надо ли чистить до конца строки
// void LCD_ShowFloat( float fval,  unsigned char frac_Count, unsigned char xCoord, unsigned char yCoord, bool isClearToEndOfLine = false );

// для уменьшения размера кода
// полностью обновляет экран, выводя строку размера [lcdColCount * lcdRowCount]
// строка обязательно должна быть размера [lcdColCount * lcdRowCount] !
// void LCD_FullUpdateFromString( const unsigned char* dataVal );

#endif // #ifndef __rus1_screenLCD_AC_162A__