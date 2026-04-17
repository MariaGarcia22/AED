#include <Arduino.h>

// ─── I2C Scanner (cargar primero para verificar conexión) ─── 
#include <Wire.h> 
 
void setup() { 
    Wire.begin(); 
    Serial.begin(115200); 
    Serial.println("Escaneando bus I2C..."); 
    int deviceCount = 0; 
    for (byte addr = 1; addr < 127; addr++) { 
        Wire.beginTransmission(addr); 
        byte error = Wire.endTransmission(); 
        if (error == 0) { 
            Serial.print("Dispositivo en 0x"); 
            if (addr < 16) Serial.print("0"); 
            Serial.println(addr, HEX); 
            deviceCount++; 
        } 
    } 
    Serial.print("Total dispositivos encontrados: "); 
    Serial.println(deviceCount); 
} 
void loop() {} 
 