// �������� ������� ������

#ifndef __gsm__
#define __gsm__

#include "gsm_mainTitle.hpp"
#include "rus1_mainTitle.hpp"

#include "example_base_types.hpp"




///////////////////
// ����

// ��������� �� �������
typedef void (*pFuncSendToUart)();

// ������� ��������
#ifdef useGeneralFlag
  #define receiveGsmPackageMax 173
#else 
  #define receiveGsmPackageMax 160 //  ������ 13 ���� - ��� ����� ������������ �� f_generalFlag
#endif
#define beepOkMasCnt 1
#define beepBadMasCnt 1
#define beepLongBadMasCnt 5 // 85 // 3 ���� �� 1000 �� � ������ 300 ��, � 33 ���� �� 1000 �� � ������ 10 ������
#define beepMasCount 4
#define balansCount 7
#define msgGsmCnt 10
#ifdef testRezhim
  #define funcsSendToUart_OnStartCnt 43
#else
  #define funcsSendToUart_OnStartCnt 16
#endif
#define funcsSendSmsToUart_SmsCnt 2
#define funcsSendToUart_Test_OftenCnt 2
#define funcsSendToUart_Test_PerHourCnt 8
#define f_funcsSendToUart_FindNumberOkCnt 1
#define f_funcsSendToUart_CmdCpbs_SimCardCnt 1
#define f_funcsSendToUart_isModemReadyCnt 1
#define f_funcsSendToUart_SendSmsToNumberFromPhoneBookCnt 1



///////////////////
// ����

// ��������� �� �������
typedef void (*pFuncSendToUart)();


///////////////////////////////
// ��������� � ������� ��������

/////////////////////////////////
// ������ ��� �������� ���
/////////////////////////////////
//  �� ����� �������� ���, ����� �������� ������������ ������.
//  ��� ����� �������� � ����, ���:
// - �������� ��� ���������� � ������� (������ ����� ��� �� �������� ��� ������ ����� ���).
// - � ����� �������� ������ ��������� �� ���������� ����� ������������ ������ �������� ������.
// - � ����� �������� ������ ������������ ���� ���.
//  ����� ����� ��������, ��������� ��� ������������ �� ����� �������� ������ � t_smsDataSend.
//  ��� ��, ��������� ���:
// - pBuffer, ��� ���������� ������� ���� // �� 300 ����
// - "��������� �����", ��� �������� "�������" ���
// - ������� ������������ ���, ��� �������� "�������" ���
struct t_smsDataSend
{
  unsigned char* pBuffer;        // ��� ���������� ������� ���� �� ������
  // "�������" ���
  unsigned char smsRandomNumber; // "��������� �����", ��� �������� "�������" ���
  unsigned char smsPartsCount;   // ���������� ������, �� ������� ������� ������������ ��� // ��� �������� "�������" ���
  unsigned char smsPartIndex;    // ������� ������������ ������ ��� // ��������� � ������� // ��� �������� "�������" ���
  // ������
  unsigned int rest220_seconds;  // 10/100 // ������� ������ �� ��������� ������, ��� ���������� ������� �������
  unsigned int val220;           // 90/100 // ���������� 220
  bool is200wasUp;               // ���� true, �� ���������� ���������� ���� �����������, ����� ���� �����������
  bool isRele220_On;             //  ���� ���������� ��������/���������
  bool is220_Alarm_PowerOff;     // "�������� ��������� �������� ��-�� ������ ��� ���������� �� ������� �����������"
  unsigned char gsmLevel;        // 90/100 // ������� gsm-�������
};


//////////////////////////////// 
// ���������� �����

#define bookNumbersCount phoneBook.f_numbersCount
#define bookNumbersNeedGetCount phoneBook.f_numbersNeedGetCount
#define bookNextNumberCPBR phoneBook.f_nextNumberCPBR

// INC // ��������� ��� ����������� ������, ������� ���� ��������� �������� AT+CPBR=x
#define dIncBookNumberCPBR { bookNextNumberCPBR++; \
           if (bookNextNumberCPBR > bookNumbersCount) bookNextNumberCPBR = 1; } 
#define dDecBookNumbersCountNeedGet { bookNumbersNeedGetCount--; \
           if (!bookNumbersNeedGetCount) bookNextNumberCPBR = 0; \
           else dIncBookNumberCPBR; }
#define dResetBookNumbersCountNeedGet { phoneBook.f_numbersNeedGetCount = 0; }

// ����� �������� ����� ���� ������� �� ����� �����
// #define dStartSendSmsToAllNumbers { dSetSendSmsToAllPhonesInBook_On; dStartFindAllPhonesInBook; dSetFuncsSeries_FindNumberOk; }
// void StartSendSmsToAllNumbers() { dSetSendSmsToAllPhonesInBook_On; dStartFindAllPhonesInBook; dSetFuncsSeries_FindNumberOk; }

#define pNumberNeedSend (&phoneBook.f_numberNeedSend[0])
#define numberNeedSendCount phoneBook.f_numberNeedSendCount
#define dSetNumberNeedSendCount(x) { phoneBook.f_numberNeedSendCount = x; }
#define numberNeedSendMax 10
struct t_phoneBook
{
  // ���������� ������� � ����� // �����
  unsigned char f_numbersCount;        
  // ��������� ��� ����������� ������, ������� ���� ��������� �������� AT+CPBR=x 
  // ����� �������� �� � 1, � � ������ �������, ��� ���� "�� �����" ����� ������������� ��� �������
  unsigned char f_nextNumberCPBR;      
  // ��. isSendSmsToAllPhonesInBook 
  // ���������� �������, ������� ��� ��� �������� �� ����� � ������� �� ��� ���;
  // ���� �� ����� ����, ������ ���� ������ ������
  unsigned char f_numbersNeedGetCount; 
  // ����� ������ f_numberNeedSend 
  // ���� ����, �� �� ���� ������ ���������� ���
  unsigned char f_numberNeedSendCount; 
  // �����, �� ������� ���� ������� �������� ��� 
  // ����������� ����� SaveNumber(), ��� ������������ 
  // ����� ������ CheckPhoneNumber - ������� ����. 10 ����
  unsigned char f_numberNeedSend[numberNeedSendMax]; 
};

// ��������� ������� � �������� smsCommand � smsAnswer // ���� -1, �� �� �������
#define smsCommandIndex commandAndAnswerIndex.f_smsCommandIndex
#define smsAnswerIndex  commandAndAnswerIndex.f_smsAnswerIndex
struct t_CommandAndAnswerIndex
{
  signed char f_smsCommandIndex;
  // signed char f_smsAnswerIndex;
};
/*
//////////////////////////////// 
// ������ �� �������
#define smsAnswersCount 1
struct t_smsAnswer
{
  // �������
  const unsigned char* answer;
  // ����� �������
  const unsigned char answerCount;
} const smsAnswer[smsAnswersCount] = {
  {&ansAT[0], ansOkCnt}, // ��
  };
*/

