
////////////////////////////////////////////////////////////// 
// главный рабочий модуль по работе с индикаторной платой РУС-1
////////////////////////////////////////////////////////////// 

/*Состав модуля:
- includ-ы, согласно настройке rus1_mainTitle.h.
- "базовые" подпрограммы вывода фиксированной информации [каталог/данные].
- "базовые" подпрограммы вывода динамической информации [каталог/данные].
- подпрограммы "хождения" под дереву [каталог/данные].*/

////////// 
// includes
////////// 

#include "rus1_peripherals.hpp"       // модуль для работы с периферией
#include "example_base_functions.hpp"    // базовые функции, прототипы из языков высокого уровня
#include "rus1.hpp"
#include "rus1_screenLCD_AC-162A.hpp"      // модуль для работы с lcd
#include "example_base_strings.hpp"
    
#ifdef use_IndRUS1
  #include "rus1_stencil_funcs_changedata.hpp"
  #include "vl_TreeStructureGenerator_Structure.hpp"     // структура дерева TreeNode
  // указатель на активный TreeNode
  //////////////////////////// 
  const t_VLtreeNode* currentTreeNode = &treeNode_Root; 
  #include "rus1_stencil_funcs_changedata.hpp" // подпрограммы-трафареты вывода данных
  #include "rus1_indicatorRUS1.hpp"     // индикатор РУС1 (LCD-панель и 4 кнопками)
  #include "rus1_treenode_go.hpp"              // подпрограммы "хождения" по дереву TreeNode
#endif

#include "gsm.hpp"
#include "peripherals.hpp"

// объект для работы с индикаторной платой РУС-1
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
// подпрограммы
//////////////////////// 
/*
// убирает показ критического сообщения
void SetTypeSpecMessage_NonCritical() 
{ 
  GetTypeSpecMessage &= (~(BIT6|BIT7)); 
  dRedLed_Off; 
}
*/

// показ спец- и критического сообщения
void ShowCriticalMessage()
{
  if (!isWasTypeSpecMessage_CriticalError)
    dShowRezhimSpecMessage
  else
  {
  // используем GetIndexShowGoString
  // if (NULL != errorScreen.word) 
    if (10 > GetIndexShowGoString)
      LCD_ShowString(errorScreen.word, lcdColCount*lcdRowCount,0,0); // выводим сообщение о критическом событии
    else
      if (15 > GetIndexShowGoString)
        LCD_Clear();
    if (15 <= GetIndexShowGoString)
      GetIndexShowGoString = 0;
    else
      GetIndexShowGoString++;
  }
}

// Включение индикатора, курсор выключен  
void LCD_SetCursorBlinkOff() { unsigned char c_tmp = 0x0C; LCD_CommandsAddToBuf(&c_tmp); dSetIsCursorVisibleOFF; }

// добавление команд дисплея в очередь
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
// показ бегущей строки во всю ширину дисплея в строке row, начиная с колонки colStart
// следует выводить строку длиной минимум tree_ZnakKatalogCount
// только одна ОДНОВРЕМЕННО бегущая строка на весь экран 
// иначе лучше двигать построчно вверх //  комфортней, когда дополнительную информацию переводить на следующую строку, а не бегущей строкой
//                     выводимая строка                                      с какой колонки         в какой строке экрана
void LCD_ShowGoString(unsigned char const* str, unsigned char strCount, unsigned char colStart, unsigned char rowScreen)
{
  // сколько событий LCD_ShowGoString произойдёт для того, когда закончится движение строки
  // отталкиваемся не от длины строки, а от количества событий LCD_ShowGoString
  unsigned char outStrCnt = (lcdColCount-colStart);
  if (strCount < outStrCnt)
    LCD_ShowString(str, strCount, colStart, rowScreen);
  else
  {
    unsigned char timer_StrGo = ((strCount - outStrCnt) << goStringSdvigUndex);  
    // если надо в начальную позицию
    if (!GetIndexShowGoString) // если вызов не из LCD_ShowGoString_Katalog
      LCD_ShowString(str, outStrCnt,colStart,rowScreen);
    else
      if (GetIndexShowGoString>pauseCntBeforeGoString)
        if (GetIndexShowGoString<=(pauseCntBeforeGoString+timer_StrGo+goStringSdvigUndex))
          if ( 0 == (GetIndexShowGoString & ((unsigned int)(0xFFFF >> (16-goStringSdvigUndex))) ) ) // только для делителя = 2 (сдвиг на 1)!
          {
            unsigned char indexX = (unsigned char)(GetIndexShowGoString - pauseCntBeforeGoString) >> goStringSdvigUndex;
            LCD_ShowString(str+indexX, outStrCnt, colStart, rowScreen);
          }
    // меняем f_indexShowGoString, на следующий раз
    // нужно для мигания строки (в root)
    // и др. эффектов
    dIncIndexShowGoString(timer_StrGo+(pauseCntBeforeGoString+pauseCntAfterGoString));
  }    
}
#endif


