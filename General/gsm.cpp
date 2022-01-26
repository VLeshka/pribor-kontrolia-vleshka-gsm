// ������ � gsm

// ������������ GSM-������ (������) NEOWAY M590:
// NEOWAY M590 ����� ������ �������� ��� ���� ��������� ����������.
// 1. �� ��������� ����� ������������ ��� ���, ������� ����������� ���.
// 2. ��� ���������� ��� ������������� �������� �������, ��� ����� ������ ������ �����.

// ������, ��������� �� uart �� ������, �������������� ��:
//  - "�����"     - ����� ������ �� ������ � ���������, ��������� ���, ��� "�����" (����������� 
//    ����� OK �� ����� ������).
//  - "���������" - ����� ������ �� ������ � �� ��������� ��� "�����" (��� OK �� ����� ������).

// ������ ������ ������ � ������� 
// (��������� �� �.1 �� �����; ���� �� ������ ���-������ ��������� ����� - ������������ ������):
// 1. �������� �������, ������ �������� �� ������ ��������� "MODEM:STARTUP".
// 2. ������ �������� �� ������ ��������� "+PBREADY".
// 3. ������ ������ ������� ������ dSetFuncsSeries_OnPbReady � ��������� ��.

// ���������� �� ����� ����:
// - ���������� ������ - ��� ��� ������, ������� �������� �� ���-�����.
// - ��� ��������� ������ ������������.
// - ���������� ���������� ������� �������������� �������� ���-�����.
// - ����� ����������� ��� �� ����������� ������ ��������������� ��� �������.
// - ���������� ��� ������ ����������� �������.
// - �������� ��� �������������� �� �������� (���� �������, ������� �������� �� ���-�����) 
//   � ��������� (����� ��������, ���������� ���).
// - ����� - ��� ������ ������� � ���- ����� (book �� ������� NEOWAY M590).

// �������� ������� ��� ���� ��������� � ����� ���������� � �������:
// - ���������� ���� ��-�� ������ ���������� ������� �� ������� �����������.
// - ������ �� ��� ���-�������, ����� ������� "?".

// ��������� ����� ��� ��������, ���������� ���, ���������� � �������:
// - ������ ������� "?" (����� - ����������).
// - �������� �������� �������, ����� �������� ��������� ������� �� ����������� (� � ����� 
//   �������� ���������� ������ ������� �� ��������).

// ������ ��������� ����� ����������� ���:
// 1. �� ������������ ��� ������������ ��� �����.
// 2. ����� ����� �������������: ��������� � ��������� ����� ����������� �� 
//    ����������, �������� 'v' � 'V' - ���������� �����.
// 3. ����������� ������ f_smsCommandIndex � smsCommand.
// 4. �������� ���� bit_FindOkNumberInBook.
// 5. ���������, �� ������, ������ �� ����� � ���������� ��� � ������� �� ������������ ���.
// 6. ���� ������� ��� ������ �� ���������� ����� � �� ����� ����������� ������, ��������� 
// � ������ ������������ ��� ��� � ������������� ������ � ���������� ���.
// 7. ���������� ��� bit_FindOkNumberInBook.
// 8. ���� ���-������� ������� �������� ��������, ������ ��� bit_SendSmsToAllPhonesInBook. 
// 9. ���� �������� �������� �� ���������, ���������� ������ ������������ ��� ��������� ���.
// 10. ���� ��������� �������� ��������, �� ��������� � ������� �� ������ 
//    ������ �� ���������� ����� ����� � �������� ��������� ���.

#include "msp430G2553.h"

#define gsm

// ������������ �� ��� �������� ��� receiveGsmPackage
// �� ��������� ���������� ��� � PDU > (receiveGsmPackageMax - 13)
// ������ ��� ������������
// #define use_buffer_for_send_sms 

#include "gsm_mainTitle.hpp"

#include "rus1.hpp"
#include "gsmUartCommands.hpp"
#include "gsm.hpp"
#include "gsmUartCommands.hpp"
#include "example_base_functions.hpp"
#include "example_options.hpp"
#include "peripherals.hpp"
#include "example_base_types.hpp"
#include "rus1_screenLCD_AC-162A.hpp"
#include "example_base_strings.hpp"

///////////////////////////////////////////////////////////////////
// gsm types

t_phoneBook phoneBook = { 0, 0 };

// ��������� ������� � �������� smsCommand � smsAnswer // ���� -1, �� �� �������
t_CommandAndAnswerIndex commandAndAnswerIndex = { -1/*, 0*/};

// sms-������� ������
t_smsCommand const smsCommand[smsCommandCount] = {
  // 0 // ����������
  {smsCommand_Monitoring, smsCommand_MonitoringCnt, smsCommand_MonitoringCnt},            
  // 1 // ���������� � ��������� ��������
  {smsCommand_220_OffThenOn, smsCommand_220_OffThenOnCnt, smsCommand_220_OffThenOnCnt},   
  // 2 // ���������� ��������
  {smsCommand_220_Off, smsCommand_220_OffCnt, smsCommand_220_OffCnt},             
  // 3 // ��������� ��������
  {smsCommand_220_On, smsCommand_220_OnCnt, smsCommand_220_OnCnt},               
  // 4 // ���������� � ��������� ��������
  {smsCommand_220_OffThenOn_Rus, smsCommand_220_OffThenOn_RusCnt, smsCommand_220_OffThenOn_RusCaptionCnt}, 
  // 5 // ���������� ��������
  {smsCommand_220_Off_Rus, smsCommand_220_Off_RusCnt, smsCommand_220_Off_RusCaptionCnt},     
  // 6 // ��������� ��������
  {smsCommand_220_On_Rus, smsCommand_220_On_RusCnt, smsCommand_220_On_RusCaptionCnt},       
  };

// �������� ������
t_beep beep = { -1, -1, 0 };

// 0 - �������� ������ � ������� ����������
const unsigned char beepOkMas[beepOkMasCnt] = { 1 };
// 1 - �������� ������ � ������ ����������
const unsigned char beepBadMas[beepBadMasCnt] = { 10 };
// 2 - ������ �������� ������ � ����� ������ ����������
// ������� 5, �������� 85; �� �������� - �������, ��������� ������ ������� flash-������
const unsigned char beepLongBadMas[beepLongBadMasCnt] =
// ��������� �������� �������, ������� �������� ������������ �������� ��������
// 1   2    3   4    5   6    7   8    9   10
// *        *        *        *        *       
{ 10,   3, 10,   3, 10};/*, 100, 10, 100, 10, 100,
  10, 100, 10, 100, 10, 100, 10, 100, 10, 100,
  10, 100, 10, 100, 10, 100, 10, 100, 10, 100,
  10, 100, 10, 100, 10, 100, 10, 100, 10, 100,
  10, 100, 10, 100, 10, 100, 10, 100, 10, 100,
  10, 100, 10, 100, 10, 100, 10, 100, 10, 100, 
  10, 100, 10, 100, 10, 100, 10, 100, 10, 100,
  10, 100, 10, 100, 10, 100, 10, 100, 10, 100, 
  10, 100, 10, 100, 10 };*/
// ����� ������ 3, �� ��� ��������� �������� ������ � ����� ������� ���������� 
//  � ���������� ������������� ������� ������� �� ���.

