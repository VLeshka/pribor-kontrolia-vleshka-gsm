
// .h-���� ������ �� ������ � LCD-������� 

#ifndef __rus1_screenLCD_AC_162A__
#define __rus1_screenLCD_AC_162A__

#include "rus1_mainTitle.hpp"

#include "example_base_functions.hpp"

// ��� ������������� � ������� ������������ ���������� ������
extern unsigned char* pLcdText;       
// ����� ��� ������������ ��������� � pLcdText
// ����� ����� �� ����������� � ������� LCD-������ � ����� LCD (����� ������ ��� �������), � ����� ������� ������
extern unsigned char* pLcdTextBuffer; 
// ��������� ������, ���������� �� ������� 
// ��������� �������������� ����� ������� ������ �� �������
extern unsigned char* pLastSimbolLCD; // ����� ������ ��� �� �������� ������� ����������������

// ������� ��� �������� �������� ������� ���� � ��������� ������� �������� lcd-������� ���� AC-162A
// ��� ������� ���������� ������� ��������� ������� ���� ����� ����������
// 0x5F = '_'
extern const unsigned char lcd_tab_rus[192+64];

// ���������� ����� � �������� �������

// ��� � ������� rus1_*.* ������ ��� 2-�������� ������� (��������� ��� ����������� ���������� ����� lcd-�������: 
// ������ ������ - ��������� ����������, ������ ������ - � ��������). 
// �� �������� lcd-������, �������� ����� �������� ������������ ���������� �������� �������.

#define	lcdRowCount	2	// ������������ ���������� ����� ����������
#define	lcdColCount	16	// ������������ ���������� �������� � ������� ����������




// ��������� �������� ��� ������ � lcd-��������
#define	lcdCmdWriteDataToRamDelay 40  // �������� ��� ���������� Write Data to RAM ��� �������, ���
#define	lcdDataWriteDelay         40  // �������� ��� ���������� Write Data to RAM ��� ��������, ���

// f_lcdText
#define GetLcdText f_lcdText
#define pLcdBegin (&f_lcdText[0][0])

// f_lcdTextBuffer
#define GetLcdTextBuffer f_lcdTextBuffer
// ������ � ������������ � � ������ 0
#define GetCharInRow0 f_lcdTextBuffer[0]
// ������ � ������������ � � ������ 1
#define GetCharInRow1 f_lcdTextBuffer[1]
#define pLcdBufBegin (&f_lcdTextBuffer[0][0])
#define pLcdBufEnd (&f_lcdTextBuffer[lcdRowCount-1][lcdColCount-1])



/////////////////////////
// ���������� LCD-��������
/////////////////////////

// ������ �� ����� �
#define dLcdE_On {portLcdOutE |= portLcdPinE;}
#define dLcdE_Off {portLcdOutE &= ~ portLcdPinE;}

// ������� 4 ���� 8-������� ������� // ���������� ������ ������ 4 ���� ����� (7 6 5 4 3 2 1 0)
#define dLcd_SetData(x) {portLcdOutData = ((portLcdOutData & 0xF0) | (x & 0x0F));}

// ������ �� ����� RS
#define dLcdRS(x) {if (0==x) portLcdOutRS &= ~ portLcdPinRS; \
      else portLcdOutRS |= portLcdPinRS;}
#define dLcdRsOff {portLcdOutRS &= ~ portLcdPinRS;}
#define dLcdRsOn  {portLcdOutRS |= portLcdPinRS;}
#define dLcdRsInvert  {portLcdOutRS ^= portLcdPinRS;}



/////////////////////////////////////////
// ��� �������� ��� ������ � ������ ������
/////////////////////////////////////////

// f_lcdTextBuffer
#define pLcdTextBuffer_Reset { \
   for (unsigned char* pChar=pLcdBufBegin; pChar<(pLcdBufBegin + (lcdColCount*lcdRowCount)); pChar++) \
        *pChar = 0; }


// f_lcdText
#define dLcdText_Reset { \
   for (unsigned char* pChar=pLcdBegin; pChar<(pLcdBegin + (lcdColCount*lcdRowCount)); pChar++) \
        *pChar = 0; }



// ��������� ����� � ��������� �������
#define dLCD_FigureToLcdChar( val ) (val + 0x30)

// ������� �������
// #define dLCD_Clear { for (unsigned char* pTemp = pLcdBufBegin; pTemp<pLcdBufBegin+(lcdColCount*lcdRowCount); pTemp++) *pTemp = ' '; }

