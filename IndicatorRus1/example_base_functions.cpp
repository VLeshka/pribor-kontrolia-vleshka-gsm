// ������� �������, ��������� �� ������ �������� ������
// ��� ������������� �� ����������� �������
                 
// ����� ������������, ��� ������ � LCD-������� ������������ ������
// ����������� ������ �� IAR

#define example_base_functions

#include "msp430G2553.h"

#include "rus1_mainTitle.hpp"

#include "example_base_functions.hpp"
#include "rus1_screenLCD_AC-162A.hpp"
#include "rus1_indicatorRUS1.hpp"
#include "rus1_peripherals.hpp"


// ���������� ���������� ����� �� ������, ���������� 16-������ ����� � ���������� ����
// ����������������� ����� ������ ���� ����������
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

// ��������� � ������ ���������� � ������� ��������� ����� � �������
void TranslateSmallToLarge(unsigned char* text, unsigned char textCnt)
{
  unsigned char* textEnd = text + textCnt;
  for (; text<textEnd; text++)
    if (*text > 0x60)
    {
      if (*text < 0x7B)
        *text = *text - 0x20;
      else
        // ������� ����� � � �� ��������� - �� ����� �� ��� 
	    // � ������������ ���-��������
		// + ����� ��� �� ������� �� ����������
        if (*text >= 0xE0)
          // if (*text <= 0xFF) // the result is allways true
            *text = *text - 0x20;
    }
}

// ���������� 2 ���������� �� ����� ������, ����� �� ��� �� �����������
bool CompareStrings(const unsigned char* string1, const unsigned char* string2, unsigned char stringsCount)
{
  const unsigned char* pEnd = string1 + stringsCount - 1;
  for (; string1 <= pEnd; string1++, string2++)
    if (*string1 != *string2)
      return(false);
  return(true);
}

// ���������� ���������� ���� � �����, ������� ���� "�����"
unsigned char GetLengthOfSignedLong(signed long val) // �� -2 147 483 648 �� 2 147 483 647 = �������� 11 ������
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

// ����������� ����� � ������
void LongToStr( signed long val, unsigned char* pStr, unsigned char strCnt )
{
  unsigned char *posAllowEnd = pStr + strCnt; // � ������ ��������� ��� ������ ��������
  if (0 > val)
  {
    *pStr = '-';
    pStr++;
  }
  pStr = pStr + GetLengthOfSignedLong(val) - 1; // � ������ ��������� �������� ��������
  // � ������� �����, �������
  while (val > 9)
  {		
    // ������� long �� 10 ����� ������� ��������
    signed long tmp = ( val >> 1 );
    tmp += (tmp >> 1);
    tmp += (tmp >> 4);
    tmp += (tmp >> 8);
    tmp += (tmp >> 16);
    tmp >>= 3;
    // ��������� �������
    // ���������� *10 ������ 5� ������������ ����������
	unsigned char cifra = val - (tmp * 10); 
    // �������� �� ��������� 10 
	//  � ���� ������ ������� = 10, ��������� ������� �� 1
    if (cifra>9)
    {
      cifra -= 10; 
      tmp++;
    }
    val = tmp;
    // ������� � ���������
    if (pStr < posAllowEnd)
      *pStr = dLCD_FigureToLcdChar(cifra);
    pStr--;
  } // while (val > 9)
  if (pStr < posAllowEnd)
    *pStr = dLCD_FigureToLcdChar((unsigned char)val);
}

// ����� x ��� � ��������� �� ����� ���
//////////////////////////////////////////////// 
// ��� GetCpuFreqMHz ���
// ������ (GetCpuFreqMHz-1) ���� �������� � ��������� ������, ����� 1 ���� � ������� ��������� ��������
// ������ ����: (GetCpuFreqMHz-1) ����
// ��������� ����: �������� ��� �������� ��� ��� 1���
// ���������� ����� 4 �����
// ������ ���� ��������� 7 ������, ��� ���� ����� ������ ������ �������� ������� = 4 ����� ������
// 1 ���� � ������� ��������� ��������
// ���������� ����� 4 �����
// ������ ���� ��������� 7 ������, ��� ���� ����� ������ ������ �������� ������� = 4 ����� ������
// ���������� � ������ �� - 2 ����� (��������� ������ ��)
// ����� �� - 5 ������
// ������� �� �� - 2 �����
// ���������� ����� (GetCpuFreqMHz-1) 4 �����
// � (GetCpuFreqMHz-1) ����� ������ ������ �������� ������� = 4 ����� ������
/////////////////
// ������������� > 25 ���
void DelayMicroS(unsigned int x)
{ 
  // (GetCpuFreqMHz-1) ���� ��������
  for (unsigned int g=1; g<(unsigned int)GetCpuFreqMHz; g++)
    for (unsigned int ii=4+4; ii<x; ii+=7); 
  // 1 ���� � ������� ��������� ��������
#ifdef use_WatchDog_for_PUC
  dWatchDogReset;
  for (unsigned int ii=2+5 + 4+4 + 4+4 +2 +5; ii<x; ii+=7);
#else
  for (unsigned int ii=2+5 + 4+4 + 4+4 +2; ii<x; ii+=7);
#endif
}

// ����� x ��
// �� ��������� DelayMicroS
//////////////////////////////////////////////// 
// void DelayMilliS(unsigned int x) { for (unsigned int ii=1; ii<x; ii++) DelayMicroS(1000); }

// ����� x �� � ��������� �� ����� ���
// �� ��������� DelayMicroS
//////////////////////////////////////////////// 
//  + ��� ������ �� push.w R10 = 3 �����
//  + ��� ������ �� mov.w R12,R13 = 1 ����
//  + ��� ���������� �� pop.w R10 = 2 �����
void DelayMilliS(unsigned int x) 
{ 
  for (unsigned int ii=1; ii<x; ii++)
     DelayMicroS(1000);
  // 1 �� � ������� ��������� ��������
  for (unsigned int g=1; g<(unsigned int)GetCpuFreqMHz; g++)
    for (unsigned int ii=4+4; ii<1000; ii+=6);
#ifdef use_WatchDog_for_PUC
  dWatchDogReset;
  for (unsigned int ii=2+5 + 3+1 + 4+4 + 4+4 + 4+4 + 2 +2 +5; ii<1000; ii+=6);
#else
  for (unsigned int ii=2+5 + 3+1 + 4+4 + 4+4 + 4+4 + 2 +2; ii<1000; ii+=6);
#endif
}
