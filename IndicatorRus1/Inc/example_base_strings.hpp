
// ���� ������ � ������� ��� �������� �������� ������ �� ������ � ������������ ������ ���-1

#ifndef __example_base_strings__
#define __example_base_strings__

#include "rus1_mainTitle.hpp"

#include "rus1_screenLCD_AC-162A.hpp"

// ����� �����
#ifdef verify_FunctionShowInfo_NULL
#define lineBaseStringCnt 89
#else
#define lineBaseStringCnt 74
#endif
extern const unsigned char lineBaseString[lineBaseStringCnt];
// ������ "��������� ������"
extern const unsigned char lineSetupModem[lcdColCount];


// ERROR
#define str_ERROR_Cnt 5
extern const unsigned char str_ERROR[str_ERROR_Cnt];                                                               

// ������ ��� �������� ��/���
#define varsDaNetCnt 2
extern const unsigned char* varsDaNet[varsDaNetCnt];

//////////////////////////////////////// 
// ������������� ���������� �� �����
// ������ ������ (lcdColCount*lcdRowCount).
// ��� ������ ����� �� �����, �� ����������� 
// � ������ ������� ����� ����������� �� 
// ������ ������ �������.
///////////////////////////////////

// "���������"-"��"
extern const unsigned char lineVupolnenoOk[lcdColCount*lcdRowCount];

// "��������"-"��"
extern const unsigned char lineCancelOk[lcdColCount*lcdRowCount];

// ������������ ������
extern const unsigned char lines_ModemRestart[lcdColCount*lcdRowCount];

// VLeshka-GSM
extern const unsigned char lineGsmVLeshka[lcdColCount*lcdRowCount];

// ��������� "��� ������� �� ���-�����!"
extern const unsigned char lines_NoSimCardNumbers[lcdColCount*lcdRowCount];

// ��������� ������
extern const unsigned char lines_ModemSetupModem[lcdColCount*lcdRowCount];

// ���������� �� �������� ���� ���������� �������� �����
extern const unsigned char lines_Power220_Alarm_PowerOff_Upper[lcdColCount*lcdRowCount];
// ���������� �� �������� ���� ���������� �������� �����
extern const unsigned char lines_Power220_Alarm_PowerOff_Lower[lcdColCount*lcdRowCount];
// ���������� ���������������
extern const unsigned char lines_Power220_Norm[lcdColCount*lcdRowCount];

// ����������� � ����
//////////////////// 

// ����� �� ����� ������������������ � gsm-����
extern const unsigned char line_ModemPbReady_Error[lcdColCount*lcdRowCount];

// ��������� ������
/////////////////

// ����� ��������
extern const unsigned char lines_ModemPowerOn[lcdColCount*lcdRowCount];
// GSM-����� �� ���������
extern const unsigned char lines_GsmModem_Off[lcdColCount*lcdRowCount];
// sim-����� �� ����������
extern const unsigned char lines_sim_Card_Off[lcdColCount*lcdRowCount];



//////////////////////////////////////// 
// ��� ����������� sms-���������
//////////////////////////////////////// 

// ����� ������� �������, �� "�������" ����� �� 2-� ���
#define spravkaPart1Cnt 67 // ������ ���� �� ��������� ������
extern const unsigned char spravkaPart1[spravkaPart1Cnt];
#define spravkaPart2Cnt 57 // ������ ���� �� ��������� ������
extern const unsigned char spravkaPart2[spravkaPart2Cnt];

// ������� "RESTART": ���� ���. ����� 86400�.
#define strCommandRestartCnt 40
extern const unsigned char strCommandRestart[strCommandRestartCnt];

// ���������� ���������������
#define strNapriazhenieNormalizovalosCnt 27
extern const unsigned char strNapriazhenieNormalizovalos[strNapriazhenieNormalizovalosCnt];

// � � ��������� ������
#define strC_Cnt 2
extern const unsigned char strC[strC_Cnt];

// � � ��������� ������
#define strB_cnt 2
extern const unsigned char strB[strB_cnt];

