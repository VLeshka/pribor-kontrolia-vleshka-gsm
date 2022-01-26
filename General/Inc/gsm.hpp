// основной рабочий модуль

#ifndef __gsm__
#define __gsm__

#include "gsm_mainTitle.hpp"
#include "rus1_mainTitle.hpp"

#include "example_base_types.hpp"




///////////////////
// типы

// указатель на функцию
typedef void (*pFuncSendToUart)();

// размеры массивов
#ifdef useGeneralFlag
  #define receiveGsmPackageMax 173
#else 
  #define receiveGsmPackageMax 160 //  теряем 13 байт - это когда отказываемся от f_generalFlag
#endif
#define beepOkMasCnt 1
#define beepBadMasCnt 1
#define beepLongBadMasCnt 5 // 85 // 3 раза по 1000 мс с паузой 300 мс, и 33 раза по 1000 мс с паузой 10 секунд
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
// типы

// указатель на функцию
typedef void (*pFuncSendToUart)();


///////////////////////////////
// структуры и массивы структур

/////////////////////////////////
// данные для отправки смс
/////////////////////////////////
//  Во время отправки смс, могут меняться отправляемые данные.
//  Это может привести к тому, что:
// - отправка смс происходит с ошибкой (вторая часть смс не подходит для первой части смс).
// - в одной рассылке разным контактам из телефонной книги отправляются разные значения данных.
// - к концу рассылки данные противоречат теме смс.
//  Чтобы этого избежать, сохраняем все изменяющиеся по числу символов данные в t_smsDataSend.
//  Так же, размещаем там:
// - pBuffer, для уменьшения размера кода // на 300 байт
// - "случайное число", для отправки "длинных" смс
// - счётчик отправленных смс, для отправки "длинных" смс
struct t_smsDataSend
{
  unsigned char* pBuffer;        // для уменьшения размера кода на фшешке
  // "длинные" смс
  unsigned char smsRandomNumber; // "случайное число", для отправки "длинных" смс
  unsigned char smsPartsCount;   // количество частей, из которых состоит отправляемая смс // для отправки "длинных" смс
  unsigned char smsPartIndex;    // счётчик отправленных частей смс // нумерация с единицы // для отправки "длинных" смс
  // данные
  unsigned int rest220_seconds;  // 10/100 // сколько секунд до включения модема, при выполнении команды рестарт
  unsigned int val220;           // 90/100 // напряжение 220
  bool is200wasUp;               // если true, то напряжение повышалось выше допустимого, иначе ниже допустимого
  bool isRele220_On;             //  реле напряжения включено/выключено
  bool is220_Alarm_PowerOff;     // "аварийно выключили нагрузку из-за выхода там напряжения за пределы допустимого"
  unsigned char gsmLevel;        // 90/100 // уровень gsm-сигнала
};


//////////////////////////////// 
// телефонная книга

#define bookNumbersCount phoneBook.f_numbersCount
#define bookNumbersNeedGetCount phoneBook.f_numbersNeedGetCount
#define bookNextNumberCPBR phoneBook.f_nextNumberCPBR

// INC // следующий №пп телефонного номера, который надо отправить командой AT+CPBR=x
#define dIncBookNumberCPBR { bookNextNumberCPBR++; \
           if (bookNextNumberCPBR > bookNumbersCount) bookNextNumberCPBR = 1; } 
#define dDecBookNumbersCountNeedGet { bookNumbersNeedGetCount--; \
           if (!bookNumbersNeedGetCount) bookNextNumberCPBR = 0; \
           else dIncBookNumberCPBR; }
#define dResetBookNumbersCountNeedGet { phoneBook.f_numbersNeedGetCount = 0; }

// старт рассылки смски всем номерам из книги симки
// #define dStartSendSmsToAllNumbers { dSetSendSmsToAllPhonesInBook_On; dStartFindAllPhonesInBook; dSetFuncsSeries_FindNumberOk; }
// void StartSendSmsToAllNumbers() { dSetSendSmsToAllPhonesInBook_On; dStartFindAllPhonesInBook; dSetFuncsSeries_FindNumberOk; }

#define pNumberNeedSend (&phoneBook.f_numberNeedSend[0])
#define numberNeedSendCount phoneBook.f_numberNeedSendCount
#define dSetNumberNeedSendCount(x) { phoneBook.f_numberNeedSendCount = x; }
#define numberNeedSendMax 10
struct t_phoneBook
{
  // количество номеров в книге // симки
  unsigned char f_numbersCount;        
  // следующий №пп телефонного номера, который надо отправить командой AT+CPBR=x 
  // может начаться не с 1, а с любого другого, при этом "по кругу" будут вытаскиваться №пп номеров
  unsigned char f_nextNumberCPBR;      
  // см. isSendSmsToAllPhonesInBook 
  // количество номеров, сколько над ещё вытащить из книги и послать на них смс;
  // если не равен нулю, значит надо читать дальше
  unsigned char f_numbersNeedGetCount; 
  // длина номера f_numberNeedSend 
  // если ноль, то не надо никуда отправлять смс
  unsigned char f_numberNeedSendCount; 
  // номер, на который надо послать ответное смс 
  // сохраняется через SaveNumber(), где пропускается 
  // через фильтр CheckPhoneNumber - поэтому макс. 10 цифр
  unsigned char f_numberNeedSend[numberNeedSendMax]; 
};

// выбранные индексы в массивах smsCommand и smsAnswer // если -1, то не выбраны
#define smsCommandIndex commandAndAnswerIndex.f_smsCommandIndex
#define smsAnswerIndex  commandAndAnswerIndex.f_smsAnswerIndex
struct t_CommandAndAnswerIndex
{
  signed char f_smsCommandIndex;
  // signed char f_smsAnswerIndex;
};
/*
//////////////////////////////// 
// ответы на команду
#define smsAnswersCount 1
struct t_smsAnswer
{
  // команда
  const unsigned char* answer;
  // длина команды
  const unsigned char answerCount;
} const smsAnswer[smsAnswersCount] = {
  {&ansAT[0], ansOkCnt}, // ОК
  };
*/

// sms-команды модему
#define smsCommandCount 7
struct t_smsCommand
{
  // команда
  const unsigned char* command;
  // длина команды по факту
  const unsigned char realCommandCount;
  // длина команды для пользователя
  const unsigned char userCommandCount;
};


/////////////////////////////////
// define
/////////////////////////////////

// сколько 100мс пауза между командами по умолчанию
#define gsmDefaultPauseBetweenCommands 1


