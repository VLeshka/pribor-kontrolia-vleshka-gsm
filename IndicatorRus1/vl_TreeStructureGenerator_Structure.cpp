
#define vl_TreeStructureGenerator_Structure

#include "msp430G2553.h"

#include "vl_TreeStructureGenerator_Functions.hpp"
#include "vl_TreeStructureGenerator_Structure.hpp"
#include "rus1.hpp"
#include "example_base_types.hpp"
#include "example_options.hpp"
#include "rus1_stencil_funcs_changedata.hpp"
#include "rus1_screenLCD_AC-162A.hpp"
#include "example_base_strings.hpp"



// ������ ��� TreeNode data ����� ����� ����. � ���. ���� ��� ������������
#define strCntBeforeDataMenuPauseRestart 4 // ����� ������ ����� �������
#pragma location = addr_DataForChangeWithCursor_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke
__root const t_dataForChangeWithCursor dataForChangeWithCursor_Pauza_mezhdu_vykl__i_rele_pri_perezagruzke = {
  &nastr_SekRestart, 
  minDataMenuPauseRestart,
  maxDataMenuPauseRestart,
  line_Probel, 
  strC, 
  linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke, 
  strCntBeforeDataMenuPauseRestart,
  1,
  linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzkeCnt
};

// ������ ��� TreeNode data ����������� ���������� ���������� ��������
#define strCntBeforeData220 4 // ����� ������ ����� �������
#pragma location = addr_DataForChangeWithCursor_Maksimal_no_dopustimoe_napryazhenie_nagruzki
__root const t_dataForChangeWithCursor dataForChangeWithCursor_Maksimal_no_dopustimoe_napryazhenie_nagruzki = {
  &nastr_DataMax220, 
  minDataMax220,
  maxDataMax220,
  line_Probel,
  strB,
  lineMaksimal_no_dopustimoe_napryazhenie_nagruzki, 
  strCntBeforeData220,
  1,
  lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt
};

// ������ ��� TreeNode data ���������� ���������� ���������� ��������
#pragma location = addr_DataForChangeWithCursor_Minimal_no_dopustimoe_napryazhenie_nagruzki
__root const t_dataForChangeWithCursor dataForChangeWithCursor_Minimal_no_dopustimoe_napryazhenie_nagruzki = {
  &nastr_DataMin220, 
  minDataMin220,
  maxDataMin220,
  line_Probel,
  strB,
  lineMinimal_no_dopustimoe_napryazhenie_nagruzki,
  strCntBeforeData220,
  1,
  lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt
};

// ������ ��� TreeNode �������� �� ��� ��� ������ ���������� �� ���������� �������
#pragma location = addr_DataForChangeArrayStr_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely
__root const t_dataForChangeIndex_ArrayStr dataForChangeIndex_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely = {
  &nastr_IndexSendSmsOnBad220,
  varsDaNet,
  linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely,
  varsDaNetCnt,
  linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predelyCnt
};

// ������ ��� TreeNode �������� ����
#pragma location = addr_DataForChangeArrayStr_Vklyuchit__zvuk
__root const t_dataForChangeIndex_ArrayStr dataForChangeIndex_Vklyuchit__zvuk = {
  &nastr_IndexBeepOff,
  varsDaNet,
  lineVklyuchit__zvuk, 
  varsDaNetCnt,
  lineVklyuchit__zvukCnt
};



////////////////// 
// ����� ����������

// ������ ��� TreeNode ����������
#define lineInformatsiyaCnt tree_NameKatalogCount
  const unsigned char lineInformatsiya[lineInformatsiyaCnt] = 
//                     0   1   2   3   4   5   6   7   8   9   10  11  12  13  14
                    {'�','�','�','�','�','�','�','�','�','�',' ',' ',' ',' ',' ' };
#pragma location = addr_DataForShowGoString_Katalog_Informatsiya
__root const t_strAndStrCnt dataForShowGoString_Katalog_Informatsiya = {
  lineInformatsiya,
  lineInformatsiyaCnt
};

// ������ ��� TreeNode ���������� ����
#define lineNapryazhenie_releCnt 16
  const unsigned char lineNapryazhenie_rele[lineNapryazhenie_releCnt] = 
//   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14
  { '�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�',':' };
#pragma location = addr_DataForShowGoString_Katalog_Napryazhenie_rele
__root const t_strAndStrCnt dataForShowGoString_Katalog_Napryazhenie_rele = {
  lineNapryazhenie_rele,
  lineNapryazhenie_releCnt-1
};
  
// ������ ��� TreeNode ������ ���-�����
#define lineBalans_sim_kartyCnt 16
  const unsigned char lineBalans_sim_karty[lineBalans_sim_kartyCnt] = 
