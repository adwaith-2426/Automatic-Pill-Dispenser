# Automatic Pill Dispenser

An ESP32-based automatic pill dispensing system designed to provide scheduled medication reminders and controlled pill dispensing. The system uses an RTC for accurate timekeeping, a stepper motor for compartment selection, a servo motor for dispensing, and a Telegram bot for remote alarm configuration and notifications.

## Features

* 9 individual pill compartments
* Up to 9 scheduled medication alarms
* DS3231 RTC for accurate timekeeping
* 28BYJ-48 stepper motor for compartment rotation
* Servo motor for opening the dispensing gate
* ESP32 as the main controller
* OLED display for user feedback
* Buzzer for medication reminders
* Physical push button for pill-taken confirmation
* Telegram bot for remote alarm configuration
* Wi-Fi connectivity through ESP32
* Sequential pill dispensing
* Telegram notification when a pill is dispensed

## System Overview

The system consists of an ESP32 controller connected to an RTC, stepper motor, servo motor, OLED display, buzzer, push button, and Wi-Fi-enabled Telegram bot.

The DS3231 keeps track of the current time. When a scheduled alarm is reached, the ESP32 controls the stepper motor to position the appropriate pill compartment. The servo motor then operates the dispensing gate.

The user is alerted using the buzzer and OLED display. The physical button can be pressed to confirm that the pill has been taken. A Telegram message is also sent to notify the user about the dispensing event.

## Hardware Components

| Component              | Purpose                      |
| ---------------------- | ---------------------------- |
| ESP32                  | Main controller              |
| DS3231 RTC             | Accurate timekeeping         |
| 28BYJ-48 Stepper Motor | Rotates pill container       |
| ULN2003 Driver         | Drives the stepper motor     |
| Servo Motor            | Opens/closes dispensing gate |
| SSD1306 OLED           | Displays system status       |
| Buzzer                 | Medication reminder          |
| Push Button            | Pill-taken confirmation      |
| Pill Container         | Stores scheduled doses       |

## Pin Configuration

| Component    | ESP32 Pin |
| ------------ | --------: |
| DS3231 SDA   |   GPIO 21 |
| DS3231 SCL   |   GPIO 22 |
| OLED SDA     |   GPIO 21 |
| OLED SCL     |   GPIO 22 |
| Stepper IN1  |   GPIO 16 |
| Stepper IN2  |   GPIO 18 |
| Stepper IN3  |   GPIO 17 |
| Stepper IN4  |   GPIO 19 |
| Servo Signal |   GPIO 13 |
| Buzzer       |   GPIO 25 |
| Push Button  |   GPIO 27 |

The OLED and DS3231 share the ESP32 I²C bus.

## Working Principle

### 1. System Initialization

When the system starts, the ESP32 initializes the RTC, OLED, stepper motor, servo motor, buzzer, and push button.

The ESP32 then connects to the configured Wi-Fi network.

### 2. Alarm Configuration

Medication times can be configured using the Telegram bot.

Example:

```text
/setalarm 08:00
/setalarm 14:00
/setalarm 20:00
```

The system can store up to nine alarm times.

### 3. Pill Compartment Selection

The pill container consists of nine compartments.

The stepper motor rotates the container to position the required compartment at the dispensing location.

### 4. Pill Dispensing

After the correct compartment reaches the dispensing position, the servo motor operates the dispensing gate.

The gate opens to allow the pill to exit and then closes.

### 5. User Alert

The OLED displays:

```text
Take Pills
```

The buzzer provides an audible reminder.

### 6. Pill Confirmation

The user presses the physical button after taking the pill.

The OLED then displays:

```text
Pill Taken
```

If the button is not pressed within the initial reminder period, the system displays:

```text
Pill Not Taken
```

and provides another reminder.

## Telegram Bot

The Telegram bot provides a simple interface for configuring medication alarms.

### Command

```text
/setalarm HH:MM
```

Example:

```text
/setalarm 08:30
```

The configured alarm is stored in the next available slot.

> **Security:** Wi-Fi credentials, Telegram bot tokens, and Chat IDs are not included in this repository. Replace the placeholder values in the source code with your own credentials when running the project locally.

## Software

### Development Environment

* Arduino IDE
* ESP32 Board Package
* C/C++

### Libraries

The project uses the following Arduino libraries:

* Wire
* RTClib
* Adafruit GFX Library
* Adafruit SSD1306
* Stepper
* WiFi
* WiFiClientSecure
* UniversalTelegramBot
* ESP32Servo

## Program Flow

```text
Start
  │
  ▼
Initialize ESP32 and peripherals
  │
  ▼
Connect to Wi-Fi
  │
  ▼
Read current time from DS3231
  │
  ▼
Check scheduled alarm
  │
  ├── No → Continue monitoring
  │
  └── Yes
       │
       ▼
Rotate pill container
       │
       ▼
Open dispensing gate
       │
       ▼
Dispense pill
       │
       ▼
Activate buzzer + OLED reminder
       │
       ▼
Wait for button confirmation
       │
       ├── Button pressed → "Pill Taken"
       │
       └── No button → "Pill Not Taken"
       │
       ▼
Send Telegram notification
       │
       ▼
Continue monitoring
```

## Repository Structure

```text
Automatic-Pill-Dispenser/
│
├── README.md
│
├── src/
│   └── Automatic_Pill_Dispenser_Code.ino
│
├── hardware/
│   ├── components_specifications.png
│   └── pin_diagram.png
│
├── images/
│   ├── project_layout.png
│   ├── algorithm.png
│   ├── project_top_view.png
│   └── wire_connections.png
│
├── documentation/
│   └── Project_Presentation.pdf
│
└── telegram/
    └── telegram_commands.md
```

## Installation

### 1. Install Arduino IDE

Install the Arduino IDE and configure the ESP32 board package.

### 2. Install Required Libraries

Install all libraries listed in the **Software** section through the Arduino IDE Library Manager.

### 3. Configure Credentials

Open:

```text
src/Automatic_Pill_Dispenser.ino
```

Replace the placeholder values with your own Wi-Fi and Telegram credentials.

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

#define BOT_TOKEN "YOUR_TELEGRAM_BOT_TOKEN"
#define CHAT_ID "YOUR_CHAT_ID"
```

**Never commit real credentials to GitHub.**

### 4. Connect the Hardware

Connect the components according to the pin configuration provided in the `hardware` folder.

### 5. Upload the Program

Select the appropriate ESP32 board and COM port in Arduino IDE and upload the program.

## Testing

The following modules can be tested individually before complete system integration:

* RTC time reading
* OLED display
* Stepper motor rotation
* Servo motor operation
* Buzzer
* Push button
* Wi-Fi connection
* Telegram communication
* Complete dispensing sequence

## Future Improvements

Possible improvements include:

* Mobile application interface
* Multiple medication types
* Quantity monitoring
* Low-pill detection
* Medication history
* Cloud-based monitoring
* Rechargeable battery operation
* Improved mechanical dispensing mechanism
* Emergency notification to a caregiver
* Sensor-based pill detection

## Academic Project

This project was developed as an academic engineering project to demonstrate the integration of:

* Embedded systems
* IoT
* Motor control
* Real-time clock systems
* Wireless communication
* Human-machine interaction

## Author

**Adwaith C B**

B.Tech Electronics & Communication Engineering
Government Engineering College Palakkad

## License

This project is intended for educational and academic purposes.