// sms-������� ������
#define smsCommandCount 7
struct t_smsCommand
{
  // �������
  const unsigned char* command;
  // ����� ������� �� �����
  const unsigned char realCommandCount;
  // ����� ������� ��� ������������
  const unsigned char userCommandCount;
};


/////////////////////////////////
// define
/////////////////////////////////

// ������� 100�� ����� ����� ��������� �� ���������
#define gsmDefaultPauseBetweenCommands 1


//  ������ ������ ��� smsCommandIndex = smsCommandIndex_Bad:
// ? - ����������
// ������������, RESTART - ���� � ��������
// ��������, ON - ��������
// ���������, OFF - ���������
// ������� - �������
// ������123.
#define smsCommandIndex_Bad          -1
//  ��� smsCommandIndex = smsCommandIndex_Monitoring:
/*- ���� ����������� ������� RESTART, �� ������������ ��� ��� ��� ������ �� ������� smsCommandIndex_20_OffThenOn, 
  � ��������� ���������� ������ �� ��������� ���� (������� �������������).
- � ��������� �������:*/
// ���� ���������
// ���������� 220�
// ������ 100%
// ��� ������� 200
// ������100,9
#define smsCommandIndex_Monitoring   0

//  ������ ������ ��� smsCommandIndex = smsCommandIndex_20_OffThenOn:
// ������� "������������": ���� ���. ����� 86400�.
// ���������� 220�
// ������100,9
#define smsCommandIndex_20_OffThenOn 1
#define smsCommandIndex_20_OffThenOn_Rus 4

//  ������ ������ ��� smsCommandIndex = smsCommandIndex_220_Off:
// ������� "���������" ���������
// ���� ���������
// ���������� 220�
// ������100,9
#define smsCommandIndex_220_Off      2
#define smsCommandIndex_220_Off_Rus  5

//  ������ ������ ��� smsCommandIndex = smsCommandIndex_220_On:
// ������� "��������" ���������
// ���� ��������
// ���������� 220�
// ������100,9
#define smsCommandIndex_220_On       3
#define smsCommandIndex_220_On_Rus   6

// �������� ������
struct t_beep
{
  // ������ ������� ��������� ������� beepMas "������ - ����� - ������ - ..." � 100��
  // 0 - �������� ������ � ������� ����������
  // 1 - �������� ������ � ������ ����������
  // 2 - ������ �������� ������ � ������ ���������� // ���������� �� ��������� ����������� // ���������� GSM-��������
  // 3 - ��������� �������� ������ � ���������� ������������� ������� // ������� �� ���
  char f_beepMasIndex; // ���� -1, �� ��������� ������� ��������� �� ����
  // ������ � "�������" ������� beepDelay ��������� t_beep
  char f_beepDelay100msIndex; // ���� -1, �� �������� ������ ��� �� �������
  // ������� 100��, ��� ������� ������� � beepMas[beepMasIndex], ��� ������� ��������� �������
  unsigned int f_beepSignal_100msCnt;
};

struct t_beepMas
{
  const unsigned char beepDelayCount; // ����� beepDelay
  const unsigned char* beepDelay;     // ������ ��������� ������� "������ - ����� - ������ - ..." � 100��
};


// �������� gsm � ��� ussd-������� ����������� �������
#define gsmOperatorUssdCnt 4
struct t_gsmOperatorUssd 
{
  const unsigned char* dataCOPS; // ��������� gsm, ���������� �������� AT+COPS? // ������� CmdCopsGet()
  const unsigned char* dataCOPSussdBalance; // USSD-������� ��������� ������� ��������� gsm
  const unsigned char dataCOPS_Cnt; // ����� dataCOPS
  const unsigned char dataCOPSussdBalance_Cnt; // ����� dataCOPSussdBalance
};

// ������� gsm-������� � ��� ��������
struct t_gsmLevel
{
  // ������� �������, % 
  // level = x * 100 / 31
  // x ����� ���� � ��������� 0..31 ��� 99. ��� ������ ��� �����, �� 99 ������ ��� ����������. 
  // unsigned char level;
  // ����������� ������ �����, %
  // quality = 100 - x * 100 / 7
  // x ����� ���� �� 0 �� 7, ��� ����� ������ ��� �������� ����� �����.
  // unsigned char quality; 
  
  // ����� ������� �������
  // generalLevel = level * quality / 100
  unsigned char generalLevel;
};

// ������� ����������� �������
//////////////////////////////////////////// 
// ��������� �� ������� ����������� �������
// � �����
// ���������� 100�� ����� ��������� ������ � �������� ��������� ������� �� �������
struct t_funcsSeries
{
  const pFuncSendToUart* nextFunc;
  unsigned char funcsCnt;
  unsigned char waitBetween_100ms;
};

// ��������� �� ��������� � ����� ����� � ��� �����
// ��� ��������� �� ������
struct t_errorScreen
{
  const unsigned char* word;
  // unsigned char wordCnt; // ����� ������ = (lcdColCount*lcdRowCount)
};

// �������� ���
// ������� �� ��� � ����� ������ �: ���� ( 0 != sms.smsTextCnt)
/////////////////////////////////////////////////
// ��������� �� �����, ������ ���
// ��������� �� ����� ���������
// ����� ����� ���������
// ����� ������, ������ ���
// ��������� �� ����� ������, ����� �� ���� ����
// ��������� �� �����, ����� �� ���� ����
// ��������� �� ���, ����� �� ���� ����
// ��������� �� ����, ����� �� ���� ����
// ��������� �� ������, ����� �� ���� ����
// ��������� �� �������, ����� �� ���� ����
// ��������� �� ������������, ����� �� ���� ����
/*struct t_Sms
{
  unsigned char* smsNumber;
  unsigned char* smsText;
  unsigned char smsNumberCnt;
  unsigned char smsTextCnt;
  unsigned char* day;
  unsigned char* month;
  unsigned char* year;
  unsigned char* hour;
  unsigned char* minutes;
  unsigned char* seconds;
  unsigned char* milliseconds;
} sms = { NULL, NULL, 0 , 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
*/



//////////// 
// defines
//////////// 



// �������� ������
//////////////// 
// ������ ������� ��������� ������� beepMas "������ - ����� - ������ - ..." � 100��
// f_beepMasIndex
#define beepMasIndex beep.f_beepMasIndex
#define isBeepMasSignal ( (char)-1 != beep.f_beepMasIndex ) // ������� ��������� ������� ��������? // ���� ������ �������� ������?
// #define dSetBeepMasIndex(x) { beep.f_beepMasIndex = x; beep.f_beepDelay100msIndex = -1;}
#define dResetBeepMasIndex { beep.f_beepMasIndex = -1; } // ������� ��������� �������
#define isEndBeepMasIndex ( ( (beepMas[beepMasIndex].beepDelayCount - 1) <= beep.f_beepDelay100msIndex ) \
                           & (isBeepDelay100ms_Began) )
