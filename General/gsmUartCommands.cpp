// работа с gsm

// GSM-МОДУЛЬ NEOWAY M590 ПОСЛЕ РЕСТАРТА ВСЕ ВВЕДЁННЫЕ НАСТРОЙКИ СБРАСЫВАЕТ

// используем команду без эха
// эхо используем для идентификации принятых ответов, для каких команд пришёл ответ
// команды в Uart GSM-модему

#define gsmUartCommands

#include "msp430G2553.h"

#include "gsmUartCommands.hpp"
#include "gsm.hpp"
#include "peripherals.hpp"
#include "example_base_functions.hpp"
#include "example_base_strings.hpp"

// посылает символ в uart
void SendCharToUart(unsigned char simbol)
  { \
    while (!(IFG2&UCA0TXIFG)) dWatchDogReset; \
    UCA0TXBUF = simbol; \
  }

// посылает строку в uart И СТАВИТ ОЧЕРЕДЬ НАСТРОЕК
void SendStringToUart( const unsigned char* str, unsigned char strCnt)
  {
    GreenLedSignal_ON();
    for (unsigned char const* s=str;s<(str+strCnt);s++)
      {
#ifdef use_WatchDog_for_PUC
        dWatchDogReset;
#endif
        while (!(IFG2&UCA0TXIFG)) dWatchDogReset;
        UCA0TXBUF = *s;
      }
    dResetFromLastCommand100ms;
    dSetWaitAnswer_On; /*на любую команду модему следует ожидать ответа от него*/
#ifdef testRezhim
    errorScreen.word = str;
    // errorScreen.wordCnt = strCnt;
#endif
  }


// посылает в uart символ окончания команды, для модема
void SendEndSimbolToUart() 
{ 
  while (!(IFG2&UCA0TXIFG)); UCA0TXBUF = (unsigned char)13;
  while (!(IFG2&UCA0TXIFG)); UCA0TXBUF = (unsigned char)10; 
}


// посылаем первую часть смс номеру из phoneBook
  // 1.AT+SMGF=0 // при парсинге которого выполняем следующие пункты 2 и 3
  // 2.AT+CMGS=xx // количество байт в пакете PDU за исключением поля SCA // Важно, чтобы эта команда завершалась символом '\r' (0Dh), а не чем-то другим
  // 3.Ввод данных пакета PDU
#define sendSmsBufTempCnt 3
void SendSmsToNumberFromPhoneBook()
{  
  unsigned char udl = PreparePduForSms(); 
  
  // в качестве буфера используем последние ячейки буфера UART - всё равно они в крайнем случае потом заполняются строками
  LongToStr(udl, &receiveGsmPackage[receiveGsmPackageMax - sendSmsBufTempCnt], sendSmsBufTempCnt); 
  
  SendStringToUart(cmdSendSmsStart, cmdSendSmsStartCnt); 
  SendStringToUart(&receiveGsmPackage[receiveGsmPackageMax - sendSmsBufTempCnt],  GetLengthOfSignedLong(udl)); 
  
  // закрываем команду
  SendCharToUart(0x0D);  
  dSetSentForSms_Cmgs_On; // ставим флаг "послана команда AT+CMGS" // первая команда при отправке смс
  dSetWaitAnswer_Off; // "OK" не ждать
}

