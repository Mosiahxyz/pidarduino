#include <PID_v1.h>

// Define os pinos dos sensores de linha
const int sensorEsquerdo = A0;
const int sensorCentro = A1;
const int sensorDireito = A2;

// Define os pinos dos motores
const int motorEsquerdo = 5;
const int motorDireito = 6;

// Parâmetros do PID
double setpoint = 0; // Valor de referência (sensor do meio)
double input, output;
double Kp = 1.0; // Constante proporcional
double Ki = 0.1; // Constante integral
double Kd = 0.05; // Constante derivativo
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  pinMode(sensorEsquerdo, INPUT);
  pinMode(sensorCentro, INPUT);
  pinMode(sensorDireito, INPUT);
  pinMode(motorEsquerdo, OUTPUT);
  pinMode(motorDireito, OUTPUT);

  Serial.begin(9600);
  pid.SetMode(AUTOMATIC);
}

void loop() {
  int leituraEsquerda = digitalRead(sensorEsquerdo);
  int leituraCentro = digitalRead(sensorCentro);
  int leituraDireita = digitalRead(sensorDireito);

  // Calcula o erro para o PID (sensor do meio - leitura do centro)
  input = leituraCentro;

  // Calcula a saída do PID
  pid.Compute();

  // Ajusta os motores com base na saída do PID
  int velocidadeBase = 150; // Velocidade base dos motores
  int velocidadeEsquerda = velocidadeBase - output;
  int velocidadeDireita = velocidadeBase + output;

  // Limita a velocidade dos motores para valores aceitáveis
  velocidadeEsquerda = constrain(velocidadeEsquerda, 0, 255);
  velocidadeDireita = constrain(velocidadeDireita, 0, 255);

  // Define as velocidades dos motores
  analogWrite(motorEsquerdo, velocidadeEsquerda);
  analogWrite(motorDireito, velocidadeDireita);

  // Exibe informações no monitor serial
  Serial.print("Leituras: ");
  Serial.print(leituraEsquerda);
  Serial.print(" ");
  Serial.print(leituraCentro);
  Serial.print(" ");
  Serial.println(leituraDireita);
  Serial.print("Saída do PID: ");
  Serial.println(output);

  delay(10); // Pequeno atraso para evitar leituras rápidas dos sensores
}
