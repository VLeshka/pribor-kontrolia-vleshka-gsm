// �������� ������� ������

#define example_work

#include "rus1_peripherals.hpp"

// ����� ����������� �������:
// 
//  *.cpp // ������������
//    |- *.h // ����������, ���������, ��������� �����������, ��� ������� (����� ������������� � ������-bool)
//    |- * _def.cpp // ������������� � ������-bool
//    |- *_types.h // ��������� �������� ����� ������
// ����������:
//  ��������� � �������, ������� ������������ ������ 1 ��� � ������������, ���������� ����� �������������

#include "msp430G2553.h"

#ifdef testRus1
  #define testRezhim
#endif

#include "example_base_strings.hpp"
#include "example_base_functions.hpp"
#include "example_options.hpp"
#include "peripherals.hpp"
#include "example_options.hpp"
#include "gsm.hpp"  // gsm-�����
#include "rus1.hpp"  // gsm-�����
#include "vl_TreeStructureGenerator_Functions.hpp"// ������ // ������ � LCD-�������� ��� ����������� ���1 (LCD-������� � 4 ��������)

  // ��� ������ ��������� (������������� �������, ���������� ������ � �.�.)
  void OnProgramStart(void)
  {
    //  Stop watchdog timer to prevent time out reset
#ifndef use_WatchDog_for_PUC
    WDTCTL = WDTPW + WDTHOLD; // ����� ��������, � ����������� �� use_WatchDog_for_PUC
#endif

    Syst_InitOnStart(); // ������������� ������� [���������, ��� ������]
    LoadAllData(); // ��������� ������ � ������

    RezhimSpecMessage_On(lineGsmVLeshka, 0, 0); // ������� ��������� � �������� �������
    
    dBeepSignal_Good; // dBeepSignal_LongOk; // dBeepSignal_LongBad; // dBeepSignal_Bad;     
    
    __enable_interrupt(); // ��������� ��������� ����������
  }
