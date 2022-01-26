
// .h-���� �������� �������� ������ �� ������ � ������������ ������ ���-1

#ifndef __rus1__
#define __rus1__

#include "rus1_mainTitle.hpp"

// ��������� �� (NULL == *(currentTreeNode->pFunctionShowInfo)
// #define verify_FunctionShowInfo_NULL


// ���� �� ���������� ������� ������, LCD_ShowGoString ������������ - ������
#ifndef use_GoString
  #define LCD_ShowGoString LCD_ShowString
#endif

//////////////////////////////////// 
// ��� ������������� ���������� ���-1 
//////////////////////////////////// 

// ����� �������������� ���������� (���)
#define rrp_0 0 //  ��� ������ �������������� ���������� (������� ������)
#define rrp_1 1 //  �������������� ���������� (� ���� �� ��������� ���������)
#define rrp_2 2 //  �������������� ���������� (��������� "�����")
#define rrp_3 3 //  �������������� ���������� (��������� "����")
#define rrp_4 4 //  ����� ������ ��������� �� ������� ����������� �������� // ����� ���������!
#define rrp_5 5 //  ����� ������ ��������� �� ��������� ��������

// ����� � ������� ������� ������� ������� ��������
#define cursorKatalog 0x3E // 62

// ����� � ������� ������� ������� ����������� ��������� ����������
// ��������� � ����� ������, ������������ �������� ����������
#define cursorEnter 133 // 0x7E // 126

// ����� � ������� ������� ������� ��������� ��������, ������� �������� ������
// #define cursorLastKatalog 0xFF // 255
#define cursorLastKatalog 187 // 0xDC // 220



// ���������� ��������, ������������ �� ���� �������� ��������� ��������
#define tree_ZnakKatalogCount ((unsigned char)(1))
// ���������� �������� � ������ �������� ��������, ����������� �� �����
#define tree_NameKatalogCount ((unsigned char)(lcdColCount-tree_ZnakKatalogCount))

// ������������ ����� ������, �������� �� ������ ���������������� �������
// ��� vlTree.f_commandsCount
#define commandsCountMax 4


  
//////////////////////////////// 
// ��������� ��� ������� ������
//////////////////////////////// 

// �� ����, ��� ������ ����� ���������
// ���������� ������� LCD_ShowGoString
#define pauseCntBeforeGoString 30
// ����� ����, ��� ������ "�������" �� �����
// ���������� ������� LCD_ShowGoString
#define pauseCntAfterGoString 30

// ������ ���������� �������� ������� ������
// ������ GoStringSdvigUndex ������� ��������� ������
//  = 2 ^ goStringSdvigUndex
// ����� �� 1 = ��������� �� 2
#define goStringSdvigUndex 1




////////////////////////////// 
// ��� ��������� ������ �������
////////////////////////////// 

// ���������� NULL-�������
// #ifdef verify_FunctionShowInfo_NULL
  #define Tree_ShowNULLkatalog() \
  LCD_ShowString( &tree_ShowNULLkatalogString[tree_ZnakKatalogCount], tree_NameKatalogCount, tree_ZnakKatalogCount, GetScreenRowIndex )
// #endif

// ��������� � �������� �������
#define Tree_JumpToRootKatalog() { Rezhim6_On(Show_Root); currentTreeNode = &treeNode_Root; }

// ������� ��� ������� � ������ 0 (��������� � ����), �� ������� col �� ����� ������
#define LCD_ClearCharsToEndFromRow0(col) LCD_ShowString(line_Probel, lcdColCount-col, col, 0)
// ������� ��� ������� � ������ 1 (��������� � ����), �� ������� col �� ����� ������
#define LCD_ClearCharsToEndFromRow1(col) LCD_ShowString(line_Probel, lcdColCount-col, col, 1)

// ������� ��� ������� � ������ 0 (��������� � ����), � ������ �� ������� col (��������� � ����, ������������)
#define LCD_ClearCharsFromBeginFromRow0(col) \
   LCD_ShowString(line_Probel, col, 0, 0)
