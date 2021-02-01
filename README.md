# Sensirion UART SVM40 Arduino Library

This is the Sensirion SVM40 library for Arduino using the
modules UART interface.

# Installation

To install, download the latest release as .zip file and add it to your
[Arduino IDE](http://www.arduino.cc/en/main/software) via

	Sketch => Include Library => Add .ZIP Library...

Don't forget to **install the dependencies** listed below the same way via `Add
.ZIP Library`

Note: Installation via the Arduino Library Manager is coming soon.

# Dependencies

* [Sensirion Core](https://github.com/Sensirion/arduino-core)

# Quick Start

1. Connect the SVM40 Sensor to one of your Arduino board's
   UART buses. We recommend using `Serial1`. Check the pinout of your Arduino
   board to find the correct pins. The pinout of the SVM40
   Sensor board can be found in the data sheet.

	* **VDD** of the SEK-SVM40 to the **3.3V** of your Arduino board (5V works as well)
	* **GND** of the SEK-SVM40 to the **GND** of your Arduino board
	* **RX** of the SEK-SVM40 to the **UART1-TX** of your Arduino board
	* **TX** of the SEK-SVM40 to the **UART1-RX** of your Arduino board

2. Open the `exampleUsage` sample project within the Arduino IDE

		File => Examples => Sensirion UART SVM40 => exampleUsage

3. Depending on your Arduino board you may need to adapt the `#define
   SENSOR_SERIAL_INTERFACE` to choose the correct serial interface.

4. Click the `Upload` button in the Arduino IDE or

		Sketch => Upload

5. When the upload process has finished, open the `Serial Monitor` or `Serial
   Plotter` via the `Tools` menu to observe the measurement values. Note that
   the `Baud Rate` in the corresponding window has to be set to `115200 baud`.

# Contributing

**Contributions are welcome!**

We develop and test this driver using our company internal tools (version
control, continuous integration, code review etc.) and automatically
synchronize the master branch with GitHub. But this doesn't mean that we don't
respond to issues or don't accept pull requests on GitHub. In fact, you're very
welcome to open issues or create pull requests :)

This Sensirion library uses
[`clang-format`](https://releases.llvm.org/download.html) to standardize the
formatting of all our `.cpp` and `.h` files. Make sure your contributions are
formatted accordingly:

The `-i` flag will apply the format changes to the files listed.

```bash
clang-format -i src/*.cpp src/*.h
```

Note that differences from this formatting will result in a failed build until
they are fixed.

# License

See [LICENSE](LICENSE).
