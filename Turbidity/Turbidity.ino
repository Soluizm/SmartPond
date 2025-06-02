#define BLYNK_TEMPLATE_ID "TMPL6xsz-GaXN"
#define BLYNK_TEMPLATE_NAME "Smart Fish Monitoring"


#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial

#define USE_NODE_MCU_BOARD  // Ganti sesuai board
#include "BlynkEdgent.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RELAY1_PIN D5
#define RELAY2_PIN D6

LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

void sendSensor() {
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (3.3 / 1024.0);
  String status;

  if (sensorValue >= 800) {
    status = "Bersih";
  } else if (sensorValue >= 400) {
    status = "Keruh";
  } else {
    status = "Pekat";
  }

  Blynk.virtualWrite(V0, status);   // Status air
  Blynk.virtualWrite(V1, voltage);  // Tegangan

  lcd.setCursor(0, 0);
  lcd.print("Monitoring Air    ");
  lcd.setCursor(0, 1);
  lcd.print("Sts:");
  lcd.print(status);
  lcd.print(" V:");
  lcd.print(voltage, 1);
}

BLYNK_WRITE(V2) {
  digitalWrite(RELAY1_PIN, param.asInt());
}

BLYNK_WRITE(V3) {
  digitalWrite(RELAY2_PIN, param.asInt());
}

BLYNK_WRITE(V4) {
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (3.3 / 1024.0);
  String status;
  if (sensorValue >= 800) {
    status = "Bersih";
  } else if (sensorValue >= 400) {
    status = "Keruh";
  } else {
    status = "Pekat";
  }
  digitalWrite(RELAY2_PIN, param.asInt());
}

void setup()
{
  Serial.begin(115200);
  delay(100);

  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();

  pinMode(RELAY1_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, LOW);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY2_PIN, LOW);

  BlynkEdgent.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  BlynkEdgent.run();
  timer.run();
}