
// .h-файл главного рабочего модуля по работе с индикаторной платой РУС-1

#ifndef __rus1__
#define __rus1__

#include "rus1_mainTitle.hpp"

// проверять ли (NULL == *(currentTreeNode->pFunctionShowInfo)
// #define verify_FunctionShowInfo_NULL


// если не используем бегущие строки, LCD_ShowGoString использовать - лишнее
#ifndef use_GoString
  #define LCD_ShowGoString LCD_ShowString
#endif

//////////////////////////////////// 
// при использовании индикатора РУС-1 
//////////////////////////////////// 

// режим редактирования переменной (ррп)
#define rrp_0 0 //  нет режима редактирования переменной (обычная работа)
#define rrp_1 1 //  редактирование переменной (и пока не произошло изменения)
#define rrp_2 2 //  редактирование переменной (изменение "вверх")
#define rrp_3 3 //  редактирование переменной (изменение "вниз")
#define rrp_4 4 //  после вывода сообщения об успешно выполненной операции // висит сообщение!
#define rrp_5 5 //  после вывода сообщения об отменённой операции

// адрес в кодовой таблице символа курсора каталога
#define cursorKatalog 0x3E // 62

// адрес в кодовой таблице символа возможности изменения переменной
// выводится в конце строки, показывающей значение переменной
#define cursorEnter 133 // 0x7E // 126

// адрес в кодовой таблице символа указателя каталога, который содержит данные
// #define cursorLastKatalog 0xFF // 255
#define cursorLastKatalog 187 // 0xDC // 220



// количество символов, приходящихся на знак указания активного каталога
#define tree_ZnakKatalogCount ((unsigned char)(1))
// количество символов в каждом названии каталога, умещающихся на экран
#define tree_NameKatalogCount ((unsigned char)(lcdColCount-tree_ZnakKatalogCount))

// максимальный буфер команд, отличных от команд позиционирования курсора
// для vlTree.f_commandsCount
#define commandsCountMax 4


  
//////////////////////////////// 
// настройки для бегущей строки
//////////////////////////////// 

// до того, как строка начнёт двигаться
// количество вызовов LCD_ShowGoString
#define pauseCntBeforeGoString 30
// после того, как строка "приедет" до конца
// количество вызовов LCD_ShowGoString
#define pauseCntAfterGoString 30

// индекс замедления движения бегущей строки
// каждое GoStringSdvigUndex событие обновляем строку
//  = 2 ^ goStringSdvigUndex
// сдвиг на 1 = умножение на 2
#define goStringSdvigUndex 1




////////////////////////////// 
// для упрощения вызова функций
////////////////////////////// 

// показывает NULL-каталог
// #ifdef verify_FunctionShowInfo_NULL
  #define Tree_ShowNULLkatalog() \
  LCD_ShowString( &tree_ShowNULLkatalogString[tree_ZnakKatalogCount], tree_NameKatalogCount, tree_ZnakKatalogCount, GetScreenRowIndex )
// #endif

// переходит в корневой каталог
#define Tree_JumpToRootKatalog() { Rezhim6_On(Show_Root); currentTreeNode = &treeNode_Root; }

// удаляет все символы в строке 0 (нумерация с нуля), со столбца col до конца строки
#define LCD_ClearCharsToEndFromRow0(col) LCD_ShowString(line_Probel, lcdColCount-col, col, 0)
// удаляет все символы в строке 1 (нумерация с нуля), со столбца col до конца строки
#define LCD_ClearCharsToEndFromRow1(col) LCD_ShowString(line_Probel, lcdColCount-col, col, 1)

// удаляет все символы в строке 0 (нумерация с нуля), с начала до столбца col (нумерация с нуля, включительно)
#define LCD_ClearCharsFromBeginFromRow0(col) \
   LCD_ShowString(line_Probel, col, 0, 0)