//  пример ответа при smsCommandIndex = smsCommandIndex_Bad:
// ? - мониторинг
// ПЕРЕЗАГРУЗКА, RESTART - выкл и включить
// ВКЛЮЧИТЬ, ON - включить
// ВЫКЛЮЧИТЬ, OFF - выключить
// СПРАВКА - справка
// Баланс123.
#define smsCommandIndex_Bad          -1
//  при smsCommandIndex = smsCommandIndex_Monitoring:
/*- если выполняется команда RESTART, то отправляется смс как при ответе на команду smsCommandIndex_20_OffThenOn, 
  с указанием количества секунд до включения реле (секунды отсчитываются).
- в остальных случаях:*/
// Реле выключено
// Напряжение 220В
// Сигнал 100%
// Дов номеров 200
// Баланс100,9
#define smsCommandIndex_Monitoring   0

//  пример ответа при smsCommandIndex = smsCommandIndex_20_OffThenOn:
// Команда "ПЕРЕЗАГРУЗКА": реле вкл. через 86400с.
// Напряжение 220В
// Баланс100,9
#define smsCommandIndex_20_OffThenOn 1
#define smsCommandIndex_20_OffThenOn_Rus 4

//  пример ответа при smsCommandIndex = smsCommandIndex_220_Off:
// Команда "ВЫКЛЮЧИТЬ" выполнена
// Реле выключено
// Напряжение 220В
// Баланс100,9
#define smsCommandIndex_220_Off      2
#define smsCommandIndex_220_Off_Rus  5

//  пример ответа при smsCommandIndex = smsCommandIndex_220_On:
// Команда "ВКЛЮЧИТЬ" выполнена
// Реле включено
// Напряжение 220В
// Баланс100,9
#define smsCommandIndex_220_On       3
#define smsCommandIndex_220_On_Rus   6

// звуковой сигнал
struct t_beep
{
  // индекс массива звукового сигнала beepMas "сигнал - пауза - сигнал - ..." в 100мс
  // 0 - звуковой сигнал о хорошем результате
  // 1 - звуковой сигнал о плохом результате
  // 2 - долгий звуковой сигнал о плохом результате // напряжение за пределами допустимого // недоступен GSM-оператор
  // 3 - несколько коротких гудков о выполнении поступленного задания // команды по смс
  char f_beepMasIndex; // если -1, то звукового сигнала создавать не надо
  // индекс в "простом" массиве beepDelay структуры t_beep
  char f_beepDelay100msIndex; // если -1, то звуковой сигнал ещё не начался
  // счётчик 100мс, для отсчёта индекса в beepMas[beepMasIndex], для разного звукового сигнала
  unsigned int f_beepSignal_100msCnt;
};

struct t_beepMas
{
  const unsigned char beepDelayCount; // длина beepDelay
  const unsigned char* beepDelay;     // массив звукового сигнала "сигнал - пауза - сигнал - ..." в 100мс
};


// оператор gsm и его ussd-команда определения баланса
#define gsmOperatorUssdCnt 4
struct t_gsmOperatorUssd 
{
  const unsigned char* dataCOPS; // операторы gsm, получаемые командой AT+COPS? // функция CmdCopsGet()
  const unsigned char* dataCOPSussdBalance; // USSD-команда получения баланса оператора gsm
  const unsigned char dataCOPS_Cnt; // длина dataCOPS
  const unsigned char dataCOPSussdBalance_Cnt; // длина dataCOPSussdBalance
};

// уровень gsm-сигнала и его качество
struct t_gsmLevel
{
  // уровень сигнала, % 
  // level = x * 100 / 31
  // x может быть в диапазоне 0..31 или 99. Чем больше тем лучше, но 99 значит его отсутствие. 
  // unsigned char level;
  // коэффициент ошибок связи, %
  // quality = 100 - x * 100 / 7
  // x может быть от 0 до 7, чем число меньше тем качество связи лучше.
  // unsigned char quality; 
  
  // общий уровень сигнала
  // generalLevel = level * quality / 100
  unsigned char generalLevel;
};

// очередь выполняемых функций
//////////////////////////////////////////// 
// указатель на очередь выполняемых функций
// её длина
// количество 100мс между принятием ответа и посылкой следующей команды из очереди
struct t_funcsSeries
{
  const pFuncSendToUart* nextFunc;
  unsigned char funcsCnt;
  unsigned char waitBetween_100ms;
};

// указатель на посланное в модем слово и его длину
// для сообщения об ошибке
struct t_errorScreen
{
  const unsigned char* word;
  // unsigned char wordCnt; // длина всегда = (lcdColCount*lcdRowCount)
};

// принятое смс
// принято ли смс и можно читать её: если ( 0 != sms.smsTextCnt)
/////////////////////////////////////////////////
// указатель на номер, откуда смс
// указатель на текст сообщения
// длина этого сообщения
// длина номера, откуда смс
// указатель на число месяца, слово из двух букв
// указатель на месяц, слово из двух букв
// указатель на год, слово из двух букв
// указатель на часы, слово из двух букв
// указатель на минуты, слово из двух букв
// указатель на секунды, слово из двух букв
// указатель на миллисекунды, слово из двух букв
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



// звуковой сигнал
//////////////// 
// индекс массива звукового сигнала beepMas "сигнал - пауза - сигнал - ..." в 100мс
// f_beepMasIndex
#define beepMasIndex beep.f_beepMasIndex
#define isBeepMasSignal ( (char)-1 != beep.f_beepMasIndex ) // счётчик звукового сигнала работает? // надо делать звуковой сигнал?
// #define dSetBeepMasIndex(x) { beep.f_beepMasIndex = x; beep.f_beepDelay100msIndex = -1;}
#define dResetBeepMasIndex { beep.f_beepMasIndex = -1; } // останов звукового сигнала
#define isEndBeepMasIndex ( ( (beepMas[beepMasIndex].beepDelayCount - 1) <= beep.f_beepDelay100msIndex ) \
                           & (isBeepDelay100ms_Began) )
// f_beepDelay100msIndex
#define beepDelay100msIndex beep.f_beepDelay100msIndex
#define isBeepDelay100ms_End ( (beepMas[beepMasIndex].beepDelayCount - 1) != beep.f_beepDelay100msIndex ) // дошли до конца beepDelay ?
#define isBeepDelay100ms_Began ( (char)-1 != beep.f_beepDelay100msIndex ) // ещё не начался звук
#define dSetBeepDelay100msIndex_Next { beep.f_beepDelay100msIndex++; \
            dSetBeepSignal_100msCnt(beepMas[beep.f_beepMasIndex].beepDelay[beep.f_beepDelay100msIndex]); }
