EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:conn
LIBS:valves
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:contrib
LIBS:ecc83-pp-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "ECC Push-Pull"
Date "vendredi 04 juillet 2014"
Rev "0.1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	6250 3900 6350 3900
Wire Wire Line
	5950 3300 5950 3500
Connection ~ 4650 3950
Wire Wire Line
	4650 3900 4650 4850
Wire Wire Line
	4650 3900 4850 3900
Wire Wire Line
	1950 4950 2050 4950
Wire Wire Line
	2050 4950 2050 4750
Wire Wire Line
	4350 4450 4350 4750
Wire Wire Line
	7150 4600 7150 4450
Wire Wire Line
	4350 4250 4650 4250
Connection ~ 4650 4250
Wire Wire Line
	7150 3600 7150 3500
Wire Wire Line
	5050 4300 5050 4850
Wire Wire Line
	6500 3350 6500 3450
Wire Wire Line
	5950 3300 7150 3300
Connection ~ 6900 3300
Wire Wire Line
	6350 3900 6350 3350
Connection ~ 6350 3350
Wire Wire Line
	6500 3950 6500 4350
Connection ~ 6500 4250
Wire Wire Line
	7150 4250 6900 4250
Wire Wire Line
	6900 4250 6900 4850
Wire Wire Line
	2150 4750 2150 4950
Wire Wire Line
	2150 4850 1950 4850
Connection ~ 2150 4850
Wire Wire Line
	4300 4700 4350 4700
Connection ~ 4350 4700
Wire Wire Line
	1950 4850 1950 4750
Wire Wire Line
	5150 3350 6500 3350
Wire Wire Line
	5150 3350 5150 3500
Wire Wire Line
	6050 4300 6050 4350
Wire Wire Line
	6050 4350 6500 4350
$Comp
L ECC83 U1
U 3 1 48B4F266
P 2050 4300
F 0 "U1" H 2180 4610 50  0000 C CNN
F 1 "ECC83" H 2400 4000 50  0000 C CNN
F 2 "Valves:VALVE-ECC-83-1" V 1800 4300 30  0000 C CNN
F 3 "" H 2050 4300 60  0001 C CNN
	3    2050 4300
	1    0    0    -1  
$EndComp
$Comp
L ECC83 U1
U 2 1 48B4F263
P 5150 3900
F 0 "U1" H 5280 4210 50  0000 C CNN
F 1 "ECC83" H 5350 3550 50  0000 C CNN
F 2 "Valves:VALVE-ECC-83-1" H 5350 3500 30  0000 C CNN
F 3 "" H 5150 3900 60  0001 C CNN
	2    5150 3900
	1    0    0    -1  
$EndComp
$Comp
L ECC83 U1
U 1 1 48B4F256
P 5950 3900
F 0 "U1" H 6080 4210 50  0000 C CNN
F 1 "ECC83" H 5700 3600 50  0000 C CNN
F 2 "Valves:VALVE-ECC-83-1" H 6220 3500 50  0001 C CNN
F 3 "" H 5950 3900 60  0001 C CNN
	1    5950 3900
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 457DBAF8
P 7150 4600
F 0 "#PWR01" H 7150 4600 30  0001 C CNN
F 1 "GND" H 7150 4530 30  0001 C CNN
F 2 "" H 7150 4600 60  0001 C CNN
F 3 "" H 7150 4600 60  0001 C CNN
	1    7150 4600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 457DBAF5
P 6900 5350
F 0 "#PWR02" H 6900 5350 30  0001 C CNN
F 1 "GND" H 6900 5280 30  0001 C CNN
F 2 "" H 6900 5350 60  0001 C CNN
F 3 "" H 6900 5350 60  0001 C CNN
	1    6900 5350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 457DBAF1
P 5050 5350
F 0 "#PWR03" H 5050 5350 30  0001 C CNN
F 1 "GND" H 5050 5280 30  0001 C CNN
F 2 "" H 5050 5350 60  0001 C CNN
F 3 "" H 5050 5350 60  0001 C CNN
	1    5050 5350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 457DBAEF
P 4650 5350
F 0 "#PWR04" H 4650 5350 30  0001 C CNN
F 1 "GND" H 4650 5280 30  0001 C CNN
F 2 "" H 4650 5350 60  0001 C CNN
F 3 "" H 4650 5350 60  0001 C CNN
	1    4650 5350
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG05
U 1 1 457DBAC0
P 4300 4700
F 0 "#FLG05" H 4300 4970 30  0001 C CNN
F 1 "PWR_FLAG" H 4300 4930 30  0000 C CNN
F 2 "" H 4300 4700 60  0001 C CNN
F 3 "" H 4300 4700 60  0001 C CNN
	1    4300 4700
	0    -1   -1   0   
$EndComp
$Comp
L CONN_2 P4
U 1 1 456A8ACC
P 2050 5300
F 0 "P4" V 2000 5300 40  0000 C CNN
F 1 "CONN_2" V 2100 5300 40  0000 C CNN
F 2 "connect:bornier2" H 2050 5300 60  0001 C CNN
F 3 "" H 2050 5300 60  0001 C CNN
	1    2050 5300
	0    1    1    0   
