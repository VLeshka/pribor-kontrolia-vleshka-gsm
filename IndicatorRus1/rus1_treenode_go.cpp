
// модуль функций "хождения" по дереву TreeNode

#define rus1_treenode_go

#include "msp430G2553.h"

#include "rus1_screenLCD_AC-162A.hpp"
#include "rus1.hpp"
#include "vl_TreeStructureGenerator_Structure.hpp"



///////////////////////
// описания подпрограмм
///////////////////////


  // входит в дочерний TreeNode
void Tree_EnterChildTreeNode()
  {
    // если изменяется логика, следует использовать проверку (NULL==currentTreeNode)
    // if (NULL==currentTreeNode) return;
    if (!currentTreeNode->childsTreeNodeCount) return;
    dResetIndexShowGoString;
    dResetTreeNodeOfFirstRowIndex;
    dResetTreeNodeActiveIndex;
    currentTreeNode = currentTreeNode->pChildsTreeNode[0];    
    Tree_ShowVisibleTreeNodes();
  }

  // выходит в родительский TreeNode
void Tree_OutUpTreeNode()
  {
    // если изменяется логика, следует использовать проверку (NULL==currentTreeNode)
    // if (NULL==currentTreeNode) return;
    // проверка не в корень ли зашли
    if (NULL == currentTreeNode->pParent) 
      return;
    dResetIndexShowGoString;
    currentTreeNode = currentTreeNode->pParent;
    // находим индекс активного TreeNode и индекс TreeNode для первой строки экрана
    if (NULL != currentTreeNode->pParent) 
    {
      // вычисляем активный индекс TreeNode первой строки экрана
      // индекс активного TreeNode
      dResetTreeNodeActiveIndex;
      const t_VLtreeNode *const *sbm_end = currentTreeNode->pParent->pChildsTreeNode + currentTreeNode->pParent->childsTreeNodeCount;
      for (const t_VLtreeNode *const *sbm = currentTreeNode->pParent->pChildsTreeNode; sbm < sbm_end; sbm++, /*dIncTreeNodeActiveIndex*/vlTree.f_TreeNodeActiveIndex++ )
        if (*sbm == currentTreeNode)
        {
          dSetTreeNodeOfFirstRowIndex( GetTreeNodeActiveIndex );
          if ( GetTreeNodeActiveIndex == (currentTreeNode->pParent->childsTreeNodeCount-1) )
            if (currentTreeNode->pParent->childsTreeNodeCount > 1)
              dDecTreeNodeOfFirstRowIndex;
          break;
        }
    }
    Tree_ShowVisibleTreeNodes();
  }

  // переходит в следующий по уровню TreeNode
void Tree_NextTreeNode() 
  {
    // если изменяется логика, следует использовать проверку (NULL==currentTreeNode)
    // if (NULL==currentTreeNode) return;
    if (NULL == currentTreeNode->pParent) return;
    if (isCurrentTreeNode_FullScreen)
    {
      if (GetTreeNodeActiveIndex < (currentTreeNode->pParent->childsTreeNodeCount-1))
      {
        dIncScreenRowIndex;
        dIncTreeNodeActiveIndex;
        currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeActiveIndex];
        Tree_ShowVisibleTreeNodes();
      }
    }
    else
    if (GetTreeNodeActiveIndex < (currentTreeNode->pParent->childsTreeNodeCount-1))
    // упрощённый код, под 2-строчный дисплей
      {
        dResetIndexShowGoString;             
// если используем "бегущую строку", то "возвращаем" первоначальное название TreeNode на место
#ifdef use_GoString
        // 1. Определяем GetTreeNodeOfFirstRowIndex
        // 2. Определяем GetTreeNodeActiveIndex
        // 3. Выводим currentTreeNode от GetTreeNodeOfFirstRowIndex и GetTreeNodeActiveIndex
        if (GetTreeNodeActiveIndex != GetTreeNodeOfFirstRowIndex)
          dIncTreeNodeOfFirstRowIndex;
        dIncTreeNodeActiveIndex;
        // GetTreeNodeOfFirstRowIndex
        dSetIsTreeNode_DisplayedOFF;
        dSetIsShowKatStrelka_OFF;
        dResetScreenRowIndex;
        currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeOfFirstRowIndex];
#ifdef verify_FunctionShowInfo_NULL
        if (NULL == *(currentTreeNode->pFunctionShowInfo))
          Tree_ShowNULLkatalog();
        else
