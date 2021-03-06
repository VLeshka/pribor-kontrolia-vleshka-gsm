// ???????? ?????? ???????? ?????? ? ???????? "?????? ???????? VLeshka-GSM"

#ifndef __gsm_mainTitle__
#define __gsm_mainTitle__

// ?????? "?????? ???????? VLeshka-GSM" ?????? ? ?????????????? ???????????? ????? ???-1 (??. rus1_mainTitle.h).

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ????????? ????????????
////////////////////////////////////////////////////////////////////// 
// 
// ??????? ?????????:
// ????? ?????????  - "???????" ?????????????? ?????????.
// ?????? ??????????????? ???????? ???????? - ???? ????? ??????????? ? ???????.
// ?????? ??????????????? ???????? ??? ? ??????? - GSM-???? ??????????, ????? ????????? ????????, ?????? 
//   ???????? ?????????.
// 
// ??????? ?????????:
// ????? ????????? - "??????" ?????????????? ?????????.
// ?????? ??????? ???????? - ???????? ??????????? ???????. ????????? ?? ???? ??????????? ??????? ????????? 
//   ? ?????????; ???? ????????? ? ??????????? ??????? ?????? (???????? ??????), ?? ??? ????????? ????????? ? 
//   ???????? ????????.



///////////////////////////////////////////////////////////////////////////////////////////////
// ???????? ?????????? ?? ????????
///////////////////////////////////////////////////////////////////////////////////////////////
// 
// ??? ??????? ?? ????????? ? ?????????? ???? ???????? ?????? ????? ???-?????????.
// 
// ??????????? ?????????? ?????????? ? ?????????? ?????????? ?????????? ???????? ? ?????????? ??????? 
// "??????????? ?????????? ?????????? ????????" ? "?????????? ?????????? ?????????? ????????" ??????????????.
// 
// ????? ?????????? ???????? ??? ??????????? ?????? ?????????? ?? ??????? ?????:
// - ?? ??????????? ?????????? ?????????? - 15 ??????????? (??????? ???????? ? ???????? ??????????? ????????? ????
// 5 ??????????? + ????? ??????? ???? (?????? 8, ???????? 15 ???????????).
// - ?? ?????????? ?????????? ?????????? - 500 ??????????? (???????? ??????????? ????? 500 ??????????? + ????? 
// ??????? ???? (?????? 8, ???????? 15 ???????????)).
// 
// ????? ???? ????????, ?????? ?????? "?? ??????" ? ???????? ???????? ??? ?????? ?????????? ?? ??????? ???????????
// ????????.
// ?????????, ????? ???? ???????? ? ?????? "?? ?? ??????", ?? ?????????????.
// 
// ? ??????, ????? ???? ????????? ????????????? (??? ?????? ?????????? ?? ??????? ???????????), ? ?????????? 
// "?????????? ????" ??????? ??????????, ??? ???? ????????? ?????????????.
// 
// ????? ???? ????????? ??????? (???????? ?????????), ? ?????????? "?????????? ????" ??????? ??????????, ??? ???? 
// ????????? ???????. ????? ???????? ????, ???? ????????? ??????? ????????.


///////////////////////////////////////////////////////////////////////////////////////////
// ???????? ??????????
///////////////////////////////////////////////////////////////////////////////////////////
// 
// ???????? ?????????? ???????? ? ????????? ??????? "???????? ????".
// ???????? ?????????? ??????????? ??? ??????????? ? ??????????? ???????:
// - ???????? ?????? ? ??????? ??????? (????????? ? ?????????????? ???????).
// - ???????? ?????? ?? ????? ??????? ??????? (??????? ??????? ?? ???).
// - ???????? ?????? ? ?????? ??????? (??????????? ???????????? ??????).
// - ???????? ?????? ?? ????? ?????? ??????? (????? ?????????? ?? ???????? ?? ??????? ?????, ??????????? ??????).



