
// ������ 
// ������ ��� �������� ����������, ��� ��������

#define example_options

#include "msp430G2553.h"

#include "example_options.hpp"
#include "example_base_functions.hpp"
#include "rus1.hpp" // �������������
#include "rus1_screenLCD_AC-162A.hpp"

// ��������� ������
// ��� ���������� � � �� SaveData, ��� ������ - ������ �������
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
// ��������� ��� ���������� ���������� ��������
unsigned int* pVal220 = &adc220mas[0]; 

// �������� �������� ADC10, ����������� � �����
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
  // ����� �������� ������� ~70�, �������, ����� ���������� ��������� � �� ����� ����� ����, �� ���������� 70�.
  // ����� ����� ��������, ��� ��������� ���������� ���� adc220ustavka, ��������� ���������� ������ ����.
  if (res < adc220ustavka)
    return(0);
  else
    return(dRound(adc10_Calc_FromX(res)));
}
/*
// ��������� ���� � ��� �������� adc220, �� ��������� ������� �������� ��������
void CheckAdc220_MaxMin()
{
  adc220_max = dRound(adc10_Calc_FromY(nastr_DataMax220));
//   adc220_maxUstavka = adc220_max-adc220ustavka;
  adc220_min = dRound(adc10_Calc_FromY(nastr_DataMin220));
//   adc220_minUstavka = adc220_min+adc220ustavka;
}
*/


// ������ � FLASH
// �����, � �������� ���������� ������ ��������� ����������
// ���������� Information memory B, C, D
#define adrFlashInfoCount 3

//////////// 
// flash
//////////// 

#ifdef useBCDbyTurns
// ��� ��������� ������� ����-������ ����� �������� ������ ����������
// ����� ������ ��������:
//  - ����� ������ (������ ����� - �����; ������ 0xFF ������ 0)
//  - ����������
// ������ ������� (��� �����������) ����� Information Memory
// ��� ����������� ������ ���������� �������� (���� ��������� ����������� ��������� �� �����), � ��� ��������� ����� flash
//                                                     Info D  Info C  Info B
const unsigned int adrFlashInfo[adrFlashInfoCount] = {0x1000, 0x1040, 0x1080};
// �������� ������ adrFlashInfo - ����� flash ���� ����� ����������
unsigned char adrFlashInfoIndex = 0;

// ���� ������� � "������" m, ���������� ������ � adrFlashInfo. ���� �� �����, �� ���������� -1.
signed char GetFlashMetka(unsigned int m)
{ 
  for (char ii=0; ii<adrFlashInfoCount; ii++)
    if (m == *((unsigned int *)adrFlashInfo[ii]))
      return(ii);
  return(-1);
}
// � ������ ��� �� ���������� ������
bool IsFirstBoot()
{
  for (unsigned char ii=1; ii<adrFlashInfoCount; ii++)
     if (0xFFFF != *((unsigned int *)adrFlashInfo[ii]))
       return(false);
  return(true);
}
#endif // ifdef useBCDbyTurns

// ��������� ������ � ������; ���� ���� ���� ���������� �����������, ������ ������ error � ��� ���������� ���� ������� Default 
void LoadAllData( void )
{
  if (0xFFFF == *((unsigned int*)adrFlashWrite))
  {
    ResetAllData();
    return;
  }
}

// ��������� ������ �� ������, �������� ��������, ����������� �� ��������� � pSave
void SaveData( signed long* pSave)
{  
  // ���������� � �������� ������ f_lcdText
  unsigned long* pTo = ((unsigned long*)&f_lcdText[0][0]);
  for (signed long* pFlash=(signed long*)(adrFlashWrite + sizeof(unsigned int)); pFlash<(signed long*)(adrFlashWrite + sizeof(unsigned int) + sizeof(defaultData)); pTo++, pFlash++)
    if (pSave == pFlash)
      *pTo = *pFlash + GetDeltaChange;
    else
      *pTo = *pFlash;
  
  // ������� ����-������
  PrepareFlashForWriteData();  
  // �������� �������
  unsigned int* pFlash = (unsigned int*)adrFlashWrite;
  while (FCTL3 & BUSY); *pFlash++ = 0x00;
  for (unsigned int* ii=((unsigned int*)&f_lcdText[0][0]); ii<(unsigned int*)(&f_lcdText[0][0]+sizeof(defaultData)); ii++, pFlash++)
  { while (FCTL3 & BUSY); *pFlash = *ii; }
  // ��������� ������ � ����-�������
  FinishFlashForWriteData();
}

