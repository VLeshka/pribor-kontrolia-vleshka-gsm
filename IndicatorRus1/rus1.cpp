
////////////////////////////////////////////////////////////// 
// ������� ������� ������ �� ������ � ������������ ������ ���-1
////////////////////////////////////////////////////////////// 

/*������ ������:
- includ-�, �������� ��������� rus1_mainTitle.h.
- "�������" ������������ ������ ������������� ���������� [�������/������].
- "�������" ������������ ������ ������������ ���������� [�������/������].
- ������������ "��������" ��� ������ [�������/������].*/

////////// 
// includes
////////// 

#include "rus1_peripherals.hpp"       // ������ ��� ������ � ����������
#include "example_base_functions.hpp"    // ������� �������, ��������� �� ������ �������� ������
#include "rus1.hpp"
#include "rus1_screenLCD_AC-162A.hpp"      // ������ ��� ������ � lcd
#include "example_base_strings.hpp"
    
#ifdef use_IndRUS1
  #include "rus1_stencil_funcs_changedata.hpp"
  #include "vl_TreeStructureGenerator_Structure.hpp"     // ��������� ������ TreeNode
  // ��������� �� �������� TreeNode
  //////////////////////////// 
  const t_VLtreeNode* currentTreeNode = &treeNode_Root; 
  #include "rus1_stencil_funcs_changedata.hpp" // ������������-��������� ������ ������
  #include "rus1_indicatorRUS1.hpp"     // ��������� ���1 (LCD-������ � 4 ��������)
  #include "rus1_treenode_go.hpp"              // ������������ "��������" �� ������ TreeNode
#endif

#include "gsm.hpp"
#include "peripherals.hpp"

// ������ ��� ������ � ������������ ������ ���-1
t_VLtreeWork vlTree = {
#ifdef use_IndRUS1
            0,0,
            0,0,
            0,0,
            0,0,
            
#endif    // #ifdef use_IndRUS1
            NULL, 0, 0,
            0, { 0x00, 0x00, 0x00, 0x00},
            0};  



//////////////////////// 
// ������������
//////////////////////// 
/*
// ������� ����� ������������ ���������
void SetTypeSpecMessage_NonCritical() 
{ 
  GetTypeSpecMessage &= (~(BIT6|BIT7)); 
  dRedLed_Off; 
}
*/

// ����� ����- � ������������ ���������
void ShowCriticalMessage()
{
  if (!isWasTypeSpecMessage_CriticalError)
    dShowRezhimSpecMessage
  else
  {
  // ���������� GetIndexShowGoString
  // if (NULL != errorScreen.word) 
    if (10 > GetIndexShowGoString)
      LCD_ShowString(errorScreen.word, lcdColCount*lcdRowCount,0,0); // ������� ��������� � ����������� �������
    else
      if (15 > GetIndexShowGoString)
        LCD_Clear();
    if (15 <= GetIndexShowGoString)
      GetIndexShowGoString = 0;
    else
      GetIndexShowGoString++;
  }
}

// ��������� ����������, ������ ��������  
void LCD_SetCursorBlinkOff() { unsigned char c_tmp = 0x0C; LCD_CommandsAddToBuf(&c_tmp); dSetIsCursorVisibleOFF; }

// ���������� ������ ������� � �������
void LCD_CommandsAddToBuf( const unsigned char* cmds, unsigned char cmdsCnt ) 
{ 
  if ( cmdsCnt > (commandsCountMax - GetCommandsCount) )
  {
#ifdef use_IndRUS1
    IndRus1_SetRezhimLCD();
#endif    
    for (unsigned char cc = GetCommandsCount; cc < (commandsCountMax - GetCommandsCount); cc++)
    {
      LCD_WriteCommandQuickly(GetNextCommand); 
      DelayMicroS(lcdCmdWriteDataToRamDelay);
    }
#ifdef use_IndRUS1
    IndRus1_SetRezhimButton();
#endif
  }
  for (unsigned char cc=0; cc<cmdsCnt; cc++, cmds++)
    dAddCommand(*cmds);
}


