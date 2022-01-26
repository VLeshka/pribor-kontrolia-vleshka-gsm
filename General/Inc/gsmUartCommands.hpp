// �������� ������� ������

#ifndef __gsmUartCommands__
#define __gsmUartCommands__

#include "gsm_mainTitle.hpp"
#include "rus1_mainTitle.hpp"

// ��� ������������ ������� �� ������ �������
#define strCmdCpbs_SimCardCnt 12
unsigned char const strCmdCpbs_SimCard[strCmdCpbs_SimCardCnt] = { 'A','T','+','C','P','B','S','=','"','S','M','"' };


/////////////////////////////
// ���������

// �������� ������ � uart
void SendCharToUart(unsigned char simbol);

// �������� ������ � uart � ������ ������� ��������
void SendStringToUart( const unsigned char* str, unsigned char strCnt);

// �������� � uart ������ ��������� �������, ��� ������
// #define dSendEndSimbolToUart { 
void SendEndSimbolToUart();

// �������� ������ ����� ��� ������ �� phoneBook
// 1.AT+SMGF=0 // ��� �������� �������� ��������� ��������� ������ 2 � 3
// 2.AT+CMGS=xx // ���������� ���� � ������ PDU �� ����������� ���� SCA // �����, ����� ��� ������� ����������� �������� '\r' (0Dh), � �� ���-�� ������
// 3.���� ������ ������ PDU
void SendSmsToNumberFromPhoneBook();

/*
///////////////////////
// GPS-������� � UART
////////////////////// 

// ������� ��� ������ SIM808
/////////////////////////////

// ������� ��������� GPS-������
// GPS power control
void CmdGpsPwr_On();

// ������� ���������� GPS-������
// Close GPS
void CmdGpsClose();

// ������� ��������� ���������� GPS
// Get currenr GPS location info 
void CmdGpsInfo_0();

// ������� ��������� ������� GPS
// Get currenr GPS status
void CmdGpsStatus_Get();

// ������� GPS NMEA data output from AT UART
// GPS NMEA data output control = GSA
void CmdGpsPsout_8();

// ������� GPS NMEA data output from AT UART
// GPS NMEA data output control = RMC
void CmdGpsPsout_32();

// ������� Reset GPS (Cold Start)
// Reset GPS (Cold Start)
void CmdGpsRst_0();

// ������� Reset GPS (Hot Start)
// Reset GPS (Hot Start)
void CmdGpsRst_1();



// ������� ��� ������ A7
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
// GSM-������� � UART
////////////////////// 

//////////////// 
// USSD-�������

// USSD-������ �������
void CmdAskUssdBalance();

//////////////////////////// 
// ������ �������

#ifdef testRezhim

  // �������� ���-��������� smsText �� ����� number � GSM-��������� (��� ������� ����)
  // Message sending:+CMGS
void SendSms(unsigned char const* smsText, unsigned char smsTextCnt, unsigned char const* number, unsigned char numberCnt);

// ������� ��� �� ������
// ������ ����� �������� ���
void CmdSendTestSms_part1();
// ������ ����� �������� ���
void CmdSendTestSms_part2();

// �������� ������
void CmdTestRing();

// ��� ��� ���������� ������ �������. 
// ����� ��������� ���� ������� ����� ������������� ������������ �������� �������� � ������ ����� � ���������� ���������.
void CmdAt();

// Check network registration status
// AT+CREG?
void CmdCreg();

// This command is to check the information of the module,including factory, model and version.
// ati
void CmdAti();

// Get the module�s baud rate
// AT+IPR?
void CmdIprGet();

// ������� ����� ������ SMS �� GSM-������
// AT+CSCA?
void CmdCscaInfo();

#endif

// ������� �������� ������� ������
// AT+CPAS
// �����:
// 0: ready(the module can implement AT commands)
// 2: unknow(unknown status)
// 3: ringing(the module can implement AT command,it will be ringing status when
//    there is an incoming call)
// 4: call in progress(the module can implement AT command, in call connecting or
//    caller ringing status.)
// 5: asleep(Module is in sleep mode, not ready)
void CmdCpas();

// ������� ��������� ���������� ������
// This command is to set SMS input mode.
// ��������� ����� ON
void CmdCmgf_On();
// ��������� ����� OFF
void CmdCmgf_Off();

// AT+CSQ - ���������� � �������� �������.
// ������ ������ ��� ����� +CSQ, ��������: 17,0
// 17 - ������� �������. ����� ���� � ��������� 0..31 ��� 99. ��� ������ ��� �����, �� 99 ������ ��� ����������. 
// 0 - ����������� ������ ����� (�.�. RXQUAL). ����� ���� �� 0 �� 7, ��� ����� ������ ��� �������� ����� �����.
void CmdAskLevel();

// AT+CLIP=1 - ��������� ����������� ������ ��������� ������
// ���� ��������� ������� � 0, �� ����������� ������ ���������� � ������ ������ ������ ����������� ����� ����� ������ ��������� "RING".
void CmdAon_On();
void CmdAon_Off();

// AT+CSCS="GSM" - ����������� ��������� ������
// AT+CSCS=<chset> - Set TE character sets:
// < chset >:
// 1. �GSM�: GSM default alphabet (GSM03.38.6.2.1)
// 2. �HEX�: character strings consist only of hexadecimal numbers from 00 to
//     FF; e.g. "032FE6" equals three 8-bit characters with decimal values 3, 47
//     and 230; no conversions to the original MT character set shall be done
// 3. �IRA�: international reference alphabet.(ITU-T T.50).
// 4. �PCCP437�: PC character set Code Page 437
// 5. �8859-1�: ISO 8859 Latin 1 character set
// 6. �UCS2�: 16-bit universal multiple-octet coded character set
//     (USO/IEC10646); UCS2 character strings are converted to hexadecimal
//     numbers from 0000 to FFFF. Only the strings found in quotation marks are
//     UCS2 coded, the rest of commands or responses, remains in IRA alphabet.
void CmdCscsGsm();
void CmdCscsUcs2();

// Set message indication Format
// This command is to set how to inform the user after receiving new message from the network.
void CmdCnmi();

// ���� ���������� ���������� // ��������� ���-�����
// CHOOSE PHONE BOOK STORAGE:+CPBS
void CmdCpbs_SimCard();

// ���������� � ���������� �����
// Check the phone book storage and total capbility
void CmdCpbs_Check();

// ��������� ���������� ����� � ���������� �����
void CmdAskNextNumber();

// ����������� �������� ������� �������� ������
// AT+IFC=1,1
void CmdIfc();

// ��� ��� �������
// This command is to delete messages from the current storage.
// AT+CMGD=<index> [,<delflag>]
void CmdSmsDelete();

// ��������/��������� ���
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

// ���������� � GSM-���������
// Query the network operator
// AT+COPS?
void CmdCopsGet();

// ��������� ���������� � GSM-��������� � ��������� ������
// Query the network operator
// AT+COPS=0,0
void CmdCopsSet_Alphavit();

#endif // #ifndef __gsmUartCommands__