// ��������� ����� � ������� � ������ 1 (��������� � ����)
#define dInsertInRow1toX(x, s) {*(pLcdBufBegin + lcdColCount + x) = s; }








////////////////////////////////////////////////////////////////////////////// 
// �����, ��, ��� ������������ TIMING CHARACTERISTICS ��� ������ � lcd-��������
////////////////////////////////////////////////////////////////////////////// 


// ����� tAH = 20 ��
// ��� 16��� ���������� ������: � 1 ��� 16 ������, � 1 ����� 62,5 �� // ������ ����������
#define dDelayTAH { __no_operation(); }

// ���� ���������� ���������� �������, �� ����� ���������� � ��������� �� ������
// ���� ���������� ���������� �������, �� ����� ����� �� ������������, �.�. �� �������� 
//      �������� ������ ������� ����� �������
#ifdef use_Constant_FreqCPU

// ����� tAS = 60 ��
// ��� 16��� ���������� ������: � 1 ��� 16 ������, � 1 ����� 62,5 �� // ������ ����������
#if (GetCpuFreqMHz > 16)
  #define dDelayTAS { __delay_cycles(2); }
#else
  #define dDelayTAS { __no_operation(); }
#endif



// ����� PWEH = 450 ��
// 
// ��� 16��� ���������� ������: � 1 ��� 16 ������, � 1 ����� 62,5 �� // ������ ����������
// ��� 16��� 450/62.5 = 7.2 ����� => ����� 8 ������
// ��� 1��� � 1 ����� 1 ��� = 1000 �� 450/1000 = 0.45 ����� => ����� 1 ����
///////////////////////////////////////////////////////////////////////////////////////

// ��� 16��� 7.20 ������
// ��� 15��� 6.75 ������
// ��� 14��� 6.30 ������
// ��� 13��� 5.85 ������
// ��� 12��� 5.40 ������
// ��� 11��� 4.95 ������
// ��� 10��� 4.50 ������
// ��� 9��� 4.05 ������
// ��� 8��� 3.60 ������
// ��� 7��� 3.15 ������
// ��� 6��� 2.70 ������
// ��� 5��� 2.25 ������
// ��� 4��� 1.80 ������
// ��� 3��� 1.35 ������
// ��� 2��� 0.90 ������
// ��� 1��� 0.45 ������
#ifdef use_WatchDog_for_PUC

#if (GetCpuFreqMHz>15)
#define dDelayPWEN { __delay_cycles(3); \
                     dWatchDogReset; } // 8 ������
#elif (GetCpuFreqMHz>13)
#define dDelayPWEN { __delay_cycles(2); \
                     dWatchDogReset; } // 7 ������
#elif (GetCpuFreqMHz>11)
#define dDelayPWEN { __no_operation(); \
                     dWatchDogReset; } // 6 ������
#elif (GetCpuFreqMHz>8)
#define dDelayPWEN { dWatchDogReset; } // 5 ������
#elif (GetCpuFreqMHz>6)
#define dDelayPWEN { __delay_cycles(4); } // 4 �����
#elif (GetCpuFreqMHz>4)
#define dDelayPWEN { __delay_cycles(3); } // 3 �����
#elif (GetCpuFreqMHz>2)
#define dDelayPWEN { __delay_cycles(2); } // 2 �����
#else
#define dDelayPWEN { __no_operation(); } // 1 ����
#endif

#else // #ifdef use_WatchDog_for_PUC

#if (GetCpuFreqMHz>15)
#define dDelayPWEN { __delay_cycles(8); } // 8 ������
#elif (GetCpuFreqMHz>13)
#define dDelayPWEN { __delay_cycles(7); } // 7 ������
#elif (GetCpuFreqMHz>11)
#define dDelayPWEN { __delay_cycles(6); } // 6 ������
#elif (GetCpuFreqMHz>8)
#define dDelayPWEN { __delay_cycles(5); } // 5 ������
#elif (GetCpuFreqMHz>6)
#define dDelayPWEN { __delay_cycles(4); } // 4 �����
#elif (GetCpuFreqMHz>4)
#define dDelayPWEN { __delay_cycles(3); } // 3 �����
#elif (GetCpuFreqMHz>2)
#define dDelayPWEN { __delay_cycles(2); } // 2 �����
#else
#define dDelayPWEN { __no_operation(); } // 1 ����
#endif

#endif // #ifdef use_WatchDog_for_PUC .. #else

