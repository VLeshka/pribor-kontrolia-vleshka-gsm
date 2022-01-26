//////////////////////////////////////////// 
// модуль для описания прерывания таймера
//////////////////////////////////////////// 

#ifndef __rus1_interruptLcdTimer__
#define __rus1_interruptLcdTimer__


// событие 10 мс
#define lcd_Is10ms   ( (0!=(timeMask & BIT0)) )
// событие 100 мс
#define lcd_Is100ms  ( (0!=(timeMask & BIT1)) )
// событие 1 с
#define lcd_Is1s     ( (0!=(timeMask & BIT2)) )
// событие 1 мин
#define lcd_Is1min   ( (0!=(timeMask & BIT3)) )
// событие 1 час
#define lcd_Is1hour  ( (0!=(timeMask & BIT4)) )
// событие 1 день = 24 часа
#define lcd_Is1day   ( (0!=(timeMask & BIT5)) )
// событие 1 месяц
#define lcd_Is1month ( (0!=(timeMask & BIT6)) )
// событие 1 год
#define lcd_Is1year  ( (0!=(timeMask & BIT7)) )

#endif // __rus1_interruptLcdTimer__