/*
///////////////////////
// GPS-команды в UART
////////////////////// 

// команды для модуля SIM808
/////////////////////////////

// команда включения GPS-модуля
// GPS power control
#define strCmdGpsPowerOnCnt 12
unsigned char const strCmdGpsPowerOn[strCmdGpsPowerOnCnt] = {'A','T','+','C','G','P','S','P','W','R','=','1'};
void CmdGpsPwr_On() { SendStringToUart(strCmdGpsPowerOn, strCmdGpsPowerOnCnt); SendEndSimbolToUart; }

// команда выключения GPS-модуля
// Close GPS
#define strCmdGpsCloseCnt 12
unsigned char const strCmdGpsClose[strCmdGpsCloseCnt] = {'A','T','+','C','G','P','S','P','W','R','=','0'};
void CmdGpsClose() { SendStringToUart(strCmdGpsClose, strCmdGpsCloseCnt); SendEndSimbolToUart; }

// команда получения информации GPS
// Get currenr GPS location info 
#define strCmdGpsInfo_0Cnt 12
unsigned char const strCmdGpsInfo_0[strCmdGpsInfo_0Cnt] = {'A','T','+','C','G','P','S','I','N','F','=','0'};
void CmdGpsInfo_0() { SendStringToUart(strCmdGpsInfo_0, strCmdGpsInfo_0Cnt); SendEndSimbolToUart; }

// команда получения статуса GPS
// Get currenr GPS status
#define strCmdGpsStatus_GetCnt 14
unsigned char const strCmdGpsStatus_Get[strCmdGpsStatus_GetCnt] = {'A','T','+','C','G','P','S','S','T','A','T','U','S','?'};
void CmdGpsStatus_Get() { SendStringToUart(strCmdGpsStatus_Get, strCmdGpsStatus_GetCnt); SendEndSimbolToUart; }

// команда GPS NMEA data output from AT UART
// GPS NMEA data output control = GSA
#define strCmdGpsPsout_8Cnt 12
unsigned char const strCmdGpsPsout_8[strCmdGpsPsout_8Cnt] = {'A','T','+','C','G','P','S','O','U','T','=','8'};
void CmdGpsPsout_8() { SendStringToUart(strCmdGpsPsout_8, strCmdGpsPsout_8Cnt); SendEndSimbolToUart; }

// команда GPS NMEA data output from AT UART
// GPS NMEA data output control = RMC
#define strCmdGpsPsout_32Cnt 13
unsigned char const strCmdGpsPsout_32[strCmdGpsPsout_32Cnt] = {'A','T','+','C','G','P','S','O','U','T','=','3','2'};
void CmdGpsPsout_32() { SendStringToUart(strCmdGpsPsout_32, strCmdGpsPsout_32Cnt); SendEndSimbolToUart; }

// команда Reset GPS (Cold Start)
// Reset GPS (Cold Start)
#define strCmdGpsRst_0Cnt 12
unsigned char const strCmdGpsRst_0[strCmdGpsRst_0Cnt] = {'A','T','+','C','G','P','S','R','S','T','=','0'};
void CmdGpsRst_0() { SendStringToUart(strCmdGpsRst_0, strCmdGpsRst_0Cnt); SendEndSimbolToUart; }

// команда Reset GPS (Hot Start)
// Reset GPS (Hot Start)
#define strCmdGpsRst_1Cnt 12
unsigned char const strCmdGpsRst_1[strCmdGpsRst_1Cnt] = {'A','T','+','C','G','P','S','R','S','T','=','1'};
void CmdGpsRst_1() { SendStringToUart(strCmdGpsRst_1, strCmdGpsRst_1Cnt); SendEndSimbolToUart; }


// команды для модуля A7
/////////////////////////////

// Read Current GPS ASCII data Command Format Query
// AT+GPSRD=?
#define strCmdGpsRd_QueryCnt 10
unsigned char const strCmdGpsRd_Query[strCmdGpsRd_QueryCnt] = {'A','T','+','G','P','S','R','D','=','?'};
void CmdGpsRd_Query() { SendStringToUart(strCmdGpsRd_Query, strCmdGpsRd_QueryCnt); SendEndSimbolToUart; }

// Read Current GPS ASCII data in RMC
// AT+GPSRD=10,0
// #define strCmdGpsRd_10Cnt 13
// unsigned char const strCmdGpsRd_10[strCmdGpsRd_10Cnt] = {'A','T','+','G','P','S','R','D','=','1','0',',','0'};
#define strCmdGpsRd_10Cnt 11
unsigned char const strCmdGpsRd_10[strCmdGpsRd_10Cnt] = {'A','T','+','G','P','S','R','D','=','1','0'};
void CmdGpsRd_10() { SendStringToUart(strCmdGpsRd_10, strCmdGpsRd_10Cnt); SendEndSimbolToUart; }

// Open GPS
// AT+GPS=1
#define strCmdGps_1Cnt 8
unsigned char const strCmdGps_1[strCmdGps_1Cnt] = {'A','T','+','G','P','S','=','1'};
void CmdGps_1() { SendStringToUart(strCmdGps_1, strCmdGps_1Cnt); SendEndSimbolToUart; }

// Close GPS
// AT+GPS=0
#define strCmdGps_0Cnt 8
unsigned char const strCmdGps_0[strCmdGps_0Cnt] = {'A','T','+','G','P','S','=','0'};
void CmdGps_0() { SendStringToUart(strCmdGps_0, strCmdGps_0Cnt); SendEndSimbolToUart; }

// Open AGPS
// AT+AGPS=1
#define strCmdAGps_1Cnt 9
unsigned char const strCmdAGps_1[strCmdAGps_1Cnt] = {'A','T','+','A','G','P','S','=','1'};
void CmdAGps_1() { SendStringToUart(strCmdAGps_1, strCmdAGps_1Cnt); SendEndSimbolToUart; }

// Close AGPS
// AT+AGPS=0
#define strCmdAGps_0Cnt 9
unsigned char const strCmdAGps_0[strCmdAGps_0Cnt] = {'A','T','+','A','G','P','S','=','0'};
void CmdAGps_0() { SendStringToUart(strCmdAGps_0, strCmdAGps_0Cnt); SendEndSimbolToUart; }
*/


