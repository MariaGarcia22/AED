#include <Arduino.h>

#include <Arduino.h>
#include <Servo.h>

// 🔧 Declaración de la función (SOLUCION AL ERROR)
void procesarComando(char* cmd);

const int PIN_LED   = 9;
const int PIN_SERVO = 10;
const int PIN_POT   = A0;

Servo miServo;

bool modoAuto = true;
char bufferCmd[32];
byte idx = 0;

void setup() {
    Serial.begin(115200);
    pinMode(PIN_LED, OUTPUT);

    miServo.attach(PIN_SERVO);
    miServo.write(90);

    Serial.println("Lab 07 - PWM + Servo + ADC");
    Serial.println("Comandos: D:0-255 | S:0-180 | AUTO | POT");
}

void loop() {

    // 📥 Leer comandos seriales
    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\n') {
            bufferCmd[idx] = '\0';
            procesarComando(bufferCmd);
            idx = 0;
        } 
        else if (c != '\r' && idx < 31) {
            bufferCmd[idx++] = c;
        }
    }

    // 🔄 MODO AUTOMÁTICO
    if (modoAuto) {
        static int val = 0;
        static int dir = 1;
        static unsigned long ultimo = 0;

        if (millis() - ultimo > 10) {
            ultimo = millis();

            val += dir * 2;

            if (val >= 255) { val = 255; dir = -1; }
            if (val <= 0)   { val = 0;   dir =  1; }

            analogWrite(PIN_LED, val);

            int grados = map(val, 0, 255, 0, 180);
            miServo.write(grados);
        }
    } 
    
    // 🎚️ MODO POTENCIÓMETRO
    else {
        int lecturaPot = analogRead(PIN_POT);

        int grados = map(lecturaPot, 0, 1023, 0, 180);
        int brillo = map(lecturaPot, 0, 1023, 0, 255);

        miServo.write(grados);
        analogWrite(PIN_LED, brillo);

        static unsigned long ultimoReport = 0;

        if (millis() - ultimoReport > 500) {
            ultimoReport = millis();

            float voltaje = lecturaPot * (5.0 / 1023.0);

            Serial.print("ADC=");
            Serial.print(lecturaPot);
            Serial.print(" V=");
            Serial.print(voltaje, 2);
            Serial.print(" Servo=");
            Serial.print(grados);
            Serial.print(" deg LED=");
            Serial.println(brillo);
        }
    }
}

// 🧠 PROCESAR COMANDOS
void procesarComando(char* cmd) {

    if (strcmp(cmd, "AUTO") == 0) {
        modoAuto = true;
        Serial.println("[OK] Modo automatico");
    }

    else if (strcmp(cmd, "POT") == 0) {
        modoAuto = false;
        Serial.println("[OK] Modo potenciometro");
    }

    else if (strncmp(cmd, "D:", 2) == 0) {
        int v = atoi(cmd + 2);
        v = constrain(v, 0, 255);

        analogWrite(PIN_LED, v);
        modoAuto = false;

        Serial.print("[OK] LED duty=");
        Serial.println(v);
    }

    else if (strncmp(cmd, "S:", 2) == 0) {
        int g = atoi(cmd + 2);
        g = constrain(g, 0, 180);

        miServo.write(g);
        modoAuto = false;

        Serial.print("[OK] Servo=");
        Serial.print(g);
        Serial.println(" grados");
    }

    else {
        Serial.println("[ERROR] Comando no valido");
    }
}