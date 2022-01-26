
// ������ ������� "��������" �� ������ TreeNode

#define rus1_treenode_go

#include "msp430G2553.h"

#include "rus1_screenLCD_AC-162A.hpp"
#include "rus1.hpp"
#include "vl_TreeStructureGenerator_Structure.hpp"



///////////////////////
// �������� �����������
///////////////////////


  // ������ � �������� TreeNode
void Tree_EnterChildTreeNode()
  {
    // ���� ���������� ������, ������� ������������ �������� (NULL==currentTreeNode)
    // if (NULL==currentTreeNode) return;
    if (!currentTreeNode->childsTreeNodeCount) return;
    dResetIndexShowGoString;
    dResetTreeNodeOfFirstRowIndex;
    dResetTreeNodeActiveIndex;
    currentTreeNode = currentTreeNode->pChildsTreeNode[0];    
    Tree_ShowVisibleTreeNodes();
  }

  // ������� � ������������ TreeNode
void Tree_OutUpTreeNode()
  {
    // ���� ���������� ������, ������� ������������ �������� (NULL==currentTreeNode)
    // if (NULL==currentTreeNode) return;
    // �������� �� � ������ �� �����
    if (NULL == currentTreeNode->pParent) 
      return;
    dResetIndexShowGoString;
    currentTreeNode = currentTreeNode->pParent;
    // ������� ������ ��������� TreeNode � ������ TreeNode ��� ������ ������ ������
    if (NULL != currentTreeNode->pParent) 
    {
      // ��������� �������� ������ TreeNode ������ ������ ������
      // ������ ��������� TreeNode
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

  // ��������� � ��������� �� ������ TreeNode
void Tree_NextTreeNode() 
  {
    // ���� ���������� ������, ������� ������������ �������� (NULL==currentTreeNode)
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
    // ���������� ���, ��� 2-�������� �������
      {
        dResetIndexShowGoString;             
// ���� ���������� "������� ������", �� "����������" �������������� �������� TreeNode �� �����
#ifdef use_GoString
        // 1. ���������� GetTreeNodeOfFirstRowIndex
        // 2. ���������� GetTreeNodeActiveIndex
        // 3. ������� currentTreeNode �� GetTreeNodeOfFirstRowIndex � GetTreeNodeActiveIndex
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
          // ������ ���������� ������
          GetCharInRow1[0] = GetCharInRow0[0];
          GetCharInRow0[0] = ' ';
          dIncTreeNodeActiveIndex;
          dIncScreenRowIndex;
          currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeActiveIndex];   
        }
        else
        {
          // ����� �������������� ������
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

  // ��������� � ����������� �� ������ TreeNode
void Tree_PreviousTreeNode()
  {
    // ���� ���������� ������, ������� ������������ �������� (NULL==currentTreeNode)
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
       // ���������� ���, ��� 2-�������� �������
      {
        dResetIndexShowGoString;
// ���� ���������� "������� ������", �� "����������" �������������� �������� TreeNode �� �����
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
        // ��� ������� ������
        dResetScreenRowIndex;
#else        
        if (GetTreeNodeActiveIndex != GetTreeNodeOfFirstRowIndex)
        {
          // ������ ���������� ������
          GetCharInRow0[0] = GetCharInRow1[0];
          GetCharInRow1[0] = ' ';
          dDecTreeNodeActiveIndex;
          dDecScreenRowIndex;
          currentTreeNode = currentTreeNode->pParent->pChildsTreeNode[GetTreeNodeActiveIndex];
        }
        else
        {
          // ����� �������������� ������
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