///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ????????? ?????? ??? ?????? ? ??????? ? ???????? ???????? ??????????? ??????
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// ????????? ?????? ??????:
// - ??????????? ?????? - ??????, ?? ??????????? ???????? ??????. ????? ?????? ????????? ?? ??????.
// - ????????????? ?????? - ??????, ??????? ???????? ????? ?????? ????????? ??????. ?????????, ??? ??? ???????? 
// ??-?? ????????? ??????, ? ?? ??????????????? ??????? ?????????? ?????? ????????????? ??????, ???????? ???????
// ?????? ?? ????????? ?????? (??? ?????? ??? ???? ?? ???????????????).
// ??????????:
// ?? ????????????????? ??????? ?????? watchdog (????????? ????????????? ????? ???????? ??? ?????????? ???????).
// 
// ??? ?????? ??????????????? ????????? ?? ?????.
// ????????? ? ????? ?????? ????? ?????? ???????? ??????? ? ???????? ?????? ?? ?????? ?????????, - ??????? ???? 
// ? ???, ??? ??? ??????? ??????????? ?????? ???????? ??????? ????????? ?????????? ???, ????? ???????? ??????? 
// ?????????? ?????? ?? ???????? "??????????" ??-???????.
// 
// ????? ????????? ???????, ?????? ????? ???????????? ?????? ?? ?????????? ??????????? ??????.
// ?????? ??????????? ?????? (? ??????? ???????????????? ??? ????????? ???????):
// 1. ????? ?? ??????????.
// 2. ??????????? ???-????? ? ??????.
// 3. GSM-???? ???????????.
// 4. ??????????? ?????? ? ???????? ?????? ???-?????.
// 
// ????? ????????? ????????? ??????, ?? ????????????? ?????????????? ? GSM-????, ??? ???????? ?????? ?????? 
// 5 ??????. 
// ???? GSM-????? ????? ??????, ?? ???????? ?????????? ????? ? ???????? ????????? ?? ?????? ?? ???????? 
// ??????????? ? GSM-????. 
// ? ?? ?? ?????, ???? ???-????? ??????????? ??? ??????????, ???????? ????????? ?? ?????? ?? ???????? 
// ??????????? ?????????? ???????????. 
// ?? ????????? ?????, ????? ????????? ????????? ?????? (????? ????, ??? ????? ??????? ????? 'MODEM:STARTUP')
// ?????? ???????? ?????? ?? ????????? ???-????? ? ?????? (??? ????? ?????? ?? ?? 1 ?? 5 ??????). ???? 
// ???-????? ????????????, ????????? ????????? ? ????????? ?????? ? ????????? ?? ???? ????????? '+PBREADY'.
// ????? ????????? ????????? ?? ?????? ? ??????????? ? GSM-????, ???????????? ?????????? ??? ?????? ???????? 
// ? ?????????.
// 
// ?????????? 1.
// ???? ???????? ????????? ?????????? ??????? ? ???????? ?????? ?????? ?? ??????????? ?????? ? GSM-????, ??
// ?? ??????? 'AT+CPBS?' ????? ???????? ?????? ????? ????????? ?????? (?? 7 ??????) ????? ????????? (?? 
// ????? ??????? ?? ??????? 'AT+CPBS?' ?????????? 'ERROR'). ??????? ???????? ????? ??????? ????????? ? 
// ??????????? ? GSM-???? ?????? ??? ?? ????? ???????, ????? ???????? ?????? ?????????? ??????? ? ???????? 
// ?????? ???-?????.
// 
// ?????????? 2.
// ????? ???, ??? ??????? ????????? ??????? ??????, ??? ?????????? ??????? ???????? ??????? ??????, ? ?????? 
// ????? ????? ??????? ?????????? ??????.