// f_BeepSignal_100msCnt
#define isBeepSignal_100msCnt_Wait ( 0 != beep.f_beepSignal_100msCnt )
#define dDecBeepSignal_100msCnt_Wait { if (beep.f_beepSignal_100msCnt) beep.f_beepSignal_100msCnt--; } // декремент счётчика
#define dSetBeepSignal_100msCnt(x) { beep.f_beepSignal_100msCnt = x; }

// включить логическую единицу на ножке подачи: звуковой сигнал о хорошем результате
#define dBeepSignal_Good { SetBeepMasIndex(0); }
// включить логическую единицу на ножке подачи: звуковой сигнал о плохом результате
#define dBeepSignal_Bad { SetBeepMasIndex(1); }
// включить логическую единицу на ножке подачи: долгий звуковой сигнал о очень плохом результате
#define dBeepSignal_LongBad { SetBeepMasIndex(2); }
#define isBeepSignal_LongBad ( 2 == beep.f_beepMasIndex )
// включить логическую единицу на ножке подачи: долгий звуковой сигнал о очень хорошем результате // о выполнении поступленного задания
#define dBeepSignal_LongOk { SetBeepMasIndex(3); }

// f_beepDelayIndex
#define dIncBeepDelayIndex { beep.f_beepDelay100msIndex++; }
#define dResetBeepDelayIndex { beep.f_beepDelay100msIndex = 0; }




#ifdef useGeneralFlag
/////////////////////////////
// f_generalFlag
////////////////////////// 

// биты 0-3 - флаги настройки модема после его включения

// модем послал "модем только что стартовал и требует настройки"
#define isModemStartUp ( 0 != (f_generalFlag & bit_ModemStartUp) )
// устанавливает бит "модем только что стартовал и требует настройки"
#define dSetModemStartUp_On { f_generalFlag |= bit_ModemStartUp; }
// сбрасывает бит "модем только что стартовал и требует настройки"
#define dSetModemStartUp_Off { f_generalFlag &= ~bit_ModemStartUp;  }

// модем послал "модем нашёл симку и успешно зарегистрировался в GSM-сети"
#define isPbReady ( 0 != (f_generalFlag & bit_PbReady) )
// устанавливает бит "модем успешно зарегистрировался в GSM-сети"
#define dSetPbReady_On { f_generalFlag |= bit_PbReady; }
// сбрасывает бит "модем успешно зарегистрировался в GSM-сети"
#define dSetPbReady_Off { f_generalFlag &= ~bit_PbReady; }

// модем полностью прошёл настройку и считано количество номеров с симки
#define isSetupOnStartUp ( 0 != (f_generalFlag & bit_SetupOnStartUp) )
// устанавливает бит "модем полностью прошёл настройку и считано количество номеров с симки"
#define dSetSetupOnStartUp_On { f_generalFlag |= bit_SetupOnStartUp; }
// сбрасывает бит "модем полностью прошёл настройку и считано количество номеров с симки"
#define dSetSetupOnStartUp_Off { f_generalFlag &= ~bit_SetupOnStartUp; }

// модем положительно ответил на команду запроса статуса "ready"
#define isModemReady ( 0 != (f_generalFlag & bit_ModemReady) )
// устанавливает бит "модем положительно ответил на команду запроса статуса "ready"
#define dSetModemReady_On { f_generalFlag |= bit_ModemReady; }
// сбрасывает бит "модем положительно ответил на команду запроса статуса "ready"
#define dSetModemReady_Off { f_generalFlag &= ~bit_ModemReady; }


////////////// 
// общие флаги

// "послана команда Cmgf_On"
#define isCmgf_On ( 0 != (f_generalFlag & bit_Cmgf_On) )
#define dSetCmgf_On_On { f_generalFlag |= bit_Cmgf_On; }
#define dSetCmgf_On_Off { f_generalFlag &= ~bit_Cmgf_On; }

////////////////////////// 
// команды модему

// "по окончании выполнения очереди gsm-команд, сбросить режим показа спецсообщения, если он введён"
#define isRezhimSpecMessage_OffOnEndFuncsSeries ( 0 != (f_generalFlag & bit_RezhimSpecMessage_OffOnEndFuncsSeries) )
#define dSetRezhimSpecMessage_OffOnEndFuncsSeries_On { f_generalFlag |= bit_RezhimSpecMessage_OffOnEndFuncsSeries; }
#define dSetRezhimSpecMessage_OffOnEndFuncsSeries_Off { RezhimSpecMessage_Off(); \
                                                        f_generalFlag &= ~bit_RezhimSpecMessage_OffOnEndFuncsSeries; \
                                                        dRedLed_Off; \
                                                        dGreenLed_Off; \
                                                        dResetGreenLedSignalCnt; }

// "ожидаем ли ответа НА ПОСЛАННУЮ КОМАНДУ МОДЕМУ" // ожидаем сообщение с ОК на конце
#define isWaitAnswer ( 0 != (f_generalFlag & bit_WaitAnswer) )
// устанавливает бит "ожидаем ли ответа НА ПОСЛАННУЮ КОМАНДУ МОДЕМУ"
#define dSetWaitAnswer_On { f_generalFlag |= bit_WaitAnswer; }
// сбрасывает бит "ожидаем ли ответа НА ПОСЛАННУЮ КОМАНДУ МОДЕМУ"
#define dSetWaitAnswer_Off { f_generalFlag &= ~bit_WaitAnswer; }

///////////////////
// напряжение

// вручную отключено реле командой OFF
#define is220commandOff ( 0 != (f_generalFlag & bit_220commandOff) )
// устанавливает бит "ожидаем ли ответа на посланную команду"
#define dSet220commandOff_On { f_generalFlag |= bit_220commandOff; }
// сбрасывает бит "ожидаем ли ответа на посланную команду"
#define dSet220commandOff_Off { f_generalFlag &= ~bit_220commandOff; }

// "пауза в процессе выключения, а затем включения нагрузки"
#define isPower220_PauseThenOn ( 0 != (f_generalFlag & bit_Power220_OffThenOn) )
#define dSetPower220_OffThenOn_On { f_generalFlag |= bit_Power220_OffThenOn; restart220_seconds = nastr_SekRestart; }
#define dSetPower220_OffThenOn_Off { f_generalFlag &= ~bit_Power220_OffThenOn; }

