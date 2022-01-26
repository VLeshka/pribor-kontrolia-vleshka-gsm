// Работа с gsm

// Перезаггузка GSM-модуля (модема) NEOWAY M590:
// NEOWAY M590 после своего рестарта все свои настройки сбрасывает.
// 1. По умолчанию после перезагрузки эха нет, поэтому настраиваем эхо.
// 2. Эхо используем для идентификации принятых ответов, для каких команд пришёл ответ.

// Пакеты, пришедшие по uart от модема, подразделяются на:
//  - "ответ"     - пакет пришёл от модема и распознан, благодаря эху, как "ответ" (обязательно 
//    имеет OK на конце пакета).
//  - "сообщение" - пакет пришёл от модема и не распознан как "ответ" (без OK на конце пакета).

// Логика начала работы с модемом 
// (переходим от п.1 до конца; если не пришёл где-нибудь ожидаемый пакет - перезагрузка модема):
// 1. Включаем питание, ставим ожидание от модема сообщения "MODEM:STARTUP".
// 2. Ставим ожидание от модема сообщения "+PBREADY".
// 3. Ставим модему очередь команд dSetFuncsSeries_OnPbReady и выполняем их.

// Соглашения по этому коду:
// - доверенные номера - это все номера, которые записаны на сим-карту.
// - все остальные номера игнорируются.
// - количество доверенных номеров ограничивается ёмкостью сим-карты.
// - любая поступившая смс от доверенного номера рассматривается как команда.
// - отправляем смс только доверенными номерам.
// - рассылка смс подразделяется на массовую (всем номерам, которые записаны на сим-карте) 
//   и одиночную (ответ абоненту, пославшему смс).
// - книга - это список номеров в сим- карте (book из мануала NEOWAY M590).

// Массовая расылка смс всем абонентам в книге происходит в случаях:
// - отключения реле из-за выхода напряжения питания за пределы допустимого.
// - ответа на все смс-команды, кроме команды "?".

// Одиночный ответ смс абоненту, пославшему смс, происходит в случаях:
// - подачи команды "?" (ответ - мониторинг).
// - ошибочно поданной команды, когда поданная абонентом команда не распознаётся (и в ответ 
//   абоненту посылается смской справка по командым).

// Логика обработки любой поступившей смс:
// 1. Из поступившего смс запоминается его номер.
// 2. Текст смски анализируется: маленькие и заглавные буквы принимаются за 
//    одинаковые, например 'v' и 'V' - одинаковые буквы.
// 3. Обновляется индекс f_smsCommandIndex в smsCommand.
// 4. Ставится флаг bit_FindOkNumberInBook.
// 5. Считываем, по одному, номеру из книги и сравниваем его с номером из поступившего смс.
// 6. Если считали все номера из телефонной книги и не нашли идентичного номера, относимся 
// к номеру поступившего смс как к недоверенному номеру и игнорируем его.
// 7. Сбрасываем бит bit_FindOkNumberInBook.
// 8. Если смс-команда требует массовую рассылку, ставим бит bit_SendSmsToAllPhonesInBook. 
// 9. Если массовая рассылка не требуется, отправляем номеру поступившего смс требуемое смс.
// 10. Если требуется массовая рассылка, то считываем с первого по одному 
//    номеру из телефонной книги симки и посылаем требуемое смс.

#include "msp430G2553.h"

#define gsm

// использовать ли для отправки смс receiveGsmPackage
// не позволяет отправлять смс с PDU > (receiveGsmPackageMax - 13)
// только для тестирования
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

// выбранные индексы в массивах smsCommand и smsAnswer // если -1, то не выбраны
t_CommandAndAnswerIndex commandAndAnswerIndex = { -1/*, 0*/};

// sms-команды модему
t_smsCommand const smsCommand[smsCommandCount] = {
  // 0 // мониторинг
  {smsCommand_Monitoring, smsCommand_MonitoringCnt, smsCommand_MonitoringCnt},            
  // 1 // выключение и включение нагрузки
  {smsCommand_220_OffThenOn, smsCommand_220_OffThenOnCnt, smsCommand_220_OffThenOnCnt},   
  // 2 // выключение нагрузки
  {smsCommand_220_Off, smsCommand_220_OffCnt, smsCommand_220_OffCnt},             
  // 3 // включение нагрузки
  {smsCommand_220_On, smsCommand_220_OnCnt, smsCommand_220_OnCnt},               
  // 4 // выключение и включение нагрузки
  {smsCommand_220_OffThenOn_Rus, smsCommand_220_OffThenOn_RusCnt, smsCommand_220_OffThenOn_RusCaptionCnt}, 
  // 5 // выключение нагрузки
  {smsCommand_220_Off_Rus, smsCommand_220_Off_RusCnt, smsCommand_220_Off_RusCaptionCnt},     
  // 6 // включение нагрузки
  {smsCommand_220_On_Rus, smsCommand_220_On_RusCnt, smsCommand_220_On_RusCaptionCnt},       
  };

// звуковой сигнал
t_beep beep = { -1, -1, 0 };

// 0 - звуковой сигнал о хорошем результате
const unsigned char beepOkMas[beepOkMasCnt] = { 1 };
// 1 - звуковой сигнал о плохом результате
const unsigned char beepBadMas[beepBadMasCnt] = { 10 };
// 2 - долгий звуковой сигнал о очень плохом результате
// минимум 5, максимум 85; на практике - столько, насколько хватит остатка flash-памяти
const unsigned char beepLongBadMas[beepLongBadMasCnt] =
// звёздочкой отмечены столбцы, которые отмечают длительность звучания динамика
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
// Когда задано 3, то это несколько коротких гудков о очень хорошем результате 
//  о выполнении поступленного задания команды по смс.

// Задаёт 10 раз по 700 мс с паузой 300 мс, и 33 раза по 700 мс с паузой 10 секунд.
#define beepLongOkMasCnt 5 
const unsigned char beepLongOkMas[beepLongOkMasCnt] = { 1, 2, 1, 2, 1 };
t_beepMas const beepMas[beepMasCount] = { 
  { beepOkMasCnt, &beepOkMas[0] },
  { beepBadMasCnt, &beepBadMas[0] },
  { beepLongBadMasCnt, &beepLongBadMas[0] },
  { beepLongOkMasCnt, &beepLongOkMas[0] }
  };

