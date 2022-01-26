
// ������ // ������ ��� ��������

#ifndef __example_options__
#define __example_options__


// ������ � FLASH

// �����, � �������� ���������� ������ ��������� ����������
// ���������� Information memory B, C, D
#define adrFlashInfoCount 3

#include "rus1_mainTitle.hpp"

#define use_Constant_FreqCPU


// ��������� ������ �� ���� ������
// �������� ��� �����, ����� �� ��������� ����
// x - �������� adc10
// y - ��������������� �������� adc10 ���������� �� ��������
// ������� ������������: x2> x1, y2 > y1.
#define adc10_x1 ((double)231)
#define adc10_y1 ((double)120)
#define adc10_x2 ((double)853)
#define adc10_y2 ((double)258)

extern const double adc10_constX;
extern const double adc10_constX2;

extern const double adc10_constY;
extern const double adc10_constY2;
// ���������� ��������: 2^x �� 64
#define adc220count 8 
extern unsigned int adc220mas[adc220count];
// ��������� ��� ���������� ���������� ��������
extern unsigned int* pVal220; 



#define adc10_Calc_FromX(x) (adc10_constX * x - adc10_constX2)

#define adc10_Calc_FromY(y) (adc10_constY * y - adc10_constY2)


// �������, �����
#define val220ustavka 5// 10 
// �������, ADC10
// const int adc220ustavka = (dRound((adc10_constY * val220ustavka)));
#define adc220ustavka 22// 45

// �����������, ������������ � ���������� �������� ����������� ��������
//////////////////////////////////////////////////////////////// 

// ����� ����� ����������� � ���������� ��������, �������
// signed long
#define minDataMenuPauseRestart 3
#define maxDataMenuPauseRestart 86400 // 24 ����
#define nastr_SekRestartDefault 10

// ����������� ���������� ���������� ��������
// signed long
#define minDataMax220 230
#define maxDataMax220 280
#define nastr_DataMax220Default 250

// ���������� ���������� ���������� ��������
// signed long
#define minDataMin220 130
#define maxDataMin220 210
#define nastr_DataMin220Default 180

// �������� �� ��� ��� ������ ���������� �� ���������� �������
// unsigned char
#define nastr_IndexSendSmsOnBad220Default 1// 0

// ��������� ����
// unsigned char
#define nastr_IndexBeepOffDefault 0// 1


// �����, � �������� ������ ��� ������������ �� ���� ����������
#define addrDefaultOptions (0xE000)

// ��������� ������
// ��� ���������� � � �� SaveData, ��� ������ - ������ �������
#define defaultDataCnt 5



// ���� ���������� ������ �� ���������
#ifndef useBCDbyTurns
  #define adrFlashWrite (0x1040) // (0x1080)
#endif

///////////////////////////////////////////////////////////
// ���������� ������ ������������ �� ���� ����������

// ����� ����� ����������� � ���������� ��������, �������
#define addrNastr_SekRestart (adrFlashWrite + sizeof(unsigned int))

// ����������� ���������� ���������� ��������
#define addrNastr_DataMax220 (addrNastr_SekRestart + sizeof(unsigned long))

// ���������� ���������� ���������� ��������
#define addrNastr_DataMin220 (addrNastr_DataMax220 + sizeof(unsigned long))

////////////////////////// 
// ���/��
/////////////////////

// �������� �� ��� ��� ������ ���������� �� ���������� �������
#define addrNastr_IndexSendSmsOnBad220 (addrNastr_DataMin220 + sizeof(unsigned long))

// ��������� ����
#define addrNastr_IndexBeepOff (addrNastr_IndexSendSmsOnBad220 + sizeof(unsigned long))

// ��������� ���� � ��� �������� adc220, �� ��������� ������� �������� ��������
// ����� ������ ��� �� ��������� �������� �� ��������� ���������� �� �������� ADC
// ����������� ���������� �������� val220, ���� ������� �������� ��������, �����
#define addrAdc220_max (addrNastr_IndexBeepOff + sizeof(unsigned long))
// ���������� ���������� �������� val220, ���� ������� �������� ��������, �����
#define addrAdc220_min (addrAdc220_max + sizeof(unsigned int))

// ���������� ���������� ���� ����� ������������ �� ���� �����������
// #define sizeAllSaveData (sizeof(defaultData) + 4)// sizeof(t_defaultOptions) // (addrNastr_Free - addrFirstSaveFlashData)

// ���� ����������
/////////////////

// ����� ����� ����������� � ���������� ��������, �������
#define nastr_SekRestart (*(signed long*) addrNastr_SekRestart)

// ����������� ���������� ���������� ��������
#define nastr_DataMax220 (*(signed long*)addrNastr_DataMax220) // (*(@ addrNastr_DataMax220))

// ���������� ���������� ���������� ��������
#define nastr_DataMin220 (*(signed long*) addrNastr_DataMin220)


////////////////////////// 
// ��/���
/////////////////////

// �������� �� ��� ��� ������ ���������� �� ���������� �������
#define nastr_IndexSendSmsOnBad220 (*(signed long*) addrNastr_IndexSendSmsOnBad220)
// ��������� ����
#define nastr_IndexBeepOff (*(signed long*) addrNastr_IndexBeepOff)

// ����������� ���������� �������� val220, ���� ������� �������� ��������, �����
// signed int adc220_max;
#define adc220_max (*(signed int*) addrAdc220_max)
// ���������� ���������� �������� val220, ���� ������� �������� ��������, �����
// signed int adc220_min;
#define adc220_min (*(signed int*) addrAdc220_min)


////////////////////////////////////////////////////////////////////////////////////// 
// ����� ������� ���������� ����� � ���, �� �������� ����� ������������ �� ���� �����������
#define addrNastr_Free 0x0200

// ��������� �������� ADC10
#define dVal220add(x) { *pVal220 = x; \
                       if (pVal220 == &adc220mas[adc220count-1]) pVal220 = &adc220mas[0]; else pVal220++; \
                       }



// �������� �������� ADC10
unsigned int GetVal220();

// ��������� ���� � ��� �������� adc220, �� ��������� ������� �������� ��������
// void CheckAdc220_MaxMin();

// ���� ������� � "������" m, ���������� ������ � adrFlashInfo. ���� �� �����, �� ���������� -1.
signed char GetFlashMetka(unsigned char m);

// ��������� ������ � ������; ���� ���� ���� ���������� �����������, ������ ������ error � ��� ���������� ���� ������� Default 
void LoadAllData( void );

// ��������� ������ �� ������, �������� ��������, ����������� �� ��������� � pSave
void SaveData( signed long* pSave);

// ���������� ����������� ������ �� �������� �� ���������
void ResetAllData();

// ������� �������������� ����-������ ��� ������
// ������������� WatchDog!
void PrepareFlashForWriteData();

// ��������� ������ � �������������� ����-������� (����� ������)
void FinishFlashForWriteData();

// ���������� ��������� �� ������ ����-������
// void Show_FlashError( void );

#endif // #ifndef __example_options__