// ����� 10 ��� �� 700 �� � ������ 300 ��, � 33 ���� �� 700 �� � ������ 10 ������.
#define beepLongOkMasCnt 5 
const unsigned char beepLongOkMas[beepLongOkMasCnt] = { 1, 2, 1, 2, 1 };
t_beepMas const beepMas[beepMasCount] = { 
  { beepOkMasCnt, &beepOkMas[0] },
  { beepBadMasCnt, &beepBadMas[0] },
  { beepLongBadMasCnt, &beepLongBadMas[0] },
  { beepLongOkMasCnt, &beepLongOkMas[0] }
  };

// ��������� gsm // ����� ��� �����������, ����� ussd-������� ������������ ��� ����������� �������
// 0 - ������
// 1 - �������
// 2 - ���
signed char gsmOperatorUssd_Index = -1; // ���� -1, �� �������� gsm �� ��������
const t_gsmOperatorUssd gsmOperatorUssd[gsmOperatorUssdCnt] = {
  {&lineCOPS_Beeline[0], &lineUSSDbalance_Beeline[0], lineCOPS_BeelineCnt, lineUSSDbalance_BeelineCnt}, 
  {&lineCOPS_Megafon[0], &lineUSSDbalance_MegafonAndMts[0], lineCOPS_MegafonCnt, lineUSSDbalance_MegafonAndMtsCnt}, 
  {&lineCOPS_Mts[0], &lineUSSDbalance_MegafonAndMts[0], lineCOPS_MtsCnt, lineUSSDbalance_MegafonAndMtsCnt},
  {&lineCOPS_Tele2[0], &lineUSSDbalance_Tele2[0], lineCOPS_Tele2Cnt, lineUSSDbalance_Tele2Cnt}
  };

// ������� gsm-������� � ��� ��������
t_gsmLevel gsmLevel = { 0 };// { 0, 0 };

// ������� ����������� �������
t_funcsSeries funcsSeries = { NULL, 0, 0};

// ��������� �� ��������� � ����� ����� � ��� �����
t_errorScreen errorScreen = { NULL/*, 0*/ };

/////////////////////////////////
// ������ ��� �������� ���
t_smsDataSend sds;

// ����� ����� ������ �� UART.
// ����� ������ receiveGsmPackage �����, ��������� ������ ������
// ��� ����������� ������ ������ � ���������� ����� � PDU-������� � ������ ����� �������� �� 16 �������� ��
// ������ ������ ������� (��������� ������� � ������������ �������� ������ ����� ��������������), ���� 
// ������� 113 ����
__no_init unsigned char receiveGsmPackage[receiveGsmPackageMax] @ addrNastr_Free; // receive_Package
// ��������� �� ��������� ������ ��� ����������, � receiveGsmPackage
unsigned char* pReceiveGsmPackageActive = &receiveGsmPackage[0]; 

// ��������� �� ������ �������� ���������� � ������ // ��� ��������� � �������� ������
// ������ �������� ���������� - ����� � ������ ������ ��� ������� �������� ������ � �������, � ��� �� ���-�����
unsigned char* answerEffectiveDataBegin = NULL;

// ��������� �� ����� �������� ���������� � ������ // ��� ��������� � �������� ������
// ����� �������� ���������� - ����� � ����� ������ ����� �� ��� ������� �������� ������ � �������
unsigned char* answerEffectiveDataEnd = NULL;

// ���������� ������������ � ���������� TIMER, ��� �������� ��� ����� ��-������
unsigned int f_timerCounter_AtPauseDelay = 0; // ���������������� � ������������ � itterruptsIndRus1_AtPauseDelay

// ������ ����� // ������� ��� ����� ����� � ��������� ����� // ������ �� ������ �� ussd-������ � ������� �����
unsigned char balans[balansCount] = {'?',' ',' ',' ',' ',' ',' '};

// ������� ������ ����� ��������� �������
unsigned char modemAnswerCnt = 0;

/////////////////////////////////////////////////////////////////////////
// ������ ���������� �� ��� ��������� ������ ��������� ������ � �� �������
// ��� ���������
const t_strAndStrCnt msgGsm[msgGsmCnt] = { 
{ &msgModemStartUp[0], ansModemStartUpCnt },    // 0 - �������� ����� ������
{ &msgPbReady[0], msgPbReadyCnt },              // 1 - �������� ����������� � gsm-����
{ &msgRing[0], msgRingCnt },                    // 2 - �������� ����������� ������
{ &msgSms[0], msgSmsCnt },                      // 3 - �������� ����������� sms
{ &msgLevel[0], msgLevelCnt },                  // 4 - ������� gsm-�������
{ &msgGsmInfo[0], msgGsmInfoCnt },              // 5 - ���������� � gsm-���������
{ &msgGsmUssd[0], msgGsmUssdCnt },              // 6 - ����� �� ussd-������
{ &msgGsmBookInfo[0], msgGsmBookInfoCnt },      // 7 - ���������� � ���������� ����� // ���������� �������
{ &msgGsmPhoneNumber[0], msgGsmPhoneNumberCnt },// 8 - ��������� ���������� ����� � ���������� �����
{ &msgGsmStatus[0], msgGsmStatusCnt }           // 9 - ������ ������
};


////////////////////////////////////////////////////////////////////////////// 
// ������ ���������� �� ������� �������� ������ ������ ����� ������ ������
const pFuncSendToUart f_FuncsSendToUart_OnStart[funcsSendToUart_OnStartCnt] = {
  CmdCmeeSet_1, // 1: enable result code +CME ERROR:<err>,use numeric <err> to get value;
  CmdAte_On,    // ��������/��������� ���
  CmdCnmi,
  CmdCmgf_On, // ��������� GSM-������ // �� ��������, CmdCmgf_On 2 ���� - ������ ����, ���� 3 ����
  CmdCmgf_On, 
  CmdCmgf_On, 
  CmdCscsGsm, 
  CmdIfc,
  CmdSmsDelete,
  CmdAon_On,
  CmdCopsGet,
  CmdAskUssdBalance, // ��� CmdAskUssdBalance ����� GSM-�����
  CmdCmgf_Off,   // ��������� PDU-������
  CmdCscsUcs2,
  CmdAskLevel,
  CmdCpbs_Check,
 
#ifdef testRezhim
 CmdAt, // CmdSendTestSms,
 CmdSendTestSms_part1,
 CmdSendTestSms_part2,
 CmdAt, // CmdTestRing,
   
 // ��������� ��� ������� ������
 CmdAt,
 CmdCmgf_On,
 CmdCmgf_Off,
 CmdAskLevel,
 CmdAon_On,
 CmdAon_Off,
 CmdCnmi,
 CmdCpbs_SimCard,
 CmdIfc,
 CmdSmsDelete,
 CmdAte_On,
 CmdAte_Off,

 CmdCpas,
 CmdCreg,
 CmdCmeeGet,
 CmdCmeeSet_0,
 CmdCmeeSet_1,
 CmdCmeeSet_2,
 CmdAti,
 CmdIprGet,  
 CmdCopsSet_Alphavit
   
 CmdCscsUcs2,
 CmdCscsGsm,
#endif
};

#ifdef testRezhim
// ������ ������ ��� ������� ��������� ���
const pFuncSendToUart f_FuncsSendToUart_Sms[funcsSendSmsToUart_SmsCnt] = {
  CmdSendTestSms_part1,
  CmdSendTestSms_part2
};
#endif



// ������� ������ ��� ������������ ������
///////////////////////////////////////////
// ������ �������� ���������� �������
const pFuncSendToUart f_FuncsSendToUart_Test_Often[funcsSendToUart_Test_OftenCnt] = {
  CmdAskLevel,
  CmdCopsGet
};