// � � �����. ������ � ��������� ������
#define strB_alarmCnt 3
extern const unsigned char strB_alarm[strB_alarmCnt];

// ������ 
#define strSignalCnt 7
extern const unsigned char strSignal[strSignalCnt];
// % � ��������� ������
#define strPersentCnt 2
extern const unsigned char strPersent[strPersentCnt];

// ��� ������� 
#define strDovNomerovCnt 12
extern const unsigned char strDovNomerov[strDovNomerovCnt];

// ���� ��������� � ��������� ������
#define strVukluchenoCnt 15
//                                                       0                   5       7           10                  15 
extern const unsigned char strVuklucheno[strVukluchenoCnt];
// ���� ��������� � ��������� ������
#define strVkluchenoCnt 14
extern const unsigned char strVklucheno[strVkluchenoCnt];

// ������ ���� ���� ������
#define strSecundBuloVushePorogaCnt 19
extern const unsigned char strSecundBuloVushePoroga[strSecundBuloVushePorogaCnt];

// ������ ���� ���� ������
#define strSecundBuloNizhePorogaCnt 19
extern const unsigned char strSecundBuloNizhePoroga[strSecundBuloNizhePorogaCnt];

// ������:
#define strBalansCnt 6
extern const unsigned char strBalans[strBalansCnt];

// ������� "���������" ���������
#define smsCommand_220_Off_VupolnenaCnt 30
extern const unsigned char smsCommand_220_Off_Vupolnena[smsCommand_220_Off_VupolnenaCnt];

// ������� "��������" ���������
#define smsCommand_220_On_VupolnenaCnt 29
extern const unsigned char smsCommand_220_On_Vupolnena[smsCommand_220_On_VupolnenaCnt];



//////////////////////////////////////////// 
// ��� ������� ������ � ����������� �����
///////////////////////////////////////////

// ������� �������� ������
#define simbolsLineBreakCnt 4
extern const unsigned char simbolsLineBreak[simbolsLineBreakCnt];


/////////////////////////
// ���������� sms-������� ������
// ������� � ������ ���
////////////////////// 

/////////////////////////////////
// ���������� � ��������� ��������
#define smsCommand_220_OffThenOnCnt 7
//                                                                             0                   5       7           10                  15 
// extern const unsigned char smsCommand_220_OffThenOn[smsCommand_220_OffThenOnCnt] = {'R','E','S','T','A','R','T'};
#define smsCommand_220_OffThenOn (&spravkaPart1[29])
// ��� ��������� ���������� ��������
#define smsCommand_220_OffThenOn_RusCaptionCnt 12
extern const unsigned char smsCommand_220_OffThenOn_RusCaption[smsCommand_220_OffThenOn_RusCaptionCnt];
// ��� ���������� ��������
#define smsCommand_220_OffThenOn_RusCnt 4
#define smsCommand_220_OffThenOn_Rus smsCommand_220_OffThenOn_RusCaption

// ����������
#define smsCommand_MonitoringCnt 1
extern const unsigned char smsCommand_Monitoring[smsCommand_MonitoringCnt];

/////////////////////////
// UART-������� ������
/////////////////////////

// ������ ����� ���, ����� ����� ������� AT+CMGS=�
#define cmdSendSmsStartCnt 8
extern const unsigned char cmdSendSmsStart[cmdSendSmsStartCnt];

////////////////////////////////////////////////// 
// USSD-������� ������� ������� ��� gsm-����������
/////////////////////////////////////////////////
// ������
#define lineUSSDbalance_BeelineCnt 20
extern const unsigned char lineUSSDbalance_Beeline[lineUSSDbalance_BeelineCnt];
// �������, ���
#define lineUSSDbalance_MegafonAndMtsCnt 20
extern const unsigned char lineUSSDbalance_MegafonAndMts[lineUSSDbalance_MegafonAndMtsCnt];
// ����2
#define lineUSSDbalance_Tele2Cnt 20
extern const unsigned char lineUSSDbalance_Tele2[lineUSSDbalance_Tele2Cnt];