// "аварийно выключили нагрузку из-за выхода там напряжения за пределы допустимого"
#define isPower220_Alarm_PowerOff ( 0 != (f_generalFlag & bit_Power220_Alarm_PowerOff) )
#define dSetPower220_Alarm_PowerOff_On { f_generalFlag |= bit_Power220_Alarm_PowerOff; }
#define dSetPower220_Alarm_PowerOff_Off { f_generalFlag &= ~bit_Power220_Alarm_PowerOff; }

// требуется разослать смс об аварийном отключении реле массовой рассылкой
#define isNeedSendSmsOnPower220_Alarm_PowerOff ( 0 != (f_generalFlag & bit_NeedSendSmsOnPower220_Alarm_PowerOff) )
#define dSetNeedSendSmsOnPower220_Alarm_PowerOff_On { f_generalFlag |= bit_NeedSendSmsOnPower220_Alarm_PowerOff; }
#define dSetNeedSendSmsOnPower220_Alarm_PowerOff_Off { f_generalFlag &= ~bit_NeedSendSmsOnPower220_Alarm_PowerOff; }

//////// 
// SMS

// ищем в телефонной книге номер, совпадающий с номером пославшего смс или звонящего
#define isFindOkNumberInBook ( 0 != (f_generalFlag & bit_FindOkNumberInBook) )
// устанавливает бит "послана команда AT+CMGS"
#define dSetFindOkNumberInBook_On { f_generalFlag |= bit_FindOkNumberInBook; }
// сбрасывает бит "послана команда AT+CMGS"
#define dSetFindOkNumberInBook_Off { f_generalFlag &= ~bit_FindOkNumberInBook; }

// послана команда AT+CMGS // первая команда при отпрваке смс
#define isSentForSms_Cmgs ( 0 != (f_generalFlag & bit_SentForSms_Cmgs) )
// устанавливает бит "послана команда AT+CMGS"
#define dSetSentForSms_Cmgs_On { f_generalFlag |= bit_SentForSms_Cmgs; }
// сбрасывает бит "послана команда AT+CMGS"
#define dSetSentForSms_Cmgs_Off { f_generalFlag &= ~bit_SentForSms_Cmgs; }

// "делаем смс-рассылку всем номерам в книге"
#define isSendSmsToAllPhonesInBook ( 0 != (f_generalFlag & bit_SendSmsToAllPhonesInBook) )
// устанавливает бит "делаем смс-рассылку всем номерам в книге"
#define dSetSendSmsToAllPhonesInBook_On { f_generalFlag |= bit_SendSmsToAllPhonesInBook; } // использовать только в dStartFindAllPhonesInBook
// сбрасывает бит "делаем смс-рассылку всем номерам в книге"
#define dSetSendSmsToAllPhonesInBook_Off { f_generalFlag &= ~bit_SendSmsToAllPhonesInBook; }

// закончили посылать смс
#define isSendSmsComplete ( 0 != (f_generalFlag & bit_SendSmsComplete) )
// устанавливает бит "закончили посылать смс"
#define dSetSendSmsComplete_On { f_generalFlag |= bit_SendSmsComplete; }
// сбрасывает бит "закончили посылать смс"
#define dSetSendSmsComplete_Off { f_generalFlag &= ~bit_SendSmsComplete; }

#else // #ifdef useGeneralFlag

// флаги настройки модема после его включения

// модем послал "модем только что стартовал и требует настройки"
#define isModemStartUp f_generalFlag.f_isModemStartUp
#define dSetModemStartUp_On { f_generalFlag.f_isModemStartUp = true; }
#define dSetModemStartUp_Off { f_generalFlag.f_isModemStartUp = false; }

// модем послал "модем успешно зарегистрировался в GSM-сети"
#define isPbReady f_generalFlag.f_isPbReady
#define dSetPbReady_On { f_generalFlag.f_isPbReady = true; }
#define dSetPbReady_Off { f_generalFlag.f_isPbReady = false; }

// модем полностью успешно прошёл настройку
#define isSetupOnStartUp f_generalFlag.f_isSetupOnStartUp
#define dSetSetupOnStartUp_On { f_generalFlag.f_isSetupOnStartUp = true; }
#define dSetSetupOnStartUp_Off { f_generalFlag.f_isSetupOnStartUp = false; }

// модем положительно ответил на команду запроса статуса "ready"
#define isModemReady f_generalFlag.f_isModemReady
#define dSetModemReady_On { f_generalFlag.f_isModemReady = true; }
#define dSetModemReady_Off { f_generalFlag.f_isModemReady = false; }


////////////////////////// 
// пакеты модему

// "по окончании выполнения очереди gsm-команд, сбросить режим показа спецсообщения, если он введён"
#define isRezhimSpecMessage_OffOnEndFuncsSeries f_generalFlag.f_isRezhimSpecMessage_OffOnEndFuncsSeries
#define dSetRezhimSpecMessage_OffOnEndFuncsSeries_On { f_generalFlag.f_isRezhimSpecMessage_OffOnEndFuncsSeries = true; }
#define dSetRezhimSpecMessage_OffOnEndFuncsSeries_Off { RezhimSpecMessage_Off(); \
                                                        f_generalFlag.f_isRezhimSpecMessage_OffOnEndFuncsSeries = false; }

// "ожидаем ли ответа НА ПОСЛАННУЮ КОМАНДУ МОДЕМУ" // ожидаем сообщение с ОК на конце
#define isWaitAnswer f_generalFlag.f_isWaitAnswer
#define dSetWaitAnswer_On { f_generalFlag.f_isWaitAnswer = true; }
#define dSetWaitAnswer_Off { f_generalFlag.f_isWaitAnswer = false; }


///////////////////
// напряжение

// вручную отключить реле командой OFF
#define is220commandManualOff f_generalFlag.f_is220commandManualOff
#define dSet220commandManualOff_On { f_generalFlag.f_is220commandManualOff = true; }
#define dSet220commandManualOff_Off { f_generalFlag.f_is220commandManualOff = false; }

// "пауза в процессе выключения, а затем включения нагрузки"
#define isPower220_PauseThenOn f_generalFlag.f_isPower220_OffThenOn
#define dSetPower220_OffThenOn_On { f_generalFlag.f_isPower220_OffThenOn = true; restart220_seconds = nastr_SekRestart; }
#define dSetPower220_OffThenOn_Off { f_generalFlag.f_isPower220_OffThenOn = false; }

// "аварийно выключили нагрузку из-за выхода там напряжения за пределы допустимого"
#define isPower220_Alarm_PowerOff f_generalFlag.f_isPower220_Alarm_PowerOff
#define dSetPower220_Alarm_PowerOff_On { f_generalFlag.f_isPower220_Alarm_PowerOff = true; }
#define dSetPower220_Alarm_PowerOff_Off { f_generalFlag.f_isPower220_Alarm_PowerOff = false; }

