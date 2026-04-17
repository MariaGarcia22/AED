#include <Arduino.h>

// ─── Lectura BMP280 con reporte estructurado ─── 
#include <Wire.h> 
#include <Adafruit_BMP280.h> 
 
Adafruit_BMP280 bmp; 
const float PRESION_MAR = 1013.25;  // hPa al nivel del mar 
unsigned long ultimaLectura = 0;

const long INTERVALO = 3000;  // leer cada 3 segundos 
int numerolectura = 0; 
 
void setup() { 
    Serial.begin(115200); 
    Serial.println("Lab 06 - Sensor BMP280 via I2C"); 
    if (!bmp.begin(0x76)) {  // probar 0x77 si falla 
        Serial.println("ERROR: BMP280 no encontrado. Verificar conexion y direccion I2C."); 
        while (1) delay(100);  // detener ejecucion 
    } 
    // Configuracion: modo normal, oversampling x2 
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, 
                    Adafruit_BMP280::SAMPLING_X2, 
                    Adafruit_BMP280::SAMPLING_X16, 
                    Adafruit_BMP280::FILTER_X16, 
                    Adafruit_BMP280::STANDBY_MS_500); 
    Serial.println("BMP280 inicializado correctamente."); 
    Serial.println("N | Temp(C) | Presion(hPa) | Altitud(m)"); 
    Serial.println("--|---------|-------------|----------"); 
} 
 
void loop() { 
    if (millis() - ultimaLectura >= INTERVALO) { 
        ultimaLectura = millis(); 
        numerolectura++; 
        float temp   = bmp.readTemperature(); 
        float presion = bmp.readPressure() / 100.0F;  // Pa -> hPa 
        float altitud = bmp.readAltitude(PRESION_MAR); 
        Serial.print(numerolectura); Serial.print(" | "); 
        Serial.print(temp, 1); Serial.print(" C   | "); 
        Serial.print(presion, 2); Serial.print(" hPa | "); 
        Serial.print(altitud, 1); Serial.println(" m"); 
    } 
} 
 