// f_beepDelay100msIndex
#define beepDelay100msIndex beep.f_beepDelay100msIndex
#define isBeepDelay100ms_End ( (beepMas[beepMasIndex].beepDelayCount - 1) != beep.f_beepDelay100msIndex ) // ����� �� ����� beepDelay ?
#define isBeepDelay100ms_Began ( (char)-1 != beep.f_beepDelay100msIndex ) // ��� �� ������� ����
#define dSetBeepDelay100msIndex_Next { beep.f_beepDelay100msIndex++; \
            dSetBeepSignal_100msCnt(beepMas[beep.f_beepMasIndex].beepDelay[beep.f_beepDelay100msIndex]); }
// f_BeepSignal_100msCnt
#define isBeepSignal_100msCnt_Wait ( 0 != beep.f_beepSignal_100msCnt )
#define dDecBeepSignal_100msCnt_Wait { if (beep.f_beepSignal_100msCnt) beep.f_beepSignal_100msCnt--; } // ��������� ��������
#define dSetBeepSignal_100msCnt(x) { beep.f_beepSignal_100msCnt = x; }

// �������� ���������� ������� �� ����� ������: �������� ������ � ������� ����������
#define dBeepSignal_Good { SetBeepMasIndex(0); }
// �������� ���������� ������� �� ����� ������: �������� ������ � ������ ����������
#define dBeepSignal_Bad { SetBeepMasIndex(1); }
// �������� ���������� ������� �� ����� ������: ������ �������� ������ � ����� ������ ����������
#define dBeepSignal_LongBad { SetBeepMasIndex(2); }
#define isBeepSignal_LongBad ( 2 == beep.f_beepMasIndex )
// �������� ���������� ������� �� ����� ������: ������ �������� ������ � ����� ������� ���������� // � ���������� ������������� �������
#define dBeepSignal_LongOk { SetBeepMasIndex(3); }

// f_beepDelayIndex
#define dIncBeepDelayIndex { beep.f_beepDelay100msIndex++; }
#define dResetBeepDelayIndex { beep.f_beepDelay100msIndex = 0; }




#ifdef useGeneralFlag
/////////////////////////////
// f_generalFlag
////////////////////////// 

// ���� 0-3 - ����� ��������� ������ ����� ��� ���������

// ����� ������ "����� ������ ��� ��������� � ������� ���������"
#define isModemStartUp ( 0 != (f_generalFlag & bit_ModemStartUp) )
// ������������� ��� "����� ������ ��� ��������� � ������� ���������"
#define dSetModemStartUp_On { f_generalFlag |= bit_ModemStartUp; }
// ���������� ��� "����� ������ ��� ��������� � ������� ���������"
#define dSetModemStartUp_Off { f_generalFlag &= ~bit_ModemStartUp;  }

// ����� ������ "����� ����� ����� � ������� ����������������� � GSM-����"
#define isPbReady ( 0 != (f_generalFlag & bit_PbReady) )
// ������������� ��� "����� ������� ����������������� � GSM-����"
#define dSetPbReady_On { f_generalFlag |= bit_PbReady; }
// ���������� ��� "����� ������� ����������������� � GSM-����"
#define dSetPbReady_Off { f_generalFlag &= ~bit_PbReady; }

// ����� ��������� ������ ��������� � ������� ���������� ������� � �����
#define isSetupOnStartUp ( 0 != (f_generalFlag & bit_SetupOnStartUp) )
// ������������� ��� "����� ��������� ������ ��������� � ������� ���������� ������� � �����"
#define dSetSetupOnStartUp_On { f_generalFlag |= bit_SetupOnStartUp; }
// ���������� ��� "����� ��������� ������ ��������� � ������� ���������� ������� � �����"
#define dSetSetupOnStartUp_Off { f_generalFlag &= ~bit_SetupOnStartUp; }

// ����� ������������ ������� �� ������� ������� ������� "ready"
#define isModemReady ( 0 != (f_generalFlag & bit_ModemReady) )
// ������������� ��� "����� ������������ ������� �� ������� ������� ������� "ready"
#define dSetModemReady_On { f_generalFlag |= bit_ModemReady; }
// ���������� ��� "����� ������������ ������� �� ������� ������� ������� "ready"
#define dSetModemReady_Off { f_generalFlag &= ~bit_ModemReady; }


////////////// 
// ����� �����

// "������� ������� Cmgf_On"
#define isCmgf_On ( 0 != (f_generalFlag & bit_Cmgf_On) )
#define dSetCmgf_On_On { f_generalFlag |= bit_Cmgf_On; }
#define dSetCmgf_On_Off { f_generalFlag &= ~bit_Cmgf_On; }

////////////////////////// 
// ������� ������

// "�� ��������� ���������� ������� gsm-������, �������� ����� ������ �������������, ���� �� �����"
#define isRezhimSpecMessage_OffOnEndFuncsSeries ( 0 != (f_generalFlag & bit_RezhimSpecMessage_OffOnEndFuncsSeries) )
#define dSetRezhimSpecMessage_OffOnEndFuncsSeries_On { f_generalFlag |= bit_RezhimSpecMessage_OffOnEndFuncsSeries; }
#define dSetRezhimSpecMessage_OffOnEndFuncsSeries_Off { RezhimSpecMessage_Off(); \
                                                        f_generalFlag &= ~bit_RezhimSpecMessage_OffOnEndFuncsSeries; \
                                                        dRedLed_Off; \
                                                        dGreenLed_Off; \
                                                        dResetGreenLedSignalCnt; }

// "������� �� ������ �� ��������� ������� ������" // ������� ��������� � �� �� �����
#define isWaitAnswer ( 0 != (f_generalFlag & bit_WaitAnswer) )
// ������������� ��� "������� �� ������ �� ��������� ������� ������"
#define dSetWaitAnswer_On { f_generalFlag |= bit_WaitAnswer; }
// ���������� ��� "������� �� ������ �� ��������� ������� ������"
#define dSetWaitAnswer_Off { f_generalFlag &= ~bit_WaitAnswer; }

///////////////////
// ����������

// ������� ��������� ���� �������� OFF
#define is220commandOff ( 0 != (f_generalFlag & bit_220commandOff) )
// ������������� ��� "������� �� ������ �� ��������� �������"
#define dSet220commandOff_On { f_generalFlag |= bit_220commandOff; }
// ���������� ��� "������� �� ������ �� ��������� �������"
#define dSet220commandOff_Off { f_generalFlag &= ~bit_220commandOff; }

// "����� � �������� ����������, � ����� ��������� ��������"
#define isPower220_PauseThenOn ( 0 != (f_generalFlag & bit_Power220_OffThenOn) )
#define dSetPower220_OffThenOn_On { f_generalFlag |= bit_Power220_OffThenOn; restart220_seconds = nastr_SekRestart; }
#define dSetPower220_OffThenOn_Off { f_generalFlag &= ~bit_Power220_OffThenOn; }