////////////////////////////////////////////////////////////////// 
// ????????? ?????? ????????? ?????????
////////////////////////////////////////////////////////////////// 
// ???????
// Root(0xE000)--->??????????--->?????????? ????----------------------------------------------->data ?????????? ????
//              |             |->?????? ???-?????---------------------------------------------->data ?????? ???-?????
//              |             |->??????? GSM-???????------------------------------------------->data ??????? GSM-???????
//              |             |->???? ? GSM-?????????------------------------------------------>data ???? ? GSM-?????????
//              |->?????????---->????? ????? ????. ? ???. ???? ??? ????????????---------------->data ????? ????? ????. ? ???. ???? ??? ????????????
//                            |->??????????? ?????????? ?????????? ????????-------------------->data ??????????? ?????????? ?????????? ????????
//                            |->?????????? ?????????? ?????????? ????????--------------------->data ?????????? ?????????? ?????????? ????????
//                            |->???????? ?? ??? ??? ?????? ?????????? ?? ?????????? ???????--->data ???????? ?? ??? ??? ?????? ?????????? ?? ?????????? ???????
//                            |->???????? ????------------------------------------------------->data ???????? ????
//                            |->???????? ??? ?????????---------------------------------------->data ???????? ??? ?????????



//////////////////////////////////////////// 
// ???????? ???????
//////////////////////////////////////////// 

// ?????????? ??????????? ??????, ???? ??? ????????????. ??? ?? ?????????? ??-??????? ?????????? ?????? ?? 
// ???????? "??????????".


/////////////////////////////////////////
// ??????? ??????????
/////////////////////////////////////////


/////////////////////////////////////////
// ?????????? ????
// 
// ?????????? ?????????? ?? ???? ??????????, ? ????? ????????? ???? (???./????.).


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ?????? ???-?????
// 
// ???? ????????? ?????? ?? ?????????, ?? ???????????? "????????? ??????".
// ???? ????????? ?????? ?????????, ?? ?? ??????? ?????????? GSM-????????? ??? ?? ??????? ???????? ????? ?? 
// USSD-?????? ?? ??????, ???????????? "??? GSM-?????!".
// ???? USSD-?????? ?????? ???????, ???????????? ?????? ???-?????. ?????? ???????????? ????? ?????? ?????, ??????????? ? 
// ?????? ?? USSD-?????? (??????? ????? "?????" ? "???????"). ???? ????? ? ?????? ?? USSD-?????? ?????????????, ?? ?????? 
// ???????? ??????? ???????????? ???? ??????? "?".
// 
// ?????? ??? ? GSM-????????? ????????????? ?????????? ?? ?????????? ??????? ?? ????? ???-?????.
// ??? ??, ????? ????????? ???????? ???-?????????, ??????????????? ????????????? ? GSM-????????? ?????????? ??
// ?????????? ??????? ?? ????? ???-?????. ??????? ???? ? ????, ??? GSM-???????? ????? ?? ????? ????????? 
// ?????????? ? ??????? ???-????? ?????? ??????? (? ?????? ??????, ???????) ????? ???????? ???????? ???-?????????!
// ????? ????????? ???????, ????? ??????????? ??? ??????????. 
// 
// ????????? ?????? ??????? ? ???????: ???????, ?? ?????????? GSM-???????????: ??????, ???, ???????, ????2.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ??????? GSM-???????
// 
// ?????? ?????? ??????????? ?????? ?? ?????? GSM-???????.
// ??? ??, ????? ????????? ???????, ????? ??????????? ??? ??????????.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ?????????? ? GSM-?????????
// 
// ???? ????????? ?????? ?? ?????????, ?? ???????????? "????????? ??????".
// ???? ????????? ?????? ?????????, ?? ? ????? ?? ?????? ? GSM-????????? ????? ???????? ???????, ???????????? "??? GSM-?????!".