// операторы gsm // нужен для определения, какую ussd-команду использовать для определения баланса
// 0 - билайн
// 1 - мегафон
// 2 - мтс
signed char gsmOperatorUssd_Index = -1; // если -1, то оператор gsm не определён
const t_gsmOperatorUssd gsmOperatorUssd[gsmOperatorUssdCnt] = {
  {&lineCOPS_Beeline[0], &lineUSSDbalance_Beeline[0], lineCOPS_BeelineCnt, lineUSSDbalance_BeelineCnt}, 
  {&lineCOPS_Megafon[0], &lineUSSDbalance_MegafonAndMts[0], lineCOPS_MegafonCnt, lineUSSDbalance_MegafonAndMtsCnt}, 
  {&lineCOPS_Mts[0], &lineUSSDbalance_MegafonAndMts[0], lineCOPS_MtsCnt, lineUSSDbalance_MegafonAndMtsCnt},
  {&lineCOPS_Tele2[0], &lineUSSDbalance_Tele2[0], lineCOPS_Tele2Cnt, lineUSSDbalance_Tele2Cnt}
  };

// уровень gsm-сигнала и его качество
t_gsmLevel gsmLevel = { 0 };// { 0, 0 };

// очередь выполняемых функций
t_funcsSeries funcsSeries = { NULL, 0, 0};

// указатель на посланное в модем слово и его длину
t_errorScreen errorScreen = { NULL/*, 0*/ };

/////////////////////////////////
// данные для отправки смс
t_smsDataSend sds;

// буфер приёма пакета по UART.
// задаём размер receiveGsmPackage такой, насколько хватит памяти
// для полноценого чтения номера с телефонной книги в PDU-формате и выводе имени абонента из 16 символов во
// второй строке дисплея (остальные символы в наименовании контакта номера будут игнорироваться), надо 
// минимум 113 байт
__no_init unsigned char receiveGsmPackage[receiveGsmPackageMax] @ addrNastr_Free; // receive_Package
// указатель на СЛЕДУЮЩИЙ индекс для заполнения, в receiveGsmPackage
unsigned char* pReceiveGsmPackageActive = &receiveGsmPackage[0]; 

// указатель на начало полезной информации в ответе // для ускорения и экономии флешки
// начало полезной информации - когда с начала прошли все символы перевода строки и пробела, а так же эхо-ответ
unsigned char* answerEffectiveDataBegin = NULL;

// указатель на конец полезной информации в ответе // для ускорения и экономии флешки
// конец полезной информации - когда с конца отошли назад на все символы перевода строки и пробелы
unsigned char* answerEffectiveDataEnd = NULL;

// количество срабатываний в прерывании TIMER, для выдержки при приёме АТ-пакета
unsigned int f_timerCounter_AtPauseDelay = 0; // инкрементируется и сравнивается с itterruptsIndRus1_AtPauseDelay

// баланс симки // дробное или целое число в строковой форме // берётся из ответа на ussd-запрос о балансе симки
unsigned char balans[balansCount] = {'?',' ',' ',' ',' ',' ',' '};

// счётчик ошибок после посланной команды
unsigned char modemAnswerCnt = 0;

/////////////////////////////////////////////////////////////////////////
// массив указателей на все возможные строки сообщения модема с их длинами
// для упрощения
const t_strAndStrCnt msgGsm[msgGsmCnt] = { 
{ &msgModemStartUp[0], ansModemStartUpCnt },    // 0 - успешный старт модема
{ &msgPbReady[0], msgPbReadyCnt },              // 1 - успешная регистрация в gsm-сети
{ &msgRing[0], msgRingCnt },                    // 2 - успешное поступление звонка
{ &msgSms[0], msgSmsCnt },                      // 3 - успешное поступление sms
{ &msgLevel[0], msgLevelCnt },                  // 4 - уровень gsm-сигнала
{ &msgGsmInfo[0], msgGsmInfoCnt },              // 5 - информация о gsm-операторе
{ &msgGsmUssd[0], msgGsmUssdCnt },              // 6 - ответ на ussd-запрос
{ &msgGsmBookInfo[0], msgGsmBookInfoCnt },      // 7 - информация о телефонной книге // количество номеров
{ &msgGsmPhoneNumber[0], msgGsmPhoneNumberCnt },// 8 - выбранный телефонный номер с телефонной книги
{ &msgGsmStatus[0], msgGsmStatusCnt }           // 9 - статус модема
};


