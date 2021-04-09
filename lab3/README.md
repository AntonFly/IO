# Лабораторная работа 3

**Название:** "Разработка драйверов сетевых устройств"

**Цель работы:** получить знания и навыки разработки драйверов сетевых интерфейсов для операционной системы Linux

## Описание функциональности драйвера
Создать виртуальный интерфейс, который перехватывает пакеты протокола ICMP (Internet Control Message Protocol) – только тип 8. Сохранять данные.  
Состояние разбора пакетов необходимо выводить в файл `/proc/var1`

## Инструкция по сборке
Для сборки драйвера выполнить:
```bash
make
```

## Инструкция пользователя
После успешной сборки загрузить полученный модуль:
```bash
insmod lab3.ko
```
Проверить, что драйвер загрузился без ошибок с помощью команды `dmesg`, в выводе должно быть подобное:
```
lab3: create link vni0
lab3: registered rx handler for enp0s3
lab3: successfully loaded
lab3: device opened: name=vni0
```

## Примеры использования
Далее начнем отправлять ICMP пакеты на родительский интерфейс `enp0s3`. Выполним серию команд:
```
ping 192.168.43.16

```
Проверим содержимое файла `/proc/var1`:
```
ICMP packets recieved: 4
All packets recieved: 5
Data bytes recieved : 224
```
Также получение ICMP пакетов отображается в `dmesg`:
```
protocol is icmp
Icmp type is 8
protocol is icmp
Icmp type is 8
protocol is icmp
Icmp type is 8
protocol is icmp

Далее посмотрим статистику по принятым пакетам выполнив `ifconfig -a`:
```
vni0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST> mtu 1500
ether 08:00:27:cf:1e:09 txqueuelen 1000 (Ethernet)
RX packets 4 bytes 336 (336.0 B)
RX errors 0 dropped 0 overruns 0 frame 0
TX packets 0 bytes 0 (0.0 B)
TX errors 0 dropped 0 overruns 0 carrier 0 collisions 0
```
Видно, что интерфейс получил ровно 4 ICMP пакетов, которые мы отправили ранее.

После звершения работы можно выгрузить модуль из ядра:
```bash
rmmod lab3
```

