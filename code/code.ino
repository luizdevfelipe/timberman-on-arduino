// Biblioteca que controla o display LCD
#include <LiquidCrystal_I2C.h>
// Biblioteca que controla a EEPROM
#include <EEPROM.h>

// instanciando a variável do display
LiquidCrystal_I2C lcd_1(0x27, 16, 2);

// os pinos dos comentários são de 1 a 16 da matriz 1588BS com sua respectiva linha ou coluna
// os pinos das contantes são os pinos/portas do Arduino equivalentes

// pino 1 linha 5
#define row_5 A2
// pino 2 linha 7
#define row_7 A3
// pino 3 coluna 2
#define col_2 7
// pino 4 coluna 3
#define col_3 6
// pino 5 linha 8
#define row_8 5
// pino 6 coluna 5
#define col_5 4
// pino 7 linha 6
#define row_6 3
// pino 8 linha 3
#define row_3 2
// pino 15 coluna 7
#define col_7 8
// pino 14 linha 2
#define row_2 9
// pino 12 linha 4
#define row_4 10
// pino 11 coluna 6
#define col_6 11
// pino 10 coluna 4
#define col_4 12
// pino 9 linha 1
#define row_1 13
// pino A0 botão esquedo
#define left A0
// pino A1 botão direito
#define right A1

// matriz que representa os mapas usados durante o jogo
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
    { 0, 1, 1, 1, 1, 0, 0, 0 },
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
    { 0, 0, 0, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 } }
};

// Definição das funções que serão utilizadas
bool jogando();
void mostraDisplay(char lado = 'n', bool bate = false);
void mostraTempo();
void mostraPontuacao();
void selectRow(int row);
void selectCol(int col, int state);
void definir_valores_padroes();

// Definição de variáveis que serão utilizadas
bool perdeu;
long mapa[10];
int atual;
int altura;
int pontuacao;
unsigned long tempoAnterior;
int anteriorEsq;
int anteriorDir;
bool temp[8][8];
char ladoAnterior;
float tempo;
int nivel;

void setup() {
  // inicializa o objeto de controle do display
  lcd_1.init();
  // define o modo de operação de cada pino
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  // função que irá definir os valores iniciais do jogo
  definir_valores_padroes();
}

void loop() {
  // delay que evita mal contato de leitura dos botões
  delay(5);
  // condição que verifica se um dois dois botões foi pressionado, iniciando o jogo
  if (digitalRead(left) == HIGH || digitalRead(right) == HIGH) {
    // loop que fica ativo enquanto o jogador não perdeu, executando a lógica do jogo
    while (!perdeu && tempo > 0) {
      // obtém o tempo atual desde que o Arduino foi inicializado em ms
      unsigned long tempoAtual = millis();
      // realiza a jogada do jogador, retornando se ele bateu ao não
      bool bateu = jogando();
      // se o jogador não bateu é verificada a condição para que o tempo restante diminua, senão ele aumenta
      if (!bateu) {
        // o tempo disponível diminui, se passados 380 milissegundos
        if (tempoAtual - tempoAnterior >= 380) {
          // cada condição abaixo reduzirá o tempo relacionando-o com a pontuação atual, garantindo maiores dificuldades
          if (pontuacao >= 20 && pontuacao < 50) {
            tempo = tempo - 1.1;
            nivel = 2;
          } else if (pontuacao >= 50 && pontuacao < 80) {
            tempo = tempo - 1.2;
            nivel = 3;
          } else if (pontuacao >= 80 && pontuacao < 110) {
            tempo = tempo - 1.5;
            nivel = 4;
          } else if (pontuacao >= 110) {
            tempo = tempo - 1.7;
            nivel = 5;
          } else {
            tempo = tempo - 1.0;
            nivel = 1;
          }
          tempoAnterior = tempoAtual;
          // exibe no lcd o tempo para fazer a próxima jogada e o "nível" de dificuldade atual
          mostraTempo();
        }
      } else {
        // o tempo disponível é incrementado, quando o jogador bate, de acordo com sua pontuação também
        if (tempo <= 5) {
          if (pontuacao >= 20 && pontuacao < 50) {
            tempo = tempo + 0.7;
          } else if (pontuacao >= 50 && pontuacao < 80) {
            tempo = tempo + 0.5;
          } else if (pontuacao >= 80 && pontuacao < 110) {
            tempo = tempo + 0.1;
          } else if (pontuacao >= 110) {
            tempo = tempo + 0.06;
          } else {
            tempo = tempo + 1;
          }
        }
      }
    }
    // exibe o placar, com uma função dedicada ao perder
    mostraPontuacao();
    // quando perde, redefine os valores de variáveis
    definir_valores_padroes();
  } else {
    // exibe o personagem do lado esquerdo, sem bater e esperando o jogo começar
    mostraDisplay('e', false);
  }
}