// ������� ��� ������� � ������ 1 (��������� � ����), � ������ �� ������� col (��������� � ����, ������������)
#define LCD_ClearCharsFromBeginFromRow1(col) \
   LCD_ShowString(line_Probel, col, 0, 1)
     
     
     
// currentTreeNode
/////////////////

// ��������� �� TreeNode �� ���� �����
#define isCurrentTreeNode_FullScreen ( (0 == currentTreeNode->childsTreeNodeCount) | isCurrentTreeNode_Root )
// �������� �� TreeNode ��������
#define isCurrentTreeNode_Root (currentTreeNode == &treeNode_Root)
// ��������� �� �������� ���� ������ �������� ������
// #define isCurrentTreeNode_ChangableData (BIT0 & currentTreeNode->flag)
#define isCurrentTreeNode_ChangableData ( 0!= (BIT0 & currentTreeNode->flag) )


// ����� �������������?
#define isRezhimSpecMessage ( NULL != vlTree.f_strSpecMessage )
// ������ ������� ������ �������������?
#define isRezhimSpecMessage_Time100msShow ( 0 != vlTree.f_100msSpecMessageCnt )

// ���� �� ��� � ������ ������� �������
#define lcd_IsCommandON (0 != GetCommandsCount)

// ���� �� ��������� ��������� ������
// ��� ��������� �������������� ���������� ����� �������
#define isNeedSaveData ( (0 != GetDeltaChange) & (rrp_4 == GetRRP) )





// ����� ������� ��� ������� [�������� �� ������� ���������������� �������]
// ���������� ���������� �������
// ������� ����������� ������ ��-�������
#define dLCD_CommandMake { LCD_WriteCommandQuickly( vlTree.f_commands[0]); \
  vlTree.f_commandsCount--; \
  if (vlTree.f_commandsCount) { \
    unsigned char* pFromCommandMake = &vlTree.f_commands[1]; \
    for (unsigned char* pToCommandMake = &vlTree.f_commands[0]; \
         pToCommandMake<(&vlTree.f_commands[0]+vlTree.f_commandsCount); \
         pToCommandMake++, pFromCommandMake++) \
      *pToCommandMake = *pFromCommandMake; \
  } } \
/*
#define dLCD_CommandMake { LCD_WriteCommandQuickly( vlTree.f_commands[vlTree.f_commandsCount-1]); \
                              vlTree.f_commandsCount--; }
*/
// ����� ������� ������
#define dLCD_CommandClear { vlTree.f_commandsCount = 0; }


#ifdef use_IndRUS1

// ��������� ����������, ������ �������  
#define dLCD_SetCursorBlinkOn  { unsigned char c_tmp = 0x0F; LCD_CommandsAddToBuf(&c_tmp, 1); dSetIsCursorVisibleON; }
// ��������� ����������, ������ ��������  
// #define dLCD_SetCursorBlinkOff { unsigned char c_tmp = 0x0C; LCD_CommandsAddToBuf(&c_tmp); dSetIsCursorVisibleOFF; }



// ���������� "����" ���������� ���������� ��������� ������
// ��. isNeedSaveData
#define dSetNeedSaveData_Off dResetDeltaChange

#endif // #ifdef use_IndRUS1


/////////////////////////////////////////
// ��� �������� ��� ������ � ������ ������
/////////////////////////////////////////


// struct t_VLtreeWork
///////////////////////


// f_rezhimSpecMessage
/////////////////////

// ����� ������ �������� ���������
/////////////////////////////////
// ���������� �������������
#define dShowRezhimSpecMessage { LCD_ShowString(vlTree.f_strSpecMessage, lcdColCount*lcdRowCount,0,0); }
// ������������� ����� ������������� // ���� ��� ���������� ���������� � f_ , ������ ������ � ��� ������ ����� �������������
#define dSetSpecRezhimSpecMessage(x, y, z) { vlTree.f_strSpecMessage = x; \
                                             vlTree.f_100msSpecMessageCnt = y; \
                                             dSetTypeSpecMessage(z); }
