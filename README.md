# pribor-kontrolia-vleshka-gsm
Lcd display 2x16, gsm sms with russian simbols, adc10 at 16 KByte binary.

"Прибор контроля VLeshka-GSM" (далее - прибор) представляет собой последовательно подключаемое к нагрузке (переменное напряжение 220В 15А) реле, имеющее LCD-дисплей и 4 кнопки навигации по нему.

Прибор контролирует подаваемое на нагрузку напряжение на допустимое значение (с отправкой смс-уведомлений на русском языке размером 1 смс).

Прибор принимает следующие смс-команды на русском и английском языках (с обязательным ответом на русском языке смс-сообщением размером 1 или 2 смс):
- мониторинг (информация с датчиков);
- постоянное отключение реле;
- временное отключение реле;
- включение реле.

Прибор принимает и отправляет смс только на номера из записной книжки сим-карты. 

Проверена работа со следующими GSM-операторами: билайн, мтс, мегафон, теле2.

Для полной информации см. файл General\gsm_mainTitle.hpp
