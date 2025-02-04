### Загрузчики UNO для нестандартных частот

#### Исходник

Optiboot_flash build output for atmega328p
<https://github.com/MCUdude/optiboot_flash>

#### Загрузка

```bash
avrdude -v -C /etc/avrdude.conf -c usbasp -p m328p -P com1 -U flash:w:boot_m328p_115200_25_MHz.hex
```