// "�������� ��������� �������� ��-�� ������ ��� ���������� �� ������� �����������"
#define isPower220_Alarm_PowerOff ( 0 != (f_generalFlag & bit_Power220_Alarm_PowerOff) )
#define dSetPower220_Alarm_PowerOff_On { f_generalFlag |= bit_Power220_Alarm_PowerOff; }
#define dSetPower220_Alarm_PowerOff_Off { f_generalFlag &= ~bit_Power220_Alarm_PowerOff; }

// ��������� ��������� ��� �� ��������� ���������� ���� �������� ���������
#define isNeedSendSmsOnPower220_Alarm_PowerOff ( 0 != (f_generalFlag & bit_NeedSendSmsOnPower220_Alarm_PowerOff) )
#define dSetNeedSendSmsOnPower220_Alarm_PowerOff_On { f_generalFlag |= bit_NeedSendSmsOnPower220_Alarm_PowerOff; }
#define dSetNeedSendSmsOnPower220_Alarm_PowerOff_Off { f_generalFlag &= ~bit_NeedSendSmsOnPower220_Alarm_PowerOff; }

//////// 
// SMS

// ���� � ���������� ����� �����, ����������� � ������� ���������� ��� ��� ���������
#define isFindOkNumberInBook ( 0 != (f_generalFlag & bit_FindOkNumberInBook) )
// ������������� ��� "������� ������� AT+CMGS"
#define dSetFindOkNumberInBook_On { f_generalFlag |= bit_FindOkNumberInBook; }
// ���������� ��� "������� ������� AT+CMGS"
#define dSetFindOkNumberInBook_Off { f_generalFlag &= ~bit_FindOkNumberInBook; }

// ������� ������� AT+CMGS // ������ ������� ��� �������� ���
#define isSentForSms_Cmgs ( 0 != (f_generalFlag & bit_SentForSms_Cmgs) )
// ������������� ��� "������� ������� AT+CMGS"
#define dSetSentForSms_Cmgs_On { f_generalFlag |= bit_SentForSms_Cmgs; }
// ���������� ��� "������� ������� AT+CMGS"
#define dSetSentForSms_Cmgs_Off { f_generalFlag &= ~bit_SentForSms_Cmgs; }

// "������ ���-�������� ���� ������� � �����"
#define isSendSmsToAllPhonesInBook ( 0 != (f_generalFlag & bit_SendSmsToAllPhonesInBook) )
// ������������� ��� "������ ���-�������� ���� ������� � �����"
#define dSetSendSmsToAllPhonesInBook_On { f_generalFlag |= bit_SendSmsToAllPhonesInBook; } // ������������ ������ � dStartFindAllPhonesInBook
// ���������� ��� "������ ���-�������� ���� ������� � �����"
#define dSetSendSmsToAllPhonesInBook_Off { f_generalFlag &= ~bit_SendSmsToAllPhonesInBook; }

// ��������� �������� ���
#define isSendSmsComplete ( 0 != (f_generalFlag & bit_SendSmsComplete) )
// ������������� ��� "��������� �������� ���"
#define dSetSendSmsComplete_On { f_generalFlag |= bit_SendSmsComplete; }
// ���������� ��� "��������� �������� ���"
#define dSetSendSmsComplete_Off { f_generalFlag &= ~bit_SendSmsComplete; }

#else // #ifdef useGeneralFlag

// ����� ��������� ������ ����� ��� ���������

// ����� ������ "����� ������ ��� ��������� � ������� ���������"
#define isModemStartUp f_generalFlag.f_isModemStartUp
#define dSetModemStartUp_On { f_generalFlag.f_isModemStartUp = true; }
#define dSetModemStartUp_Off { f_generalFlag.f_isModemStartUp = false; }

// ����� ������ "����� ������� ����������������� � GSM-����"
#define isPbReady f_generalFlag.f_isPbReady
#define dSetPbReady_On { f_generalFlag.f_isPbReady = true; }
#define dSetPbReady_Off { f_generalFlag.f_isPbReady = false; }

// ����� ��������� ������� ������ ���������
#define isSetupOnStartUp f_generalFlag.f_isSetupOnStartUp
#define dSetSetupOnStartUp_On { f_generalFlag.f_isSetupOnStartUp = true; }
#define dSetSetupOnStartUp_Off { f_generalFlag.f_isSetupOnStartUp = false; }

// ����� ������������ ������� �� ������� ������� ������� "ready"
#define isModemReady f_generalFlag.f_isModemReady
#define dSetModemReady_On { f_generalFlag.f_isModemReady = true; }
#define dSetModemReady_Off { f_generalFlag.f_isModemReady = false; }


////////////////////////// 
// ������ ������

// "�� ��������� ���������� ������� gsm-������, �������� ����� ������ �������������, ���� �� �����"
#define isRezhimSpecMessage_OffOnEndFuncsSeries f_generalFlag.f_isRezhimSpecMessage_OffOnEndFuncsSeries
#define dSetRezhimSpecMessage_OffOnEndFuncsSeries_On { f_generalFlag.f_isRezhimSpecMessage_OffOnEndFuncsSeries = true; }
#define dSetRezhimSpecMessage_OffOnEndFuncsSeries_Off { RezhimSpecMessage_Off(); \
                                                        f_generalFlag.f_isRezhimSpecMessage_OffOnEndFuncsSeries = false; }

// "������� �� ������ �� ��������� ������� ������" // ������� ��������� � �� �� �����
#define isWaitAnswer f_generalFlag.f_isWaitAnswer
#define dSetWaitAnswer_On { f_generalFlag.f_isWaitAnswer = true; }
#define dSetWaitAnswer_Off { f_generalFlag.f_isWaitAnswer = false; }


///////////////////
// ����������

// ������� ��������� ���� �������� OFF
#define is220commandManualOff f_generalFlag.f_is220commandManualOff
#define dSet220commandManualOff_On { f_generalFlag.f_is220commandManualOff = true; }
#define dSet220commandManualOff_Off { f_generalFlag.f_is220commandManualOff = false; }

// "����� � �������� ����������, � ����� ��������� ��������"
#define isPower220_PauseThenOn f_generalFlag.f_isPower220_OffThenOn
#define dSetPower220_OffThenOn_On { f_generalFlag.f_isPower220_OffThenOn = true; restart220_seconds = nastr_SekRestart; }
#define dSetPower220_OffThenOn_Off { f_generalFlag.f_isPower220_OffThenOn = false; }

// "�������� ��������� �������� ��-�� ������ ��� ���������� �� ������� �����������"
#define isPower220_Alarm_PowerOff f_generalFlag.f_isPower220_Alarm_PowerOff
#define dSetPower220_Alarm_PowerOff_On { f_generalFlag.f_isPower220_Alarm_PowerOff = true; }
#define dSetPower220_Alarm_PowerOff_Off { f_generalFlag.f_isPower220_Alarm_PowerOff = false; }

// ��������� ��������� ��� �� ��������� ���������� ���� �������� ���������
#define isNeedSendSmsOnPower220_Alarm_PowerOff f_generalFlag.f_isNeedSendSmsOnPower220_Alarm_PowerOff
#define dSetNeedSendSmsOnPower220_Alarm_PowerOff_On { f_generalFlag.f_isNeedSendSmsOnPower220_Alarm_PowerOff = true; }
#define dSetNeedSendSmsOnPower220_Alarm_PowerOff_Off { f_generalFlag.f_isNeedSendSmsOnPower220_Alarm_PowerOff = false; }


