# Smart Buck Intelligent DC-DC Converter
Buck topology voltage converter with power monitoring functionality using the TI INA219 current sense IC.

The provided package includes the source code, necessary dependencies, PCB design, and 3D model files for a buck topology printed circuit board. This PCB is designed to efficiently power various devices, including a dedicated astronomy camera, using a higher voltage lithium-ion battery as the input source. A notable feature of this project design is the inclusion of power monitoring functionality through the utilization of the Texas Instruments INA219 Zero-Drift current/power monitor integrated circuit. The converter's output is a regulated 12 volts DC, capable of delivering up to 3 amperes of current, achieved by employing the Texas Instruments LM2596 SIMPLE SWITCHER 3A Step-Down Voltage Regulator. This configuration proves particularly beneficial when powering a dedicated astrophotography camera with active CMOS sensor cooling. The ability to monitor the camera's power consumption in relation to the fixed temperature setpoint and the surrounding environmental temperature adds significant value to this setup.

The barrel jack style connectors used for the voltage input and output are standard 5.5x2.1mm size, which is very common amongst astronomy equipment and many other consumer electronics.

The embedded firmware for the converter runs on the Microchip SAMD21E17A ARM Cortex M0 microcontroller, and can relay power monitoring statistics to a connected computer over a serial communications port.

<img src="https://github.com/fbutkovich/Smart-Buck-Intelligent-DC-DC-Converter/blob/main/docs/assets/images/Smart Buck Converter Top Isometric Rev2.jpg" style="display: block; margin: auto;" />

<img src="https://github.com/fbutkovich/Smart-Buck-Intelligent-DC-DC-Converter/blob/main/docs/assets/images/Smart Buck Converter Bottom Isometric Rev2.jpg" style="display: block; margin: auto;" />

The input screw terminal block is intended to be connected to a power tool battery using an “power wheels adapter”, which is fundamentally a flying lead breakout for the voltage of common power tool battery packs (Dewalt, Milwaukee, Makita etc.) that allows replacement of the standard lead-acid battery that comes in most childrens motorized cars.

<img src="https://github.com/fbutkovich/Smart-Buck-Intelligent-DC-DC-Converter/blob/main/docs/assets/images/IMG_2854.JPG" style="display: block; margin: auto;" />

After the order and assembly of the first revision, as to be expected with any new electronic design there were several mistakes I ended up throubleshooting, thus leading to a few cuts and jumpers on the PCB itself. All of the corrections to the initial design were translated to the current revision of the schematic and layout and include:

* Ground plane disconnected from GND pins of U2, causing numerous headaches when attempting to flash bootloader onto IC and debug.
* Wrong pinout on U6, causing GND to be disconnected and frying IC upon first powerup. STCS05DR symbol and footprint were downloaded directly  from SnapEDA, however it was later discovered they were incorrect for KiCad.
* SWCLK signal needed 1kOhm pullup resistor for the SWD interface to work properly.
* Pinout of reset button was incorrect, again this components (TL3342F160QG) symbol and footprint for KiCad were sourced from SnapEDA, however the symbol led to the ground being connected to pushbutton in the normally     closed position, causing the microcontroller to be in a constant state of reset.
* Footprint size and capacity of feed-forward capacitor required for the LM2596 adjustable buck regulator were incorrect.

<img src="https://github.com/fbutkovich/Smart-Buck-Intelligent-DC-DC-Converter/blob/main/docs/assets/images/IMG_2811.JPG" style="display: block; margin: auto;" />

<img src="https://github.com/fbutkovich/Smart-Buck-Intelligent-DC-DC-Converter/blob/main/docs/assets/images/IMG_2812.JPG" style="display: block; margin: auto;" />

# Known Issues
* The onboard 3.3V regulator can be fed from dual voltage supplies, either the USB input, or the 18V battery input. Diodes D1 and D6 (PMEG3015EH) are placed on each voltage rail before tying together at the regulator VIN. After a few uses of the design, it was discovered that D1 had burnt up as the onboard microcontroller would no longer show up in windows as a COM port, and the 3.3V indicator LED would not light up, if an 18V battery was not also connected. The only reason I could think of on why D1 would malfunction would be a higher than rated reverse current due to such a high reverse voltage from the battery input (typically 16-22V) being imposed on the diode, the datasheet specs the reverse current anywhere between 400-1000uA when VR=30V, which is not being exceeded in this use case, however, this also strongly depends on the ambient temperature. I know for a fact that the inside of the enclosure the smart buck PCB is housed in gets considerablly warm when there is a large amount of power draw, which could be what is shortening the lifespan of the diodes. I haven't had time yet to find a suitible replacement diode that will be more robust.

# Project Goal
The primary objective of this project is to develop a compact and high-performance portable power bank that caters specifically to applications requiring 12VDC. The aim is to match the capabilities of commercially available alternatives like the Jackery power station, Celestron PowerTank, and Apertura Portable Telescope Power Supply. Not only does this design excel in size reduction, but it also offers superior performance monitoring and control. By accommodating a variable input DC voltage, which can be sourced from a wide range of commonly available power tool batteries from various manufacturers, the versatility of this power bank is limitless.

I believe the thorough documentation of this electronics project has instilled in me enough confidence to share it with the community as an open-source endeavor. I enthusiastically welcome further contributions to enhance both the hardware design and software implementation. Moreover, I'm open to ideas for additional features and improved functionality, as collaboration and innovation are key factors in driving this project forward.

# License
MIT license
