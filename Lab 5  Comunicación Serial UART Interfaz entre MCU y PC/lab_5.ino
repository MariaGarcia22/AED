// lab05_uart_comandos.ino
// Comunicación UART bidireccional con parser de comandos

const int LED_PIN = 13;
bool ledEstado = false;
String bufferCmd = "";

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    Serial.println("==============================");
    Serial.println("Sistema UART - Arduino/ATmega");
    Serial.println("Baud rate: 115200 | Formato: 8N1");
    Serial.println("Comandos: LED_ON, LED_OFF, STATUS, BLINK:N");
    Serial.println("==============================");
}

void loop() {
    while (Serial.available() > 0) {
        char c = Serial.read();

        if (c == '\n') {
            procesarComando(bufferCmd);
            bufferCmd = "";
        } 
        else if (c != '\r') {
            bufferCmd += c;
        }
    }
}

void procesarComando(String cmd) {
    cmd.trim();

    Serial.print("[CMD RECIBIDO]: '");
    Serial.print(cmd);
    Serial.println("'");

    if (cmd == "LED_ON") {
        ledEstado = true;
        digitalWrite(LED_PIN, HIGH);
        Serial.println("[OK] LED encendido");
    }
    else if (cmd == "LED_OFF") {
        ledEstado = false;
        digitalWrite(LED_PIN, LOW);
        Serial.println("[OK] LED apagado");
    }
    else if (cmd == "STATUS") {
        Serial.println("--- STATUS ---");
        Serial.print("LED: ");
        Serial.println(ledEstado ? "ON" : "OFF");

        Serial.print("Uptime: ");
        Serial.print(millis() / 1000);
        Serial.println(" s");

        Serial.print("Free SRAM aprox: ");
        Serial.print(freeRam());
        Serial.println(" bytes");
    }
    else if (cmd.startsWith("BLINK:")) {
        int veces = cmd.substring(6).toInt();

        for (int i = 0; i < veces; i++) {
            digitalWrite(LED_PIN, HIGH);
            delay(150);
            digitalWrite(LED_PIN, LOW);
            delay(150);
        }

        Serial.print("[OK] Blink x");
        Serial.println(veces);
    }
    else {
        Serial.print("[ERROR] Comando desconocido: '");
        Serial.print(cmd);
        Serial.println("'");
    }
}

int freeRam() {
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}