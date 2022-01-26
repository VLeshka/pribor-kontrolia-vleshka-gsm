// ��������� ������ TreeNode
// ���������� ���������� TreeStructureGenerator
// ������ �� ����� ����� "� ��������� TreeNode ������ ����"

#ifndef __vl_TreeStructureGenerator_Structure__
#define __vl_TreeStructureGenerator_Structure__

#include "msp430G2553.h"

#include "rus1_mainTitle.hpp"

#include "rus1.hpp"
#include "example_base_types.hpp"
#include "example_options.hpp"
#include "rus1_stencil_funcs_changedata.hpp"
#include "rus1_screenLCD_AC-162A.hpp"
#include "example_base_strings.hpp"


/////////////////////
// extern
///////////////

extern const unsigned long defaultData[defaultDataCnt];

//////////// 
// ������������

// TreeNode ����� ���� ��������
extern void Show_Reset_All_Options( void );

// ��������� �� ������������� ������������ �������� (1, 10, 100, ...), � ����������� ����� ������ �� ������.
// ������� �������� ����������� ���������� ��������� �������. ��������� ������� ���������� ������������� ������� 
// ������ "�����" � "����".
// � ���������� ������� ���������� ������� ����� GetLnChange, ����� �� ������������ ������� ������ �� �������.
extern void Data_ChangeWithCursor(); 

// ��������� �� ������� ���������� �������� (������), � ����������� ����� ������ �� ������.
// ��������� ��� �� ��������� ���������, � ����������� �� ������ ������������.
extern void Data_ChangeIndex_ArrayStr();

// ���������� ������� ������ �������� ��������
extern void LCD_ShowGoString_Katalog();

////////////////// 
// �������� �������

extern void Show_Root( void );

////////////////// 
// ����� ����������

// TreeNode ����������
extern void Show_Informatsiya( void );

// TreeNode data ���������� ����
extern void Show_data_Napryazhenie_rele( void );

// TreeNode data ������ ���-�����
extern void Show_data_Balans_sim_karty( void );

// TreeNode data ������� GSM-�������
extern void Show_data_Uroven__GSM_signala( void );

// TreeNode data ���� � GSM-���������
extern void Show_data_Info_o_GSM_operatore( void );

// ����������� �������/������:
//    - ������  - TreeNode, ������������ ��� ������ ����������� ������ �����; �� ����� �������� ������� �����.
//      �������� ������� ���������� ����� TreeNode � ����� ��������.
//    - ������� - TreeNode, ������������ �������� (�� ����� � ������, ����� root) ��� ������.


///////////////////////////////////////////////////
// � ������ �������� ������ ������������� �� flash
////////////////////////////////////////////////// 

/*
��������� ������:

�������
Root(0xE000)--->����������--->���������� ����----------------------------------------------->data ���������� ����
             |             |->������ ���-�����---------------------------------------------->data ������ ���-�����
             |             |->������� GSM-�������------------------------------------------->data ������� GSM-�������
             |             |->���� � GSM-���������------------------------------------------>data ���� � GSM-���������
             |->���������---->����� ����� ����. � ���. ���� ��� ������������---------------->data ����� ����� ����. � ���. ���� ��� ������������
                           |->����������� ���������� ���������� ��������-------------------->data ����������� ���������� ���������� ��������
                           |->���������� ���������� ���������� ��������--------------------->data ���������� ���������� ���������� ��������
                           |->�������� �� ��� ��� ������ ���������� �� ���������� �������--->data �������� �� ��� ��� ������ ���������� �� ���������� �������
                           |->�������� ����------------------------------------------------->data �������� ����
                           |->�������� ��� ���������---------------------------------------->data �������� ��� ���������
*/


///////////////////////////////////////////////////////
// ������ ��� ������� ������ �������� ��������
/////////////////////////////////////////////////////