// ��������� vlTree.f_rezhimSpecMessageShow100msCnt
#define dDecRezhimSpecMessageTime100msCnt { vlTree.f_100msSpecMessageCnt--; }
// ����� �������� ������ ���������
#define dResetRezhimSpecMessageTime100msCnt { vlTree.f_100msSpecMessageCnt = 1; } // 0 ������ ������ - ����� ��������� �������� dDecRezhimSpecMessageTime100msCnt
// ��������� vlTree.f_100msSpecMessageCnt
#define SetRezhimSpecMessage_Show100ms( x ) { vlTree.f_100msSpecMessageCnt = x; }


#ifdef use_IndRUS1

// f_flag
//////// 

// f_flag
  // BIT0, BIT1, BIT2 - ����� �������������� ���������� (���)
  // ��� ����� ��� "������" ������ � �������� ��������� ��������.
  // ���������� ������:
  //  specRezhim_0 - ��� �������������� ���������� (������� ������)
  //  specRezhim_1 - �������������� ���������� (� ���� �� ��������� ���������)
  //  specRezhim_2 - �������������� ���������� (��������� "�����")
  //  specRezhim_3 - �������������� ���������� (��������� "����")
  //  specRezhim_4 - ����� ������ ��������� � ������� ����������� �������������� ����������
  //  specRezhim_5 - ����� ������ ��������� �� ������ �������������� ����������
// �������� ���
#define GetRRP ( vlTree.f_flag & (BIT0 + BIT1 + BIT2) )
// ��������� ���
// #define dSetRRP( x ) { vlTree.f_flag &= (~(BIT0 + BIT1 + BIT2)); vlTree.f_flag |= (x); \
//                        if (rrp_0 != x) { dResetCnt1s_ChangeOff; dLCD_CommandClear; } }

// f_flag
// BIT3 ����������, ����� ������ ������� "�������" (�����)
#define GetIsCursorVisible    ( 0 != (vlTree.f_flag & BIT3) )
#define dSetIsCursorVisibleON  { vlTree.f_flag |= BIT3; }
#define dSetIsCursorVisibleOFF { vlTree.f_flag &= (~BIT3); }

// f_flag
// BIT4 ����������, ����� TreeNode ����������� ���� �� 1 ���
#define GetIsTreeNode_Displayed    ( 0 != (vlTree.f_flag & BIT4) )
#define dSetIsTreeNode_DisplayedON  { vlTree.f_flag |= BIT4; }
#define dSetIsTreeNode_DisplayedOFF { vlTree.f_flag &= (~BIT4); }

// f_flag
// BIT5 ����������, ����� ������ ������������ ��������� (�������) �� ������� � ��������� ������
#define GetIsShowKatStrelka ( 0 != (vlTree.f_flag & BIT5) )
#define dSetIsShowKatStrelka_ON { vlTree.f_flag |= BIT5; }
#define dSetIsShowKatStrelka_OFF { vlTree.f_flag &= (~BIT5); }
#define dSetIsShowKatStrelka( x ) { if (x) dSetIsShowKatStrelka_ON else dSetIsShowKatStrelka_OFF }
#define dReverseIsShowKatStrelka { if (GetIsShowKatStrelka) dSetIsShowKatStrelka_OFF else dSetIsShowKatStrelka_ON; }

// f_flag
// BIT6 ����� ������� ���������������� �������� ������� �� ���� ���������, BIT6=false
//      ���� �� �������������� ���������� ������� ���������������� �������
// f_CursorVisiblePosition_Need
#define GetIsCursorVisiblePosition_Need ( 0 != (vlTree.f_flag & BIT6) )
#define dSetIsCursorVisiblePosition_Need_ON { vlTree.f_flag |= BIT6; }
#define dSetIsCursorVisiblePosition_Need_OFF { vlTree.f_flag &= (~BIT6); }

// f_flag
// BIT7 ���� �� �������������������� �������
#define lcd_IsReInitLCD ( 0 != (vlTree.f_flag & BIT7) )
#define dSetIsReInitLCD_ON { vlTree.f_flag |= BIT7; }
#define dSetIsReInitLCD_OFF { vlTree.f_flag &= (~BIT7); }