//                     0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
                    { '�','�','�','�','�','�',' ','�','�','�','-','�','�','�','�','�' };
#pragma location = addr_DataForShowGoString_Katalog_Balans_sim_karty
__root const t_strAndStrCnt dataForShowGoString_Katalog_Balans_sim_karty = {
  lineBalans_sim_karty, 
  lineBalans_sim_kartyCnt
};

// ������ ��� TreeNode ������� GSM-�������
#define lineUroven__GSM_signalaCnt 19
  const unsigned char lineUroven__GSM_signala[lineUroven__GSM_signalaCnt] = 
//                     0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18
                    { '�','�','�','�','�','�','�',' ','G','S','M','-','�','�','�','�','�','�','�' };
#pragma location = addr_DataForShowGoString_Katalog_Uroven__GSM_signala
__root const t_strAndStrCnt dataForShowGoString_Katalog_Uroven__GSM_signala = {
  lineUroven__GSM_signala,
  lineUroven__GSM_signalaCnt
};

// ������ ��� TreeNode ���� � GSM-���������
#define lineInfo_o_GSM_operatoreCnt 20
  const unsigned char lineInfo_o_GSM_operatore[lineInfo_o_GSM_operatoreCnt] = 
//                     0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19
                    { '�','�','�','�',' ','�',' ','G','S','M','-','�','�','�','�','�','�','�','�','�' };
#pragma location = addr_DataForShowGoString_Katalog_Info_o_GSM_operatore
__root const t_strAndStrCnt dataForShowGoString_Katalog_Info_o_GSM_operatore = {
  lineInfo_o_GSM_operatore,
  lineInfo_o_GSM_operatoreCnt
};


/////////////////
// ����� ���������

// ������ ��� TreeNode ���������
#define lineNastroykiCnt tree_NameKatalogCount
  const unsigned char lineNastroyki[lineNastroykiCnt] = 
//                     0   1   2   3   4   5   6   7   8   9   10  11  12  13  14
                    { '�','�','�','�','�','�','�','�','�',' ',' ',' ',' ',' ',' '};
#pragma location = addr_DataForShowGoString_Katalog_Nastroyki
__root const t_strAndStrCnt dataForShowGoString_Katalog_Nastroyki = {
  lineNastroyki, 
  lineNastroykiCnt
};

// ������ ��� TreeNode ����� ����� ����. � ���. ���� ��� ������������
#define linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzkeCnt 47
const unsigned char linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke[linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzkeCnt] = 
//  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41
 { '�','�','�','�','�',' ','�','�','�','�','�',' ','�','�','�','�','.',' ','�',' ','�','�','�','.',' ','�','�','�','�',' ','�','�','�',' ','�','�','�','�','�','�','�','�','�','�','�','�',':' };
#pragma location = addr_DataForShowGoString_Katalog_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke
__root const t_strAndStrCnt dataForShowGoString_Katalog_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke = {
  linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke, 
  linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzkeCnt-1
};

// ������ ��� TreeNode ����������� ���������� ���������� ��������
//#define lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt 43
//  const unsigned char lineMaksimal_no_dopustimoe_napryazhenie_nagruzki[lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt] = 
//  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37
// { '�','�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�',':' };

#define lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt 33
  const unsigned char lineMaksimal_no_dopustimoe_napryazhenie_nagruzki[lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt] = 
//  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37
 { '�','�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','�' };

#pragma location = addr_DataForShowGoString_Katalog_Maksimal_no_dopustimoe_napryazhenie_nagruzki
__root const t_strAndStrCnt dataForShowGoString_Katalog_Maksimal_no_dopustimoe_napryazhenie_nagruzki = {
  lineMaksimal_no_dopustimoe_napryazhenie_nagruzki,
  lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt-1
};

// ������ ��� TreeNode ���������� ���������� ���������� ��������
//#define lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt 42
//  const unsigned char lineMinimal_no_dopustimoe_napryazhenie_nagruzki[lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt] = 
//  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36
// { '�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�',':' };

#define lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt 32
  const unsigned char lineMinimal_no_dopustimoe_napryazhenie_nagruzki[lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt] = 
//  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36
 { '�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','�' };

#pragma location = addr_DataForShowGoString_Katalog_Minimal_no_dopustimoe_napryazhenie_nagruzki
__root const t_strAndStrCnt dataForShowGoString_Katalog_Minimal_no_dopustimoe_napryazhenie_nagruzki = {
  lineMinimal_no_dopustimoe_napryazhenie_nagruzki, 
  lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt-1
};