// требуется разослать смс об аварийном отключении реле массовой рассылкой
#define isNeedSendSmsOnPower220_Alarm_PowerOff f_generalFlag.f_isNeedSendSmsOnPower220_Alarm_PowerOff
#define dSetNeedSendSmsOnPower220_Alarm_PowerOff_On { f_generalFlag.f_isNeedSendSmsOnPower220_Alarm_PowerOff = true; }
#define dSetNeedSendSmsOnPower220_Alarm_PowerOff_Off { f_generalFlag.f_isNeedSendSmsOnPower220_Alarm_PowerOff = false; }


//////// 
// SMS

// "послана команда Cmgf_On"
#define isCmgf_On f_generalFlag.f_isCmgf_On
#define dSetCmgf_On_On { f_generalFlag.f_isCmgf_On = true; }
#define dSetCmgf_On_Off { f_generalFlag.f_isCmgf_On = false; }

// ищем в телефонной книге номер, совпадающий с номером пославшего смс или звонящего
#define isFindOkNumberInBook f_generalFlag.f_isFindOkNumberInBook
#define dSetFindOkNumberInBook_On { f_generalFlag.f_isFindOkNumberInBook = true; }
#define dSetFindOkNumberInBook_Off { f_generalFlag.f_isFindOkNumberInBook = false; }

// послана команда AT+CMGS // первая команда при отпрваке смс
#define isSentForSms_Cmgs f_generalFlag.f_isSentForSms_Cmgs
#define dSetSentForSms_Cmgs_On { f_generalFlag.f_isSentForSms_Cmgs = true; }
#define dSetSentForSms_Cmgs_Off { f_generalFlag.f_isSentForSms_Cmgs = false; }

// "делаем смс-рассылку всем номерам в книге"
#define isSendSmsToAllPhonesInBook f_generalFlag.f_isSendSmsToAllPhonesInBook
#define dSetSendSmsToAllPhonesInBook_On { f_generalFlag.f_isSendSmsToAllPhonesInBook= true; } // использовать только в dStartFindAllPhonesInBook
#define dSetSendSmsToAllPhonesInBook_Off { f_generalFlag.f_isSendSmsToAllPhonesInBook = false; }

// закончили посылать смс
#define isSendSmsComplete f_generalFlag.f_isSendSmsComplete
#define dSetSendSmsComplete_On { f_generalFlag.f_isSendSmsComplete = true; }
#define dSetSendSmsComplete_Off { f_generalFlag.f_isSendSmsComplete = false; }

#endif // #else // #ifdef useGeneralFlag



#define dRelePower220_Off { P1OUT |= dRelePower220_Bit; }

// парсинг ответа // инфомация о телефонной книге // количество номеров
// void ParseAnswerBookInfo() {
#define dParseAnswerBookInfo() \
  /*Доходим до запятой*/ \
  while (',' != *answerEffectiveDataBegin) \
    if (++answerEffectiveDataBegin == answerEffectiveDataEnd) return; \
  answerEffectiveDataBegin++; \
  bookNumbersCount = (*answerEffectiveDataBegin++) - 0x30; \
  while (',' != *answerEffectiveDataBegin) \
    bookNumbersCount = (10 * bookNumbersCount + ((*answerEffectiveDataBegin++) - 0x30)); \
  if (!bookNumbersCount) /*если нет кому отправлять*/ \
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

// убрать очередь выполнения команд
#define dResetFuncsSeries { f_funcsSeries = { NULL, 0, 0 }; }
// вернуть выполненную команду обратно в очередь первой, минуя команду проверки статуса
#define dResumeLastCommandFuncsSeries { \
  funcsSeries.nextFunc--; \
  funcsSeries.funcsCnt++; \
  dResetFromLastCommand100ms; \
  dResetReceiveGsmPackageCount; \
  dSetWaitAnswer_Off; \
  dSetModemReady_On; }

// выполнить следующую команду модему
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




// ставим очереди команд
////////////////////////////////////////////////// 

// ставим очередь команд при старте модема f_FuncsSendToUart_OnStart
#define dSetFuncsSeries_OnPbReady SetFuncsSeries( f_FuncsSendToUart_OnStart, \
                                                  funcsSendToUart_OnStartCnt, \
                                                  gsmDefaultPauseBetweenCommands )

// ставим очередь команд на тестирование модема каждый короткий промежуток времени
#define dSetFuncsSeries_TestModem_PerHour SetFuncsSeries( f_FuncsSendToUart_Test_PerHour, \
                                                          funcsSendToUart_Test_PerHourCnt, \
                                                          gsmDefaultPauseBetweenCommands )

// ставим очередь команд на тестирование модема каждый час
#define dSetFuncsSeries_TestModem_Often SetFuncsSeries( f_FuncsSendToUart_Test_Often, \
                                                        funcsSendToUart_Test_OftenCnt, \
                                                        gsmDefaultPauseBetweenCommands )
// ставим очередь команд: поиск номеров в телефонной книге симки
#define dSetFuncsSeries_FindNextNumber SetFuncsSeries( f_funcsSendToUart_FindNumberOk, \
                                                       f_funcsSendToUart_FindNumberOkCnt, \
                                                       gsmDefaultPauseBetweenCommands )


// ставим очередь выполнения: указываем записывать на сим-карту, этим самым узнаём, имеется ли сим-карта в модеме
// паузу надо минимум 1c
// делаем 10 запросов с интервалом (5*gsmDefaultPauseBetweenCommands)*100мс
#define dSetFuncsSeries_CmdCpbs_SimCard SetFuncsSeries( f_funcsSendToUart_CmdCpbs_SimCard, \
                                                        f_funcsSendToUart_CmdCpbs_SimCardCnt, \
                                                        (5) )

// ставим очередь команд: посылаем первую часть смс номеру из phoneBook
#define dSetFuncsSeries_SendSmsToNumberFromPhoneBook SetFuncsSeries( f_funcsSendToUart_SendSmsToNumberFromPhoneBook, \
                                                                     f_funcsSendToUart_SendSmsToNumberFromPhoneBookCnt, \
                                                                     gsmDefaultPauseBetweenCommands)

