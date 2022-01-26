
// ������ �������-"����������" ��������� ������

#define rus1_stencil_funcs_changedata

#include "msp430G2553.h"
#include "rus1.hpp"   // �������������
#include "rus1_screenLCD_AC-162A.hpp"
#include "example_base_functions.hpp"
#include "rus1_stencil_funcs_changedata.hpp"
#include "example_base_strings.hpp"
#include "example_options.hpp"

/////////////////////////
// ��������� ������
// ���������� ���, ��� 2-�������� �������
/////////////////////

// ��������� �� ������������� ������������ �������� (1, 10, 100, ...), � ����������� ����� ������ �� ������.
// ������� �������� ����������� ���������� ��������� �������. ��������� ������� ���������� ������������� ������� 
// ������ "�����" � "����".
// � ���������� ������� ���������� ������� ����� GetLnChange, ����� �� ������������ ������� ������ �� �������.
void Data_ChangeWithCursor() // ������ ���������
{
  // if (NULL != currentTreeNode->pDataForChange)
  if (isNeedSaveData) // ���� ���� ��������� ��������� ������
  {
    t_dataForChangeWithCursor* pData = (t_dataForChangeWithCursor*)currentTreeNode->pDataForChange;
    // ��������� ������
    SaveData(pData->data);
    // ���������� "����" ���������� ���������� ��������� ������
    dSetNeedSaveData_Off;
  }
  else // if (isNeedSaveData)
  {
    t_dataForChangeWithCursor* pData = (t_dataForChangeWithCursor*)currentTreeNode->pDataForChange;
    switch (GetRRP)
    {
      case rrp_0: //  ��� ������������ ������ (������� ������)
        // ���������
        LCD_ShowGoString(pData->strZagolovok, pData->strZagolovokCnt,0);
        LCD_ClearCharsToEndFromRow0(pData->strZagolovokCnt);
        // � ������ ��� ������ ������� ������ ����� ���������
        LCD_ShowString(pData->strBefore, pData->strBeforeCnt, 0, 1);
        // ��������� � ������ ��������������
        dResetLnChange;
        dResetDeltaChange;
        break;
      case rrp_1: //  �������������� ���������� (� ���� �� ��������� ���������)
        // ���������
        LCD_ShowString(lineLCD_Enter, lcdColCount, 0, 0);
        // ��������� � ������ ������ �� �������������� ���������� � �������������� ��������� ������� ������
        // ��� ���� �������� ������ ��������� �� ������, ���������� ������������� �������� "�����/����"        
        dSetIsCursorVisiblePosition_Need_ON;
        dLCD_SetCursorBlinkOn;        
        break;
      case rrp_2: //  �������������� ���������� (��������� "�����")
        // ��������� ����� �������� (����� ������ �������� - ����� switch)
        if ( ( ((signed long)(*pData->data) + (signed long)GetDeltaChange) + (signed long)dPow10(GetLnChange)) > (signed long)pData->dataMax )
          GetDeltaChange = (signed long)pData->dataMax - (signed long)(*pData->data);
        else
          GetDeltaChange += dPow10(GetLnChange);
        break;
      case rrp_3: //  �������������� ���������� (��������� "����")
        // ��������� ����� �������� (����� ������ �������� - ����� switch)
        if ( ( ((signed long)(*pData->data) + (signed long)GetDeltaChange) - (signed long)dPow10(GetLnChange)) < (signed long)pData->dataMin )
          GetDeltaChange = (signed long)pData->dataMin - (signed long)(*pData->data);
        else
          GetDeltaChange -= dPow10(GetLnChange);
        break;
      // case rrp_4: //  ����� ������ ��������� �� ������� ����������� �������� // ����� ���������!
        // �������� ������ ������ ������� � ����������� ������� Enter/Escape
      // case rrp_5: //  ����� ��������� �� ��������� ��������        
      //   return;
    default:
      return;
    }
    signed long new_data = (*((signed long*)(pData->data))) + GetDeltaChange;
    // ��������� ��������� ��������� ����� ������ ��� ������������� ��������� ������� ������
    unsigned char maxLn = GetLengthOfSignedLong(new_data)-1; // ln ������ � ����
    dSetLnChangeMax(maxLn);
    // (�������_����������_�������_�) = (������) + (�����_������)
    unsigned int baseCoordX = pData->strBeforeCnt + maxLn;
    dSetBaseXCursor(baseCoordX);    
    // ������� ����� �������� �� �����
    LCD_ShowLong(new_data, pData->strBeforeCnt,1);
    // � ������ ����� ����� ��������
    LCD_ShowString(pData->strAfter, pData->strAfterCnt, ++baseCoordX, 1);
    LCD_ClearCharsToEndFromRow1(baseCoordX + pData->strAfterCnt);
    // ������ "����"
    if (rrp_0 == GetRRP)
      GetCharInRow1[lcdColCount-1] = cursorEnter;
    
#ifdef use_WatchDog_for_PUC
    // ����� ������� ����������� ������ ���������� �� �����, ���������� ���������� ������
	dWatchDogReset; 
#endif
  } // if (isNeedSaveData) .. else
}