// ������ ��� TreeNode �������� �� ��� ��� ������ ���������� �� ���������� �������
#define linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predelyCnt 60
  const unsigned char linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely[linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predelyCnt] = 
//  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57  58
 { '�','�','�','�','�','�','�','�',' ','�','�',' ','�','�','�',' ','�','�','�',' ','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','�',' ','�','�',' ','�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�',':' };
#pragma location = addr_DataForShowGoString_Katalog_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely
__root const t_strAndStrCnt dataForShowGoString_Katalog_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely = {
  linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely, 
  linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predelyCnt-1
};

// ������ ��� TreeNode �������� ����
#define lineVklyuchit__zvukCnt tree_NameKatalogCount
  const unsigned char lineVklyuchit__zvuk[lineVklyuchit__zvukCnt] = 
// 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14
{ '�','�','�','�','�','�','�','�',' ','�','�','�','�',' ',' ' };
#pragma location = addr_DataForShowGoString_Katalog_Vklyuchit__zvuk
__root const t_strAndStrCnt dataForShowGoString_Katalog_Vklyuchit__zvuk = {
  lineVklyuchit__zvuk, 
  lineVklyuchit__zvukCnt
};

// ������ ��� TreeNode ����� ���� ��������
#define lineDataResetAll_OptionsCnt 22
  const unsigned char lineDataResetAll_Options[lineDataResetAll_OptionsCnt] = 
// 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21
{ '�','�','�','�','�','�','�','�',' ','�','�','�',' ','�','�','�','�','�','�','�','�','�' };
#pragma location = addr_DataForShowGoString_Katalog_Reset_All_Options
__root const t_strAndStrCnt dataForShowGoString_Katalog_Reset_All_Options = {
  lineDataResetAll_Options, 
  lineDataResetAll_OptionsCnt
};



////////////////// 
// �������� �������
//////////////////

__root const t_VLtreeNode *const childsOf_Root[2] = { (t_VLtreeNode*)addrTreeNode_Informatsiya,
                                                      (t_VLtreeNode*)addrTreeNode_Nastroyki };
#pragma location = addrTreeNode_Root
__root const t_VLtreeNode treeNode_Root = { NULL, Show_Root, childsOf_Root, NULL, 2, 0 };

////////////////// 
// ����� ����������

// TreeNode ����������
__root const t_VLtreeNode *const childsOf_Informatsiya[4] = { (t_VLtreeNode*)addrTreeNode_Napryazhenie_rele,
                                                   (t_VLtreeNode*)addrTreeNode_Balans_sim_karty,
                                                   (t_VLtreeNode*)addrTreeNode_Uroven__GSM_signala,
                                                   (t_VLtreeNode*)addrTreeNode_Info_o_GSM_operatore };
#pragma location = addrTreeNode_Informatsiya
__root const t_VLtreeNode treeNode_Informatsiya = { &treeNode_Root, LCD_ShowGoString_Katalog, childsOf_Informatsiya, (void*)(&dataForShowGoString_Katalog_Informatsiya), 4, 0 };

// TreeNode ���������� ����
__root const t_VLtreeNode *const childsOf_Napryazhenie_rele[1] = { (t_VLtreeNode*)addrTreeNode_data_Napryazhenie_rele };
#pragma location = addrTreeNode_Napryazhenie_rele
__root const t_VLtreeNode treeNode_Napryazhenie_rele = { &treeNode_Informatsiya, LCD_ShowGoString_Katalog, childsOf_Napryazhenie_rele, (void*)(&dataForShowGoString_Katalog_Napryazhenie_rele), 1, 0 };

// TreeNode data ���������� ����
#pragma location = addrTreeNode_data_Napryazhenie_rele
__root const t_VLtreeNode treeNode_data_Napryazhenie_rele = { &treeNode_Napryazhenie_rele, Show_data_Napryazhenie_rele, NULL, NULL, 0, 0 };

// TreeNode ������ ���-�����
__root const t_VLtreeNode *const childsOf_Balans_sim_karty[1] = { (t_VLtreeNode*)addrTreeNode_data_Balans_sim_karty };
#pragma location = addrTreeNode_Balans_sim_karty
__root const t_VLtreeNode treeNode_Balans_sim_karty = { &treeNode_Informatsiya, LCD_ShowGoString_Katalog, childsOf_Balans_sim_karty, (void*)(&dataForShowGoString_Katalog_Balans_sim_karty), 1, 0 };

// TreeNode data ������ ���-�����
#pragma location = addrTreeNode_data_Balans_sim_karty
__root const t_VLtreeNode treeNode_data_Balans_sim_karty = { &treeNode_Balans_sim_karty, Show_data_Balans_sim_karty, NULL, NULL, 0, 0 };