// f_flag
// BIT6 | BIT7
#define lcd_IsReInitLCD_or_IsCursorVisiblePosition_Need ( 0 != (vlTree.f_flag & (BIT6 + BIT7)) )
         
// f_ScreenRowIndex // � ����� ������ LCD-������ ��������� ������
#define GetScreenRowIndex vlTree.f_ScreenRowIndex
#define dSetScreenRowIndex( x ) { vlTree.f_ScreenRowIndex = (x); }
#define dResetScreenRowIndex { vlTree.f_ScreenRowIndex = 0; }
#define dIncScreenRowIndex { vlTree.f_ScreenRowIndex++; }
#define dDecScreenRowIndex { vlTree.f_ScreenRowIndex--; }

// f_TreeNodeActiveIndex // ������ ��������� TreeNode � ������������� TreeNode
#define GetTreeNodeActiveIndex    vlTree.f_TreeNodeActiveIndex
#define dSetTreeNodeActiveIndex(x) { vlTree.f_TreeNodeActiveIndex = x; }
#define dIncTreeNodeActiveIndex { vlTree.f_TreeNodeActiveIndex++; }
#define dDecTreeNodeActiveIndex { vlTree.f_TreeNodeActiveIndex--; }
#define dResetTreeNodeActiveIndex { vlTree.f_TreeNodeActiveIndex=0; }

// f_TreeNodeOfFirstRow
#define GetTreeNodeOfFirstRowIndex vlTree.f_TreeNodeOfFirstRow
#define dSetTreeNodeOfFirstRowIndex(x) { vlTree.f_TreeNodeOfFirstRow = x; }
#define dIncTreeNodeOfFirstRowIndex { vlTree.f_TreeNodeOfFirstRow++; }
#define dDecTreeNodeOfFirstRowIndex { vlTree.f_TreeNodeOfFirstRow--; }
#define dResetTreeNodeOfFirstRowIndex { vlTree.f_TreeNodeOfFirstRow=0; }

// f_lev10
#define GetLnChange vlTree.f_lev10
#define dIncLnChange { if (GetLnChange<GetLnChangeMax) GetLnChange++; }
#define dDecLnChange { if (GetLnChange>0) GetLnChange--; }
#define dResetLnChange { GetLnChange = 0; }

// f_lev10Max
#define GetLnChangeMax vlTree.f_lev10Max
#define dSetLnChangeMax( x ) { GetLnChangeMax=x; if (GetLnChange > GetLnChangeMax) GetLnChange = GetLnChangeMax; }
#define dResetLnChangeMax { GetLnChangeMax = 0; }

// f_deltaChange
#define GetDeltaChange vlTree.f_deltaChange
#define dResetDeltaChange { vlTree.f_deltaChange = 0; }

// f_xBaseCursor
#define GetBaseXCursor vlTree.f_xBaseCursor
#define dSetBaseXCursor( x ) { vlTree.f_xBaseCursor = x; }

#endif // #ifdef use_IndRUS1

// f_typeSpecMessage
#define GetTypeSpecMessage vlTree.f_typeSpecMessage
#define isTypeSpecMessage_ChangeOff (GetTypeSpecMessage | BIT5)
#define dSetTypeSpecMessage( x ) { GetTypeSpecMessage = x; } // ( (BIT7 & GetTypeSpecMessage) | x ); }
// #define dSetTypeSpecMessage_Critical { GetTypeSpecMessage = BIT7; dGreenLed_Off; } // ������� ����������� ��������� �����
#define dSetTypeSpecMessage_Critical_onZero { GetTypeSpecMessage = BIT6; }
#define dSetTypeSpecMessage_NonCritical { GetTypeSpecMessage &= (~(BIT6|BIT7)); dRedLed_Off; } // ������� ����� ������������ ���������
#define isWasTypeSpecMessage_CriticalError (BIT7 & GetTypeSpecMessage) // ���� �����-���� ����������� ��������� �� ������
#define isSpecMessage_ChangeToCritical_onZero (BIT6 & GetTypeSpecMessage)

