# include <ArduinoJson.h>
const int TAMANHO = 50;
int PinTrigger = 2; // Pino usado para disparar os pulsos do sensor
int PinEcho = 3; // Pino usado para ler a saída do sensor
unsigned long TempoEcho = 0; // Tempo em microsegundos (deve ser unsigned long)
const float VelocidadeSom_mpors = 340.0; // em metros por segundo
const float VelocidadeSom_mporus = 0.000340; // em metros por microsegundo

// Declaração da função CalculaDistancia
float CalculaDistancia(unsigned long tempo);

void setup() {
  // Configura pino de Trigger como saída e inicializa com nível baixo
  pinMode(PinTrigger, OUTPUT);
  digitalWrite(PinTrigger, LOW);
  pinMode(PinEcho, INPUT); // Configura pino ECHO como entrada

  // Inicializa a porta serial
  Serial.begin(9600);
  delay(100);
}

void loop() {
  StaticJsonDocument<TAMANHO> json;
  // Envia pulso para disparar o sensor
  DisparaPulsoUltrassonico();

  // Mede o tempo de duração do sinal no pino de leitura (us)
  TempoEcho = pulseIn(PinEcho, HIGH);

  // Exibe a distância em metros e centímetros
  json["distancia_metros"] = CalculaDistancia(TempoEcho);
  json["distancia_centimetros"] = CalculaDistancia(TempoEcho) * 100;

  serializeJson(json, Serial);
  Serial.println();

  // Aguarda um pouco antes de começar novamente
  delay(2000);
}

// Função para enviar o pulso de trigger
void DisparaPulsoUltrassonico() {
  // Para fazer o HC-SR04 enviar um pulso ultrassônico, temos que enviar
  // para o pino de trigger um sinal de nível alto com pelo menos 10us de duração
  digitalWrite(PinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(PinTrigger, LOW);
}

// Função para calcular a distância com base no tempo de voo do ultrassom
float CalculaDistancia(unsigned long tempo) {
  // A fórmula para calcular a distância: d = (tempo * velocidade do som) / 2
  // Dividimos por 2 porque o som vai e volta, então o tempo total é o dobro da distância real.
  return (tempo * VelocidadeSom_mporus) / 2.0;
}