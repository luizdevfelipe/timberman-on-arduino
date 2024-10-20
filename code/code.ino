// Biblioteca que controla o display LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// definindo variável para o display
LiquidCrystal_I2C lcd_1(0x27, 16, 2);

// os pinos dos comentários são de 1 a 16 da matriz 1588BS
// os pinos das contantes são os pinos/portas do Arduino equivalentes

// pino 1 linha 5
#define row_5 2
// pino 2 linha 7
#define row_7 3
// pino 3 coluna 2
#define col_2 4
// pino 4 coluna 3
#define col_3 5
// pino 5 linha 8
#define row_8 6
// pino 6 coluna 5
#define col_5 7
// pino 7 linha 6
#define row_6 8
// pino 8 linha 3
#define row_3 9
/* pino da última coluna que não é mais usado
 pino 16 coluna 8
 #define col_8 10
*/
// pino 15 coluna 7
#define col_7 11
// pino 14 linha 2
#define row_2 12
/* pino da primeira coluna que não é mais utilizado
 pino 13 coluna 1
 #define col_1 13
*/
// pino 12 linha 4
#define row_4 A0
// pino 11 coluna 6
#define col_6 A1
// pino 10 coluna 4
#define col_4 A2
// pino 9 linha 1
#define row_1 A3
// pino A4 botão esquedo
#define left 10
// pino A5 botão direito
#define right 13

const bool mapas[4][8][8] = {
  { { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 } },

  { { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 } },

  { { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 } },

  { { 0, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 } }
};

// Definição das funções que serão utilizadas
void selectRow(int row);
void selectCol(int col);
void mostraDisplay(bool mapas[4][8][8], long mapa[10], char lado = 'n', bool bate = false);
bool jogando();
void definir_valores_padrao();
// Definição de variáveis que serão utilizadas
bool perdeu;
long randNumber[10];
int atual;
int altura;
int pontuacao;
unsigned long tempoAnterior;
int anteriorEsq;
int anteriorDir;
bool temp[8][8];
char ladoAnterior;

void setup() {
  lcd_1.init();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  definir_valores_padrao();

  lcd_1.setBacklight(HIGH);
  lcd_1.print("Aperte um botao");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Evite os galhos!");
}

void loop() {
  int tempo = 5;
  perdeu = false;
  // delay que evita mal contato de leitura dos botões
  delay(5);  
  if (digitalRead(left) == HIGH || digitalRead(right) == HIGH) {

    while (!perdeu && tempo > 0) {
      // obtém o tempo atual desde que o Arduino foi inicializado em ms
      unsigned long tempoAtual = millis();
      // realiza a jogado do jogador, retornando se ele batou ao não
      bool bateu = jogando();
      // se o jogador não fez uma jogada
      if (!bateu) {
        // o tempo disponível diminui, se passados 1 segundo
        if (tempoAtual - tempoAnterior >= 1000) {
          tempo--;
          tempoAnterior = tempoAtual;
        }
      } else {
        if (tempo < 5) tempo++;
      }
    }

    // fazer exibir o placar, com uma função dedicada
    // Serial.print("Sua pontuação: ");
    // Serial.println(pontuacao);
    delay(2000);

    // quando perde, reseta os valores de variáveis
    definir_valores_padrao();

  } else {
    // do lado esquerdo, sem bater esperando o jogo começar
    mostraDisplay(mapas, randNumber, 'e', false);
  }
}

bool jogando() {
  char lado;
  bool bateu = false;
  int atualEsq = digitalRead(left);
  int atualDir = digitalRead(right);

  // faz a leitura dos botões, somente quando pressionado e solto (click completo)
  if (anteriorEsq == LOW && atualEsq == HIGH) {
    lado = 'e';
    bateu = true;
    // verifica se o personagem bateu a cabeça na jogada
    if (temp[5][2] == 1 && bateu) {
      perdeu = true;
    } else {
      pontuacao++;
    }
  } else if (anteriorDir == LOW && atualDir == HIGH) {
    lado = 'd';
    bateu = true;
    // verifica se o personagem bateu a cabeça na jogada
    if (temp[5][5] == 1 && bateu) {
      perdeu = true;
    } else {
      pontuacao++;
    }
  }

  anteriorEsq = atualEsq;
  anteriorDir = atualDir;
  mostraDisplay(mapas, randNumber, lado, bateu);
  return bateu;
}