/////////////////////////////////////
// РЕЖИМ СПЕЦСООБЩЕНИЯ
/////////////////////////////////////

// устанавливает режим спецсообщения (режим показа срочного сообщения)
// заместо currentTreeNode->pFunctionShowInfo инфо показывает 
// функция, показывающая сообщение (об ошибке,заставка)
//                         функция       ск 100мс показыв сообщ-е   какие светодиоды включать (если задан show100msCnt): 
// см. t_VLtreeWork
// bool RezhimSpecMessage_On( TFunPtr fun, unsigned int show100msCnt, unsigned char ledsOn )
void RezhimSpecMessage_On( const unsigned char* str, unsigned int show100msCnt, unsigned char typeMessage )
{
  /*if (str != NULL) для экономии места */
  if (vlTree.f_strSpecMessage == str)
    if (isTypeSpecMessage_ChangeOff)
      return;
  if (!isWasTypeSpecMessage_CriticalError)
  { 
#ifdef use_IndRUS1
    if (0 == GetRRP) // если редактируем переменную, то не мешаемся - просто помигаем светодиодами и всё
    {
      const unsigned char* s = NULL; // для экономии места
      if ( (!isCurrentTreeNode_Root) | (str != errorScreen.word) )
        s = str;
      dSetSpecRezhimSpecMessage(s, show100msCnt, typeMessage);        
      // dShowRezhimSpecMessage; для экономии места
    }
#endif
    // светодиоды
    // зелёный
    unsigned char usialType = (typeMessage & (~(BIT6 | BIT7)));    
    if ((BIT0 | BIT1) & usialType)
    {
      dGreenLedMust_100msOn(show100msCnt);    
      dGreenSignal_0; // сигнал светодиодом бесконечный
    }
    else
      dGreenLed_Off;
    // красный    
    if ((~(BIT0 | BIT1)) & usialType)
    {
      dRedLedMust_100msOn(show100msCnt);
      dRedSignal_0; // сигнал светодиодом бесконечный
    }
    else
      dRedLed_Off;
    // return(true);
  }
  // else return(false);
}

// выходит из режима спецсообщения, восстанавливает содержимое экрана
/*надо обеспечить, чтобы при редактировании сообщения не допускались*/
// выходит из режима спецсообщения
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

// установка ррп
void SetRRP( unsigned char x ) 
{ 
  vlTree.f_flag &= (~(BIT0 + BIT1 + BIT2)); 
  vlTree.f_flag |= (x);
  // if (rrp_0 != x) 
    // dResetCnt1s_ChangeOff;
}

// показывает бегущую строку названия каталога
void LCD_ShowGoString_Katalog() 
{
  t_strAndStrCnt* strAndStrC = (t_strAndStrCnt*)currentTreeNode->pDataForChange;
  // если TreeNode отображается в первый раз
  if ((!GetIsTreeNode_Displayed) & (!isCurrentTreeNode_FullScreen))
  { 
    // выводим знак указателя на каталог
    unsigned char* dataXCoord = pLcdBufBegin; 
    if (GetScreenRowIndex) dataXCoord += lcdColCount; 
    if (GetIsShowKatStrelka) 
    {
      //  - каталог, который содержит данные
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
// подпрограммы вывода динамической информации [каталог/данные].
////////////////////////////////////////////////////////////////////////////// 

  // выводит видимые TreeNode на дисплей
void Tree_ShowVisibleTreeNodes() 
  {
    // если изменяется логика, следует использовать проверку (NULL==currentTreeNode)
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
      // упрощённый код, под 2-строчный дисплей
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
      // для бегущей строки
      dSetScreenRowIndex( GetTreeNodeActiveIndex != GetTreeNodeOfFirstRowIndex );
    }
  }

#endif    // #ifdef use_IndRUS1