// ������ ���
const pFuncSendToUart f_FuncsSendToUart_Test_PerHour[funcsSendToUart_Test_PerHourCnt] = {
  CmdCmgf_On, // �� ��������, CmdCmgf_On 2 ���� - ������ ���� (���������� �� ����� ����� ����), ���� 3 ����
  CmdCmgf_On, 
  CmdCmgf_On, 
  CmdCscsGsm,
  CmdAskUssdBalance, // ��� ������� �������, ���� Gsm-����� ������
  CmdCmgf_Off, 
  CmdCscsUcs2,
  CmdSmsDelete
};

// ������� ������: ����� ���������� ������ � ���������� ����� �����
const pFuncSendToUart f_funcsSendToUart_FindNumberOk[f_funcsSendToUart_FindNumberOkCnt] = {
  CmdAskNextNumber
};

// ������� ������: ��������� ���������� �� ���-�����, ���� ����� �����, ������� �� ���-����� � ������
const pFuncSendToUart f_funcsSendToUart_CmdCpbs_SimCard[f_funcsSendToUart_CmdCpbs_SimCardCnt] = {
  CmdCpbs_SimCard
};

// ������� ������: �������� �����, ����� �� �� � ������
const pFuncSendToUart f_funcsSendToUart_isModemReady[f_funcsSendToUart_isModemReadyCnt] = {
  CmdCpas
};

// ������� ������: �������� ������ ����� ��� ������ �� phoneBook
const pFuncSendToUart f_funcsSendToUart_SendSmsToNumberFromPhoneBook[f_funcsSendToUart_SendSmsToNumberFromPhoneBookCnt] = {
  SendSmsToNumberFromPhoneBook
};

/*
// ������� ������: ������ ���������� ������� � �������� ������
#define f_funcsSendToUart_CmdBookInfoCnt 1
const pFuncSendToUart f_funcsSendToUart_CmdBookInfo[f_funcsSendToUart_CmdBookInfoCnt] = {
  CmdCpbs_Check
};

// ������� ������: ��������� PDU-������ // ��� �������� ���
#define f_funcsSendToUart_Sms_PduCnt 3
const pFuncSendToUart f_funcsSendToUart_Sms_Pdu[f_funcsSendToUart_Sms_PduCnt] = {
  CmdCmgf_Off, CmdCscsUcs2, NULL
};
// ������� ������: ��������� GSM-������ // ��� ����� ���
#define f_funcsSendToUart_Sms_GsmCnt 4
const pFuncSendToUart f_funcsSendToUart_Sms_Gsm[f_funcsSendToUart_Sms_GsmCnt] = {
  CmdCmgf_On, CmdCmgf_On, CmdCmgf_On, CmdCscsGsm // �� ��������, CmdCmgf_On 2 ���� - ������ ����, ���� 3 ����
};
*/


#ifdef useGeneralFlag
unsigned int f_generalFlag = 0;
#else
t_generalFlag f_generalFlag;
#endif


// ������� 100�� �� ��������� ��������� ������� ������
// ��� �������� ������� �� �������� ������ ������ ����� �������� ��������� ������� � ����� ������ �������
// ��� ����� ������ ��� ������ � ���, ������������ ������ � f_generalFlag
unsigned int fromLastCommand100ms = 0; // (gsmPowerOnWait_100ms - 3);




/////////////////////////////////////
// ������������
/////////////////////////////////////

// ���������� beep.f_beepMasIndex
void SetBeepMasIndex(unsigned char x) { beep.f_beepMasIndex = x; beep.f_beepDelay100msIndex = -1;} // 4 ����� ��������

// #define dRelePower220_Off { P1OUT |= dRelePower220_Bit; }
// void RelePower220_Off() { P1OUT |= dRelePower220_Bit; }
// #define dRelePower220_On { P1OUT &= ~dRelePower220_Bit; dSet220commandManualOff_Off; }
void RelePower220_On() { P1OUT &= ~dRelePower220_Bit; dSet220commandManualOff_Off; }

// ����� �������� ���� ������� � ���������� ����� �����
// #define dStartFindAllPhonesInBook { bookNumbersNeedGetCount = bookNumbersCount; if (0 == bookNextNumberCPBR) bookNextNumberCPBR = 1;}
void StartFindAllPhonesInBook()
{
  bookNumbersNeedGetCount = bookNumbersCount;
  if (0 == bookNextNumberCPBR)
    bookNextNumberCPBR = 1;
}

// �������� �� ������ �������� �������� ������ ��� �������
bool IsSimbolLineBreak(unsigned char simb)
{
  unsigned char const* cEnd = &simbolsLineBreak[simbolsLineBreakCnt];
  for (unsigned char const* c = &simbolsLineBreak[0]; c < cEnd; c++)
    if (*c == simb)
      return(true);
  return(false);
}

// ���������� ���������� ����� �� 2 �������� � answerEffectiveDataBegin, ������� ��������� +2
// ����������������� ����� ������ ���� ����������
unsigned char GetDecimalFromHexOctet()
{
  unsigned char res = 0;
  unsigned char* textEnd = answerEffectiveDataBegin + 2;
  for (; answerEffectiveDataBegin<textEnd; answerEffectiveDataBegin++)
  {
    // if ( (*answerEffectiveDataBegin > 0x2F) & (*answerEffectiveDataBegin < 0x3A) )
    if (*answerEffectiveDataBegin > 0x2F) 
    {
      if (*answerEffectiveDataBegin < 0x3A)
        res = res * 16 + (*answerEffectiveDataBegin - 0x30);
      else
        // ������� ����� � � �� ��������� - �� ����� �� ��� � ���-�������� � ����� ��� �� ������� �� ����������
        if (*answerEffectiveDataBegin > 0x40)
          if (*answerEffectiveDataBegin < 0x47)
            res = res * 16 + (*answerEffectiveDataBegin - 55);
    }
  }
  return(res);
}

// �������� ����� � 10-�������� ���� 900 123 45 67
unsigned char* CheckPhoneNumber(unsigned char* number, unsigned char* numberCnt)
{
  if (numberNeedSendMax < *numberCnt)
  {
    unsigned char i_temp = ((*numberCnt) - numberNeedSendMax);
    (*numberCnt) -= i_temp;
    number += i_temp;
  }
  return(number);
}

// ��������� ����� � phoneBook
void SaveNumber(unsigned char* num, unsigned char numCnt)
{
  num = CheckPhoneNumber(num, &numCnt);
  dSetNumberNeedSendCount(numCnt);
  unsigned char* numEnd = num + numCnt;
  unsigned char* pCopuTo = pNumberNeedSend;
  for (; num<numEnd; num++, pCopuTo++)
    *pCopuTo = *num;
}

// ��������� ������ sds
void SdsDataUpdate()
{
  // ������� ��� ������� ��� �� ���������
  sds.smsPartIndex = 1;
  sds.smsPartsCount = 1;
  // rand() ����� 300 ����, ������� �������� ���:
  // "��������� �����", ��� �������� "�������" ���, ���� �� ����c��� TAR ������� NameTimerForRus1
  sds.smsRandomNumber = (unsigned char)TA0R;
  // ������� ������ ��� ����������� ������ ���
  sds.rest220_seconds = restart220_seconds;
  sds.gsmLevel = gsmLevel.generalLevel;
  sds.isRele220_On = isRelePower220_On;
  sds.is220_Alarm_PowerOff = isPower220_Alarm_PowerOff;  
//   sds.val220 - ��� �������� ����������� � ParseAnswerPhoneNumber � � ���������� ADC10_VECTOR
//   sds.is200wasUp - ��� �������� ����������� � ���������� ADC10_VECTOR
}