#endif
          currentTreeNode->pFunctionShowInfo();
        // GetTreeNodeActiveIndex
        dSetIsTreeNode_DisplayedOFF;
        dSetIsShowKatStrelka_ON;
        dIncScreenRowIndex;
        currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeActiveIndex];
#ifdef verify_FunctionShowInfo_NULL
        if (NULL == *(currentTreeNode->pFunctionShowInfo))
          Tree_ShowNULLkatalog();
        else
#endif
          currentTreeNode->pFunctionShowInfo();
#else
        dSetIsShowKatStrelka_ON;
        if (GetTreeNodeActiveIndex == GetTreeNodeOfFirstRowIndex)
        {
          // просто перемещаем курсор
          GetCharInRow1[0] = GetCharInRow0[0];
          GetCharInRow0[0] = ' ';
          dIncTreeNodeActiveIndex;
          dIncScreenRowIndex;
          currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeActiveIndex];   
        }
        else
        {
          // иначе перерисовываем строки
          dKatalogCopyRow1toRow0;
          dIncTreeNodeActiveIndex;
          dSetIsTreeNode_DisplayedOFF;
          currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeActiveIndex];
          if (currentTreeNode->pFunctionShowInfo != NULL)
            currentTreeNode->pFunctionShowInfo();
          else
            Tree_ShowNULLkatalog();
          dIncTreeNodeOfFirstRowIndex;
        }         
#endif
      } // if (GetTreeNodeActiveIndex < (currentTreeNode->pParent->childsTreeNodeCount-1))
  }

  // переходит в предыдующий по уровню TreeNode
void Tree_PreviousTreeNode()
  {
    // если изменяется логика, следует использовать проверку (NULL==currentTreeNode)
    // if (NULL==currentTreeNode) return;
    if (NULL == currentTreeNode->pParent) return;
    if (isCurrentTreeNode_FullScreen) 
    {
      if (GetTreeNodeActiveIndex > 0)
      {
        dDecTreeNodeOfFirstRowIndex;
        dDecTreeNodeActiveIndex;
        currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeActiveIndex];
        Tree_ShowVisibleTreeNodes();
      }
    }
    else // if (isCurrentTreeNode_FullScreen) 
    if (GetTreeNodeActiveIndex > 0)
       // упрощённый код, под 2-строчный дисплей
      {
        dResetIndexShowGoString;
// если используем "бегущую строку", то "возвращаем" первоначальное название TreeNode на место
#ifdef use_GoString
        dSetIsShowKatStrelka_OFF;
        dSetScreenRowIndex( 1 );
        dSetIsTreeNode_DisplayedOFF
#ifdef verify_FunctionShowInfo_NULL
        if (NULL == *(currentTreeNode->pFunctionShowInfo))
          Tree_ShowNULLkatalog();
        else
#endif
          currentTreeNode->pFunctionShowInfo();
        dSetIsShowKatStrelka_ON;
        dDecScreenRowIndex;
        dDecTreeNodeOfFirstRowIndex;
        dDecTreeNodeActiveIndex;
        dSetIsTreeNode_DisplayedOFF
        currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeActiveIndex];
#ifdef verify_FunctionShowInfo_NULL
        if (NULL == *(currentTreeNode->pFunctionShowInfo))
          Tree_ShowNULLkatalog();
        else
#endif
          currentTreeNode->pFunctionShowInfo();
        // для бегущей строки
        dResetScreenRowIndex;
#else        
        if (GetTreeNodeActiveIndex != GetTreeNodeOfFirstRowIndex)
        {
          // просто перемещаем курсор
          GetCharInRow0[0] = GetCharInRow1[0];
          GetCharInRow1[0] = ' ';
          dDecTreeNodeActiveIndex;
          dDecScreenRowIndex;
          currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeActiveIndex];
        }
        else
        {
          // иначе перерисовываем строки
          dKatalogCopyRow0toRow1;
          dDecTreeNodeActiveIndex;
          dDecTreeNodeOfFirstRowIndex;
          dSetIsTreeNode_DisplayedOFF;
          currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeActiveIndex];
          if (currentTreeNode->pFunctionShowInfo != NULL)
            currentTreeNode->pFunctionShowInfo();
          else
            Tree_ShowNULLkatalog();
        }        
#endif
      } // if (GetTreeNodeActiveIndex > 0)
  }