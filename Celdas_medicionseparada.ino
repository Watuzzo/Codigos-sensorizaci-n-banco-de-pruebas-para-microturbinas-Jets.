#include "HX711.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "max6675.h"

// --- Celdas de carga (HX711) ---
#define DT1 7
#define SCK1 6
#define DT2 4
#define SCK2 3

HX711 celda1;
HX711 celda2;

// --- BMP280 ---
Adafruit_BMP280 bmp;  // I2C (0x76)

// --- Termocupla K + MAX6675 ---
int thermoDO = 12;
int thermoCS = 10;
int thermoCLK = 13;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


// Variables ambiente
float TEMPERATURA;  // temperatura ambiente (BMP280)
float PRESION;      // presión atmosférica (hPa)

// Valor que usaremos cuando la TC no esté conectada o haya fallo
const float TC_FALLO = -127.0;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sistema de medicion (2 celdas + BMP280 + MAX6675)");

  // Celdas
  celda1.begin(DT1, SCK1);
  celda2.begin(DT2, SCK2);

  celda1.set_scale(92.5);   // Ajusta segun calibracion
  celda2.set_scale(-60);    // Ajusta segun calibracion (signo si está invertida)
  celda1.tare();
  celda2.tare();

  // BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 no encontrado!");
    while (1) { delay(10); }
  }

  // MAX6675 necesita ~200ms tras energizarse
  delay(300);
}

void loop() {
  // --- Lectura celdas (promediamos internamente 10 muestras) ---
  float gramos1 = celda1.get_units(10);
  float gramos2 = celda2.get_units(10);
  float gramos_totales = gramos1 + gramos2;
  float newtons = gramos_totales * 9.81f / 1000.0f;

  // --- Ambiente ---
  TEMPERATURA = bmp.readTemperature();     // °C
  PRESION     = bmp.readPressure() / 100.0f; // hPa (no se envía, pero queda disponible)

  // --- Termocupla K (temperatura de salida) ---
  // La librería retorna NAN si no hay termocupla o hay fallo
  double temp_salida = thermocouple.readCelsius();
  if (isnan(temp_salida)) {
    temp_salida = TC_FALLO; // marcador de error
  }

  // --- Salida CSV: empuje[N], temp_ambiente[°C], temp_salida[°C] ---
  Serial.print(gramos1 * 9.81f / 1000.0, 2);
  Serial.print(",");
  Serial.print(gramos2 * 9.81f / 1000.0, 2);
  Serial.print(",");
 

  // Si quieres ver diagnóstico cuando falle la TC, descomenta:
  // if (temp_salida == TC_FALLO) Serial.println("Aviso: termocupla no conectada/fallo");

  delay(100);
}
