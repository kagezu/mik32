### Скрипты линкера и начальный файл crt0.S

#### Особенности

* Перенос кода и переменных в оперативную память одним куском
* Конфигурирование выполняется в функции [SystemInit()](../lib/mik32/source/init.cpp)
* Реализована инициализация глобальных объектов C++, после конфигурирования, до функции main()
* При записи в eeprom обработчик прерывания работает из eeprom
* Удалёно сохранение регистров из штатного обработчика прерываний, необходим атрибут `__attribute__((used, interrupt,  section(".trap_text")))`

#### Установка

* заменить файл `~.platformio/platforms/mik32/builder/frameworks/custom.py` на файл из проекта [custom.py](./builder/custom.py)
* в `platformio.ini` добавить `board_custom = ./` или каталог относительно проекта в котором есть папки **ld** и **rt**
* если опция `board_custom` не определена, будут использована папка по умолчанию: `~.platformio/packages/framework-mik32v2-sdk/shared`

### Прочее

#### /bootloader-avr

Загрузчики ATmega328P для нестандартных частот

#### /udev

Файл правил для использования программатора ELJTAG в linux

#### /uploader

Релиз загрузчика для программирования памяти MIK32 Амур платы ELBEAR ACE-UNO.
<https://gitflic.ru/project/elron-tech/elbear_uploader>
