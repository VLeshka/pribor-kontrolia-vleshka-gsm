// структура дерева TreeNode
// составлено программой TreeStructureGenerator
// адреса по одной ветке "с корневого TreeNode сверху вниз"

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
// подпрограммы

// TreeNode Сброс всех настроек
extern void Show_Reset_All_Options( void );

// Изменение на геометрически изменяющееся значение (1, 10, 100, ...), и отображение новых данных на экране.
// Порядок значения указывается положением мигающего курсора. Положение курсора изменяется длинельностью нажатия 
// кнопок "вверх" и "вниз".
// В прерывании таймера изменяется порядок числа GetLnChange, следя за длительности нажатия клавиш по времени.
extern void Data_ChangeWithCursor(); 

// Изменение по массиву допустимых значений (строки), и отображение новых данных на экране.
// Сохраняет или не сохраняет настройки, в зависимости от выбора пользователя.
extern void Data_ChangeIndex_ArrayStr();

// показывает бегущую строку названия каталога
extern void LCD_ShowGoString_Katalog();

////////////////// 
// Корневой каталог

extern void Show_Root( void );

////////////////// 
// Ветка Информация

// TreeNode Информация
extern void Show_Informatsiya( void );

// TreeNode data Напряжение реле
extern void Show_data_Napryazhenie_rele( void );

// TreeNode data Баланс сим-карты
extern void Show_data_Balans_sim_karty( void );

// TreeNode data Уровень GSM-сигнала
extern void Show_data_Uroven__GSM_signala( void );

// TreeNode data Инфо о GSM-операторе
extern void Show_data_Info_o_GSM_operatore( void );

// Определение каталог/данные:
//    - Данные  - TreeNode, использующий для своего отображения полный экран; не имеет дочерних пунктов веток.
//      Возможно наличие нескольких таких TreeNode в одном каталоге.
//    - Каталог - TreeNode, отображающий каталоги (по одной в строке, кроме root) или данные.


///////////////////////////////////////////////////
// в данном варианте дерево располагается на flash
////////////////////////////////////////////////// 

/*
Структура дерева:

каталог
Root(0xE000)--->Информация--->Напряжение реле----------------------------------------------->data Напряжение реле
             |             |->Баланс сим-карты---------------------------------------------->data Баланс сим-карты
             |             |->Уровень GSM-сигнала------------------------------------------->data Уровень GSM-сигнала
             |             |->Инфо о GSM-операторе------------------------------------------>data Инфо о GSM-операторе
             |->Настройки---->Пауза между выкл. и вкл. реле при перезагрузке---------------->data Пауза между выкл. и вкл. реле при перезагрузке
                           |->Максимально допустимое напряжение нагрузки-------------------->data Максимально допустимое напряжение нагрузки
                           |->Минимально допустимое напряжение нагрузки--------------------->data Минимально допустимое напряжение нагрузки
                           |->Посылать ли смс при выходе напряжения за допустимые пределы--->data Посылать ли смс при выходе напряжения за допустимые пределы
                           |->Включить звук------------------------------------------------->data Включить звук
                           |->Сбросить все настройки---------------------------------------->data Сбросить все настройки
*/


///////////////////////////////////////////////////////
// данные для функции показа названия каталога
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
// данные для функций из rus1_stencil_funcs_changedata
/////////////////////////////////////////////////////

#define addr_DataForChangeWithCursor_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke (addr_DataForShowGoString_Katalog_Reset_All_Options + sizeof(t_strAndStrCnt))
#define addr_DataForChangeWithCursor_Maksimal_no_dopustimoe_napryazhenie_nagruzki (addr_DataForChangeWithCursor_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke + sizeof(t_dataForChangeWithCursor))
#define addr_DataForChangeWithCursor_Minimal_no_dopustimoe_napryazhenie_nagruzki (addr_DataForChangeWithCursor_Maksimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_dataForChangeWithCursor))
#define addr_DataForChangeArrayStr_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely (addr_DataForChangeWithCursor_Minimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_dataForChangeWithCursor))
#define addr_DataForChangeArrayStr_Vklyuchit__zvuk (addr_DataForChangeArrayStr_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely + sizeof(t_dataForChangeIndex_ArrayStr))





///////////////////////////////////////////
// адреса TreeNode
///////////////////////////////////////////

////////////////// 
// Корневой каталог

// адрес TreeNode Root(0xE000)
// #define addrTreeNode_Root (0xE000)
#define addrTreeNode_Root (addr_DataForChangeArrayStr_Vklyuchit__zvuk + sizeof(t_dataForChangeIndex_ArrayStr))


