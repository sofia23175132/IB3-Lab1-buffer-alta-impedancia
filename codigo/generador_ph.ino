/*
 * Instrumentación Biomédica III — Guía de Laboratorio N.° 1
 * Generador de señal simulada de electrodo de pH
 * ESP32 — DAC interno de 8 bits en GPIO25
 *
 * Cumple los requisitos de la sección 5 de la guía:
 *  - Lee un valor de pH (0 a 14) desde el Monitor Serial.
 *  - Calcula el voltaje aplicando la ecuación de Nernst (sección 2.2),
 *    con un factor de escala (GANANCIA) para que la variación sea
 *    visible en el osciloscopio.
 *  - Limita el voltaje al rango válido del DAC (0 a 3.3 V).
 *  - Entrega el voltaje en GPIO25.
 *  - Muestra en el Monitor Serial el pH ingresado y el voltaje entregado.
 */

// El DAC interno esta en pines distintos segun el chip:
//  - ESP32 clasico (WROOM/DevKitC):  GPIO25 o GPIO26
//  - ESP32-S2:                       GPIO17 o GPIO18
// Esta directiva detecta el chip seleccionado en Herramientas > Placa
// y usa el pin correcto automaticamente.
#if CONFIG_IDF_TARGET_ESP32
  const int DAC_PIN = 25;
#elif CONFIG_IDF_TARGET_ESP32S2
  const int DAC_PIN = 17;
#else
  #error "La placa seleccionada no tiene DAC soportado por dacWrite(). Usa un ESP32 clasico o un ESP32-S2."
#endif

const float VCC       = 3.3;          // Voltaje máximo del DAC del ESP32
const float E0        = VCC / 2.0;    // Referencia en pH = 7 (punto medio del rango, 1.65 V)
const float PENDIENTE = 0.05916;      // V/pH, ecuación de Nernst a 25 °C
const float GANANCIA  = 5.0;          // Factor de escala para hacer visible la variación

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("=== Generador de senal simulada de pH ===");
  Serial.println("Ingrese un valor de pH (0 a 14) y presione Enter:");
}

void loop() {
  if (Serial.available() > 0) {
    float pH = Serial.parseFloat();

    // Vaciar el resto del buffer (salto de línea, etc.)
    while (Serial.available() > 0) {
      Serial.read();
    }

    if (pH < 0.0 || pH > 14.0) {
      Serial.println("Valor fuera de rango. Ingrese un pH entre 0 y 14.");
      return;
    }

    // Ecuacion de Nernst con factor de escala:
    // E = E0 - GANANCIA * PENDIENTE * (pH - 7)
    float voltaje = E0 - (GANANCIA * PENDIENTE * (pH - 7.0));

    // Limitar al rango valido del DAC (0 a 3.3 V)
    if (voltaje < 0.0)  voltaje = 0.0;
    if (voltaje > VCC)  voltaje = VCC;

    // Convertir a cuenta de 8 bits (0-255) para dacWrite()
    int cuentaDAC = (int)((voltaje / VCC) * 255.0);
    dacWrite(DAC_PIN, cuentaDAC);

    Serial.print("pH ingresado: ");
    Serial.print(pH, 2);
    Serial.print("  ->  Voltaje entregado (GPIO");
    Serial.print(DAC_PIN);
    Serial.print("): ");
    Serial.print(voltaje, 4);
    Serial.println(" V");
  }
}