// TreeNode ������� GSM-�������
__root const t_VLtreeNode *const childsOf_Uroven__GSM_signala[1] = { (t_VLtreeNode*)addrTreeNode_data_Uroven__GSM_signala };
#pragma location = addrTreeNode_Uroven__GSM_signala
__root const t_VLtreeNode treeNode_Uroven__GSM_signala = { &treeNode_Informatsiya, LCD_ShowGoString_Katalog, childsOf_Uroven__GSM_signala, (void*)(&dataForShowGoString_Katalog_Uroven__GSM_signala), 1, 0 };

// TreeNode data ������� GSM-�������
#pragma location = addrTreeNode_data_Uroven__GSM_signala
__root const t_VLtreeNode treeNode_data_Uroven__GSM_signala = { &treeNode_Uroven__GSM_signala, Show_data_Uroven__GSM_signala, NULL, NULL, 0, 0 };

// TreeNode ���� � GSM-���������
__root const t_VLtreeNode *const childsOf_Info_o_GSM_operatore[1] = { (t_VLtreeNode*)addrTreeNode_data_Info_o_GSM_operatore };
#pragma location = addrTreeNode_Info_o_GSM_operatore
__root const t_VLtreeNode treeNode_Info_o_GSM_operatore = { &treeNode_Informatsiya, LCD_ShowGoString_Katalog, childsOf_Info_o_GSM_operatore, (void*)(&dataForShowGoString_Katalog_Info_o_GSM_operatore), 1, 0 };

// TreeNode data ���� � GSM-���������
#pragma location = addrTreeNode_data_Info_o_GSM_operatore
__root const t_VLtreeNode treeNode_data_Info_o_GSM_operatore = { &treeNode_Info_o_GSM_operatore, Show_data_Info_o_GSM_operatore, NULL, NULL, 0, 0 };

/////////////////
// ����� ���������
/////////////////

// TreeNode ���������
__root const t_VLtreeNode *const childsOf_Nastroyki[6] = { (t_VLtreeNode*)addrTreeNode_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke,
                                                  (t_VLtreeNode*)addrTreeNode_Maksimal_no_dopustimoe_napryazhenie_nagruzki,
                                                  (t_VLtreeNode*)addrTreeNode_Minimal_no_dopustimoe_napryazhenie_nagruzki,
                                                  (t_VLtreeNode*)addrTreeNode_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely,
                                                  (t_VLtreeNode*)addrTreeNode_Vklyuchit__zvuk,
                                                  (t_VLtreeNode*)addrTreeNode_Reset_All_Options };
#pragma location = addrTreeNode_Nastroyki
__root const t_VLtreeNode treeNode_Nastroyki = { &treeNode_Root, LCD_ShowGoString_Katalog, childsOf_Nastroyki, (void*)(&dataForShowGoString_Katalog_Nastroyki), 6, 0 };

// TreeNode ����� ����� ����. � ���. ���� ��� ������������
__root const t_VLtreeNode *const childsOf_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke[1] = { (t_VLtreeNode*)addrTreeNode_data_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke };
#pragma location = addrTreeNode_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke
__root const t_VLtreeNode treeNode_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke = { &treeNode_Nastroyki, LCD_ShowGoString_Katalog, childsOf_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke, (void*)(&dataForShowGoString_Katalog_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke), 1, 0 };

// TreeNode data ����� ����� ����. � ���. ���� ��� ������������
#pragma location = addrTreeNode_data_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke
__root const t_VLtreeNode treeNode_data_Pauza_mezhdu_vykl__i_rele_pri_perezagruzke = { &treeNode_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke, Data_ChangeWithCursor, NULL, (void*)(&dataForChangeWithCursor_Pauza_mezhdu_vykl__i_rele_pri_perezagruzke), 0, BIT0 };

// TreeNode ����������� ���������� ���������� ��������
__root const t_VLtreeNode *const childsOf_Maksimal_no_dopustimoe_napryazhenie_nagruzki[1] = { (t_VLtreeNode*)addrTreeNode_data_Maksimal_no_dopustimoe_napryazhenie_nagruzki };
#pragma location = addrTreeNode_Maksimal_no_dopustimoe_napryazhenie_nagruzki
__root const t_VLtreeNode treeNode_Maksimal_no_dopustimoe_napryazhenie_nagruzki = { &treeNode_Nastroyki, LCD_ShowGoString_Katalog, childsOf_Maksimal_no_dopustimoe_napryazhenie_nagruzki, (void*)(&dataForShowGoString_Katalog_Maksimal_no_dopustimoe_napryazhenie_nagruzki), 1, 0 };