////////////////////////////////////////////////////// 
// ��������� ��������� �� ������ // �� ������ �� �������
////////////////////////////////////////////////////// 

// �������� ����� ������
#define ansModemStartUpCnt 13
extern const unsigned char msgModemStartUp[ansModemStartUpCnt];

// �������� ����������� � gsm-����
#define msgPbReadyCnt 8
extern const unsigned char msgPbReady[msgPbReadyCnt];

// �������� ����������� ������
#define msgRingCnt 4
extern const unsigned char msgRing[msgRingCnt];

// �������� ����������� sms
#define msgSmsCnt 5
extern const unsigned char msgSms[msgSmsCnt];



////////////////////////////////////////////// 
// ��������� ������ ������ �� ��������� �������
////////////////////////////////////////////// 

// ���������� � �������� GSM-������� //// AT+CSQ
#define dataATCSQ_Cnt 7
extern const unsigned char dataATCSQ[dataATCSQ_Cnt];  // ����� AT+ ���������, �.�. �� "���� ���" ���� �����
#define msgLevelCnt 5

// ���������� � gsm-��������� // AT+COPS?
#define msgGsmInfoCnt 6
extern const unsigned char msgGsmInfo[msgGsmInfoCnt];

// ����� �� ussd-������ // AT+CUSD=1,"#102#"
#define msgGsmUssdCnt 6
extern const unsigned char msgGsmUssd[msgGsmUssdCnt];

// ��������� � ���������� ����� // ���������� ������� // AT+CPBS?
#define msgGsmBookInfoCnt 6
extern const unsigned char msgGsmBookInfo[msgGsmBookInfoCnt];

// ��������� ���������� ����� � ���������� ����� // AT+CPBR=1 // ��������� ���������� ������� �� ���-����� � 1
#define msgGsmPhoneNumberCnt 6
extern const unsigned char msgGsmPhoneNumber[msgGsmPhoneNumberCnt];

// ������ ������ // AT+CPAS
#define dataATCPAS_Cnt 8
extern const unsigned char dataATCPAS[dataATCPAS_Cnt];

// ������� ������
#define strPerenosStrokiCnt 1
extern const unsigned char strPerenosStroki[strPerenosStrokiCnt];


/////////////////////////////////////////////////////////////////////
// ��������� gsm, ���������� �������� AT+COPS? // ������� CmdCopsGet()
//////////////////////////////////////////////////////////////////// 

// ������
#define lineCOPS_BeelineCnt 7
extern const unsigned char lineCOPS_Beeline[lineCOPS_BeelineCnt];
// �������
#define lineCOPS_MegafonCnt 11
extern const unsigned char lineCOPS_Megafon[lineCOPS_MegafonCnt];
// ���
#define lineCOPS_MtsCnt 7
extern const unsigned char lineCOPS_Mts[lineCOPS_MtsCnt];
// ����2
#define lineCOPS_Tele2Cnt 5
extern const unsigned char lineCOPS_Tele2[lineCOPS_Tele2Cnt];



/////////////////////////
// ��������� �� ������
///////////////////////

// unsigned char const msgLevel[msgLevelCnt] = {'+','C','S','Q',':'};
#define msgLevel (&dataATCSQ[2])

#define msgGsmStatusCnt 6
// unsigned char const msgGsmStatus[msgGsmStatusCnt] = {'+','C','P','A','S',':'};
#define msgGsmStatus (&dataATCPAS[2])

//////////////////////////
// ���/���������� ��������
//////////////////////////

// ENG ���������� ��������
#define smsCommand_220_OffCnt 3
// const unsigned char smsCommand_220_Off[smsCommand_220_OffCnt] = {'O','F','F'};
#define smsCommand_220_Off (&spravkaPart2[23])

// ��� ���������� ��������
#define smsCommand_220_Off_RusCnt 4
#define smsCommand_220_Off_RusCaptionCnt 9
#define smsCommand_220_Off_Rus (&smsCommand_220_Off_Vupolnena[9])

// ENG ��������� ��������
#define smsCommand_220_OnCnt 2
// const unsigned char smsCommand_220_On[smsCommand_220_OnCnt] = {'O','N'};
#define smsCommand_220_On (&spravkaPart1[65])

