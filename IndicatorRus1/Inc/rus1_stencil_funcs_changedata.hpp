
// .h-���� ������ �������-"����������" ��������� ������

#ifndef __rus1_stencil_funcs_changedata__
#define __rus1_stencil_funcs_changedata__

#include "rus1_mainTitle.hpp"

// ��� void Data_ChangeWithCursor(); 
struct t_dataForChangeWithCursor
{
  // �������� ���������, ������� ��������
  signed long* data; 
  // ���������� ���������� �������� ���������
  const unsigned long  dataMin; 
  // ����������� ���������� �������� ���������
  const unsigned long  dataMax; 
  // ������ ����� ������� ������ �������� (�� �� ������ ��������)
  const unsigned char* strBefore; 
  // ������ ����� ������ ������ ��������
  const unsigned char* strAfter; 
  // ������ ���������
  const unsigned char* strZagolovok; 
  // ������ 
  // ������ ����� ������� ������ �������� (�� �� ������ ��������)
  const unsigned char strBeforeCnt; 
  // ������ 
  // ������ ����� ������ ������ ��������
  const unsigned char strAfterCnt; 
  // ������ 
  // ������ ���������
  const unsigned char strZagolovokCnt; 
};

// ��� void Data_ChangeIndex_ArrayStr();
struct t_dataForChangeIndex_ArrayStr
{
  // �������� ���������, ������� �������� [������]
  signed long* dataIndex; 
  // ������ ���������� �� ������ ������� lcdColCount
  const unsigned char** dataMas; 
  const unsigned char* strZagolovok; 
  // ����������� 
  // ������ ���������� ��������
  const unsigned char dataMasCount; 
  // ������ ���������
  const unsigned char strZagolovokCnt; 
};



//////////////////////////////////// 
// ��� ������������� ���������� ���-1 
// ��������� ������
/////////////////////

// ��������� �� ������������� ������������ �������� (1, 10, 100, ...), � ����������� ����� ������ �� ������.
// ������� �������� ����������� ���������� ��������� �������. ��������� ������� ���������� ������������� ������� 
// ������ "�����" � "����".
// � ���������� ������� ���������� ������� ����� GetLnChange, ����� �� ������������ ������� ������ �� �������.
void Data_ChangeWithCursor(); 

// ��������� �� ������� ���������� �������� (������), � ����������� ����� ������ �� ������.
// ��������� ��� �� ��������� ���������, � ����������� �� ������ ������������.
void Data_ChangeIndex_ArrayStr();
/*
// ��������� ������, ��������� "������� ������������� GetLnChange->GetDeltaChange, � ����������� ����� ������ �� ������.
// ��������� ��� �� ��������� ���������, � ����������� �� ������ ������������.
void Data_ChangeFixGetLnChange(signed long data, // �������� ���������, ������� ��������
                               unsigned int lnChange, // ����� �����, � ��������, ��������, ��������� � ����
                               const unsigned long  dataMin, // ���������� ���������� �������� ���������
                               const unsigned long  dataMax, // ����������� ���������� �������� ���������
                               const unsigned char* strBefore, const unsigned char strBeforeCnt, // ������ ����� ������� ������ �������� (�� �� ������ ��������)
                               const unsigned char* strAfter,  const unsigned char strAfterCnt, // ������ ����� ������ ������ ��������
                               const unsigned char* strZagolovok, const unsigned char strZagolovokCnt); // ������ ���������
*/
/*
// ��������� �� ������� ���������� �������� (�����), � ����������� ����� ������ �� ������.
// ��������� ��� �� ��������� ���������, � ����������� �� ������ ������������.
// GetDeltaChange - ������� ����� ������ �������� ������� � �����
void Data_ChangeIndex_ArrayLong(unsigned char dataIndex, // �������� ���������, ������� �������� [������]
                               const unsigned long* dataMas, // ������ ���������� ��������
                               const unsigned int   dataMasCount, // ����������� // ������ ���������� ��������
                               const unsigned char* strBefore, const unsigned char strBeforeCnt, // ������ ����� ������� ������ �������� (�� �� ������ ��������)
                               const unsigned char* strAfter,  const unsigned char strAfterCnt, // ������ ����� ������ ������ ��������
                               const unsigned char* strZagolovok, const unsigned char strZagolovokCnt); // ������ ���������
*/

#endif // #ifndef __rus1_stencil_funcs_changedata__