// f_commandsCount
#define GetCommandsCount vlTree.f_commandsCount

// f_commands[commandsCountMax]
#define GetNextCommand vlTree.f_commands[0]
#define dAddCommand(x) { vlTree.f_commands[vlTree.f_commandsCount] = x; vlTree.f_commandsCount++; }

// ������ ��� ������������� ��������� ����������� ������
// � ���������, ��� ������� ������
// f_indexShowGoString
#define GetIndexShowGoString vlTree.f_indexShowGoString
#define dResetIndexShowGoString { vlTree.f_indexShowGoString = 0; }
/*void dIncIndexShowGoString(unsigned char maxCount) 
{ vlTree.f_indexShowGoString++; if (vlTree.f_indexShowGoString > (maxCount-1)) \
          vlTree.f_indexShowGoString = 0; }*/

#define dIncIndexShowGoString(maxCount) { vlTree.f_indexShowGoString++; \
   if (vlTree.f_indexShowGoString > (maxCount-1)) vlTree.f_indexShowGoString = 0; }
// void dIncIndexShowGoString(unsigned char maxCount) { vlTree.f_indexShowGoString++; \
  //  if (vlTree.f_indexShowGoString > (maxCount-1)) vlTree.f_indexShowGoString = 0; }

// #define dIncIndexShowGoStringBy(maxCount, delta) { vlTree.f_indexShowGoString += delta; if (vlTree.f_indexShowGoString > (maxCount-1)) \
  //         vlTree.f_indexShowGoString = 0; }






/////////////////////////
// ���������

  // �� ������ pFunctionShowInfo == NULL
  //                                                                  0                   5       7           10                  15 
#define tree_ShowNULLkatalogStringCnt 16
#define dTree_ShowNULLdata { LCD_ShowString(tree_ShowNULLkatalogString, tree_ShowNULLkatalogStringCnt,0,0); \
                                 LCD_ShowString(tree_ShowNULLkatalogString, tree_ShowNULLkatalogStringCnt,0,1); }


// �������� ��� �������� �� ������ 1 � ������ 0 (��������� � ����)
#define dKatalogCopyRow1toRow0 { unsigned char* pRow1 = pLcdBufBegin+tree_ZnakKatalogCount + lcdColCount; \
            for (unsigned char* pRow0 = pLcdBufBegin+tree_ZnakKatalogCount; pRow0 < pLcdBufBegin + lcdColCount; pRow0++, pRow1++) \
              *pRow0 = *pRow1; }
// �������� ��� �������� �� ������ 0 � ������ 1 (��������� � ����)
#define dKatalogCopyRow0toRow1 { unsigned char* pRow1 = pLcdBufBegin+tree_ZnakKatalogCount + lcdColCount; \
            for (unsigned char* pRow0 = pLcdBufBegin+tree_ZnakKatalogCount; pRow0 < pLcdBufBegin + lcdColCount; pRow0++, pRow1++) \
              *pRow1 = *pRow0; }


//////////////// 
// typedef-�
//////////////// 

// ��� �������
typedef void (*TFunPtr)( void ); //  ����������/���������� �������� �� �����



////////////////////////////////// 
////////////////////////////////// 
// ���������,������� ���� ���������
////////////////////////////////// 
////////////////////////////////// 

/////////////////////////
// ��� ���������� � ���
///////////////////////////////

// ������ ��� ������ � ������������ ������ ���-1
struct t_VLtreeWork
{
#ifdef use_IndRUS1
  // ���������� ��� ������� ������ �������/������
  //////////////////////////////////////////////////////// 