/*
// ставим очередь команд для теста GPS f_FuncsSendToUart_GpsTest
// #define dSetFuncsSeries_GpsTest SetFuncsSeries( f_FuncsSendToUart_GpsTest, funcsSendToUart_GpsTestCnt )

// ставим очередь выполнения: узнать количество номеров в записной книжке
#define dSetFuncsSeries_CmdBookInfo SetFuncsSeries( f_funcsSendToUart_CmdBookInfo, \
                                                    f_funcsSendToUart_CmdBookInfoCnt, \
                                                    gsmDefaultPauseBetweenCommands )

// ставим очередь команд: опросить модем, готов ли он к работе
#define dSetFuncsSeries_isModemReady SetFuncsSeries( f_funcsSendToUart_isModemReady, \
                                                     f_funcsSendToUart_isModemReadyCnt, \
                                                     gsmDefaultPauseBetweenCommands )

// ставим очередь выполнения: опросить модем на готовность // перед каждым выполнением команды модем опрашивается на готовность
#define dSetFuncsSeries_ModemReady SetFuncsSeries( NULL, \
                                                        1, \
                                                        gsmDefaultPauseBetweenCommands )

// ставим очередь команд: включение PDU-режима в смс
#define dSetFuncsSeries_Sms_Pdu SetFuncsSeries( f_funcsSendToUart_Sms_Pdu, \
                                                f_funcsSendToUart_Sms_PduCnt, \
                                                gsmDefaultPauseBetweenCommands )

// ставим очередь команд: включение GSM-режима в смс
#define dSetFuncsSeries_Sms_Gsm SetFuncsSeries( f_funcsSendToUart_Sms_Gsm, \
                                                f_funcsSendToUart_Sms_GsmCnt, \
                                                gsmDefaultPauseBetweenCommands )
*/

// смс пришло и доступно для забора данных
#define isSmsReceived ( 0 != sms.smsTextCnt )
// "очищает" смс (перед тем, как позволить заполнять буфер)
#define dSms_Clear { sms.smsTextCnt = 0; }





// максимальный счётчик ошибок после посланной команды
// когда модем послал в ответ ERROR
#define modemAnswerCntMax 10


/////////////////////////////////////////////////////////////////////////
// для упрощения
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

#define msgGsm_Smgs            10  // 10 - начало отсылки смс (отсылаем в режиме PDU) // ответ на AT+CMGS=xx
#define msgGsm_SmsSendComplete 11  // 11 - завершена отсылка смс (отсылаем в режиме PDU)
#define msgGsm_CmdCpbs_SimCard 12  // 12 - указываем записывать на сим-карту, этим самым узнаём, имеется ли сим-карта в модеме




////////////////////////////////// 
// паузы 100мс при работе с модемом
////////////////////////////////// 

// сколько 100 мс отводится на ожидание ожидание ответа на команду
#define gsmWaitAnswer_100ms 255

// сколько 100 мс отводится на ожидание перед включением модема [при включении/рестарте]
#define gsmPowerOnWait_100ms 30


// через сколько 100мс после последней команды посылаем тестовую команду
#define gsmTestCommandDelay_100ms 300



// bool
///////////////////////

// модем надо включить
#define isModemNeedPowerOn (fromLastCommand100ms >= gsmPowerOnWait_100ms)

// истекло ожидание ответа на команду модему
#define isEndWaitAnswerOfCommand_100ms (fromLastCommand100ms >= gsmWaitAnswer_100ms)

// количество принятых символов в пакете AT-команды
#define receiveGsmPackageCount (pReceiveGsmPackageActive - &receiveGsmPackage[0])

// "модем ожидает включения питания"
#define isModemWaitForPowerOn (0 == (bit_ModemStartUp & f_generalFlag))

// модему надо послать тестовую команду
#define isGsmTestCommandNeed (fromLastCommand100ms >= gsmTestCommandDelay_100ms)



//////////////////////// 
// основные переменные
////////////////////// 

#ifdef useGeneralFlag

// флаги настройки модема после его включения
////////////////////////////////////////////////

// модем послал "MODEM:STARTUP" 
// "модем отрапортовал, что УСПЕШНО стартовал" [и требует настройки]
#define bit_ModemStartUp   BIT0 

// модем послал "+PBREADY"
// "модем нашёл симку и успешно зарегистрировался в GSM-сети"
#define bit_PbReady        BIT1 

// модем полностью прошёл настройку и считано количество номеров с симки
#define bit_SetupOnStartUp BIT3 

// модем положительно ответил на команду запроса статуса "ready": готов
#define bit_ModemReady     BIT4 

// неисправность: установлено, что GSM-сеть отсутствует
#define bit_NoGsmConnect             BIT5 

// команды модему
///////////////////

// "по окончании выполнения очереди gsm-команд, сбросить режим показа спецсообщения, если он введён"
#define bit_RezhimSpecMessage_OffOnEndFuncsSeries BIT5 

// "ожидаем ответа НА ПОСЛАННУЮ КОМАНДУ МОДЕМУ" 
// ожидаем сообщение с ОК на конце
#define bit_WaitAnswer               BIT6 

// напряжение
/////////////////////

// вручную отключено реле командой OFF // иначе отключена автоматически, при выходе напряжение за порог
#define bit_220commandManualOff      BIT7 

// "пауза в процессе выключения, а затем включения нагрузки" 
// принята команда RESTART
#define bit_Power220_OffThenOn       BIT8 

// "аварийно выключили нагрузку из-за выхода там напряжения за пределы допустимого"
#define bit_Power220_Alarm_PowerOff  BIT9 

// требуется разослать смс об аварийном отключении реле массовой рассылкой
#define bit_NeedSendSmsOnPower220_Alarm_PowerOff BITA 

// SMS
// Согласно "Flow chart for sending text mode SMS" документа "M590 AT COMMAND SETS
// V3.0", для установки текстового режима смс требуется отправлять "AT+CMGF=1" 2 раза подряд.
// но при этом, во второй раз модем neoway m590 "OK" в ответе не отправляет.
///////////////

// послана команда Cmgf_On
#define bit_Cmgf_On                  BITB 

// ищем в телефонной книге номер, совпадающий с номером пославшего смс или звонящего 
// для определения, что номер доверен
#define bit_FindOkNumberInBook       BITC 

// послана команда AT+CMGS 
// первая команда при отправке смс 
// для PreparePduForSms
#define bit_SentForSms_Cmgs          BITD 

// "делаем смс-рассылку всем номерам в книге"
#define bit_SendSmsToAllPhonesInBook BITE 

// закончили посылать смс 
// ответ может быть такой большой, что не влазит в буффер 
// флаг "сейчас забъётся буфер"
#define bit_SendSmsComplete          BITF 

#else // #ifdef useGeneralFlag

