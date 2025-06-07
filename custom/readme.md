# Альтернативные скрипты

## Скрипты линкера и начальный файл crt0.S

### Особенности

* Реализована инициализация глобальных объектов, при совместном использовании с [SystemInit()](../lib/mik32/source/init.cpp)
* Перенос кода и переменных в оперативную память одним куском
* При записи в eeprom обработчик прерывания работает из eeprom
* Удалёно сохранение регистров из штатного обработчика прерываний, необходим атрибут `__attribute__((used, interrupt,  section(".trap_text")))`

### Установка

* заменить файл `~.platformio/platforms/mik32/builder/frameworks/custom.py` на файл из проекта [custom.py](./builder/custom.py)
* в `platformio.ini` добавить `board_custom = custom` либо вместо `custom`, название этой директории
* если опция `board_custom` не определена, будут использована папка по умолчанию: `~.platformio/packages/framework-mik32v2-sdk/shared`
