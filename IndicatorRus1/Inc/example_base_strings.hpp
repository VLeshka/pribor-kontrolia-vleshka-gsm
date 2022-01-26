
// типы данных и дефайны для главного рабочего модуля по работе с индикаторной платой РУС-1

#ifndef __example_base_strings__
#define __example_base_strings__

#include "rus1_mainTitle.hpp"

#include "rus1_screenLCD_AC-162A.hpp"

// длина строк
#ifdef verify_FunctionShowInfo_NULL
#define lineBaseStringCnt 89
#else
#define lineBaseStringCnt 74
#endif
extern const unsigned char lineBaseString[lineBaseStringCnt];
// строка "Настройка модема"
extern const unsigned char lineSetupModem[lcdColCount];


// ERROR
#define str_ERROR_Cnt 5
extern const unsigned char str_ERROR[str_ERROR_Cnt];                                                               

// массив для настроек да/нет
#define varsDaNetCnt 2
extern const unsigned char* varsDaNet[varsDaNetCnt];

//////////////////////////////////////// 
// Полноэкранные ссообщения на экран
// строки длиной (lcdColCount*lcdRowCount).
// При выводе строк на экран, не умещающиеся 
// в строке дисплея буквы переносятся на 
// другую строку дисплея.
///////////////////////////////////

// "Выполнено"-"ОК"
extern const unsigned char lineVupolnenoOk[lcdColCount*lcdRowCount];

// "Отменено"-"ОК"
extern const unsigned char lineCancelOk[lcdColCount*lcdRowCount];

// Перезагрузка модема
extern const unsigned char lines_ModemRestart[lcdColCount*lcdRowCount];

// VLeshka-GSM
extern const unsigned char lineGsmVLeshka[lcdColCount*lcdRowCount];

// сообщение "Нет номеров на сим-карте!"
extern const unsigned char lines_NoSimCardNumbers[lcdColCount*lcdRowCount];

// настройка модема
extern const unsigned char lines_ModemSetupModem[lcdColCount*lcdRowCount];

// напряжение на нагрузке выше допустимой заданной нормы
extern const unsigned char lines_Power220_Alarm_PowerOff_Upper[lcdColCount*lcdRowCount];
// напряжение на нагрузке ниже допустимой заданной нормы
extern const unsigned char lines_Power220_Alarm_PowerOff_Lower[lcdColCount*lcdRowCount];
// напряжение нормализовалось
extern const unsigned char lines_Power220_Norm[lcdColCount*lcdRowCount];

// регистрация в сети
//////////////////// 

// модем не может зарегистрироваться в gsm-сети
extern const unsigned char line_ModemPbReady_Error[lcdColCount*lcdRowCount];

// включение модема
/////////////////

// модем включаем
extern const unsigned char lines_ModemPowerOn[lcdColCount*lcdRowCount];
// GSM-модем не обнаружен
extern const unsigned char lines_GsmModem_Off[lcdColCount*lcdRowCount];
// sim-карта не обнаружена
extern const unsigned char lines_sim_Card_Off[lcdColCount*lcdRowCount];



//////////////////////////////////////// 
// для составления sms-сообщений
//////////////////////////////////////// 

// новый вариант справки, на "длинную" смску из 2-х смс
#define spravkaPart1Cnt 67 // Баланс берёт из отдельной строки
extern const unsigned char spravkaPart1[spravkaPart1Cnt];
#define spravkaPart2Cnt 57 // Баланс берёт из отдельной строки
extern const unsigned char spravkaPart2[spravkaPart2Cnt];

// Команда "RESTART": реле вкл. через 86400с.
#define strCommandRestartCnt 40
extern const unsigned char strCommandRestart[strCommandRestartCnt];

// Напряжение нормализовалось
#define strNapriazhenieNormalizovalosCnt 27
extern const unsigned char strNapriazhenieNormalizovalos[strNapriazhenieNormalizovalosCnt];

// с с переносом строки
#define strC_Cnt 2
extern const unsigned char strC[strC_Cnt];

// В с переносом строки
#define strB_cnt 2
extern const unsigned char strB[strB_cnt];

// В с воскл. знаком с переносом строки
#define strB_alarmCnt 3
extern const unsigned char strB_alarm[strB_alarmCnt];

