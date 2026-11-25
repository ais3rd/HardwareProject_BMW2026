# Hardware Project BMW 2026

A two-microcontroller distance meter. An **Arduino Uno** measures distance with an HC-SR04 ultrasonic sensor and sends it over **UART** to an **ESP32**, which shows it on a 0.96" SSD1306 OLED.

```
[HC-SR04] → Arduino Uno ──UART 9600 baud──► ESP32 → [0.96" OLED]
            ultrasonic_sensor.ino            oled_display.ino
```

## Arduino Uno (`arduino-uno/ultrasonic_sensor.ino`)
- Sends a 10 µs trigger pulse and measures the echo time with `pulseIn`
- Converts the echo time to centimeters:
  ```
  distance (cm) = duration (µs) × 0.0343 / 2
  ```
- Sends the value with one decimal place, one line at a time, through `SoftwareSerial` every 500 ms

## ESP32 (`esp32/oled_display.ino`)
- Receives each line on `Serial1` (RX = GPIO34) and converts it to a number
- Shows the distance in large text with a "cm" unit on a 128×64 SSD1306 OLED (I²C address `0x3C`)

## Wiring
| From | To |
|---|---|
| HC-SR04 TRIG | Uno D9 |
| HC-SR04 ECHO | Uno D10 |
| Uno D3 (SoftwareSerial TX) | ESP32 GPIO34 (RX) **through a voltage divider** (5 V → 3.3 V) |
| OLED SDA | ESP32 GPIO23 |
| OLED SCL | ESP32 GPIO18 |
| GND | GND on all boards |

> The Uno's TX line is 5 V and the ESP32's GPIOs are 3.3 V only. Use a resistor divider (e.g. 1 kΩ / 2 kΩ) or a level shifter.

## Libraries
- **Uno:** `SoftwareSerial` (built in)
- **ESP32:** `Wire` (built in), plus [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) and [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) from the Library Manager

## Upload
1. Open `arduino-uno/ultrasonic_sensor.ino`, select **Arduino Uno**, and upload it.
2. Open `esp32/oled_display.ino`, select your ESP32 board (ESP32 core installed), and upload it.

> `oled_display.ino` calls `unoSerial.begin(..., RX_PIN, TX_PIN)`, but the constants are declared as `rxPin` and `txPin`. Rename one or the other so they match before compiling.

## License
Released under the [MIT License](LICENSE).
