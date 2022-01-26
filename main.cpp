
// � �� msp430G2553:
// - ��������� ������� ���������� ADC10 ������, ��� � UART.
// - ��������� ������� ���������� UART ������, ��� � �������� Timer0 � Timer1.

#include "msp430G2553.h"

#include "rus1_mainTitle.hpp"          // ����������, �������� ������ ������ �������

#include "example_work.hpp"            // ������ //�������� �������������������� ������� ������
#include "example_base_functions.hpp"

int main( void )
{   
  OnProgramStart();
  while (true)
    dWatchDogReset;
}