///////////////////////
// GSM-команды в UART
////////////////////// 


//////////////// 
// USSD-команды

// USSD-запрос баланса // используется gsmOperatorUssd и gsmOperatorUssd_Index
void CmdAskUssdBalance() 
{ 
  if ((signed char)-1 == gsmOperatorUssd_Index)
    return;
/*  if (isSetupOnStartUp) // если не настроен модем, значит это при загрузке модема, когда горит сообщение "Настройка модема"
  {
    RezhimSpecMessage_On(modemUssdWaitAnswer, 0, BIT1);
    dSetRezhimSpecMessage_OffOnEndFuncsSeries_On;
  }*/
  SendStringToUart(gsmOperatorUssd[gsmOperatorUssd_Index].dataCOPSussdBalance, 
                   gsmOperatorUssd[gsmOperatorUssd_Index].dataCOPSussdBalance_Cnt); 
  SendEndSimbolToUart();
}

//////////////////////////// 
// другие команды

#ifdef testRezhim // здесь в тестрежиме - все подпрограммы, которые не используются в обычном режиме

  // отправка смс-сообщения smsText на номер number в GSM-кодировке (без русских букв)
  // Message sending:+CMGS
#define cmdSendSmsStartNumberCnt 3
  unsigned char const cmdSendSmsStartNumber[cmdSendSmsStartNumberCnt] = 
    //  0   1                                   10                                          21
    { '"','+','7' };
void SendSms(unsigned char const* smsText, unsigned char smsTextCnt, unsigned char const* number, unsigned char numberCnt) 
  {     
    // 1. Определяем модему номер.
    // 2. Пауза между командами.
    // 3. Посылаем модему смс-текст.
    
    // 1. Определяем модему номер.
    SendStringToUart(cmdSendSmsStart, cmdSendSmsStartCnt);
    SendStringToUart(cmdSendSmsStartNumber, cmdSendSmsStartNumberCnt);   
    SendStringToUart(number, numberCnt);
    while (!(IFG2&UCA0TXIFG)); UCA0TXBUF = '"';
    while (!(IFG2&UCA0TXIFG)); UCA0TXBUF = (unsigned char)13; // Enter
      
    // 2. Пауза между командами.
    // DelayMilliS(5000);
    
    // 3. Посылаем модему смс-текст.
    SendStringToUart(smsText, smsTextCnt);
    while (!(IFG2&UCA0TXIFG)); UCA0TXBUF = (unsigned char)26; // Ctrl-Z
  }

    // пример:    { 'A','T','+','C','M','G','S','=','"','+','7','9','0','6','1','3','0','1','9','8','0','"' };

