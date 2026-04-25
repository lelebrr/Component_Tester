# Sondvolt Component Tester PRO (CYD Edition)

![Hero Image](https://raw.githubusercontent.com/lelebrr/SondVolt/main/docs/assets/hero.png)

A professional-grade, multi-functional component tester built for the **ESP32-2432S028R (Cheap Yellow Display)**. This project transforms a budget-friendly board into a powerful diagnostic tool for electronics hobbyists and professionals.

## 🚀 Features

- **Component Identification**: Automatic detection of resistors, capacitors, diodes, transistors (NPN/PNP), inductors, and more.
- **Digital Multimeter**: Precision AC/DC voltage measurement with real-time waveform display (via ZMPT101B).
- **Auto-Calibration**: Self-adjusting probe offsets for high accuracy in low-resistance and capacitance measurements.
- **History Logging**: Automatically saves the last 100 measurements to the MicroSD card in CSV format.
- **Rich User Interface**:
  - Animated Splash Screen.
  - Intuitive Touch Navigation.
  - Auto-Backlight Dimming (45s timeout for energy saving).
  - Friendly Error System.
- **Audio Feedback**: Professional sound tones for success, error, and measurement status.

## 🛠 Hardware Configuration (CYD Pinout)

| Peripheral | Pin | Description |
| :--- | :--- | :--- |
| **TFT CS** | 15 | Display Chip Select |
| **TFT DC** | 2 | Data/Command |
| **TFT BL** | 21 | Backlight (PWM) |
| **Touch CS** | 33 | XPT2046 Chip Select |
| **SD CS** | 5 | MicroSD Chip Select |
| **Speaker** | 26 | Audio Output (DAC1) |
| **Probe 1** | 34 | Analog Measurement (ADC1_6) |
| **Probe 2** | 27 | Measurement / I2C SCL |
| **DS18B20** | 22 | Thermal Probe (OneWire) |

## 📖 How to Use

1. **Startup**: Power on the device. Wait for the splash animation to finish.
2. **Calibration**: Go to `Menu > Calibration`. Short-circuit the probes when asked and follow on-screen instructions.
3. **Measurement**: Connect any component to Probe 1 and Probe 2. The system will auto-detect and display the type and value.
4. **History**: View past measurements in `Menu > History`. These are synced with the `measurements.csv` on your SD card.
5. **Settings**: Adjust brightness and sound preferences in the `Settings` menu.

## ⚠️ Safety Information

> [!WARNING]
> - **NEVER** measure components in a live circuit. Discharge capacitors before testing.
> - Maximum input voltage on Probe 1/2 is **3.3V**. For higher voltages, use the specific Multimeter input (ZMPT101B module required for AC).
> - Incorrect wiring may damage the ESP32 chip.

## 🔧 Installation

1. Open this project in **VS Code with PlatformIO**.
2. Connect your CYD board via USB.
3. Click **Upload and Monitor**.
4. Format your MicroSD card to FAT32 and ensure it's inserted before booting.

---
Built with ❤️ by **Eletrônica DIY** (2026)