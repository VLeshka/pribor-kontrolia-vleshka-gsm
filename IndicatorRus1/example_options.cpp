
// пример 
// модуль для указания переменных, для настроек

#define example_options

#include "msp430G2553.h"

#include "example_options.hpp"
#include "example_base_functions.hpp"
#include "rus1.hpp" // псевдофункции
#include "rus1_screenLCD_AC-162A.hpp"

// дефолтные данные
// для сохранения в в пп SaveData, все данные - одного размера
#pragma location = addrDefaultOptions
__root const unsigned long defaultData[defaultDataCnt] = {
  nastr_SekRestartDefault,
  nastr_DataMax220Default,
  nastr_DataMin220Default,
  nastr_IndexSendSmsOnBad220Default,
  nastr_IndexBeepOffDefault
};

const double adc10_constX = (adc10_y2 - adc10_y1) / (adc10_x2 - adc10_x1);
const double adc10_constX2 = adc10_constX * adc10_x1 - adc10_y1;

const double adc10_constY = (adc10_x2 - adc10_x1) / (adc10_y2 - adc10_y1);
const double adc10_constY2 = adc10_constY * adc10_y1 - adc10_x1;
unsigned int adc220mas[adc220count];
// указатель для СЛЕДУЮЩЕГО заносимого значения
unsigned int* pVal220 = &adc220mas[0]; 

// получает значение ADC10, переведённое в вольт
unsigned int GetVal220() 
{ 
  unsigned int res = adc220mas[0];
  for (unsigned int* ii=&adc220mas[1]; ii<(&adc220mas[0]+adc220count); ii++)
    res += *ii;
#if (adc220count == 64)
  res = (res >> 6);
#elif (adc220count == 32)
  res = (res >> 5);
#elif (adc220count == 16)                        
  res = (res >> 4);
#elif (adc220count == 8)
  res = (res >> 3);
#elif (adc220count == 4)
  res = (res >> 2);
#elif (adc220count == 2)
  res = (res >> 1);
#elif (adc220count == 1)
  res = (res);
#endif
  // Можно измерить минимум ~70В, поэтому, когда напряжение отключают и по факту равно нулю, то показывает 70В.
  // Чтобы этого избежать, при понижении напряжения ниже adc220ustavka, принимаем напряжение равным нулю.
  if (res < adc220ustavka)
    return(0);
  else
    return(dRound(adc10_Calc_FromX(res)));
}
/*
// вычисляет макс и мин значения adc220, за пределами которых вырубаем нагрузку
void CheckAdc220_MaxMin()
{
  adc220_max = dRound(adc10_Calc_FromY(nastr_DataMax220));
//   adc220_maxUstavka = adc220_max-adc220ustavka;
  adc220_min = dRound(adc10_Calc_FromY(nastr_DataMin220));
//   adc220_minUstavka = adc220_min+adc220ustavka;
}
*/


// адреса в FLASH
// адрес, с которого записываем первые выбранные переменные
// используем Information memory B, C, D
#define adrFlashInfoCount 3

//////////// 
// flash
//////////// 

#ifdef useBCDbyTurns
// для повышения ресурса флеш-памяти будем сегменты записи чередовать
// карта адреса сегмента:
//  - номер записи (больше номер - новее; больше 0xFF только 0)
//  - переменные
// массив адресов (для чередования) блока Information Memory
// для возможности чтения предыдущих настроек (если настройки прочитаться корректно не могут), и для продления жизни flash
//                                                     Info D  Info C  Info B
const unsigned int adrFlashInfo[adrFlashInfoCount] = {0x1000, 0x1040, 0x1080};
// активный индекс adrFlashInfo - адрес flash куда будем записывать
unsigned char adrFlashInfoIndex = 0;

// ищем сегмент с "меткой" m, возвращает индекс в adrFlashInfo. Если не нашёл, то возвращает -1.
signed char GetFlashMetka(unsigned int m)
{ 
  for (char ii=0; ii<adrFlashInfoCount; ii++)
    if (m == *((unsigned int *)adrFlashInfo[ii]))
      return(ii);
  return(-1);
}
// в первый раз ли используем флешку
bool IsFirstBoot()
{
  for (unsigned char ii=1; ii<adrFlashInfoCount; ii++)
     if (0xFFFF != *((unsigned int *)adrFlashInfo[ii]))
       return(false);
  return(true);
}
#endif // ifdef useBCDbyTurns

// загружаем данные с флешки; если хоть одна переменная некорректна, значит флешка error и все переменные надо ставить Default 
void LoadAllData( void )
{
  if (0xFFFF == *((unsigned int*)adrFlashWrite))
  {
    ResetAllData();
    return;
  }
}

// сохраняем данные на флешку, обновляя значение, совпадающее по указателю с pSave
void SaveData( signed long* pSave)
{  
  // используем в качестве буфера f_lcdText
  unsigned long* pTo = ((unsigned long*)&f_lcdText[0][0]);
  for (signed long* pFlash=(signed long*)(adrFlashWrite + sizeof(unsigned int)); pFlash<(signed long*)(adrFlashWrite + sizeof(unsigned int) + sizeof(defaultData)); pTo++, pFlash++)
    if (pSave == pFlash)
      *pTo = *pFlash + GetDeltaChange;
    else
      *pTo = *pFlash;
  
  // готовим флеш-память
  PrepareFlashForWriteData();  
  // забиваем данными
  unsigned int* pFlash = (unsigned int*)adrFlashWrite;
  while (FCTL3 & BUSY); *pFlash++ = 0x00;
  for (unsigned int* ii=((unsigned int*)&f_lcdText[0][0]); ii<(unsigned int*)(&f_lcdText[0][0]+sizeof(defaultData)); ii++, pFlash++)
  { while (FCTL3 & BUSY); *pFlash = *ii; }
  // завершаем работу с флеш-памятью
  FinishFlashForWriteData();
}