//////// 
// SMS

// "������� ������� Cmgf_On"
#define isCmgf_On f_generalFlag.f_isCmgf_On
#define dSetCmgf_On_On { f_generalFlag.f_isCmgf_On = true; }
#define dSetCmgf_On_Off { f_generalFlag.f_isCmgf_On = false; }

// ���� � ���������� ����� �����, ����������� � ������� ���������� ��� ��� ���������
#define isFindOkNumberInBook f_generalFlag.f_isFindOkNumberInBook
#define dSetFindOkNumberInBook_On { f_generalFlag.f_isFindOkNumberInBook = true; }
#define dSetFindOkNumberInBook_Off { f_generalFlag.f_isFindOkNumberInBook = false; }

// ������� ������� AT+CMGS // ������ ������� ��� �������� ���
#define isSentForSms_Cmgs f_generalFlag.f_isSentForSms_Cmgs
#define dSetSentForSms_Cmgs_On { f_generalFlag.f_isSentForSms_Cmgs = true; }
#define dSetSentForSms_Cmgs_Off { f_generalFlag.f_isSentForSms_Cmgs = false; }

// "������ ���-�������� ���� ������� � �����"
#define isSendSmsToAllPhonesInBook f_generalFlag.f_isSendSmsToAllPhonesInBook
#define dSetSendSmsToAllPhonesInBook_On { f_generalFlag.f_isSendSmsToAllPhonesInBook= true; } // ������������ ������ � dStartFindAllPhonesInBook
#define dSetSendSmsToAllPhonesInBook_Off { f_generalFlag.f_isSendSmsToAllPhonesInBook = false; }

// ��������� �������� ���
#define isSendSmsComplete f_generalFlag.f_isSendSmsComplete
#define dSetSendSmsComplete_On { f_generalFlag.f_isSendSmsComplete = true; }
#define dSetSendSmsComplete_Off { f_generalFlag.f_isSendSmsComplete = false; }

#endif // #else // #ifdef useGeneralFlag



#define dRelePower220_Off { P1OUT |= dRelePower220_Bit; }

// ������� ������ // ��������� � ���������� ����� // ���������� �������
// void ParseAnswerBookInfo() {
#define dParseAnswerBookInfo() \
  /*������� �� �������*/ \
  while (',' != *answerEffectiveDataBegin) \
    if (++answerEffectiveDataBegin == answerEffectiveDataEnd) return; \
  answerEffectiveDataBegin++; \
  bookNumbersCount = (*answerEffectiveDataBegin++) - 0x30; \
  while (',' != *answerEffectiveDataBegin) \
    bookNumbersCount = (10 * bookNumbersCount + ((*answerEffectiveDataBegin++) - 0x30)); \
  if (!bookNumbersCount) /*���� ��� ���� ����������*/ \
  { \
    errorScreen.word = lines_NoSimCardNumbers; \
    dResetRezhimSpecMessageTime100msCnt;/*RezhimSpecMessage_On(errorScreen.word, 0, (BIT4 | BIT5 | BIT7));*/ \
  } \
  else \
  { \
    dSetSetupOnStartUp_On; \
    RezhimSpecMessage_Off(); \
  } \
// }





///////////////////////////////////
// f_funcsQueue

// ������ ������� ���������� ������
#define dResetFuncsSeries { f_funcsSeries = { NULL, 0, 0 }; }
// ������� ����������� ������� ������� � ������� ������, ����� ������� �������� �������
#define dResumeLastCommandFuncsSeries { \
  funcsSeries.nextFunc--; \
  funcsSeries.funcsCnt++; \
  dResetFromLastCommand100ms; \
  dResetReceiveGsmPackageCount; \
  dSetWaitAnswer_Off; \
  dSetModemReady_On; }

// ��������� ��������� ������� ������
// void dFuncsSeriesSendNextCommand()
#define dFuncsSeriesSendNextCommand \
{ if (isModemReady) \
  { \
    if (NULL != *(funcsSeries.nextFunc)) (*funcsSeries.nextFunc)(); \
    funcsSeries.nextFunc++; \
    funcsSeries.funcsCnt--; \
    dResetFromLastCommand100ms; \
    dSetModemReady_Off; \
  } \
  else { CmdCpas(); } \
}
/*
#define dFuncsSeriesSendNextCommand \
  { \
    if (NULL != *(funcsSeries.nextFunc)) (*funcsSeries.nextFunc)(); \
    funcsSeries.nextFunc++; \
    funcsSeries.funcsCnt--; \
    dResetFromLastCommand100ms; \
    dSetModemReady_Off; \
  }
*/




// ������ ������� ������
////////////////////////////////////////////////// 

// ������ ������� ������ ��� ������ ������ f_FuncsSendToUart_OnStart
#define dSetFuncsSeries_OnPbReady SetFuncsSeries( f_FuncsSendToUart_OnStart, \
                                                  funcsSendToUart_OnStartCnt, \
                                                  gsmDefaultPauseBetweenCommands )

// ������ ������� ������ �� ������������ ������ ������ �������� ���������� �������
#define dSetFuncsSeries_TestModem_PerHour SetFuncsSeries( f_FuncsSendToUart_Test_PerHour, \
                                                          funcsSendToUart_Test_PerHourCnt, \
                                                          gsmDefaultPauseBetweenCommands )

// ������ ������� ������ �� ������������ ������ ������ ���
#define dSetFuncsSeries_TestModem_Often SetFuncsSeries( f_FuncsSendToUart_Test_Often, \
                                                        funcsSendToUart_Test_OftenCnt, \
                                                        gsmDefaultPauseBetweenCommands )
// ������ ������� ������: ����� ������� � ���������� ����� �����
#define dSetFuncsSeries_FindNextNumber SetFuncsSeries( f_funcsSendToUart_FindNumberOk, \
                                                       f_funcsSendToUart_FindNumberOkCnt, \
                                                       gsmDefaultPauseBetweenCommands )


// ������ ������� ����������: ��������� ���������� �� ���-�����, ���� ����� �����, ������� �� ���-����� � ������
// ����� ���� ������� 1c
// ������ 10 �������� � ���������� (5*gsmDefaultPauseBetweenCommands)*100��
#define dSetFuncsSeries_CmdCpbs_SimCard SetFuncsSeries( f_funcsSendToUart_CmdCpbs_SimCard, \
                                                        f_funcsSendToUart_CmdCpbs_SimCardCnt, \
                                                        (5) )

// ������ ������� ������: �������� ������ ����� ��� ������ �� phoneBook
#define dSetFuncsSeries_SendSmsToNumberFromPhoneBook SetFuncsSeries( f_funcsSendToUart_SendSmsToNumberFromPhoneBook, \
                                                                     f_funcsSendToUart_SendSmsToNumberFromPhoneBookCnt, \
                                                                     gsmDefaultPauseBetweenCommands)

