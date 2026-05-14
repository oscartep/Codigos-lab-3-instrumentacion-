/*
 * PRÁCTICA DE INSTRUMENTACIÓN ELECTRÓNICA
 * Interfaz I2C - ESP32-C6
 * Ejercicio 2: Reloj digital + lectura de potenciómetro en pantalla OLED SSD1306
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Tamaño de la pantalla OLED
#define ANCHO_PANTALLA 128
#define ALTO_PANTALLA 64

// Pines I2C para ESP32-C6
#define PIN_SDA 6
#define PIN_SCL 7

// Pin analógico para el potenciómetro
#define PIN_POT 1

// Dirección I2C común para pantallas OLED SSD1306
#define DIRECCION_OLED 0x3C

// Valores de calibración del potenciómetro
#define ADC_MAX_OBSERVADO 3300
#define POT_MAX_OHM 5050

// Crear objeto para controlar la pantalla OLED
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);

// Variables del reloj digital
// Cambiar manualmente si se quiere empezar desde una hora especifica
int hora = 10;
int minuto = 45;
int segundo = 10;

// Variables para controlar el tiempo
unsigned long tiempoAnterior = 0;
const unsigned long intervalo = 1000;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Inicializar comunicación I2C con los pines definidos
  Wire.begin(PIN_SDA, PIN_SCL);

  // Inicializar la pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, DIRECCION_OLED)) {
    Serial.println("No se detecta la pantalla OLED SSD1306.");
    Serial.println("Revise las conexiones o pruebe cambiando la direccion I2C a 0x3D.");
    while (true);
  }

  display.clearDisplay();
  display.display();

  Serial.println("Sistema iniciado correctamente.");
  Serial.println("Reloj digital + lectura de potenciometro en pantalla OLED.");
}

void loop() {
  // Leer el valor del potenciómetro desde el ADC
  int valorPot = analogRead(PIN_POT);

  // Limitar el valor leído al rango observado experimentalmente
  valorPot = constrain(valorPot, 0, ADC_MAX_OBSERVADO);

  // Convertir el valor ADC a una resistencia aproximada en ohmios
  long resistenciaOhm = map(valorPot, 0, ADC_MAX_OBSERVADO, 0, POT_MAX_OHM);

  // Obtener el tiempo actual del programa
  unsigned long tiempoActual = millis();

  // Actualizar el reloj cada segundo
  if (tiempoActual - tiempoAnterior >= intervalo) {
    tiempoAnterior = tiempoActual;

    segundo++;

    if (segundo >= 60) {
      segundo = 0;
      minuto++;
    }

    if (minuto >= 60) {
      minuto = 0;
      hora++;
    }

    if (hora >= 24) {
      hora = 0;
    }

    // Mostrar datos también en el monitor serial
    Serial.print("Hora: ");

    if (hora < 10) Serial.print("0");
    Serial.print(hora);
    Serial.print(":");

    if (minuto < 10) Serial.print("0");
    Serial.print(minuto);
    Serial.print(":");

    if (segundo < 10) Serial.print("0");
    Serial.print(segundo);

    Serial.print(" | Potenciometro ADC: ");
    Serial.print(valorPot);

    Serial.print(" | Resistencia aprox: ");
    Serial.print(resistenciaOhm);
    Serial.println(" ohm");
  }

  // Mostrar datos en la pantalla OLED
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Mostrar reloj
  display.setTextSize(2);
  display.setCursor(12, 8);

  if (hora < 10) display.print("0");
  display.print(hora);
  display.print(":");

  if (minuto < 10) display.print("0");
  display.print(minuto);
  display.print(":");

  if (segundo < 10) display.print("0");
  display.print(segundo);

  // Mostrar valor ADC del potenciómetro
  display.setTextSize(1);
  display.setCursor(10, 38);
  display.print("POT ADC: ");
  display.print(valorPot);

  // Mostrar resistencia aproximada
  display.setCursor(10, 52);
  display.print("R: ");
  display.print(resistenciaOhm);
  display.print(" ohm");

  display.display();

  delay(100);
}