// Сигнал 
#define strSignalCnt 7
extern const unsigned char strSignal[strSignalCnt];
// % с переносом строки
#define strPersentCnt 2
extern const unsigned char strPersent[strPersentCnt];

// Дов номеров 
#define strDovNomerovCnt 12
extern const unsigned char strDovNomerov[strDovNomerovCnt];

// Реле выключено с переносом строки
#define strVukluchenoCnt 15
//                                                       0                   5       7           10                  15 
extern const unsigned char strVuklucheno[strVukluchenoCnt];
// Реле выключено с переносом строки
#define strVkluchenoCnt 14
extern const unsigned char strVklucheno[strVkluchenoCnt];

// секунд было выше порога
#define strSecundBuloVushePorogaCnt 19
extern const unsigned char strSecundBuloVushePoroga[strSecundBuloVushePorogaCnt];

// секунд было ниже порога
#define strSecundBuloNizhePorogaCnt 19
extern const unsigned char strSecundBuloNizhePoroga[strSecundBuloNizhePorogaCnt];

// Баланс:
#define strBalansCnt 6
extern const unsigned char strBalans[strBalansCnt];

// команда "ВЫКЛЮЧИТЬ" выполнена
#define smsCommand_220_Off_VupolnenaCnt 30
extern const unsigned char smsCommand_220_Off_Vupolnena[smsCommand_220_Off_VupolnenaCnt];

// команда "ВКЛЮЧИТЬ" выполнена
#define smsCommand_220_On_VupolnenaCnt 29
extern const unsigned char smsCommand_220_On_Vupolnena[smsCommand_220_On_VupolnenaCnt];



//////////////////////////////////////////// 
// для разбора текста в поступившей смске
///////////////////////////////////////////

// символы перевода строки
#define simbolsLineBreakCnt 4
extern const unsigned char simbolsLineBreak[simbolsLineBreakCnt];


/////////////////////////
// допустимые sms-команды модему
// команды в тексте смс
////////////////////// 

/////////////////////////////////
// выключение и включение нагрузки
#define smsCommand_220_OffThenOnCnt 7
//                                                                             0                   5       7           10                  15 
// extern const unsigned char smsCommand_220_OffThenOn[smsCommand_220_OffThenOnCnt] = {'R','E','S','T','A','R','T'};
#define smsCommand_220_OffThenOn (&spravkaPart1[29])
// РУС ЗАГОЛОВОК выключение нагрузки
#define smsCommand_220_OffThenOn_RusCaptionCnt 12
extern const unsigned char smsCommand_220_OffThenOn_RusCaption[smsCommand_220_OffThenOn_RusCaptionCnt];
// РУС выключение нагрузки
#define smsCommand_220_OffThenOn_RusCnt 4
#define smsCommand_220_OffThenOn_Rus smsCommand_220_OffThenOn_RusCaption

// мониторинг
#define smsCommand_MonitoringCnt 1
extern const unsigned char smsCommand_Monitoring[smsCommand_MonitoringCnt];

/////////////////////////
// UART-команды модему
/////////////////////////

// первая часть смс, левая часть команды AT+CMGS=х
#define cmdSendSmsStartCnt 8
extern const unsigned char cmdSendSmsStart[cmdSendSmsStartCnt];

////////////////////////////////////////////////// 
// USSD-команды запроса баланса для gsm-операторов
/////////////////////////////////////////////////
// билайн
#define lineUSSDbalance_BeelineCnt 20
extern const unsigned char lineUSSDbalance_Beeline[lineUSSDbalance_BeelineCnt];
// мегафон, мтс
#define lineUSSDbalance_MegafonAndMtsCnt 20
extern const unsigned char lineUSSDbalance_MegafonAndMts[lineUSSDbalance_MegafonAndMtsCnt];
// теле2
#define lineUSSDbalance_Tele2Cnt 20
extern const unsigned char lineUSSDbalance_Tele2[lineUSSDbalance_Tele2Cnt];

////////////////////////////////////////////////////// 
// возможные сообщения от модема // НЕ ОТВЕТЫ НА КОМАНДЫ
////////////////////////////////////////////////////// 

// успешный старт модема
#define ansModemStartUpCnt 13
extern const unsigned char msgModemStartUp[ansModemStartUpCnt];