// TreeNode data ����������� ���������� ���������� ��������
#pragma location = addrTreeNode_data_Maksimal_no_dopustimoe_napryazhenie_nagruzki
__root const t_VLtreeNode treeNode_data_Maksimal_no_dopustimoe_napryazhenie_nagruzki = { &treeNode_Maksimal_no_dopustimoe_napryazhenie_nagruzki, Data_ChangeWithCursor, NULL, (void*)(&dataForChangeWithCursor_Maksimal_no_dopustimoe_napryazhenie_nagruzki), 0, BIT0 };

// TreeNode ���������� ���������� ���������� ��������
__root const t_VLtreeNode *const childsOf_Minimal_no_dopustimoe_napryazhenie_nagruzki[1] = { (t_VLtreeNode*)addrTreeNode_data_Minimal_no_dopustimoe_napryazhenie_nagruzki };
#pragma location = addrTreeNode_Minimal_no_dopustimoe_napryazhenie_nagruzki
__root const t_VLtreeNode treeNode_Minimal_no_dopustimoe_napryazhenie_nagruzki = { &treeNode_Nastroyki, LCD_ShowGoString_Katalog, childsOf_Minimal_no_dopustimoe_napryazhenie_nagruzki, (void*)(&dataForShowGoString_Katalog_Minimal_no_dopustimoe_napryazhenie_nagruzki), 1, 0 };

// TreeNode data ���������� ���������� ���������� ��������
#pragma location = addrTreeNode_data_Minimal_no_dopustimoe_napryazhenie_nagruzki
__root const t_VLtreeNode treeNode_data_Minimal_no_dopustimoe_napryazhenie_nagruzki = { &treeNode_Minimal_no_dopustimoe_napryazhenie_nagruzki, Data_ChangeWithCursor, NULL, (void*)(&dataForChangeWithCursor_Minimal_no_dopustimoe_napryazhenie_nagruzki), 0, BIT0 };

// TreeNode �������� �� ��� ��� ������ ���������� �� ���������� �������
__root const t_VLtreeNode *const childsOf_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely[1] = { (t_VLtreeNode*)addrTreeNode_data_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely };
#pragma location = addrTreeNode_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely
__root const t_VLtreeNode treeNode_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely = { &treeNode_Nastroyki, LCD_ShowGoString_Katalog, childsOf_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely, (void*)(&dataForShowGoString_Katalog_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely), 1, 0 };

// TreeNode data �������� �� ��� ��� ������ ���������� �� ���������� �������
#pragma location = addrTreeNode_data_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely
__root const t_VLtreeNode treeNode_data_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely = { &treeNode_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely, Data_ChangeIndex_ArrayStr, NULL, (void*)(&dataForChangeIndex_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely), 0, BIT0 };

// TreeNode �������� ����
__root const t_VLtreeNode *const childsOf_Vklyuchit__zvuk[1] = { (t_VLtreeNode*)addrTreeNode_data_Vklyuchit__zvuk };
#pragma location = addrTreeNode_Vklyuchit__zvuk
__root const t_VLtreeNode treeNode_Vklyuchit__zvuk = { &treeNode_Nastroyki, LCD_ShowGoString_Katalog, childsOf_Vklyuchit__zvuk, (void*)(&dataForShowGoString_Katalog_Vklyuchit__zvuk), 1, 0 };

// TreeNode data �������� ����
#pragma location = addrTreeNode_data_Vklyuchit__zvuk
__root const t_VLtreeNode treeNode_data_Vklyuchit__zvuk = { &treeNode_Vklyuchit__zvuk, Data_ChangeIndex_ArrayStr, NULL, (void*)(&dataForChangeIndex_Vklyuchit__zvuk), 0, BIT0 };

// TreeNode �������� ��� ���������
__root const t_VLtreeNode *const childsOf_Reset_All_Options[1] = { (t_VLtreeNode*)addrTreeNode_data_Reset_All_Options };
#pragma location = addrTreeNode_Reset_All_Options
__root const t_VLtreeNode treeNode_Reset_All_Options = { &treeNode_Nastroyki, LCD_ShowGoString_Katalog, childsOf_Reset_All_Options, (void*)(&dataForShowGoString_Katalog_Reset_All_Options), 1, 0 };

// TreeNode data �������� ��� ���������
#pragma location = addrTreeNode_data_Reset_All_Options
__root const t_VLtreeNode treeNode_data_Reset_All_Options = { &treeNode_Reset_All_Options, Show_Reset_All_Options, NULL, NULL, 0, BIT0 };