///////////////////////////////////////////////////////////////////////////////////////////////////////
// ??????? ?????????
///////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ????? ????? ????. ? ???. ???? ??? ????????????
// 
// ?????????? ? ?????? ?????, ? ????????, ??? ?????????? ???????? ???-?????????? ??????? RESTART ??? ????????????.
// ???????? ????? ?? 3 ?? 86400 (24 ????) ??????.

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ??????????? ?????????? ?????????? ????????
// 
// ?????????? ? ?????? ??????????? ?????????? ?????????? ???????? ? ???????, ??? ?????????? ??????? ??????
// ????????? ???????? ?, ???? ??????? ? ????????? "???????? ?? ??? ??? ?????? ?????????? ?? ?????????? ???????",
// ?????????? ???????? ???-????????? ???? ????????? ?? ?????????? ????? ???-?????.

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ?????????? ?????????? ?????????? ????????
// 
// ?????????? ? ?????? ?????????? ?????????? ?????????? ???????? ? ???????; ???? ?????????? ?? ???????? 
// ?????????? ???? ?????????? ??????????? ??????????, ?????? ????????? ???????? ?, ???? ??????? ? ????????? 
// "???????? ?? ??? ??? ?????? ?????????? ?? ?????????? ???????", ?????????? ???????? ???-????????? ???? 
// ????????? ?? ?????????? ????? ???-?????.

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ???????? ?? ??? ??? ?????? ?????????? ?? ?????????? ???????
// 
// ?????????? ? ?????? ????????? (??/???), ????? ?? ????????????? ???????? ???-????????? ???? ????????? ?? 
// ?????????? ????? ???-????? ??? ?????? ?????????? ?? ?????????? ???????.

