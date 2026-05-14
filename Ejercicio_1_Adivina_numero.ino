/*
 * PRÁCTICA DE INSTRUMENTACIÓN ELECTRÓNICA
 * Interfaz UART - ESP32-C6
 * Ejercicio 1: Juego interactivo "Adivina el número"
 */

#include "esp_random.h"

// Definición de los estados del juego
const int PEDIR_MINIMO = 0;
const int PEDIR_MAXIMO = 1;
const int ADIVINANDO = 2;

// Variables principales del juego
int estadoActual = PEDIR_MINIMO;

long limiteInferior = 0;
long limiteSuperior = 0;
long numeroOculto = 0;

int intentos = 0;

// Función para limpiar datos sobrantes del monitor serial
void limpiarBufferSerial() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

// Función para iniciar una nueva partida
void iniciarPartida() {
  numeroOculto = random(limiteInferior, limiteSuperior + 1);
  intentos = 0;

  Serial.println();
  Serial.println("Partida iniciada.");
  Serial.print("Estoy pensando en un numero entre ");
  Serial.print(limiteInferior);
  Serial.print(" y ");
  Serial.println(limiteSuperior);
  Serial.println("Escribe tu primer intento:");
}

void setup() {
  // Inicialización de la comunicación serial
  Serial.begin(115200);
  delay(1000);

  // Semilla aleatoria recomendada para ESP32-C6
  randomSeed(esp_random());

  Serial.println();
  Serial.println("==================================");
  Serial.println("       JUEGO: ADIVINA EL NUMERO   ");
  Serial.println("==================================");
  Serial.println("Ingrese el valor minimo del rango:");
}

void loop() {
  // Solo se ejecuta cuando el usuario escribe algo en el monitor serial
  if (Serial.available() > 0) {

    long datoUsuario = Serial.parseInt();
    limpiarBufferSerial();

    switch (estadoActual) {

      case PEDIR_MINIMO:
        limiteInferior = datoUsuario;

        Serial.print("Valor minimo recibido: ");
        Serial.println(limiteInferior);

        Serial.println("Ahora ingrese el valor maximo del rango:");
        estadoActual = PEDIR_MAXIMO;
        break;

      case PEDIR_MAXIMO:
        limiteSuperior = datoUsuario;

        if (limiteSuperior <= limiteInferior) {
          Serial.println("Dato no valido.");
          Serial.println("El valor maximo debe ser mayor que el minimo.");
          Serial.println("Ingrese nuevamente el valor maximo:");
        } else {
          Serial.print("Valor maximo recibido: ");
          Serial.println(limiteSuperior);

          iniciarPartida();
          estadoActual = ADIVINANDO;
        }
        break;

      case ADIVINANDO:
        intentos++;

        Serial.print("Intento ");
        Serial.print(intentos);
        Serial.print(": ");
        Serial.println(datoUsuario);

        if (datoUsuario < limiteInferior || datoUsuario > limiteSuperior) {
          Serial.println("El numero ingresado esta fuera del rango definido.");
          Serial.println("Intenta nuevamente con un valor valido.");
        }
        else if (datoUsuario < numeroOculto) {
          Serial.println("El numero secreto es MAYOR.");
        }
        else if (datoUsuario > numeroOculto) {
          Serial.println("El numero secreto es MENOR.");
        }
        else {
          Serial.println();
          Serial.println("¡Correcto! Adivinaste el numero secreto.");
          Serial.print("Numero total de intentos: ");
          Serial.println(intentos);
          Serial.println("----------------------------------");
          Serial.println("Nueva partida.");
          Serial.println("Ingrese el nuevo valor minimo:");

          estadoActual = PEDIR_MINIMO;
        }
        break;
    }
  }
}