// удаляет все символы в строке 1 (нумерация с нуля), с начала до столбца col (нумерация с нуля, включительно)
#define LCD_ClearCharsFromBeginFromRow1(col) \
   LCD_ShowString(line_Probel, col, 0, 1)
     
     
     
// currentTreeNode
/////////////////

// выводится ли TreeNode во весь экран
#define isCurrentTreeNode_FullScreen ( (0 == currentTreeNode->childsTreeNodeCount) | isCurrentTreeNode_Root )
// является ли TreeNode корневым
#define isCurrentTreeNode_Root (currentTreeNode == &treeNode_Root)
// позволяет ли активный узел дерева изменять данные
// #define isCurrentTreeNode_ChangableData (BIT0 & currentTreeNode->flag)
#define isCurrentTreeNode_ChangableData ( 0!= (BIT0 & currentTreeNode->flag) )


// режим спецсообщения?
#define isRezhimSpecMessage ( NULL != vlTree.f_strSpecMessage )
// отсчёт времени показа спецсообщения?
#define isRezhimSpecMessage_Time100msShow ( 0 != vlTree.f_100msSpecMessageCnt )

// есть ли что в буфере команды дисплея
#define lcd_IsCommandON (0 != GetCommandsCount)

// надо ли применять изменённые данные
// при окончании редактирования переменной через дисплей
#define isNeedSaveData ( (0 != GetDeltaChange) & (rrp_4 == GetRRP) )





// буфер команды для дисплея [отличной от команды позиционирования курсора]
// выполнение слдедующей команды
// команды заполняются строго по-очереди
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
// сброс очереди команд
#define dLCD_CommandClear { vlTree.f_commandsCount = 0; }


#ifdef use_IndRUS1

// включение индикатора, курсор включен  
#define dLCD_SetCursorBlinkOn  { unsigned char c_tmp = 0x0F; LCD_CommandsAddToBuf(&c_tmp, 1); dSetIsCursorVisibleON; }
// включение индикатора, курсор выключен  
// #define dLCD_SetCursorBlinkOff { unsigned char c_tmp = 0x0C; LCD_CommandsAddToBuf(&c_tmp); dSetIsCursorVisibleOFF; }



// сбрасывает "флаг" надобности применения изменённых данных
// см. isNeedSaveData
#define dSetNeedSaveData_Off dResetDeltaChange

#endif // #ifdef use_IndRUS1


/////////////////////////////////////////
// для дефайнов для работы с типами данных
/////////////////////////////////////////


// struct t_VLtreeWork
///////////////////////


// f_rezhimSpecMessage
/////////////////////

// режим показа срочного сообщения
/////////////////////////////////
// показывает спецсообщение
#define dShowRezhimSpecMessage { LCD_ShowString(vlTree.f_strSpecMessage, lcdColCount*lcdRowCount,0,0); }
// устанавливает режим спецсообщения // если имя переменной начинается с f_ , значит работа с нею только через псевдоФункции
#define dSetSpecRezhimSpecMessage(x, y, z) { vlTree.f_strSpecMessage = x; \
                                             vlTree.f_100msSpecMessageCnt = y; \
                                             dSetTypeSpecMessage(z); }
// декремент vlTree.f_rezhimSpecMessageShow100msCnt
#define dDecRezhimSpecMessageTime100msCnt { vlTree.f_100msSpecMessageCnt--; }
// сброс счётчика показа сообщения
#define dResetRezhimSpecMessageTime100msCnt { vlTree.f_100msSpecMessageCnt = 1; } // 0 делать опасно - перед проверкой делается dDecRezhimSpecMessageTime100msCnt
// обновляет vlTree.f_100msSpecMessageCnt
#define SetRezhimSpecMessage_Show100ms( x ) { vlTree.f_100msSpecMessageCnt = x; }


#ifdef use_IndRUS1

// f_flag
//////// 

