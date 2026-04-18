#include <Arduino.h>

// lab08_spi_tft_ili9341.ino (ESP32) 
#include <Adafruit_GFX.h> 
#include <Adafruit_ILI9341.h> 
#include <SPI.h> 
 
// Pines SPI para ESP32 
#define TFT_CS   5 
#define TFT_DC  21 
#define TFT_RST 22 
 
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST); 
 
// Colores custom RGB565 
#define NARANJA 0xFD20 
#define GRIS    0x8410 
 
float temperatura = 24.5;  // Reemplazar con lectura real del sensor 
float humedad = 58.3; 
int frameCount = 0; 


void dibujarPanel() { 
    // Barra superior 
   // tft.fillRect(0, 0, 320, 30, P.midBlue 0x2471);  // usar valor numerico 0x2471 
    tft.fillRect(0, 0, 320, 30, 0x2471); 
    tft.setTextColor(ILI9341_WHITE); tft.setTextSize(1); 
    tft.setCursor(10, 10); tft.print("ESTACION AMBIENTAL  |  ESP32 + ILI9341"); 
    // Marco temperatura 
    tft.drawRoundRect(10, 40, 140, 100, 8, ILI9341_CYAN); 
    tft.setTextColor(ILI9341_CYAN); tft.setTextSize(1); 
    tft.setCursor(20, 55); tft.print("TEMPERATURA"); 
    // Marco humedad 
    tft.drawRoundRect(165, 40, 140, 100, 8, ILI9341_GREEN); 
    tft.setTextColor(ILI9341_GREEN); tft.setTextSize(1); 
    tft.setCursor(175, 55); tft.print("HUMEDAD"); 
    // Grafica historica (area inferior) 
    tft.drawRect(10, 155, 300, 75, GRIS); 
    tft.setTextColor(GRIS); tft.setTextSize(1); 
    tft.setCursor(15, 160); tft.print("Historial temp."); 
} 

void actualizarDatos(float temp, float hum) { 
    // Actualizar temperatura (borrar area previa y redibujar) 
    tft.fillRect(15, 70, 125, 55, ILI9341_BLACK); 
    tft.setTextColor(ILI9341_WHITE); tft.setTextSize(3); 
    tft.setCursor(20, 78); 
    tft.print(temp, 1); tft.print(" C"); 
    // Actualizar humedad 
    tft.fillRect(170, 70, 125, 55, ILI9341_BLACK); 
    tft.setTextColor(ILI9341_WHITE); tft.setTextSize(3); 
    tft.setCursor(175, 78); 
    tft.print(hum, 1); tft.print(" %"); 
}




void setup() { 
    Serial.begin(115200); 
    tft.begin(); 
    tft.setRotation(1);  // landscape 
    tft.fillScreen(ILI9341_BLACK); 
    // Pantalla de bienvenida 
    tft.setTextColor(ILI9341_CYAN); 
    tft.setTextSize(2); 
    tft.setCursor(20, 20); 
    tft.println("Lab 08 - SPI TFT"); 
    tft.setTextColor(ILI9341_WHITE); 
    tft.setTextSize(1);
     tft.setCursor(20, 50); 
    tft.println("ILI9341 240x320 @ 40MHz SPI"); 
    delay(1500); 
    tft.fillScreen(ILI9341_BLACK);
    dibujarPanel() ;
} 
 

 
void loop() { 
    frameCount++; 
    // Simular variacion del sensor (reemplazar con lectura I2C real) 
    temperatura = 24.5 + sin(frameCount * 0.05) * 3.0; 
    humedad     = 58.0 + cos(frameCount * 0.03) * 5.0; 
    actualizarDatos(temperatura, humedad); 
    delay(500); 
}