// ������������ ������
void RestartModem()
{
  dBeepSignal_Bad;
  errorScreen.word = lines_ModemRestart; // ������ ������ - ���� ��� ������ �� ������
  dResetRezhimSpecMessageTime100msCnt; // RezhimSpecMessage_On(errorScreen.word, 0, (BIT4 | BIT5 | BIT7));
  dGsmVcc_Off;
#ifdef useGeneralFlag
  f_generalFlag = 0; 
#else
  unsigned int* iiEnd = (unsigned int*)((unsigned int)&f_generalFlag + sizeof(f_generalFlag));
  for (unsigned int* ii = (unsigned int*)&f_generalFlag; ii<(unsigned int*)iiEnd; ii++)
    *ii = 0;
#endif
}

// ���������� ������� ���������� ������
// ��������� �� ������ ���������� �� ���������, ������ ������� ���������� �� ���������, ����� ����� ���������
void SetFuncsSeries(const pFuncSendToUart* f, unsigned char cnt, unsigned char pause100ms )
{ 
  funcsSeries.nextFunc = f;
  funcsSeries.funcsCnt = cnt;
  funcsSeries.waitBetween_100ms = pause100ms;
  // dResetFromLastCommand100ms;
  dSetModemReady_Off;
}

// ����� �������� ����� ���� ������� �� ����� �����
void StartSendSmsToAllNumbers() 
{ 
  SdsDataUpdate();
  dSetSendSmsToAllPhonesInBook_On; 
  StartFindAllPhonesInBook();
  dSetFuncsSeries_FindNextNumber;
}


// ����� ������ � ����� ������, ������������ � ������� pNumberNeedSend
// #define dStartFindPhoneNumberOk { dSetFindOkNumberInBook_On; dStartFindAllPhonesInBook; dSetFuncsSeries_FindNextNumber; } 
void StartFindPhoneNumberOk() 
{ 
  dSetFindOkNumberInBook_On; 
  StartFindAllPhonesInBook(); 
  dSetFuncsSeries_FindNextNumber; 
}

// �������� �� answerEffectiveDataBegin ����� ������ textLength � ��������� UCS2, ��������� �������� � pRes
// ������, ����� answerEffectiveDataBegin �� ����� �� ������� answerEffectiveDataEnd
// ���������� ��������� �� ��������� ������ � ���������� // ��� ����������� ����� ����������
unsigned char* Ucs2ToText(unsigned char* pRes, unsigned char textLength)
{
  // ��� �������� � 16-������ �������
  textLength <<= 1; // 16 ��� - 2 ������
  unsigned char* textEnd = answerEffectiveDataBegin + textLength;
  for (; answerEffectiveDataBegin < textEnd; pRes++)
  {
    if ( (answerEffectiveDataBegin + 3) > answerEffectiveDataEnd) // ����� ��� "OK" � �����
      break;
    unsigned char firstOctet = GetDecimalFromHexOctet();
    *pRes = (unsigned char)GetDecimalFromHexOctet();
    if (0 != firstOctet)
      *pRes += 176;
  }
  return(pRes);
}

// ����������� ����� number � ���������� � phoneBook, ��������� �� ���
bool isNumberOk(unsigned char* number, unsigned char numberCnt)
{
  number = CheckPhoneNumber(number, &numberCnt);
  return(CompareStrings(number, pNumberNeedSend, numberCnt));
}

