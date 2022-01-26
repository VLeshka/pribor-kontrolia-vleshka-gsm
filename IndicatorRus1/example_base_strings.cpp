
// типы данных и дефайны для главного рабочего модуля по работе с индикаторной платой РУС-1


#include "rus1_screenLCD_AC-162A.hpp"
#include "example_base_strings.hpp"

////////////////////////////////////////////////////////////////////////////////////// 
// строки для выведения на экран
// строки длиной lcdColCount
// для уменьшения занимаемого места на флешке за счёт общих пробелов, свели в один массив
// экономия 30 байт
////////////////////////////////////////////////////////////////////////////////////// 
#ifdef verify_FunctionShowInfo_NULL
#define lineBaseStringCnt 89
const unsigned char lineBaseString[lineBaseStringCnt] =
// 1               5       7       9   10
{' ','Н','е','т',' ','G','S','M','-','с',
 'в','я','з','и','!',' ','В','в','е','д',
 'и','т','е',':',' ',' ',' ',' ',' ',' ',
 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
 'Д','а',' ',' ',' ',' ',' ',' ',' ','Н',
 'е','т',' ',' ',' ',' ',' ',' ',' ','В',
 'ы','б','е','р','и','т','е',':',' ',' ',
 ' ',' ',' ',' ','Ф','у','н','к','ц','и',
 'я',' ','=',' ','N','U','L','L','!'};
#else
#define lineBaseStringCnt 74
const unsigned char lineBaseString[lineBaseStringCnt] =
// 1               5       7       9   10
{' ','Н','е','т',' ','G','S','M','-','с',
 'в','я','з','и','!',' ','В','в','е','д',
 'и','т','е',':',' ',' ',' ',' ',' ',' ',
 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
 'Д','а',' ',' ',' ',' ',' ',' ',' ','Н',
 'е','т',' ',' ',' ',' ',' ',' ',' ','В',
 'ы','б','е','р','и','т','е',':',' ',' ',
 ' ',' ',' ',' '};
#endif

// строка "Настройка модема"
const unsigned char lineSetupModem[lcdColCount] = 
// 0                   5       7           10                  15 
{'Н','а','с','т','р','о','й','к','а',' ','м','о','д','е','м','а'};


// ERROR
#define str_ERROR_Cnt 5
const unsigned char str_ERROR[str_ERROR_Cnt] = {'E','R','R','O','R'};                                                               



// массив для настроек да/нет
#define varsDaNetCnt 2
const unsigned char* varsDaNet[varsDaNetCnt] = { &strNet[0], &strDa[0] };

// перезагрузка
#define line_ModemRestart (&lines_ModemPbReady_Wait[lcdColCount])





//////////////////////////////////////// 
// Полноэкранные ссообщения на экран
// строки длиной (lcdColCount*lcdRowCount).
// При выводе строк на экран, не умещающиеся 
// в строке дисплея буквы переносятся на 
// другую строку дисплея.
///////////////////////////////////

// "Выполнено"-"ОК"
const unsigned char lineVupolnenoOk[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ',' ',' ','В','Ы','П','О','Л','Н','Е','Н','О',' ',' ',' ',' ',
 ' ',' ',' ',' ',' ',' ',' ','O','K',' ',' ',' ',' ',' ',' ',' '};

// "Отменено"-"ОК"
const unsigned char lineCancelOk[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ',' ',' ',' ','О','Т','М','Е','Н','Е','Н','О',' ',' ',' ',' ',
 ' ',' ',' ',' ',' ',' ',' ','O','K',' ',' ',' ',' ',' ',' ',' '};

