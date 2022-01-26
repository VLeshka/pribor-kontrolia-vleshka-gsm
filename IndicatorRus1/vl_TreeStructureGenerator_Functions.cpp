
// ������������ ��� ������ TreeNode
// ���������� ���������� TreeStructureGenerator

#define vl_TreeStructureGenerator_Functions

#include "msp430G2553.h"

#include "rus1_screenLCD_AC-162A.hpp"
#include "vl_TreeStructureGenerator_Functions.hpp"
#include "vl_TreeStructureGenerator_Structure.hpp"
#include "rus1.hpp"
#include "rus1_treenode_go.hpp"
#include "example_base_strings.hpp"

#include "peripherals.hpp" // ������� ������� ������ �� ������ � ������������ ������ ���-1
#include "gsm.hpp"


//////////////////////////////////////////////////////// 
// ������������ ��� ������ � ������� TreeNode
/////////////////////////////////////////////////

#ifdef use_IndRUS1
////////////////// 
// �������� �������
// ���������� ����������� ������, ���� ��� ������������. ��� � ���������� ��-������� ���������� ������ �� 
// �������� "����������".
// � �������� "����������" ������ treenode ���������� ����������� ����������� ����������, ������� � �������
// treenode ���� ������������ ����������� ����������. � ���� ��, ��� ����������� ��-������� ������ �� 
// �������� "����������", ���������, ����� ������ treenode ��� �� ���� ���� �������������� ������������.
#define delayShow_Root 133
#define indexShow_RootMax (delayShow_Root*treeNode_Informatsiya.childsTreeNodeCount)
unsigned int indexShow_Root = 0; // ������� GetIndexShowGoString, �.�. GetIndexShowGoString ������������ � �������� ��������
unsigned int indexRoot_old = 0xFFFF; // ����� ��� ���������� �������� GetIndexShowGoString ��� �������� ��������, ��� ����������� ������
void Show_Root()
{
  // ���� ���� ����������� ������ ���������, ���������� ���
  if (isWasTypeSpecMessage_CriticalError)
  {
    ShowCriticalMessage();
    return;
  }
  // ���� ���������� �� ����������� ��������� divmod16u, ���� ��������� index
  unsigned char indexRoot = 0;
  unsigned int i_temp = indexShow_Root;
  while (i_temp > delayShow_Root)
  { indexRoot++; i_temp -= delayShow_Root; }
  if (indexRoot_old != indexRoot)
    dResetIndexShowGoString;
  treeNode_Informatsiya.pChildsTreeNode[indexRoot]->pChildsTreeNode[0]->pFunctionShowInfo();
  indexRoot_old = indexRoot;
  indexShow_Root++; 
  if (indexShow_Root > indexShow_RootMax) 
    indexShow_Root = 0;
}
#else
void Show_Root()
{
  Show_data_Napryazhenie_fazy();
}
#endif

////////////////// 
// ����� ����������

// TreeNode data ���������� ����, ��������� ����
#define lcdTextVal220AfterValCnt 16
const unsigned char lcdTextVal220AfterVal_AutoOff[lcdTextVal220AfterValCnt] = 
  //   0   1   2   3   4   5   6   7   8   9   10  11  12            15              19
    {' ',' ',' ','�',',',' ','�','�','�','.',' ','�','�','�','�','.'};
const unsigned char lcdTextVal220AfterVal_ManualOff[lcdTextVal220AfterValCnt] = 
    {' ',' ',' ','�',',',' ','�','�','�','�',' ','�','�','�','�','.'};
const unsigned char lcdTextVal220AfterVal_On[lcdTextVal220AfterValCnt] = 
    {' ',' ',' ','�',',',' ','�','�','�','�','�','�','�','�',' ',' '};
void Show_data_Napryazhenie_rele( void )
{
  // ������ ������
  LCD_ShowString(&lineNapryazhenie_rele[0], lineNapryazhenie_releCnt, 0, 0);
  // ������ ������ // � �������� ������� ���������� ������������ ������� ������
  if (!GetIndexShowGoString)
  {    
    const unsigned char* dataVal;
    if (!isRelePower220_On)
    {
      if (is220commandManualOff)
        dataVal = lcdTextVal220AfterVal_ManualOff;
      else
        dataVal = lcdTextVal220AfterVal_AutoOff;
    }
    else
      dataVal = lcdTextVal220AfterVal_On;
    LCD_ShowString(dataVal, lcdTextVal220AfterValCnt, 0, 1);
    LCD_ShowLong(GetVal220(), 0, 1);
  }
  dIncIndexShowGoString(20);
}