struct t_generalFlag
{
// флаги настройки модема после его включения
//////////////////////////////////////////////

// "модем отрапортовал, что УСПЕШНО стартовал" [и требует настройки] 
// модем послал "MODEM:STARTUP"
bool f_isModemStartUp;              

// модем послал "+PBREADY"
// "модем нашёл симку и успешно зарегистрировался в GSM-сети"
bool f_isPbReady;                   

// модем полностью успешно прошёл настройку
bool f_isSetupOnStartUp;            

// модем положительно ответил на команду запроса статуса "ready": готов
bool f_isModemReady;                

// неисправность: установлено, что GSM-сеть отсутствует
bool f_isNoGsmConnect;              

// команды модему
////////////////////

// "по окончании выполнения очереди gsm-команд, сбросить режим показа 
// спецсообщения, если он введён"
bool f_isRezhimSpecMessage_OffOnEndFuncsSeries; 

// "ожидаем ответа НА ПОСЛАННУЮ КОМАНДУ МОДЕМУ" 
// ожидаем сообщение с ОК на конце
bool f_isWaitAnswer;                

// напряжение
////////////////

// вручную отключено реле командой OFF 
// иначе отключена автоматически, при выходе напряжение за порог
bool f_is220commandManualOff;       

// "пауза в процессе выключения, а затем включения нагрузки" 
// принята команда RESTART
bool f_isPower220_OffThenOn;        

// "аварийно выключили нагрузку из-за выхода там напряжения за пределы допустимого"
bool f_isPower220_Alarm_PowerOff;   

// требуется разослать смс об аварийном отключении реле массовой рассылкой
bool f_isNeedSendSmsOnPower220_Alarm_PowerOff; 

// SMS
// Согласно "Flow chart for sending text mode SMS" документа "M590 AT COMMAND SETS
// V3.0", для установки текстового режима смс требуется отправлять "AT+CMGF=1" 2 раза подряд.
// но при этом, во второй раз модем neoway m590 "OK" в ответе не отправляет.
////////////////////////////////////////////////////////

// послана команда Cmgf_On
bool f_isCmgf_On;                   

// ищем в телефонной книге номер, совпадающий с номером пославшего смс или звонящего 
// для определения, что номер доверен
bool f_isFindOkNumberInBook;        

// послана команда AT+CMGS 
// первая команда при отправке смс 
// для PreparePduForSms
bool f_isSentForSms_Cmgs;           

// "делаем смс-рассылку всем номерам в книге"
bool f_isSendSmsToAllPhonesInBook;  

// закончили посылать смс 
// ответ может быть такой большой, что не влазит в буффер 
// флаг "сейчас забъётся буфер"
bool f_isSendSmsComplete;           
};
#endif // #ifdef useGeneralFlag ... #else


// f_timerCounter_AtPauseDelay
#define GetTimerCounter_AtPauseDelay f_timerCounter_AtPauseDelay
#define dIncTimerCounter_AtPauseDelay { f_timerCounter_AtPauseDelay++; }
#define dResetTimerCounter_AtPauseDelay { f_timerCounter_AtPauseDelay=0; }


// fromLastCommand100ms
////////////////////////// 

// инкремент счётчика 100мс после посылки последней команды модему
#define dIncFromLastCommand100ms { if (0xFFFF != fromLastCommand100ms) fromLastCommand100ms++; }
// сброс счётчика 100мс после посылки последней команды модему
#define dResetFromLastCommand100ms { fromLastCommand100ms = 0; }



// f_funcsQueue
///////////////////////////////////

// есть ли что в очереди команд модему
#define isExistsNextCommand ( funcsSeries.funcsCnt != 0 )
// пора выполнять следующую команду из очереди команд
#define dFuncsSeriesNeedStart (fromLastCommand100ms >= funcsSeries.waitBetween_100ms)
// прервать выполнение команд из очереди команд
#define dFuncsSeries_Abort { funcsSeries.funcsCnt = 0; }


///////////////////////////
// extern
///////////////////////////

// телефонная книга
extern t_phoneBook phoneBook;

// выбранные индексы в массивах smsCommand и smsAnswer // если -1, то не выбраны
extern t_CommandAndAnswerIndex commandAndAnswerIndex;

// sms-команды модему
extern t_smsCommand const smsCommand[smsCommandCount];

// звуковой сигнал
extern t_beep beep;

// 0 - звуковой сигнал о хорошем результате
extern const unsigned char beepOkMas[beepOkMasCnt];
// 1 - звуковой сигнал о плохом результате
extern const unsigned char beepBadMas[beepBadMasCnt];
// 2 - долгий звуковой сигнал о очень плохом результате
// минимум 5, максимум 85; но на практике берём столько размера массива, насколько хватит остатка flash-памяти
extern const unsigned char beepLongBadMas[beepLongBadMasCnt];
// 3 - несколько коротких гудков о очень хорошем результате 
//  о выполнении поступленного задания 
// команды по смс

// задаёт 10 раз по 700 мс с паузой 300 мс, и 33 раза по 700 мс с паузой 10 секунд
#define beepLongOkMasCnt 5 
extern const unsigned char beepLongOkMas[beepLongOkMasCnt];
extern t_beepMas const beepMas[beepMasCount];

// операторы gsm // нужен для определения, какую ussd-команду использовать для определения баланса
// 0 - билайн
// 1 - мегафон
// 2 - мтс
extern signed char gsmOperatorUssd_Index;
extern const t_gsmOperatorUssd gsmOperatorUssd[gsmOperatorUssdCnt];

// уровень gsm-сигнала и его качество
extern t_gsmLevel gsmLevel;

// очередь выполняемых функций
extern t_funcsSeries funcsSeries;

// указатель на посланное в модем слово и его длину
extern t_errorScreen errorScreen;

/////////////////////////////////
// данные для отправки смс
extern t_smsDataSend sds;

// буфер приёма пакета по UART
// задаём размер receiveGsmPackage такой, насколько хватит памяти
// для полноценого чтения номера с телефонной книги в PDU-формате и выводе имени абонента из 16 символов во
// второй строке дисплея (остальные символы в наименовании контакта номера будут игнорироваться), надо 
// минимум 113 байт
extern unsigned char receiveGsmPackage[receiveGsmPackageMax];
// указатель на СЛЕДУЮЩИЙ индекс для заполнения, в receiveGsmPackage
extern unsigned char* pReceiveGsmPackageActive;

// количество срабатываний в прерывании TIMER, для выдержки при приёме АТ-пакета
extern unsigned int f_timerCounter_AtPauseDelay;

