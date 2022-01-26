
// ������ ������ � ����������� (LCD-������ + 4 ������) ��� "��� �����"

#ifndef __rus1_indicatorRUS1__
#define __rus1_indicatorRUS1__

#include "rus1_mainTitle.hpp"

//////////////////// 
// ��������� ������
////////////////// 

// ���� ����� ������ ���� "�� ��������", ����� ������ � LCD_WriteByte ���� ��������� 
// ������ "�����"
#define indRus1_ButtonUp BIT3 // 7
// ������ "����"
#define indRus1_ButtonDown BIT2 // 6
// ������ "�����"
#define indRus1_ButtonEnter BIT1 // 5
// ������ "������"
#define indRus1_ButtonEscape BIT0 // 4

// ���� ��� ������������� ������
#define indRus1_InPort P2IN

// ����� ���� ������
#define indRus1_ButtonAll (indRus1_ButtonUp | indRus1_ButtonDown | indRus1_ButtonEnter | indRus1_ButtonEscape)

#ifdef use_ButtonsShift

// Shift-������
#define indRus1_ButtonsShift (indRus1_ButtonEnter+indRus1_ButtonEscape)

// "�� �������": ������������� ����� �� ��������, �� ��������� ������� ���������� ������� ������� ������
// "����� �������", ����-�������: ������������� ����� ����� ������� ������� ������

// ������ � buttonDelayWaitFlag: ������ �� "�� �������" Shift-������
#define buttonBounceDelay_Shift ( indRus1_ButtonsShift )

// ������ � buttonDelayWaitFlag: ������ �� "����� �������" Shift-������
#define buttonPostDelay_Shift ( indRus1_ButtonsShift << 8)

// ������ � buttonDelayWaitFlag: ������ �� "�� �������" ��� "����� �������" ����� Shift-������
#define buttonBounceAndPostDelayMask_Shift ( (indRus1_ButtonsShift << 8) | indRus1_ButtonsShift )

// ������ � buttonDelayWaitFlag: ������ �� "�� �������" ��� "����� �������" Shift-������ Esc
#define buttonBounceAndPostDelay_Esc ( (indRus1_ButtonEscape << 8) | indRus1_ButtonEscape )

#endif // #ifdef use_ButtonsShift



// ��������� � ������ LCD
#define isIndRus1_RezhimLCD (portLcdDirData & indRus1_ButtonAll)

// ������� �� ����� �� �������� ������
// ����� ����, ��� ������������� ������� ������, � ��� ����� �� ��������, ����� ���������� ������� ��������� ������� ������ 
#define isWaitForButtonBounceDelay (buttonMask_Pressed & 0x00FF)
// ������� �� ����� ����� ������� ��������� ������� ������ 
#define isWaitForButtonPostDelay   (buttonMask_Pressed & 0xFF00)

// ������ �� ������ "�����"
#define isButtonUpPressed     (0==(indRus1_InPort & indRus1_ButtonUp))
// ������ �� ������ "����"
#define isButtonDownPressed   (0==(indRus1_InPort & indRus1_ButtonDown))
// ������ �� ������ "�����"
#define isButtonEnterPressed  (0==(indRus1_InPort & indRus1_ButtonEnter))
// ������ �� ������ "������"
#define isButtonEscapePressed (0==(indRus1_InPort & indRus1_ButtonEscape))





// ����������� ��������� ���1 �� ����� ���������� �� LCD
#define dIndRus1_SetRezhimLCD {portLcdDirData |= indRus1_ButtonAll; /*������� ����������� PxDIR =1 �� �����*/ \
                         portLcdRenData &= (~indRus1_ButtonAll); /*��������� ��������*/ \
                         portLcdOutData &= (~indRus1_ButtonAll); /*�������� ������ = 0*/ \
                         /*__delay_cycles(5);*/ } /*����� �� ����������� �������� �� ����� �����*/