// ����� TcycE
// ����� ���� ������ � �������� �� 1000��, ��� = 520 ��
// ��� 16��� ���������� ������: � 1 ��� 16 ������, � 1 ����� 62,5 �� // ������ ����������
// ��� 16��� 520/62.5 = 8.32 ����� => ������ ����� 9 ������
// ��� 1��� � 1 ����� 1 ��� = 1000 �� 520/1000 = 0.52 ����� => ����� 1 ����
///////////////////////////////////////////////////////////////////////////////////////

// ��� 16��� 8.32 ������
// ��� 15��� 7.80 ������
// ��� 14��� 7.28 ������
// ��� 13��� 6.76 ������
// ��� 12��� 6.24 ������
// ��� 11��� 5.72 ������
// ��� 10��� 5.20 ������
// ��� 9��� 4.68 ������
// ��� 8��� 4.16 ������
// ��� 7��� 3.64 ������
// ��� 6��� 3.12 ������
// ��� 5��� 2.60 ������
// ��� 4��� 2.08 ������
// ��� 3��� 1.56 ������
// ��� 2��� 1.04 ������
// ��� 1��� 0.52 ������
#ifdef use_WatchDog_for_PUC

#if (GetCpuFreqMHz>15)
#define dDelayToCycleE { __delay_cycles(4); \
                         dWatchDogReset; }     // 9 ������
#elif (GetCpuFreqMHz>13)
#define dDelayToCycleE { __delay_cycles(3); \
                         dWatchDogReset; }     // 8 ������
#elif (GetCpuFreqMHz>11)
#define dDelayToCycleE { __delay_cycles(2); \
                         dWatchDogReset; }     // 7 ������
#elif (GetCpuFreqMHz>9)
#define dDelayToCycleE { __no_operation(); \
                         dWatchDogReset; }     // 6 ������
#elif (GetCpuFreqMHz>7)
#define dDelayToCycleE { dWatchDogReset; }      // 5 ������
#elif (GetCpuFreqMHz>5)
#define dDelayToCycleE { __delay_cycles(4); } // 4 �����
#elif (GetCpuFreqMHz>3)
#define dDelayToCycleE { __delay_cycles(3); } // 3 �����
#elif (GetCpuFreqMHz>1)
#define dDelayToCycleE { __delay_cycles(2); } // 2 �����
#else
#define dDelayToCycleE { __no_operation(); }  // 1 ����
#endif

#else // #ifdef use_WatchDog_for_PUC

#if (GetCpuFreqMHz>15)
#define dDelayToCycleE { __delay_cycles(9); } // 9 ������
#elif (GetCpuFreqMHz>13)
#define dDelayToCycleE { __delay_cycles(8); } // 8 ������
#elif (GetCpuFreqMHz>11)
#define dDelayToCycleE { __delay_cycles(7); } // 7 ������
#elif (GetCpuFreqMHz>9)
#define dDelayToCycleE { __delay_cycles(6); } // 6 ������
#elif (GetCpuFreqMHz>7)
#define dDelayToCycleE { __delay_cycles(5); } // 5 ������
#elif (GetCpuFreqMHz>5)
#define dDelayToCycleE { __delay_cycles(4); } // 4 �����
#elif (GetCpuFreqMHz>3)
#define dDelayToCycleE { __delay_cycles(3); } // 3 �����
#elif (GetCpuFreqMHz>1)
#define dDelayToCycleE { __delay_cycles(2); } // 2 �����
#else
#define dDelayToCycleE { __no_operation(); }  // 1 ����
#endif

#endif // #ifdef use_WatchDog_for_PUC .. #else

#else // #iddef use_Constant_FreqCPU

// ����� tAS = 60 ��
// ��� 16��� ���������� ������: � 1 ��� 16 ������, � 1 ����� 62,5 �� // ������ ����������
#define dDelayTAS { __delay_cycles(1); }



// ����� PWEH = 450 ��
// 
// ��� 16��� ���������� ������: � 1 ��� 16 ������, � 1 ����� 62,5 �� // ������ ����������
// ��� 16��� 450/62.5 = 7.2 ����� => ����� 8 ������
#ifdef use_WatchDog_for_PUC
#define dDelayPWEN { __delay_cycles(3); \
                     dWatchDogReset; } // 8 ������
#else // #ifdef use_WatchDog_for_PUC
#define dDelayPWEN { __delay_cycles(8); } // 8 ������
#endif // #ifdef use_WatchDog_for_PUC .. #else

