#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Servo.h>

Adafruit_BMP280 bmp;

// Pines
const int LED = 9;
const int SERVO_PIN = 10;

Servo servo;

// Variables
String cmd = "";
float temperatura = 0;
bool ledEstado = false;
int servoPos = 90;

unsigned long tiempo = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  servo.attach(SERVO_PIN);

  // Inicializar BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("Error BMP280");
    while (1);
  }

  Serial.println("Sistema iniciado (Arduino UNO)");
}

void loop() {

  // Leer temperatura cada 2 segundos
  if (millis() - tiempo > 2000) {
    tiempo = millis();

    temperatura = bmp.readTemperature();

    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" C");

    controlAutomatico();
  }

  // Leer comandos UART
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      procesar(cmd);
      cmd = "";
    } else if (c != '\r') {
      cmd += c;
    }
  }
}

// ---------------- FUNCIONES ----------------

void controlAutomatico() {
  if (temperatura > 25) {
    ledEstado = true;
    servoPos = 180;
  } else {
    ledEstado = false;
    servoPos = 90;
  }

  digitalWrite(LED, ledEstado);
  servo.write(servoPos);
}

void procesar(String c) {
  c.trim();

  if (c == "LED_ON") {
    ledEstado = true;
    Serial.println("[OK] LED encendido");
  }
  else if (c == "LED_OFF") {
    ledEstado = false;
    Serial.println("[OK] LED apagado");
  }
  else if (c == "STATUS") {
    Serial.println("--- STATUS ---");
    Serial.print("Temp: "); Serial.println(temperatura);
    Serial.print("LED: "); Serial.println(ledEstado ? "ON" : "OFF");
    Serial.print("Servo: "); Serial.println(servoPos);
  }
  else if (c.startsWith("SERVO:")) {
    servoPos = c.substring(6).toInt();
    servoPos = constrain(servoPos, 0, 180);
    Serial.print("[OK] Servo a ");
    Serial.println(servoPos);
  }
  else {
    Serial.println("[ERROR] Comando no reconocido");
  }

  digitalWrite(LED, ledEstado);
  servo.write(servoPos);
}