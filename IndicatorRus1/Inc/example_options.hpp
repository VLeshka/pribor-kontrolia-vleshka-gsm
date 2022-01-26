
// пример // модуль для настроек

#ifndef __example_options__
#define __example_options__


// адреса в FLASH

// адрес, с которого записываем первые выбранные переменные
// используем Information memory B, C, D
#define adrFlashInfoCount 3

#include "rus1_mainTitle.hpp"

#define use_Constant_FreqCPU


// уравнение прямой по двум точкам
// забиваем две точки, далее всё считается само
// x - значение adc10
// y - соответствующее значению adc10 напряжение на нагрузке
// условие корректности: x2> x1, y2 > y1.
#define adc10_x1 ((double)231)
#define adc10_y1 ((double)120)
#define adc10_x2 ((double)853)
#define adc10_y2 ((double)258)

extern const double adc10_constX;
extern const double adc10_constX2;

extern const double adc10_constY;
extern const double adc10_constY2;
// допустимые значения: 2^x до 64
#define adc220count 8 
extern unsigned int adc220mas[adc220count];
// указатель для СЛЕДУЮЩЕГО заносимого значения
extern unsigned int* pVal220; 



#define adc10_Calc_FromX(x) (adc10_constX * x - adc10_constX2)

#define adc10_Calc_FromY(y) (adc10_constY * y - adc10_constY2)


// уставка, Вольт
#define val220ustavka 5// 10 
// уставка, ADC10
// const int adc220ustavka = (dRound((adc10_constY * val220ustavka)));
#define adc220ustavka 22// 45

// минимальные, максимальные и дефоолтные значения сохраняемых настроек
//////////////////////////////////////////////////////////////// 

// пауза между выключением и включением нагрузки, секунды
// signed long
#define minDataMenuPauseRestart 3
#define maxDataMenuPauseRestart 86400 // 24 часа
#define nastr_SekRestartDefault 10

// максимально допустимое напряжение нагрузки
// signed long
#define minDataMax220 230
#define maxDataMax220 280
#define nastr_DataMax220Default 250

// минимально допустимое напряжение нагрузки
// signed long
#define minDataMin220 130
#define maxDataMin220 210
#define nastr_DataMin220Default 180

// Посылать ли смс при выходе напряжения за допустимые пределы
// unsigned char
#define nastr_IndexSendSmsOnBad220Default 1// 0

// Отключить звук
// unsigned char
#define nastr_IndexBeepOffDefault 0// 1


// адрес, с которого держим все записываемые во флеш переменные
#define addrDefaultOptions (0xE000)

// дефолтные данные
// для сохранения в в пп SaveData, все данные - одного размера
#define defaultDataCnt 5



// куда записываем данные по умолчанию
#ifndef useBCDbyTurns
  #define adrFlashWrite (0x1040) // (0x1080)
#endif

///////////////////////////////////////////////////////////
// раздельные адреса записываемых во флеш переменных

// пауза между выключением и включением нагрузки, секунды
#define addrNastr_SekRestart (adrFlashWrite + sizeof(unsigned int))

// максимально допустимое напряжение нагрузки
#define addrNastr_DataMax220 (addrNastr_SekRestart + sizeof(unsigned long))

// минимально допустимое напряжение нагрузки
#define addrNastr_DataMin220 (addrNastr_DataMax220 + sizeof(unsigned long))

////////////////////////// 
// нет/да
/////////////////////

// Посылать ли смс при выходе напряжения за допустимые пределы
#define addrNastr_IndexSendSmsOnBad220 (addrNastr_DataMin220 + sizeof(unsigned long))

// Отключить звук
#define addrNastr_IndexBeepOff (addrNastr_IndexSendSmsOnBad220 + sizeof(unsigned long))

// вычисляет макс и мин значения adc220, за пределами которых вырубаем нагрузку
// чтобы каждый раз не вычислять значение из реального напряжения до значения ADC
// максимально допустимая величина val220, ниже которой вырубаем нагрузку, Вольт
#define addrAdc220_max (addrNastr_IndexBeepOff + sizeof(unsigned long))
// минимально допустимая величина val220, ниже которой вырубаем нагрузку, Вольт
#define addrAdc220_min (addrAdc220_max + sizeof(unsigned int))

// количество занимаемых байт всеми сохраняемыми во флеш переменными
// #define sizeAllSaveData (sizeof(defaultData) + 4)// sizeof(t_defaultOptions) // (addrNastr_Free - addrFirstSaveFlashData)

// сами переменные
/////////////////

// пауза между выключением и включением нагрузки, секунды
#define nastr_SekRestart (*(signed long*) addrNastr_SekRestart)

// максимально допустимое напряжение нагрузки
#define nastr_DataMax220 (*(signed long*)addrNastr_DataMax220) // (*(@ addrNastr_DataMax220))

// минимально допустимое напряжение нагрузки
#define nastr_DataMin220 (*(signed long*) addrNastr_DataMin220)


////////////////////////// 
// да/нет
/////////////////////

// Посылать ли смс при выходе напряжения за допустимые пределы
#define nastr_IndexSendSmsOnBad220 (*(signed long*) addrNastr_IndexSendSmsOnBad220)
// Отключить звук
#define nastr_IndexBeepOff (*(signed long*) addrNastr_IndexBeepOff)

// максимально допустимая величина val220, ниже которой вырубаем нагрузку, Вольт
// signed int adc220_max;
#define adc220_max (*(signed int*) addrAdc220_max)
// минимально допустимая величина val220, ниже которой вырубаем нагрузку, Вольт
// signed int adc220_min;
#define adc220_min (*(signed int*) addrAdc220_min)


////////////////////////////////////////////////////////////////////////////////////// 
// адрес первого свободного байта в ОЗУ, не занятого всеми сохраняемыми во флеш переменными
#define addrNastr_Free 0x0200

// добавляет значение ADC10
#define dVal220add(x) { *pVal220 = x; \
                       if (pVal220 == &adc220mas[adc220count-1]) pVal220 = &adc220mas[0]; else pVal220++; \
                       }



// получает значение ADC10
unsigned int GetVal220();

// вычисляет макс и мин значения adc220, за пределами которых вырубаем нагрузку
// void CheckAdc220_MaxMin();

// ищем сегмент с "меткой" m, возвращает индекс в adrFlashInfo. Если не нашёл, то возвращает -1.
signed char GetFlashMetka(unsigned char m);

// загружаем данные с флешки; если хоть одна переменная некорректна, значит флешка error и все переменные надо ставить Default 
void LoadAllData( void );

// сохраняем данные на флешку, обновляя значение, совпадающее по указателю с pSave
void SaveData( signed long* pSave);

// сбрасываем сохраняемые данные на значения по умолчанию
void ResetAllData();

// готовит информационную флеш-память для записи
// осталавливает WatchDog!
void PrepareFlashForWriteData();

// завершает работу с информационной флеш-памятью (после записи)
void FinishFlashForWriteData();

// показывает сообщение об ошибке флеш-памяти
// void Show_FlashError( void );

#endif // #ifndef __example_options__