/*
// ������ ������� ������ ��� ����� GPS f_FuncsSendToUart_GpsTest
// #define dSetFuncsSeries_GpsTest SetFuncsSeries( f_FuncsSendToUart_GpsTest, funcsSendToUart_GpsTestCnt )

// ������ ������� ����������: ������ ���������� ������� � �������� ������
#define dSetFuncsSeries_CmdBookInfo SetFuncsSeries( f_funcsSendToUart_CmdBookInfo, \
                                                    f_funcsSendToUart_CmdBookInfoCnt, \
                                                    gsmDefaultPauseBetweenCommands )

// ������ ������� ������: �������� �����, ����� �� �� � ������
#define dSetFuncsSeries_isModemReady SetFuncsSeries( f_funcsSendToUart_isModemReady, \
                                                     f_funcsSendToUart_isModemReadyCnt, \
                                                     gsmDefaultPauseBetweenCommands )

// ������ ������� ����������: �������� ����� �� ���������� // ����� ������ ����������� ������� ����� ������������ �� ����������
#define dSetFuncsSeries_ModemReady SetFuncsSeries( NULL, \
                                                        1, \
                                                        gsmDefaultPauseBetweenCommands )

// ������ ������� ������: ��������� PDU-������ � ���
#define dSetFuncsSeries_Sms_Pdu SetFuncsSeries( f_funcsSendToUart_Sms_Pdu, \
                                                f_funcsSendToUart_Sms_PduCnt, \
                                                gsmDefaultPauseBetweenCommands )

// ������ ������� ������: ��������� GSM-������ � ���
#define dSetFuncsSeries_Sms_Gsm SetFuncsSeries( f_funcsSendToUart_Sms_Gsm, \
                                                f_funcsSendToUart_Sms_GsmCnt, \
                                                gsmDefaultPauseBetweenCommands )
*/

// ��� ������ � �������� ��� ������ ������
#define isSmsReceived ( 0 != sms.smsTextCnt )
// "�������" ��� (����� ���, ��� ��������� ��������� �����)
#define dSms_Clear { sms.smsTextCnt = 0; }





// ������������ ������� ������ ����� ��������� �������
// ����� ����� ������ � ����� ERROR
#define modemAnswerCntMax 10


/////////////////////////////////////////////////////////////////////////
// ��� ���������
#define msgGsm_ModemStartUp   0
#define msgGsm_PbReady        1
#define msgGsm_Ring           2
#define msgGsm_Sms            3
#define msgGsm_Level          4
#define msgGsm_InfoOperator   5
#define msgGsm_Ussd           6
#define msgGsm_BookInfo       7
#define msgGsm_PhoneNumber    8
#define msgGsm_Status         9

#define msgGsm_Smgs            10  // 10 - ������ ������� ��� (�������� � ������ PDU) // ����� �� AT+CMGS=xx
#define msgGsm_SmsSendComplete 11  // 11 - ��������� ������� ��� (�������� � ������ PDU)
#define msgGsm_CmdCpbs_SimCard 12  // 12 - ��������� ���������� �� ���-�����, ���� ����� �����, ������� �� ���-����� � ������




////////////////////////////////// 
// ����� 100�� ��� ������ � �������
////////////////////////////////// 

// ������� 100 �� ��������� �� �������� �������� ������ �� �������
#define gsmWaitAnswer_100ms 255

// ������� 100 �� ��������� �� �������� ����� ���������� ������ [��� ���������/��������]
#define gsmPowerOnWait_100ms 30


// ����� ������� 100�� ����� ��������� ������� �������� �������� �������
#define gsmTestCommandDelay_100ms 300



// bool
///////////////////////

// ����� ���� ��������
#define isModemNeedPowerOn (fromLastCommand100ms >= gsmPowerOnWait_100ms)

// ������� �������� ������ �� ������� ������
#define isEndWaitAnswerOfCommand_100ms (fromLastCommand100ms >= gsmWaitAnswer_100ms)

// ���������� �������� �������� � ������ AT-�������
#define receiveGsmPackageCount (pReceiveGsmPackageActive - &receiveGsmPackage[0])

// "����� ������� ��������� �������"
#define isModemWaitForPowerOn (0 == (bit_ModemStartUp & f_generalFlag))

// ������ ���� ������� �������� �������
#define isGsmTestCommandNeed (fromLastCommand100ms >= gsmTestCommandDelay_100ms)



//////////////////////// 
// �������� ����������
////////////////////// 

#ifdef useGeneralFlag

// ����� ��������� ������ ����� ��� ���������
////////////////////////////////////////////////

// ����� ������ "MODEM:STARTUP" 
// "����� ������������, ��� ������� ���������" [� ������� ���������]
#define bit_ModemStartUp   BIT0 

// ����� ������ "+PBREADY"
// "����� ����� ����� � ������� ����������������� � GSM-����"
#define bit_PbReady        BIT1 

// ����� ��������� ������ ��������� � ������� ���������� ������� � �����
#define bit_SetupOnStartUp BIT3 

// ����� ������������ ������� �� ������� ������� ������� "ready": �����
#define bit_ModemReady     BIT4 

// �������������: �����������, ��� GSM-���� �����������
#define bit_NoGsmConnect             BIT5 

// ������� ������
///////////////////

// "�� ��������� ���������� ������� gsm-������, �������� ����� ������ �������������, ���� �� �����"
#define bit_RezhimSpecMessage_OffOnEndFuncsSeries BIT5 

// "������� ������ �� ��������� ������� ������" 
// ������� ��������� � �� �� �����
#define bit_WaitAnswer               BIT6 

// ����������
/////////////////////

// ������� ��������� ���� �������� OFF // ����� ��������� �������������, ��� ������ ���������� �� �����
#define bit_220commandManualOff      BIT7 

// "����� � �������� ����������, � ����� ��������� ��������" 
// ������� ������� RESTART
#define bit_Power220_OffThenOn       BIT8 

// "�������� ��������� �������� ��-�� ������ ��� ���������� �� ������� �����������"
#define bit_Power220_Alarm_PowerOff  BIT9 

// ��������� ��������� ��� �� ��������� ���������� ���� �������� ���������
#define bit_NeedSendSmsOnPower220_Alarm_PowerOff BITA 

// SMS
// �������� "Flow chart for sending text mode SMS" ��������� "M590 AT COMMAND SETS
// V3.0", ��� ��������� ���������� ������ ��� ��������� ���������� "AT+CMGF=1" 2 ���� ������.
// �� ��� ����, �� ������ ��� ����� neoway m590 "OK" � ������ �� ����������.
///////////////

// ������� ������� Cmgf_On
#define bit_Cmgf_On                  BITB 

// ���� � ���������� ����� �����, ����������� � ������� ���������� ��� ��� ��������� 
// ��� �����������, ��� ����� �������
#define bit_FindOkNumberInBook       BITC 