// ����������� ��������� ���1 �� �������� ���������� �� ������
#define dIndRus1_SetRezhimButton {portLcdDirData &= (~indRus1_ButtonAll); /*������� ����������� PxDIR // =0 �� ����*/ \
                         portLcdRenData |= indRus1_ButtonAll; /*��������� ��������*/ \
                         portLcdOutData |= indRus1_ButtonAll; } /*�������� ������� � �������*/

// ������� ����� ������� ������
#define dButtonMask_Pressed_Clear   { buttonMask_Pressed = 0x00; }


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������������ ��������� ������� ������
// ���������� ������ �� 1 ����, ������� ���������� � define
// ���������� ������ ����� ��������� 6 �� �������
// ��������� 6 ������������ ��� ������� ����� ������ (��. rus1_interruptTimer.cpp -> ����������� ������)
////////////////////////////////////////

// ������ "�����"
// #pragma inline=forced //  ��������� Optimization �� ���� medium, �� ��� ����� �����������
// ������ ����������� � ������ ��������� � �������� ������������� ����
// void IndRUS1_PressButtonUp( void )
#define dIndRUS1_PressButtonUp \
  { \
    switch (GetRRP) \
      { \
        case rrp_0: /* ���� ���������� ������ ������� (�� ������, ��������)*/ \
          Tree_PreviousTreeNode(); \
          break; \
        case rrp_1: /* ��� ������������ ������ (������� ������)*/ \
        case rrp_2: /* �������������� ���������� (��������� "�����")*/ \
        case rrp_3: /* �������������� ���������� (��������� "����")*/ \
          SetRRP(rrp_2); \
          currentTreeNode->pFunctionShowInfo(); /*����� ���� FunctionShowInfo != NULL*/ \
          break; \
        case rrp_4: /* ����� ������ ��������� �� ������� ����������� ��������*/ \
        case rrp_5: /* ����� ������ ��������� �� ��������� ��������*/ \
          dResetIndexShowGoString; \
          currentTreeNode->pFunctionShowInfo(); \
          SetRRP(rrp_0); \
          break; \
        default: \
          break; \
      } \
  }
  
  // ������ "����"
// void IndRUS1_PressButtonDown( void )
#define dIndRUS1_PressButtonDown \
  { \
    switch (GetRRP) \
      { \
        case rrp_0: /* ���� ���������� ������ ������� (�� ������, ��������)*/ \
          Tree_NextTreeNode(); \
          break; \
        case rrp_1: /* ��� ������������ ������ (������� ������)*/ \
        case rrp_2: /* �������������� ���������� (��������� "�����")*/ \
        case rrp_3: /* �������������� ���������� (��������� "����")*/ \
          SetRRP(rrp_3); \
          currentTreeNode->pFunctionShowInfo(); /*����� ���� FunctionShowInfo != NULL*/ \
          break; \
        case rrp_4: /* ����� ������ ��������� �� ������� ����������� ��������*/ \
        case rrp_5: /* ����� ������ ��������� �� ��������� ��������*/ \
          dResetIndexShowGoString; \
          currentTreeNode->pFunctionShowInfo(); \
          SetRRP(rrp_0); \
          break; \
        default: \
          break; \
      } \
  }

  // ������ "Enter"