// f_flag
  // BIT0, BIT1, BIT2 - режим редактирования переменной (ррп)
  // ррп нужен для "связки" кнопок с диалогом изменения настроек.
  // допустимые режимы:
  //  specRezhim_0 - нет редактирования переменной (обычная работа)
  //  specRezhim_1 - редактирование переменной (и пока не произошло изменения)
  //  specRezhim_2 - редактирование переменной (изменение "вверх")
  //  specRezhim_3 - редактирование переменной (изменение "вниз")
  //  specRezhim_4 - после вывода сообщения о успешно выполненном редактировании переменной
  //  specRezhim_5 - после вывода сообщения об отмене редактирования переменной
// получить ррп
#define GetRRP ( vlTree.f_flag & (BIT0 + BIT1 + BIT2) )
// установка ррп
// #define dSetRRP( x ) { vlTree.f_flag &= (~(BIT0 + BIT1 + BIT2)); vlTree.f_flag |= (x); \
//                        if (rrp_0 != x) { dResetCnt1s_ChangeOff; dLCD_CommandClear; } }

// f_flag
// BIT3 установлен, когда курсор дисплея "включён" (виден)
#define GetIsCursorVisible    ( 0 != (vlTree.f_flag & BIT3) )
#define dSetIsCursorVisibleON  { vlTree.f_flag |= BIT3; }
#define dSetIsCursorVisibleOFF { vlTree.f_flag &= (~BIT3); }

// f_flag
// BIT4 установлен, когда TreeNode отрисовался хотя бы 1 раз
#define GetIsTreeNode_Displayed    ( 0 != (vlTree.f_flag & BIT4) )
#define dSetIsTreeNode_DisplayedON  { vlTree.f_flag |= BIT4; }
#define dSetIsTreeNode_DisplayedOFF { vlTree.f_flag &= (~BIT4); }

// f_flag
// BIT5 установлен, когда ДОЛЖЕН показываться указатель (стрелка) на каталог в выводимой строке
#define GetIsShowKatStrelka ( 0 != (vlTree.f_flag & BIT5) )
#define dSetIsShowKatStrelka_ON { vlTree.f_flag |= BIT5; }
#define dSetIsShowKatStrelka_OFF { vlTree.f_flag &= (~BIT5); }
#define dSetIsShowKatStrelka( x ) { if (x) dSetIsShowKatStrelka_ON else dSetIsShowKatStrelka_OFF }
#define dReverseIsShowKatStrelka { if (GetIsShowKatStrelka) dSetIsShowKatStrelka_OFF else dSetIsShowKatStrelka_ON; }

// f_flag
// BIT6 когда команду позиционирования ВИДИМОГО курсора не надо выполнять, BIT6=false
//      флаг на принудительное выполнение команды позиционирования курсора
// f_CursorVisiblePosition_Need
#define GetIsCursorVisiblePosition_Need ( 0 != (vlTree.f_flag & BIT6) )
#define dSetIsCursorVisiblePosition_Need_ON { vlTree.f_flag |= BIT6; }
#define dSetIsCursorVisiblePosition_Need_OFF { vlTree.f_flag &= (~BIT6); }

// f_flag
// BIT7 надо ли переинициализировать дисплей
#define lcd_IsReInitLCD ( 0 != (vlTree.f_flag & BIT7) )
#define dSetIsReInitLCD_ON { vlTree.f_flag |= BIT7; }
#define dSetIsReInitLCD_OFF { vlTree.f_flag &= (~BIT7); }

// f_flag
// BIT6 | BIT7
#define lcd_IsReInitLCD_or_IsCursorVisiblePosition_Need ( 0 != (vlTree.f_flag & (BIT6 + BIT7)) )
         
// f_ScreenRowIndex // в какой строке LCD-панели выводится строка
#define GetScreenRowIndex vlTree.f_ScreenRowIndex
#define dSetScreenRowIndex( x ) { vlTree.f_ScreenRowIndex = (x); }
#define dResetScreenRowIndex { vlTree.f_ScreenRowIndex = 0; }
#define dIncScreenRowIndex { vlTree.f_ScreenRowIndex++; }
#define dDecScreenRowIndex { vlTree.f_ScreenRowIndex--; }

