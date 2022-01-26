
// В мк msp430G2553:
// - приоритет вектора прерывания ADC10 меньше, чем у UART.
// - приоритет вектора прерывания UART меньше, чем у таймеров Timer0 и Timer1.

#include "msp430G2553.h"

#include "rus1_mainTitle.hpp"          // инструкция, указания режима работы прибора

#include "example_work.hpp"            // пример //основной неспециализированный рабочий модуль
#include "example_base_functions.hpp"

int main( void )
{   
  OnProgramStart();
  while (true)
    dWatchDogReset;
}