// ������� ������� AT+CMGS 
// ������ ������� ��� �������� ��� 
// ��� PreparePduForSms
#define bit_SentForSms_Cmgs          BITD 

// "������ ���-�������� ���� ������� � �����"
#define bit_SendSmsToAllPhonesInBook BITE 

// ��������� �������� ��� 
// ����� ����� ���� ����� �������, ��� �� ������ � ������ 
// ���� "������ �������� �����"
#define bit_SendSmsComplete          BITF 

#else // #ifdef useGeneralFlag

struct t_generalFlag
{
// ����� ��������� ������ ����� ��� ���������
//////////////////////////////////////////////

// "����� ������������, ��� ������� ���������" [� ������� ���������] 
// ����� ������ "MODEM:STARTUP"
bool f_isModemStartUp;              

// ����� ������ "+PBREADY"
// "����� ����� ����� � ������� ����������������� � GSM-����"
bool f_isPbReady;                   

// ����� ��������� ������� ������ ���������
bool f_isSetupOnStartUp;            

// ����� ������������ ������� �� ������� ������� ������� "ready": �����
bool f_isModemReady;                

// �������������: �����������, ��� GSM-���� �����������
bool f_isNoGsmConnect;              

// ������� ������
////////////////////

// "�� ��������� ���������� ������� gsm-������, �������� ����� ������ 
// �������������, ���� �� �����"
bool f_isRezhimSpecMessage_OffOnEndFuncsSeries; 

// "������� ������ �� ��������� ������� ������" 
// ������� ��������� � �� �� �����
bool f_isWaitAnswer;                

// ����������
////////////////

// ������� ��������� ���� �������� OFF 
// ����� ��������� �������������, ��� ������ ���������� �� �����
bool f_is220commandManualOff;       

// "����� � �������� ����������, � ����� ��������� ��������" 
// ������� ������� RESTART
bool f_isPower220_OffThenOn;        

// "�������� ��������� �������� ��-�� ������ ��� ���������� �� ������� �����������"
bool f_isPower220_Alarm_PowerOff;   

// ��������� ��������� ��� �� ��������� ���������� ���� �������� ���������
bool f_isNeedSendSmsOnPower220_Alarm_PowerOff; 

// SMS
// �������� "Flow chart for sending text mode SMS" ��������� "M590 AT COMMAND SETS
// V3.0", ��� ��������� ���������� ������ ��� ��������� ���������� "AT+CMGF=1" 2 ���� ������.
// �� ��� ����, �� ������ ��� ����� neoway m590 "OK" � ������ �� ����������.
////////////////////////////////////////////////////////

// ������� ������� Cmgf_On
bool f_isCmgf_On;                   

// ���� � ���������� ����� �����, ����������� � ������� ���������� ��� ��� ��������� 
// ��� �����������, ��� ����� �������
bool f_isFindOkNumberInBook;        

// ������� ������� AT+CMGS 
// ������ ������� ��� �������� ��� 
// ��� PreparePduForSms
bool f_isSentForSms_Cmgs;           

// "������ ���-�������� ���� ������� � �����"
bool f_isSendSmsToAllPhonesInBook;  

// ��������� �������� ��� 
// ����� ����� ���� ����� �������, ��� �� ������ � ������ 
// ���� "������ �������� �����"
bool f_isSendSmsComplete;           
};
#endif // #ifdef useGeneralFlag ... #else


// f_timerCounter_AtPauseDelay
#define GetTimerCounter_AtPauseDelay f_timerCounter_AtPauseDelay
#define dIncTimerCounter_AtPauseDelay { f_timerCounter_AtPauseDelay++; }
#define dResetTimerCounter_AtPauseDelay { f_timerCounter_AtPauseDelay=0; }


// fromLastCommand100ms
////////////////////////// 

// ��������� �������� 100�� ����� ������� ��������� ������� ������
#define dIncFromLastCommand100ms { if (0xFFFF != fromLastCommand100ms) fromLastCommand100ms++; }
// ����� �������� 100�� ����� ������� ��������� ������� ������
#define dResetFromLastCommand100ms { fromLastCommand100ms = 0; }



// f_funcsQueue
///////////////////////////////////

// ���� �� ��� � ������� ������ ������
#define isExistsNextCommand ( funcsSeries.funcsCnt != 0 )
// ���� ��������� ��������� ������� �� ������� ������
#define dFuncsSeriesNeedStart (fromLastCommand100ms >= funcsSeries.waitBetween_100ms)
// �������� ���������� ������ �� ������� ������
#define dFuncsSeries_Abort { funcsSeries.funcsCnt = 0; }


///////////////////////////
// extern
///////////////////////////

// ���������� �����
extern t_phoneBook phoneBook;

// ��������� ������� � �������� smsCommand � smsAnswer // ���� -1, �� �� �������
extern t_CommandAndAnswerIndex commandAndAnswerIndex;

// sms-������� ������
extern t_smsCommand const smsCommand[smsCommandCount];

// �������� ������
extern t_beep beep;

// 0 - �������� ������ � ������� ����������
extern const unsigned char beepOkMas[beepOkMasCnt];
// 1 - �������� ������ � ������ ����������
extern const unsigned char beepBadMas[beepBadMasCnt];
// 2 - ������ �������� ������ � ����� ������ ����������
// ������� 5, �������� 85; �� �� �������� ���� ������� ������� �������, ��������� ������ ������� flash-������
extern const unsigned char beepLongBadMas[beepLongBadMasCnt];
// 3 - ��������� �������� ������ � ����� ������� ���������� 
//  � ���������� ������������� ������� 
// ������� �� ���

// ����� 10 ��� �� 700 �� � ������ 300 ��, � 33 ���� �� 700 �� � ������ 10 ������
#define beepLongOkMasCnt 5 
extern const unsigned char beepLongOkMas[beepLongOkMasCnt];
extern t_beepMas const beepMas[beepMasCount];

// ��������� gsm // ����� ��� �����������, ����� ussd-������� ������������ ��� ����������� �������
// 0 - ������
// 1 - �������
// 2 - ���
extern signed char gsmOperatorUssd_Index;
extern const t_gsmOperatorUssd gsmOperatorUssd[gsmOperatorUssdCnt];

// ������� gsm-������� � ��� ��������
extern t_gsmLevel gsmLevel;

// ������� ����������� �������
extern t_funcsSeries funcsSeries;

// ��������� �� ��������� � ����� ����� � ��� �����
extern t_errorScreen errorScreen;

/////////////////////////////////
// ������ ��� �������� ���
extern t_smsDataSend sds;

// ����� ����� ������ �� UART
// ����� ������ receiveGsmPackage �����, ��������� ������ ������
// ��� ����������� ������ ������ � ���������� ����� � PDU-������� � ������ ����� �������� �� 16 �������� ��
// ������ ������ ������� (��������� ������� � ������������ �������� ������ ����� ��������������), ���� 
// ������� 113 ����
extern unsigned char receiveGsmPackage[receiveGsmPackageMax];
// ��������� �� ��������� ������ ��� ����������, � receiveGsmPackage
extern unsigned char* pReceiveGsmPackageActive;