////////////////// 
// Ветка Информация

// адрес TreeNode Информация
#define addrTreeNode_Informatsiya (addrTreeNode_Root + sizeof(t_VLtreeNode))

// адрес TreeNode Напряжение реле
#define addrTreeNode_Napryazhenie_rele (addrTreeNode_Informatsiya + sizeof(t_VLtreeNode))

// адрес TreeNode data Напряжение реле
#define addrTreeNode_data_Napryazhenie_rele (addrTreeNode_Napryazhenie_rele + sizeof(t_VLtreeNode))

// адрес TreeNode Баланс сим-карты
#define addrTreeNode_Balans_sim_karty (addrTreeNode_data_Napryazhenie_rele + sizeof(t_VLtreeNode))

// адрес TreeNode data Баланс сим-карты
#define addrTreeNode_data_Balans_sim_karty (addrTreeNode_Balans_sim_karty + sizeof(t_VLtreeNode))

// адрес TreeNode Уровень GSM-сигнала
#define addrTreeNode_Uroven__GSM_signala (addrTreeNode_data_Balans_sim_karty + sizeof(t_VLtreeNode))

// адрес TreeNode data Уровень GSM-сигнала
#define addrTreeNode_data_Uroven__GSM_signala (addrTreeNode_Uroven__GSM_signala + sizeof(t_VLtreeNode))

// адрес TreeNode Инфо о GSM-операторе
#define addrTreeNode_Info_o_GSM_operatore (addrTreeNode_data_Uroven__GSM_signala + sizeof(t_VLtreeNode))

// адрес TreeNode data Инфо о GSM-операторе
#define addrTreeNode_data_Info_o_GSM_operatore (addrTreeNode_Info_o_GSM_operatore + sizeof(t_VLtreeNode))


/////////////////
// Ветка Настройки

// адрес TreeNode Настройки
#define addrTreeNode_Nastroyki (addrTreeNode_data_Info_o_GSM_operatore + sizeof(t_VLtreeNode))

// адрес TreeNode Пауза между выкл. и вкл. реле при перезагрузке
#define addrTreeNode_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke (addrTreeNode_Nastroyki + sizeof(t_VLtreeNode))

// адрес TreeNode data Пауза между выкл. и вкл. реле при перезагрузке
#define addrTreeNode_data_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke (addrTreeNode_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke + sizeof(t_VLtreeNode))

// адрес TreeNode Максимально допустимое напряжение нагрузки
#define addrTreeNode_Maksimal_no_dopustimoe_napryazhenie_nagruzki (addrTreeNode_data_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke + sizeof(t_VLtreeNode))

// адрес TreeNode data Максимально допустимое напряжение нагрузки
#define addrTreeNode_data_Maksimal_no_dopustimoe_napryazhenie_nagruzki (addrTreeNode_Maksimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_VLtreeNode))

// адрес TreeNode Минимально допустимое напряжение нагрузки
#define addrTreeNode_Minimal_no_dopustimoe_napryazhenie_nagruzki (addrTreeNode_data_Maksimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_VLtreeNode))

// адрес TreeNode data Минимально допустимое напряжение нагрузки
#define addrTreeNode_data_Minimal_no_dopustimoe_napryazhenie_nagruzki (addrTreeNode_Minimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_VLtreeNode))

// адрес TreeNode Посылать ли смс при выходе напряжения за допустимые пределы
#define addrTreeNode_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely (addrTreeNode_data_Minimal_no_dopustimoe_napryazhenie_nagruzki + sizeof(t_VLtreeNode))

// адрес TreeNode data Посылать ли смс при выходе напряжения за допустимые пределы
#define addrTreeNode_data_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely (addrTreeNode_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely + sizeof(t_VLtreeNode))

// адрес TreeNode Включить звук
#define addrTreeNode_Vklyuchit__zvuk (addrTreeNode_data_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely + sizeof(t_VLtreeNode))

// адрес TreeNode data Включить звук
#define addrTreeNode_data_Vklyuchit__zvuk (addrTreeNode_Vklyuchit__zvuk + sizeof(t_VLtreeNode))

// адрес TreeNode Сбросить все настройки
#define addrTreeNode_Reset_All_Options (addrTreeNode_data_Vklyuchit__zvuk + sizeof(t_VLtreeNode))

// адрес TreeNode data Сбросить все настройки
#define addrTreeNode_data_Reset_All_Options (addrTreeNode_Reset_All_Options + sizeof(t_VLtreeNode))