// успешная регистрация в gsm-сети
#define msgPbReadyCnt 8
extern const unsigned char msgPbReady[msgPbReadyCnt];

// успешное поступление звонка
#define msgRingCnt 4
extern const unsigned char msgRing[msgRingCnt];

// успешное поступление sms
#define msgSmsCnt 5
extern const unsigned char msgSms[msgSmsCnt];



////////////////////////////////////////////// 
// возможные ответы модема на посланные команды
////////////////////////////////////////////// 

// информация о качестве GSM-сигнала //// AT+CSQ
#define dataATCSQ_Cnt 7
extern const unsigned char dataATCSQ[dataATCSQ_Cnt];  // здесь AT+ оставляем, т.к. он "один раз" тоже нужен
#define msgLevelCnt 5

// информация о gsm-операторе // AT+COPS?
#define msgGsmInfoCnt 6
extern const unsigned char msgGsmInfo[msgGsmInfoCnt];

// ответ на ussd-запрос // AT+CUSD=1,"#102#"
#define msgGsmUssdCnt 6
extern const unsigned char msgGsmUssd[msgGsmUssdCnt];

// инфомация о телефонной книге // количество номеров // AT+CPBS?
#define msgGsmBookInfoCnt 6
extern const unsigned char msgGsmBookInfo[msgGsmBookInfoCnt];

// выбранный телефонный номер с телефонной книги // AT+CPBR=1 // нумерация телефонных номеров на сим-карте с 1
#define msgGsmPhoneNumberCnt 6
extern const unsigned char msgGsmPhoneNumber[msgGsmPhoneNumberCnt];

// статус модема // AT+CPAS
#define dataATCPAS_Cnt 8
extern const unsigned char dataATCPAS[dataATCPAS_Cnt];

// перенос строки
#define strPerenosStrokiCnt 1
extern const unsigned char strPerenosStroki[strPerenosStrokiCnt];


/////////////////////////////////////////////////////////////////////
// операторы gsm, получаемые командой AT+COPS? // функция CmdCopsGet()
//////////////////////////////////////////////////////////////////// 

// билайн
#define lineCOPS_BeelineCnt 7
extern const unsigned char lineCOPS_Beeline[lineCOPS_BeelineCnt];
// мегафон
#define lineCOPS_MegafonCnt 11
extern const unsigned char lineCOPS_Megafon[lineCOPS_MegafonCnt];
// мтс
#define lineCOPS_MtsCnt 7
extern const unsigned char lineCOPS_Mts[lineCOPS_MtsCnt];
// теле2
#define lineCOPS_Tele2Cnt 5
extern const unsigned char lineCOPS_Tele2[lineCOPS_Tele2Cnt];



/////////////////////////
// указатели на строки
///////////////////////

// unsigned char const msgLevel[msgLevelCnt] = {'+','C','S','Q',':'};
#define msgLevel (&dataATCSQ[2])

#define msgGsmStatusCnt 6
// unsigned char const msgGsmStatus[msgGsmStatusCnt] = {'+','C','P','A','S',':'};
#define msgGsmStatus (&dataATCPAS[2])

//////////////////////////
// вкл/выключение нагрузки
//////////////////////////

// ENG выключение нагрузки
#define smsCommand_220_OffCnt 3
// const unsigned char smsCommand_220_Off[smsCommand_220_OffCnt] = {'O','F','F'};
#define smsCommand_220_Off (&spravkaPart2[23])

// РУС выключение нагрузки
#define smsCommand_220_Off_RusCnt 4
#define smsCommand_220_Off_RusCaptionCnt 9
#define smsCommand_220_Off_Rus (&smsCommand_220_Off_Vupolnena[9])

// ENG включение нагрузки
#define smsCommand_220_OnCnt 2
// const unsigned char smsCommand_220_On[smsCommand_220_OnCnt] = {'O','N'};
#define smsCommand_220_On (&spravkaPart1[65])

// РУС включение нагрузки
#define smsCommand_220_On_RusCnt 3
#define smsCommand_220_On_RusCaptionCnt 8
#define smsCommand_220_On_Rus (&smsCommand_220_On_Vupolnena[9])

