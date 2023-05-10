# Smart Buck Intelligent DC-DC Converter
Buck topology voltage converter with power monitoring functionality using the TI INA219 current sense IC.

Source code + dependencies, PCB design and 3D model files for a buck topology printed circuit board to power (but not limited to) a dedicated astronomy camera from a higher voltage lithium-ion battery source. A key project design feature is power monitoring functionality using the Texas
Instruments INA219 Zero-Drift current/power monitor integrated circuit. The converter's output is 12 volts DC regulated up to 3 amperes of current achieved by utilizing the Texas Instruments LM2596 SIMPLE SWITCHER 3A Step-Down Voltage Regulator.

The barrel jack style connectors used for the voltage input and output are standard 5.5x2.1mm size, which is very common amongst astronomy equipment and many other consumer electronics. The input screw terminal block is intended to be connected to a power tool battery using an “hot-wheels adapter” which is fundamentally a flying lead breakout for the battery voltage. 

The embedded firmware for the converter runs on the Microchip SAMD21E17A ARM Cortex M0 microcontroller, and can relay power monitoring statistics to a connected computer over a serial communications port.

<img src="https://github.com/fbutkovich/Smart-Buck-Intelligent-DC-DC-Converter/blob/main/docs/assets/images/Smart Buck Converter Top Isometric.jpg" style="display: block; margin: auto;" />