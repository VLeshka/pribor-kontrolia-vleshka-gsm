// базовые функции, прототипы из языков высокого уровня

#ifndef __example_base_functions__
#define __example_base_functions__

#include "rus1_mainTitle.hpp"

// меняет знак числа // signed!
#define dInvert(x) ( (~(x))+1 )

// возвращает Hex-вид float-числа
#define dFloatToHex(x) ((unsigned long)( *((unsigned long*)&(x))))
                     
// вытаскивает из Hex-записи float-число
#define dHexToFloat(x) ( (float)( *((float*)&(x))) )

// округляет вещественное число до ближайшего целого
#define dRound(x) ( (long)((x)+0.5) )

// возвращает 10^x числом типа ulong
#define dPow10(x) valPow10_long[x]
#ifdef dPow10
  #define pValPow10_long &valPow10_long[0]
  const unsigned long valPow10_long[10] = 
  { 1,           // 0
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000}; // 9 (нумерация с нуля)
  // 4294967295 //   unsigned long: От 0 до 4 294 967 295
#endif

// сброс сторожевого таймера
#ifdef use_WatchDog_for_PUC
  #define dWatchDogReset { WDTCTL = (WDTPW+ WDTCNTCL); } // минимум 5 тактов, + операции с двумя регистрами по подготовке констант 0xA5 и 0x80
#else
  #define dWatchDogReset
#endif



// настоящие функции
/////////////////////////////

// возвращает десятичное число из строки, содержащей 16-ричные числа в символьном виде
// шестнадцатеричные числа должны быть заглавными
unsigned long GetDecimalFromHexString(unsigned char* text, unsigned char textCnt);

// переводит в тексте английские и русские маленькие буквы в большие
void TranslateSmallToLarge(unsigned char* text, unsigned char textCnt);

// сравнивает 2 одинаковые по длине строки, равны ли они по содержимому
bool CompareStrings(const unsigned char* string1, const unsigned char* string2, unsigned char stringsCount);

// возвращает минимальное из двух чисел
unsigned char Min(unsigned char val1, unsigned char val2);

// округляет float до заданного количества знаков после запятой // RoundTo(2.34,1) может вернуть как 2.299999
float RoundTo(float val, unsigned char fracCount);

// возвращает количество цифр в числе, включая знак "минус"
unsigned char GetLengthOfSignedLong(signed long val); // от -2 147 483 648 до 2 147 483 647 = максимум 10 цифр
                    

// преобразует число в строку
void LongToStr( signed long val, unsigned char* pStr, unsigned char strCnt );

// пауза x мкс
//////////////////////////////////////////////// 
// для GetCpuFreqMHz МГц
// делаем (GetCpuFreqMHz-1) цикл ожидания с вложенным циклом, затем 1 цикл с вычетом служебных расходов
// первый цикл: (GetCpuFreqMHz-1) цикл
// вложенный цикл: ожидание для заданных мкс для 1МГц
// подготовка цикла 4 такта
// каждый цикл заниммает 7 тактов, при этом перед первым циклом проверка условия = 2 такта лишних
// 1 цикл с вычетом служебных расходов
// подготовка цикла 4 такта
// каждый цикл заниммает 7 тактов, при этом перед первым циклом проверка условия = 2 такта лишних
// подготовка к вызову пп - 2 такта (параметры вызова пп)
// вызов пп - 5 тактов
// возврат из пп - 2 такта
// подготовка цикла (GetCpuFreqMHz-1) 4 такта
// в (GetCpuFreqMHz-1) перед первым циклом проверка условия = 2 такта лишних
// рекомендуется > 25 мкс
void DelayMicroS(unsigned int x);

// пауза x мс
//////////////////////////////////////////////// 
void DelayMilliS(unsigned int x);

#endif // #ifndef __example_base_functions__