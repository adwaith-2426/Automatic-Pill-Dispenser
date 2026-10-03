//Libraries Used
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Stepper.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ESP32Servo.h>

const char* ssid = "YOUR_WiFi";
const char* password = "YOUR_WiFi_Password";
#define BOT_TOKEN "YOUR_BOT_TOKEN"
#define CHAT_ID "YOUR_CHAT ID"
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
unsigned long lastTelegramCheck = 0;
#define STEPS 2048
Stepper stepper(STEPS, 16, 18, 17, 19);
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Servo doorServo;
#define SERVO_PIN 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RTC_DS3231 rtc;
#define BUZZER 25
#define BUTTON 27
bool firstPill = true;
bool checkButton()
{
  if (digitalRead(BUTTON) == LOW)
  {
    delay(50); // debounce
    if (digitalRead(BUTTON) == LOW)
    {
      Serial.println("Button Pressed");
      return true;
    }
  }
  return false;
}

// Alarm times
int alarmHour[9] = {0};
int alarmMinute[9] = {0};
int totalAlarms = 9;
int alarmIndex = 0;
int lastAlarmIndex = -1;
int lastMinute = -1;
int currentSlot = 0;
void setup()
{
  Serial.begin(115200);
  Wire.begin(21, 22);
  stepper.setSpeed(10);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);   // ✅ button setup
  doorServo.setPeriodHertz(50);
  doorServo.attach(SERVO_PIN, 500, 2400);
  doorServo.write(90);
  doorServo.write(90);   // closed position
  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  Serial.println("System Ready");
  WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED)
{
  delay(500);
  Serial.print(".");
}

Serial.println("\nWiFi Connected");

client.setInsecure();
}

void loop()
{
  if (millis() - lastTelegramCheck > 2000)
{
  handleTelegram();
  lastTelegramCheck = millis();
}
  DateTime now = rtc.now();

  Serial.print("Time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

if (currentSlot < totalAlarms &&
    now.hour() == alarmHour[currentSlot] &&
    now.minute() == alarmMinute[currentSlot] &&
    !(lastMinute == now.minute()))
{
  Serial.println("Dispensing correct slot");

  dispensePill();
  alertUser();

  lastMinute = now.minute();
  currentSlot++;
}
  if (currentSlot >= totalAlarms)
{
  Serial.println("All pills done → Resetting");

  currentSlot = 0;
  alarmIndex = 0;
}
  delay(500);
}
// void returnToInitial()
// {
//   Serial.println("Returning to initial position...");

//   // Reverse full rotation (approx)
//   stepper.step(-2048);  

//   doorServo.write(90);

//   delay(1000);

//   Serial.println("Reset done");
// }
void operateServo()
{
  Serial.println("Opening door");

  doorServo.write(0);   // open
  delay(2000);           // allow pill to fall

  Serial.println("Closing door");

  doorServo.write(90);    // close
  delay(1000);
}
void dispensePill()
{
  static int stepCount = 0;

  if (firstPill)
  {
    Serial.println("First pill - no rotation");
    firstPill = false;
  }
  else
  {
    Serial.println("Rotating for next pill");

    if (stepCount % 9 == 0)
      stepper.step(224);
    else
      stepper.step(228);

    stepCount++;
    delay(500);  // settle
  }

  // ✅ Servo runs EVERY time
  operateServo();
   String message = " Take your Pills! ";

  DateTime now = rtc.now();
  message += String(now.hour()) + ":" + String(now.minute());

  bot.sendMessage(CHAT_ID, message, "");

}
void handleTelegram()
{
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  while (numNewMessages)
  {
    for (int i = 0; i < numNewMessages; i++)
    {
      String text = bot.messages[i].text;
      String chat_id = bot.messages[i].chat_id;

      if (chat_id != CHAT_ID) continue;

      if (text.startsWith("/setalarm"))
      {
        int h, m;

        if (sscanf(text.c_str(), "/setalarm %d:%d", &h, &m) == 2)
        {
          if (alarmIndex < totalAlarms)
          {
            alarmHour[alarmIndex] = h;
            alarmMinute[alarmIndex] = m;

            bot.sendMessage(chat_id, "Alarm stored at slot " + String(alarmIndex), "");

            alarmIndex++;
          }
          else
          {
            bot.sendMessage(chat_id, "All alarms already set!", "");
          }
        }
        else
        {
          bot.sendMessage(chat_id, "Use: /setalarm HH:MM", "");
        }
      }
    }

    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
}
// ---------------- ALERT FUNCTION ----------------
void alertUser()
 {
  bool pillTaken = false;

  // ---------------- FIRST ALERT ----------------
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("Take Pills");
  display.display();

  unsigned long start = millis();

  while (millis() - start < 5000)
  {
    digitalWrite(BUZZER, HIGH);
    delay(200);

    if (checkButton()) {
      pillTaken = true;
      break;
    }

    digitalWrite(BUZZER, LOW);
    delay(200);

    if (checkButton()) {
      pillTaken = true;
      break;
    }
  }

  digitalWrite(BUZZER, LOW);

  // ---------------- REPEAT UNTIL TAKEN ----------------
  while (!pillTaken)
  {
    // Wait 5 sec
    unsigned long waitStart = millis();
    while (millis() - waitStart < 5000)
    {
      if (checkButton()) {
        pillTaken = true;
        break;
      }
    }

    if (pillTaken) break;

    // Show "Pill Not Taken" + buzzer
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("Pill Not\nTaken");
    display.display();

    unsigned long alarmStart = millis();

    while (millis() - alarmStart < 5000)
    {
      digitalWrite(BUZZER, HIGH);
      delay(200);

      if (checkButton()) {
        pillTaken = true;
        break;
      }

      digitalWrite(BUZZER, LOW);
      delay(200);

      if (checkButton()) {
        pillTaken = true;
        break;
      }
    }

    digitalWrite(BUZZER, LOW);
  }

  // ---------------- FINAL STATE ----------------
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("Pill Taken");
  display.display();

  delay(3000);

  display.clearDisplay();
  display.display();
 }