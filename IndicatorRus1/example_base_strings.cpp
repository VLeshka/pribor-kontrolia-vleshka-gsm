
// ���� ������ � ������� ��� �������� �������� ������ �� ������ � ������������ ������ ���-1


#include "rus1_screenLCD_AC-162A.hpp"
#include "example_base_strings.hpp"

////////////////////////////////////////////////////////////////////////////////////// 
// ������ ��� ��������� �� �����
// ������ ������ lcdColCount
// ��� ���������� ����������� ����� �� ������ �� ���� ����� ��������, ����� � ���� ������
// �������� 30 ����
////////////////////////////////////////////////////////////////////////////////////// 
#ifdef verify_FunctionShowInfo_NULL
#define lineBaseStringCnt 89
const unsigned char lineBaseString[lineBaseStringCnt] =
// 1               5       7       9   10
{' ','�','�','�',' ','G','S','M','-','�',
 '�','�','�','�','!',' ','�','�','�','�',
 '�','�','�',':',' ',' ',' ',' ',' ',' ',
 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
 '�','�',' ',' ',' ',' ',' ',' ',' ','�',
 '�','�',' ',' ',' ',' ',' ',' ',' ','�',
 '�','�','�','�','�','�','�',':',' ',' ',
 ' ',' ',' ',' ','�','�','�','�','�','�',
 '�',' ','=',' ','N','U','L','L','!'};
#else
#define lineBaseStringCnt 74
const unsigned char lineBaseString[lineBaseStringCnt] =
// 1               5       7       9   10
{' ','�','�','�',' ','G','S','M','-','�',
 '�','�','�','�','!',' ','�','�','�','�',
 '�','�','�',':',' ',' ',' ',' ',' ',' ',
 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
 '�','�',' ',' ',' ',' ',' ',' ',' ','�',
 '�','�',' ',' ',' ',' ',' ',' ',' ','�',
 '�','�','�','�','�','�','�',':',' ',' ',
 ' ',' ',' ',' '};
#endif

// ������ "��������� ������"
const unsigned char lineSetupModem[lcdColCount] = 
// 0                   5       7           10                  15 
{'�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�'};


// ERROR
#define str_ERROR_Cnt 5
const unsigned char str_ERROR[str_ERROR_Cnt] = {'E','R','R','O','R'};                                                               



// ������ ��� �������� ��/���
#define varsDaNetCnt 2
const unsigned char* varsDaNet[varsDaNetCnt] = { &strNet[0], &strDa[0] };

// ������������
#define line_ModemRestart (&lines_ModemPbReady_Wait[lcdColCount])





//////////////////////////////////////// 
// ������������� ���������� �� �����
// ������ ������ (lcdColCount*lcdRowCount).
// ��� ������ ����� �� �����, �� ����������� 
// � ������ ������� ����� ����������� �� 
// ������ ������ �������.
///////////////////////////////////

// "���������"-"��"
const unsigned char lineVupolnenoOk[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ',' ',' ','�','�','�','�','�','�','�','�','�',' ',' ',' ',' ',
 ' ',' ',' ',' ',' ',' ',' ','O','K',' ',' ',' ',' ',' ',' ',' '};

// "��������"-"��"
const unsigned char lineCancelOk[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ',' ',' ',' ','�','�','�','�','�','�','�','�',' ',' ',' ',' ',
 ' ',' ',' ',' ',' ',' ',' ','O','K',' ',' ',' ',' ',' ',' ',' '};