// ���������� ����������� ������ �� �������� �� ���������
void ResetAllData()
{ 
  // ������� ����-������
  PrepareFlashForWriteData();  
  // �������� �������
  unsigned int* pFlash = (unsigned int*)adrFlashWrite;
  while (FCTL3 & BUSY); *pFlash++ = 0x00;
  for (unsigned int* ii=(unsigned int*)addrDefaultOptions; ii<(unsigned int*)(addrDefaultOptions+sizeof(defaultData)); ii++, pFlash++)
  { while (FCTL3 & BUSY); *pFlash = *ii; }
  // ��������� ������ � ����-�������
  FinishFlashForWriteData();
}

// ������� �������������� ����-������ ��� ������
// ������������� WatchDog!
void PrepareFlashForWriteData()
{
#ifdef use_WatchDog_for_PUC
    //  Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD; // ����� ��������, � ����������� �� use_WatchDog_for_PUC
#endif
  
  // ����� ��� ��������� ������
  unsigned int* pFlash = (unsigned int*)adrFlashWrite;
  
  //   ������� ������� ��������� ���������� ����������� flash-������ ������ ���������� � ��������� �� ~257 ��� �� ~476 ���
  // MSP430G2553: fFTG Flash timing generator frequency 257 - 476 kHz
  //  Divide Flash clock by 1 to 64 using FN0 to FN5 according to: 32*FN5 + 16*FN4 + 8*FN3 + 4*FN2 + 2*FN1 + FN0 + 1
  //   ��������� ������� ���������� ���� ��� ���������������� ����� ���� �������� � 1 �� 0, ������
  // ��� ��������� ��� ��������� � 0 �� 1 ��������� ���� ��������.
  // erase
  while (FCTL3 & BUSY);
  //  FWKEY - Flash key for write 
  //  Clear Lock bit
  FCTL3 = FWKEY;                            
  FCTL2 = FWKEY + FSSEL1 + FN5 + FN3;
  //  ������� ������� MERAS=0 ERASE=1 
  //  Set Erase bit
  FCTL1 = FWKEY + ERASE;                      
  //  Dummy write to erase Flash seg
  *(unsigned int *)pFlash = 0;              

  // ���������� � ��������� ������
  while (FCTL3 & BUSY);
  // ���������� LOCK
  FCTL3 = FWKEY;                 
  //  Clk = SMCLK/40
  FCTL2 = FWKEY + FSSEL_1 + FN5 + FN3; 
  // ��������� ������
  FCTL1 = FWKEY+WRT;             
}

// ��������� ������ � �������������� ����-������� (����� ������)
void FinishFlashForWriteData()
{
  // ��� �������� ����� � ���, ����������� ������������ adc220_max � adc220_min �� ������
  unsigned int* pFlash = (unsigned int*)(adrFlashWrite + sizeof(unsigned int) + sizeof(defaultData));
//   adc220_max = dRound(adc10_Calc_FromY(nastr_DataMax220));
  while (FCTL3 & BUSY); *pFlash++ = dRound(adc10_Calc_FromY(nastr_DataMax220));
//   adc220_min = dRound(adc10_Calc_FromY(nastr_DataMin220));
  while (FCTL3 & BUSY); *pFlash = dRound(adc10_Calc_FromY(nastr_DataMin220));

  /* ����� �.�. �� ������� ����� �� ������
  // ��������� ����������
  pFlash--;
  for (unsigned char* ii=(unsigned char*)addrNastr_Free-1; ii>=(unsigned char*)addrFirstSaveFlashData; ii--, pFlash--)
    if ( *pFlash-- != *ii-- )
    {
      RezhimSpecMessage_On(Show_FlashError, 0, BIT0);
#ifdef use_WatchDog_for_PUC
    // ��������
    // �������� - SMCLK
    // ��������� ������ WDT+: ������� ��������� ������� /32768
    WDTCTL = (WDTPW+ WDTCNTCL); // +WDTIS0+WDTIS1
#endif
      return(false);
    }
*/
    // ���������
  while (FCTL3 & BUSY);
  //  Clear WRT bit
  FCTL1 = FWKEY;                            
  //  LOCK - Lock bit: 1 - Flash is locked (read only) 
  //  Set LOCK bit
  FCTL3 = FWKEY + LOCK;                       
#ifdef use_WatchDog_for_PUC
    // ��������
    // �������� - SMCLK
    // ��������� ������ WDT+: ������� ��������� ������� /32768
    WDTCTL = (WDTPW+ WDTCNTCL); // +WDTIS0+WDTIS1
#endif
}

/*
// ���������� ��������� �� ������ ����-������
void Show_FlashError( void )
{
  LCD_ShowString(lines_FlashError, lcdColCount*lcdRowCount,0,0);
}
*/