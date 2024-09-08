// os pinos dos comentários são de 1 a 16 da matriz 1588BS
// os pinos das contantes são os pinos/portas do Arduino equivalentes

// Resistores de 330

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
// pino 16 coluna 8
#define col_8 10
// pino 15 coluna 7
#define col_7 11

// Resistores de 220

// pino 14 linha 2
#define row_2 12
// pino 13 coluna 1
#define col_1 13
// pino 12 linha 4
#define row_4 A0
// pino 11 coluna 6
#define col_6 A1
// pino 10 coluna 4
#define col_4 A2
// pino 9 linha 1
#define row_1 A3


int data[8][8] = {
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 0, 1, 1, 0, 0, 0},
    {0, 1, 0, 1, 1, 0, 0, 0}
};

// Definição das funções que serão utilizadas
void selectRow(int row);
void selectCol (int col);

void setup() {
  for(int i = 2; i <= 13; i++){
    pinMode(i, OUTPUT);
  }

    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
}

void loop() {
  for(int j=0; j<8; j++){
    selectRow(j+1);
    for(int i=0; i<8; i++)selectCol(i+1, data[j][i]);
    delay(2);
  }
}

void selectRow (int row){
  if (row==1) digitalWrite(row_1, HIGH); else digitalWrite(row_1, LOW);
  if (row==2) digitalWrite(row_2, HIGH); else digitalWrite(row_2, LOW);
  if (row==3) digitalWrite(row_3, HIGH); else digitalWrite(row_3, LOW);
  if (row==4) digitalWrite(row_4, HIGH); else digitalWrite(row_4, LOW);
  if (row==5) digitalWrite(row_5, HIGH); else digitalWrite(row_5, LOW);
  if (row==6) digitalWrite(row_6, HIGH); else digitalWrite(row_6, LOW);
  if (row==7) digitalWrite(row_7, HIGH); else digitalWrite(row_7, LOW);
  if (row==8) digitalWrite(row_8, HIGH); else digitalWrite(row_8, LOW);
}

void selectCol (int col, int state){
  
  if (col==1 && state == 1) digitalWrite(col_1, LOW); else digitalWrite(col_1, HIGH);
  if (col==2 && state == 1) digitalWrite(col_2, LOW); else digitalWrite(col_2, HIGH);
  if (col==3 && state == 1) digitalWrite(col_3, LOW); else digitalWrite(col_3, HIGH);
  if (col==4 && state == 1) digitalWrite(col_4, LOW); else digitalWrite(col_4, HIGH);
  if (col==5 && state == 1) digitalWrite(col_5, LOW); else digitalWrite(col_5, HIGH);
  if (col==6 && state == 1) digitalWrite(col_6, LOW); else digitalWrite(col_6, HIGH);
  if (col==7 && state == 1) digitalWrite(col_7, LOW); else digitalWrite(col_7, HIGH);
  if (col==8 && state == 1) digitalWrite(col_8, LOW); else digitalWrite(col_8, HIGH);
}