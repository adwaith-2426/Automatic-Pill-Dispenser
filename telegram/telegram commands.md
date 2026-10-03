# Telegram Bot Commands

The Automatic Pill Dispenser uses a Telegram bot to configure medication alarm times and provide dispensing notifications.

## Available Command

### `/setalarm HH:MM`

Sets a medication alarm at the specified time.

**Format:**

```text
/setalarm HH:MM
```

**Example:**

```text
/setalarm 08:00
```

The alarm is stored in the next available alarm slot.

## Setting Multiple Alarms

The system supports up to **9 alarm slots**.

For example:

```text
/setalarm 08:00
/setalarm 12:30
/setalarm 14:00
/setalarm 16:30
/setalarm 18:00
/setalarm 20:00
```

Each command adds the specified time to the next available slot.

## Bot Responses

When an alarm is successfully stored, the bot sends a confirmation message indicating the alarm slot.

Example:

```text
Alarm stored at slot 0
```

If all nine alarm slots have already been configured, the bot responds:

```text
All alarms already set!
```

If the command format is incorrect, the bot responds:

```text
Use: /setalarm HH:MM
```

## Medication Notification

When a scheduled medication time is reached and the pill is dispensed, the system sends a Telegram notification.

Example:

```text
Take your Pills! 08:00
```

The displayed time corresponds to the time obtained from the DS3231 RTC.

## Important Notes

* Alarm times use the **24-hour format**.
* Up to **9 alarms** can be configured.
* The alarm is stored in the next available slot.
* The Telegram bot only accepts commands from the configured Chat ID.
* The bot token and Chat ID are private credentials and must not be published in the repository.
* This documentation does not contain any authentication credentials.

## Example Usage

A typical setup could be:

```text
/setalarm 08:00
```

Bot:

```text
Alarm stored at slot 0
```

Then:

```text
/setalarm 14:00
```

Bot:

```text
Alarm stored at slot 1
```

At 08:00, the dispenser rotates to the required compartment, dispenses the pill, activates the user alert, and sends a Telegram notification.