// _и_
#define strProbelI_ProbelCnt 3
#define strProbelI_Probel (&spravkaPart1[36])

// Напряжение
#define strNapriazhenieCnt 11
#define strNapriazhenie strNapriazhenieNormalizovalos

// выше порога
#define strVushePorogaCnt 12
#define strVushePoroga (&strSecundBuloVushePoroga[7])

// перезагрузка
#define line_ModemRestart (&lines_ModemPbReady_Wait[lcdColCount])

// OK
#define str_OK_Cnt 2
#define str_OK (&lineVupolnenoOk[23])

// статус
#define msgGsmStatus (&dataATCPAS[2])
// уровень сигнала
#define msgLevelCnt 5
#define msgLevel (&dataATCSQ[2])

// ниже порога
#define strNizhePorogaCnt 12
#define strNizhePoroga (&strSecundBuloNizhePoroga[7])

// _на_
#define strProbelNaProbelCnt 4
#define strProbelNaProbel (&modemUssdWaitAnswer[15])


// строка "Нет GSM-связи"
//                                                       0                   5       7           10                  15 
#define line_ModemNoGsmConnect lineBaseString
// {' ','Н','е','т',' ','G','S','M','-','с','в','я','з','и','!',' '}
//   0                   5       7           10                  15 
// строка "Введите:"
// const unsigned char lineLCD_Enter[lcdColCount] =  {' ','В','в','е','д','и','т','е',':',' ',' ',' ',' ',' ',' ',' '};    
#define lineLCD_Enter (&lineBaseString[15])
#define line_Probel (&lineBaseString[24])
// строка "Да"
// const unsigned char strDa[lcdColCount] =          {' ',' ',' ',' ',' ',' ',' ','Д','а',' ',' ',' ',' ',' ',' ',' '};
#define strDa (&lineBaseString[33])
// строка "Нет"
// const unsigned char strNet[lcdColCount] =         {' ',' ',' ',' ',' ',' ',' ','Н','е','т',' ',' ',' ',' ',' ',' '};
#define strNet (&lineBaseString[42])
// строка "Выберите:"
// const unsigned char lineLCD_Check[lcdColCount] =  {' ','В','ы','б','е','р','и','т','е',':',' ',' ',' ',' ',' ',' '}; 
#define lineLCD_Check (&lineBaseString[58])
#ifdef verify_FunctionShowInfo_NULL
// строка для случая (NULL == currentTreeNode -> pFunctionShowInfo)
// const unsigned char tree_ShowNULLkatalogString[lcdColCount] = {' ','Ф','у','н','к','ц','и','я',' ','=',' ','N','U','L','L','!'};
#define tree_ShowNULLkatalogString (&lineBaseString[73])
#endif

// строка vleshka@mail.ru
//                                                    0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
// const unsigned char lineVLeshkaMail[lcdColCount] = {' ','v','l','e','s','h','k','a','@','m','a','i','l','.','r','u'};
// строка Автор
//                                                      0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
// const unsigned char lineVLeshkaAuthor[lcdColCount] = {' ',' ','В','а','с','и','л','ь','е','в',' ','А','.','В','.',' '};                                                               


/////////////////////////
// допустимые sms-команды модему
// команды в тексте смс
////////////////////// 

// выключение и включение нагрузки
#define smsCommand_220_OffThenOnCnt 7
//                                                                             0                   5       7           10                  15 
// const unsigned char smsCommand_220_OffThenOn[smsCommand_220_OffThenOnCnt] = {'R','E','S','T','A','R','T'};
#define smsCommand_220_OffThenOn (&spravkaPart1[29])
// РУС ЗАГОЛОВОК выключение нагрузки
#define smsCommand_220_OffThenOn_RusCaptionCnt 12

////////////////////////// 
// включение нагрузки

// ENG включение нагрузки
#define smsCommand_220_OnCnt 2
// const unsigned char smsCommand_220_On[smsCommand_220_OnCnt] = {'O','N'};
#define smsCommand_220_On (&spravkaPart1[65])

// РУС включение нагрузки
#define smsCommand_220_On_RusCnt 3
#define smsCommand_220_On_RusCaptionCnt 8
#define smsCommand_220_On_Rus (&smsCommand_220_On_Vupolnena[9])

#endif // #ifndef __example_base_strings__