// составлено программой TreeStructureGenerator

#ifndef __vl_TreeStructureGenerator_Functions__
#define __vl_TreeStructureGenerator_Functions__

#include "rus1_mainTitle.hpp"

/////////////////////////////////////////////////////
// прототипы подпрограмм для работы с деревом TreeNode


////////////////// 
// Корневой каталог

void Show_Root( void );


////////////////// 
// Ветка Информация

// TreeNode Информация
void Show_Informatsiya( void );

// TreeNode data Напряжение реле
void Show_data_Napryazhenie_rele( void );

// TreeNode data Баланс сим-карты
void Show_data_Balans_sim_karty( void );

// TreeNode data Уровень GSM-сигнала
void Show_data_Uroven__GSM_signala( void );

// TreeNode data Инфо о GSM-операторе
void Show_data_Info_o_GSM_operatore( void );


/////////////////
// Ветка Настройки

// TreeNode Выполнение сброса всех настроек
void Show_Reset_All_Options( void );

#endif // #ifndef __vl_TreeStructureGenerator_Functions__