// посылка смс по частям
// первая часть тестовой смс
void CmdSendTestSms_part1() 
{ 
  
  // sms gsm
  SendStringToUart(strCmdSendTestSms_part1, strCmdSendTestSms_part1Cnt); 
  SendCharToUart((unsigned char)13); // Enter
  dSetWaitAnswer_Off;
  
  /*
  // sms pdu
  SendStringToUart(cmdSendSmsStart, cmdSendSmsStartCnt);
  while (!(IFG2&UCA0TXIFG)); UCA0TXBUF = (unsigned char)27;
  while (!(IFG2&UCA0TXIFG)); UCA0TXBUF = (unsigned char)13;
  
  // DelayMilliS(5000);
  
  // SendStringToUart(&receiveGsmPackage[0], udl+1 + 7);
  SendStringToUart(&tmp[0], 28);
      while (!(IFG2&UCA0TXIFG)); UCA0TXBUF = (unsigned char)26; // (0x1A);
      dSetWaitAnswer_Off;
  */
}
// вторая часть тестовой смс
#define strCmdSendTestSms_part2Cnt 2
unsigned char const strCmdSendTestSms_part2[strCmdSendTestSms_part2Cnt] = { 'O','K' };
void CmdSendTestSms_part2() 
{ 
  // SendStringToUart(lineLCD_OK, lcdColCount); 
  SendStringToUart(strCmdSendTestSms_part2, strCmdSendTestSms_part2Cnt); 
  SendCharToUart((unsigned char)26); // Ctrl-Z
}

// тестовый звонок
#define strTestRingCnt 16
unsigned char const strTestRing[strTestRingCnt] = { 'A','T','D','+','7','9','0','6','1','3','0','1','9','8','0',';' };
void CmdTestRing() { SendStringToUart(strTestRing, strTestRingCnt); SendEndSimbolToUart(); }

// Это так называемая пустая команда. 
// После получения этой команды модем автоматически подстраивает скорость передачи и формат знака к параметрам терминала.
#define strCmdAtCnt 2
unsigned char const strCmdAt[strCmdAtCnt] = {'A','T'};
void CmdAt() { SendStringToUart(strCmdAt, strCmdAtCnt); SendEndSimbolToUart(); }

// Check network registration status
// AT+CREG?
#define strCmdCregCnt 8
unsigned char const strCmdCreg[strCmdCregCnt] = { 'A','T','+','C','R','E','G','?' };
void CmdCreg() { SendStringToUart(strCmdCreg,   strCmdCregCnt); SendEndSimbolToUart(); }

// This command is to check the information of the module, including factory, model and version.
// ati
#define strCmdAtiCnt 3
unsigned char const strCmdAti[strCmdCmeeGetCnt] = { 'A','T','I' };
void CmdAti() { SendStringToUart(strCmdAti,   strCmdAtiCnt); SendEndSimbolToUart(); }

// Get the module’s baud rate
// AT+IPR?
#define strCmdIprGetCnt 7
unsigned char const strCmdIprGet[strCmdIprGetCnt] = { 'A','T','+','I','P','R','?' };
void CmdIprGet() { SendStringToUart(strCmdIprGet,   strCmdIprGetCnt); SendEndSimbolToUart(); }

// считать номер Центра SMS из GSM-модуля
// AT+CSCA?
#define strCmdCscaInfoCnt 8
unsigned char const strCmdCscaInfo[strCmdCscaInfoCnt] = { 'A','T','+','C','S','C','A','?' };
void CmdCscaInfo() { SendStringToUart(strCmdCscaInfo,   strCmdCscaInfoCnt); SendEndSimbolToUart(); }

#endif // #ifdef testRezhim

// Команда проверки статуса модуля
// AT+CPAS
// ответ:
// 0: ready(the module can implement AT commands)
// 2: unknow(unknown status)
// 3: ringing(the module can implement AT command,it will be ringing status when
//    there is an incoming call)
// 4: call in progress(the module can implement AT command, in call connecting or
//    caller ringing status.)
// 5: asleep(Module is in sleep mode, not ready)
// от себя (из "Руководство по использованию АТ-команд для GSM/GPRS модемов"): 1 - недоступно (невозможны команды)
#define strCmdCpasCnt 7
// unsigned char const strCmdCpas[strCmdCpasCnt] = { 'A','T','+','C','P','A','S' };
#define strCmdCpas dataATCPAS
void CmdCpas() { SendStringToUart(strCmdCpas,   strCmdCpasCnt); SendEndSimbolToUart(); }