#define addr_DataForShowGoString_Katalog_Informatsiya (addrDefaultOptions + sizeof(defaultData))
#define addr_DataForShowGoString_Katalog_Napryazhenie_rele (addr_DataForShowGoString_Katalog_Informatsiya + sizeof(t_strAndStrCnt))
#define addr_DataForShowGoString_Katalog_Balans_sim_karty (addr_DataForShowGoString_Katalog_Napryazhenie_rele + sizeof(t_strAndStrCnt))
#define addr_DataForShowGoString_Katalog_Uroven__GSM_signala (addr_DataForShowGoString_Katalog_Balans_sim_karty + sizeof(t_strAndStrCnt))
#define addr_DataForShowGoString_Katalog_Info_o_GSM_operatore (addr_DataForShowGoString_Katalog_Uroven__GSM_signala + sizeof(t_strAndStrCnt))
#define addr_DataForShowGoString_Katalog_Nastroyki (addr_DataForShowGoString_Katalog_Info_o_GSM_operatore + sizeof(t_strAndStrCnt))
#define addr_DataForShowGoString_Katalog_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke (addr_DataForShowGoString_Katalog_Nastroyki + sizeof(t_strAndStrCnt))
#define addr_DataForShowGoString_Katalog_Maksimal_no_dopustimoe_napryazhenie_nagruzki (addr_DataForShowGoString_Katalog_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke + sizeof(t_strAndStrCnt))
#define addr_DataForShowGoString_Katalog_Minimal_no_dopustimoe_napryazhenie_nagruzki (addr_DataForShowGoString_Katalog_Maksimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_strAndStrCnt))
#define addr_DataForShowGoString_Katalog_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely (addr_DataForShowGoString_Katalog_Minimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_strAndStrCnt))
#define addr_DataForShowGoString_Katalog_Vklyuchit__zvuk (addr_DataForShowGoString_Katalog_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely + sizeof(t_strAndStrCnt))
#define addr_DataForShowGoString_Katalog_Reset_All_Options (addr_DataForShowGoString_Katalog_Vklyuchit__zvuk + sizeof(t_strAndStrCnt))





///////////////////////////////////////////////////////
// ������ ��� ������� �� rus1_stencil_funcs_changedata
/////////////////////////////////////////////////////

#define addr_DataForChangeWithCursor_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke (addr_DataForShowGoString_Katalog_Reset_All_Options + sizeof(t_strAndStrCnt))
#define addr_DataForChangeWithCursor_Maksimal_no_dopustimoe_napryazhenie_nagruzki (addr_DataForChangeWithCursor_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke + sizeof(t_dataForChangeWithCursor))
#define addr_DataForChangeWithCursor_Minimal_no_dopustimoe_napryazhenie_nagruzki (addr_DataForChangeWithCursor_Maksimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_dataForChangeWithCursor))
#define addr_DataForChangeArrayStr_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely (addr_DataForChangeWithCursor_Minimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_dataForChangeWithCursor))
#define addr_DataForChangeArrayStr_Vklyuchit__zvuk (addr_DataForChangeArrayStr_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely + sizeof(t_dataForChangeIndex_ArrayStr))





///////////////////////////////////////////
// ������ TreeNode
///////////////////////////////////////////

////////////////// 
// �������� �������

// ����� TreeNode Root(0xE000)
// #define addrTreeNode_Root (0xE000)
#define addrTreeNode_Root (addr_DataForChangeArrayStr_Vklyuchit__zvuk + sizeof(t_dataForChangeIndex_ArrayStr))


////////////////// 
// ����� ����������

// ����� TreeNode ����������
#define addrTreeNode_Informatsiya (addrTreeNode_Root + sizeof(t_VLtreeNode))

// ����� TreeNode ���������� ����
#define addrTreeNode_Napryazhenie_rele (addrTreeNode_Informatsiya + sizeof(t_VLtreeNode))

// ����� TreeNode data ���������� ����
#define addrTreeNode_data_Napryazhenie_rele (addrTreeNode_Napryazhenie_rele + sizeof(t_VLtreeNode))