#ifdef use_GoString
// ����� ������� ������ �� ��� ������ ������� � ������ row, ������� � ������� colStart
// ������� �������� ������ ������ ������� tree_ZnakKatalogCount
// ������ ���� ������������ ������� ������ �� ���� ����� 
// ����� ����� ������� ��������� ����� //  ����������, ����� �������������� ���������� ���������� �� ��������� ������, � �� ������� �������
//                     ��������� ������                                      � ����� �������         � ����� ������ ������
void LCD_ShowGoString(unsigned char const* str, unsigned char strCount, unsigned char colStart, unsigned char rowScreen)
{
  // ������� ������� LCD_ShowGoString ��������� ��� ����, ����� ���������� �������� ������
  // ������������� �� �� ����� ������, � �� ���������� ������� LCD_ShowGoString
  unsigned char outStrCnt = (lcdColCount-colStart);
  if (strCount < outStrCnt)
    LCD_ShowString(str, strCount, colStart, rowScreen);
  else
  {
    unsigned char timer_StrGo = ((strCount - outStrCnt) << goStringSdvigUndex);  
    // ���� ���� � ��������� �������
    if (!GetIndexShowGoString) // ���� ����� �� �� LCD_ShowGoString_Katalog
      LCD_ShowString(str, outStrCnt,colStart,rowScreen);
    else
      if (GetIndexShowGoString>pauseCntBeforeGoString)
        if (GetIndexShowGoString<=(pauseCntBeforeGoString+timer_StrGo+goStringSdvigUndex))
          if ( 0 == (GetIndexShowGoString & ((unsigned int)(0xFFFF >> (16-goStringSdvigUndex))) ) ) // ������ ��� �������� = 2 (����� �� 1)!
          {
            unsigned char indexX = (unsigned char)(GetIndexShowGoString - pauseCntBeforeGoString) >> goStringSdvigUndex;
            LCD_ShowString(str+indexX, outStrCnt, colStart, rowScreen);
          }
    // ������ f_indexShowGoString, �� ��������� ���
    // ����� ��� ������� ������ (� root)
    // � ��. ��������
    dIncIndexShowGoString(timer_StrGo+(pauseCntBeforeGoString+pauseCntAfterGoString));
  }    
}
#endif


/////////////////////////////////////
// ����� �������������
/////////////////////////////////////

// ������������� ����� ������������� (����� ������ �������� ���������)
// ������� currentTreeNode->pFunctionShowInfo ���� ���������� 
// �������, ������������ ��������� (�� ������,��������)
//                         �������       �� 100�� ������� �����-�   ����� ���������� �������� (���� ����� show100msCnt): 
// ��. t_VLtreeWork
// bool RezhimSpecMessage_On( TFunPtr fun, unsigned int show100msCnt, unsigned char ledsOn )
void RezhimSpecMessage_On( const unsigned char* str, unsigned int show100msCnt, unsigned char typeMessage )
{
  /*if (str != NULL) ��� �������� ����� */
  if (vlTree.f_strSpecMessage == str)
    if (isTypeSpecMessage_ChangeOff)
      return;
  if (!isWasTypeSpecMessage_CriticalError)
  { 
#ifdef use_IndRUS1
    if (0 == GetRRP) // ���� ����������� ����������, �� �� �������� - ������ �������� ������������ � ��
    {
      const unsigned char* s = NULL; // ��� �������� �����
      if ( (!isCurrentTreeNode_Root) | (str != errorScreen.word) )
        s = str;
      dSetSpecRezhimSpecMessage(s, show100msCnt, typeMessage);        
      // dShowRezhimSpecMessage; ��� �������� �����
    }
#endif
    // ����������
    // ������
    unsigned char usialType = (typeMessage & (~(BIT6 | BIT7)));    
    if ((BIT0 | BIT1) & usialType)
    {
      dGreenLedMust_100msOn(show100msCnt);    
      dGreenSignal_0; // ������ ����������� �����������
    }
    else
      dGreenLed_Off;
    // �������    
    if ((~(BIT0 | BIT1)) & usialType)
    {
      dRedLedMust_100msOn(show100msCnt);
      dRedSignal_0; // ������ ����������� �����������
    }
    else
      dRedLed_Off;
    // return(true);
  }
  // else return(false);
}

// ������� �� ������ �������������, ��������������� ���������� ������
/*���� ����������, ����� ��� �������������� ��������� �� �����������*/
// ������� �� ������ �������������
void RezhimSpecMessage_Off() 
{ 
  vlTree.f_strSpecMessage = NULL;
  dSetTypeSpecMessage(0);
  Tree_ShowVisibleTreeNodes(); 
  dRedAndGreenLed_Off;
}



#ifndef use_IndRUS1
  #define LCD_ShowGoString_Katalog LCD_ShowString
#endif
    
#ifdef use_IndRUS1

