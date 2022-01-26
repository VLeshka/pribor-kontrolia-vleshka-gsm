// основной рабочий модуль

#ifndef __gsmUartCommands__
#define __gsmUartCommands__

#include "gsm_mainTitle.hpp"
#include "rus1_mainTitle.hpp"

// для облегчённого доступа из других модулей
#define strCmdCpbs_SimCardCnt 12
unsigned char const strCmdCpbs_SimCard[strCmdCpbs_SimCardCnt] = { 'A','T','+','C','P','B','S','=','"','S','M','"' };


/////////////////////////////
// прототипы

// посылает символ в uart
void SendCharToUart(unsigned char simbol);

// посылает строку в uart И СТАВИТ ОЧЕРЕДЬ НАСТРОЕК
void SendStringToUart( const unsigned char* str, unsigned char strCnt);

// посылает в uart символ окончания команды, для модема
// #define dSendEndSimbolToUart { 
void SendEndSimbolToUart();

// посылаем первую часть смс номеру из phoneBook
// 1.AT+SMGF=0 // при парсинге которого выполняем следующие пункты 2 и 3
// 2.AT+CMGS=xx // количество байт в пакете PDU за исключением поля SCA // Важно, чтобы эта команда завершалась символом '\r' (0Dh), а не чем-то другим
// 3.Ввод данных пакета PDU
void SendSmsToNumberFromPhoneBook();

/*
///////////////////////
// GPS-команды в UART
////////////////////// 

// команды для модуля SIM808
/////////////////////////////

// команда включения GPS-модуля
// GPS power control
void CmdGpsPwr_On();

// команда выключения GPS-модуля
// Close GPS
void CmdGpsClose();

// команда получения информации GPS
// Get currenr GPS location info 
void CmdGpsInfo_0();

// команда получения статуса GPS
// Get currenr GPS status
void CmdGpsStatus_Get();

// команда GPS NMEA data output from AT UART
// GPS NMEA data output control = GSA
void CmdGpsPsout_8();

// команда GPS NMEA data output from AT UART
// GPS NMEA data output control = RMC
void CmdGpsPsout_32();

// команда Reset GPS (Cold Start)
// Reset GPS (Cold Start)
void CmdGpsRst_0();

// команда Reset GPS (Hot Start)
// Reset GPS (Hot Start)
void CmdGpsRst_1();



// команды для модуля A7
/////////////////////////////

// Read Current GPS ASCII data Command Format Query
// AT+GPSRD=?
void CmdGpsRd_Query();

// Read Current GPS ASCII data
// AT+GPSRD=10,0
void CmdGpsRd_10();

// Open GPS
// AT+GPS=1
void CmdGps_1();

// Close GPS
// AT+GPS=0
void CmdGps_0();

// Open AGPS
// AT+AGPS=1
void CmdAGps_1();

// Close AGPS
// AT+AGPS=0
void CmdAGps_0();
*/




///////////////////////
// GSM-команды в UART
////////////////////// 

//////////////// 
// USSD-команды

// USSD-запрос баланса
void CmdAskUssdBalance();

//////////////////////////// 
// другие команды

#ifdef testRezhim

  // отправка смс-сообщения smsText на номер number в GSM-кодировке (без русских букв)
  // Message sending:+CMGS
void SendSms(unsigned char const* smsText, unsigned char smsTextCnt, unsigned char const* number, unsigned char numberCnt);

// посылка смс по частям
// первая часть тестовой смс
void CmdSendTestSms_part1();
// вторая часть тестовой смс
void CmdSendTestSms_part2();

// тестовый звонок
void CmdTestRing();

// Это так называемая пустая команда. 
// После получения этой команды модем автоматически подстраивает скорость передачи и формат знака к параметрам терминала.
void CmdAt();

// Check network registration status
// AT+CREG?
void CmdCreg();

// This command is to check the information of the module,including factory, model and version.
// ati
void CmdAti();

// Get the module’s baud rate
// AT+IPR?
void CmdIprGet();

// считать номер Центра SMS из GSM-модуля
// AT+CSCA?
void CmdCscaInfo();

#endif

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
void CmdCpas();

// команда установки текстового режима
// This command is to set SMS input mode.
// текстовый режим ON
void CmdCmgf_On();
// текстовый режим OFF
void CmdCmgf_Off();

// AT+CSQ - информация о качестве сигнала.
// Модуль вернет два числа +CSQ, например: 17,0
// 17 - уровень сигнала. Может быть в диапазоне 0..31 или 99. Чем больше тем лучше, но 99 значит его отсутствие. 
// 0 - коэффициент ошибок связи (т.н. RXQUAL). Может быть от 0 до 7, чем число меньше тем качество связи лучше.
void CmdAskLevel();

// AT+CLIP=1 - включение определения номера входящего звонка
// Если отправить команду с 0, то определение номера отключится и модуль вместо номера вызывающего будет слать только сообщение "RING".
void CmdAon_On();
void CmdAon_Off();

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
void CmdCscsGsm();
void CmdCscsUcs2();

// Set message indication Format
// This command is to set how to inform the user after receiving new message from the network.
void CmdCnmi();

// куда записываем информацию // указываем сим-карту
// CHOOSE PHONE BOOK STORAGE:+CPBS
void CmdCpbs_SimCard();

// информация о телефонной книге
// Check the phone book storage and total capbility
void CmdCpbs_Check();

// следующий телефонный номер с телефонной книги
void CmdAskNextNumber();

// программный контроль потоком передачи данных
// AT+IFC=1,1
void CmdIfc();

// все смс удалить
// This command is to delete messages from the current storage.
// AT+CMGD=<index> [,<delflag>]
void CmdSmsDelete();

// включает/выключает эхо
// This command is to enable /disable Echo.
// ATE<n>
void CmdAte_On();
void CmdAte_Off();

// Get Error indication message:+CMEE
// AT+CMEE?
void CmdCmeeGet();
    
// Set Error indication message:+CMEE
// AT+CMEE=[<n>]
// <n>:
// Get value:
// 0: disable result code +CME ERROR:<err>, use ERROR,no need to input 0;
// 1: enable result code +CME ERROR:<err>,use numeric <err> to get value;
// 2: enable result code +CME ERROR: <err>,use lengthy <err> to get value.
void CmdCmeeSet_0();
void CmdCmeeSet_1();
void CmdCmeeSet_2();

// информация о GSM-операторе
// Query the network operator
// AT+COPS?
void CmdCopsGet();

// перевести информацию о GSM-операторе в буквенный формат
// Query the network operator
// AT+COPS=0,0
void CmdCopsSet_Alphavit();

#endif // #ifndef __gsmUartCommands__