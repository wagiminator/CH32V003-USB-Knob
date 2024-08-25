# CH32V003 USB Knob - Rotary Encoder with USB Interface
The USB Knob is a versatile device that combines the functionality of a rotary encoder with the convenience of a USB interface. It is based on the cheap CH32V003 32-bit RISC-V microcontroller. One of the standout features of the USB Knob is that it requires no driver installation, making it incredibly easy to use. The device acts as a USB human interface device (HID), meaning that it can be recognized and utilized by most computers without the need for additional software. This makes the USB Knob an ideal choice for a wide range of applications, including volume control for PCs.

![USB_Knob_pic1.jpg](https://raw.githubusercontent.com/wagiminator/CH32V003-USB-Knob/main/documentation/USB_Knob_pic1.jpg)

# Hardware
## Schematic
![USB_Knob_wiring.png](https://raw.githubusercontent.com/wagiminator/CH32V003-USB-Knob/main/documentation/USB_Knob_wiring.png)

## The CH32V003 Family of 32-bit RISC-V Microcontrollers
The CH32V003 series is a collection of industrial-grade general-purpose microcontrollers that utilize the QingKe RISC-V2A core design supporting the RV32EC instruction set. These microcontrollers are equipped with various features such as a 48MHz system main frequency, 16KB flash, 2KB SRAM, 2.7V - 5.5V supply voltage support, a single-wire serial debug interface, low power consumption, and an ultra-small package. Additionally, the CH32V003 series includes a built-in set of components including a DMA controller, a 10-bit ADC, op-amp comparators, multiple timers, and standard communication interfaces such as USART, I2C, and SPI.

## Building Instructions
1. Take the Gerber files (the *zip* file inside the *hardware* folder) and upload them to a PCB (printed circuit board) manufacturer of your choice (e.g., [JLCPCB](https://jlcpcb.com/)). They will use these files to create the circuit board for your device and send it to you.
2. Once you have the PCB, you can start soldering the components onto it. Use the BOM (bill of materials) and schematic as a guide to make sure everything is connected correctly. You can find the corresponding files in the *hardware* folder.
3. Upload the firmware by following the instructions in the next section (see below).
4. To create the case for your device, use the *stl* files in the *3dprint* folder with your 3D printer. Make sure to use transparent filament for the ring.
5. After printing, secure the PCB to the bottom of the case using four self-tapping M2x5mm screws.
6. Next, glue the ring from the bottom into the circular recess in the top of the case.
7. Finally, assemble the case. Place the knob onto the rotary encoder. Your device is now ready to use!

![USB_Knob_pic2.jpg](https://raw.githubusercontent.com/wagiminator/CH32V003-USB-Knob/main/documentation/USB_Knob_pic2.jpg)

# Software
## Software USB
Since the CH32V003 lacks a hardware USB peripheral, USB functionality is handled in software. Charles Lohr's excellent implementation, [RV003USB](https://github.com/cnlohr/rv003usb), is used for this. It emulates a USB low-speed device using pin-change interrupts and bit-banging, with assembly code for the low-level protocol and some C code for higher-level functionality.

## Firmware Versions
### Volume Knob
This firmware enables the USB Knob to control the volume of your computer. By turning the knob, you can increase or decrease the volume, while pressing the knob will mute or unmute the sound. The device is recognized as a USB HID consumer multimedia keyboard and should work immediately without the need for any driver installation.

### Mouse Wheel Knob
This firmware is designed to transform the USB Knob into a mouse wheel. By rotating the knob, it emulates the movement of a mouse wheel, allowing users to scroll up and down web pages, among other things. Pressing the knob is equivalent to clicking on the mouse wheel or the middle mouse button. The device registers as a USB HID mouse on the computer and should work immediately without the need for driver installation.

### Custom Knob
This firmware is perfect for customizing it according to your own preferences. It allows for the free mapping of USB HID actions to USB Knob events. The firmware is structured and commented in such a way that it should be possible to make adjustments even with basic programming skills. It is designed as a USB HID composite device that combines a keyboard, a mouse, and a game controller.

## Programming and Debugging Device
To program the CH32V003 microcontroller, you will need a special programming device which utilizes the proprietary single-wire serial debug interface (SDI). The [WCH-LinkE](http://www.wch-ic.com/products/WCH-Link.html) (pay attention to the "E" in the name) is a suitable device for this purpose and can be purchased commercially for around $4. This debugging tool is not only compatible with the CH32V003 but also with other WCH RISC-V and ARM-based microcontrollers.

![CH32V003_wch-linke.jpg](https://raw.githubusercontent.com/wagiminator/Development-Boards/main/CH32V003F4P6_DevBoard/documentation/CH32V003_wch-linke.jpg)

To use the WCH-LinkE on Linux, you need to grant access permissions beforehand by executing the following commands:
```
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="1a86", ATTR{idProduct}=="8010", MODE="666"' | sudo tee /etc/udev/rules.d/99-WCH-LinkE.rules
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="1a86", ATTR{idProduct}=="8012", MODE="666"' | sudo tee -a /etc/udev/rules.d/99-WCH-LinkE.rules
sudo udevadm control --reload-rules
```

On Windows, if you need to you can install the WinUSB driver over the WCH interface 1 using the [Zadig](https://zadig.akeo.ie/) tool.

To upload the firmware, you need to ensure that the USB Knob is disconnected from USB. Then, you should make the following connections to the WCH-LinkE:

```
WCH-LinkE      USB-Knob
+-------+      +------+
|  SWDIO| <--> |DIO   |
|    GND| ---> |GND   |
|    3V3| ---> |VCC   |
+-------+      +------+
```

If the blue LED on the WCH-LinkE remains illuminated once it is connected to the USB port, it means that the device is currently in ARM mode and must be switched to RISC-V mode initially. There are a few ways to accomplish this:
- You can utilize the Python command-line tool [rvprog](https://pypi.org/project/rvprog/) (with *-v* option).
- Alternatively, you can select "WCH-LinkRV" in the software provided by WCH, such as MounRiver Studio or WCH-LinkUtility.
- Another option is to hold down the ModeS button on the device while plugging it into the USB port.

More information can be found in the [WCH-Link User Manual](http://www.wch-ic.com/downloads/WCH-LinkUserManual_PDF.html).

## Compiling and Uploading Firmware using the Makefile
### Linux
Install the toolchain (GCC compiler, Python3, and rvprog):
```
sudo apt install build-essential libnewlib-dev gcc-riscv64-unknown-elf
sudo apt install python3 python3-pip
pip install rvprog
```

Disconnect the USB cable from the USB Knob. Connect the USB-Knob via the 3-pin PROG header to the WCH-LinkE programming device. Open a terminal and navigate to the folder with the *makefile*. Run the following command to compile and upload:
```
make flash
```

### Other Operating Systems
Follow the instructions on [CNLohr's ch32v003fun page](https://github.com/cnlohr/ch32v003fun/wiki/Installation) to set up the toolchain on your respective operating system (for Windows, use WSL). Also, install [Python3](https://www.pythontutorial.net/getting-started/install-python/) and [rvprog](https://pypi.org/project/rvprog/). Compile and upload with "make flash". Note that I only have Debian-based Linux and have not tested it on other operating systems.

## Uploading pre-compiled Firmware Binary
WCH offers the free but closed-source software [WCH-LinkUtility](https://www.wch.cn/downloads/WCH-LinkUtility_ZIP.html) to upload the precompiled hex-file with Windows. Select the "WCH-LinkRV" mode in the software, open the *<firmware>.hex* file in the *bin* folder and upload it to the microcontroller.

Alternatively, there is an open-source tool called [minichlink](https://github.com/cnlohr/ch32v003fun/tree/master/minichlink) developed by Charles Lohr (CNLohr). It can be used with Windows, Linux and Mac.

If you have installed [Python3](https://www.pythontutorial.net/getting-started/install-python/) on your system, you can also use the platform-independent open-source command-line tool [rvprog](https://pypi.org/project/rvprog/) for uploading:
```
rvprog -f bin/<firmware>.bin
```

# References, Links and Notes
- [EasyEDA Design Files](https://oshwlab.com/wagiminator)
- [CNLohr: rv003usb](https://github.com/cnlohr/rv003usb)
- [MCU Templates](https://github.com/wagiminator/MCU-Templates)
- [MCU Flash Tools](https://github.com/wagiminator/MCU-Flash-Tools)
- [CH32V003 Datasheets](http://www.wch-ic.com/products/CH32V003.html)
- [ATtiny85 TinyKnob](https://github.com/wagiminator/ATtiny85-TinyKnob)
- [CH32X033 USB Knob](https://github.com/wagiminator/CH32X033-USB-Knob)
- [CH552E USB Knob](https://github.com/wagiminator/CH552-USB-Knob)
- [CH552E MacroPad mini](https://github.com/wagiminator/CH552-Macropad-mini)
- [CH552G MacroPad plus](https://github.com/wagiminator/CH552-MacroPad-plus)

![USB_Knob_pic3.jpg](https://raw.githubusercontent.com/wagiminator/CH32V003-USB-Knob/main/documentation/USB_Knob_pic3.jpg)

# License
![license.png](https://i.creativecommons.org/l/by-sa/3.0/88x31.png)

This work is licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License. 
(http://creativecommons.org/licenses/by-sa/3.0/)