// ��������� ���
void SetRRP( unsigned char x ) 
{ 
  vlTree.f_flag &= (~(BIT0 + BIT1 + BIT2)); 
  vlTree.f_flag |= (x);
  // if (rrp_0 != x) 
    // dResetCnt1s_ChangeOff;
}

// ���������� ������� ������ �������� ��������
void LCD_ShowGoString_Katalog() 
{
  t_strAndStrCnt* strAndStrC = (t_strAndStrCnt*)currentTreeNode->pDataForChange;
  // ���� TreeNode ������������ � ������ ���
  if ((!GetIsTreeNode_Displayed) & (!isCurrentTreeNode_FullScreen))
  { 
    // ������� ���� ��������� �� �������
    unsigned char* dataXCoord = pLcdBufBegin; 
    if (GetScreenRowIndex) dataXCoord += lcdColCount; 
    if (GetIsShowKatStrelka) 
    {
      //  - �������, ������� �������� ������
      if (1==currentTreeNode->childsTreeNodeCount)
      {
        if (0==currentTreeNode->pChildsTreeNode[0]->childsTreeNodeCount)
          *dataXCoord = cursorLastKatalog;
        else
          *dataXCoord = cursorKatalog;
      }
      else
        *dataXCoord = cursorKatalog;
    }
    else
      *dataXCoord = ' '; 
    unsigned char goStrCount = strAndStrC->strCnt;
    if (goStrCount > (lcdColCount - tree_ZnakKatalogCount))
      goStrCount = (lcdColCount - tree_ZnakKatalogCount);
    LCD_ShowString(strAndStrC->str, goStrCount, tree_ZnakKatalogCount, GetScreenRowIndex);
  } // if ((!GetIsTreeNode_Displayed) & (!isCurrentTreeNode_FullScreen))
#ifdef use_GoString
  else
    LCD_ShowGoString(strAndStrC->str, strAndStrC->strCnt, tree_ZnakKatalogCount, GetScreenRowIndex);
#endif
  dSetIsTreeNode_DisplayedON;
}



////////////////////////////////////////////////////////////////////////////// 
// ������������ ������ ������������ ���������� [�������/������].
////////////////////////////////////////////////////////////////////////////// 

  // ������� ������� TreeNode �� �������
void Tree_ShowVisibleTreeNodes() 
  {
    // ���� ���������� ������, ������� ������������ �������� (NULL==currentTreeNode)
    // if (NULL==currentTreeNode) { LCD_ShowStartText; return; }
    dSetIsTreeNode_DisplayedOFF;
    dResetIndexShowGoString;
    if (isCurrentTreeNode_FullScreen)
    {          
#ifdef verify_FunctionShowInfo_NULL
        if (NULL == *(currentTreeNode->pFunctionShowInfo))
          Tree_ShowNULLkatalog();
        else
#endif
          currentTreeNode->pFunctionShowInfo();
    }
    else
    {
      // ���������� ���, ��� 2-�������� �������
      dResetScreenRowIndex;
      dSetIsShowKatStrelka( GetTreeNodeActiveIndex == GetTreeNodeOfFirstRowIndex );      
      const t_VLtreeNode* treenode_Save = currentTreeNode; 
      currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeOfFirstRowIndex];      
#ifdef verify_FunctionShowInfo_NULL
      if (NULL == *(currentTreeNode->pFunctionShowInfo))
        Tree_ShowNULLkatalog();
      else
#endif
        currentTreeNode->pFunctionShowInfo();
      currentTreeNode = treenode_Save;
      if (GetTreeNodeOfFirstRowIndex < (currentTreeNode->pParent->childsTreeNodeCount-1))
      {
        dIncScreenRowIndex;
        dReverseIsShowKatStrelka;
        dSetIsTreeNode_DisplayedOFF;
        const t_VLtreeNode* treenode_Save = currentTreeNode;
        currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeOfFirstRowIndex+1];
#ifdef verify_FunctionShowInfo_NULL
        if (NULL == *(currentTreeNode->pFunctionShowInfo))
          Tree_ShowNULLkatalog();
        else
#endif
          currentTreeNode->pFunctionShowInfo();
        currentTreeNode = treenode_Save;
      }
      else
        LCD_ClearCharsToEndFromRow1(0);  
      // ��� ������� ������
      dSetScreenRowIndex( GetTreeNodeActiveIndex != GetTreeNodeOfFirstRowIndex );
    }
  }

#endif    // #ifdef use_IndRUS1