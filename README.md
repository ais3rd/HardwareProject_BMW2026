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
| Uno D3 (SoftwareSerial TX) | ESP32 GPIO34 (RX) **through a voltage divider** (see below) |
| OLED SDA | ESP32 GPIO23 |
| OLED SCL | ESP32 GPIO18 |
| GND | GND on all boards |

## Voltage divider (Uno TX → ESP32 RX)
The Uno's TX line outputs **5 V**, but the ESP32's GPIOs only take **3.3 V**, so connecting them directly can damage GPIO34. A two-resistor voltage divider lowers the signal to a safe level.

### Circuit
```
Uno D3 (TX, 5 V) ──[ R1 = 1 kΩ ]──┬──► ESP32 GPIO34 (RX, ~3.3 V)
                                  │
                              [ R2 = 2 kΩ ]
                                  │
                                 GND (shared by Uno and ESP32)
```

1. Connect **R1 (1 kΩ)** from Uno **D3** to a junction point.
2. Connect **R2 (2 kΩ)** from that junction to **GND**.
3. Connect the **junction** to ESP32 **GPIO34**.
4. Make sure the Uno and ESP32 **share a common GND**.

### Formula
```
Vout = Vin × R2 / (R1 + R2)
     = 5 V × 2 kΩ / (1 kΩ + 2 kΩ)
     = 3.33 V
```

### Other resistor pairs
Any pair with **R2 ≈ 2 × R1** works:

| R1 | R2 | Vout |
|---|---|---|
| 1 kΩ | 2 kΩ | 3.33 V |
| 10 kΩ | 20 kΩ | 3.33 V |
| 1 kΩ | 2.2 kΩ | 3.44 V |
| 4.7 kΩ | 10 kΩ | 3.40 V |

If you don't have a 2 kΩ resistor, use two 1 kΩ resistors in series. Keep the values in the 1–20 kΩ range. Much higher values make the edges slow and can corrupt data at higher baud rates.

> Only the Uno → ESP32 line needs a divider. Data flows one way here, so no ESP32 → Uno line is used. GPIO34 is input-only, which is fine for RX.

## Libraries
- **Uno:** `SoftwareSerial` (built in)
- **ESP32:** `Wire` (built in), plus [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) and [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) from the Library Manager

## Upload
1. Open `arduino-uno/ultrasonic_sensor.ino`, select **Arduino Uno**, and upload it.
2. Open `esp32/oled_display.ino`, select your ESP32 board (ESP32 core installed), and upload it.

## License
Released under the [MIT License](LICENSE).