////////////////////////////////////////////////////////////////////////////// 
// массив указателей на функции отправки команд модему после старта модема
const pFuncSendToUart f_FuncsSendToUart_OnStart[funcsSendToUart_OnStartCnt] = {
  CmdCmeeSet_1, // 1: enable result code +CME ERROR:<err>,use numeric <err> to get value;
  CmdAte_On,    // включает/выключает эхо
  CmdCnmi,
  CmdCmgf_On, // установка GSM-режима // на практике, CmdCmgf_On 2 раза - бывает мало, надо 3 раза
  CmdCmgf_On, 
  CmdCmgf_On, 
  CmdCscsGsm, 
  CmdIfc,
  CmdSmsDelete,
  CmdAon_On,
  CmdCopsGet,
  CmdAskUssdBalance, // для CmdAskUssdBalance нужен GSM-режим
  CmdCmgf_Off,   // установка PDU-режима
  CmdCscsUcs2,
  CmdAskLevel,
  CmdCpbs_Check,
 
#ifdef testRezhim
 CmdAt, // CmdSendTestSms,
 CmdSendTestSms_part1,
 CmdSendTestSms_part2,
 CmdAt, // CmdTestRing,
   
 // тестируем все команды подряд
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
// очеред команд для посылки тестового смс
const pFuncSendToUart f_FuncsSendToUart_Sms[funcsSendSmsToUart_SmsCnt] = {
  CmdSendTestSms_part1,
  CmdSendTestSms_part2
};
#endif



// очередь команд для тестирования модема
///////////////////////////////////////////
// каждый короткий промежуток времени
const pFuncSendToUart f_FuncsSendToUart_Test_Often[funcsSendToUart_Test_OftenCnt] = {
  CmdAskLevel,
  CmdCopsGet
};

// каждый час
const pFuncSendToUart f_FuncsSendToUart_Test_PerHour[funcsSendToUart_Test_PerHourCnt] = {
  CmdCmgf_On, // на практике, CmdCmgf_On 2 раза - бывает мало (независимо от паузы между ними), надо 3 раза
  CmdCmgf_On, 
  CmdCmgf_On, 
  CmdCscsGsm,
  CmdAskUssdBalance, // для запроса баланса, надо Gsm-режим модема
  CmdCmgf_Off, 
  CmdCscsUcs2,
  CmdSmsDelete
};

// очередь команд: поиск следующего номера в телефонной книге симки
const pFuncSendToUart f_funcsSendToUart_FindNumberOk[f_funcsSendToUart_FindNumberOkCnt] = {
  CmdAskNextNumber
};

// очередь команд: указываем записывать на сим-карту, этим самым узнаём, имеется ли сим-карта в модеме
const pFuncSendToUart f_funcsSendToUart_CmdCpbs_SimCard[f_funcsSendToUart_CmdCpbs_SimCardCnt] = {
  CmdCpbs_SimCard
};

// очередь команд: опросить модем, готов ли он к работе
const pFuncSendToUart f_funcsSendToUart_isModemReady[f_funcsSendToUart_isModemReadyCnt] = {
  CmdCpas
};

// очередь команд: посылаем первую часть смс номеру из phoneBook
const pFuncSendToUart f_funcsSendToUart_SendSmsToNumberFromPhoneBook[f_funcsSendToUart_SendSmsToNumberFromPhoneBookCnt] = {
  SendSmsToNumberFromPhoneBook
};

/*
// очередь команд: узнать количество номеров в записной книжке
#define f_funcsSendToUart_CmdBookInfoCnt 1
const pFuncSendToUart f_funcsSendToUart_CmdBookInfo[f_funcsSendToUart_CmdBookInfoCnt] = {
  CmdCpbs_Check
};

// очередь команд: включение PDU-режима // для отправки смс
#define f_funcsSendToUart_Sms_PduCnt 3
const pFuncSendToUart f_funcsSendToUart_Sms_Pdu[f_funcsSendToUart_Sms_PduCnt] = {
  CmdCmgf_Off, CmdCscsUcs2, NULL
};
// очередь команд: включение GSM-режима // для приёма смс
#define f_funcsSendToUart_Sms_GsmCnt 4
const pFuncSendToUart f_funcsSendToUart_Sms_Gsm[f_funcsSendToUart_Sms_GsmCnt] = {
  CmdCmgf_On, CmdCmgf_On, CmdCmgf_On, CmdCscsGsm // на практике, CmdCmgf_On 2 раза - бывает мало, надо 3 раза
};
*/


#ifdef useGeneralFlag
unsigned int f_generalFlag = 0;
#else
t_generalFlag f_generalFlag;
#endif


// счётчик 100мс от последней посланной команды модему
// это основной счётчик на отправку команд модему после отправки последней команды и после старта прибора
// что нужно делать при работе с ним, определяется флагом в f_generalFlag
unsigned int fromLastCommand100ms = 0; // (gsmPowerOnWait_100ms - 3);




/////////////////////////////////////
// подпрограммы
/////////////////////////////////////

// определяет beep.f_beepMasIndex
void SetBeepMasIndex(unsigned char x) { beep.f_beepMasIndex = x; beep.f_beepDelay100msIndex = -1;} // 4 байта экономии

// #define dRelePower220_Off { P1OUT |= dRelePower220_Bit; }
// void RelePower220_Off() { P1OUT |= dRelePower220_Bit; }
// #define dRelePower220_On { P1OUT &= ~dRelePower220_Bit; dSet220commandManualOff_Off; }
void RelePower220_On() { P1OUT &= ~dRelePower220_Bit; dSet220commandManualOff_Off; }

// старт перебора всех номеров в телефонной книге симки
// #define dStartFindAllPhonesInBook { bookNumbersNeedGetCount = bookNumbersCount; if (0 == bookNextNumberCPBR) bookNextNumberCPBR = 1;}
void StartFindAllPhonesInBook()
{
  bookNumbersNeedGetCount = bookNumbersCount;
  if (0 == bookNextNumberCPBR)
    bookNextNumberCPBR = 1;
}

// является ли символ символом перевода строки или пробела
bool IsSimbolLineBreak(unsigned char simb)
{
  unsigned char const* cEnd = &simbolsLineBreak[simbolsLineBreakCnt];
  for (unsigned char const* c = &simbolsLineBreak[0]; c < cEnd; c++)
    if (*c == simb)
      return(true);
  return(false);
}

// возвращает десятичное число из 2 символов в answerEffectiveDataBegin, сдвигая указатель +2
// шестнадцатеричные числа должны быть заглавными
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
        // русские буквы ё Ё не учитываем - всё равно их нет в смс-командах и текст смс на дисплее не показываем
        if (*answerEffectiveDataBegin > 0x40)
          if (*answerEffectiveDataBegin < 0x47)
            res = res * 16 + (*answerEffectiveDataBegin - 55);
    }
  }
  return(res);
}

// приводит номер к 10-значному виду 900 123 45 67
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

// сохраняет номер в phoneBook
void SaveNumber(unsigned char* num, unsigned char numCnt)
{
  num = CheckPhoneNumber(num, &numCnt);
  dSetNumberNeedSendCount(numCnt);
  unsigned char* numEnd = num + numCnt;
  unsigned char* pCopuTo = pNumberNeedSend;
  for (; num<numEnd; num++, pCopuTo++)
    *pCopuTo = *num;
}

// обновляет данные sds
void SdsDataUpdate()
{
  // знаения для посылки смс по умолчанию
  sds.smsPartIndex = 1;
  sds.smsPartsCount = 1;
  // rand() весит 300 байт, поэтому заменяем его:
  // "случайное число", для отправки "длинных" смс, берём из региcтра TAR таймера NameTimerForRus1
  sds.smsRandomNumber = (unsigned char)TA0R;
  // текущие данные для составления текста смс
  sds.rest220_seconds = restart220_seconds;
  sds.gsmLevel = gsmLevel.generalLevel;
  sds.isRele220_On = isRelePower220_On;
  sds.is220_Alarm_PowerOff = isPower220_Alarm_PowerOff;  
//   sds.val220 - это значение заполняется в ParseAnswerPhoneNumber и в прерывании ADC10_VECTOR
//   sds.is200wasUp - это значение заполняется в прерывании ADC10_VECTOR
}

