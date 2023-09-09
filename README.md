# CH32V003 Stereo FM Transmitter
With the portable Li-Ion battery powered Stereo FM Transmitter you can transmit audio of any kind. Simply plug your audio source into the 3.5mm audio jack, set the desired frequency, and then tune any standard FM radio receiver to receive the signal. This device is equipped with cost-effective components, including a CH32V003J4M6 32-bit RISC-V microcontroller, a KT0803K/L transmitter IC, a 128x32 pixel OLED display (SSD1306), and a Li-Ion battery charger (TP4054).

![FM_Transmitter_pic1.jpg](https://raw.githubusercontent.com/wagiminator/CH32V003-FM-Transmitter/main/documentation/FM_Transmitter_pic1.jpg)

# Hardware
## Schematic
![FM_Transmitter_wiring.png](https://raw.githubusercontent.com/wagiminator/CH32V003-FM-Transmitter/main/documentation/FM_Transmitter_wiring.png)

## The CH32V003 Family of 32-bit RISC-V Microcontrollers
The CH32V003 series is a collection of industrial-grade general-purpose microcontrollers that utilize the QingKe RISC-V2A core design supporting the RV32EC instruction set. These microcontrollers are equipped with various features such as a 48MHz system main frequency, 16KB flash, 2KB SRAM, wide voltage support, a single-wire serial debug interface, low power consumption, and an ultra-small package. Additionally, the CH32V003 series includes a built-in set of components including a DMA controller, a 10-bit ADC, op-amp comparators, multiple timers, and standard communication interfaces such as USART, I2C, and SPI.

## KT0803K or KT0803L Radio-Station-on-a-Chip
The KT0803K/L is a low cost Monolithic Digital Stereo FM Transmitter, designed to process high-fidelity stereo audio signal and transmit modulated FM signal over a short range. The KT0803K/L features dual 20-bit ΔΣ audio ADCs, a high-fidelity digital stereo audio processor and a fully integrated radio frequency (RF) transmitter. An on-chip low-drop-out regulator (LDO) allows the chip to be integrated in a wide range of low-voltage battery-operated systems with power supply ranging from 1.6V to 3.6V. The KT0803K/L can be controlled via its I²C interface.

## TP4054 Li-Ion Battery Charge IC
The TP4054 is a complete constant-current/constant-voltage linear charger for single cell lithium-ion batteries. Its small package and low external component count make the TP4054 ideally suited for portable applications.

## ME6209 3.3V Linear Voltage Regulator
The ME6209 series are a group of positive voltage output, three–pin regulator, that provide a high current (max 250mA) even when the input/output voltage differential is small (80mV dropout voltage). Low power consumption (3µA quiescent current) and high accuracy (+/-2%) is achieved through CMOS technology. They allow input voltages as high as 18V.

# Building Instructions
Solder all components to the PCB. Solder the wire antenna to the corresponding pad on the board. A 75cm (30" = λ / 4) long 28AWG flexible silicone insulated wire works very well.

![FM_Transmitter_pic4.jpg](https://raw.githubusercontent.com/wagiminator/CH32V003-FM-Transmitter/main/documentation/FM_Transmitter_pic4.jpg)

3D print the case. Glue the battery into the case with double-sided tape. Thread the wire antenna through the small hole in the housing.

![FM_Transmitter_pic5.jpg](https://raw.githubusercontent.com/wagiminator/CH32V003-FM-Transmitter/main/documentation/FM_Transmitter_pic5.jpg)

Connect the battery to the JST connector on the board. Pay attention to the correct polarity, unfortunately there is no standard here! Place the board on the case and screw it with four M2x5mm self-tapping screws.

![FM_Transmitter_pic6.jpg](https://raw.githubusercontent.com/wagiminator/CH32V003-FM-Transmitter/main/documentation/FM_Transmitter_pic6.jpg)

# Compiling and Uploading Firmware
## Programming and Debugging Devices
To program the CH32V003 microcontroller, you will need a special programming device which utilizes the proprietary single-wire serial debug interface (SDI). The [WCH-LinkE](http://www.wch-ic.com/products/WCH-Link.html) (pay attention to the "E" in the name) is a suitable device for this purpose and can be purchased commercially for around $4. This debugging tool is not only compatible with the CH32V003 but also with other WCH RISC-V and ARM-based microcontrollers.

![CH32V003_wch-linke.jpg](https://raw.githubusercontent.com/wagiminator/Development-Boards/main/CH32V003F4P6_DevBoard/documentation/CH32V003_wch-linke.jpg)

To upload the firmware, you need to ensure that the FM Transmitter is switched off or the battery is removed. Then, you should make the following connections to the WCH-LinkE:

```
WCH-LinkE    FM Transmitter
+-------+      +-------+
|  SWDIO| <--> |DIO    |
|    GND| ---> |GND    |
|    3V3| ---> |3V3    |
+-------+      +-------+
```

If the blue LED on the WCH-LinkE remains illuminated once it is connected to the USB port, it means that the device is currently in ARM mode and must be switched to RISC-V mode initially. There are a few ways to accomplish this:
- You can utilize the Python tool called rvmode.py, which is provided in the software/tools folder.
- Alternatively, you can select "WCH-LinkRV" in the software provided by WCH, such as MounRiver Studio or WCH-LinkUtility.
- Another option is to hold down the ModeS button on the device while plugging it into the USB port.

More information can be found in the [WCH-Link User Manual](http://www.wch-ic.com/downloads/WCH-LinkUserManual_PDF.html).

## Compiling and Uploading (Linux)
To use the WCH-LinkE on Linux, you need to grant access permissions beforehand by executing the following commands:
```
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="1a86", ATTR{idProduct}=="8010", MODE="666"' | sudo tee /etc/udev/rules.d/99-WCH-LinkE.rules
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="1a86", ATTR{idProduct}=="8012", MODE="666"' | sudo tee -a /etc/udev/rules.d/99-WCH-LinkE.rules
sudo udevadm control --reload-rules
```

Install the toolchain (GCC compiler, Python3, and PyUSB):
```
sudo apt install build-essential libnewlib-dev gcc-riscv64-unknown-elf
sudo apt install python3 python3-pip
python3 -m pip install pyusb
```

Switch off the FM Transmitter or remove the battery. Connect the FM Transmitter via the 3-pin PROG header to the WCH-LinkE programming device. Open a terminal and navigate to the folder with the makefile. Run the following command to compile and upload:
```
make flash
```

If you want to just upload the pre-compiled binary, run the following command instead:
```
python3 ./tools/rvprog.py -f transmitter.bin
```

## Uploading Firmware Binary (Windows/Mac)
WCH offers the free but closed-source software [WCH-LinkUtility](https://www.wch.cn/downloads/WCH-LinkUtility_ZIP.html) to upload the precompiled hex-file with Windows. Select the "WCH-LinkRV" mode in the software, open the transmitter.hex file and upload it to the microcontroller.

Alternatively, there is a platform-independent open-source tool called minichlink developed by Charles Lohr (CNLohr), which can be found [here](https://github.com/cnlohr/ch32v003fun/tree/master/minichlink). It can be used with Windows, Linux and Mac.

If you have installed [Python3](https://www.pythontutorial.net/getting-started/install-python/) and [pyusb](https://github.com/pyusb/pyusb) on your system, you can also use the included Python tool rvprog.py:
```
python3 ./tools/rvprog.py -f transmitter.bin
```

# Operating Instructions
1. Make sure that the wire antenna is laid as straight as possible horizontally or vertically.
2. Turn on the transmitter using the power switch.
3. Use the OK key to switch between transmitter frequency and audio gain display/control mode.
4. Use the UP or DOWN key to increase/decrease frequency/gain.
5. If the battery is weak, recharge it via the USB-C port.

![FM_Transmitter_pic7.jpg](https://raw.githubusercontent.com/wagiminator/CH32V003-FM-Transmitter/main/documentation/FM_Transmitter_pic7.jpg)

If you don't have an FM radio receiver and/or you want to build one yourself, take a look [here](https://github.com/wagiminator/ATtiny412-PocketRadio).

# References, Links and Notes
- [EasyEDA Design Files](https://oshwlab.com/wagiminator)
- [CNLohr: ch32003fun](https://github.com/cnlohr/ch32v003fun)
- [WCH: CH32V003 datasheets](http://www.wch-ic.com/products/CH32V003.html)
- [WCH: WCH-Link user manual](http://www.wch-ic.com/downloads/WCH-LinkUserManual_PDF.html)
- [WCH: Official Store on AliExpress](https://wchofficialstore.aliexpress.com)
- [KTMicro: KT0803K datasheet](https://www.elechouse.com/elechouse/images/product/FM%20Transmitter%20Module/KT0803K.pdf)
- [KTMicro: KT0803L datasheet](http://radio-z.ucoz.lv/kt_0803/KT0803L_V1.3.pdf)
- [Microne: ME6209 datasheet](https://datasheet.lcsc.com/lcsc/1811081822_MICRONE-Nanjing-Micro-One-Elec-ME6209A33M3G_C83508.pdf)
- [TP4054 datasheet](https://media.digikey.com/pdf/Data%20Sheets/UTD%20Semi%20PDFs/TP4054.pdf)
- [SSD1306 datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [128x32 OLED on Aliexpress](http://aliexpress.com/wholesale?SearchText=128+32+0.91+oled)
- [FM Pocket Radio](https://github.com/wagiminator/ATtiny412-PocketRadio)

![FM_Transmitter_pic2.jpg](https://raw.githubusercontent.com/wagiminator/CH32V003-FM-Transmitter/main/documentation/FM_Transmitter_pic2.jpg)

# License

![license.png](https://i.creativecommons.org/l/by-sa/3.0/88x31.png)

This work is licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License. 
(http://creativecommons.org/licenses/by-sa/3.0/)
