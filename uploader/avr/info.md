### Загрузчики UNO для нестандартных частот

#### Исходник

Optiboot_flash build output for atmega328p
<https://github.com/MCUdude/optiboot_flash>

#### Загрузка

```bash
avrdude -v -C /etc/avrdude.conf -c usbasp -p m328p -P com1 -U flash:w:boot_m328p_115200_25_MHz.hex
```

#### Выставление частоты

Пример изменения на 27МГц:
27 000 000 / 8 / 115 200 + 1 = 30 => 0x1C
-(27 * 61) = 0xF991

В файле boot_m328p_115200_25_MHz.hex произвести замены:

+ 8AE1 => 8CE1 (1C)
+ 2BE0 => 21E9 (91)
+ 3AEF => 39EF (F9)

И обновить хеш суммы.
