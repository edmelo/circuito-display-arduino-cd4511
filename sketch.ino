// Programa: CD4511 + Potenciômetro + Push-button (Latch)
// Autor: Adaptado

// Definição dos pinos de saída para o CD4511
#define PinoA0 4
#define PinoA1 5
#define PinoA2 6
#define PinoA3 7

// Entradas
#define PotPin A0       // Potenciômetro
#define ButtonPin 2     // Push-button de trava

int numero = 0;              // Valor atual (0–9)
bool travado = false;        // Estado do latch
int ultimoPot = 0;           // Última leitura do potenciômetro
int tolerancia = 20;         // Sensibilidade para detectar movimento
bool ultimoEstadoBotao = HIGH;

void setup() {
  Serial.begin(9600);

  // Saídas para o CD4511
  pinMode(PinoA0, OUTPUT);
  pinMode(PinoA1, OUTPUT);
  pinMode(PinoA2, OUTPUT);
  pinMode(PinoA3, OUTPUT);

  // Entradas
  pinMode(PotPin, INPUT);
  pinMode(ButtonPin, INPUT_PULLUP); // Botão com resistor interno

  Serial.println("Iniciando contador com potenciometro e latch...");
  mostrarNumero(numero);
}

void loop() {
  // --- Leitura do botão (com latch) ---
  bool estadoBotao = digitalRead(ButtonPin);
  if (estadoBotao == LOW && ultimoEstadoBotao == HIGH) {
    travado = !travado;  // Alterna o estado
    Serial.print("Latch: ");
    Serial.println(travado ? "Travado" : "Liberado");
    delay(200); // debounce simples
  }
  ultimoEstadoBotao = estadoBotao;

  // --- Se não estiver travado, lê o potenciômetro ---
  if (!travado) {
    int leituraPot = analogRead(PotPin);

    if (abs(leituraPot - ultimoPot) > tolerancia) {
      if (leituraPot > ultimoPot) {
        numero++;
        if (numero > 9) numero = 0;
      } else {
        numero--;
        if (numero < 0) numero = 9;
      }
      mostrarNumero(numero);
      Serial.print("Numero: ");
      Serial.println(numero);

      ultimoPot = leituraPot;
      delay(150); // pequena pausa para evitar múltiplas contagens
    }
  }
}

// --- Função para enviar número ao CD4511 ---
void mostrarNumero(int n) {
  digitalWrite(PinoA0, n & 0x01);
  digitalWrite(PinoA1, (n >> 1) & 0x01);
  digitalWrite(PinoA2, (n >> 2) & 0x01);
  digitalWrite(PinoA3, (n >> 3) & 0x01);
}