  // ����� ���� 
  ////////////// 
  // BIT0, BIT1, BIT2 - ����� �������������� ���������� (���)
  // ��� ����� ��� "������" ������ �:
  //  - �����������.
  //  - ���������� �������� �������������.
  // ���������� ������:
  //  rrp_0 - ��� �������������� ���������� (������� ������)
  //  rrp_1 - �������������� ���������� (� ���� �� ��������� ���������)
  //  rrp_2 - �������������� ���������� (��������� "�����")
  //  rrp_3 - �������������� ���������� (��������� "����")
  //  rrp_4 - ����� ������ ��������� � ������� ����������� �������������� ����������
  //  rrp_5 - ����� ������ ��������� �� ������ �������������� ����������
  // BIT3 ����������, ����� ������ ������� "�������" (�����)
  // BIT4 ����������, ����� TreeNode ����������� ���� �� 1 ���
  // BIT5 ����������, ����� ������ ������������ ��������� (�������) �� ������� � ��������� ������
  // BIT6 ����� ������� ���������������� �������� ������� �� ���� ���������, BIT6==false // ���� �� �������������� ���������� ������� ���������������� �������
  // BIT7 ��������� ������� �������  
  unsigned char f_flag;
  // ������ � ����� ������ ��������� ������ // ��� ������� ������ ����
  unsigned char f_ScreenRowIndex;
  
  // TreeNode - ���� ������ �������/������
  // ������ ��������� TreeNode � ������������� TreeNode
  signed char f_TreeNodeActiveIndex; 
  // ������ � ������� �������� treeNode [� ������������� TreeNode], ������� � ������ ������ �������
  signed char f_TreeNodeOfFirstRow;                   
  
  // ��������� ���������� [������������� ����� �������]
  // � ����� ������, �������� f_deltaChange �� ��������� 10 //  lg(f_deltaChange) = f_lev10
  // ����� �� ���� ����������� � ���� ����������, ������������ ������ ������� ����� ������ � ����� ���������
  // ��� ������� ������ �������������, ����� ������ �������� //  + ��� �������� �������������� ���������� �� ������
  // ������ � ����
  /////////////////////////////////////////////////////////
  unsigned char f_lev10; // lg(f_deltaChange) = f_lev10 
  // ����������� ���������� �������� f_lev10
  unsigned char f_lev10Max;
  
  // ��������� ���������� [������������� ����� �������]
  /////////////////////////////////////////////////////////
  // ������� ����� ���������� � ����� ���������, ��. f_lnChange
  signed long f_deltaChange;
  // ����� ��������� ������� ���������������� ������� [��� ��������� ���������� ����� �������]
  // ��������� [�������] ���������� �������
  unsigned char f_xBaseCursor;
  
#endif    // #ifdef use_IndRUS1
  
  // ����� ������������� // �� ������������ �������������!
  // ������� currentTreeNode->pFunctionShowInfo ���� ���������� ��������� �� ���� ����� 
  // ����� ���� ���������� ��� ����� ������ ��������������
  // ����� ������������� �������, ����� f_strSpecMessage != NULL
  // ����� ������������� ��������, ����� f_strSpecMessage == NULL
  // ����� �� �����������, �.�. ��� ������ = (lcdRowCount*lcdColCount)
  const unsigned char* f_strSpecMessage;  
  // ������� 100�� ���������� �������������
  // ���� == 0, �� ��_�������_������/����������
  unsigned char f_100msSpecMessageCnt;
  // ��� �������������
  // 0 - "�������"
  // BIT0 - "�������" �� ������
  // BIT1 - "�������" ����� ������
  // BIT2 - "������" �� ������
  // BIT3 - "������" ����� ������
  // ���� ��������� BIT5 - ������, ����� �������� cnt1s_ChangeOff ������, ���������� ����� �������������
  // ���� ��������� BIT6 - ������, �� ��������� ������� ������ �������������, ��������� BIT7; // ������ ������ ��������� ������������ �����������
  // ���� ��������� BIT7 - ������, "�����������" �������������; // ��������� ������ ������
  unsigned char f_typeSpecMessage;
  
  // ����� ��������� �����������,�������� �� ������� ���������������� �������
  // ������� ������, �������� �� ������� ���������������� �������
  unsigned char f_commandsCount;  
  unsigned char f_commands[commandsCountMax];
  
  // ������ ��� ������������� ��������� ����������� ������
  // � ���������, ��� ������� ������
  unsigned int f_indexShowGoString;

};