// TreeNode data ������ ���-�����
void Show_data_Balans_sim_karty( void )
{
  // ������� ��������� TreeNode
  LCD_ShowGoString(lineBalans_sim_karty, lineBalans_sim_kartyCnt,0);
  // ������� ������
  if (isWasTypeSpecMessage_CriticalError)
    if (line_ModemPbReady_Error == errorScreen.word)
    {
      LCD_ShowString(line_ModemNoGsmConnect, lcdColCount, 0, 1);
      return;
    }
  if (!isSetupOnStartUp)
    LCD_ShowString(lineSetupModem, lcdColCount, 0, 1);
  else
  {
    LCD_ShowString(line_Probel, lcdColCount, 0, 1);
    LCD_ShowString(balans, balansCount, 3, 1);    
  }
}

// TreeNode data ������� GSM-�������
#define lineDataUroven__GSM_signalaCnt 16
  const unsigned char lineDataUroven__GSM_signala[lineDataUroven__GSM_signalaCnt] = 
//   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18
  {'�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�',':'};
#define lineUroven__GSM_signala_levelCnt 9
//   const unsigned char lineUroven__GSM_signala_level[lineUroven__GSM_signala_levelCnt] = 
//   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18
//   {'%',',',' ','�','�','�','.',':',' '};
void Show_data_Uroven__GSM_signala( void )
{
  // ������� ��������� TreeNode
  LCD_ShowGoString(lineDataUroven__GSM_signala, lineDataUroven__GSM_signalaCnt,0);
  // ������� ������
  LCD_ShowString(line_Probel, lcdColCount, 0, 1);  
  unsigned char indexX = 5;
  LCD_ShowLong(gsmLevel.generalLevel, indexX, 1);
  indexX +=  GetLengthOfSignedLong(gsmLevel.generalLevel);
  GetCharInRow1[indexX] = strPersent[0];
}

// TreeNode data ���� � GSM-���������
#define lineDataInfo_o_GSM_operatoreCnt 16
const unsigned char lineDataInfo_o_GSM_operatore[lineDataInfo_o_GSM_operatoreCnt] = 
//   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19
  {' ','G','S','M','-','�','�','�','�','�','�','�','�',':',' ',' '};
void Show_data_Info_o_GSM_operatore( void )
{
  // ������ ������
  LCD_ShowString(&lineDataInfo_o_GSM_operatore[0], lineDataInfo_o_GSM_operatoreCnt, 0, 0);
  // ������� ������
  if ((signed char)-1 == gsmOperatorUssd_Index)
    if (isWasTypeSpecMessage_CriticalError)
      if (line_ModemPbReady_Error == errorScreen.word)
      {
        LCD_ShowString(line_ModemNoGsmConnect, lcdColCount, 0, 1);
        return;
      }
  if (!isSetupOnStartUp)
    LCD_ShowString(lineSetupModem, lcdColCount, 0, 1);
  else
  {
    LCD_ShowString(line_Probel, lcdColCount, 0, 1);
    LCD_ShowString(gsmOperatorUssd[gsmOperatorUssd_Index].dataCOPS, gsmOperatorUssd[gsmOperatorUssd_Index].dataCOPS_Cnt, 2, 1);
  }
}




//////////////////////////////////////////// 
// ���������
//////////////////////////////////////////// 

// TreeNode ����� ���� ��������
#define lineDataMenuResetAll_OptionsCnt (lcdRowCount*lcdColCount)
const unsigned char lineDataMenuResetAll_Options[lineDataMenuResetAll_OptionsCnt] = 
//  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  
{ ' ',' ','�','�','�','�','�','�','�','�',' ','�','�','�',' ',' ',
  ' ',' ',' ','�','�','�','�','�','�','�','�','�','?',' ',' ',' ' };
void Show_Reset_All_Options( void )
{
    // ����� ��� "������ � ������", ������� �������:
    //  - ���������� "���������" �����.
    //  - ������ dSetSpecRezhim.
    //  - ������� ������� � ����
    switch (GetRRP)
    {
    case rrp_0: //  ��� ������������ ������ (������� ������)
      // GetLnChangeMax ����� �� ������������ ��� �������, ������� ���������� ��� ��� ����
      // ����� != 0xFF, ������ ���������� ��������������
      if (0xFF != GetLnChangeMax)
      {
        // ���������� GetLnChangeMax ��� ���� ��� ������� ������ ���������
        LCD_ShowString(lineDataMenuResetAll_Options, lineDataMenuResetAll_OptionsCnt, 0, 0);
        SetRRP(rrp_1);
      }
      else
      {
        Tree_OutUpTreeNode();
        dResetLnChangeMax;
      }
      return;
    case rrp_4: //  ����� ������ ��������� �� ������� ����������� �������� // ����� ���������!
      ResetAllData();
    case rrp_5: // ��������� �� ��������� ��������
      dSetLnChangeMax(0xFF);
      return;
    default:
      return;
    }             
}