// função que irá ler a jogada o usuário, verificando se foi válida, aumentando a pontuação e atualizando a matriz led
bool jogando() {
  // variáveis com valores padrões antes da leitura e verificação do click pelo jogador
  char lado;
  bool bateu = false;
  // leitura das portas do Arduino referentes aos botões
  int atualEsq = digitalRead(left);
  int atualDir = digitalRead(right);

  // faz comparação dos estados dos botões, sendo válido somente quando pressionado e solto (click completo)
  if (anteriorEsq == LOW && atualEsq == HIGH) {
    lado = 'e';
    bateu = true;
    // verifica se o personagem bateu a cabeça na jogada, senão aumenta a pontuação
    if (temp[5][2] == 1 && bateu) {
      perdeu = true;
    } else {
      pontuacao++;
    }
  } else if (anteriorDir == LOW && atualDir == HIGH) {
    lado = 'd';
    bateu = true;
    // verifica se o personagem bateu a cabeça na jogada, senão aumenta a pontuação
    if (temp[5][5] == 1 && bateu) {
      perdeu = true;
    } else {
      pontuacao++;
    }
  }
  // atualiza os valores das variáveis do click anterior
  anteriorEsq = atualEsq;
  anteriorDir = atualDir;
  // chama a função que exibe a jogada na matriz 1588BS
  mostraDisplay(lado, bateu);
  // retorna o valor de "bateu" para ver se o tempo será incrementado ou diminuído
  return bateu;
}

// função responsável por atualizar o display led 8x8
void mostraDisplay(char lado = 'n', bool bate = false) {
  // define um vetor temporário para auxiliar durante a troca de valores da matriz
  bool t[8][8];
  // verifica se todos os mapas foram utilizados durante a partida, para voltar do começo
  if (atual == 8) atual = 0;
  // verifica se o jogador bateu, fazendo com que o vetor a ser exibido na matriz led "avance" uma linha para baixo
  if (bate) {
    // laço que irá iterar para cada linha e coluna da matriz, alterando-a para que a imagem "avance"
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        // vetor temporário que terá todo o mapa atual, tornando possível avançar uma posição para cada linha
        t[row][col] = temp[row][col];
        // se a linha for a primeira, ela recebe as colunas da última linha do próximo mapa
        if (row == 0) {
          // verificação para confirmar que o próximo mapa não foi todo consumido ainda
          if (altura <= 7) {
            temp[0][col] = mapas[mapa[atual + 1]][7 - altura][col];
          } else {
            // caso tenha chegado ao fim do próximo mapa é redefinida a altura e passado para um terceiro novo mapa
            altura = 0;
            atual++;
          }
        // caso não esteja na primeira linha, o vetor é definido de modo que as linhas sejam adiantadas em uma unidade
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
  // atualiza o lado anterior
  ladoAnterior = lado;

  // exibe o vetor no display selecionando led por led
  for (int row = 0; row < 8; row++) {
    selectRow(row + 1);
    for (int col = 0; col < 8; col++) selectCol(col + 1, temp[row][col]);
    delay(1);
  }
}

// função que mostra o nível e tempo restante para o jogador fazer uma jogada
void mostraTempo() {
  lcd_1.clear();
  lcd_1.print("Nivel Atual: ");
  lcd_1.print(nivel);
  lcd_1.setCursor(0, 1);
  lcd_1.print("Tempo: ");
  // o tempo é exibido em formato de cerquilhas
  for (int i = 0; i < tempo; i++) {
    lcd_1.print("#");
  }
}

// função que exibe a pontuação após o jogador perder, com um efeito, piscando a luz de fundo
void mostraPontuacao() {
  lcd_1.clear();
  lcd_1.print("Voce Perdeu!");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Pontuacao: ");
  lcd_1.print(pontuacao);
  lcd_1.setBacklight(LOW);
  delay(1000);
  lcd_1.setBacklight(HIGH);
  delay(1000);
  lcd_1.setBacklight(LOW);
  delay(1000);
  lcd_1.setBacklight(HIGH);
  delay(1000);
  lcd_1.setBacklight(LOW);
  delay(1000);
  lcd_1.setBacklight(HIGH);
  delay(2100);
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
}

// função que define os valores padrões de uma nova partida
void definir_valores_padroes() {
  // Verifica se a pontuação é maior que 60, exibindo o recorde do jogo, senão uma mensagem explicativa
  if (pontuacao > 60) {
    // Verifica se a pontuação atual foi maior que o recorde registrado, armazenando-a
    if (pontuacao > EEPROM.read(0)) {
      EEPROM.write(0, pontuacao);
    }
    // Exibe a maior pontuação registrada
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Maior Pontuacao");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Registrada: ");
    lcd_1.print(EEPROM.read(0));
  } else {
    // Exibe uma mensagem instrutiva
    lcd_1.setBacklight(HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Aperte um botao");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Evite os galhos!");
  }
  // Gerando 10 valores aleatórios compreendidos entre 0 e 3
  randomSeed(analogRead(A0));
  for (int i = 0; i < 10; i++) {
    mapa[i] = random(0, 4);
  }
  atual = 0;
  altura = 0;
  pontuacao = 0;
  tempoAnterior = 0;
  anteriorEsq = LOW;
  anteriorDir = LOW;
  ladoAnterior = 'e';
  tempo = 5;
  perdeu = false;
  // Copia elemento por elemento em um vetor temporário
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      temp[row][col] = mapas[mapa[0]][row][col];
    }
  }
}