// Include Libraries
#include <Wire.h>
#include <TinyGsmClient.h>

// TTGO T-Call Pin Definitions
#define MODEM_PWKEY          4
#define MODEM_RST            5
#define I2C_SDA              21
#define I2C_SCL              22
#define MODEM_POWER_ON       23
#define MODEM_RX             26
#define MODEM_TX             27

// BME280 Pin Definitions
#define I2C_SDA_2            18
#define I2C_SCL_2            19

// Serial Settings
#define SerialMon Serial
#define SerialAT Serial1

// TinyGSM Library Settings
#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER   1024

// Sleep Settings
#define uS_TO_S_FACTOR 1000000UL
#define TIME_TO_SLEEP  3600

// IP5306 Settings
#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

// Conditional Includes
#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

// Moisture Sensor Settings
float analogPin = 4;
float val = 0;
float mappedVal = 0;

// Valve Control Settings
int openVal = 50;
float openLength = 5.8;
int valve = 0;
float openLengthTotal = 0;

// GPRS Settings
const char apn[] = "three.co.uk";
const char gprsUser[] = "";
const char gprsPass[] = "";

// SIM Card PIN
const char simPIN[] = "";

// ThingSpeak Settings
const char server[] = "thingspeak.com";
const char resource[] = "/update";
const int port = 80;
String apiKeyValue = "<YOUR_API_KEY>";

// I2C for SIM800 (to keep it running when powered from battery)
TwoWire I2CPower = TwoWire(0);

// TinyGSM Client for Internet connection
TinyGsmClient client(modem);

bool setPowerBoostKeepOn(int en) {
  I2CPower.beginTransmission(IP5306_ADDR);
  I2CPower.write(IP5306_REG_SYS_CTL0);
  if (en) {
    I2CPower.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    I2CPower.write(0x35); // 0x37 is default reg value
  }
  return I2CPower.endTransmission() == 0;
}

void setup() {
  // Set serial monitor debugging window baud rate to 115200
  SerialMon.begin(115200);

  // Start I2C communication
  I2CPower.begin(I2C_SDA, I2C_SCL, 400000);

  // Keep power when running from battery
  bool isPowerBoostOk = setPowerBoostKeepOn(1);
    SerialMon.print(F("IP5306 KeepOn "));
    SerialMon.println(isPowerBoostOk ? F("OK") : F("FAIL"));

  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(9600, SERIAL_8N1, 27, 26); // RX, TX pins for ESP32 SIM800L module
  delay(3000);

  // Connect to the cellular network
  SerialMon.println("Initializing modem...");
  modem.gprsConnect(apn, user, password);

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3) {
    modem.simUnlock(simPIN);
  }

  pinMode(analogPin, INPUT); // Set pin 12 as an input
  pinMode(14, OUTPUT);       // Set pin 14 as output
}

void thingspeak() {
  SerialMon.print(F("Connecting to APN: "));
  SerialMon.print(apn);
  
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(F(" fail"));
  } else {
    SerialMon.println(F(" OK"));

    SerialMon.print(F("Connecting to "));
    SerialMon.print(server);
    
    if (!client.connect(server, port)) {
      SerialMon.println(F(" fail"));
    } else {
      SerialMon.println(F(" OK"));

      // Prepare your HTTP POST request data
      char httpRequestData[128];
      snprintf_P(httpRequestData, sizeof(httpRequestData),
                 PSTR("api_key=%s&field1=%d&field2=%d"),
                 apiKeyValue, mappedVal, Open_Length_Total);

      // Perform HTTP POST request
      SerialMon.println(F("Performing HTTP POST request..."));

      static const char PROGMEM httpRequestTemplate[] =
        "POST %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s";

      char httpRequest[256];
      snprintf_P(httpRequest, sizeof(httpRequest), httpRequestTemplate,
                 resource, server, strlen(httpRequestData), httpRequestData);

      client.write((const uint8_t *)httpRequest, strlen(httpRequest));

      unsigned long timeout = millis();
      while (client.connected() && millis() - timeout < 10000L) {
        // Print available data (HTTP response from server)
        while (client.available()) {
          char c = client.read();
          SerialMon.print(c);
          timeout = millis();
        }
      }
      SerialMon.println();

      // Close client and disconnect
      client.stop();
      SerialMon.println(F("Server disconnected"));
      modem.gprsDisconnect();
      SerialMon.println(F("GPRS disconnected"));
    }
  }
}



float moisture_sens(int analogPin) {
  int val = analogRead(analogPin);
  val = (1 / (val)) * 100000;

  float minimum = 26;
  int maximum = 700;
  int newmin = 0;
  int newmax = 100;
  float mappedVal = ((val - minimum) / maximum) * 100;

  return mappedVal;
}

int valve_control(float mappedVal, int openVal, int openLength, int controlPin) {
  static int openLengthTotal = 0;
  
  if (mappedVal <= openVal) {
    digitalWrite(controlPin, HIGH);
    delay(openLength * 1000);
    digitalWrite(controlPin, LOW);
    openLengthTotal += openLength;
  }

  return openLengthTotal;
}

void loop() {
  int analogPin = 12;
  float mappedVal = moisture_sens(analogPin);
  int openVal = 30;
  int openLength = 5;
  int controlPin = 14;
  int openLengthTotal = valve_control(mappedVal, openVal, openLength, controlPin);

  thingspeak();
  delay(15000);
}