/////////////////////////
// extern
//////////// 


// данные для TreeNode data Пауза между выкл. и вкл. реле при перезагрузке
#define strCntBeforeDataMenuPauseRestart 4 // длина строки перед данными
extern const t_dataForChangeWithCursor dataForChangeWithCursor_Pauza_mezhdu_vykl__i_rele_pri_perezagruzke;

// данные для TreeNode data Максимально допустимое напряжение нагрузки
#define strCntBeforeData220 4 // длина строки перед данными
extern const t_dataForChangeWithCursor dataForChangeWithCursor_Maksimal_no_dopustimoe_napryazhenie_nagruzki;

// данные для TreeNode data Минимально допустимое напряжение нагрузки
extern const t_dataForChangeWithCursor dataForChangeWithCursor_Minimal_no_dopustimoe_napryazhenie_nagruzki;

// данные для TreeNode Посылать ли смс при выходе напряжения за допустимые пределы
extern const t_dataForChangeIndex_ArrayStr dataForChangeIndex_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely;

// данные для TreeNode Включить звук
extern const t_dataForChangeIndex_ArrayStr dataForChangeIndex_Vklyuchit__zvuk;



////////////////// 
// Ветка Информация

// данные для TreeNode Информация
#define lineInformatsiyaCnt tree_NameKatalogCount
extern const unsigned char lineInformatsiya[lineInformatsiyaCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Informatsiya;

// данные для TreeNode Напряжение реле
#define lineNapryazhenie_releCnt 16
extern const unsigned char lineNapryazhenie_rele[lineNapryazhenie_releCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Napryazhenie_rele;
  
// данные для TreeNode Баланс сим-карты
#define lineBalans_sim_kartyCnt 16
extern const unsigned char lineBalans_sim_karty[lineBalans_sim_kartyCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Balans_sim_karty;

// данные для TreeNode Уровень GSM-сигнала
#define lineUroven__GSM_signalaCnt 19
extern const unsigned char lineUroven__GSM_signala[lineUroven__GSM_signalaCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Uroven__GSM_signala;

// данные для TreeNode Инфо о GSM-операторе
#define lineInfo_o_GSM_operatoreCnt 20
extern const unsigned char lineInfo_o_GSM_operatore[lineInfo_o_GSM_operatoreCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Info_o_GSM_operatore;


/////////////////
// Ветка Настройки

// данные для TreeNode Настройки
#define lineNastroykiCnt tree_NameKatalogCount
extern const unsigned char lineNastroyki[lineNastroykiCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Nastroyki;

// данные для TreeNode Пауза между выкл. и вкл. реле при перезагрузке
#define linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzkeCnt 47
extern const unsigned char linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke[linePauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzkeCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Pauza_mezhdu_vykl__i_vkl__rele_pri_perezagruzke;

// данные для TreeNode Максимально допустимое напряжение нагрузки
//#define lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt 43
#define lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt 33
extern const unsigned char lineMaksimal_no_dopustimoe_napryazhenie_nagruzki[lineMaksimal_no_dopustimoe_napryazhenie_nagruzkiCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Maksimal_no_dopustimoe_napryazhenie_nagruzki;

// данные для TreeNode Минимально допустимое напряжение нагрузки
//#define lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt 42
#define lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt 32
extern const unsigned char lineMinimal_no_dopustimoe_napryazhenie_nagruzki[lineMinimal_no_dopustimoe_napryazhenie_nagruzkiCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Minimal_no_dopustimoe_napryazhenie_nagruzki;

// данные для TreeNode Посылать ли смс при выходе напряжения за допустимые пределы
#define linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predelyCnt 60
extern const unsigned char linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely[linePosylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predelyCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Posylat__li_sms_pri_vykhode_napryazheniya_za_dopustimye_predely;

// данные для TreeNode Включить звук
#define lineVklyuchit__zvukCnt tree_NameKatalogCount
extern const unsigned char lineVklyuchit__zvuk[lineVklyuchit__zvukCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Vklyuchit__zvuk;

// данные для TreeNode Сброс всех настроек
#define lineDataResetAll_OptionsCnt 22
extern const unsigned char lineDataResetAll_Options[lineDataResetAll_OptionsCnt];
extern const t_strAndStrCnt dataForShowGoString_Katalog_Reset_All_Options;



////////////////// 
// Корневой каталог

extern const t_VLtreeNode treeNode_Root;
extern const t_VLtreeNode treeNode_Informatsiya;

#endif // #ifndef __vl_TreeStructureGenerator_Structure__