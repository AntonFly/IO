# Лабораторная работа 2

**Название:** "Разработка драйверов блочных устройств"

**Цель работы:** получить знания и навыки разработки драйверов блочных устройств для операционной системы Linux.

## Описание функциональности драйвера

Драйвер блочного устройства создает виртуальный жесткий диск в оперативной памяти с размером 50 Мбайт. Созданный виртуальный диск содержит один первичный раздел размером 10Мбайт и один расширенный раздел, содержащий два логических раздела размером 20Мбайт каждый.

## Инструкция по сборке

Собирается с помощью команды ```make```.

## Инструкция пользователя

Для запуска драйвера необходимо ввести команду ```insmod bl_driver.ko``` , которая загружает модуль ядра в систему.
Для выгрузки модуля драйвера нужно ввести команду ```sudo rmmod bl_driver```.

## Примеры использования

### Вывод разделов виртуального диска

```bash
eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo parted /dev/mydisk
GNU Parted 3.2
Using /dev/mydisk
Welcome to GNU Parted! Type 'help' to view a list of commands.
(parted) unit mib print                                                   
Model: Unknown (unknown)
Disk /dev/mydisk: 50,0MiB
Sector size (logical/physical): 512B/512B
Partition Table: msdos
Disk Flags: 

Number  Start    End      Size     Type      File system  Flags
 1      0,00MiB  10,0MiB  10,0MiB  primary
 2      10,0MiB  50,0MiB  40,0MiB  extended
 5      10,0MiB  30,0MiB  20,0MiB  logical
 6      30,0MiB  50,0MiB  20,0MiB  logical

```

### Форматирование разделов диска с помощью утилиты mkfs.vfat

```bash
eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo mkfs.vfat /dev/mydisk1
mkfs.fat 4.1 (2017-01-24)
eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo mkfs.vfat /dev/mydisk5
mkfs.fat 4.1 (2017-01-24)
eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo mkfs.vfat /dev/mydisk6
mkfs.fat 4.1 (2017-01-24)

```

### Измерение скорости передачи данных при копировании файлов между разделами созданного виртуального диска

```bash
eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo dd if=/dev/mydisk1 of=/dev/mydisk5 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,488883 s, 20,9 MB/s

eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo dd if=/dev/mydisk1 of=/dev/mydisk6 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,461115 s, 22,2 MB/s

eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo dd if=/dev/mydisk5 of=/dev/mydisk6 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,52265 s, 19,6 MB/s

```

### Измерение скорости передачи данных при копировании файлов между разделами виртуального и реального жестких дисков

```bash
eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo dd if=/dev/sda1 of=/dev/mydisk1 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,479736 s, 21,3 MB/s

eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo dd if=/dev/sda1 of=/dev/mydisk5 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,396182 s, 25,8 MB/s

eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo dd if=/dev/sda1 of=/dev/mydisk6 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,488824 s, 20,9 MB/s

eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo dd if=/dev/mydisk1 of=/dev/sda1 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 6,50557 s, 1,6 MB/s

eve@eve-pc:~/Desktop/vv/r/io-an/lab2$ sudo dd if=/dev/mydisk5 of=/dev/sda1 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 6,6681 s, 1,5 MB/s

```
