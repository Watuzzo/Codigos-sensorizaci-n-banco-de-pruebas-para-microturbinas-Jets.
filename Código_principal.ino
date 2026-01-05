#include "HX711.h"

#define DT 7
#define SCK 6

HX711 celda;

void setup() {
  Serial.begin(115200);   // Importante: debe coincidir con el baudrate del programa receptor
  Serial.println("Iniciando sistema de medición de empuje");

  celda.begin(DT, SCK);
  celda.set_scale(92.5);  // Usa aquí tu factor de escala calibrado
  celda.tare();           // Tara
}

void loop() {
  float gramos = celda.get_units(10); // promedio de 10 lecturas
  float newtons = gramos * 9.81 / 1000.0; // convierte gramos a N

  // Enviar datos con el formato que espera el sistema de adquisición
  Serial.print(newtons, 2);       // Empuje (2 decimales)
  Serial.print(",");             
  Serial.print(0.0);             // Temp ambiente por ahora = 0
  Serial.print(",");
  Serial.println(0.0);           // Temp tobera por ahora = 0

  delay(300); // cada ~300 ms
}




