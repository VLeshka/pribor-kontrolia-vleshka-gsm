//////////////////////////////////////////// 
// ������ ��� �������� ���������� �������
//////////////////////////////////////////// 

#ifndef __rus1_interruptLcdTimer__
#define __rus1_interruptLcdTimer__


// ������� 10 ��
#define lcd_Is10ms   ( (0!=(timeMask & BIT0)) )
// ������� 100 ��
#define lcd_Is100ms  ( (0!=(timeMask & BIT1)) )
// ������� 1 �
#define lcd_Is1s     ( (0!=(timeMask & BIT2)) )
// ������� 1 ���
#define lcd_Is1min   ( (0!=(timeMask & BIT3)) )
// ������� 1 ���
#define lcd_Is1hour  ( (0!=(timeMask & BIT4)) )
// ������� 1 ���� = 24 ����
#define lcd_Is1day   ( (0!=(timeMask & BIT5)) )
// ������� 1 �����
#define lcd_Is1month ( (0!=(timeMask & BIT6)) )
// ������� 1 ���
#define lcd_Is1year  ( (0!=(timeMask & BIT7)) )

#endif // __rus1_interruptLcdTimer__