//////////////////////// 
// ��� ���������� �� ����
//////////////////////// 

// ��������� ������� ���� ������ [treeNode]
// treeNode - ���� ������ �������/������
#ifdef use_IndRUS1
struct t_VLtreeNode
{
// TreeNode ������� ��:
//  - ��������� �� ����������� TreeNode; == 0 ������ � root
//  - ��������� �� ������� ������ ���������� TreeNode �� �����
//  - ������� �������� ��������� ���� t_VLtreeNode; ������� "������ ����"
//  - ��������� �� ������ ��� ������� ������ ���������� TreeNode �� �����
//  - ����� ������. ������� � ��������:
//    * BIT0 - ��������� �� treeNode �������� ������
//    * BIT1-BIT7 - ������
  
  // ��������� �� ��������; ����� == NULL, ������ � �����
  const t_VLtreeNode* pParent;                
  
  // ��������� �� ������� ������ ���������� TreeNode �� �����
  const TFunPtr pFunctionShowInfo;            
  
  // ������ �������� TreeNode
  const t_VLtreeNode* const *pChildsTreeNode; 
  
  // ��������� �� ������ ��� ������� ������ ���������� TreeNode �� �����
  const void* pDataForChange;                 
  
  // ������ ������� �������� TreeNode
  const unsigned char childsTreeNodeCount;    
  
  // ����� ������
  const unsigned char flag;                   
};

#endif // #ifdef use_IndRUS1


///////////////
// extern
//////////// 

extern t_VLtreeWork vlTree;

#ifdef use_IndRUS1
  extern const t_VLtreeNode* currentTreeNode;
#endif

/////////////////////////////////
// ���������
/////////////////////////////////

// ������� ����� ������������ ���������
// void SetTypeSpecMessage_NonCritical();

// ����� ����- � ������������ ���������
void ShowCriticalMessage();

// ��������� ����������, ������ ��������  
void LCD_SetCursorBlinkOff();

#ifdef use_GoString
// ����� ������� ������ �� ��� ������ ������� � ������ row, ������� � ������� colStart
// ������� �������� ������ ������ ������� tree_ZnakKatalogCount
// ������ ���� ������������ ������� ������ �� ���� ����� 
// ����� ����� ������� ��������� ����� //  ����������, ����� �������������� ���������� ���������� �� ��������� ������, � �� ������� �������
//                     ��������� ������                                      � ����� �������         � ����� ������ ������
void LCD_ShowGoString(unsigned char const* goStr, unsigned char goStrCount, unsigned char colStart = 0, unsigned char rowScreen = 0);
#endif

// ���������� ������ ������� � �������
void LCD_CommandsAddToBuf( const unsigned char* cmds, unsigned char cmdsCnt = 1 );



/////////////////////////////
// ����� �������������
///////////////////////////

// ������������� ����� ������������� (����� ������ �������� ���������)
// ������� currentTreeNode->pFunctionShowInfo ���� ���������� 
// �������, ������������ ��������� (�� ������,��������)
//                         �������       �� 100�� ������� �����-�       ����� ���������� ��������: 
//                                       1 ������� == 100��;            BIT0 - ������� 
//                                       0 - �� ������� ������          BIT1 - ������
// bool RezhimSpecMessage_On( TFunPtr fun, unsigned int show100msCnt = 0, unsigned char ledsOn = 0 );
void RezhimSpecMessage_On( const unsigned char* str, unsigned int show100msCnt, unsigned char ledsOn );

// ������� �� ������ �������������, ��������������� ���������� ������
void RezhimSpecMessage_Off();



#ifdef use_IndRUS1

// ��������� ���
void SetRRP( unsigned char x );

// ���������� ������� ������ �������� ��������
void LCD_ShowGoString_Katalog();



////////////////////////////////////////////////////////////////////////////// 
// ������������ ������ ������������ ���������� [�������/������].
////////////////////////////////////////////////////////////////////////////// 

  // ������� ������� TreeNode �� �������
void Tree_ShowVisibleTreeNodes();

#endif // #ifdef use_IndRUS1
#endif // #ifndef __rus1__