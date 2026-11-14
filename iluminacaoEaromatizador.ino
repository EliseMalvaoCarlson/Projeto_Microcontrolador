
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define TRIG_CIMA 4
#define ECHO_CIMA 5
#define TRIG_BAIXO 18
#define ECHO_BAIXO 19
#define PINO_RELE 15
#define PINO_BUZZER 23

#define LARGURA_OLED 128
#define ALTURA_OLED 64
#define ENDERECO_OLED 0x3C

Adafruit_SSD1306 oled(
  LARGURA_OLED,
  ALTURA_OLED,
  &Wire,
  -1
);

const int DISTANCIA_GATILHO = 100;

const unsigned long TEMPO_MAXIMO_MOVIMENTO = 3000;
const unsigned long TEMPO_MAXIMO_LAMPADA =
  2UL * 60UL * 60UL * 1000UL;
const unsigned long DURACAO_BIP = 1000;

unsigned long tempoPrimeiraDeteccao = 0;
int estadoMovimento = 0;
int contadorPessoas = 0;

// Controle da lampada
bool lampadaBloqueada = false;
unsigned long tempoLampadaLigada = 0;

// Controle do aromatizador
unsigned long tempoInicioBip = 0;
bool buzzerTocando = false;

// Detecta mudancas na contagem
int ultimoEstadoPessoas = 0;

// Mensagem temporaria no display
unsigned long tempoMensagem = 0;
bool mensagemFalha = false;

long lerDistancia(int pinoTrig, int pinoEcho) {
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);

  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);

  long duracao = pulseIn(pinoEcho, HIGH, 30000);

  if (duracao == 0) {
    return 999;
  }

  return duracao * 0.034 / 2;
}

void atualizarDisplayStatus() {
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);

  // Titulo
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println("CONTROLE DO AMBIENTE");

  oled.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  // Contagem
  oled.setTextSize(2);
  oled.setCursor(0, 19);
  oled.print("Pessoas: ");
  oled.println(contadorPessoas);

  // Estado da lampada
  oled.setTextSize(1);
  oled.setCursor(0, 43);

  if (contadorPessoas == 0) {
    oled.println("Luz: DESLIGADA");
  } else if (lampadaBloqueada) {
    oled.println("Luz: BLOQUEADA");
  } else {
    oled.println("Luz: LIGADA");
  }

  // Estado do aromatizador
  oled.setCursor(0, 54);

  if (buzzerTocando) {
    oled.println("Aromatizador: ATIVO");
  } else {
    oled.println("Aromatizador: PRONTO");
  }

  oled.display();
}

void atualizarLampada() {
  bool deveLigar =
    (contadorPessoas > 0 && !lampadaBloqueada);

  digitalWrite(PINO_RELE, deveLigar ? HIGH : LOW);
}

void acionarAromatizador() {
  tone(PINO_BUZZER, 2000);

  buzzerTocando = true;
  tempoInicioBip = millis();
}

void atualizarAromatizador() {
  if (!buzzerTocando) {
    return;
  }

  if (millis() - tempoInicioBip >= DURACAO_BIP) {
    noTone(PINO_BUZZER);
    buzzerTocando = false;
    atualizarDisplayStatus();
  }
}

void mostrarFalhaMovimento() {
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);

  oled.setCursor(0, 10);
  oled.println("MOVIMENTO FALHOU");

  oled.setCursor(0, 30);
  oled.println("Tente novamente");

  oled.display();

  tempoMensagem = millis();
  mensagemFalha = true;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_CIMA, OUTPUT);
  pinMode(ECHO_CIMA, INPUT);
  pinMode(TRIG_BAIXO, OUTPUT);
  pinMode(ECHO_BAIXO, INPUT);

  pinMode(PINO_RELE, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  digitalWrite(PINO_RELE, LOW);
  noTone(PINO_BUZZER);

  Wire.begin(21, 22);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, ENDERECO_OLED)) {
    Serial.println("Falha ao iniciar o OLED");
    while (true) {
      delay(100);
    }
  }

  oled.clearDisplay();
  oled.display();

  atualizarLampada();
  atualizarDisplayStatus();
}

void loop() {
  long distCima = lerDistancia(TRIG_CIMA, ECHO_CIMA);

  delay(10);

  long distBaixo = lerDistancia(TRIG_BAIXO, ECHO_BAIXO);

  bool detectouCima = distCima < DISTANCIA_GATILHO;
  bool detectouBaixo = distBaixo < DISTANCIA_GATILHO;

  // Timeout de movimento
  if (estadoMovimento != 0 &&
      millis() - tempoPrimeiraDeteccao >
      TEMPO_MAXIMO_MOVIMENTO) {

    estadoMovimento = 0;
    mostrarFalhaMovimento();
  }

  if (estadoMovimento == 0) {

    // Entrada: sensor superior primeiro
    if (detectouCima && !detectouBaixo) {
      estadoMovimento = 1;
      tempoPrimeiraDeteccao = millis();
    }

    // Saida: sensor inferior primeiro
    else if (detectouBaixo && !detectouCima) {
      estadoMovimento = 2;
      tempoPrimeiraDeteccao = millis();
    }
  }

  else if (estadoMovimento == 1) {

    if (detectouBaixo) {
      contadorPessoas++;
      estadoMovimento = 0;
    }
  }

  else if (estadoMovimento == 2) {

    if (detectouCima) {

      if (contadorPessoas > 0) {
        contadorPessoas--;
      }

      estadoMovimento = 0;
    }
  }

  // Detecta alteracoes no numero de pessoas
  if (contadorPessoas != ultimoEstadoPessoas) {

    // Ambiente passou de vazio para ocupado
    if (ultimoEstadoPessoas == 0 &&
        contadorPessoas > 0) {

      tempoLampadaLigada = millis();
      lampadaBloqueada = false;
    }

    // A ultima pessoa saiu
    if (ultimoEstadoPessoas > 0 &&
        contadorPessoas == 0) {

      lampadaBloqueada = false;

      // Um unico bip de 1 segundo
      acionarAromatizador();
    }

    ultimoEstadoPessoas = contadorPessoas;
    atualizarDisplayStatus();
  }

  // Limite de duas horas de iluminacao
  if (contadorPessoas > 0 && !lampadaBloqueada) {

    if (millis() - tempoLampadaLigada >=
        TEMPO_MAXIMO_LAMPADA) {

      lampadaBloqueada = true;
    }
  }

  atualizarLampada();
  atualizarAromatizador();

  // Restaura a tela principal apos uma falha
  if (mensagemFalha &&
      millis() - tempoMensagem >= 1500) {

    mensagemFalha = false;
    atualizarDisplayStatus();
  }

  delay(50);
}
