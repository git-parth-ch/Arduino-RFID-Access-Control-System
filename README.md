*🔐 Arduino RFID Access Control System*

An RFID-based access control system built using an Arduino-compatible board and the MFRC522 RFID module.
The system reads an RFID/NFC card UID, sends it to a connected Python application via serial communication, and waits for an authorization response (GRANT or DENIED) to control LED indicators.

**Features**

- Reads RFID/NFC card UID

- Sends UID to Python via Serial (USB)

- Waits for authentication response with timeout

- Green LED for access granted

- Red LED for access denied

- Error indication if no response received

- Automatic reset after each scan

**Hardware Requirements**

- Arduino board (Uno, Nano, Mega, etc.)

- MFRC522 RFID reader

- RFID/NFC cards or tags

- 1x Green LED

- 1x Red LED

- 2x 220Ω resistors (for LEDs)

- Jumper wires

- Breadboard

- USB cable

🔌 Wiring Connections
📍 MFRC522 → Arduino
MFRC522 Pin	Arduino Uno Pin
SDA (SS)	10
SCK	13
MOSI	11
MISO	12
RST	9
3.3V	3.3V
GND	GND

⚠️ Important: The MFRC522 works at 3.3V, not 5V.

💡 LEDs
Component	Arduino Pin
Green LED	7
Red LED	6

Each LED should have a 220Ω resistor in series.

**📦 Required Libraries**

Install the following libraries in the Arduino IDE:

SPI (built-in)

MFRC522 library by Miguel Balboa

To install:

- Open Arduino IDE

- Go to Sketch → Include Library → Manage Libraries

- Search for MFRC522

- Install the library

**How It Works**

The system waits for an RFID card.

When detected, it reads the UID.

The UID is sent over Serial (9600 baud) to a Python program.

The Arduino waits up to 2 seconds for a response:

"GRANT" → Green LED ON

"DENIED" → Red LED ON

No response → Red LED blinks 5 times

LEDs stay on for 2 seconds.

System resets and waits for next card.

📡 Serial Communication Protocol

Baud Rate: 9600

UID Format: Uppercase hex bytes separated by spaces
Example:

A1 B2 C3 D4


Expected Python Response:

GRANT


or

DENIED


⚠️ Response must end with a newline (\n).

⏱️ Configurable Parameters
unsigned long responseTimeout = 2000; // Max wait for Python response (ms)
unsigned long ledDisplayTime = 2000;  // LED on duration (ms)

**Use Cases**

- Door access control

- Attendance system

- Smart lockers

- Secure device login

- IoT authentication systems

**⚠️ Troubleshooting**

Problem: RFID not detected

Check wiring connections

Ensure 3.3V supply

Confirm correct SS and RST pins

Problem: No response from Python

Verify baud rate is 9600

Ensure Python script sends newline

Check correct COM port

Problem: Random characters in Serial Monitor

Make sure baud rate matches (9600)

**🔒 Security Note**

This project relies on UID-based authentication.
For higher security applications, consider:

- Encrypted communication

- Secure elements

- MIFARE sector authentication

- Backend database verification