// f_TreeNodeActiveIndex // индекс активного TreeNode у родительского TreeNode
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
// #define dSetTypeSpecMessage_Critical { GetTypeSpecMessage = BIT7; dGreenLed_Off; } // красным светодиодом управляем потом
#define dSetTypeSpecMessage_Critical_onZero { GetTypeSpecMessage = BIT6; }
#define dSetTypeSpecMessage_NonCritical { GetTypeSpecMessage &= (~(BIT6|BIT7)); dRedLed_Off; } // убирает показ критического сообщения
#define isWasTypeSpecMessage_CriticalError (BIT7 & GetTypeSpecMessage) // было когда-либо критическое сообщение об ошибке
#define isSpecMessage_ChangeToCritical_onZero (BIT6 & GetTypeSpecMessage)

// f_commandsCount
#define GetCommandsCount vlTree.f_commandsCount

// f_commands[commandsCountMax]
#define GetNextCommand vlTree.f_commands[0]
#define dAddCommand(x) { vlTree.f_commands[vlTree.f_commandsCount] = x; vlTree.f_commandsCount++; }

// индекс для динамического изменения содержимого экрана
// в частности, для бегущей строки
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
// остальное

  // на случай pFunctionShowInfo == NULL
  //                                                                  0                   5       7           10                  15 
#define tree_ShowNULLkatalogStringCnt 16
#define dTree_ShowNULLdata { LCD_ShowString(tree_ShowNULLkatalogString, tree_ShowNULLkatalogStringCnt,0,0); \
                                 LCD_ShowString(tree_ShowNULLkatalogString, tree_ShowNULLkatalogStringCnt,0,1); }


// копирует имя каталога из строки 1 в строку 0 (нумерация с нуля)
#define dKatalogCopyRow1toRow0 { unsigned char* pRow1 = pLcdBufBegin+tree_ZnakKatalogCount + lcdColCount; \
            for (unsigned char* pRow0 = pLcdBufBegin+tree_ZnakKatalogCount; pRow0 < pLcdBufBegin + lcdColCount; pRow0++, pRow1++) \
              *pRow0 = *pRow1; }
// копирует имя каталога из строки 0 в строку 1 (нумерация с нуля)
#define dKatalogCopyRow0toRow1 { unsigned char* pRow1 = pLcdBufBegin+tree_ZnakKatalogCount + lcdColCount; \
            for (unsigned char* pRow0 = pLcdBufBegin+tree_ZnakKatalogCount; pRow0 < pLcdBufBegin + lcdColCount; pRow0++, pRow1++) \
              *pRow1 = *pRow0; }


//////////////// 
// typedef-ы
//////////////// 

// тип функция
typedef void (*TFunPtr)( void ); //  передавать/возвращать значения не будем



////////////////////////////////// 
////////////////////////////////// 
// структуры,объекты типа структура
////////////////////////////////// 
////////////////////////////////// 

/////////////////////////
// для размещения в ОЗУ
///////////////////////////////

// объект для работы с индикаторной платой РУС-1
struct t_VLtreeWork
{
#ifdef use_IndRUS1
  // переменные для функции вывода каталог/данные
  //////////////////////////////////////////////////////// 

