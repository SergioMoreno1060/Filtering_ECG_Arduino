#include <Arduino.h>

#define M 37  // Número de coeficientes del filtro
#define SAMPLE_RATE 44100  // Frecuencia de muestreo en Hz
#define SIGNAL_LENGTH 1000  // Longitud de la señal de prueba

// Coeficientes del filtro FIR pasabajo (frecuencia de corte 200 Hz)
const float h[M] = {-0.001, 0.001, 0.002, -0.0, -0.004, -0.004, 0.005, 0.011, -0.0, -0.018, -0.014, 
                    0.019, 0.039, -0.0, -0.067, -0.058, 0.091, 0.3, 0.4, 0.3, 0.091, -0.058, 
                    -0.067, -0.0, 0.039, 0.019, -0.014, -0.018, -0.0, 0.011, 0.005, -0.004, 
                    -0.004, -0.0, 0.002, 0.001, -0.001};

float x_buffer[M] = {0};  // Buffer circular para las entradas

// // Aproximación rápida de seno
// inline float fastSin(float x) {
//   const float B = 4/PI;
//   const float C = -4/(PI*PI);
//   float y = B * x + C * x * abs(x);
//   return y;
// }

// // Función inline para generar la señal de prueba
// inline float generateTestSignal(int index) {
//   // Usando aproximaciones rápidas de seno
//   return fastSin(2 * PI * 100 * index / SAMPLE_RATE) + 
//          0.5f * fastSin(2 * PI * 300 * index / SAMPLE_RATE);
// }

inline float generateTestSignal(int index) {
  // Usar la función sin() estándar
  return sin(2 * PI * 100 * index / SAMPLE_RATE) + 
         sin(2 * PI * 300 * index / SAMPLE_RATE);
}

// Función inline para aplicar el filtro FIR
inline float applyFilter(float input) {
  float output = 0;

  // Mover las muestras en el buffer
  for (int i = M - 1; i > 0; i--) {
    x_buffer[i] = x_buffer[i - 1];
  }

  x_buffer[0] = input;  // Insertar nueva muestra al inicio del buffer

  // Calcular la salida del filtro
  for (int i = 0; i < M; i++) {
    output += h[i] * x_buffer[i];
  }

  return output;
}

void setup() {
  Serial.begin(115200);  // Iniciar comunicación serial a 115200 baud
  Serial.println("Input,Output"); // Encabezados para el CSV
}

void loop() {
  static int index = 0;
  
  // Generar muestra, aplicar filtro
  float input = generateTestSignal(index);
  float output = applyFilter(input);
  
  // Enviar la señal de entrada y la señal filtrada en formato CSV
  Serial.print(input);    // Enviar la señal de entrada
  Serial.print(",");      // Separador CSV
  Serial.println(output); // Enviar la señal filtrada
  
  // Simular la frecuencia de muestreo con delay en microsegundos
  delayMicroseconds(1000000 / SAMPLE_RATE); // Controlar la tasa de muestreo

  // Incrementar el índice para generar la siguiente muestra
  if (++index >= SIGNAL_LENGTH) index = 0;
}
