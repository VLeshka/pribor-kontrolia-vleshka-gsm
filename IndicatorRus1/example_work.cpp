// основной рабочий модуль

#define example_work

#include "rus1_peripherals.hpp"

// схема организация модулей:
// 
//  *.cpp // подпрограммы
//    |- *.h // переменные, константы, прототипы подпрограмм, все дефайны (кроме псевдофункций и псевдо-bool)
//    |- * _def.cpp // псевдофункции и псевдо-bool
//    |- *_types.h // отдельное описание типов данных
// Примечание:
//  константы и дефайны, которые используются только 1 раз в подпрограмме, приводятся перед подпрограммой

#include "msp430G2553.h"

#ifdef testRus1
  #define testRezhim
#endif

#include "example_base_strings.hpp"
#include "example_base_functions.hpp"
#include "example_options.hpp"
#include "peripherals.hpp"
#include "example_options.hpp"
#include "gsm.hpp"  // gsm-модем
#include "rus1.hpp"  // gsm-модем
#include "vl_TreeStructureGenerator_Functions.hpp"// пример // работа с LCD-дисплеем или индикатором РУС1 (LCD-дисплей и 4 кнопками)

  // при старте программы (инициализыция системы, подготовка портов и т.д.)
  void OnProgramStart(void)
  {
    //  Stop watchdog timer to prevent time out reset
#ifndef use_WatchDog_for_PUC
    WDTCTL = WDTPW + WDTHOLD; // потом включаем, в зависимости от use_WatchDog_for_PUC
#endif

    Syst_InitOnStart(); // инициализация системы [периферии, при старте]
    LoadAllData(); // загружаем данные с флешки

    RezhimSpecMessage_On(lineGsmVLeshka, 0, 0); // выводим сообщение о названии прибора
    
    dBeepSignal_Good; // dBeepSignal_LongOk; // dBeepSignal_LongBad; // dBeepSignal_Bad;     
    
    __enable_interrupt(); // разрешаем глобально прерывания
  }