// ����� TcycE
// ����� ���� ������ � �������� �� 1000��, ��� = 520 ��
// ��� 16��� ���������� ������: � 1 ��� 16 ������, � 1 ����� 62,5 �� // ������ ����������
// ��� 16��� 520/62.5 = 8.32 ����� => ������ ����� 9 ������
#ifdef use_WatchDog_for_PUC
#define dDelayToCycleE { __delay_cycles(4); \
                         dWatchDogReset; }     // 9 ������
#else // #ifdef use_WatchDog_for_PUC
#define dDelayToCycleE { __delay_cycles(9); } // 9 ������
#endif

#endif // #ifdef use_Constant_FreqCPU

// ����� ����� ������ ���������� �������
// �� ������������: Wait for more than 30ms after Vdd rises to 4.5V
// �� ��������: ��� 3.3� 30�� ����, ������� 100��.
#define dDelayBeforeFirstStart { DelayMilliS(100); }

// ������� ������ �� LCD-������� ����������� ������
// ��������� ����� �� �����������, �.�. ������ � LCD-�������� - ������ � ����. ���������� �������
#define dLCD_WriteCharQuickly(data) { \
        dLcdRsOn; \
        dDelayTAS; \
        \
        dLcdE_On; \
        __delay_cycles(5); \
        dLcd_SetData(data >> 4); \
        __delay_cycles(4); \
        dLcdE_Off; \
        \
        dDelayToCycleE; \
        \
        dLcdE_On; \
        __delay_cycles(5); \
        dLcd_SetData(data&0x0F); \
        __delay_cycles(4); \
        dLcdE_Off; \
        }

// ��� ������� Write Data To Ram
// ��� ������� �� �������
#define dLCD_Cmd( x, y ) { dLCD_WriteCommandQuickly(x); DelayMicroS(y); }


// ���������� ��� ����� � ������� �������� LCD-�����������
#define dLCD_NumberToCharHex( val ) (numbersForCharHex[val])

#define LCD_ShowLong( val, xCoord, yCoord ) \
  LongToStr(val, &GetLcdTextBuffer[yCoord][xCoord], lcdColCount-xCoord)


//////// 
// extern
////////     

// ���������� ����� �� �� ����������
extern unsigned char f_lcdText[lcdRowCount][lcdColCount];

// ����� ������ ������ �� �� ��������� [����� ������ �����]
// ����� ����� �� ����������� LCD ������ � �������� � ����� LCD, ����� ������ ��� �������, � ����� ������� ������
extern unsigned char f_lcdTextBuffer[lcdRowCount][lcdColCount];



// ������� ������� // ��� �������� ����� �� ������
void LCD_Clear();

// ������� ������� �� LCD-������� ����������� ������
// ��������� ����� �� �����������, �.�. ������ � LCD-�������� - ������ � ����. ���������� �������
void LCD_WriteCommandQuickly(unsigned char data);

    // ���������� ��� ����� � ������� �������� LCD-�����������
    // 0x00 - 0x0F ��������� � ANSI-1251
unsigned char LCD_NumberToCharHex(unsigned char val);
    
// ������� ������ � ����������� unsigned char xCoord, unsigned char yCoord
// ���� ������� �� ��������� ������ - ����������, ����� ����������� �� ������ ������
// ���������� ���, ��� 2-�������� �������
void LCD_ShowString(unsigned char const* dataVal, unsigned char val_len, unsigned char xCoord = 0, unsigned char yCoord = 0);
// void LCD_ShowString_(unsigned char const* dataVal, unsigned char val_len, unsigned char* dataXCoordBegin);
// #define LCD_ShowString0(p,pl,x) LCD_ShowString_(p,pl,(pLcdBufBegin+x))
// #define LCD_ShowString1(p,pl,x) LCD_ShowString_(p,pl,(pLcdBufBegin+x+lcdColCount))
// #define LCD_ShowString(p,pl,x,y) LCD_ShowString_(p,pl,(pLcdBufBegin+x+(y*lcdColCount)))


    // ������� float �� ������ � �������� ����������; ���� ������� �� ������ - ����������
    //                   ��������    ����� ���� � ����. �����  ���������� �          ���������� Y          ���� �� ������� �� ����� ������
// void LCD_ShowFloat( float fval,  unsigned char frac_Count, unsigned char xCoord, unsigned char yCoord, bool isClearToEndOfLine = false );

// ��� ���������� ������� ����
// ��������� ��������� �����, ������ ������ ������� [lcdColCount * lcdRowCount]
// ������ ����������� ������ ���� ������� [lcdColCount * lcdRowCount] !
// void LCD_FullUpdateFromString( const unsigned char* dataVal );

#endif // #ifndef __rus1_screenLCD_AC_162A__