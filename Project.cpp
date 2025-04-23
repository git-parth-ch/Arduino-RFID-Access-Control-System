#include <SPI.h>        // Required for SPI communication
#include <MFRC522.h>    // Required for MFRC522 RFID module

// --- Pin Definitions ---
#define SS_PIN    10  // Slave Select pin for RFID module
#define RST_PIN   9   // Reset pin for RFID module
#define GREEN_LED 7   // Pin for the Green LED
#define RED_LED   6   // Pin for the Red LED

// --- RFID Instance ---
MFRC522 rfid(SS_PIN, RST_PIN); // Create MFRC522 instance

// --- Variables ---
unsigned long responseTimeout = 2000; // Max time to wait for Python response (milliseconds)
unsigned long ledDisplayTime = 2000;  // How long to keep LEDs on after response

void setup() {
  Serial.begin(9600); // Initialize serial communication
  while (!Serial);      // Wait for serial port to connect (needed for some Arduinos)

  SPI.begin();          // Initialize SPI bus
  rfid.PCD_Init();      // Initialize MFRC522 RFID reader

  Serial.println("Yo");

  pinMode(GREEN_LED, OUTPUT); // Set Green LED pin as output
  pinMode(RED_LED, OUTPUT);   // Set Red LED pin as output

  // Ensure LEDs are off initially
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void loop() {
  // Look for new RFID cards
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(50); // Small delay if no card is present
    return;    // Go back to the start of the loop
  }

  // --- Card Detected ---

  // Construct UID string
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      Serial.print("0"); // Add leading zero for single-digit hex values
      uidString += "0";
    }
    Serial.print(rfid.uid.uidByte[i], HEX);
    uidString += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) {
      Serial.print(" "); // Add space between bytes
      uidString += " ";
    }
  }
  Serial.println(); // Send newline to indicate end of UID transmission
  uidString.toUpperCase(); // Convert to uppercase for consistent comparison in Python

  // --- Wait for Response from Python ---
  Serial.flush(); // Ensure UID is sent before waiting for response
  unsigned long startTime = millis();
  String response = "";

  while (millis() - startTime < responseTimeout) {
    if (Serial.available() > 0) {
      response = Serial.readStringUntil('\n');
      response.trim(); // Remove any leading/trailing whitespace/newline
      break; // Exit loop once response is received
    }
  }

  // --- Process Response ---
  if (response == "GRANT") {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else if (response == "DENIED") {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  } else {
    for(int i=0; i<5; i++) {
        digitalWrite(RED_LED, HIGH); delay(100);
        digitalWrite(RED_LED, LOW); delay(100);
    }
  }

  // Keep LEDs on for a bit, then turn off
  delay(ledDisplayTime);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  // Halt the PICC (RFID card) and stop encryption
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(500); // Short delay before next scan attempt
}