// команда установки текстового режима
/////////////////////////////////////////////////////////////////////////////////////////////
//    Согласно "Flow chart for sending text mode SMS" документа "M590 AT COMMAND SETS
// V3.0", для установки текстового режима смс требуется отправлять "AT+CMGF=1" 2 раза подряд.
//    При этом, согласно "Flow chart for sending PDU mode SMS" из того же документа, для 
// установки PDU-режима смс требуется отправлять "AT+CMGF=0" только 1 раз.
//////////////////////////////////////////////////////////////////////////////////////////// 
// This command is to set SMS input mode.
// If sending PDU SMS, the code should be set to UCS2; if sending text SMS, the code should be set to GSM.
#define strCmdCmgfCnt 9
// текстовый режим ON // GSM mode
unsigned char const strCmdCmgf_On[strCmdCmgfCnt] =  { 'A','T','+','C','M','G','F','=','1'};
void CmdCmgf_On()  
{ 
//    Согласно "Flow chart for sending text mode SMS" документа "M590 AT COMMAND SETS
// V3.0", для установки текстового режима смс требуется отправлять "AT+CMGF=1" 2 раза подряд.
// но при этом, во второй раз модем neoway m590 "OK" в ответе не отправляет.
  bool before = isCmgf_On; // почему-то
  SendStringToUart(strCmdCmgf_On,  strCmdCmgfCnt); 
  SendEndSimbolToUart();
  if (before)
  // во второй раз модем "OK" не отправляет
    dSetWaitAnswer_Off;
  dSetCmgf_On_On;
}
// текстовый режим OFF // PDU mode
unsigned char const strCmdCmgf_Off[strCmdCmgfCnt] = { 'A','T','+','C','M','G','F','=','0'};
void CmdCmgf_Off() 
{ 
  SendStringToUart(strCmdCmgf_Off, strCmdCmgfCnt); 
  SendEndSimbolToUart(); 
  dSetCmgf_On_Off;
}


// AT+CSQ - информация о качестве сигнала.
// Модуль вернет два числа +CSQ, например: 17,0
// 17 - уровень сигнала. Может быть в диапазоне 0..31 или 99. Чем больше тем лучше, но 99 значит его отсутствие. 
// 0 - коэффициент ошибок связи (т.н. RXQUAL). Может быть от 0 до 7, чем число меньше тем качество связи лучше.
#define strCmdAskLevelCnt 6
// unsigned char const strCmdAskLevel[strCmdAskLevelCnt] = { 'A','T','+','C','S','Q'};
#define strCmdAskLevel dataATCSQ
void CmdAskLevel() { SendStringToUart(strCmdAskLevel, strCmdAskLevelCnt); SendEndSimbolToUart(); }

// AT+CLIP=1 - включение определения номера входящего звонка
// Если отправить команду с 0, то определение номера отключится и модуль вместо номера вызывающего будет слать только сообщение "RING".
#define strCmdAonCnt 9
unsigned char const strCmdAon_On [strCmdAonCnt] = { 'A','T','+','C','L','I','P','=','1' };
void CmdAon_On()  { SendStringToUart(strCmdAon_On,  strCmdAonCnt); SendEndSimbolToUart(); }
unsigned char const strCmdAon_Off[strCmdAonCnt] = { 'A','T','+','C','L','I','P','=','0' };
void CmdAon_Off() { SendStringToUart(strCmdAon_Off, strCmdAonCnt); SendEndSimbolToUart(); }

