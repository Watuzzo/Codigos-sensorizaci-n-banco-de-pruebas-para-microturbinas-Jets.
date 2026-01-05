#include "HX711.h"

#define DT 6
#define SCK 7

HX711 celda;

void setup() {
  Serial.begin(9600);
  Serial.println("Calibración de celda de carga");

  celda.begin(DT, SCK);
  celda.set_scale();   // Factor de escala por defecto
  celda.tare();        // Poner en cero

  Serial.println("Coloca un peso conocido sobre la celda (esperando 10 segundos)");
  delay(10000);

  long lectura = celda.get_units(20); // promedio de 20 lecturas
  Serial.print("Lectura cruda promedio: ");
  Serial.println(lectura);

  Serial.println("Divide esta lectura entre el peso real en gramos para obtener el factor de escala");
}

void loop() {
  // vacío, solo se corre una vez
}

