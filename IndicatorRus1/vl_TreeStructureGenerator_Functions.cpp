
// подпрограммы для дерева TreeNode
// составлено программой TreeStructureGenerator

#define vl_TreeStructureGenerator_Functions

#include "msp430G2553.h"

#include "rus1_screenLCD_AC-162A.hpp"
#include "vl_TreeStructureGenerator_Functions.hpp"
#include "vl_TreeStructureGenerator_Structure.hpp"
#include "rus1.hpp"
#include "rus1_treenode_go.hpp"
#include "example_base_strings.hpp"

#include "peripherals.hpp" // главный рабочий модуль по работе с индикаторной платой РУС-1
#include "gsm.hpp"


//////////////////////////////////////////////////////// 
// подпрограммы для работы с деревом TreeNode
/////////////////////////////////////////////////

#ifdef use_IndRUS1
////////////////// 
// Корневой каталог
// Отображает критическую ошибку, если она присутствует. При её отсутствии по-очереди отображает данные из 
// каталога "Информация".
// В каталоге "Информация" каждый treenode отображает динамически изменящуюся информацию, поэтому у каждого
// treenode своя подпрограмма отображения информации. К тому же, для отображения по-очереди данных из 
// каталога "Информация", требуется, чтобы каждый treenode так же имел свою индивидуальную подпрограмму.
#define delayShow_Root 133
#define indexShow_RootMax (delayShow_Root*treeNode_Informatsiya.childsTreeNodeCount)
unsigned int indexShow_Root = 0; // заместо GetIndexShowGoString, т.к. GetIndexShowGoString используется в дочерних тринодах
unsigned int indexRoot_old = 0xFFFF; // чтобы при надобности обнулять GetIndexShowGoString для дочерних тринодов, для корректного показа
void Show_Root()
{
  // если было критическое плохое сообщение, показываем его
  if (isWasTypeSpecMessage_CriticalError)
  {
    ShowCriticalMessage();
    return;
  }
  // чтоб компилятор не использовал медленный divmod16u, сами вычисляем index
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
// Ветка Информация

// TreeNode data Напряжение реле, состояние реле
#define lcdTextVal220AfterValCnt 16
const unsigned char lcdTextVal220AfterVal_AutoOff[lcdTextVal220AfterValCnt] = 
  //   0   1   2   3   4   5   6   7   8   9   10  11  12            15              19
    {' ',' ',' ','В',',',' ','а','в','т','.',' ','в','ы','к','л','.'};
const unsigned char lcdTextVal220AfterVal_ManualOff[lcdTextVal220AfterValCnt] = 
    {' ',' ',' ','В',',',' ','в','р','у','ч',' ','в','ы','к','л','.'};
const unsigned char lcdTextVal220AfterVal_On[lcdTextVal220AfterValCnt] = 
    {' ',' ',' ','В',',',' ','в','к','л','ю','ч','е','н','о',' ',' '};
void Show_data_Napryazhenie_rele( void )
{
  // первая строка
  LCD_ShowString(&lineNapryazhenie_rele[0], lineNapryazhenie_releCnt, 0, 0);
  // вторая строка // в качестве костыля используем коэффициенты бегущей строки
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

// TreeNode data Баланс сим-карты
void Show_data_Balans_sim_karty( void )
{
  // выводим заголовок TreeNode
  LCD_ShowGoString(lineBalans_sim_karty, lineBalans_sim_kartyCnt,0);
  // выводим данные
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

// TreeNode data Уровень GSM-сигнала
#define lineDataUroven__GSM_signalaCnt 16
  const unsigned char lineDataUroven__GSM_signala[lineDataUroven__GSM_signalaCnt] = 
//   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18
  {'У','р','о','в','е','н','ь',' ','с','и','г','н','а','л','а',':'};
#define lineUroven__GSM_signala_levelCnt 9
//   const unsigned char lineUroven__GSM_signala_level[lineUroven__GSM_signala_levelCnt] = 
//   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18
//   {'%',',',' ','к','а','ч','.',':',' '};
void Show_data_Uroven__GSM_signala( void )
{
  // выводим заголовок TreeNode
  LCD_ShowGoString(lineDataUroven__GSM_signala, lineDataUroven__GSM_signalaCnt,0);
  // выводим данные
  LCD_ShowString(line_Probel, lcdColCount, 0, 1);  
  unsigned char indexX = 5;
  LCD_ShowLong(gsmLevel.generalLevel, indexX, 1);
  indexX +=  GetLengthOfSignedLong(gsmLevel.generalLevel);
  GetCharInRow1[indexX] = strPersent[0];
}

// TreeNode data Инфо о GSM-операторе
#define lineDataInfo_o_GSM_operatoreCnt 16
const unsigned char lineDataInfo_o_GSM_operatore[lineDataInfo_o_GSM_operatoreCnt] = 
//   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19
  {' ','G','S','M','-','о','п','е','р','а','т','о','р',':',' ',' '};
void Show_data_Info_o_GSM_operatore( void )
{
  // первая строка
  LCD_ShowString(&lineDataInfo_o_GSM_operatore[0], lineDataInfo_o_GSM_operatoreCnt, 0, 0);
  // выводим данные
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
// Настройки
//////////////////////////////////////////// 

// TreeNode Сброс всех настроек
#define lineDataMenuResetAll_OptionsCnt (lcdRowCount*lcdColCount)
const unsigned char lineDataMenuResetAll_Options[lineDataMenuResetAll_OptionsCnt] = 
//  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  
{ ' ',' ','С','б','р','о','с','и','т','ь',' ','в','с','е',' ',' ',
  ' ',' ',' ','н','а','с','т','р','о','й','к','и','?',' ',' ',' ' };
void Show_Reset_All_Options( void )
{
    // здесь без "захода в ДАННЫЕ", поэтому вручную:
    //  - используем "сторонние" флаги.
    //  - ставим dSetSpecRezhim.
    //  - выходим обратно в МЕНЮ
    switch (GetRRP)
    {
    case rrp_0: //  нет специального режима (обычная работа)
      // GetLnChangeMax здесь не используется как таковое, поэтому используем его как флаг
      // когда != 0xFF, значит происходит редактирование
      if (0xFF != GetLnChangeMax)
      {
        // используем GetLnChangeMax как флаг для ручного вывода сообщения
        LCD_ShowString(lineDataMenuResetAll_Options, lineDataMenuResetAll_OptionsCnt, 0, 0);
        SetRRP(rrp_1);
      }
      else
      {
        Tree_OutUpTreeNode();
        dResetLnChangeMax;
      }
      return;
    case rrp_4: //  после вывода сообщения об успешно выполненной операции // висит сообщение!
      ResetAllData();
    case rrp_5: // сообщение об отменённой операции
      dSetLnChangeMax(0xFF);
      return;
    default:
      return;
    }             
}