// AT+CSCS="GSM" - определение кодировки текста
// AT+CSCS=<chset> - Set TE character sets:
// < chset >:
// 1. “GSM”: GSM default alphabet (GSM03.38.6.2.1)
// 2. “HEX”: character strings consist only of hexadecimal numbers from 00 to
//     FF; e.g. "032FE6" equals three 8-bit characters with decimal values 3, 47
//     and 230; no conversions to the original MT character set shall be done
// 3. “IRA”: international reference alphabet.(ITU-T T.50).
// 4. “PCCP437”: PC character set Code Page 437
// 5. “8859-1”: ISO 8859 Latin 1 character set
// 6. “UCS2”: 16-bit universal multiple-octet coded character set
//     (USO/IEC10646); UCS2 character strings are converted to hexadecimal
//     numbers from 0000 to FFFF. Only the strings found in quotation marks are
//     UCS2 coded, the rest of commands or responses, remains in IRA alphabet.
#define strCmdCscsGsmCnt 13
unsigned char const strCmdCscsGsm[strCmdCscsGsmCnt] = { 'A','T','+','C','S','C','S','=','"','G','S','M','"' };
void CmdCscsGsm()  { SendStringToUart(strCmdCscsGsm,  strCmdCscsGsmCnt); SendEndSimbolToUart(); }
#define strCmdCscsUcs2Cnt 14
unsigned char const strCmdCscsUcs2[strCmdCscsUcs2Cnt] = { 'A','T','+','C','S','C','S','=','"','U','C','S','2','"' };
void CmdCscsUcs2()  { SendStringToUart(strCmdCscsUcs2,  strCmdCscsUcs2Cnt); SendEndSimbolToUart(); }

// Set message indication Format
// This command is to set how to inform the user after receiving new message from the network.
#define strCmdCnmiCnt 18
// unsigned char const strCmdCnmi[strCmdCnmiCnt] = { 'A','T','+','C','N','M','I','=','2',',','2' };
unsigned char const strCmdCnmi[strCmdCnmiCnt] = { 'A','T','+','C','N','M','I','=','2',',','2',',','0',',','0',',','0' };
void CmdCnmi()  { SendStringToUart(strCmdCnmi,  strCmdCnmiCnt); SendEndSimbolToUart(); }

// куда записываем информацию // указываем записывать на сим-карту
// CHOOSE PHONE BOOK STORAGE:+CPBS
// объявлены в .h
// #define strCmdCpbs_SimCardCnt 12
// unsigned char const strCmdCpbs_SimCard[strCmdCpbs_SimCardCnt] = { 'A','T','+','C','P','B','S','=','"','S','M','"' };
void CmdCpbs_SimCard()  
{   
  SendStringToUart(strCmdCpbs_SimCard,  strCmdCpbs_SimCardCnt); 
  SendEndSimbolToUart();
}

// информация о телефонной книге // AT+CPBS?
// Check the phone book storage and total capbility
#define strCmdCpbs_CheckCnt 8
unsigned char const strCmdCpbs_Check[strCmdCpbs_CheckCnt] = { 'A','T','+','C','P','B','S','?' };
void CmdCpbs_Check()  { SendStringToUart(strCmdCpbs_Check,  strCmdCpbs_CheckCnt); SendEndSimbolToUart(); }

// следующий телефонный номер с телефонной книги // AT+CPBR=1 // нумерация телефонных номеров на сим-карте с 1
#define cpbrBufTempCnt 3
#define strCmdAskNumberByIndexCnt 8
unsigned char const strCmdAskNumberByIndex[strCmdAskNumberByIndexCnt] = { 'A','T','+','C','P','B','R','=' };
void CmdAskNextNumber()
{
  if (!bookNumbersNeedGetCount)
    return;
  SendStringToUart(strCmdAskNumberByIndex,  strCmdAskNumberByIndexCnt); 

  // в качестве буфера используем последние ячейки буфера UART - всё равно они в крайнем случае потом заполняются строками
  LongToStr(bookNextNumberCPBR, &receiveGsmPackage[receiveGsmPackageMax - cpbrBufTempCnt], cpbrBufTempCnt); 
  SendStringToUart(&receiveGsmPackage[receiveGsmPackageMax - cpbrBufTempCnt],  GetLengthOfSignedLong(bookNextNumberCPBR)); 
  
  SendEndSimbolToUart(); 
  dDecBookNumbersCountNeedGet;
}

// программный контроль потоком передачи данных
// AT+IFC=1,1
#define strCmdIfcCnt 12
unsigned char const strCmdIfc[strCmdIfcCnt] = { 'A','T','+','I','F','C','=','1',',','1' };
void CmdIfc()  { SendStringToUart(strCmdIfc,  strCmdIfcCnt); SendEndSimbolToUart(); }