$EndComp
$Comp
L C C1
U 1 1 4549F4BE
P 6900 3500
F 0 "C1" H 6950 3600 50  0000 L CNN
F 1 "10uF" H 6650 3600 50  0000 L CNN
F 2 "discret:C2V10" H 6900 3500 60  0001 C CNN
F 3 "" H 6900 3500 60  0001 C CNN
	1    6900 3500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 4549F4B9
P 4350 4750
F 0 "#PWR06" H 4350 4750 30  0001 C CNN
F 1 "GND" H 4350 4680 30  0001 C CNN
F 2 "" H 4350 4750 60  0001 C CNN
F 3 "" H 4350 4750 60  0001 C CNN
	1    4350 4750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 4549F4B3
P 7150 3600
F 0 "#PWR07" H 7150 3600 30  0001 C CNN
F 1 "GND" H 7150 3530 30  0001 C CNN
F 2 "" H 7150 3600 60  0001 C CNN
F 3 "" H 7150 3600 60  0001 C CNN
	1    7150 3600
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P3
U 1 1 4549F4A5
P 7500 3400
F 0 "P3" V 7450 3400 40  0000 C CNN
F 1 "POWER" V 7550 3400 40  0000 C CNN
F 2 "connect:bornier2" H 7500 3400 60  0001 C CNN
F 3 "" H 7500 3400 60  0001 C CNN
	1    7500 3400
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P2
U 1 1 4549F46C
P 7500 4350
F 0 "P2" V 7450 4350 40  0000 C CNN
F 1 "OUT" V 7550 4350 40  0000 C CNN
F 2 "connect:bornier2" H 7500 4350 60  0001 C CNN
F 3 "" H 7500 4350 60  0001 C CNN
	1    7500 4350
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P1
U 1 1 4549F464
P 4000 4350
F 0 "P1" V 3950 4350 40  0000 C CNN
F 1 "IN" V 4050 4350 40  0000 C CNN
F 2 "connect:bornier2" V 4150 4350 60  0000 C CNN
F 3 "" H 4000 4350 60  0001 C CNN
	1    4000 4350
	-1   0    0    1   
$EndComp
$Comp
L C C2
U 1 1 4549F3BE
P 6700 4250
F 0 "C2" H 6750 4350 50  0000 L CNN
F 1 "680nF" H 6750 4150 50  0000 L CNN
F 2 "discret:C2" H 6700 4250 60  0001 C CNN
F 3 "" H 6700 4250 60  0001 C CNN
	1    6700 4250
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 4549F3AD
P 6900 5100
F 0 "R3" V 6980 5100 50  0000 C CNN
F 1 "100K" V 6900 5100 50  0000 C CNN
F 2 "discret:R3" H 6900 5100 60  0001 C CNN
F 3 "" H 6900 5100 60  0001 C CNN
	1    6900 5100
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 4549F3A2
P 4650 5100
F 0 "R4" V 4730 5100 50  0000 C CNN
F 1 "47K" V 4650 5100 50  0000 C CNN
F 2 "discret:R3" H 4650 5100 60  0001 C CNN
F 3 "" H 4650 5100 60  0001 C CNN
	1    4650 5100
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 4549F39D
P 5050 5100
F 0 "R2" V 5130 5100 50  0000 C CNN
F 1 "1.5K" V 5050 5100 50  0000 C CNN
F 2 "discret:R3" H 5050 5100 60  0001 C CNN
F 3 "" H 5050 5100 60  0001 C CNN
	1    5050 5100
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 4549F38A
P 6500 3700
F 0 "R1" V 6580 3700 50  0000 C CNN
F 1 "1.5K" V 6500 3700 50  0000 C CNN
F 2 "discret:R3" H 6500 3700 60  0001 C CNN
F 3 "" H 6500 3700 60  0001 C CNN
	1    6500 3700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 53B6F370
P 6900 3700
F 0 "#PWR08" H 6900 3700 30  0001 C CNN
F 1 "GND" H 6900 3630 30  0001 C CNN
F 2 "" H 6900 3700 60  0001 C CNN
F 3 "" H 6900 3700 60  0001 C CNN
	1    6900 3700
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P5
U 1 1 54A5890A
P 6050 7050
F 0 "P5" H 6130 7050 40  0000 L CNN
F 1 "CONN_1" H 6050 7105 30  0001 C CNN
F 2 "connect:1pin" H 6050 7050 60  0001 C CNN
F 3 "" H 6050 7050 60  0000 C CNN
	1    6050 7050
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P6
U 1 1 54A58C65
P 6050 7150
F 0 "P6" H 6130 7150 40  0000 L CNN
F 1 "CONN_1" H 6050 7205 30  0001 C CNN
F 2 "connect:1pin" H 6050 7150 60  0001 C CNN
F 3 "" H 6050 7150 60  0000 C CNN
	1    6050 7150
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P7
U 1 1 54A58C8A
P 6050 7250
F 0 "P7" H 6130 7250 40  0000 L CNN
F 1 "CONN_1" H 6050 7305 30  0001 C CNN
F 2 "connect:1pin" H 6050 7250 60  0001 C CNN
F 3 "" H 6050 7250 60  0000 C CNN
	1    6050 7250
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P8
U 1 1 54A58CA3
P 6050 7350
F 0 "P8" H 6130 7350 40  0000 L CNN
F 1 "CONN_1" H 6050 7405 30  0001 C CNN
F 2 "connect:1pin" H 6050 7350 60  0001 C CNN
F 3 "" H 6050 7350 60  0000 C CNN
	1    6050 7350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