  // общий флаг 
  ////////////// 
  // BIT0, BIT1, BIT2 - режим редактирования переменной (ррп)
  // ррп нужен для "связки" кнопок с:
  //  - сообщениями.
  //  - изменением настроек пользователем.
  // допустимые режимы:
  //  rrp_0 - нет редактирования переменной (обычная работа)
  //  rrp_1 - редактирование переменной (и пока не произошло изменения)
  //  rrp_2 - редактирование переменной (изменение "вверх")
  //  rrp_3 - редактирование переменной (изменение "вниз")
  //  rrp_4 - после вывода сообщения о успешно выполненном редактировании переменной
  //  rrp_5 - после вывода сообщения об отмене редактирования переменной
  // BIT3 установлен, когда курсор дисплея "включён" (виден)
  // BIT4 установлен, когда TreeNode отрисовался хотя бы 1 раз
  // BIT5 установлен, когда ДОЛЖЕН показываться указатель (стрелка) на каталог в выводимой строке
  // BIT6 когда команду позиционирования ВИДИМОГО курсора не надо выполнять, BIT6==false // флаг на принудительное выполнение команды позиционирования курсора
  // BIT7 требуется рестарт дисплея  
  unsigned char f_flag;
  // индекс в какую строку выводится строка // для функций вывода слов
  unsigned char f_ScreenRowIndex;
  
  // TreeNode - узел дерева каталог/данные
  // индекс активного TreeNode у родительского TreeNode
  signed char f_TreeNodeActiveIndex; 
  // индекс в массиве дочерних treeNode [у родительского TreeNode], который в первой строке дисплея
  signed char f_TreeNodeOfFirstRow;                   
  
  // изменение переменной [пользователем через дисплей]
  // в ОБЩЕМ случае, логарифм f_deltaChange по основанию 10 //  lg(f_deltaChange) = f_lev10
  // чтобы не быть привязанным к типу переменной, рассчитываем только разницу между старым и новым значением
  // эта разница всегда целочисленная, чтобы быстро работать //  + даёт гибкость предоставления информации на экране
  // отсчёт с нуля
  /////////////////////////////////////////////////////////
  unsigned char f_lev10; // lg(f_deltaChange) = f_lev10 
  // максимально допустимое значение f_lev10
  unsigned char f_lev10Max;
  
  // изменение переменной [пользователем через дисплей]
  /////////////////////////////////////////////////////////
  // разница между переменной и новой величиной, см. f_lnChange
  signed long f_deltaChange;
  // буфер отдельной команды позиционирования курсора [при изменении переменной через дисплей]
  // установка [базовой] координаты курсора
  unsigned char f_xBaseCursor;
  
#endif    // #ifdef use_IndRUS1
  
  // режим спецсообщения // НЕ КРИТИЧЕСКОГО СПЕЦСООБЩЕНИЯ!
  // заместо currentTreeNode->pFunctionShowInfo инфо показывает сообщение во весь экран 
  // может быть неожиданно при любом режиме редактирования
  // режим спецсообщения включён, когда f_strSpecMessage != NULL
  // режим спецсообщения выключён, когда f_strSpecMessage == NULL
  // длина не указывается, т.к. она всегда = (lcdRowCount*lcdColCount)
  const unsigned char* f_strSpecMessage;  
  // сколько 100мс показывать спецсообщение
  // если == 0, то до_нажатия_кнопки/бесконечно
  unsigned char f_100msSpecMessageCnt;
  // тип спецсообщения
  // 0 - "обычное"
  // BIT0 - "хорошее" не важное
  // BIT1 - "хорошее" очень важное
  // BIT2 - "плохое" не важное
  // BIT3 - "плохое" очень важное
  // если поставлен BIT5 - значит, после ожидания cnt1s_ChangeOff секунд, возвращаем режим спецсообщения
  // если поставлен BIT6 - значит, по истечении времени показа спецсообщения, поставить BIT7; // замену строки сообщения обеспечивает программист
  // если поставлен BIT7 - значит, "критическое" спецсообщение; // сообщение должно мигать
  unsigned char f_typeSpecMessage;
  
  // буфер отдельной спецкоманды,отличной от команды позиционирования курсора
  // очередь команд, отличных от команды позиционирования курсора
  unsigned char f_commandsCount;  
  unsigned char f_commands[commandsCountMax];
  
