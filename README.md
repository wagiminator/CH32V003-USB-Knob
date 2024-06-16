# CH32V003 USB Knob - Rotary Encoder with USB Interface
The USB Knob is a versatile device that combines the functionality of a rotary encoder with the convenience of a USB interface. It is based on the cheap CH32V003 32-bit RISC-V microcontroller. One of the standout features of the USB Knob is that it requires no driver installation, making it incredibly easy to use. The device acts as a USB human interface device (HID), meaning that it can be recognized and utilized by most computers without the need for additional software. This makes the USB Knob an ideal choice for a wide range of applications, including volume control for PCs.

![USB_Knob_pic1.jpg](https://raw.githubusercontent.com/wagiminator/CH32V003-USB-Knob/main/documentation/USB_Knob_pic1.jpg)

# Hardware
## Schematic
![USB_Knob_wiring.png](https://raw.githubusercontent.com/wagiminator/CH32V003-USB-Knob/main/documentation/USB_Knob_wiring.png)

## The CH32V003 Family of 32-bit RISC-V Microcontrollers
The CH32V003 series is a collection of industrial-grade general-purpose microcontrollers that utilize the QingKe RISC-V2A core design supporting the RV32EC instruction set. These microcontrollers are equipped with various features such as a 48MHz system main frequency, 16KB flash, 2KB SRAM, wide voltage support, a single-wire serial debug interface, low power consumption, and an ultra-small package. Additionally, the CH32V003 series includes a built-in set of components including a DMA controller, a 10-bit ADC, op-amp comparators, multiple timers, and standard communication interfaces such as USART, I2C, and SPI.

## Building Instructions
1. Take the Gerber files and send them to a PCB manufacturer of your choosing. They will use these files to create the circuit board for your device.
2. Once you have the PCB, you can start soldering the components onto it. Use the BOM (bill of materials) and schematic as a guide to make sure everything is connected correctly.
3. Upload the firmware by following the instructions in the next section (see below).
4. To create the case for your device, use the STL files with your 3D printer. Make sure to use transparent filament for the ring.
5. After printing, secure the PCB to the bottom of the case using four self-tapping M2x5mm screws.
6. Next, glue the ring from the bottom into the circular recess in the top of the case.
7. Finally, assemble the case. Place the knob onto the rotary encoder. Your device is now ready to use!

![USB_Knob_pic2.jpg](https://raw.githubusercontent.com/wagiminator/CH32V003-USB-Knob/main/documentation/USB_Knob_pic2.jpg)

# Software
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

To upload the firmware, you need to ensure that the USB Knob is disconnected from USB. Then, you should make the following connections to the WCH-LinkE:

```
WCH-LinkE      USB-Knob
+-------+      +------+
|  SWDIO| <--> |DIO   |
|    GND| ---> |GND   |
|    3V3| ---> |3V3   |
+-------+      +------+
```

If the blue LED on the WCH-LinkE remains illuminated once it is connected to the USB port, it means that the device is currently in ARM mode and must be switched to RISC-V mode initially. There are a few ways to accomplish this:
- You can utilize the Python tool rvprog.py (with -v option), which is provided in the software/tools folder.
- Alternatively, you can select "WCH-LinkRV" in the software provided by WCH, such as MounRiver Studio or WCH-LinkUtility.
- Another option is to hold down the ModeS button on the device while plugging it into the USB port.

More information can be found in the [WCH-Link User Manual](http://www.wch-ic.com/downloads/WCH-LinkUserManual_PDF.html).

## Compiling and Uploading Firmware (Linux)
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

Disconnect the USB cable from the USB Knob. Connect the USB-Knob via the 3-pin PROG header to the WCH-LinkE programming device. Open a terminal and navigate to the folder with the makefile. Run the following command to compile and upload:
```
make flash
```

If you want to just upload the pre-compiled binary, run the following command instead:
```
python3 ./tools/rvprog.py -f <firmware>.bin
```

## Uploading Firmware Binary (Windows/Mac)
WCH offers the free but closed-source software [WCH-LinkUtility](https://www.wch.cn/downloads/WCH-LinkUtility_ZIP.html) to upload the precompiled hex-file with Windows. Select the "WCH-LinkRV" mode in the software, open the .hex file and upload it to the microcontroller.

Alternatively, there is a platform-independent open-source tool called minichlink developed by Charles Lohr (CNLohr), which can be found [here](https://github.com/cnlohr/ch32v003fun/tree/master/minichlink). It can be used with Windows, Linux and Mac.

If you have installed [Python3](https://www.pythontutorial.net/getting-started/install-python/) and [pyusb](https://github.com/pyusb/pyusb) on your system, you can also use the included Python tool rvprog.py:
```
python3 ./tools/rvprog.py -f <firmware>.bin
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