// ������������ ������
const unsigned char lines_ModemRestart[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
// {' ','�','�','�','�','�','�',' ','�','�','�','�','�','�','!',' ',
//  ' ',' ','�','�','�','�','�','�','�','�','�','�','�','�',' ',' '};
{'�','�','�','�','�',' ','�','�',' ','�','�','�','�','�','�','�',
 ' ','�','�','�','�','�','�','�','�','�','�','�','�','.','.',' '};

// VLeshka-GSM
const unsigned char lineGsmVLeshka[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{'�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�',' ',
 ' ',' ','V','L','e','s','h','k','a','-','G','S','M',' ',' ',' '};

// ��������� "��� ������� �� ���-�����!"
const unsigned char lines_NoSimCardNumbers[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','�','�','�',' ','�','�','�','�','�','�','�',' ','�','�',' ',
 ' ',' ',' ','�','�','�','-','�','�','�','�','�','!',' ',' ',' '};

// ��������� ������
const unsigned char lines_ModemSetupModem[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','�','�','�','�','�','�','�','�','�',' ',' ',' ',' ',' ',' ',
 ' ','G','S','M','-','�','�','�','�','�','�','.','.',' ',' ',' '};

// ���������� �� �������� ���� ���������� �������� �����
const unsigned char lines_Power220_Alarm_PowerOff_Upper[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{'�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�','-',
 '�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','�'};
// ���������� �� �������� ���� ���������� �������� �����
const unsigned char lines_Power220_Alarm_PowerOff_Lower[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ',' ',' ','�','�','�','�','�','�','�','�','�','�',' ',' ',' ',
 '�','�','�','�',' ','�','�','�','�','�','�','�','�','�','�','�'};
// ���������� ���������������
const unsigned char lines_Power220_Norm[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ',' ',' ','�','�','�','�','�','�','�','�','�','�',' ',' ',' ',
 '�','�','�','�','�','�','�','�','�','�','�','�','�','�','�',' '};
/*
// ���������� ��������� �� �������� ������ �� gsm-��������� �� ussd-������
//                                                                      0                   5       7           10                  15 
const unsigned char modemUssdWaitAnswer[lcdColCount + lcdColCount] = 
{'�','�','�','�','�','�','�','�',' ','�','�','�','�','�','�',' ',
 '�','�',' ','U','S','S','D','-','�','�','�','�','�','�','.','.'};

// ��������� "������ ����-������"
const unsigned char lines_FlashError[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{'�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','-',
 '�','�',' ','f','l','a','s','h','-','�','�','�','�','�','�','!'};
*/


// ������� � ���-���������
///////////////////////////////
/*
// �������: ���������� � ��������� ��������
const unsigned char lines_220_OffThenOn[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{'�','�','�','�','�','�','�',':',' ','�','�','�','�','�','�','.',
 '�',' ','�','�','�','.',' ','�','�','�','�','�','�','�','�',' '};
// �������: ���������� ��������
//                                                        0                   5       7           10                  15 
const unsigned char lines_220_Off[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','�','�','�','�','�','�','�',':',' ','�','�','�','�','�','-',
 ' ','�','�','�','�','�',' ','�','�','�','�','�','�','�','�',' '};
// �������: ��������� ��������
const unsigned char lines_220_On[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','�','�','�','�','�','�','�',':',' ','�','�','�','�','-',' ',
 ' ','�','�','�','�','�',' ','�','�','�','�','�','�','�','�',' '};
// �������: ������ ����������
// �� ������� ? � �������
const unsigned char lines_220_info[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','�','�','�','�','�','�','�',':',' ','�','�','�','�','�','�',
 ' ',' ',' ','�','�','�','�','�','�','�','�','�','�',' ',' ',' '};
*/


// ����������� � ����
//////////////////// 

// ����� �� ����� ������������������ � gsm-����
const unsigned char line_ModemPbReady_Error[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
// {'G','S','M',' ','�','�',' ','�','�','�','�','�','�','�','�','�',
//  ' ','�','�','�','�','�','�','�','�','�','�','�','�','.','.',' '};
{' ',' ','G','S','M','-','�','�','�','�',' ','�','�',' ',' ',' ',
 ' ',' ','�','�','�','�','�','�','�','�','�','�','!',' ',' ',' ',};

/*
// ����� ������������ � gsm-����
const unsigned char lines_ModemPbReady_Wait[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ',' ','�','�','�','�','�','�','�','�','�','�','�',' ',' ',' ',
 ' ',' ','�',' ','G','S','M','-','�','�','�','�','.','.',' ',' '};
*/



// ��������� ������
/////////////////

// ����� ��������
const unsigned char lines_ModemPowerOn[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','�','�','�','�','�','�','�','�','�',' ',' ',' ',' ',' ',' ',
 ' ','G','S','M','-','�','�','�','�','�','�','.','.',' ',' ',' '};
// GSM-����� �� ���������
const unsigned char lines_GsmModem_Off[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
// {'�','�','�','�','�',' ','�','�',' ','�','�','�','�','-','�','�',
//  ' ','�','�','�','�','�','�','�','�','�','�','�','�','.','.',' '};
{' ',' ','G','S','M','-','�','�','�','�','�',' ','�','�',' ',' ',
 ' ',' ',' ','�','�','�','�','�','�','�','�','�','!',' ',' ',' '};
// sim-����� �� ����������
const unsigned char lines_sim_Card_Off[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
// {' ',' ','S','i','m','-','�','�','�','�','�',' ','�','�',' ',' ',
//  ' ',' ','�','�','�','�','�','�','�','�','�','�','!',' ',' ',' '};
{' ',' ',' ','S','I','M','-','�','�','�','�','�',' ',' ',' ',' ',
 ' ','�','�',' ','�','�','�','�','�','�','�','�','�','�','!',' '};


//////////////////////////////////////// 
// ��� ����������� sms-���������
//////////////////////////////////////// 
/*
// ������ ��� ��� �������������
#define tmpPduCnt 28
const unsigned char tmpPdu[tmpPduCnt] = 
// { 0x00,0x01,0x00,0x0B,0x91,0x97,0x60,0x31,0x10,0x89,0xF0,0x00,0x08,0x00 };
{'0','0','0','1','0','0','0','B','9','1','9','7','6','0','3','1','1','0','8','9','F','0','0','0','0','8','0','0'};
*/
/*
// ������ ������� �������, �� 1 �����
#define spravkaCnt 59 // ������ ���� �� ��������� ������
const unsigned char spravka[spravkaCnt] = 
//     0                   5       7           10                  15 
    {'?',' ','-',' ','�','�','�','�','�','�','�','�','�','�',0x0A,
     'R','E','S','T','A','R','T',' ','-',' ','�','�','�','�',' ','�',' ','�','�','�','.',0x0A,
     'O','N',' ','-',' ','�','�','�','.',0x0A,
     'O','F','F',' ','-',' ','�','�','�','�','.',0x0A,
    };// '�','�','�','�','�','�'};
*/
// ����� ������� �������, �� "�������" ����� �� 2-� ���
#define spravkaPart1Cnt 67 // ������ ���� �� ��������� ������
const unsigned char spravkaPart1[spravkaPart1Cnt] = 
//     0                   5       7           10                  15 
    {'?',' ','-',' ','�','�','�','�','�','�','�','�','�','�',0x0A,
     '�','�','�','�','�','�','�','�','�','�','�','�',',',' ','R',
     'E','S','T','A','R','T',' ','-',' ','�','�','�','�',' ','�',
     ' ','�','�','�','�','�','�','�','�',0x0A,
     '�','�','�','�','�','�','�','�',',',' ','O','N'};
#define spravkaPart2Cnt 57 // ������ ���� �� ��������� ������
const unsigned char spravkaPart2[spravkaPart2Cnt] = 
//     0                   5       7           10                  15 
    {' ','-',' ','�','�','�','�','�','�','�','�',0x0A,
     '�','�','�','�','�','�','�','�','�',',',' ','O','F','F',' ',
     '-',' ','�','�','�','�','�','�','�','�','�',0x0A,
     '�','�','�','�','�','�','�',' ','-',' ','�','�','�','�','�','�','�',0x0A};

/*
#define spravkaCnt 33
const unsigned char spravka[spravkaCnt] = 
// 26{ 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
// 26{ 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
// 33{ '�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�' };
// 33{ '�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�' };

// { '1','.',',','/','?','<','>','`','~','-','_','=','+','\\','|',0x0A,
  // '!','@','#','$','%','^','&','*','(',')',0x0A,
  // '!','"','�',';','%',':','?','*','(',')' };
*/

#define strProbelI_ProbelCnt 3
#define strProbelI_Probel (&spravkaPart1[36])

// ������� "RESTART": ���� ���. ����� 86400�.
#define strCommandRestartCnt 40
const unsigned char strCommandRestart[strCommandRestartCnt] = 
// 0   1                                   10                                          21
{'�','�','�','�','�','�','�',' ','"','�',
 '�','�','�','�','�','�','�','�','�','�',
 '�','"',':',' ','�','�','�','�',' ','�',
 '�','�','.',' ','�','�','�','�','�',' '};

// ���������� ���������������
#define strNapriazhenieNormalizovalosCnt 27
const unsigned char strNapriazhenieNormalizovalos[strNapriazhenieNormalizovalosCnt] = 
// 0                   5       7           10                  15 
{'�','�','�','�','�','�','�','�','�','�',
 ' ','�','�','�','�','�','�','�','�','�',
 '�','�','�','�','�','�',0x0A};

// ����������
#define strNapriazhenieCnt 11
#define strNapriazhenie strNapriazhenieNormalizovalos

// � � ��������� ������
#define strC_Cnt 2
const unsigned char strC[strC_Cnt] = {'�',0x0A};

// � � ��������� ������
#define strB_cnt 2
const unsigned char strB[strB_cnt] = {'�',0x0A};

// � � �����. ������ � ��������� ������
#define strB_alarmCnt 3
const unsigned char strB_alarm[strB_alarmCnt] = {'�','!',0x0A};

// ������ 
#define strSignalCnt 7
const unsigned char strSignal[strSignalCnt] = {'�','�','�','�','�','�',' '};
// % � ��������� ������
#define strPersentCnt 2
const unsigned char strPersent[strPersentCnt] = {'%',0x0A};

// ��� ������� 
#define strDovNomerovCnt 12
const unsigned char strDovNomerov[strDovNomerovCnt] = {'�','�','�',' ','�','�','�','�','�','�','�',' '};
// ������� ������
#define strPerenosStrokiCnt 1
const unsigned char strPerenosStroki[strPerenosStrokiCnt] = {0x0A};

// ���� ��������� � ��������� ������
#define strVukluchenoCnt 15
//                                                       0                   5       7           10                  15 
const unsigned char strVuklucheno[strVukluchenoCnt] = {'�','�','�','�',' ','�','�','�','�','�','�','�','�','�',0x0A};
// ���� ��������� � ��������� ������
#define strVkluchenoCnt 14
const unsigned char strVklucheno[strVkluchenoCnt] =   {'�','�','�','�',' ','�','�','�','�','�','�','�','�',0x0A};

// ������ ���� ���� ������
#define strSecundBuloVushePorogaCnt 19
const unsigned char strSecundBuloVushePoroga[strSecundBuloVushePorogaCnt] = 
// 0                   5       7           10                  15                  20
{'�',' ','�','�','�','�',' ','�','�','�','�',' ','�','�','�','�','�','�',' '};
#define strVushePorogaCnt 12
#define strVushePoroga (&strSecundBuloVushePoroga[7])

// ������ ���� ���� ������
#define strSecundBuloNizhePorogaCnt 19
const unsigned char strSecundBuloNizhePoroga[strSecundBuloNizhePorogaCnt] = 
// 0                   5       7           10                  15                  20
{'�',' ','�','�','�','�',' ','�','�','�','�',' ','�','�','�','�','�','�',' '};
#define strNizhePorogaCnt 12
#define strNizhePoroga (&strSecundBuloNizhePoroga[7])

// ������:
#define strBalansCnt 6
const unsigned char strBalans[strBalansCnt] = {'�','�','�','�','�','�'};

#define strProbelNaProbelCnt 4
#define strProbelNaProbel (&modemUssdWaitAnswer[15])

// ������� "���������" ���������
#define smsCommand_220_Off_VupolnenaCnt 30
const unsigned char smsCommand_220_Off_Vupolnena[smsCommand_220_Off_VupolnenaCnt] =
// 0                   5       7           10                  15 
{'�','�','�','�','�','�','�',' ','"','�',
 '�','�','�','�','�','�','�','�','"',' ',
 '�','�','�','�','�','�','�','�','�',0x0A};

// ������� "��������" ���������
#define smsCommand_220_On_VupolnenaCnt 29
const unsigned char smsCommand_220_On_Vupolnena[smsCommand_220_On_VupolnenaCnt] =
// 0                   5       7           10                  15 
{'�','�','�','�','�','�','�',' ','"','�',
 '�','�','�','�','�','�','�','"',' ','�',
 '�','�','�','�','�','�','�','�',0x0A};




//////////////////////////////////////////// 
// ��� ������� ������ � ����������� �����
///////////////////////////////////////////

// ������� �������� ������
#define simbolsLineBreakCnt 4
const unsigned char simbolsLineBreak[simbolsLineBreakCnt] = {0, 13, 10, ' '};





/////////////////////////
// ���������� sms-������� ������
// ������� � ������ ���
////////////////////// 

/////////////////////////////////
// ���������� � ��������� ��������
#define smsCommand_220_OffThenOnCnt 7
//                                                                             0                   5       7           10                  15 
// const unsigned char smsCommand_220_OffThenOn[smsCommand_220_OffThenOnCnt] = {'R','E','S','T','A','R','T'};
#define smsCommand_220_OffThenOn (&spravkaPart1[29])
// ��� ��������� ���������� ��������
#define smsCommand_220_OffThenOn_RusCaptionCnt 12
//                                                                                                   0                   5       7           10                  15 
const unsigned char smsCommand_220_OffThenOn_RusCaption[smsCommand_220_OffThenOn_RusCaptionCnt] = {'�','�','�','�','�','�','�','�','�','�','�','�'};
// ��� ���������� ��������
#define smsCommand_220_OffThenOn_RusCnt 4
#define smsCommand_220_OffThenOn_Rus smsCommand_220_OffThenOn_RusCaption

/////////////////////////
// ���������� ��������

// ENG ���������� ��������
#define smsCommand_220_OffCnt 3
// const unsigned char smsCommand_220_Off[smsCommand_220_OffCnt] = {'O','F','F'};
#define smsCommand_220_Off (&spravkaPart2[23])

// ��� ���������� ��������
#define smsCommand_220_Off_RusCnt 4
#define smsCommand_220_Off_RusCaptionCnt 9
#define smsCommand_220_Off_Rus (&smsCommand_220_Off_Vupolnena[9])

////////////////////////// 
// ��������� ��������

// ENG ��������� ��������
#define smsCommand_220_OnCnt 2
// const unsigned char smsCommand_220_On[smsCommand_220_OnCnt] = {'O','N'};
#define smsCommand_220_On (&spravkaPart1[65])

// ��� ��������� ��������
#define smsCommand_220_On_RusCnt 3
#define smsCommand_220_On_RusCaptionCnt 8
#define smsCommand_220_On_Rus (&smsCommand_220_On_Vupolnena[9])

////////////////////// 
// ����������

#define smsCommand_MonitoringCnt 1
//                                                                       0                   5       7           10                  15 
const unsigned char smsCommand_Monitoring[smsCommand_MonitoringCnt] = {'?'};


/*�� ������� ����� �� ������
//////////////// 
// �������� �����
#define smsCommand_AddPhoneCnt 3
//                                                         0                   5       7           10                  15 
const unsigned char smsCommand_AddPhone[smsCommand_AddPhoneCnt] = {'A','D','D'};

///////////////
// ������� �����
#define smsCommand_DeletePhoneCnt 6
//                                                            0                   5       7           10                  15 
const unsigned char smsCommand_DeletePhone[smsCommand_DeletePhoneCnt] = {'D','E','L','E','T','E'};
*/




/////////////////////////////////////////////////////////////////////
// ��������� gsm, ���������� �������� AT+COPS? // ������� CmdCopsGet()
//////////////////////////////////////////////////////////////////// 

// ������
#define lineCOPS_BeelineCnt 7
//                                                              0                   5       7           10                  15 
const unsigned char lineCOPS_Beeline[lineCOPS_BeelineCnt] = { 'B','e','e','l','i','n','e'};
// �������
#define lineCOPS_MegafonCnt 11
//                                                              0                   5       7           10                  15 
const unsigned char lineCOPS_Megafon[lineCOPS_MegafonCnt] = { 'M','e','g','a','F','o','n',' ','R','U','S'};
// ���
#define lineCOPS_MtsCnt 7
//                                                              0                   5       7           10                  15 
const unsigned char lineCOPS_Mts[lineCOPS_MtsCnt] =         { 'M','T','S','-','R','U','S'};
// ����2
#define lineCOPS_Tele2Cnt 5
//                                                              0                   5       7           10                  15 
const unsigned char lineCOPS_Tele2[lineCOPS_Tele2Cnt] =     { 'M','O','T','I','V'};



/////////////////////////
// UART-������� ������
/////////////////////////

// ������ ����� ���, ����� ����� ������� AT+CMGS=�
#define cmdSendSmsStartCnt 8
unsigned char const cmdSendSmsStart[cmdSendSmsStartCnt] = 
// 0   1                                   10                                          21
{'A','T','+','C','M','G','S','='};

////////////////////////////////////////////////// 
// USSD-������� ������� ������� ��� gsm-����������
/////////////////////////////////////////////////
// ������
#define lineUSSDbalance_BeelineCnt 20
//                                                                            0                   5       7           10                  15              19
const unsigned char lineUSSDbalance_Beeline[lineUSSDbalance_BeelineCnt] = { 'A','T','+','C','U','S','D','=','1',',','"','#','1','0','2','#','"',',','1','5'};
// �������, ���
#define lineUSSDbalance_MegafonAndMtsCnt 20
//                                                                                        0                   5       7           10                  15              19
const unsigned char lineUSSDbalance_MegafonAndMts[lineUSSDbalance_MegafonAndMtsCnt] = { 'A','T','+','C','U','S','D','=','1',',','"','*','1','0','0','#','"',',','1','5'};
// ����2
#define lineUSSDbalance_Tele2Cnt 20
//                                                                            0                   5       7           10                  15              19
const unsigned char lineUSSDbalance_Tele2[lineUSSDbalance_Tele2Cnt] = { 'A','T','+','C','U','S','D','=','1',',','"','#','1','0','5','#','"',',','1','5'};

// ������� ������� ������ ����� pdu
// #define lineUSSDbalance_MegafonCnt 25
// const unsigned char lineUSSDbalance_Megafon[lineUSSDbalance_MegafonCnt] = { 'A','T','+','C','U','S','D','=','1',',','"','A','A','1','8','0','C','3','6','0','2','"',',','1','5'};
// #define lineUSSDbalance_MegafonCnt 35
// const unsigned char lineUSSDbalance_Megafon[lineUSSDbalance_MegafonCnt] = 
// 0                   5       7           10                  15                  20              24                                      34
// {'A','T','+','C','U','S','D','=','1',',','"','0','0','2','A','0','0','3','1','0','0','3','0','0','0','3','0','0','0','2','3','"',',','1','5'};




////////////////////////////////////////////////////// 
// ��������� ��������� �� ������ // �� ������ �� �������
////////////////////////////////////////////////////// 

// �������� ����� ������
#define ansModemStartUpCnt 13
//                                                           0   1                                           12
unsigned char const msgModemStartUp[ansModemStartUpCnt] = {'M','O','D','E','M',':','S','T','A','R','T','U','P'};

// �������� ����������� � gsm-����
#define msgPbReadyCnt 8
unsigned char const msgPbReady[msgPbReadyCnt] = {'+','P','B','R','E','A','D','Y'};

// �������� ����������� ������
#define msgRingCnt 4
unsigned char const msgRing[msgRingCnt] = {'R','I','N','G'};

// �������� ����������� sms
#define msgSmsCnt 5
unsigned char const msgSms[msgSmsCnt] = {'+','C','M','T',':'};



////////////////////////////////////////////// 
// ��������� ������ ������ �� ��������� �������
////////////////////////////////////////////// 

// 


// ���������� � �������� GSM-������� //// AT+CSQ
#define dataATCSQ_Cnt 7
unsigned char const dataATCSQ[dataATCSQ_Cnt] = {'A','T','+','C','S','Q',':'};  // ����� AT+ ���������, �.�. �� "���� ���" ���� �����
#define msgLevelCnt 5
// unsigned char const msgLevel[msgLevelCnt] = {'+','C','S','Q',':'};
#define msgLevel (&dataATCSQ[2])

// ���������� � gsm-��������� // AT+COPS?
#define msgGsmInfoCnt 6
unsigned char const msgGsmInfo[msgGsmInfoCnt] = {'+','C','O','P','S',':'};

// ����� �� ussd-������ // AT+CUSD=1,"#102#"
#define msgGsmUssdCnt 6
unsigned char const msgGsmUssd[msgGsmUssdCnt] = {'+','C','U','S','D',':'};

// ��������� � ���������� ����� // ���������� ������� // AT+CPBS?
#define msgGsmBookInfoCnt 6
unsigned char const msgGsmBookInfo[msgGsmBookInfoCnt] = {'+','C','P','B','S',':'};

// ��������� ���������� ����� � ���������� ����� // AT+CPBR=1 // ��������� ���������� ������� �� ���-����� � 1
#define msgGsmPhoneNumberCnt 6
unsigned char const msgGsmPhoneNumber[msgGsmPhoneNumberCnt] = {'+','C','P','B','R',':'};

// ������ ������ // AT+CPAS
#define dataATCPAS_Cnt 8
unsigned char const dataATCPAS[dataATCPAS_Cnt] = {'A','T','+','C','P','A','S',':'};
#define msgGsmStatusCnt 6
// unsigned char const msgGsmStatus[msgGsmStatusCnt] = {'+','C','P','A','S',':'};
#define msgGsmStatus (&dataATCPAS[2])