// баланс симки // дробное или целое число в строковой форме // берётся из ответа на ussd-запрос о балансе симки
extern unsigned char balans[balansCount];

// счётчик ошибок после посланной команды
extern unsigned char modemAnswerCnt;

/////////////////////////////////////////////////////////////////////////
// массив указателей на все возможные строки сообщения модема с их длинами
// для упрощения
extern const t_strAndStrCnt msgGsm[msgGsmCnt];

////////////////////////////////////////////////////////////////////////////// 
// массив указателей на функции отправки команд модему после старта модема
extern const pFuncSendToUart f_FuncsSendToUart_OnStart[funcsSendToUart_OnStartCnt];

#ifdef testRezhim
// очередь команд для посылки тестового смс
extern const pFuncSendToUart f_FuncsSendToUart_Sms[funcsSendSmsToUart_SmsCnt];
#endif



// очередь команд для тестирования модема
///////////////////////////////////////////
// каждый короткий промежуток времени
extern const pFuncSendToUart f_FuncsSendToUart_Test_Often[funcsSendToUart_Test_OftenCnt];

// каждый час
extern const pFuncSendToUart f_FuncsSendToUart_Test_PerHour[funcsSendToUart_Test_PerHourCnt];

// очередь команд: поиск следующего номера в телефонной книге симки
extern const pFuncSendToUart f_funcsSendToUart_FindNumberOk[f_funcsSendToUart_FindNumberOkCnt];

// очередь команд: указываем записывать на сим-карту, этим самым узнаём, имеется ли сим-карта в модеме
extern const pFuncSendToUart f_funcsSendToUart_CmdCpbs_SimCard[f_funcsSendToUart_CmdCpbs_SimCardCnt];

// очередь команд: опросить модем, готов ли он к работе
extern const pFuncSendToUart f_funcsSendToUart_isModemReady[f_funcsSendToUart_isModemReadyCnt];

// очередь команд: посылаем первую часть смс номеру из phoneBook
extern const pFuncSendToUart f_funcsSendToUart_SendSmsToNumberFromPhoneBook[f_funcsSendToUart_SendSmsToNumberFromPhoneBookCnt];

#ifdef useGeneralFlag
extern unsigned int f_generalFlag;
#else
extern t_generalFlag f_generalFlag;
#endif


// счётчик 100мс от последней посланной команды модему
// это основной счётчик на отправку команд модему после отправки последней команды и после старта прибора
// что нужно делать при работе с ним, определяется флагом в f_generalFlag
extern unsigned int fromLastCommand100ms;

// указатель на начало полезной информации в ответе // для ускорения и экономии флешки
// начало полезной информации - когда с начала прошли все символы перевода строки и пробела, а так же эхо-ответ
extern unsigned char* answerEffectiveDataBegin;

// указатель на конец полезной информации в ответе // для ускорения и экономии флешки
// конец полезной информации - когда с конца отошли назад на все символы перевода строки и пробелы
extern unsigned char* answerEffectiveDataEnd;




/////////////////////////////
// прототипы

// определяет beep.f_beepMasIndex
void SetBeepMasIndex(unsigned char x);

// реле 220
// void RelePower220_Off();
void RelePower220_On();

// старт перебора всех номеров в телефонной книге симки
void StartFindAllPhonesInBook();

// является ли символ символом перевода строки или пробела
bool IsSimbolLineBreak(unsigned char simb);
  
// возвращает десятичное число из 2 символов в answerEffectiveDataBegin, сдвигая указатель +2
// шестнадцатеричные числа должны быть заглавными
unsigned char GetDecimalFromHexOctet();

// приводит номер к 10-значному виду 906 123 45 67
unsigned char* CheckPhoneNumber(unsigned char* number, unsigned char* numberCnt);

// сохраняет номер в phoneBook
void SaveNumber(unsigned char* num, unsigned char numCnt);

// перезагрузка модема
void RestartModem();

// установить очередь выполнения команд
void SetFuncsSeries(const pFuncSendToUart* f, unsigned char cnt, unsigned char pause100ms = 0);

// старт рассылки смски всем номерам из книги симки
void StartSendSmsToAllNumbers();

// старт поиска в книге номера, совпадающего с номером pNumberNeedSend
void StartFindPhoneNumberOk();

// анализирует номер из книги с сохранённым, разрешён ли он для работы с ним
bool isNumberOk(unsigned char* number, unsigned char numberCnt);

// анализирует сообщение, пришедшее в uart: приводит индекс сообщения в msgGsm
signed char IndexReservedMsg();

// вставяляет в указатель pSms слово word в формате unicode; возвращая указатель на СЛЕДУЮЩИЙ ДЛЯ ЗАПОЛНЕНИЯ индекс в receiveGsmPackage для заполнения
void AddToPointUCS2(const unsigned char* word, unsigned char wordCnt);
// вставляет в указатель или в UART значение числа в шестнадцатеричном формате 0x00, при этом число 0x00 переводится в текстовый вид и занимает два байта
// возвращая указатель на СЛЕДУЮЩИЙ ДЛЯ ЗАПОЛНЕНИЯ индекс в receiveGsmPackage для заполнения
void AddToPointUCS2(unsigned char number);

// готовит PDU-часть смс, возвращает количество байт в пакете PDU за исключением поля SCA (для команды AT+CMGS=xx)
// PDU-часть заполнять только с помощью AddToPointUCS2, из-за use_buffer_for_send_sms // для облегчения отладки
unsigned char PreparePduForSms();

/////////////////////////
// парсиннг

// парсинг уровня gsm-сигнала на: уровень и качество
void ParseReservedLevelGsm();

// парсинг входящего звонка
void ParseReservedRing();

// парсинг смс (из receiveGsmPackage) в PDU-формате
void ParseReservedSms();

// парсинг уровня gsm-сигнала на: уровень и качество
// void ParseReservedLevelGsm();

// парсинг ответа информация об операторе gsm
void ParseReservedOperatorGsm();

// парсинг ответа на первый запрос ussd
void ParseAnswerUssd();

// парсинг текста смс // поиск команды в нём // если не нашёл, возвращает -1
void ParseTextSms(unsigned char* smsText, unsigned char smsTextCnt);

// парсинг ответа // инфомация о телефонной книге // количество номеров
// void ParseAnswerBookInfo();

// парсинг ответа // выбранный телефонный номер с телефонной книги
void ParseAnswerPhoneNumber();

// парсинг ответа // первая часть смс
void ParseAnswerCmgs();

#endif // #ifndef __gsm__