// сбрасываем сохраняемые данные на значения по умолчанию
void ResetAllData()
{ 
  // готовим флеш-память
  PrepareFlashForWriteData();  
  // забиваем данными
  unsigned int* pFlash = (unsigned int*)adrFlashWrite;
  while (FCTL3 & BUSY); *pFlash++ = 0x00;
  for (unsigned int* ii=(unsigned int*)addrDefaultOptions; ii<(unsigned int*)(addrDefaultOptions+sizeof(defaultData)); ii++, pFlash++)
  { while (FCTL3 & BUSY); *pFlash = *ii; }
  // завершаем работу с флеш-памятью
  FinishFlashForWriteData();
}

// готовит информационную флеш-память для записи
// осталавливает WatchDog!
void PrepareFlashForWriteData()
{
#ifdef use_WatchDog_for_PUC
    //  Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD; // потом включаем, в зависимости от use_WatchDog_for_PUC
#endif
  
  // адрес для фиктивной записи
  unsigned int* pFlash = (unsigned int*)adrFlashWrite;
  
  //   Рабочая частота тактового генератора контроллера flash-памяти должна находиться в диапазоне от ~257 кГц до ~476 кГц
  // MSP430G2553: fFTG Flash timing generator frequency 257 - 476 kHz
  //  Divide Flash clock by 1 to 64 using FN0 to FN5 according to: 32*FN5 + 16*FN4 + 8*FN3 + 4*FN2 + 2*FN1 + FN0 + 1
  //   Состояние каждого отдельного бита при программировании может быть изменено с 1 на 0, однако
  // для изменения его состояния с 0 на 1 требуется цикл стирания.
  // erase
  while (FCTL3 & BUSY);
  //  FWKEY - Flash key for write 
  //  Clear Lock bit
  FCTL3 = FWKEY;                            
  FCTL2 = FWKEY + FSSEL1 + FN5 + FN3;
  //  очищаем сегмент MERAS=0 ERASE=1 
  //  Set Erase bit
  FCTL1 = FWKEY + ERASE;                      
  //  Dummy write to erase Flash seg
  *(unsigned int *)pFlash = 0;              

  // подготовка к забиванию данных
  while (FCTL3 & BUSY);
  // Сбрасываем LOCK
  FCTL3 = FWKEY;                 
  //  Clk = SMCLK/40
  FCTL2 = FWKEY + FSSEL_1 + FN5 + FN3; 
  // Разрешаем запись
  FCTL1 = FWKEY+WRT;             
}

// завершает работу с информационной флеш-памятью (после записи)
void FinishFlashForWriteData()
{
  // для экономии места в ОЗУ, располагаем неизменяемые adc220_max и adc220_min на флешке
  unsigned int* pFlash = (unsigned int*)(adrFlashWrite + sizeof(unsigned int) + sizeof(defaultData));
//   adc220_max = dRound(adc10_Calc_FromY(nastr_DataMax220));
  while (FCTL3 & BUSY); *pFlash++ = dRound(adc10_Calc_FromY(nastr_DataMax220));
//   adc220_min = dRound(adc10_Calc_FromY(nastr_DataMin220));
  while (FCTL3 & BUSY); *pFlash = dRound(adc10_Calc_FromY(nastr_DataMin220));

  /* убрал т.к. не хватает места на флешке
  // проверяем записанное
  pFlash--;
  for (unsigned char* ii=(unsigned char*)addrNastr_Free-1; ii>=(unsigned char*)addrFirstSaveFlashData; ii--, pFlash--)
    if ( *pFlash-- != *ii-- )
    {
      RezhimSpecMessage_On(Show_FlashError, 0, BIT0);
#ifdef use_WatchDog_for_PUC
    // включить
    // источник - SMCLK
    // интервала модуля WDT+: Частота тактового сигнала /32768
    WDTCTL = (WDTPW+ WDTCNTCL); // +WDTIS0+WDTIS1
#endif
      return(false);
    }
*/
    // завершаем
  while (FCTL3 & BUSY);
  //  Clear WRT bit
  FCTL1 = FWKEY;                            
  //  LOCK - Lock bit: 1 - Flash is locked (read only) 
  //  Set LOCK bit
  FCTL3 = FWKEY + LOCK;                       
#ifdef use_WatchDog_for_PUC
    // включить
    // источник - SMCLK
    // интервала модуля WDT+: Частота тактового сигнала /32768
    WDTCTL = (WDTPW+ WDTCNTCL); // +WDTIS0+WDTIS1
#endif
}

/*
// показывает сообщение об ошибке флеш-памяти
void Show_FlashError( void )
{
  LCD_ShowString(lines_FlashError, lcdColCount*lcdRowCount,0,0);
}
*/