// ���������� ������������ � ���������� TIMER, ��� �������� ��� ����� ��-������
extern unsigned int f_timerCounter_AtPauseDelay;

// ������ ����� // ������� ��� ����� ����� � ��������� ����� // ������ �� ������ �� ussd-������ � ������� �����
extern unsigned char balans[balansCount];

// ������� ������ ����� ��������� �������
extern unsigned char modemAnswerCnt;

/////////////////////////////////////////////////////////////////////////
// ������ ���������� �� ��� ��������� ������ ��������� ������ � �� �������
// ��� ���������
extern const t_strAndStrCnt msgGsm[msgGsmCnt];

////////////////////////////////////////////////////////////////////////////// 
// ������ ���������� �� ������� �������� ������ ������ ����� ������ ������
extern const pFuncSendToUart f_FuncsSendToUart_OnStart[funcsSendToUart_OnStartCnt];

#ifdef testRezhim
// ������� ������ ��� ������� ��������� ���
extern const pFuncSendToUart f_FuncsSendToUart_Sms[funcsSendSmsToUart_SmsCnt];
#endif



// ������� ������ ��� ������������ ������
///////////////////////////////////////////
// ������ �������� ���������� �������
extern const pFuncSendToUart f_FuncsSendToUart_Test_Often[funcsSendToUart_Test_OftenCnt];

// ������ ���
extern const pFuncSendToUart f_FuncsSendToUart_Test_PerHour[funcsSendToUart_Test_PerHourCnt];

// ������� ������: ����� ���������� ������ � ���������� ����� �����
extern const pFuncSendToUart f_funcsSendToUart_FindNumberOk[f_funcsSendToUart_FindNumberOkCnt];

// ������� ������: ��������� ���������� �� ���-�����, ���� ����� �����, ������� �� ���-����� � ������
extern const pFuncSendToUart f_funcsSendToUart_CmdCpbs_SimCard[f_funcsSendToUart_CmdCpbs_SimCardCnt];

// ������� ������: �������� �����, ����� �� �� � ������
extern const pFuncSendToUart f_funcsSendToUart_isModemReady[f_funcsSendToUart_isModemReadyCnt];

// ������� ������: �������� ������ ����� ��� ������ �� phoneBook
extern const pFuncSendToUart f_funcsSendToUart_SendSmsToNumberFromPhoneBook[f_funcsSendToUart_SendSmsToNumberFromPhoneBookCnt];

#ifdef useGeneralFlag
extern unsigned int f_generalFlag;
#else
extern t_generalFlag f_generalFlag;
#endif


// ������� 100�� �� ��������� ��������� ������� ������
// ��� �������� ������� �� �������� ������ ������ ����� �������� ��������� ������� � ����� ������ �������
// ��� ����� ������ ��� ������ � ���, ������������ ������ � f_generalFlag
extern unsigned int fromLastCommand100ms;

// ��������� �� ������ �������� ���������� � ������ // ��� ��������� � �������� ������
// ������ �������� ���������� - ����� � ������ ������ ��� ������� �������� ������ � �������, � ��� �� ���-�����
extern unsigned char* answerEffectiveDataBegin;

// ��������� �� ����� �������� ���������� � ������ // ��� ��������� � �������� ������
// ����� �������� ���������� - ����� � ����� ������ ����� �� ��� ������� �������� ������ � �������
extern unsigned char* answerEffectiveDataEnd;




/////////////////////////////
// ���������

// ���������� beep.f_beepMasIndex
void SetBeepMasIndex(unsigned char x);

// ���� 220
// void RelePower220_Off();
void RelePower220_On();

// ����� �������� ���� ������� � ���������� ����� �����
void StartFindAllPhonesInBook();

// �������� �� ������ �������� �������� ������ ��� �������
bool IsSimbolLineBreak(unsigned char simb);
  
// ���������� ���������� ����� �� 2 �������� � answerEffectiveDataBegin, ������� ��������� +2
// ����������������� ����� ������ ���� ����������
unsigned char GetDecimalFromHexOctet();

// �������� ����� � 10-�������� ���� 906 123 45 67
unsigned char* CheckPhoneNumber(unsigned char* number, unsigned char* numberCnt);

// ��������� ����� � phoneBook
void SaveNumber(unsigned char* num, unsigned char numCnt);

// ������������ ������
void RestartModem();

// ���������� ������� ���������� ������
void SetFuncsSeries(const pFuncSendToUart* f, unsigned char cnt, unsigned char pause100ms = 0);

// ����� �������� ����� ���� ������� �� ����� �����
void StartSendSmsToAllNumbers();

// ����� ������ � ����� ������, ������������ � ������� pNumberNeedSend
void StartFindPhoneNumberOk();

// ����������� ����� �� ����� � ����������, �������� �� �� ��� ������ � ���
bool isNumberOk(unsigned char* number, unsigned char numberCnt);

// ����������� ���������, ��������� � uart: �������� ������ ��������� � msgGsm
signed char IndexReservedMsg();

// ���������� � ��������� pSms ����� word � ������� unicode; ��������� ��������� �� ��������� ��� ���������� ������ � receiveGsmPackage ��� ����������
void AddToPointUCS2(const unsigned char* word, unsigned char wordCnt);
// ��������� � ��������� ��� � UART �������� ����� � ����������������� ������� 0x00, ��� ���� ����� 0x00 ����������� � ��������� ��� � �������� ��� �����
// ��������� ��������� �� ��������� ��� ���������� ������ � receiveGsmPackage ��� ����������
void AddToPointUCS2(unsigned char number);

// ������� PDU-����� ���, ���������� ���������� ���� � ������ PDU �� ����������� ���� SCA (��� ������� AT+CMGS=xx)
// PDU-����� ��������� ������ � ������� AddToPointUCS2, ��-�� use_buffer_for_send_sms // ��� ���������� �������
unsigned char PreparePduForSms();

/////////////////////////
// ��������

// ������� ������ gsm-������� ��: ������� � ��������
void ParseReservedLevelGsm();

// ������� ��������� ������
void ParseReservedRing();

// ������� ��� (�� receiveGsmPackage) � PDU-�������
void ParseReservedSms();

// ������� ������ gsm-������� ��: ������� � ��������
// void ParseReservedLevelGsm();

// ������� ������ ���������� �� ��������� gsm
void ParseReservedOperatorGsm();

// ������� ������ �� ������ ������ ussd
void ParseAnswerUssd();

// ������� ������ ��� // ����� ������� � �� // ���� �� �����, ���������� -1
void ParseTextSms(unsigned char* smsText, unsigned char smsTextCnt);

// ������� ������ // ��������� � ���������� ����� // ���������� �������
// void ParseAnswerBookInfo();

// ������� ������ // ��������� ���������� ����� � ���������� �����
void ParseAnswerPhoneNumber();

// ������� ������ // ������ ����� ���
void ParseAnswerCmgs();

#endif // #ifndef __gsm__