// перезагрузка модема
void RestartModem()
{
  dBeepSignal_Bad;
  errorScreen.word = lines_ModemRestart; // теперь ошибка - если нет ответа от модема
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

// установить очередь выполнения команд
// указатель на массив указателей на процедуры, размер массива указателей на процедуры, пауза между командами
void SetFuncsSeries(const pFuncSendToUart* f, unsigned char cnt, unsigned char pause100ms )
{ 
  funcsSeries.nextFunc = f;
  funcsSeries.funcsCnt = cnt;
  funcsSeries.waitBetween_100ms = pause100ms;
  // dResetFromLastCommand100ms;
  dSetModemReady_Off;
}

// старт рассылки смски всем номерам из книги симки
void StartSendSmsToAllNumbers() 
{ 
  SdsDataUpdate();
  dSetSendSmsToAllPhonesInBook_On; 
  StartFindAllPhonesInBook();
  dSetFuncsSeries_FindNextNumber;
}


// старт поиска в книге номера, совпадающего с номером pNumberNeedSend
// #define dStartFindPhoneNumberOk { dSetFindOkNumberInBook_On; dStartFindAllPhonesInBook; dSetFuncsSeries_FindNextNumber; } 
void StartFindPhoneNumberOk() 
{ 
  dSetFindOkNumberInBook_On; 
  StartFindAllPhonesInBook(); 
  dSetFuncsSeries_FindNextNumber; 
}

// выдирает из answerEffectiveDataBegin текст длиной textLength в кодировке UCS2, результат помещает в pRes
// следит, чтобы answerEffectiveDataBegin не вышел за пределы answerEffectiveDataEnd
// возвращает указатель на последний символ в результате // для определения длины результата
unsigned char* Ucs2ToText(unsigned char* pRes, unsigned char textLength)
{
  // нам прислали в 16-битном формате
  textLength <<= 1; // 16 бит - 2 октета
  unsigned char* textEnd = answerEffectiveDataBegin + textLength;
  for (; answerEffectiveDataBegin < textEnd; pRes++)
  {
    if ( (answerEffectiveDataBegin + 3) > answerEffectiveDataEnd) // здесь без "OK" в конце
      break;
    unsigned char firstOctet = GetDecimalFromHexOctet();
    *pRes = (unsigned char)GetDecimalFromHexOctet();
    if (0 != firstOctet)
      *pRes += 176;
  }
  return(pRes);
}

// анализирует номер number с сохранённым в phoneBook, совпадают ли они
bool isNumberOk(unsigned char* number, unsigned char numberCnt)
{
  number = CheckPhoneNumber(number, &numberCnt);
  return(CompareStrings(number, pNumberNeedSend, numberCnt));
}

// анализирует сообщение, пришедшее в uart: приводит индекс сообщения в msgGsm; если не нашёл - возвращает -1
// так же, вычисляет answerEffectiveDataBegin и answerEffectiveDataEnd
signed char IndexReservedMsg()
{    
  // 1.Выделяем первое слово в буфере (указатель на него и его длину).
  // Учитываем символы перевода строки в начале и в конце.
  answerEffectiveDataBegin = &receiveGsmPackage[0]; // индекс первого символа, который не символ перевода строки, с начала буфера.
  answerEffectiveDataEnd = pReceiveGsmPackageActive - 1; // указатель на последний НЕ-Line-Break символ в принятом пакете
  while (IsSimbolLineBreak(*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return(-1);
  while (IsSimbolLineBreak(*answerEffectiveDataEnd))
    if (answerEffectiveDataBegin >= --answerEffectiveDataEnd) return(-1);  
  // 2.Проверяем на корректность
  if ((answerEffectiveDataEnd - answerEffectiveDataBegin) < 4) return(-1);  
  // 3.Если буфер забит, значит послали смс - в буфере с самого начала "каша" и буфер не вмещает все данные (мало ОЗУ)
  if (isSendSmsComplete)
    return(msgGsm_SmsSendComplete);
  // 4.Смотрим эхо-ответ "AT+"
  if (CompareStrings(dataATCSQ, answerEffectiveDataBegin, 3))
  {
    // для случаев, когда надо ждать "OK" от модема, надо пропускать ненужный эхо-ответ
    // если это эхо-ответ на команду указываем записывать на сим-карту, этим самым узнаём, имеется ли сим-карта в модеме
    if (CompareStrings(strCmdCpbs_SimCard, answerEffectiveDataBegin, strCmdCpbs_SimCardCnt))
      return(msgGsm_CmdCpbs_SimCard);
    // если это эхо-ответ на команду первой части отправки смс
    if (CompareStrings(cmdSendSmsStart, answerEffectiveDataBegin, cmdSendSmsStartCnt))
      return(msgGsm_Smgs);      
    // пропускаем эхо-ответ
    while (!IsSimbolLineBreak(*answerEffectiveDataBegin))
      if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return(-1);
    while (IsSimbolLineBreak(*answerEffectiveDataBegin))
      if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return(-1);      
  }
  // 5.узнаём длину слова
  if (answerEffectiveDataBegin == answerEffectiveDataEnd) return(-1);
  unsigned char* endWord = answerEffectiveDataBegin + 1; // указатель на последний символ слова
  while (!IsSimbolLineBreak(*endWord))
    if (++endWord >= answerEffectiveDataEnd) { endWord++; break; } //  endWord++ для корректировки под length
  unsigned char length = endWord - answerEffectiveDataBegin;  
  // 6.Сравниваем его со всеми словами в msgGsm.
  for (unsigned char indexMsgGsm=0; indexMsgGsm<msgGsmCnt; indexMsgGsm++)
    if (msgGsm[indexMsgGsm].strCnt == length) // для ускорения сразу сравниваем длины строк
      if (CompareStrings(&(msgGsm[indexMsgGsm].str)[0], answerEffectiveDataBegin, length))
      {
        // передвигаем answerEffectiveDataBegin за пределы слова msgGsm[indexMsgGsm].msg
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

// вставяляет в указатель pSms слово word в формате unicode; возвращая указатель на СЛЕДУЮЩИЙ ДЛЯ ЗАПОЛНЕНИЯ индекс в receiveGsmPackage для заполнения
// проверено со всеми символами стандартной клавиатуры PC
void AddToPointUCS2(const unsigned char* word, unsigned char wordCnt)
{
  const unsigned char* wordEnd = word + wordCnt - 1;
  for (; word<=wordEnd; word++)
    if ( (0x0D == *word) | (0x0A == *word) ) // символы переноса строки
    {
      AddToPointUCS2(0x00);
      AddToPointUCS2(0x0A);
    }
    else
      /*if ('№' == *word) не учитываем - всё равно не отправляем в смс
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
      /*if ('Ё' == *word)        не учитываем - всё равно не отправляем в смс
        AddToPointUCS2(0x01);
      else
        if ('ё' == *word)
          AddToPointUCS2(0x51);
        else*/
          AddToPointUCS2(*word - 0xB0);
    }
}
// вставляет в указатель или в UART значение числа в шестнадцатеричном формате 0x00, при этом число 0x00 переводится в текстовый вид и занимает два байта
// возвращая указатель на СЛЕДУЮЩИЙ ДЛЯ ЗАПОЛНЕНИЯ индекс в receiveGsmPackage для заполнения
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

#define maxSmsRusChars 70 // максимальное количесвтво русских символов в смс
// добавляет в указатель значение числа; возвращая указатель на СЛЕДУЮЩИЙ ДЛЯ ЗАПОЛНЕНИЯ индекс в receiveGsmPackage для заполнения
#define usc2bufTempCnt 5
void AddToPointUCS2_num(signed long num)
{
  // в качестве буфера используем последние ячейки буфера UART - всё равно они в крайнем случае потом заполняются строками
  LongToStr(num, &receiveGsmPackage[receiveGsmPackageMax - usc2bufTempCnt], usc2bufTempCnt); 
  AddToPointUCS2(&receiveGsmPackage[receiveGsmPackageMax - usc2bufTempCnt], GetLengthOfSignedLong(num));
}
// добавляет в указатель напряжение; возвращая указатель на СЛЕДУЮЩИЙ ДЛЯ ЗАПОЛНЕНИЯ индекс в receiveGsmPackage для заполнения
void AddToPointUCS2_220()
{
  AddToPointUCS2(strNapriazhenie, strNapriazhenieCnt);
  AddToPointUCS2_num(sds.val220);
  AddToPointUCS2(strB, strB_cnt);
}
// добавляет в указатель реле вкл/выкл; возвращая указатель на СЛЕДУЮЩИЙ ДЛЯ ЗАПОЛНЕНИЯ индекс в receiveGsmPackage для заполнения
void AddToPointUCS2_rele()
{
  if (sds.isRele220_On)
    AddToPointUCS2(strVklucheno, strVkluchenoCnt);    
  else
    AddToPointUCS2(strVuklucheno, strVukluchenoCnt);
}
// готовит PDU-часть смс, возвращает количество байт в пакете PDU за исключением поля SCA (для команды AT+CMGS=xx)
// PDU-часть заполнять только с помощью AddToPointUCS2, из-за use_buffer_for_send_sms // для облегчения отладки
unsigned char PreparePduForSms()
{
  sds.pBuffer = &receiveGsmPackage[0];

  // SCA (Service Center Address) = 0x00
  AddToPointUCS2(0x00);
  
  // PDU Type // для упрощения: если одиночная смс, то 0x01; если "длинная", то 0x41
  if (sds.smsPartsCount == 1)
    AddToPointUCS2(0x01);
  else
    AddToPointUCS2(0x41);
  // MR (Message Reference) — порядковый номер сообщения, определяется самим модулем. В PDU значение поля = 00 // я ставлю sds.smsPartIndex
  AddToPointUCS2(sds.smsPartIndex);
  // DA (Destination Address) — номер телефона получателя сообщения
  unsigned char pduNumberCnt = numberNeedSendCount; // количество реальных цифр в номере
  unsigned char* pduNumderIndex = pNumberNeedSend; // индекс // сперва - первая реальная цифра
  // перевод номера в формат +7
  // количество цифр, исходим из вида +7
 //  if (numberNeedSendMax == numberNeedSendCount) // если номер вида 900 123 45 67
  {
    AddToPointUCS2(++pduNumberCnt);
    AddToPointUCS2(0x91); //  байт «Тип номера получателя» 
    AddToPointUCS2((*pduNumderIndex++ << 4) | 0x07);
  }
//   else
  { // на коротком номере не протестировано
    // AddToPointUCS2(pduNumberCnt);
  //   AddToPointUCS2(0x91); //  байт «Тип номера получателя» 
  }
  bool isAddFF = ( (pduNumberCnt %2) > 0); // если нечётно, то в конце добавляется 0xFF
  unsigned char* pduEndNumder = pNumberNeedSend + numberNeedSendCount - 1;
  if (isAddFF)
    pduEndNumder--;
  for (; pduNumderIndex<=pduEndNumder; pduNumderIndex += 2)
    AddToPointUCS2(((*(pduNumderIndex+1)-0x30) << 4) | (*pduNumderIndex-0x30));
  if (isAddFF)
    AddToPointUCS2(0xF0 | (*(++pduEndNumder)-0x30));
  // PID (Protocol Identifier) — идентификатор протокола
  AddToPointUCS2(0x00);
  // DCS (Data Coding Scheme) — кодировка сообщения // кодировка UCS2, поэтому = 08h
  AddToPointUCS2(0x08);
  // VP (Validity Period) — время жизни сообщения // не используется, поэтому отсутствует
  // UDL (User Data Length) — длина поля UD (сообщения) в байтах // в случае с кодировкой UCS2, равно количество_символов*2
  // количество букв, отправленных в смс = ( (pBuffer-1) - pUDL)
  
  // сперва отправляем pdu, потом текст, потом "выравнивающий" текст // баланс симки
  // pdu всегда = 70, для упрощения 
  // поэтому длина смс всегда 70 русских символов; если меньше, увеличиваем до 70 пробелами
  AddToPointUCS2(maxSmsRusChars << 1);
  // для выравнивания количества символов в смс до 70 при выводе последней строки // баланса симки
  unsigned char* pTextSms_Begin = sds.pBuffer;
  
  // из-за того, что при выполнении команды RESTART, отправляется смс как при ответе на команду smsCommandIndex_20_OffThenOn,
  // при RESTART заменяем командный индекс smsCommandIndex_Monitoring на smsCommandIndex_20_OffThenOn, не портя smsCommandIndex
  signed char tempCommandIndex = smsCommandIndex;
  // если надо отправить смс из-за выхода напряжения за допустимые пределы, смотрим на напругу,
  // иначе смотрим на команду в смс-сообщении
  if ( /*(isPower220_Alarm_PowerOff) & */(isNeedSendSmsOnPower220_Alarm_PowerOff) )
    // tempCommandIndex = smsAlarm220;
  {
    if (!sds.is220_Alarm_PowerOff) // если напряжение вернулось в норму раньше интечения срока secondsPowerAlarmMax
      {
        if (isStartSecondsPowerAlarm)
        {
          // Напряжение 220В и 15с было выше порога 250В
          // Напряжение 220В и 15с было ниже порога 150В
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
          // Напряжение нормализовалось
          AddToPointUCS2(strNapriazhenieNormalizovalos, strNapriazhenieNormalizovalosCnt);
      }
      else // if (!sds.is220_Alarm_PowerOff)
      {
        AddToPointUCS2(strNapriazhenie, strNapriazhenieCnt);            
        if (sds.is200wasUp)
        {
          // Напряжение выше порога 250В!
          AddToPointUCS2(strVushePoroga, strVushePorogaCnt);
          AddToPointUCS2_num(nastr_DataMax220);
        }
        else
        {
          // Напряжение ниже порога 150В!
          AddToPointUCS2(strNizhePoroga, strNizhePorogaCnt);
          AddToPointUCS2_num(nastr_DataMin220);
        }
        AddToPointUCS2(strB, strB_cnt);
      } // if (!sds.is220_Alarm_PowerOff) .. else
        // Реле включено/выключено
        AddToPointUCS2_rele();
        // Напряжение 220В
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
        AddToPointUCS2(0x05); // длина поля
        AddToPointUCS2(0x00); // IEI
        AddToPointUCS2(0x03); // 
        AddToPointUCS2(sds.smsRandomNumber);
        AddToPointUCS2(sds.smsPartsCount); // IED2, количество частей в сообщении
        AddToPointUCS2(sds.smsPartIndex); //// IED3, Порядковый номер сообщения
        if (1 == sds.smsPartIndex)
          AddToPointUCS2(spravkaPart1, spravkaPart1Cnt);
        else
          AddToPointUCS2(spravkaPart2, spravkaPart2Cnt);
        break;
      case smsCommandIndex_Monitoring:
        // Реле включено/выключено
        AddToPointUCS2_rele();
        // Напряжение 220В
        AddToPointUCS2_220();
        // Сигнал 100%
        AddToPointUCS2(strSignal, strSignalCnt);
        AddToPointUCS2_num(sds.gsmLevel);
        AddToPointUCS2(strPersent, strPersentCnt);
        // Дов номеров 200
        AddToPointUCS2(strDovNomerov, strDovNomerovCnt);
        AddToPointUCS2_num(bookNumbersCount);
        AddToPointUCS2(strPerenosStroki, strPerenosStrokiCnt);
        break;
      case smsCommandIndex_20_OffThenOn_Rus:
        // Команда "RESTART": реле вкл. через 86400с.
        AddToPointUCS2(strCommandRestart, strCommandRestartCnt);
        AddToPointUCS2_num(sds.rest220_seconds);
        AddToPointUCS2(strC, strC_Cnt);
      case smsCommandIndex_220_Off_Rus:
      case smsCommandIndex_220_On_Rus:
        if (smsCommandIndex_20_OffThenOn != tempCommandIndex)
        {
          // Команда "ON/OFF" выполнена
          if (smsCommandIndex_220_On_Rus == tempCommandIndex)
            AddToPointUCS2(smsCommand_220_On_Vupolnena, smsCommand_220_On_VupolnenaCnt);
          else
            AddToPointUCS2(smsCommand_220_Off_Vupolnena, smsCommand_220_Off_VupolnenaCnt);
          // Реле включено/выключено
          AddToPointUCS2_rele();
        }        
        // Напряжение 220В
        AddToPointUCS2_220();
        break;
      default:
        break;  
    } // switch
  } // if ( (isPower220_Alarm_PowerOff) & (isNeedSendSmsOnPower220_Alarm_PowerOff) ) .. else
  // баланс, и выравниваем смс до 70 символов
  if (sds.smsPartsCount == sds.smsPartIndex)
  {
    // длина смс всегда 70 русских символов; если меньше, увеличиваем до 70 пробелами
    // строка баланса
    AddToPointUCS2(strBalans, strBalansCnt);
    // значение баланса и недостающие пробелы для достижения 70 символов
    unsigned char enteredChars = ((sds.pBuffer - pTextSms_Begin) >> 2); // сколько символов уже ввели в смску
    unsigned char balansChars = maxSmsRusChars - enteredChars; // сколько символов осталось ввести до 70 символов
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
// парсинг UART-пакетов



// парсинг уровня gsm-сигнала на: уровень и качество
// x может быть в диапазоне 0..31 или 99. Чем больше тем лучше, но 99 значит его отсутствие
// level = x * 100 / 31
const unsigned char masGsmLevel[31] = 
{  3,  6, 10, 13, 16, 19, 23, 26, 29, 32, 
  35, 39, 42, 45, 48, 52, 55, 58, 61, 65, 
  68, 71, 74, 77, 81, 84, 87, 90, 94, 97,
  100 };
// x может быть от 0 до 7, чем число меньше тем качество связи лучше, но 99 значит "Bit error rate can not be measured"
// quality = 100 - x * 100 / 7
// const unsigned char masGsmQuality[8] = { 100, 86, 71, 57, 43, 29, 14, 0 };
void ParseReservedLevelGsm()
{ \
  /*уровень сигнала, % */ \
  unsigned int res = (*answerEffectiveDataBegin++) - 0x30; \
  if (',' != *answerEffectiveDataBegin) \
    res = (10 * res + ((*answerEffectiveDataBegin++) - 0x30)); \
  if (99 == res) \
    res = 0; \
  else \
    res = masGsmLevel[res]; // это быстрее и меньше занимает, чем res/31
  /*коэффициент ошибок связи, %*/  \
  answerEffectiveDataBegin++;
  unsigned char quality = ((*answerEffectiveDataBegin) - 0x30);
  // если надо % качество, то masGsmQuality[(*answerEffectiveDataBegin) - 0x30] / 100;
  // если надо generalLevel, то:
  // res = res * ((*answerEffectiveDataBegin) - 0x30) / 7;
  // оптимизированный способ из справочника
  if (9 == quality)
    gsmLevel.generalLevel = 0;
  else
  {
    res = res * (7 - quality);
    unsigned int q = (res >> 1);
    unsigned int qTemp = q >> 3;
    q += qTemp;
    q = q + (q >> 6);
    // q = q + (q >> 12) + (q >> 24); // максимум = 100*7 = 700 = 10 1011 1100, тоесть сдвиг > 10 бит - лишнее
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

// парсинг входящего звонка
void ParseReservedRing()
{
  smsCommandIndex = smsCommandIndex_Bad;

  // доходим до номера
  while ('"' != (*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  answerEffectiveDataBegin++;
  unsigned char* pBeginNumber = answerEffectiveDataBegin;
  // определяем количество номеров
  while ('"' != (*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  unsigned char length = answerEffectiveDataBegin - pBeginNumber;
  // записываем номер
  SaveNumber(pBeginNumber, length);
}

// парсинг смс (из receiveGsmPackage) в PDU-формате
void ParseReservedSms()
{
  // сбрасываем всё, что связано с предыдущим смс
  /////////////////////////////////////////////////
  smsCommandIndex = -1;

  // парсим пакет от модема
  //////////////////////////// 
  // доходим до номера sms-центра и проходим его
  while (!IsSimbolLineBreak(*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) break;
  while (IsSimbolLineBreak(*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) break;
  // первое число октета sms-центра всегда равно 0
  answerEffectiveDataBegin++;
  unsigned char i_temp = (*answerEffectiveDataBegin++) - 0x30;
  i_temp <<= 1;
  answerEffectiveDataBegin += i_temp;
  // октет "тип PDU"
  answerEffectiveDataBegin += 2;
  // количество цифр в номере отправившего смс
  i_temp = GetDecimalFromHexOctet();
  // 2 - пропускаем количество цифр в номере и ещё 2 - 91 не включает
  answerEffectiveDataBegin += (2);
  // приводим в читаемый вид номер пославшего мс абонента и сохраняем его в книге
  unsigned char* iiEnd = answerEffectiveDataBegin + i_temp;
  for (unsigned char* ii=answerEffectiveDataBegin; ii<iiEnd; ii++)
  {
    unsigned char buf = *ii;
    *ii = *(ii+1);
    ii++;
    *ii = buf;
  }    
  SaveNumber(answerEffectiveDataBegin, i_temp);
  // пропускаем в номере абонента символ 'F', если он есть
  answerEffectiveDataBegin += i_temp;  
  if ('0' != *answerEffectiveDataBegin)
    answerEffectiveDataBegin ++;
  // 2 - пропускает октет "00" и ещё 1 - пропускает первую цифру из "08"
  answerEffectiveDataBegin += (2 + 1);
  // если = 08, значит нам прислали смс в 16-битном формате UCS2
  bool isUCS2 = (8 == ((*answerEffectiveDataBegin++) - 0x30));
  // 14 - 7 октетов даты и времени смс
  answerEffectiveDataBegin += 14;
  // количество символов (в 7-битном формате) или октетов (в 16-битном)
  unsigned char textLength= GetDecimalFromHexOctet();
  if (!textLength)
    return;
  // получаем текст смс в читабельном виде
  // для текста используем буфер UART
  unsigned char* pRes = &receiveGsmPackage[0];    
  if (isUCS2)
    pRes = Ucs2ToText(pRes, textLength);
  else
  {
    // нам прислали в 7-битном формате
    unsigned char bits7 = 7; // счётчик битов от 0 до 7
    *pRes = 0; // результат - байт
    while (textLength)
    {
      unsigned char oct = 8; // 8 бит
      unsigned char data = GetDecimalFromHexOctet(); // байт из входных данных      
      while (oct)
      {
        *pRes >>= 1; // готовим место для бита
        if (data & 0x01) // заполняем его
          *pRes |= 0x80;
        data >>= 1; // следующий бит
        bits7--;
        oct--;
        if (!bits7) // если 7 бит уже есть, берём результат и готовимся к следующим 7 битам
        {
          *pRes++ >>= 1; // получаем ноль в старшем бите
          bits7 = 7;
          *pRes = 0;
          textLength--;
          if (!textLength)
            break;
        }
      }
      if ( answerEffectiveDataBegin  > answerEffectiveDataEnd) // здесь без "OK" в конце
        break;
    }
  }
  
  // вытаскиваем команду из текста (удаляем пробел в начале)
  unsigned char* pTextStart = &receiveGsmPackage[0];
  while (IsSimbolLineBreak(*pTextStart))
    if (++pTextStart == pRes) return;
  unsigned char length = pRes - pTextStart;
  // переводим в тексте английские и русские маленькие буквы в большие
  TranslateSmallToLarge(pTextStart, length);
  
  // Сравниваем его со всеми словами в smsCommand
  for (unsigned char indexSmsCommand=0; indexSmsCommand<smsCommandCount; indexSmsCommand++)
    if (smsCommand[indexSmsCommand].realCommandCount <= length)
      if (CompareStrings(smsCommand[indexSmsCommand].command, pTextStart, smsCommand[indexSmsCommand].realCommandCount))
      {
        smsCommandIndex = indexSmsCommand;
        return;
      }  
}

// парсинг ответа информация об операторе gsm
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
  /*Доходим до: название оператора gsm*/
    // GSM-сеть отсутствует
  while ('"' != (*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  /*Определяем название оператора gsm*/
  unsigned char* beginGsmOperator = ++answerEffectiveDataBegin;
  while ('"' != (*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  unsigned char lengthOperatorGsm = answerEffectiveDataBegin - beginGsmOperator;
  /*Сравниваем название оператора с операторами в массиве gsmOperatorUssd*/
  for (unsigned char indexOperatorGsm=0; indexOperatorGsm<gsmOperatorUssdCnt; indexOperatorGsm++)
  {
    unsigned char length = gsmOperatorUssd[indexOperatorGsm].dataCOPS_Cnt;
    if (gsmOperatorUssd[indexOperatorGsm].dataCOPS_Cnt > lengthOperatorGsm)
      length = lengthOperatorGsm;
    /*например "MTS" и "MTS RUS" - одинаковые, поэтому сравниваем по длине самого короткого названия*/
    if (CompareStrings(&(gsmOperatorUssd[indexOperatorGsm].dataCOPS)[0], beginGsmOperator, length))
    {
      gsmOperatorUssd_Index = indexOperatorGsm;
      return;
    }
  }
  gsmOperatorUssd_Index = -1;
}

// парсинг ответа на запрос ussd
void ParseAnswerUssd()
{
  // ищем первое число-точка-число, это и есть баланс в рублях
  while ('"' != (*answerEffectiveDataBegin))
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  unsigned char* pBalans = &balans[0];
  unsigned char* pBalansEnd = &balans[balansCount-1];
  // знак минус не ищем - он может быть в UCS2-формате, а так же это может быть обычным дефисом
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

// парсинг ответа // выбранный телефонный номер с телефонной книги
void ParseAnswerPhoneNumber()
{
  // 1.AT+SMGF=0 // при парсинге которого выполняем следующие пункты 2 и 3
  // 2.AT+CMGS=xx // количество байт в пакете PDU за исключением поля SCA // Важно, чтобы эта команда завершалась символом '\r' (0Dh), а не чем-то другим
  // 3.Ввод данных пакета PDU
  
  // Доходим до первых кавычек
  while ('"' != *answerEffectiveDataBegin)
    if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
  answerEffectiveDataBegin++; // корректность
  // доходим до вторых кавычек
  unsigned char* secondQuote = answerEffectiveDataBegin + 1;
  while ('"' != *secondQuote)
    if (++secondQuote >= answerEffectiveDataEnd) return;
  unsigned char length = secondQuote - answerEffectiveDataBegin;
  
  if (isFindOkNumberInBook)
  {
    if (!isNumberOk(answerEffectiveDataBegin, length)) // если номер недоверенный, то смотрим на другой номер
    {
      if (bookNumbersNeedGetCount)
        dSetFuncsSeries_FindNextNumber; // следующий номер из книгиCmdAskNextNumber();
      else
        dSetFindOkNumberInBook_Off;
      return;
    }
    else // если номер доверенный, то выполняем поданную команду
    {
      SdsDataUpdate();
      dBeepSignal_LongOk;
      // if (!isPower220_Alarm_PowerOff)
        sds.val220 = GetVal220(); // запоминаем текущее значение напряжения на нагрузке и рассылаем его      
      // unsigned int pause100msSpecMessage = 100;
      if (smsCommandIndex_Monitoring != smsCommandIndex)
      {
        if (smsCommandIndex_Bad == smsCommandIndex)
          sds.smsPartsCount = 2;
        else // if (smsCommandIndex_Bad == smsCommandIndex)
        {
          dSetFindOkNumberInBook_Off;
          dResetBookNumbersCountNeedGet;          
          // выполнение действий с железом прибора, которые требует смс
          dSetPower220_OffThenOn_Off;          
          switch (smsCommandIndex)
          {
            case smsCommandIndex_20_OffThenOn:
            case smsCommandIndex_20_OffThenOn_Rus:
              smsCommandIndex = smsCommandIndex_20_OffThenOn_Rus;
              // выключаем, а затем включаем нагрузку
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
          // при массовой рассылке первую смс отправляем уже сейчас, потому после StartSendSmsToAllNumbers() счётчик декрементируем
          bookNumbersNeedGetCount--;
          StartSendSmsToAllNumbers();          
        } // if (smsCommandIndex_Bad == smsCommandIndex) .. else
      }
      // готовим данные для выдирания имени абонента из контакта телефонной книги
      answerEffectiveDataBegin = secondQuote + 1;
      while ('"' != *answerEffectiveDataBegin)
        if (++answerEffectiveDataBegin >= answerEffectiveDataEnd) return;
      answerEffectiveDataBegin++;
      while ('"' != *answerEffectiveDataEnd)
        answerEffectiveDataEnd--;
      // готовим сообщение на дисплей
      LCD_Clear();
      // 1 строка
      // здесь указываем максимальное количество символов, которое залезет на экран
      //  + надеемся ограничение перекодируемых символов по answerEffectiveDataEnd
      unsigned char* pLength = Ucs2ToText(pLcdBufBegin, lcdColCount-1);
      GetCharInRow0[pLength-pLcdBufBegin] = ':';
      // 2 строка
      LCD_ShowString(smsCommand[smsCommandIndex].command, smsCommand[smsCommandIndex].userCommandCount, 2, 1);
      // спецсообщение
      RezhimSpecMessage_On(pLcdBufBegin, 0, BIT1);
    } // if (!isNumberOk(answerEffectiveDataBegin, length)) ... else
  }
  else  // if (isFindOkNumberInBook)
    if (isSendSmsToAllPhonesInBook)
      SaveNumber(answerEffectiveDataBegin, length);
  // посылаем первую часть смс
  dSetFuncsSeries_SendSmsToNumberFromPhoneBook;
}

// парсинг ответа // первая часть смс 
// если массовая рассылка, посылает следующему номеру (вытаскивает из книги симки)
void ParseAnswerCmgs()
{
  if (*answerEffectiveDataEnd != '>')
    return;
#ifdef use_buffer_for_send_sms
  unsigned char udl = PreparePduForSms(); // пауза после ответа модема выдержана, можно работать с модемом
  SendStringToUart(&receiveGsmPackage[0], (udl << 1) + 2);
#else
  PreparePduForSms(); // пауза после ответа модема выдержана, можно посылать смс
#endif
  SendCharToUart((unsigned char)0x1A);
  // смс послана, снимаем флаг "послана команда AT+CMGS" // первая команда при отправке смс
  dSetSentForSms_Cmgs_Off;
  // если массовая рассылка, посылает следующему номеру (вытаскивает из книги симки)
  dSetSendSmsComplete_On; // ставим флаг "сейчас забъётся буфер" // "ОК" не ждать
  // SendStringToUart не используем, поэтому dSetWaitAnswer_Off; dSetIndexReservedMsgAfterWaitAnswer_Off; не надо
}