// void IndRUS1_PressButtonEnter( void )
#define dIndRUS1_PressButtonEnter \
  { \
    switch (GetRRP) \
      { \
        case rrp_0: /* ��� ������������ ������ (������� ������)*/ \
          if (isCurrentTreeNode_ChangableData) \
          { \
            SetRRP(rrp_1); \
            currentTreeNode->pFunctionShowInfo(); \
          } \
          else \
            Tree_EnterChildTreeNode(); \
          break; \
        case rrp_1: /* �������������� ���������� (� ���� �� ��������� ���������)*/ \
        case rrp_2: /* �������������� ���������� (��������� "�����")*/ \
        case rrp_3: /* �������������� ���������� (��������� "����")*/ \
          if (GetIsCursorVisible) \
            LCD_SetCursorBlinkOff(); /*���������� ����������, ������ �������� *//*����� �� ��������������*/ \
          SetRRP(rrp_4); \
          dSetIsReInitLCD_ON; /*����� ��������� ����������� ������ � �������� ��������, ����� �������������������� �������*/ \
          LCD_ShowString(lineVupolnenoOk, lcdColCount*lcdRowCount, 0, 0); \
          currentTreeNode->pFunctionShowInfo(); /*���������� ������*/ \
          break; \
        case rrp_4: /* ����� ������ ��������� �� ������� ����������� ��������*/ \
        case rrp_5: /* ����� ������ ��������� �� ��������� ��������*/ \
          dResetIndexShowGoString; \
          currentTreeNode->pFunctionShowInfo(); \
          SetRRP(rrp_0); \
          break; \
        default: \
          break; \
      } \
  }

  // ������ "Cancel"
// void IndRUS1_PressButtonEscape( void )
#define dIndRUS1_PressButtonEscape \
  { \
    switch (GetRRP) \
      { \
        case rrp_0: /* ��� ������������ ������ (������� ������)*/ \
          Tree_OutUpTreeNode(); \
          break; \
        case rrp_1: /* �������������� ���������� (� ���� �� ��������� ���������)*/ \
        case rrp_2: /* �������������� ���������� (��������� "�����")*/ \
        case rrp_3: /* �������������� ���������� (��������� "����")*/ \
          if (GetIsCursorVisible) \
            LCD_SetCursorBlinkOff(); /*���������� ����������, ������ �������� *//*����� �� ��������������*/ \
          SetRRP(rrp_5); \
          dSetIsReInitLCD_ON; /*����� ��������� ����������� ������ � �������� ��������, ����� �������������������� �������*/ \
          LCD_ShowString(lineCancelOk, lcdColCount*lcdRowCount, 0, 0); \
          break; \
        case rrp_4: /* ����� ������ ��������� �� ������� ����������� ��������*/ \
        case rrp_5: /* ����� ������ ��������� �� ��������� ��������*/ \
          dResetIndexShowGoString; \
          currentTreeNode->pFunctionShowInfo(); /*����� ���� FunctionShowInfo != NULL*/ \
          SetRRP(rrp_0); \
          break; \
        default: \
          break; \
      } \
  }


/////////////
// extern
//////////// 

// �����, ������������, ��� �������������� ������� ������
// ����� != 0, �������������� ������
// ���� �� BIT0 �� BIT7 ��������� � ��������� ������� indRus1_InPort, ���������� ���
//    ��������� ������� ������, � ���������� �������� ����� ��� ������ �� ��������
// ���� �� BIT8 �� BIT15 (= "�������� ����� indRus1_InPort" << 8) - ������� �������� ����� �� ��������,  
//    ���������� ������� ������� ������, � ������� ����� ����� ������� ������� ������
extern unsigned int buttonMask_Pressed;

// ������� ���������� ������� � ������
extern unsigned int interruptsIndRus1_ButtonCnt;

// ������� ������������ ������� lcd ��� ��������� ���������� [����� �������]
extern unsigned int f_timerCounterChangePerem;
// ������� ������������ ������� lcd ��� ��������� deltaChange, ��� ������� ������
extern unsigned int f_timerCounterLnChangeKeyPressed;
// ������� ������������ ������� lcd ��� ��������� deltaChange, ��� ��������� ������
extern unsigned int f_timerCounterLnChangeKeyUnPressed;



///////////////////////////
// ��������� �����������
///////////////////////////

// ����������� ��������� ���1 �� ����� ���������� �� LCD
void IndRus1_SetRezhimLCD();

// ����������� ��������� ���1 �� �������� ���������� �� ������
void IndRus1_SetRezhimButton();

#endif // #ifndef __rus1_indicatorRUS1__