#include <Arduino.h>

#define POT_PIN 34
#define LED_PIN 14

QueueHandle_t Cola_Lecturas;



void Lectura_Potenciometro(void *parametro) {
  for (;;) {
    int lectura = analogRead(POT_PIN);
    xQueueSend(Cola_Lecturas, &lectura, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}
void Procesamiento_Lecturas(void *parametro) {
      int lectura;
  for (;;) {
    if (xQueueReceive(Cola_Lecturas, &lectura, portMAX_DELAY) == pdTRUE) {
      Serial.printf("Lectura del potenciómetro: %d\n", lectura);
    }
  }
}
void LED(void *parametro) {
  for (;;) {
    int lectura = analogRead(POT_PIN);
    int pwm = map(lectura, 0, 4095, 0, 255);
    analogWrite(LED_PIN, pwm);
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Cola_Lecturas = xQueueCreate(10, sizeof(int));
  pinMode(LED_PIN, OUTPUT);
  xTaskCreate(Lectura_Potenciometro, "Lectura_Potenciometro", 2048, NULL, 1, NULL);
  xTaskCreate(Procesamiento_Lecturas, "Procesamiento_Lecturas", 2048, NULL, 1, NULL);
  xTaskCreate(LED, "LED", 2048, NULL, 1, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}

