#include <DHT.h>
#include <ArduinoJson.h>

#define BUTTON_K_PIN 14   // Pino do botão para Potássio (K)
#define BUTTON_P_PIN 27   // Pino do botão para Fósforo (P)
#define DHT_PIN 4         // Pino de dados do DHT22
#define DHT_TYPE DHT22    // Tipo de sensor DHT
#define LDR_PIN 34        // Pino ADC para o LDR
#define LED_PIN 18        // Pino para o LED
#define DEBOUNCE_DELAY 50 // Tempo de debounce em milissegundos

DHT dht(DHT_PIN, DHT_TYPE);

// Estados dos nutrientes no solo
bool isKPresent = false;
bool isPPresent = false;

// Estados anteriores dos botões
bool lastButtonKState = HIGH;
bool lastButtonPState = HIGH;

unsigned long lastDebounceTimeK = 0;
unsigned long lastDebounceTimeP = 0;

void setup() {
  Serial.begin(115200);

  // Configura os pinos dos botões como entrada com pull-up interno
  pinMode(BUTTON_K_PIN, INPUT_PULLUP);
  pinMode(BUTTON_P_PIN, INPUT_PULLUP);

  // Configura o pino do LED como saída
  pinMode(LED_PIN, OUTPUT);

  // Inicializa o sensor DHT22
  dht.begin();

  Serial.println("Sistema iniciado. Controle de Potássio (K), Fósforo (P), monitoramento de luz e umidade prontos.");
}

void loop() {
  // Verifica o estado do botão para Potássio (K)
  checkButton(BUTTON_K_PIN, isKPresent, lastButtonKState, lastDebounceTimeK, "K");

  // Verifica o estado do botão para Fósforo (P)
  checkButton(BUTTON_P_PIN, isPPresent, lastButtonPState, lastDebounceTimeP, "P");

  // Leitura do sensor DHT22
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Erro ao ler o sensor DHT22!");
  } else {
    // Leitura do LDR
    float lightLevel = readLightLevel();

    // Inferência do nível de água com base na umidade
    if (humidity < 30) {
      Serial.println("Nível de água: Baixo");
      digitalWrite(LED_PIN, HIGH);  // Acende o LED
      printJson(humidity, lightLevel, isKPresent, isPPresent);
    } else {
      Serial.println("Nível de água: Adequado");
      digitalWrite(LED_PIN, LOW);   // Apaga o LED
      printJson(humidity, lightLevel, isKPresent, isPPresent);
    }
  }

  delay(2000); // Aguarda 2 segundos antes da próxima leitura
}

// Função para verificar o estado dos botões e alternar o estado dos nutrientes
void checkButton(int buttonPin, bool &nutrientState, bool &lastButtonState, unsigned long &lastDebounceTime, const char* nutrientName) {
  bool reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading == LOW && lastButtonState == HIGH) {
      // Alterna o estado do nutriente
      nutrientState = !nutrientState;
      Serial.print("Presença de ");
      Serial.print(nutrientName);
      Serial.print(": ");
      Serial.println(nutrientState ? "Ativado" : "Desativado");
    }
  }

  lastButtonState = reading;
}

// Função para ler o nível de luminosidade
float readLightLevel() {
  int ldrValue = analogRead(LDR_PIN);
  float ldrPercentage = (ldrValue / 4095.0) * 100;
  Serial.print("Intensidade de Luz (LDR): ");
  Serial.print(ldrValue);
  Serial.print(" (");
  Serial.print(ldrPercentage);
  Serial.println("%)");
  return ldrPercentage;
}

// Função para exibir os dados em formato JSON no monitor serial
void printJson(float humidity, float lightLevel, bool potassiumPresent, bool phosphorusPresent) {
  DynamicJsonDocument doc(1024);

  // Preenche o documento JSON
  doc["humidity"] = humidity;
  doc["light_percentage"] = lightLevel;
  doc["potassium_present"] = potassiumPresent;
  doc["phosphorus_present"] = phosphorusPresent;

  // Exibe o JSON no monitor serial
  Serial.println("Dados em formato JSON:");
  serializeJsonPretty(doc, Serial);
  Serial.println();
}
