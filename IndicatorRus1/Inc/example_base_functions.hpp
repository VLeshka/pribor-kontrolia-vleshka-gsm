// ������� �������, ��������� �� ������ �������� ������

#ifndef __example_base_functions__
#define __example_base_functions__

#include "rus1_mainTitle.hpp"

// ������ ���� ����� // signed!
#define dInvert(x) ( (~(x))+1 )

// ���������� Hex-��� float-�����
#define dFloatToHex(x) ((unsigned long)( *((unsigned long*)&(x))))
                     
// ����������� �� Hex-������ float-�����
#define dHexToFloat(x) ( (float)( *((float*)&(x))) )

// ��������� ������������ ����� �� ���������� ������
#define dRound(x) ( (long)((x)+0.5) )

// ���������� 10^x ������ ���� ulong
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
    1000000000}; // 9 (��������� � ����)
  // 4294967295 //   unsigned long: �� 0 �� 4 294 967 295
#endif

// ����� ����������� �������
#ifdef use_WatchDog_for_PUC
  #define dWatchDogReset { WDTCTL = (WDTPW+ WDTCNTCL); } // ������� 5 ������, + �������� � ����� ���������� �� ���������� �������� 0xA5 � 0x80
#else
  #define dWatchDogReset
#endif



// ��������� �������
/////////////////////////////

// ���������� ���������� ����� �� ������, ���������� 16-������ ����� � ���������� ����
// ����������������� ����� ������ ���� ����������
unsigned long GetDecimalFromHexString(unsigned char* text, unsigned char textCnt);

// ��������� � ������ ���������� � ������� ��������� ����� � �������
void TranslateSmallToLarge(unsigned char* text, unsigned char textCnt);

// ���������� 2 ���������� �� ����� ������, ����� �� ��� �� �����������
bool CompareStrings(const unsigned char* string1, const unsigned char* string2, unsigned char stringsCount);

// ���������� ����������� �� ���� �����
unsigned char Min(unsigned char val1, unsigned char val2);

// ��������� float �� ��������� ���������� ������ ����� ������� // RoundTo(2.34,1) ����� ������� ��� 2.299999
float RoundTo(float val, unsigned char fracCount);

// ���������� ���������� ���� � �����, ������� ���� "�����"
unsigned char GetLengthOfSignedLong(signed long val); // �� -2 147 483 648 �� 2 147 483 647 = �������� 10 ����
                    

// ����������� ����� � ������
void LongToStr( signed long val, unsigned char* pStr, unsigned char strCnt );

// ����� x ���
//////////////////////////////////////////////// 
// ��� GetCpuFreqMHz ���
// ������ (GetCpuFreqMHz-1) ���� �������� � ��������� ������, ����� 1 ���� � ������� ��������� ��������
// ������ ����: (GetCpuFreqMHz-1) ����
// ��������� ����: �������� ��� �������� ��� ��� 1���
// ���������� ����� 4 �����
// ������ ���� ��������� 7 ������, ��� ���� ����� ������ ������ �������� ������� = 2 ����� ������
// 1 ���� � ������� ��������� ��������
// ���������� ����� 4 �����
// ������ ���� ��������� 7 ������, ��� ���� ����� ������ ������ �������� ������� = 2 ����� ������
// ���������� � ������ �� - 2 ����� (��������� ������ ��)
// ����� �� - 5 ������
// ������� �� �� - 2 �����
// ���������� ����� (GetCpuFreqMHz-1) 4 �����
// � (GetCpuFreqMHz-1) ����� ������ ������ �������� ������� = 2 ����� ������
// ������������� > 25 ���
void DelayMicroS(unsigned int x);

// ����� x ��
//////////////////////////////////////////////// 
void DelayMilliS(unsigned int x);

#endif // #ifndef __example_base_functions__