  // индекс для динамического изменения содержимого экрана
  // в частности, для бегущей строки
  unsigned int f_indexShowGoString;

};



//////////////////////// 
// для размещения во флеш
//////////////////////// 

// структура объекта узла дерева [treeNode]
// treeNode - узел дерева каталог/данные
#ifdef use_IndRUS1
struct t_VLtreeNode
{
// TreeNode состоит из:
//  - указателя на родителский TreeNode; == 0 только у root
//  - указателя на функцию вывода информации TreeNode на экран
//  - массива дочерних элементов типа t_VLtreeNode; порядок "сверху вниз"
//  - указатель на данные для функции вывода информации TreeNode на экран
//  - маска флагов. Начиная с младшего:
//    * BIT0 - позволяет ли treeNode изменять данные
//    * BIT1-BIT7 - резерв
  
  // указатель на родителя; когда == NULL, значит в корне
  const t_VLtreeNode* pParent;                
  
  // указатель на функцию вывода информации TreeNode на экран
  const TFunPtr pFunctionShowInfo;            
  
  // массив дочерних TreeNode
  const t_VLtreeNode* const *pChildsTreeNode; 
  
  // указатель на данные для функции вывода информации TreeNode на экран
  const void* pDataForChange;                 
  
  // размер массива дочерних TreeNode
  const unsigned char childsTreeNodeCount;    
  
  // маска флагов
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
// прототипы
/////////////////////////////////

// убирает показ критического сообщения
// void SetTypeSpecMessage_NonCritical();

// показ спец- и критического сообщения
void ShowCriticalMessage();

// Включение индикатора, курсор выключен  
void LCD_SetCursorBlinkOff();

#ifdef use_GoString
// показ бегущей строки во всю ширину дисплея в строке row, начиная с колонки colStart
// следует выводить строку длиной минимум tree_ZnakKatalogCount
// только одна ОДНОВРЕМЕННО бегущая строка на весь экран 
// иначе лучше двигать построчно вверх //  комфортней, когда дополнительную информацию переводить на следующую строку, а не бегущей строкой
//                     выводимая строка                                      с какой колонки         в какой строке экрана
void LCD_ShowGoString(unsigned char const* goStr, unsigned char goStrCount, unsigned char colStart = 0, unsigned char rowScreen = 0);
#endif

// добавление команд дисплея в очередь
void LCD_CommandsAddToBuf( const unsigned char* cmds, unsigned char cmdsCnt = 1 );



/////////////////////////////
// РЕЖИМ СПЕЦСООБЩЕНИЯ
///////////////////////////

// устанавливает режим спецсообщения (режим показа срочного сообщения)
// заместо currentTreeNode->pFunctionShowInfo инфо показывает 
// функция, показывающая сообщение (об ошибке,заставка)
//                         функция       ск 100мс показыв сообщ-е       какие светодиоды включать: 
//                                       1 единица == 100мс;            BIT0 - красный 
//                                       0 - до нажатия кнопки          BIT1 - зелёный
// bool RezhimSpecMessage_On( TFunPtr fun, unsigned int show100msCnt = 0, unsigned char ledsOn = 0 );
void RezhimSpecMessage_On( const unsigned char* str, unsigned int show100msCnt, unsigned char ledsOn );

// выходит из режима спецсообщения, восстанавливает содержимое экрана
void RezhimSpecMessage_Off();



#ifdef use_IndRUS1

// установка ррп
void SetRRP( unsigned char x );

// показывает бегущую строку названия каталога
void LCD_ShowGoString_Katalog();



////////////////////////////////////////////////////////////////////////////// 
// подпрограммы вывода динамической информации [каталог/данные].
////////////////////////////////////////////////////////////////////////////// 

  // выводит видимые TreeNode на дисплей
void Tree_ShowVisibleTreeNodes();

#endif // #ifdef use_IndRUS1
#endif // #ifndef __rus1__