// Перезагрузка модема
const unsigned char lines_ModemRestart[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
// {' ','О','ш','и','б','к','а',' ','м','о','д','е','м','а','!',' ',
//  ' ',' ','п','е','р','е','з','а','г','р','у','з','к','а',' ',' '};
{'М','о','д','е','м',' ','н','е',' ','о','т','е','ч','а','е','т',
 ' ','П','е','р','е','з','а','г','р','у','з','к','а','.','.',' '};

// VLeshka-GSM
const unsigned char lineGsmVLeshka[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{'П','р','и','б','о','р',' ','к','о','н','т','р','о','л','я',' ',
 ' ',' ','V','L','e','s','h','k','a','-','G','S','M',' ',' ',' '};

// сообщение "Нет номеров на сим-карте!"
const unsigned char lines_NoSimCardNumbers[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','Н','е','т',' ','н','о','м','е','р','о','в',' ','н','а',' ',
 ' ',' ',' ','с','и','м','-','к','а','р','т','е','!',' ',' ',' '};

// настройка модема
const unsigned char lines_ModemSetupModem[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','Н','а','с','т','р','о','й','к','а',' ',' ',' ',' ',' ',' ',
 ' ','G','S','M','-','м','о','д','е','м','а','.','.',' ',' ',' '};

// напряжение на нагрузке выше допустимой заданной нормы
const unsigned char lines_Power220_Alarm_PowerOff_Upper[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{'П','р','е','в','ы','ш','е','н','о',' ','д','о','п','у','с','-',
 'т','и','м','о','е',' ','н','а','п','р','я','ж','е','н','и','е'};
// напряжение на нагрузке ниже допустимой заданной нормы
const unsigned char lines_Power220_Alarm_PowerOff_Lower[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ',' ',' ','Н','а','п','р','я','ж','е','н','и','е',' ',' ',' ',
 'н','и','ж','е',' ','д','о','п','у','с','т','и','м','о','г','о'};
// напряжение нормализовалось
const unsigned char lines_Power220_Norm[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ',' ',' ','Н','а','п','р','я','ж','е','н','и','е',' ',' ',' ',
 'н','о','р','м','а','л','и','з','о','в','а','л','о','с','ь',' '};
/*
// показывает сообщение об ожидании ответа от gsm-оператора на ussd-запрос
//                                                                      0                   5       7           10                  15 
const unsigned char modemUssdWaitAnswer[lcdColCount + lcdColCount] = 
{'О','ж','и','д','а','н','и','е',' ','о','т','в','е','т','а',' ',
 'н','а',' ','U','S','S','D','-','з','а','п','р','о','с','.','.'};

// сообщение "Ошибка флеш-памяти"
const unsigned char lines_FlashError[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{'А','п','п','а','р','а','т','н','а','я',' ','о','ш','и','б','-',
 'к','а',' ','f','l','a','s','h','-','п','а','м','я','т','и','!'};
*/


// команды в смс-сообщении
///////////////////////////////
/*
// Команда: выключение и включение нагрузки
const unsigned char lines_220_OffThenOn[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{'К','о','м','а','н','д','а',':',' ','в','ы','к','л','ю','ч','.',
 'и',' ','в','к','л','.',' ','н','а','г','р','у','з','к','и',' '};
// Команда: выключение нагрузки
//                                                        0                   5       7           10                  15 
const unsigned char lines_220_Off[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','К','о','м','а','н','д','а',':',' ','в','ы','к','л','ю','-',
 ' ','ч','е','н','и','е',' ','н','а','г','р','у','з','к','и',' '};
// Команда: включение нагрузки
const unsigned char lines_220_On[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','К','о','м','а','н','д','а',':',' ','в','к','л','ю','-',' ',
 ' ','ч','е','н','и','е',' ','н','а','г','р','у','з','к','и',' '};
// Команда: запрос информации
// за команды ? и СПРАВКА
const unsigned char lines_220_info[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','К','о','м','а','н','д','а',':',' ','з','а','п','р','о','с',
 ' ',' ',' ','и','н','ф','о','р','м','а','ц','и','и',' ',' ',' '};
*/


// регистрация в сети
//////////////////// 

// модем не может зарегистрироваться в gsm-сети
const unsigned char line_ModemPbReady_Error[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
// {'G','S','M',' ','н','е',' ','о','б','н','а','р','у','ж','е','н',
//  ' ','П','е','р','е','з','а','г','р','у','з','к','а','.','.',' '};
{' ',' ','G','S','M','-','с','е','т','ь',' ','н','е',' ',' ',' ',
 ' ',' ','о','б','н','а','р','у','ж','е','н','а','!',' ',' ',' ',};

/*
// модем регистрируем в gsm-сети
const unsigned char lines_ModemPbReady_Wait[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ',' ','Р','е','г','и','с','т','р','а','ц','и','я',' ',' ',' ',
 ' ',' ','в',' ','G','S','M','-','с','е','т','и','.','.',' ',' '};
*/



// включение модема
/////////////////

// модем включаем
const unsigned char lines_ModemPowerOn[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
{' ','В','к','л','ю','ч','е','н','и','е',' ',' ',' ',' ',' ',' ',
 ' ','G','S','M','-','м','о','д','е','м','а','.','.',' ',' ',' '};
// GSM-модем не обнаружен
const unsigned char lines_GsmModem_Off[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
// {'М','о','д','е','м',' ','н','е',' ','в','к','л','ч','-','с','я',
//  ' ','П','е','р','е','з','а','г','р','у','з','к','а','.','.',' '};
{' ',' ','G','S','M','-','м','о','д','е','м',' ','н','е',' ',' ',
 ' ',' ',' ','о','б','н','а','р','у','ж','е','н','!',' ',' ',' '};
// sim-карта не обнаружена
const unsigned char lines_sim_Card_Off[lcdColCount*lcdRowCount] = 
// 0                   5       7           10                  15 
// {' ',' ','S','i','m','-','к','а','р','т','а',' ','н','е',' ',' ',
//  ' ',' ','о','б','н','а','р','у','ж','е','н','а','!',' ',' ',' '};
{' ',' ',' ','S','I','M','-','к','а','р','т','а',' ',' ',' ',' ',
 ' ','н','е',' ','о','б','н','а','р','у','ж','е','н','а','!',' '};


//////////////////////////////////////// 
// для составления sms-сообщений
//////////////////////////////////////// 
/*
// пустая смс для эксперинмента
#define tmpPduCnt 28
const unsigned char tmpPdu[tmpPduCnt] = 
// { 0x00,0x01,0x00,0x0B,0x91,0x97,0x60,0x31,0x10,0x89,0xF0,0x00,0x08,0x00 };
{'0','0','0','1','0','0','0','B','9','1','9','7','6','0','3','1','1','0','8','9','F','0','0','0','0','8','0','0'};
*/
/*
// старый вариант справки, на 1 смску
#define spravkaCnt 59 // Баланс берёт из отдельной строки
const unsigned char spravka[spravkaCnt] = 
//     0                   5       7           10                  15 
    {'?',' ','-',' ','м','о','н','и','т','о','р','и','н','г',0x0A,
     'R','E','S','T','A','R','T',' ','-',' ','в','ы','к','л',' ','и',' ','в','к','л','.',0x0A,
     'O','N',' ','-',' ','в','к','л','.',0x0A,
     'O','F','F',' ','-',' ','в','ы','к','л','.',0x0A,
    };// 'Б','а','л','а','н','с'};
*/
// новый вариант справки, на "длинную" смску из 2-х смс
#define spravkaPart1Cnt 67 // Баланс берёт из отдельной строки
const unsigned char spravkaPart1[spravkaPart1Cnt] = 
//     0                   5       7           10                  15 
    {'?',' ','-',' ','м','о','н','и','т','о','р','и','н','г',0x0A,
     'П','Е','Р','Е','З','А','Г','Р','У','З','К','А',',',' ','R',
     'E','S','T','A','R','T',' ','-',' ','в','ы','к','л',' ','и',
     ' ','в','к','л','ю','ч','и','т','ь',0x0A,
     'В','К','Л','Ю','Ч','И','Т','Ь',',',' ','O','N'};
#define spravkaPart2Cnt 57 // Баланс берёт из отдельной строки
const unsigned char spravkaPart2[spravkaPart2Cnt] = 
//     0                   5       7           10                  15 
    {' ','-',' ','в','к','л','ю','ч','и','т','ь',0x0A,
     'В','Ы','К','Л','Ю','Ч','И','Т','Ь',',',' ','O','F','F',' ',
     '-',' ','в','ы','к','л','ю','ч','и','т','ь',0x0A,
     'С','П','Р','А','В','К','А',' ','-',' ','с','п','р','а','в','к','а',0x0A};

/*
#define spravkaCnt 33
const unsigned char spravka[spravkaCnt] = 
// 26{ 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
// 26{ 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
// 33{ 'А','Б','В','Г','Д','Е','Ё','Ж','З','И','Й','К','Л','М','Н','О','П','Р','С','Т','У','Ф','Х','Ц','Ч','Ш','Щ','Ъ','Ы','Ь','Э','Ю','Я' };
// 33{ 'а','б','в','г','д','е','ё','ж','з','и','й','к','л','м','н','о','п','р','с','т','у','ф','х','ц','ч','ш','щ','ъ','ы','ь','э','ю','я' };

// { '1','.',',','/','?','<','>','`','~','-','_','=','+','\\','|',0x0A,
  // '!','@','#','$','%','^','&','*','(',')',0x0A,
  // '!','"','№',';','%',':','?','*','(',')' };
*/

#define strProbelI_ProbelCnt 3
#define strProbelI_Probel (&spravkaPart1[36])

// Команда "RESTART": реле вкл. через 86400с.
#define strCommandRestartCnt 40
const unsigned char strCommandRestart[strCommandRestartCnt] = 
// 0   1                                   10                                          21
{'К','о','м','а','н','д','а',' ','"','П',
 'Е','Р','Е','З','А','Г','Р','У','З','К',
 'А','"',':',' ','р','е','л','е',' ','в',
 'к','л','.',' ','ч','е','р','е','з',' '};

// Напряжение нормализовалось
#define strNapriazhenieNormalizovalosCnt 27
const unsigned char strNapriazhenieNormalizovalos[strNapriazhenieNormalizovalosCnt] = 
// 0                   5       7           10                  15 
{'Н','а','п','р','я','ж','е','н','и','е',
 ' ','н','о','р','м','а','л','и','з','о',
 'в','а','л','о','с','ь',0x0A};

// Напряжение
#define strNapriazhenieCnt 11
#define strNapriazhenie strNapriazhenieNormalizovalos

// с с переносом строки
#define strC_Cnt 2
const unsigned char strC[strC_Cnt] = {'с',0x0A};

// В с переносом строки
#define strB_cnt 2
const unsigned char strB[strB_cnt] = {'В',0x0A};

// В с воскл. знаком с переносом строки
#define strB_alarmCnt 3
const unsigned char strB_alarm[strB_alarmCnt] = {'В','!',0x0A};

// Сигнал 
#define strSignalCnt 7
const unsigned char strSignal[strSignalCnt] = {'С','и','г','н','а','л',' '};
// % с переносом строки
#define strPersentCnt 2
const unsigned char strPersent[strPersentCnt] = {'%',0x0A};

// Дов номеров 
#define strDovNomerovCnt 12
const unsigned char strDovNomerov[strDovNomerovCnt] = {'Д','о','в',' ','н','о','м','е','р','о','в',' '};
// перенос строки
#define strPerenosStrokiCnt 1
const unsigned char strPerenosStroki[strPerenosStrokiCnt] = {0x0A};

// Реле выключено с переносом строки
#define strVukluchenoCnt 15
//                                                       0                   5       7           10                  15 
const unsigned char strVuklucheno[strVukluchenoCnt] = {'Р','е','л','е',' ','в','ы','к','л','ю','ч','е','н','о',0x0A};
// Реле выключено с переносом строки
#define strVkluchenoCnt 14
const unsigned char strVklucheno[strVkluchenoCnt] =   {'Р','е','л','е',' ','в','к','л','ю','ч','е','н','о',0x0A};

// секунд было выше порога
#define strSecundBuloVushePorogaCnt 19
const unsigned char strSecundBuloVushePoroga[strSecundBuloVushePorogaCnt] = 
// 0                   5       7           10                  15                  20
{'с',' ','б','ы','л','о',' ','в','ы','ш','е',' ','п','о','р','о','г','а',' '};
#define strVushePorogaCnt 12
#define strVushePoroga (&strSecundBuloVushePoroga[7])

// секунд было ниже порога
#define strSecundBuloNizhePorogaCnt 19
const unsigned char strSecundBuloNizhePoroga[strSecundBuloNizhePorogaCnt] = 
// 0                   5       7           10                  15                  20
{'с',' ','б','ы','л','о',' ','н','и','ж','е',' ','п','о','р','о','г','а',' '};
#define strNizhePorogaCnt 12
#define strNizhePoroga (&strSecundBuloNizhePoroga[7])

// Баланс:
#define strBalansCnt 6
const unsigned char strBalans[strBalansCnt] = {'Б','а','л','а','н','с'};

#define strProbelNaProbelCnt 4
#define strProbelNaProbel (&modemUssdWaitAnswer[15])

// команда "ВЫКЛЮЧИТЬ" выполнена
#define smsCommand_220_Off_VupolnenaCnt 30
const unsigned char smsCommand_220_Off_Vupolnena[smsCommand_220_Off_VupolnenaCnt] =
// 0                   5       7           10                  15 
{'К','о','м','а','н','д','а',' ','"','В',
 'Ы','К','Л','Ю','Ч','И','Т','Ь','"',' ',
 'в','ы','п','о','л','н','е','н','а',0x0A};

// команда "ВКЛЮЧИТЬ" выполнена
#define smsCommand_220_On_VupolnenaCnt 29
const unsigned char smsCommand_220_On_Vupolnena[smsCommand_220_On_VupolnenaCnt] =
// 0                   5       7           10                  15 
{'К','о','м','а','н','д','а',' ','"','В',
 'К','Л','Ю','Ч','И','Т','Ь','"',' ','в',
 'ы','п','о','л','н','е','н','а',0x0A};




//////////////////////////////////////////// 
// для разбора текста в поступившей смске
///////////////////////////////////////////

// символы перевода строки
#define simbolsLineBreakCnt 4
const unsigned char simbolsLineBreak[simbolsLineBreakCnt] = {0, 13, 10, ' '};





/////////////////////////
// допустимые sms-команды модему
// команды в тексте смс
////////////////////// 

/////////////////////////////////
// выключение и включение нагрузки
#define smsCommand_220_OffThenOnCnt 7
//                                                                             0                   5       7           10                  15 
// const unsigned char smsCommand_220_OffThenOn[smsCommand_220_OffThenOnCnt] = {'R','E','S','T','A','R','T'};
#define smsCommand_220_OffThenOn (&spravkaPart1[29])
// РУС ЗАГОЛОВОК выключение нагрузки
#define smsCommand_220_OffThenOn_RusCaptionCnt 12
//                                                                                                   0                   5       7           10                  15 
const unsigned char smsCommand_220_OffThenOn_RusCaption[smsCommand_220_OffThenOn_RusCaptionCnt] = {'П','Е','Р','Е','З','А','Г','Р','У','З','К','А'};
// РУС выключение нагрузки
#define smsCommand_220_OffThenOn_RusCnt 4
#define smsCommand_220_OffThenOn_Rus smsCommand_220_OffThenOn_RusCaption

/////////////////////////
// выключение нагрузки

// ENG выключение нагрузки
#define smsCommand_220_OffCnt 3
// const unsigned char smsCommand_220_Off[smsCommand_220_OffCnt] = {'O','F','F'};
#define smsCommand_220_Off (&spravkaPart2[23])

// РУС выключение нагрузки
#define smsCommand_220_Off_RusCnt 4
#define smsCommand_220_Off_RusCaptionCnt 9
#define smsCommand_220_Off_Rus (&smsCommand_220_Off_Vupolnena[9])

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

////////////////////// 
// мониторинг

#define smsCommand_MonitoringCnt 1
//                                                                       0                   5       7           10                  15 
const unsigned char smsCommand_Monitoring[smsCommand_MonitoringCnt] = {'?'};


/*не хватило места на флешке
//////////////// 
// добавить номер
#define smsCommand_AddPhoneCnt 3
//                                                         0                   5       7           10                  15 
const unsigned char smsCommand_AddPhone[smsCommand_AddPhoneCnt] = {'A','D','D'};

///////////////
// удалить номер
#define smsCommand_DeletePhoneCnt 6
//                                                            0                   5       7           10                  15 
const unsigned char smsCommand_DeletePhone[smsCommand_DeletePhoneCnt] = {'D','E','L','E','T','E'};
*/




/////////////////////////////////////////////////////////////////////
// операторы gsm, получаемые командой AT+COPS? // функция CmdCopsGet()
//////////////////////////////////////////////////////////////////// 

// билайн
#define lineCOPS_BeelineCnt 7
//                                                              0                   5       7           10                  15 
const unsigned char lineCOPS_Beeline[lineCOPS_BeelineCnt] = { 'B','e','e','l','i','n','e'};
// мегафон
#define lineCOPS_MegafonCnt 11
//                                                              0                   5       7           10                  15 
const unsigned char lineCOPS_Megafon[lineCOPS_MegafonCnt] = { 'M','e','g','a','F','o','n',' ','R','U','S'};
// мтс
#define lineCOPS_MtsCnt 7
//                                                              0                   5       7           10                  15 
const unsigned char lineCOPS_Mts[lineCOPS_MtsCnt] =         { 'M','T','S','-','R','U','S'};
// теле2
#define lineCOPS_Tele2Cnt 5
//                                                              0                   5       7           10                  15 
const unsigned char lineCOPS_Tele2[lineCOPS_Tele2Cnt] =     { 'M','O','T','I','V'};



/////////////////////////
// UART-команды модему
/////////////////////////

// первая часть смс, левая часть команды AT+CMGS=х
#define cmdSendSmsStartCnt 8
unsigned char const cmdSendSmsStart[cmdSendSmsStartCnt] = 
// 0   1                                   10                                          21
{'A','T','+','C','M','G','S','='};

////////////////////////////////////////////////// 
// USSD-команды запроса баланса для gsm-операторов
/////////////////////////////////////////////////
// билайн
#define lineUSSDbalance_BeelineCnt 20
//                                                                            0                   5       7           10                  15              19
const unsigned char lineUSSDbalance_Beeline[lineUSSDbalance_BeelineCnt] = { 'A','T','+','C','U','S','D','=','1',',','"','#','1','0','2','#','"',',','1','5'};
// мегафон, мтс
#define lineUSSDbalance_MegafonAndMtsCnt 20
//                                                                                        0                   5       7           10                  15              19
const unsigned char lineUSSDbalance_MegafonAndMts[lineUSSDbalance_MegafonAndMtsCnt] = { 'A','T','+','C','U','S','D','=','1',',','"','*','1','0','0','#','"',',','1','5'};
// теле2
#define lineUSSDbalance_Tele2Cnt 20
//                                                                            0                   5       7           10                  15              19
const unsigned char lineUSSDbalance_Tele2[lineUSSDbalance_Tele2Cnt] = { 'A','T','+','C','U','S','D','=','1',',','"','#','1','0','5','#','"',',','1','5'};

// попытка сделать запрос через pdu
// #define lineUSSDbalance_MegafonCnt 25
// const unsigned char lineUSSDbalance_Megafon[lineUSSDbalance_MegafonCnt] = { 'A','T','+','C','U','S','D','=','1',',','"','A','A','1','8','0','C','3','6','0','2','"',',','1','5'};
// #define lineUSSDbalance_MegafonCnt 35
// const unsigned char lineUSSDbalance_Megafon[lineUSSDbalance_MegafonCnt] = 
// 0                   5       7           10                  15                  20              24                                      34
// {'A','T','+','C','U','S','D','=','1',',','"','0','0','2','A','0','0','3','1','0','0','3','0','0','0','3','0','0','0','2','3','"',',','1','5'};




////////////////////////////////////////////////////// 
// возможные сообщения от модема // НЕ ОТВЕТЫ НА КОМАНДЫ
////////////////////////////////////////////////////// 

// успешный старт модема
#define ansModemStartUpCnt 13
//                                                           0   1                                           12
unsigned char const msgModemStartUp[ansModemStartUpCnt] = {'M','O','D','E','M',':','S','T','A','R','T','U','P'};

// успешная регистрация в gsm-сети
#define msgPbReadyCnt 8
unsigned char const msgPbReady[msgPbReadyCnt] = {'+','P','B','R','E','A','D','Y'};

// успешное поступление звонка
#define msgRingCnt 4
unsigned char const msgRing[msgRingCnt] = {'R','I','N','G'};

// успешное поступление sms
#define msgSmsCnt 5
unsigned char const msgSms[msgSmsCnt] = {'+','C','M','T',':'};



////////////////////////////////////////////// 
// возможные ответы модема на посланные команды
////////////////////////////////////////////// 

// 


// информация о качестве GSM-сигнала //// AT+CSQ
#define dataATCSQ_Cnt 7
unsigned char const dataATCSQ[dataATCSQ_Cnt] = {'A','T','+','C','S','Q',':'};  // здесь AT+ оставляем, т.к. он "один раз" тоже нужен
#define msgLevelCnt 5
// unsigned char const msgLevel[msgLevelCnt] = {'+','C','S','Q',':'};
#define msgLevel (&dataATCSQ[2])

// информация о gsm-операторе // AT+COPS?
#define msgGsmInfoCnt 6
unsigned char const msgGsmInfo[msgGsmInfoCnt] = {'+','C','O','P','S',':'};

// ответ на ussd-запрос // AT+CUSD=1,"#102#"
#define msgGsmUssdCnt 6
unsigned char const msgGsmUssd[msgGsmUssdCnt] = {'+','C','U','S','D',':'};

// инфомация о телефонной книге // количество номеров // AT+CPBS?
#define msgGsmBookInfoCnt 6
unsigned char const msgGsmBookInfo[msgGsmBookInfoCnt] = {'+','C','P','B','S',':'};

// выбранный телефонный номер с телефонной книги // AT+CPBR=1 // нумерация телефонных номеров на сим-карте с 1
#define msgGsmPhoneNumberCnt 6
unsigned char const msgGsmPhoneNumber[msgGsmPhoneNumberCnt] = {'+','C','P','B','R',':'};

// статус модема // AT+CPAS
#define dataATCPAS_Cnt 8
unsigned char const dataATCPAS[dataATCPAS_Cnt] = {'A','T','+','C','P','A','S',':'};
#define msgGsmStatusCnt 6
// unsigned char const msgGsmStatus[msgGsmStatusCnt] = {'+','C','P','A','S',':'};
#define msgGsmStatus (&dataATCPAS[2])