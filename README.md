# Smart Buck Intelligent DC-DC Converter
Buck topology voltage converter with power monitoring functionality using the TI INA219 current sense IC.

Source code + dependencies, PCB design and 3D model files for a buck topology printed circuit board to power (but not limited to) a dedicated astronomy camera from a higher voltage lithium-ion battery source. A key project design feature is power monitoring functionality using the Texas
Instruments INA219 Zero-Drift current/power monitor integrated circuit. The converter's output is 12 volts DC regulated up to 3 amperes of current achieved by utilizing the Texas Instruments LM2596 SIMPLE SWITCHER 3A Step-Down Voltage Regulator. When used to power a dedicated astrophotography camera with active CMOS sensor cooling, being able to monitor how much power the camera draws in relation to the fixed temperature setpoint as well as the environmental temperature is a useful ability.

The barrel jack style connectors used for the voltage input and output are standard 5.5x2.1mm size, which is very common amongst astronomy equipment and many other consumer electronics.

The embedded firmware for the converter runs on the Microchip SAMD21E17A ARM Cortex M0 microcontroller, and can relay power monitoring statistics to a connected computer over a serial communications port.

<img src="https://github.com/fbutkovich/Smart-Buck-Intelligent-DC-DC-Converter/blob/main/docs/assets/images/Smart Buck Converter Top Isometric.jpg" style="display: block; margin: auto;" />

<img src="https://github.com/fbutkovich/Smart-Buck-Intelligent-DC-DC-Converter/blob/main/docs/assets/images/Smart Buck Converter Bottom Isometric.jpg" style="display: block; margin: auto;" />

The input screw terminal block is intended to be connected to a power tool battery using an “power wheels adapter”, which is fundamentally a flying lead breakout for the voltage of common power tool battery packs (Dewalt, Milwaukee, Makita etc.) that allows replacement of the standard lead-acid battery that comes in most childrens motorized cars.

<img src="https://github.com/fbutkovich/Smart-Buck-Intelligent-DC-DC-Converter/blob/main/docs/assets/images/IMG_2854.JPG" style="display: block; margin: auto;" />

After the order and assembly of the first revision, as to be expected with any new electronic design there were several mistakes I ended up throubleshooting, thus leading to a few cuts and jumpers on the PCB itself. All of the corrections to the initial design were translated to the current revision of the schematic and layout and include:

* Ground plane disconnected from GND pins of U2, causing numerous headaches when attempting to flash bootloader onto IC and debug.
* Wrong pinout on U6, causing GND to be disconnected and frying IC upon first powerup. STCS05DR symbol and footprint were downloaded directly from SnapEDA, however it was later discovered they were incorrect for KiCad.
* SWCLK signal needed 1kOhm pullup resistor for the SWD interface to work properly.
* Pinout of reset button was incorrect, again this components (TL3342F160QG) symbol and footprint for KiCad were sourced from SnapEDA, however the symbol led to the ground being connected to pushbutton in the normally     closed position, causing the microcontroller to be in a constant state of reset.
* Footprint size and capacity of feed-forward capacitor required for the LM2596 adjustable buck regulator were incorrect.

<img src="https://github.com/fbutkovich/Smart-Buck-Intelligent-DC-DC-Converter/blob/main/docs/assets/images/IMG_2811.JPG" style="display: block; margin: auto;" />

<img src="https://github.com/fbutkovich/Smart-Buck-Intelligent-DC-DC-Converter/blob/main/docs/assets/images/IMG_2812.JPG" style="display: block; margin: auto;" />

The desired goal of this project is to create a portable power bank suited for applications requiring 12VDC, that is more compact and offers better performance monitoring and control than commercially available products that exist (such as the Jackery power station, Celestron PowerTank, Apertura Portable Telescope Power Supply) to name a few. Since this design is meant to accept a variable input DC voltage which can be obtained from common power tool batteries, of which there numerous manufacturers of, the versatility is endless. 

This is one of the first electronics projects of which I believe I have documented well enough, that I am confident in sharing with the community as an open source project. I would love to see further contributions to either the hardware design or software, and even ideas for added features and improved functionality. 

# Licence
MIT license
