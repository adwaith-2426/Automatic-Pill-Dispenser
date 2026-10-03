# Hardware Pin Configuration

This document describes the connections between the ESP32 and the peripherals used in the Automatic Pill Dispenser.

## ESP32 Pin Configuration

| Component       | Pin / Signal | ESP32 GPIO |
| --------------- | ------------ | ---------: |
| DS3231 RTC      | SDA          |    GPIO 21 |
| DS3231 RTC      | SCL          |    GPIO 22 |
| OLED SSD1306    | SDA          |    GPIO 21 |
| OLED SSD1306    | SCL          |    GPIO 22 |
| Stepper ULN2003 | IN1          |    GPIO 16 |
| Stepper ULN2003 | IN2          |    GPIO 18 |
| Stepper ULN2003 | IN3          |    GPIO 17 |
| Stepper ULN2003 | IN4          |    GPIO 19 |
| Servo Motor     | Signal       |    GPIO 13 |
| Buzzer          | Signal       |    GPIO 25 |
| Push Button     | Signal       |    GPIO 27 |

## I²C Devices

The DS3231 RTC and SSD1306 OLED communicate with the ESP32 using the I²C protocol.

| I²C Signal |   ESP32 |
| ---------- | ------: |
| SDA        | GPIO 21 |
| SCL        | GPIO 22 |

The OLED uses I²C address:

```text
0x3C
```

## Stepper Motor

The project uses a **28BYJ-48 stepper motor** with a **ULN2003 driver board**.

| ULN2003 Pin |   ESP32 |
| ----------- | ------: |
| IN1         | GPIO 16 |
| IN2         | GPIO 18 |
| IN3         | GPIO 17 |
| IN4         | GPIO 19 |

The motor is controlled using the Arduino `Stepper` library.

The current software configuration uses:

```cpp
#define STEPS 2048
```

and:

```cpp
Stepper stepper(STEPS, 16, 18, 17, 19);
```

## Servo Motor

The servo controls the dispensing gate.

| Servo  | Connection |
| ------ | ---------- |
| Signal | GPIO 13    |
| VCC    | 5 V supply |
| GND    | Common GND |

The servo should preferably be powered from an appropriate external 5 V supply when required by the motor's current demand.

The external supply ground must be connected to the ESP32 ground.

## Buzzer

| Buzzer |   ESP32 |
| ------ | ------: |
| Signal | GPIO 25 |
| GND    |     GND |

The buzzer is activated during medication reminders.

## Push Button

The push button is connected to:

```text
GPIO 27
```

The software uses the ESP32's internal pull-up resistor:

```cpp
pinMode(BUTTON, INPUT_PULLUP);
```

Therefore, the button is connected between GPIO 27 and GND.

When the button is pressed:

```text
GPIO 27 → LOW
```

## Power and Ground

All modules that communicate directly with the ESP32 should share a common ground.

The stepper motor and servo may require separate power supplies depending on the selected motors and operating conditions.

**Important:** Always connect the grounds of external motor supplies to the ESP32 GND when the devices share control signals.

## Hardware Summary

```text
                    ┌──────────────────────┐
                    │        ESP32         │
                    │                      │
        I²C ───────►│ GPIO 21/22           │◄──── DS3231 RTC
        I²C ───────►│ GPIO 21/22           │◄──── OLED
                    │                      │
        Stepper ───►│ GPIO 16,18,17,19     │──── ULN2003
                    │                      │
        Servo ─────►│ GPIO 13              │──── Servo
                    │                      │
        Buzzer ────►│ GPIO 25              │──── Buzzer
                    │                      │
        Button ────►│ GPIO 27              │◄──── Push Button
                    │                      │
                    └──────────────────────┘
```

## Notes

* GPIO numbers refer to the ESP32 GPIO numbering used in the Arduino program.
* The exact power requirements depend on the specific hardware modules used.
* Do not power motors directly from an ESP32 GPIO pin.
* Ensure a common ground between the ESP32 and externally powered motor circuits.