// ����� TreeNode ������ ���-�����
#define addrTreeNode_Balans_sim_karty (addrTreeNode_data_Napryazhenie_rele + sizeof(t_VLtreeNode))

// ����� TreeNode data ������ ���-�����
#define addrTreeNode_data_Balans_sim_karty (addrTreeNode_Balans_sim_karty + sizeof(t_VLtreeNode))

// ����� TreeNode ������� GSM-�������
#define addrTreeNode_Uroven__GSM_signala (addrTreeNode_data_Balans_sim_karty + sizeof(t_VLtreeNode))

// ����� TreeNode data ������� GSM-�������
#define addrTreeNode_data_Uroven__GSM_signala (addrTreeNode_Uroven__GSM_signala + sizeof(t_VLtreeNode))

// ����� TreeNode ���� � GSM-���������
#define addrTreeNode_Info_o_GSM_operatore (addrTreeNode_data_Uroven__GSM_signala + sizeof(t_VLtreeNode))

// ����� TreeNode data ���� � GSM-���������
#define addrTreeNode_data_Info_o_GSM_operatore (addrTreeNode_Info_o_GSM_operatore + sizeof(t_VLtreeNode))


/////////////////
// ����� ���������

// ����� TreeNode ���������
#define addrTreeNode_Nastroyki (addrTreeNode_data_Info_o_GSM_operatore + sizeof(t_VLtreeNode))

// ����� TreeNode ����� ����� ����. � ���. ���� ��� ������������
#define addrTreeNode_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke (addrTreeNode_Nastroyki + sizeof(t_VLtreeNode))

// ����� TreeNode data ����� ����� ����. � ���. ���� ��� ������������
#define addrTreeNode_data_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke (addrTreeNode_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke + sizeof(t_VLtreeNode))

// ����� TreeNode ����������� ���������� ���������� ��������
#define addrTreeNode_Maksimal_no_dopustimoe_napryazhenie_nagruzki (addrTreeNode_data_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke + sizeof(t_VLtreeNode))

// ����� TreeNode data ����������� ���������� ���������� ��������
#define addrTreeNode_data_Maksimal_no_dopustimoe_napryazhenie_nagruzki (addrTreeNode_Maksimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_VLtreeNode))

// ����� TreeNode ���������� ���������� ���������� ��������
#define addrTreeNode_Minimal_no_dopustimoe_napryazhenie_nagruzki (addrTreeNode_data_Maksimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_VLtreeNode))

// ����� TreeNode data ���������� ���������� ���������� ��������
#define addrTreeNode_data_Minimal_no_dopustimoe_napryazhenie_nagruzki (addrTreeNode_Minimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_VLtreeNode))

// ����� TreeNode �������� �� ��� ��� ������ ���������� �� ���������� �������
#define addrTreeNode_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely (addrTreeNode_data_Minimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_VLtreeNode))

// ����� TreeNode data �������� �� ��� ��� ������ ���������� �� ���������� �������
#define addrTreeNode_data_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely (addrTreeNode_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely + sizeof(t_VLtreeNode))

// ����� TreeNode �������� ����
#define addrTreeNode_Vklyuchit__zvuk (addrTreeNode_data_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely + sizeof(t_VLtreeNode))

// ����� TreeNode data �������� ����
#define addrTreeNode_data_Vklyuchit__zvuk (addrTreeNode_Vklyuchit__zvuk + sizeof(t_VLtreeNode))

// ����� TreeNode �������� ��� ���������
#define addrTreeNode_Reset_All_Options (addrTreeNode_data_Vklyuchit__zvuk + sizeof(t_VLtreeNode))

// ����� TreeNode data �������� ��� ���������
#define addrTreeNode_data_Reset_All_Options (addrTreeNode_Reset_All_Options + sizeof(t_VLtreeNode))


/////////////////////////
// extern
//////////// 


// ������ ��� TreeNode data ����� ����� ����. � ���. ���� ��� ������������
#define strCntBeforeDataMenuPauseRestart 4 // ����� ������ ����� �������
extern const t_dataForChangeWithCursor dataForChangeWithCursor_Pauza_mezhdu_vykl__i_rele_pri_perezagruzke;