// ����������� ���������, ��������� � uart: �������� ������ ��������� � msgGsm; ���� �� ����� - ���������� -1
// ��� ��, ��������� answerEffectiveDataBegin � answerEffectiveDataEnd
signed char IndexReservedMsg()
{    
  // 1.�������� ������ ����� � ������ (��������� �� ���� � ��� �����).
  // ��������� ������� �������� ������ � ������ � � �����.
  answerEffectiveDataBegin = &receiveGsmPackage[0]; // ������ ������� �������, ������� �� ������ �������� ������, � ������ ������.
  answerEffectiveDataEnd = pReceiveGsmPackageActive - 1; // ��������� �� ��������� ��-Line-Break ������ � �������� ������
  while (IsSimbolLineBreak(*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return(-1);
  while (IsSimbolLineBreak(*answerEffectiveDataEnd))
    if (answerEffectiveDataBegin >= --answerEffectiveDataEnd) return(-1);  
  // 2.��������� �� ������������
  if ((answerEffectiveDataEnd - answerEffectiveDataBegin) < 4) return(-1);  
  // 3.���� ����� �����, ������ ������� ��� - � ������ � ������ ������ "����" � ����� �� ������� ��� ������ (���� ���)
  if (isSendSmsComplete)
    return(msgGsm_SmsSendComplete);
  // 4.������� ���-����� "AT+"
  if (CompareStrings(dataATCSQ, answerEffectiveDataBegin, 3))
  {
    // ��� �������, ����� ���� ����� "OK" �� ������, ���� ���������� �������� ���-�����
    // ���� ��� ���-����� �� ������� ��������� ���������� �� ���-�����, ���� ����� �����, ������� �� ���-����� � ������
    if (CompareStrings(strCmdCpbs_SimCard, answerEffectiveDataBegin, strCmdCpbs_SimCardCnt))
      return(msgGsm_CmdCpbs_SimCard);
    // ���� ��� ���-����� �� ������� ������ ����� �������� ���
    if (CompareStrings(cmdSendSmsStart, answerEffectiveDataBegin, cmdSendSmsStartCnt))
      return(msgGsm_Smgs);      
    // ���������� ���-�����
    while (!IsSimbolLineBreak(*answerEffectiveDataBegin))
      if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return(-1);
    while (IsSimbolLineBreak(*answerEffectiveDataBegin))
      if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return(-1);      
  }
  // 5.����� ����� �����
  if (answerEffectiveDataBegin == answerEffectiveDataEnd) return(-1);
  unsigned char* endWord = answerEffectiveDataBegin + 1; // ��������� �� ��������� ������ �����
  while (!IsSimbolLineBreak(*endWord))
    if (++endWord >= answerEffectiveDataEnd) { endWord++; break; } //  endWord++ ��� ������������� ��� length
  unsigned char length = endWord - answerEffectiveDataBegin;  
  // 6.���������� ��� �� ����� ������� � msgGsm.
  for (unsigned char indexMsgGsm=0; indexMsgGsm<msgGsmCnt; indexMsgGsm++)
    if (msgGsm[indexMsgGsm].strCnt == length) // ��� ��������� ����� ���������� ����� �����
      if (CompareStrings(&(msgGsm[indexMsgGsm].str)[0], answerEffectiveDataBegin, length))
      {
        // ����������� answerEffectiveDataBegin �� ������� ����� msgGsm[indexMsgGsm].msg
        answerEffectiveDataBegin += length;
        if (answerEffectiveDataBegin >= answerEffectiveDataEnd)
          answerEffectiveDataBegin = answerEffectiveDataEnd;
        else
          while (IsSimbolLineBreak(*answerEffectiveDataBegin))
            if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) break;
        return(indexMsgGsm);
      }
  return(-1);
}

// ���������� � ��������� pSms ����� word � ������� unicode; ��������� ��������� �� ��������� ��� ���������� ������ � receiveGsmPackage ��� ����������
// ��������� �� ����� ��������� ����������� ���������� PC
void AddToPointUCS2(const unsigned char* word, unsigned char wordCnt)
{
  const unsigned char* wordEnd = word + wordCnt - 1;
  for (; word<=wordEnd; word++)
    if ( (0x0D == *word) | (0x0A == *word) ) // ������� �������� ������
    {
      AddToPointUCS2(0x00);
      AddToPointUCS2(0x0A);
    }
    else
      /*if ('�' == *word) �� ��������� - �� ����� �� ���������� � ���
      {
        AddToPointUCS2(0x21);
        AddToPointUCS2(0x16);
      }
      else*/
    if (*word < 0xA8)
    {
      AddToPointUCS2(0x00);
      AddToPointUCS2(*word);
    }
    else
    {
      AddToPointUCS2(0x04);
      /*if ('�' == *word)        �� ��������� - �� ����� �� ���������� � ���
        AddToPointUCS2(0x01);
      else
        if ('�' == *word)
          AddToPointUCS2(0x51);
        else*/
          AddToPointUCS2(*word - 0xB0);
    }
}
// ��������� � ��������� ��� � UART �������� ����� � ����������������� ������� 0x00, ��� ���� ����� 0x00 ����������� � ��������� ��� � �������� ��� �����
// ��������� ��������� �� ��������� ��� ���������� ������ � receiveGsmPackage ��� ����������
void AddToPointUCS2(unsigned char number)
{
#ifdef use_buffer_for_send_sms
  *(sds.pBuffer)++ = LCD_NumberToCharHex(number >> 4);
  *(sds.pBuffer)++ = LCD_NumberToCharHex(number & 0x0F);
#else
  if (isSentForSms_Cmgs)
  {
    SendCharToUart(LCD_NumberToCharHex(number >> 4));
    SendCharToUart(LCD_NumberToCharHex(number & 0x0F));
  }
  sds.pBuffer += 2;
#endif
}

#define maxSmsRusChars 70 // ������������ ����������� ������� �������� � ���
// ��������� � ��������� �������� �����; ��������� ��������� �� ��������� ��� ���������� ������ � receiveGsmPackage ��� ����������
#define usc2bufTempCnt 5
void AddToPointUCS2_num(signed long num)
{
  // � �������� ������ ���������� ��������� ������ ������ UART - �� ����� ��� � ������� ������ ����� ����������� ��������
  LongToStr(num, &receiveGsmPackage[receiveGsmPackageMax - usc2bufTempCnt], usc2bufTempCnt); 
  AddToPointUCS2(&receiveGsmPackage[receiveGsmPackageMax - usc2bufTempCnt], GetLengthOfSignedLong(num));
}
// ��������� � ��������� ����������; ��������� ��������� �� ��������� ��� ���������� ������ � receiveGsmPackage ��� ����������
void AddToPointUCS2_220()
{
  AddToPointUCS2(strNapriazhenie, strNapriazhenieCnt);
  AddToPointUCS2_num(sds.val220);
  AddToPointUCS2(strB, strB_cnt);
}
// ��������� � ��������� ���� ���/����; ��������� ��������� �� ��������� ��� ���������� ������ � receiveGsmPackage ��� ����������
void AddToPointUCS2_rele()
{
  if (sds.isRele220_On)
    AddToPointUCS2(strVklucheno, strVkluchenoCnt);    
  else
    AddToPointUCS2(strVuklucheno, strVukluchenoCnt);
}
// ������� PDU-����� ���, ���������� ���������� ���� � ������ PDU �� ����������� ���� SCA (��� ������� AT+CMGS=xx)
// PDU-����� ��������� ������ � ������� AddToPointUCS2, ��-�� use_buffer_for_send_sms // ��� ���������� �������
unsigned char PreparePduForSms()
{
  sds.pBuffer = &receiveGsmPackage[0];

  // SCA (Service Center Address) = 0x00
  AddToPointUCS2(0x00);
  
  // PDU Type // ��� ���������: ���� ��������� ���, �� 0x01; ���� "�������", �� 0x41
  if (sds.smsPartsCount == 1)
    AddToPointUCS2(0x01);
  else
    AddToPointUCS2(0x41);
  // MR (Message Reference) � ���������� ����� ���������, ������������ ����� �������. � PDU �������� ���� = 00 // � ������ sds.smsPartIndex
  AddToPointUCS2(sds.smsPartIndex);
  // DA (Destination Address) � ����� �������� ���������� ���������
  unsigned char pduNumberCnt = numberNeedSendCount; // ���������� �������� ���� � ������
  unsigned char* pduNumderIndex = pNumberNeedSend; // ������ // ������ - ������ �������� �����
  // ������� ������ � ������ +7
  // ���������� ����, ������� �� ���� +7
 //  if (numberNeedSendMax == numberNeedSendCount) // ���� ����� ���� 900 123 45 67
  {
    AddToPointUCS2(++pduNumberCnt);
    AddToPointUCS2(0x91); //  ���� ���� ������ ����������� 
    AddToPointUCS2((*pduNumderIndex++ << 4) | 0x07);
  }
//   else
  { // �� �������� ������ �� ��������������
    // AddToPointUCS2(pduNumberCnt);
  //   AddToPointUCS2(0x91); //  ���� ���� ������ ����������� 
  }
  bool isAddFF = ( (pduNumberCnt %2) > 0); // ���� �������, �� � ����� ����������� 0xFF
  unsigned char* pduEndNumder = pNumberNeedSend + numberNeedSendCount - 1;
  if (isAddFF)
    pduEndNumder--;
  for (; pduNumderIndex<=pduEndNumder; pduNumderIndex += 2)
    AddToPointUCS2(((*(pduNumderIndex+1)-0x30) << 4) | (*pduNumderIndex-0x30));
  if (isAddFF)
    AddToPointUCS2(0xF0 | (*(++pduEndNumder)-0x30));
  // PID (Protocol Identifier) � ������������� ���������
  AddToPointUCS2(0x00);
  // DCS (Data Coding Scheme) � ��������� ��������� // ��������� UCS2, ������� = 08h
  AddToPointUCS2(0x08);
  // VP (Validity Period) � ����� ����� ��������� // �� ������������, ������� �����������
  // UDL (User Data Length) � ����� ���� UD (���������) � ������ // � ������ � ���������� UCS2, ����� ����������_��������*2
  // ���������� ����, ������������ � ��� = ( (pBuffer-1) - pUDL)
  
  // ������ ���������� pdu, ����� �����, ����� "�������������" ����� // ������ �����
  // pdu ������ = 70, ��� ��������� 
  // ������� ����� ��� ������ 70 ������� ��������; ���� ������, ����������� �� 70 ���������
  AddToPointUCS2(maxSmsRusChars << 1);
  // ��� ������������ ���������� �������� � ��� �� 70 ��� ������ ��������� ������ // ������� �����
  unsigned char* pTextSms_Begin = sds.pBuffer;
  
  // ��-�� ����, ��� ��� ���������� ������� RESTART, ������������ ��� ��� ��� ������ �� ������� smsCommandIndex_20_OffThenOn,
  // ��� RESTART �������� ��������� ������ smsCommandIndex_Monitoring �� smsCommandIndex_20_OffThenOn, �� ����� smsCommandIndex
  signed char tempCommandIndex = smsCommandIndex;
  // ���� ���� ��������� ��� ��-�� ������ ���������� �� ���������� �������, ������� �� �������,
  // ����� ������� �� ������� � ���-���������
  if ( /*(isPower220_Alarm_PowerOff) & */(isNeedSendSmsOnPower220_Alarm_PowerOff) )
    // tempCommandIndex = smsAlarm220;
  {
    if (!sds.is220_Alarm_PowerOff) // ���� ���������� ��������� � ����� ������ ��������� ����� secondsPowerAlarmMax
      {
        if (isStartSecondsPowerAlarm)
        {
          // ���������� 220� � 15� ���� ���� ������ 250�
          // ���������� 220� � 15� ���� ���� ������ 150�
          AddToPointUCS2_220();
          AddToPointUCS2(strProbelI_Probel, strProbelI_ProbelCnt);
          AddToPointUCS2_num(secondsPowerAlarmLeft);
          if (sds.is200wasUp)
          {            
            AddToPointUCS2(strSecundBuloVushePoroga, strSecundBuloVushePorogaCnt);
            AddToPointUCS2_num(nastr_DataMax220);            
          }
          else
          {            
            AddToPointUCS2(strSecundBuloNizhePoroga, strSecundBuloNizhePorogaCnt);
            AddToPointUCS2_num(nastr_DataMin220);
          }
          AddToPointUCS2(strB, strB_cnt);
        }
        else // if (isStartSecondsPowerAlarm)
          // ���������� ���������������
          AddToPointUCS2(strNapriazhenieNormalizovalos, strNapriazhenieNormalizovalosCnt);
      }
      else // if (!sds.is220_Alarm_PowerOff)
      {
        AddToPointUCS2(strNapriazhenie, strNapriazhenieCnt);            
        if (sds.is200wasUp)
        {
          // ���������� ���� ������ 250�!
          AddToPointUCS2(strVushePoroga, strVushePorogaCnt);
          AddToPointUCS2_num(nastr_DataMax220);
        }
        else
        {
          // ���������� ���� ������ 150�!
          AddToPointUCS2(strNizhePoroga, strNizhePorogaCnt);
          AddToPointUCS2_num(nastr_DataMin220);
        }
        AddToPointUCS2(strB, strB_cnt);
      } // if (!sds.is220_Alarm_PowerOff) .. else
        // ���� ��������/���������
        AddToPointUCS2_rele();
        // ���������� 220�
        AddToPointUCS2_220();
  }
  else // if ( (isPower220_Alarm_PowerOff) & (isNeedSendSmsOnPower220_Alarm_PowerOff) )
  {
    if (smsCommandIndex_Monitoring == tempCommandIndex)
      if (isPower220_PauseThenOn)
        tempCommandIndex = smsCommandIndex_20_OffThenOn; 
    switch (tempCommandIndex)
    {
      case smsCommandIndex_Bad:
        AddToPointUCS2(0x05); // ����� ����
        AddToPointUCS2(0x00); // IEI
        AddToPointUCS2(0x03); // 
        AddToPointUCS2(sds.smsRandomNumber);
        AddToPointUCS2(sds.smsPartsCount); // IED2, ���������� ������ � ���������
        AddToPointUCS2(sds.smsPartIndex); //// IED3, ���������� ����� ���������
        if (1 == sds.smsPartIndex)
          AddToPointUCS2(spravkaPart1, spravkaPart1Cnt);
        else
          AddToPointUCS2(spravkaPart2, spravkaPart2Cnt);
        break;
      case smsCommandIndex_Monitoring:
        // ���� ��������/���������
        AddToPointUCS2_rele();
        // ���������� 220�
        AddToPointUCS2_220();
        // ������ 100%
        AddToPointUCS2(strSignal, strSignalCnt);
        AddToPointUCS2_num(sds.gsmLevel);
        AddToPointUCS2(strPersent, strPersentCnt);
        // ��� ������� 200
        AddToPointUCS2(strDovNomerov, strDovNomerovCnt);
        AddToPointUCS2_num(bookNumbersCount);
        AddToPointUCS2(strPerenosStroki, strPerenosStrokiCnt);
        break;
      case smsCommandIndex_20_OffThenOn_Rus:
        // ������� "RESTART": ���� ���. ����� 86400�.
        AddToPointUCS2(strCommandRestart, strCommandRestartCnt);
        AddToPointUCS2_num(sds.rest220_seconds);
        AddToPointUCS2(strC, strC_Cnt);
      case smsCommandIndex_220_Off_Rus:
      case smsCommandIndex_220_On_Rus:
        if (smsCommandIndex_20_OffThenOn != tempCommandIndex)
        {
          // ������� "ON/OFF" ���������
          if (smsCommandIndex_220_On_Rus == tempCommandIndex)
            AddToPointUCS2(smsCommand_220_On_Vupolnena, smsCommand_220_On_VupolnenaCnt);
          else
            AddToPointUCS2(smsCommand_220_Off_Vupolnena, smsCommand_220_Off_VupolnenaCnt);
          // ���� ��������/���������
          AddToPointUCS2_rele();
        }        
        // ���������� 220�
        AddToPointUCS2_220();
        break;
      default:
        break;  
    } // switch
  } // if ( (isPower220_Alarm_PowerOff) & (isNeedSendSmsOnPower220_Alarm_PowerOff) ) .. else
  // ������, � ����������� ��� �� 70 ��������
  if (sds.smsPartsCount == sds.smsPartIndex)
  {
    // ����� ��� ������ 70 ������� ��������; ���� ������, ����������� �� 70 ���������
    // ������ �������
    AddToPointUCS2(strBalans, strBalansCnt);
    // �������� ������� � ����������� ������� ��� ���������� 70 ��������
    unsigned char enteredChars = ((sds.pBuffer - pTextSms_Begin) >> 2); // ������� �������� ��� ����� � �����
    unsigned char balansChars = maxSmsRusChars - enteredChars; // ������� �������� �������� ������ �� 70 ��������
    if (balansChars > balansCount)
    {
      AddToPointUCS2(balans, balansCount);
      for (unsigned char ii=balansCount; ii<balansChars; ii++)
      {
        AddToPointUCS2(0x00);
        AddToPointUCS2(' ');
      }
    }
    else
      AddToPointUCS2(balans, balansChars);  
  }
  return((sds.pBuffer - &receiveGsmPackage[0] - 2) >> 1);
}




///////////////////////////////////
// ������� UART-�������



// ������� ������ gsm-������� ��: ������� � ��������
// x ����� ���� � ��������� 0..31 ��� 99. ��� ������ ��� �����, �� 99 ������ ��� ����������
// level = x * 100 / 31
const unsigned char masGsmLevel[31] = 
{  3,  6, 10, 13, 16, 19, 23, 26, 29, 32, 
  35, 39, 42, 45, 48, 52, 55, 58, 61, 65, 
  68, 71, 74, 77, 81, 84, 87, 90, 94, 97,
  100 };
// x ����� ���� �� 0 �� 7, ��� ����� ������ ��� �������� ����� �����, �� 99 ������ "Bit error rate can not be measured"
// quality = 100 - x * 100 / 7
// const unsigned char masGsmQuality[8] = { 100, 86, 71, 57, 43, 29, 14, 0 };
void ParseReservedLevelGsm()
{ \
  /*������� �������, % */ \
  unsigned int res = (*answerEffectiveDataBegin++) - 0x30; \
  if (',' != *answerEffectiveDataBegin) \
    res = (10 * res + ((*answerEffectiveDataBegin++) - 0x30)); \
  if (99 == res) \
    res = 0; \
  else \
    res = masGsmLevel[res]; // ��� ������� � ������ ��������, ��� res/31
  /*����������� ������ �����, %*/  \
  answerEffectiveDataBegin++;
  unsigned char quality = ((*answerEffectiveDataBegin) - 0x30);
  // ���� ���� % ��������, �� masGsmQuality[(*answerEffectiveDataBegin) - 0x30] / 100;
  // ���� ���� generalLevel, ��:
  // res = res * ((*answerEffectiveDataBegin) - 0x30) / 7;
  // ���������������� ������ �� �����������
  if (9 == quality)
    gsmLevel.generalLevel = 0;
  else
  {
    res = res * (7 - quality);
    unsigned int q = (res >> 1);
    unsigned int qTemp = q >> 3;
    q += qTemp;
    q = q + (q >> 6);
    // q = q + (q >> 12) + (q >> 24); // �������� = 100*7 = 700 = 10 1011 1100, ������ ����� > 10 ��� - ������
    q >>= 2;
    unsigned int r = res - q * 7;
    gsmLevel.generalLevel = q + ((r + 1) >> 3);
  }
  if (0 == gsmLevel.generalLevel)
  {
    dSetTypeSpecMessage_Critical_onZero;
    dResetRezhimSpecMessageTime100msCnt;
  }
  else
    if (line_ModemPbReady_Error == errorScreen.word)
      if ((signed char)-1 != gsmOperatorUssd_Index)
        dSetTypeSpecMessage_NonCritical;
}

// ������� ��������� ������
void ParseReservedRing()
{
  smsCommandIndex = smsCommandIndex_Bad;

  // ������� �� ������
  while ('"' != (*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  answerEffectiveDataBegin++;
  unsigned char* pBeginNumber = answerEffectiveDataBegin;
  // ���������� ���������� �������
  while ('"' != (*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  unsigned char length = answerEffectiveDataBegin - pBeginNumber;
  // ���������� �����
  SaveNumber(pBeginNumber, length);
}

// ������� ��� (�� receiveGsmPackage) � PDU-�������
void ParseReservedSms()
{
  // ���������� ��, ��� ������� � ���������� ���
  /////////////////////////////////////////////////
  smsCommandIndex = -1;

  // ������ ����� �� ������
  //////////////////////////// 
  // ������� �� ������ sms-������ � �������� ���
  while (!IsSimbolLineBreak(*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) break;
  while (IsSimbolLineBreak(*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) break;
  // ������ ����� ������ sms-������ ������ ����� 0
  answerEffectiveDataBegin++;
  unsigned char i_temp = (*answerEffectiveDataBegin++) - 0x30;
  i_temp <<= 1;
  answerEffectiveDataBegin += i_temp;
  // ����� "��� PDU"
  answerEffectiveDataBegin += 2;
  // ���������� ���� � ������ ������������ ���
  i_temp = GetDecimalFromHexOctet();
  // 2 - ���������� ���������� ���� � ������ � ��� 2 - 91 �� ��������
  answerEffectiveDataBegin += (2);
  // �������� � �������� ��� ����� ���������� �� �������� � ��������� ��� � �����
  unsigned char* iiEnd = answerEffectiveDataBegin + i_temp;
  for (unsigned char* ii=answerEffectiveDataBegin; ii<iiEnd; ii++)
  {
    unsigned char buf = *ii;
    *ii = *(ii+1);
    ii++;
    *ii = buf;
  }    
  SaveNumber(answerEffectiveDataBegin, i_temp);
  // ���������� � ������ �������� ������ 'F', ���� �� ����
  answerEffectiveDataBegin += i_temp;  
  if ('0' != *answerEffectiveDataBegin)
    answerEffectiveDataBegin ++;
  // 2 - ���������� ����� "00" � ��� 1 - ���������� ������ ����� �� "08"
  answerEffectiveDataBegin += (2 + 1);
  // ���� = 08, ������ ��� �������� ��� � 16-������ ������� UCS2
  bool isUCS2 = (8 == ((*answerEffectiveDataBegin++) - 0x30));
  // 14 - 7 ������� ���� � ������� ���
  answerEffectiveDataBegin += 14;
  // ���������� �������� (� 7-������ �������) ��� ������� (� 16-������)
  unsigned char textLength= GetDecimalFromHexOctet();
  if (!textLength)
    return;
  // �������� ����� ��� � ����������� ����
  // ��� ������ ���������� ����� UART
  unsigned char* pRes = &receiveGsmPackage[0];    
  if (isUCS2)
    pRes = Ucs2ToText(pRes, textLength);
  else
  {
    // ��� �������� � 7-������ �������
    unsigned char bits7 = 7; // ������� ����� �� 0 �� 7
    *pRes = 0; // ��������� - ����
    while (textLength)
    {
      unsigned char oct = 8; // 8 ���
      unsigned char data = GetDecimalFromHexOctet(); // ���� �� ������� ������      
      while (oct)
      {
        *pRes >>= 1; // ������� ����� ��� ����
        if (data & 0x01) // ��������� ���
          *pRes |= 0x80;
        data >>= 1; // ��������� ���
        bits7--;
        oct--;
        if (!bits7) // ���� 7 ��� ��� ����, ���� ��������� � ��������� � ��������� 7 �����
        {
          *pRes++ >>= 1; // �������� ���� � ������� ����
          bits7 = 7;
          *pRes = 0;
          textLength--;
          if (!textLength)
            break;
        }
      }
      if ( answerEffectiveDataBegin  > answerEffectiveDataEnd) // ����� ��� "OK" � �����
        break;
    }
  }
  
  // ����������� ������� �� ������ (������� ������ � ������)
  unsigned char* pTextStart = &receiveGsmPackage[0];
  while (IsSimbolLineBreak(*pTextStart))
    if (++pTextStart == pRes) return;
  unsigned char length = pRes - pTextStart;
  // ��������� � ������ ���������� � ������� ��������� ����� � �������
  TranslateSmallToLarge(pTextStart, length);
  
  // ���������� ��� �� ����� ������� � smsCommand
  for (unsigned char indexSmsCommand=0; indexSmsCommand<smsCommandCount; indexSmsCommand++)
    if (smsCommand[indexSmsCommand].realCommandCount <= length)
      if (CompareStrings(smsCommand[indexSmsCommand].command, pTextStart, smsCommand[indexSmsCommand].realCommandCount))
      {
        smsCommandIndex = indexSmsCommand;
        return;
      }  
}

// ������� ������ ���������� �� ��������� gsm
void ParseReservedOperatorGsm()
{
  if ('2' == *answerEffectiveDataBegin)
  {
    dSetTypeSpecMessage_Critical_onZero;
    dResetRezhimSpecMessageTime100msCnt;
    return;
  }
  if (line_ModemPbReady_Error == errorScreen.word)
    if (0 != gsmLevel.generalLevel)
      dSetTypeSpecMessage_NonCritical;
  /*������� ��: �������� ��������� gsm*/
    // GSM-���� �����������
  while ('"' != (*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  /*���������� �������� ��������� gsm*/
  unsigned char* beginGsmOperator = ++answerEffectiveDataBegin;
  while ('"' != (*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  unsigned char lengthOperatorGsm = answerEffectiveDataBegin - beginGsmOperator;
  /*���������� �������� ��������� � ����������� � ������� gsmOperatorUssd*/
  for (unsigned char indexOperatorGsm=0; indexOperatorGsm<gsmOperatorUssdCnt; indexOperatorGsm++)
  {
    unsigned char length = gsmOperatorUssd[indexOperatorGsm].dataCOPS_Cnt;
    if (gsmOperatorUssd[indexOperatorGsm].dataCOPS_Cnt > lengthOperatorGsm)
      length = lengthOperatorGsm;
    /*�������� "MTS" � "MTS RUS" - ����������, ������� ���������� �� ����� ������ ��������� ��������*/
    if (CompareStrings(&(gsmOperatorUssd[indexOperatorGsm].dataCOPS)[0], beginGsmOperator, length))
    {
      gsmOperatorUssd_Index = indexOperatorGsm;
      return;
    }
  }
  gsmOperatorUssd_Index = -1;
}

// ������� ������ �� ������ ussd
void ParseAnswerUssd()
{
  // ���� ������ �����-�����-�����, ��� � ���� ������ � ������
  while ('"' != (*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  unsigned char* pBalans = &balans[0];
  unsigned char* pBalansEnd = &balans[balansCount-1];
  // ���� ����� �� ���� - �� ����� ���� � UCS2-�������, � ��� �� ��� ����� ���� ������� �������
  while ( (*answerEffectiveDataBegin < 0x30) | (*answerEffectiveDataBegin > 0x39) )
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  while ( ((*answerEffectiveDataBegin >= 0x30) & (*answerEffectiveDataBegin <= 0x39)) | (*answerEffectiveDataBegin == '.') | (*answerEffectiveDataBegin == ',') )
  {
    if ( (answerEffectiveDataBegin >= answerEffectiveDataEnd) | (pBalans > pBalansEnd) )
      break;
    else
      *pBalans = *answerEffectiveDataBegin;
    answerEffectiveDataBegin++;
    pBalans++;
  }
}

// ������� ������ // ��������� ���������� ����� � ���������� �����
void ParseAnswerPhoneNumber()
{
  // 1.AT+SMGF=0 // ��� �������� �������� ��������� ��������� ������ 2 � 3
  // 2.AT+CMGS=xx // ���������� ���� � ������ PDU �� ����������� ���� SCA // �����, ����� ��� ������� ����������� �������� '\r' (0Dh), � �� ���-�� ������
  // 3.���� ������ ������ PDU
  
  // ������� �� ������ �������
  while ('"' != *answerEffectiveDataBegin)
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  answerEffectiveDataBegin++; // ������������
  // ������� �� ������ �������
  unsigned char* secondQuote = answerEffectiveDataBegin + 1;
  while ('"' != *secondQuote)
    if (++secondQuote >= answerEffectiveDataEnd) return;
  unsigned char length = secondQuote - answerEffectiveDataBegin;
  
  if (isFindOkNumberInBook)
  {
    if (!isNumberOk(answerEffectiveDataBegin, length)) // ���� ����� ������������, �� ������� �� ������ �����
    {
      if (bookNumbersNeedGetCount)
        dSetFuncsSeries_FindNextNumber; // ��������� ����� �� �����CmdAskNextNumber();
      else
        dSetFindOkNumberInBook_Off;
      return;
    }
    else // ���� ����� ����������, �� ��������� �������� �������
    {
      SdsDataUpdate();
      dBeepSignal_LongOk;
      // if (!isPower220_Alarm_PowerOff)
        sds.val220 = GetVal220(); // ���������� ������� �������� ���������� �� �������� � ��������� ���      
      // unsigned int pause100msSpecMessage = 100;
      if (smsCommandIndex_Monitoring != smsCommandIndex)
      {
        if (smsCommandIndex_Bad == smsCommandIndex)
          sds.smsPartsCount = 2;
        else // if (smsCommandIndex_Bad == smsCommandIndex)
        {
          dSetFindOkNumberInBook_Off;
          dResetBookNumbersCountNeedGet;          
          // ���������� �������� � ������� �������, ������� ������� ���
          dSetPower220_OffThenOn_Off;          
          switch (smsCommandIndex)
          {
            case smsCommandIndex_20_OffThenOn:
            case smsCommandIndex_20_OffThenOn_Rus:
              smsCommandIndex = smsCommandIndex_20_OffThenOn_Rus;
              // ���������, � ����� �������� ��������
              dRelePower220_Off; dSet220commandManualOff_On;
              dSetPower220_OffThenOn_On;
              // pause100msSpecMessage += nastr_SekRestart*10;
              break;
            case smsCommandIndex_220_Off:
            case smsCommandIndex_220_Off_Rus:
              smsCommandIndex = smsCommandIndex_220_Off_Rus;
              dRelePower220_Off; dSet220commandManualOff_On;
              break;
            case smsCommandIndex_220_On:
            case smsCommandIndex_220_On_Rus:
              smsCommandIndex = smsCommandIndex_220_On_Rus;
              RelePower220_On();
              break;
          } // switch (smsCommandIndex)
          // ��� �������� �������� ������ ��� ���������� ��� ������, ������ ����� StartSendSmsToAllNumbers() ������� ��������������
          bookNumbersNeedGetCount--;
          StartSendSmsToAllNumbers();          
        } // if (smsCommandIndex_Bad == smsCommandIndex) .. else
      }
      // ������� ������ ��� ��������� ����� �������� �� �������� ���������� �����
      answerEffectiveDataBegin = secondQuote + 1;
      while ('"' != *answerEffectiveDataBegin)
        if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
      answerEffectiveDataBegin++;
      while ('"' != *answerEffectiveDataEnd)
        answerEffectiveDataEnd--;
      // ������� ��������� �� �������
      LCD_Clear();
      // 1 ������
      // ����� ��������� ������������ ���������� ��������, ������� ������� �� �����
      //  + �������� ����������� �������������� �������� �� answerEffectiveDataEnd
      unsigned char* pLength = Ucs2ToText(pLcdBufBegin, lcdColCount-1);
      GetCharInRow0[pLength-pLcdBufBegin] = ':';
      // 2 ������
      LCD_ShowString(smsCommand[smsCommandIndex].command, smsCommand[smsCommandIndex].userCommandCount, 2, 1);
      // �������������
      RezhimSpecMessage_On(pLcdBufBegin, 0, BIT1);
    } // if (!isNumberOk(answerEffectiveDataBegin, length)) ... else
  }
  else  // if (isFindOkNumberInBook)
    if (isSendSmsToAllPhonesInBook)
      SaveNumber(answerEffectiveDataBegin, length);
  // �������� ������ ����� ���
  dSetFuncsSeries_SendSmsToNumberFromPhoneBook;
}

// ������� ������ // ������ ����� ��� 
// ���� �������� ��������, �������� ���������� ������ (����������� �� ����� �����)
void ParseAnswerCmgs()
{
  if (*answerEffectiveDataEnd != '>')
    return;
#ifdef use_buffer_for_send_sms
  unsigned char udl = PreparePduForSms(); // ����� ����� ������ ������ ���������, ����� �������� � �������
  SendStringToUart(&receiveGsmPackage[0], (udl << 1) + 2);
#else
  PreparePduForSms(); // ����� ����� ������ ������ ���������, ����� �������� ���
#endif
  SendCharToUart((unsigned char)0x1A);
  // ��� �������, ������� ���� "������� ������� AT+CMGS" // ������ ������� ��� �������� ���
  dSetSentForSms_Cmgs_Off;
  // ���� �������� ��������, �������� ���������� ������ (����������� �� ����� �����)
  dSetSendSmsComplete_On; // ������ ���� "������ �������� �����" // "��" �� �����
  // SendStringToUart �� ����������, ������� dSetWaitAnswer_Off; dSetIndexReservedMsgAfterWaitAnswer_Off; �� ����
}