void mostraDisplay(bool mapas[4][8][8], long mapa[10], char lado = 'n', bool bate = false) {
  bool t[8][8];

  if (bate) {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        t[row][col] = temp[row][col];

        if (row == 0) {
          if (altura <= 7) {
            temp[0][col] = mapas[mapa[atual + 1]][7 - altura][col];
          } else {
            altura = 0;
            atual++;
          }

        } else {
          temp[row][col] = t[row - 1][col];
        }
      }
    }
    altura++;
  }

  if (lado == 'e') {
    // exibe o personagem do lado esquerdo
    temp[6][1] = 1;
    temp[7][1] = 1;
    if (bate) {
      // exibe o personagem batendo
      temp[6][2] = 1;
      // remove o tronco do lado oposto
      temp[6][5] = 0;
      temp[6][6] = 0;
    } else {
      // limpa o personagem batendo
      temp[6][2] = 0;
    }
    if (ladoAnterior == 'd') {
      // limpa o personagem do lado direito
      temp[6][6] = 0;
      temp[7][6] = 0;
    }
  } else if (lado == 'd') {
    // exibe o personagem do lado direito
    temp[6][6] = 1;
    temp[7][6] = 1;
    if (bate) {
      // exibe o personagem batendo
      temp[6][5] = 1;
      // remove o tronco do lado oposto
      temp[6][1] = 0;
      temp[6][2] = 0;
    } else {
      // limpa o personagem batendo
      temp[6][5] = 0;
    }
    if (ladoAnterior == 'e') {
      // limpa o personagem do lado esquerdo
      temp[6][1] = 0;
      temp[7][1] = 0;
    }
  }

  ladoAnterior = lado;

  // exibe o vetor no display selecionando led por led
  for (int row = 0; row < 8; row++) {
    selectRow(row + 1);
    for (int col = 0; col < 8; col++) selectCol(col + 1, temp[row][col]);
    delay(1);
  }
}

// função que seleciona uma linha com HIGH e ignora as outras com LOW
void selectRow(int row) {
  if (row == 1) digitalWrite(row_1, HIGH);
  else digitalWrite(row_1, LOW);
  if (row == 2) digitalWrite(row_2, HIGH);
  else digitalWrite(row_2, LOW);
  if (row == 3) digitalWrite(row_3, HIGH);
  else digitalWrite(row_3, LOW);
  if (row == 4) digitalWrite(row_4, HIGH);
  else digitalWrite(row_4, LOW);
  if (row == 5) digitalWrite(row_5, HIGH);
  else digitalWrite(row_5, LOW);
  if (row == 6) digitalWrite(row_6, HIGH);
  else digitalWrite(row_6, LOW);
  if (row == 7) digitalWrite(row_7, HIGH);
  else digitalWrite(row_7, LOW);
  if (row == 8) digitalWrite(row_8, HIGH);
  else digitalWrite(row_8, LOW);
}

// função que seleciona uma coluna com LOW e ignora as outras com HIGH
void selectCol(int col, int state) {
  // if (col == 1 && state == 1) digitalWrite(col_1, LOW);
  // else digitalWrite(col_1, HIGH);
  if (col == 2 && state == 1) digitalWrite(col_2, LOW);
  else digitalWrite(col_2, HIGH);
  if (col == 3 && state == 1) digitalWrite(col_3, LOW);
  else digitalWrite(col_3, HIGH);
  if (col == 4 && state == 1) digitalWrite(col_4, LOW);
  else digitalWrite(col_4, HIGH);
  if (col == 5 && state == 1) digitalWrite(col_5, LOW);
  else digitalWrite(col_5, HIGH);
  if (col == 6 && state == 1) digitalWrite(col_6, LOW);
  else digitalWrite(col_6, HIGH);
  if (col == 7 && state == 1) digitalWrite(col_7, LOW);
  else digitalWrite(col_7, HIGH);
  // if (col == 8 && state == 1) digitalWrite(col_8, LOW);
  // else digitalWrite(col_8, HIGH);
}
// função que define os valores padrões de uma nova partida
void definir_valores_padrao() {
  // Gerando 10 valores aleatórios compreendidos entre 0 e 3
  randomSeed(analogRead(A5));
  for (int i = 0; i < 10; i++) {
    randNumber[i] = random(0, 4);
  }
  atual = 0;
  altura = 0;
  pontuacao = 0;
  tempoAnterior = 0;
  anteriorEsq = LOW;
  anteriorDir = LOW;
  ladoAnterior = 'e';
  // Copia elemento por elemento em um vetor temporário
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      temp[row][col] = mapas[randNumber[0]][row][col];
    }
  }
}