// ������ ��� TreeNode data ����������� ���������� ���������� ��������
#define strCntBeforeData220 4 // ����� ������ ����� �������
extern const t_dataForChangeWithCursor dataForChangeWithCursor_Maksimal_no_dopustimoe_napryazhenie_nagruzki;

// ������ ��� TreeNode data ���������� ���������� ���������� ��������
extern const t_dataForChangeWithCursor dataForChangeWithCursor_Minimal_no_dopustimoe_napryazhenie_nagruzki;

// ������ ��� TreeNode �������� �� ��� ��� ������ ���������� �� ���������� �������
extern const t_dataForChangeIndex_ArrayStr dataForChangeIndex_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely;

// ������ ��� TreeNode �������� ����
extern const t_dataForChangeIndex_ArrayStr dataForChangeIndex_Vklyuchit__zvuk;



////////////////// 
// ����� ����������

// ������ ��� TreeNode ����������
#define lineInformatsiyaCnt tree_NameKatalogCount
extern const unsigned char lineInformatsiya[lineInformatsiyaCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Informatsiya;

// ������ ��� TreeNode ���������� ����
#define lineNapryazhenie_releCnt 16
extern const unsigned char lineNapryazhenie_rele[lineNapryazhenie_releCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Napryazhenie_rele;
  
// ������ ��� TreeNode ������ ���-�����
#define lineBalans_sim_kartyCnt 16
extern const unsigned char lineBalans_sim_karty[lineBalans_sim_kartyCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Balans_sim_karty;

// ������ ��� TreeNode ������� GSM-�������
#define lineUroven__GSM_signalaCnt 19
extern const unsigned char lineUroven__GSM_signala[lineUroven__GSM_signalaCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Uroven__GSM_signala;

// ������ ��� TreeNode ���� � GSM-���������
#define lineInfo_o_GSM_operatoreCnt 20
extern const unsigned char lineInfo_o_GSM_operatore[lineInfo_o_GSM_operatoreCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Info_o_GSM_operatore;


/////////////////
// ����� ���������

// ������ ��� TreeNode ���������
#define lineNastroykiCnt tree_NameKatalogCount
extern const unsigned char lineNastroyki[lineNastroykiCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Nastroyki;

// ������ ��� TreeNode ����� ����� ����. � ���. ���� ��� ������������
#define linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzkeCnt 47
extern const unsigned char linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke[linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzkeCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke;

// ������ ��� TreeNode ����������� ���������� ���������� ��������
//#define lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt 43
#define lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt 33
extern const unsigned char lineMaksimal_no_dopustimoe_napryazhenie_nagruzki[lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Maksimal_no_dopustimoe_napryazhenie_nagruzki;

// ������ ��� TreeNode ���������� ���������� ���������� ��������
//#define lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt 42
#define lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt 32
extern const unsigned char lineMinimal_no_dopustimoe_napryazhenie_nagruzki[lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Minimal_no_dopustimoe_napryazhenie_nagruzki;

// ������ ��� TreeNode �������� �� ��� ��� ������ ���������� �� ���������� �������
#define linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predelyCnt 60
extern const unsigned char linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely[linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predelyCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely;

// ������ ��� TreeNode �������� ����
#define lineVklyuchit__zvukCnt tree_NameKatalogCount
extern const unsigned char lineVklyuchit__zvuk[lineVklyuchit__zvukCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Vklyuchit__zvuk;

// ������ ��� TreeNode ����� ���� ��������
#define lineDataResetAll_OptionsCnt 22
extern const unsigned char lineDataResetAll_Options[lineDataResetAll_OptionsCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Reset_All_Options;



////////////////// 
// �������� �������

extern const t_VLtreeNode treeNode_Root;
extern const t_VLtreeNode treeNode_Informatsiya;

#endif // #ifndef __vl_TreeStructureGenerator_Structure__