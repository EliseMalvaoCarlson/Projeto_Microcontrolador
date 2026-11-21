#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define RELE_ATIVO_LOW false

#if RELE_ATIVO_LOW
  #define RELE_LIGADO LOW
  #define RELE_DESLIGADO HIGH
#else
  #define RELE_LIGADO HIGH
  #define RELE_DESLIGADO LOW
#endif

#define TRIG_CIMA 4
#define ECHO_CIMA 5
#define TRIG_BAIXO 18
#define ECHO_BAIXO 19
#define PINO_RELE 15

// Botoes opcionais para apresentacao (ligar pino ao GND)
#define PINO_BOTAO_SAIDA 25
#define PINO_BOTAO_ENTRADA 26

#define LARGURA_OLED 128
#define ALTURA_OLED 64
#define ENDERECO_OLED 0x3C

Adafruit_SSD1306 oled(LARGURA_OLED, ALTURA_OLED, &Wire, -1);

const int DISTANCIA_GATILHO = 100;
const unsigned long TEMPO_MAXIMO_MOVIMENTO = 10000;

unsigned long tempoPrimeiraDeteccao = 0;
int estadoMovimento = 0;
int contadorPessoas = 0;

bool lampadaBloqueada = false;
unsigned long tempoLampadaLigada = 0;

int ultimoEstadoPessoas = 0;

bool cimaAntes = false;
bool baixoAntes = false;

bool botaoSaidaAntes = HIGH;
bool botaoEntradaAntes = HIGH;
unsigned long ultimoCliqueBotao = 0;

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

  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println("CONTROLE DO AMBIENTE");

  oled.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  oled.setTextSize(2);
  oled.setCursor(0, 19);
  oled.print("Pessoas: ");
  oled.println(contadorPessoas);

  oled.setTextSize(1);
  oled.setCursor(0, 43);

  if (contadorPessoas == 0) {
    oled.println("Luz: DESLIGADA");
  } else if (lampadaBloqueada) {
    oled.println("Luz: BLOQUEADA");
  } else {
    oled.println("Luz: LIGADA");
  }

  oled.display();
}

void atualizarLampada() {
  if (contadorPessoas > 0) {
    digitalWrite(PINO_RELE, RELE_LIGADO);
  } else {
    digitalWrite(PINO_RELE, RELE_DESLIGADO);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_CIMA, OUTPUT);
  pinMode(ECHO_CIMA, INPUT);
  pinMode(TRIG_BAIXO, OUTPUT);
  pinMode(ECHO_BAIXO, INPUT);

  pinMode(PINO_BOTAO_SAIDA, INPUT_PULLUP);
  pinMode(PINO_BOTAO_ENTRADA, INPUT_PULLUP);

  digitalWrite(PINO_RELE, RELE_DESLIGADO);
  pinMode(PINO_RELE, OUTPUT);

  Wire.begin(21, 22);

  if (oled.begin(SSD1306_SWITCHCAPVCC, ENDERECO_OLED)) {
    oled.clearDisplay();
    oled.display();
  }

  cimaAntes = lerDistancia(TRIG_CIMA, ECHO_CIMA) < DISTANCIA_GATILHO;
  delay(15);
  baixoAntes = lerDistancia(TRIG_BAIXO, ECHO_BAIXO) < DISTANCIA_GATILHO;

  atualizarLampada();
  atualizarDisplayStatus();
}

void loop() {
  // 1. Botoes opcionais de +1 (GPIO 26) e -1 (GPIO 25)
  bool botaoEntradaAtual = digitalRead(PINO_BOTAO_ENTRADA);
  if (botaoEntradaAntes == HIGH && botaoEntradaAtual == LOW && (millis() - ultimoCliqueBotao > 250)) {
    ultimoCliqueBotao = millis();
    contadorPessoas++;
    estadoMovimento = 0;
  }
  botaoEntradaAntes = botaoEntradaAtual;

  bool botaoSaidaAtual = digitalRead(PINO_BOTAO_SAIDA);
  if (botaoSaidaAntes == HIGH && botaoSaidaAtual == LOW && (millis() - ultimoCliqueBotao > 250)) {
    ultimoCliqueBotao = millis();
    if (contadorPessoas > 0) {
      contadorPessoas--;
    }
    estadoMovimento = 0;
  }
  botaoSaidaAntes = botaoSaidaAtual;

  // 2. Leitura dos sensores ultrassonicos
  long distCima = lerDistancia(TRIG_CIMA, ECHO_CIMA);
  delay(15);
  long distBaixo = lerDistancia(TRIG_BAIXO, ECHO_BAIXO);

  bool cimaPerto = distCima < DISTANCIA_GATILHO;
  bool baixoPerto = distBaixo < DISTANCIA_GATILHO;

  bool baixoAproximou = (!baixoAntes && baixoPerto);
  bool cimaAproximou  = (!cimaAntes && cimaPerto);
  bool cimaAfastou    = (cimaAntes && !cimaPerto);
  bool baixoAfastou   = (baixoAntes && !baixoPerto);

  if (estadoMovimento != 0 && (millis() - tempoPrimeiraDeteccao > TEMPO_MAXIMO_MOVIMENTO)) {
    estadoMovimento = 0;
  }

  if (estadoMovimento == 0) {
    // Entrada: Baixo aproxima primeiro (com Cima ainda longe)
    if (baixoAproximou && !cimaPerto) {
      estadoMovimento = 1;
      tempoPrimeiraDeteccao = millis();
    }
    // Saida (partindo de longe): Cima aproxima primeiro (com Baixo ainda longe)
    else if (cimaAproximou && !baixoPerto) {
      estadoMovimento = 2;
      tempoPrimeiraDeteccao = millis();
    }
    // Saida (partindo de perto): Cima afasta primeiro (com Baixo ainda perto)
    // Se Baixo afastar primeiro, nao entra aqui: apenas reseta livre para a proxima entrada!
    else if (cimaAfastou && baixoPerto) {
      estadoMovimento = 3;
      tempoPrimeiraDeteccao = millis();
    }
  }
  else if (estadoMovimento == 1) {
    if (cimaAproximou) {
      contadorPessoas++;
      estadoMovimento = 0;
    } else if (baixoAfastou) {
      estadoMovimento = 0; // Cancelou o movimento
    }
  }
  else if (estadoMovimento == 2) {
    if (baixoAproximou) {
      if (contadorPessoas > 0) contadorPessoas--;
      estadoMovimento = 0;
    } else if (cimaAfastou) {
      estadoMovimento = 0;
    }
  }
  else if (estadoMovimento == 3) {
    if (baixoAfastou) {
      if (contadorPessoas > 0) contadorPessoas--;
      estadoMovimento = 0;
    } else if (cimaAproximou) {
      estadoMovimento = 0;
    }
  }

  cimaAntes = cimaPerto;
  baixoAntes = baixoPerto;

  // 3. Atualiza lampada e display
  if (contadorPessoas != ultimoEstadoPessoas) {
    if (ultimoEstadoPessoas == 0 && contadorPessoas > 0) {
      tempoLampadaLigada = millis();
      lampadaBloqueada = false;
    }

    if (ultimoEstadoPessoas > 0 && contadorPessoas == 0) {
      lampadaBloqueada = false;
    }

    ultimoEstadoPessoas = contadorPessoas;
    atualizarDisplayStatus();
  }

  atualizarLampada();

  delay(50);
}
