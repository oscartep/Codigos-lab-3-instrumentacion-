```cpp
/*
 * PRACTICA DE INSTRUMENTACION
 * ESP32-C6 + BMP280 SPI + OLED I2C
 */

#include <Wire.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// ======================================================
// OLED SSD1306 I2C
// ======================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 8
#define OLED_SCL 9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ======================================================
// BMP280 SPI
// ======================================================

// Pines SPI ESP32-C6
#define BMP_SCK   6
#define BMP_MISO  2
#define BMP_MOSI  7
#define BMP_CS    10

// Crear bus SPI
SPIClass spiBus(FSPI);

// Crear objeto BMP280
Adafruit_BMP280 bmp(BMP_CS, &spiBus);

// ======================================================

void setup() {

  Serial.begin(115200);

  delay(2000);

  Serial.println();
  Serial.println("=================================");
  Serial.println("ESP32-C6 + BMP280 SPI");
  Serial.println("=================================");

  // ======================================================
  // INICIALIZAR I2C OLED
  // ======================================================

  Wire.begin(OLED_SDA, OLED_SCL);

  // Inicializar pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    Serial.println("ERROR: OLED no encontrada");

    while (1);
  }

  display.clearDisplay();

  display.setTextSize(1);

  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);

  display.println("Iniciando...");

  display.display();

  // ======================================================
  // INICIALIZAR SPI BMP280
  // ======================================================

  spiBus.begin(BMP_SCK, BMP_MISO, BMP_MOSI, BMP_CS);

  // Inicializar BMP280
  if (!bmp.begin()) {

    Serial.println("ERROR: BMP280 no encontrado");
    Serial.println("Verifique conexiones SPI");

    display.clearDisplay();

    display.setCursor(0, 0);

    display.println("BMP280 ERROR");

    display.display();

    while (1);
  }

  Serial.println("BMP280 inicializado correctamente");

  // Configuracion del sensor
  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500
  );

  display.clearDisplay();

  display.setCursor(0, 0);

  display.println("BMP280 OK");

  display.display();

  delay(1500);
}

// ======================================================

void loop() {

  // Lectura de temperatura
  float temperatura = bmp.readTemperature();

  // Lectura de presion
  float presion = bmp.readPressure() / 100.0F;

  // Lectura de altitud
  float altitud = bmp.readAltitude(1013.25);

  // ======================================================
  // MONITOR SERIAL
  // ======================================================

  Serial.println("--------------------------------");

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  Serial.print("Presion: ");
  Serial.print(presion);
  Serial.println(" hPa");

  Serial.print("Altitud: ");
  Serial.print(altitud);
  Serial.println(" m");

  Serial.println("--------------------------------");

  // ======================================================
  // OLED
  // ======================================================

  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("BMP280 SPI");

  display.setCursor(0, 18);
  display.print("Temp: ");
  display.print(temperatura);
  display.println(" C");

  display.setCursor(0, 36);
  display.print("Pres:");
  display.print(presion);
  display.println(" hPa");

  display.setCursor(0, 54);
  display.print("Alt:");
  display.print(altitud);
  display.println(" m");

  display.display();

  delay(2000);
}
```