////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// ???????? ????
// 
// ?????????? ? ?????? ????????? (??/???), ???????? ??? ????????? ???? ???????.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ???-?????????
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// ?????? ????? ????????? ??????? (??. ?????? ?????? ??? ????????????) ?? ??????? ? ?????????? ??????. 
// ??????? ???????????? ????? ?????.
// 
// ?????? ?????????? ?????? ???-????????? ???????? 1 ??? 2 ??? ?? ??????? ?????.
// 
// ?????? ??? ?? ???-????? ????????? ??? ???-?????????.
// ????? ????????? ???????, ????? ????? ????????? ??? ?????????? ???-?????????.
// 
// ????? ???????? ???-?????????, ?????? ?????????? ??? ?????? ? ?????? ???-????? ?? ??????? ?????????? ? ?????? 
// ???-????? ? ??????? ????????, ?????????? ???-?????????. ???? ?????????? ?? ???????, ???-????????? ????????????, 
// ????? ???????????? ?????? ??????? ? ???-?????????.
// 
// ??? ??????????? ???-????????? ?????? ???????????? ??? ??????? (?????, ????? ??? - ??? "???????").
// 
// ??????? ????? ???? ???????? ??? ????????, ??? ? ?????????? ??????? ? ????? ?????: ??????? ???????????? 
// ? ???????????? - ?????????? ???????.
// ? ?????? ?????? ??????? ????????????.
// ????? ??????? ????? ???? ????? ?????????? ???????? (??. ??????????? ???????).
// 
// ?????? ?????? ??? ????????????:
// ? - ?????????? (?????????? ? ????????).
// RESTART, ???????????? - ????????? ? ???????? ????????.
// ON, ???????? - ???????? ????????.
// OFF, ????????? - ????????? ????????.
// ??????? ??? ????? ???????????? ??????? - ?????????? ? ????? ??????? ? ???? ?????????????? ????????. ????? 
// ???????, ?????? ?????? ?????????? ? ????? ????? ??? (??????? ??? ????? ?? ???????).
// 
// ?? ??? ???????, ????? ??????? ???????, ????? ?????????? ??? ???????? 1 ???; ?? ??????? ??????? ????? ??????????
// ??? ???????? 2 ???.
// 
// ??????????, ??????? ??????? ?????? ?????? "??????? ?????????", ??? ????????? ??????? ? ????????????????? ? 
// ??????????????? ??????? ????????????. ????? ???? "?????? ????????" ????? ???? ????? ?????????? ????? ????????, 
// ??? ??? ??????? ??????????? ?? "?????? ????????". ????? ???????, ????????, ??????? ????????????, ????????????, 
// ????, ????123ZXC ??????????? ??? ??????? ????????????. 
// 
// ????? ????? ? ???????? ???? ?????? ??? ????????? ????????? ?????? ?????????????.
// 
// ??????????? ???????:
// ? - ?????????? (?????????? ? ????????).
// RESTART, ???? - ????????? ? ???????? ????????.
// ON, ??? - ???????? ????????.
// OFF, ???? - ????????? ????????.
// ??????? "???????" ??? - ??? "??????????" ??????? ?????? ?????????? ? ????? ??????? ? ???? 
// ?????????????? ????????.
// 
//  ?????? ?????? ?? ??????? ???????:
// ? - ??????????
// ????????????, RESTART - ???? ? ????????
// ????????, ON - ????????
// ?????????, OFF - ?????????
// ??????? - ???????
// ??????123.
// 
//  ?????? ?????? ?? ??????? ???????????? ??? RESTART:
// ??????? "????????????": ???? ???. ????? 86400?.
// ?????????? 220?
// ??????100,9
// 
//  ?????? ?????? ?? ??????? ????????? ??? OFF:
// ??????? "?????????" ?????????
// ???? ?????????
// ?????????? 220?
// ??????100,9
// 
//  ?????? ?????? ?? ??????? ???????? ??? ON:
// ??????? "????????" ?????????
// ???? ????????
// ?????????? 220?
// ??????100,9
// 
//  ?????? ?????? ?? ??????? ?:
// ???? ?????????
// ?????????? 220?
// ?????? 100%
// ??? ??????? 200
// ??????100,9
// 
// ??????????:
// ???? ??????????? ??????? RESTART, ?? ? ????? ?? ??????? ? ???????????? ??? ??? ??? ?????? ?? ??????? 
// ???????????? ??? RESTART ? ????????? ?????????? ?????? ?? ????????? ???? (??????? ?????????????).
// 
// ???? ? ?????????? ??????? "?????????? ?? ??? ??? ?????? ?????????? ?? ?????????? ???????" ??????? "??",
// ?? ??????, ??? ?????? ?????????? ?? ???????? ?? ??????? ?????, ?????????? ???-????????? (???????? 
// 1 ???) ???? ?????????, ?????????? ? ?????????? ????? ???-?????.
// 
// ??? ?????? ?????????? ?? ??????? ????? ?????? ?????? ??????, ????????????? ?? ??? ??????????. ??? ????? ??????
// ??????????? ????? (15 ??????). ???? ?? ??? ????? ?????????? ??????????????, ?? ?????? ?????? ???????? ???? 
// ????????? ???-????????? ?????? ???? (??????):
// 
// ?????????? 220? ? 5? ???? ???? ?????? 250?
// ???? ????????
// ??????100,0
// 
// ?????????? 220? ? 5? ???? ???? ?????? 177?
// ???? ????????
// ??????100,0
// 
// ???? ?? ????? ???????????? ????? ?????????? ?? ??????????????, ?????? ?????? ???????? ???? ????????? 
// ???-????????? ?????? ???? (??????):
// 
// ?????????? ???? ?????? 250?
// ???? ?????????
// ?????????? 220?
// ??????100,0
// 
// ?????????? ???? ?????? 177?
// ???? ?????????
// ?????????? 160?
// ??????100,0
// 
// ????? ?????????????? ??????????, ?????? ?????? ???????? ???? ????????? ???-????????? ?????? ???? (??????):
// 
// ?????????? ???????????????
// ???? ????????
// ?????????? 220?
// ??????100,0


// ???????? ????? ???????
// #ifdef testRus1
// #define testRezhim testRus1
// #endif

// ?????????? 16-?????? ????? ?????? f_generalFlag ??? ???????? ??? // ???????? 14 ????
// ????? ?????????? ????? ?? ?????? // ???????? 138 ????
// #define useGeneralFlag

#endif // #ifndef __gsm_mainTitle__