// все смс удалить
// This command is to delete messages from the current storage.
// AT+CMGD=<index> [,<delflag>]
#define strCmdSmsDeleteCnt 11
unsigned char const strCmdSmsDelete[strCmdSmsDeleteCnt] = { 'A','T','+','C','M','G','D','=','1',',','4' };
void CmdSmsDelete()  { SendStringToUart(strCmdSmsDelete,  strCmdSmsDeleteCnt); SendEndSimbolToUart(); }

// включает/выключает эхо
// This command is to enable /disable Echo.
// ATE<n>
#define strCmdAteCnt 4
unsigned char const strCmdAte_On[strCmdAteCnt] =  { 'A','T','E','1' };
void CmdAte_On()  { SendStringToUart(strCmdAte_On,   strCmdAteCnt); SendEndSimbolToUart(); }
unsigned char const strCmdAte_Off[strCmdAteCnt] = { 'A','T','E','0' };
void CmdAte_Off()  { SendStringToUart(strCmdAte_Off, strCmdAteCnt); SendEndSimbolToUart(); }

// Get Error indication message:+CMEE
// AT+CMEE?
#define strCmdCmeeGetCnt 8
unsigned char const strCmdCmeeGet[strCmdCmeeGetCnt] = { 'A','T','+','C','M','E','E','?' };
void CmdCmeeGet() { SendStringToUart(strCmdCmeeGet, strCmdCmeeGetCnt); SendEndSimbolToUart(); }
    
// Set Error indication message:+CMEE
// AT+CMEE=[<n>]
// <n>:
// Get value:
// 0: disable result code +CME ERROR:<err>, use ERROR,no need to input 0;
// 1: enable result code +CME ERROR:<err>,use numeric <err> to get value;
// 2: enable result code +CME ERROR: <err>,use lengthy <err> to get value.
#define strCmdCmeeSetCnt 9
unsigned char const strCmdCmeeSet_0[strCmdCmeeSetCnt] = { 'A','T','+','C','M','E','E','=','0' };
void CmdCmeeSet_0()  { SendStringToUart(strCmdCmeeSet_0,  strCmdCmeeSetCnt); SendEndSimbolToUart(); }
unsigned char const strCmdCmeeSet_1[strCmdCmeeSetCnt] = { 'A','T','+','C','M','E','E','=','1' };
void CmdCmeeSet_1() { SendStringToUart(strCmdCmeeSet_1, strCmdCmeeSetCnt); SendEndSimbolToUart(); }
unsigned char const strCmdCmeeSet_2[strCmdCmeeSetCnt] = { 'A','T','+','C','M','E','E','=','2' };
void CmdCmeeSet_2() { SendStringToUart(strCmdCmeeSet_2, strCmdCmeeSetCnt); SendEndSimbolToUart(); }

// информация о GSM-операторе
// Query the network operator
// AT+COPS?
#define strCmdCopsGetCnt 8
unsigned char const strCmdCopsGet[strCmdCopsGetCnt] = { 'A','T','+','C','O','P','S','?' };
void CmdCopsGet() { SendStringToUart(strCmdCopsGet,   strCmdCopsGetCnt); SendEndSimbolToUart(); }
/*
// найти GSM-сеть и зарегистрироваться в нём
// AT+COPS=0,0
#define strCops_OnCnt 11
//                                                           0   1                                   10      12
unsigned char const strCops_On[strCops_OnCnt] = {'A','T','+','C','O','P','S','=','0',',','0'};
void CmdCops_On() { SendStringToUart(strCops_On,   strCops_OnCnt); SendEndSimbolToUart(); }
*/
// перевести информацию о GSM-операторе в буквенный формат
// Query the network operator
// AT+COPS=0,0
#define strCmdCopsSet_AlphavitCnt 11
unsigned char const strCmdCopsSet_Alphavit[strCmdCopsSet_AlphavitCnt] = { 'A','T','+','C','O','P','S','=','0',',','0' };
void CmdCopsSet_Alphavit() { SendStringToUart(strCmdCopsSet_Alphavit,   strCmdCopsSet_AlphavitCnt); SendEndSimbolToUart(); }





/////////////////////////
// команды "на заметку"
// для справки

// Set the module’s baud rate:+IPR

// Clock:+CCLK

// Input PIN code:+CPIN

// PIN enable and check function:+CLCK

// PIN change the password:+CPWD