// ��������� �� ������� ���������� �������� (������), � ����������� ����� ������ �� ������.
// ��������� ��� �� ��������� ���������, � ����������� �� ������ ������������.
void Data_ChangeIndex_ArrayStr() // ������ ���������
{
  // if (NULL != currentTreeNode->pDataForChange)
  // ���� ���� ��������� ��������� ������
  if (isNeedSaveData) 
  {
    t_dataForChangeIndex_ArrayStr* pData = (t_dataForChangeIndex_ArrayStr*)currentTreeNode->pDataForChange;
    // ��������� ������
    SaveData(pData->dataIndex);
    // ���������� "����" ���������� ���������� ��������� ������
    dSetNeedSaveData_Off;
  }
  else // if (isNeedSaveData)
  {
    t_dataForChangeIndex_ArrayStr* pData = (t_dataForChangeIndex_ArrayStr*)currentTreeNode->pDataForChange;
    switch (GetRRP)
    {
      //  ��� ������������ ������ (������� ������)
	  case rrp_0: 
        // ���������
        LCD_ShowGoString(pData->strZagolovok, pData->strZagolovokCnt,0);
        LCD_ClearCharsToEndFromRow0(pData->strZagolovokCnt);
        // ��������� � ������ ��������������
        dResetDeltaChange;
        break;
      
	  //  �������������� ���������� (� ���� �� ��������� ���������)
	  case rrp_1: 
        // ���������
        LCD_ShowString(lineLCD_Check, lcdColCount, 0, 0);
        break;
      
	  //  �������������� ���������� (��������� "�����")
	  case rrp_2: 
        // ��������� ����� �������� (����� ������ �������� - ����� switch)
        if ( ( (*((signed long*)(pData->dataIndex))) + (signed long)GetDeltaChange) < (signed long)(pData->dataMasCount-1) )
          GetDeltaChange++;
        break;
      case rrp_3: //  �������������� ���������� (��������� "����")
        // ��������� ����� �������� (����� ������ �������� - ����� switch)
        if ( ( (*((signed long*)(pData->dataIndex))) + (signed long)GetDeltaChange) > (signed long)0 )
          GetDeltaChange--;
        break;
		
      // case rrp_4: //  ����� ������ ��������� �� ������� ����������� �������� // ����� ���������!
      // case rrp_5: //  ����� ��������� �� ��������� ��������        
      //   return;
	  
      default:
        return;
    }
	
    // ������� ����� �������� �� �����
    LCD_ShowString(pData->dataMas[*pData->dataIndex + GetDeltaChange], lcdColCount, 0, 1);
    // ������ "����"
    if (rrp_0 == GetRRP)
      GetCharInRow1[lcdColCount-1] = cursorEnter;

#ifdef use_WatchDog_for_PUC
    // ����� ������� ����������� ������ ���������� �� �����, ���������� ���������� ������
	dWatchDogReset; 
#endif
  } // if (isNeedSaveData) .. else
}
/*
// ��������� ������, ��������� "������� ������������� GetLnChange->GetDeltaChange, � ����������� ����� ������ �� ������.
// ��������� ��� �� ��������� ���������, � ����������� �� ������ ������������.
void Data_ChangeFixGetLnChange(signed long data, // �������� ���������, ������� ��������
                               unsigned int lnChange, // ����� �����, � ��������, ��������, ��������� � ����
                               const unsigned long  dataMin, // ���������� ���������� �������� ���������
                               const unsigned long  dataMax, // ����������� ���������� �������� ���������
                               const unsigned char* strBefore, const unsigned char strBeforeCnt, // ������ ����� ������� ������ �������� (�� �� ������ ��������)
                               const unsigned char* strAfter,  const unsigned char strAfterCnt, // ������ ����� ������ ������ ��������
                               const unsigned char* strZagolovok, const unsigned char strZagolovokCnt) // ������ ���������
{    
    switch (GetRRP)
    {
      case rrp_0: //  ��� ������������ ������ (������� ������)
        // ���������
        LCD_ShowGoString(strZagolovok, strZagolovokCnt,0);
        LCD_ClearCharsToEndFromRow0(strZagolovokCnt);
        // � ������ ��� ������ ������� ������ ����� ���������
        LCD_ShowString(strBefore, strBeforeCnt, 0, 1);
        // ��������� � ������ ��������������
        dResetDeltaChange;
        break;
      case rrp_1: //  �������������� ���������� (� ���� �� ��������� ���������)
        // ���������
        LCD_ShowString(lineLCD_Enter, lcdColCount,0,0);
        break;
      case rrp_2: //  �������������� ���������� (��������� "�����")
        // ��������� ����� �������� (����� ������ �������� - ����� switch)
        if ( ( ((signed long)data + (signed long)GetDeltaChange) + (signed long)dPow10(lnChange)) > (signed long)dataMax )
          GetDeltaChange = (signed long)dataMax - (signed long)data;
        else
          GetDeltaChange += dPow10(lnChange);
        break;
      case rrp_3: //  �������������� ���������� (��������� "����")
        // ��������� ����� �������� (����� ������ �������� - ����� switch)
        if ( ( ((signed long)data + (signed long)GetDeltaChange) - (signed long)dPow10(lnChange)) < (signed long)dataMin )
          GetDeltaChange = (signed long)dataMin - (signed long)data;
        else
          GetDeltaChange -= dPow10(lnChange);
        break;
      // case rrp_4: //  ����� ������ ��������� �� ������� ����������� �������� // ����� ���������!
      // case rrp_5: //  ����� ��������� �� ��������� ��������        
      //   return;
    default:
      return;
    }
    signed long new_data = data + GetDeltaChange;
    // ��������� ��������� ��������� ����� ������ ��� ������������� ��������� ������� ������
    unsigned char maxLn = GetLengthOfSignedLong(new_data)-1; // ln ������ � ����
    dSetLnChangeMax(maxLn);
    // (�������_����������_�������_�) = (������) + (�����_������)
    unsigned int baseCoordX = strBeforeCnt + maxLn;
    // ������� ����� �������� �� �����
    LCD_ShowLong(new_data, strBeforeCnt,1);
    // � ������ ����� ����� ��������
    LCD_ShowString(strAfter, strAfterCnt, ++baseCoordX, 1);
    LCD_ClearCharsToEndFromRow1(baseCoordX + strAfterCnt);
    // ������ "����"
    if (rrp_0 == GetRRP)
      GetCharInRow1[lcdColCount-1] = cursorEnter;
    
#ifdef use_WatchDog_for_PUC
    dWatchDogReset; // ����� ������� ����������� ������ ���������� �� �����, ���������� ���������� ������
#endif
}

// ��������� �� ������� ���������� �������� (�����), � ����������� ����� ������ �� ������.
// ��������� ��� �� ��������� ���������, � ����������� �� ������ ������������.
// GetDeltaChange - ������� ����� ������ �������� ������� � �����
void Data_ChangeIndex_ArrayLong(unsigned char dataIndex, // �������� ���������, ������� �������� [������]
                              const unsigned long* dataMas, // ������ ���������� ��������
                              const unsigned int   dataMasCount, // ����������� // ������ ���������� ��������
                              const unsigned char* strBefore, const unsigned char strBeforeCnt, // ������ ����� ������� ������ �������� (�� �� ������ ��������)
                              const unsigned char* strAfter,  const unsigned char strAfterCnt, // ������ ����� ������ ������ ��������
                              const unsigned char* strZagolovok, const unsigned char strZagolovokCnt) // ������ ���������
{    
    switch (GetRRP)
    {
      case rrp_0: //  ��� ������������ ������ (������� ������)
        // ���������
        LCD_ShowGoString(strZagolovok, strZagolovokCnt,0);
        LCD_ClearCharsToEndFromRow0(strZagolovokCnt);
        // � ������ ��� ������ ������� ������ ����� ���������
        LCD_ShowString(strBefore, strBeforeCnt, 0, 1);  
        // ��������� � ������ ��������������
        dResetDeltaChange;
        break;
      case rrp_1: //  �������������� ���������� (� ���� �� ��������� ���������)
        // ���������
        LCD_ShowString(lineLCD_Check, lcdColCount,0,0);
        break;
      case rrp_2: //  �������������� ���������� (��������� "�����")
        // ��������� ����� �������� (����� ������ �������� - ����� switch)
        if ( ((signed int)dataIndex + (signed int)GetDeltaChange) < (signed int)(dataMasCount-1) )
          GetDeltaChange++;
        break;
      case rrp_3: //  �������������� ���������� (��������� "����")
        // ��������� ����� �������� (����� ������ �������� - ����� switch)
        if ( ((signed int)dataIndex + (signed int)GetDeltaChange) > (signed int)0 )
          GetDeltaChange--;
        break;
      // case rrp_4: //  ����� ������ ��������� �� ������� ����������� �������� // ����� ���������!
      // case rrp_5: //  ����� ��������� �� ��������� ��������        
      //   return;
    default:
      return;
    }
    signed long new_data = *(dataMas + (dataIndex + GetDeltaChange));
    // ��������� ��������� ��������� ����� ������
    // (�������_����������_�������_�) = (������) + (�����_������)
    unsigned int baseCoordX = strBeforeCnt-1 + GetLengthOfSignedLong(new_data);
    // ������� ����� �������� �� �����
    LCD_ShowLong(new_data, strBeforeCnt,1);
    // � ������ ����� ����� ��������
    LCD_ShowString(strAfter, strAfterCnt, ++baseCoordX, 1);
    LCD_ClearCharsToEndFromRow1(baseCoordX + strAfterCnt);
    // ������ "����"
    if (rrp_0 == GetRRP)
      GetCharInRow1[lcdColCount-1] = cursorEnter;
    
#ifdef use_WatchDog_for_PUC
    dWatchDogReset; // ����� ������� ����������� ������ ���������� �� �����, ���������� ���������� ������
#endif
}
*/