// ��� ��������� ��������
#define smsCommand_220_On_RusCnt 3
#define smsCommand_220_On_RusCaptionCnt 8
#define smsCommand_220_On_Rus (&smsCommand_220_On_Vupolnena[9])

// _�_
#define strProbelI_ProbelCnt 3
#define strProbelI_Probel (&spravkaPart1[36])

// ����������
#define strNapriazhenieCnt 11
#define strNapriazhenie strNapriazhenieNormalizovalos

// ���� ������
#define strVushePorogaCnt 12
#define strVushePoroga (&strSecundBuloVushePoroga[7])

// ������������
#define line_ModemRestart (&lines_ModemPbReady_Wait[lcdColCount])

// OK
#define str_OK_Cnt 2
#define str_OK (&lineVupolnenoOk[23])

// ������
#define msgGsmStatus (&dataATCPAS[2])
// ������� �������
#define msgLevelCnt 5
#define msgLevel (&dataATCSQ[2])

// ���� ������
#define strNizhePorogaCnt 12
#define strNizhePoroga (&strSecundBuloNizhePoroga[7])

// _��_
#define strProbelNaProbelCnt 4
#define strProbelNaProbel (&modemUssdWaitAnswer[15])


// ������ "��� GSM-�����"
//                                                       0                   5       7           10                  15 
#define line_ModemNoGsmConnect lineBaseString
// {' ','�','�','�',' ','G','S','M','-','�','�','�','�','�','!',' '}
//   0                   5       7           10                  15 
// ������ "�������:"
// const unsigned char lineLCD_Enter[lcdColCount] =  {' ','�','�','�','�','�','�','�',':',' ',' ',' ',' ',' ',' ',' '};    
#define lineLCD_Enter (&lineBaseString[15])
#define line_Probel (&lineBaseString[24])
// ������ "��"
// const unsigned char strDa[lcdColCount] =          {' ',' ',' ',' ',' ',' ',' ','�','�',' ',' ',' ',' ',' ',' ',' '};
#define strDa (&lineBaseString[33])
// ������ "���"
// const unsigned char strNet[lcdColCount] =         {' ',' ',' ',' ',' ',' ',' ','�','�','�',' ',' ',' ',' ',' ',' '};
#define strNet (&lineBaseString[42])
// ������ "��������:"
// const unsigned char lineLCD_Check[lcdColCount] =  {' ','�','�','�','�','�','�','�','�',':',' ',' ',' ',' ',' ',' '}; 
#define lineLCD_Check (&lineBaseString[58])
#ifdef verify_FunctionShowInfo_NULL
// ������ ��� ������ (NULL == currentTreeNode -> pFunctionShowInfo)
// const unsigned char tree_ShowNULLkatalogString[lcdColCount] = {' ','�','�','�','�','�','�','�',' ','=',' ','N','U','L','L','!'};
#define tree_ShowNULLkatalogString (&lineBaseString[73])
#endif

// ������ vleshka@mail.ru
//                                                    0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
// const unsigned char lineVLeshkaMail[lcdColCount] = {' ','v','l','e','s','h','k','a','@','m','a','i','l','.','r','u'};
// ������ �����
//                                                      0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
// const unsigned char lineVLeshkaAuthor[lcdColCount] = {' ',' ','�','�','�','�','�','�','�','�',' ','�','.','�','.',' '};                                                               


/////////////////////////
// ���������� sms-������� ������
// ������� � ������ ���
////////////////////// 

// ���������� � ��������� ��������
#define smsCommand_220_OffThenOnCnt 7
//                                                                             0                   5       7           10                  15 
// const unsigned char smsCommand_220_OffThenOn[smsCommand_220_OffThenOnCnt] = {'R','E','S','T','A','R','T'};
#define smsCommand_220_OffThenOn (&spravkaPart1[29])
// ��� ��������� ���������� ��������
#define smsCommand_220_OffThenOn_RusCaptionCnt 12

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

#endif // #ifndef __example_base_strings__