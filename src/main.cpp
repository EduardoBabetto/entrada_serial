#include <Arduino.h>

void leituraBloqueada();
void leituraNaoBloqueante();
void processarFrase(char *);
int lerProximoNumero(char *&);
void aplicarCor();

int vermelho = 0;
int verde = 0;
int azul = 0;
int brilho = 0;

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  //* readStringUntil espera no maximo esse tempo (ms) peli \n
  //* O padrao e 100ms
}

void loop()
{
  leituraNaoBloqueante();
}

void leituraBloqueada()
{
}

void leituraNaoBloqueante()
{
  const int TAMANHO_MAX = 50; //* tamanho do vetor(incluir '/0')
  static char frase[TAMANHO_MAX];
  static int position = 0;

  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == '\r')
      continue;
    //* o continue pula restante do while naquela volta e passa pra leitura
    //* do proximo caractere. O '\r' e lido ee simplesmente descartado, sem entrar
    //* no vetor
    if (c == '\n')
    {
      frase[position] = '\0';
      if (position > 0)
        processarFrase(frase);
      position = 0;
    }
    else if (position < TAMANHO_MAX - 1)
    {
      frase[position] = c;
      position++;
    }
  }
}

//* VERMELHO,VERDE, AZUL, BRANCO , DESLIGA
//! RGB r g b (EX: RGB 200 125 98)
//* BRILHO n (0 a 100%)

void processarFrase(char *texto)
{
  Serial.printf("Recebido: [%s]\r\n", texto);

  char *comando = texto;
  char *parametro = NULL; //* -> \0
  char *espaco = strchr(texto, ' ');

  if (espaco != NULL) // SE ele encontrou um espaco...
  {
    parametro = espaco + 1;
    *espaco = '\0';
  }

  //* char* endereco = 4;
  //* *endereco = p;
  //*          0123456789
  //! memoria [abcdefghij]

  if (strcasecmp(comando, "VERMELHO") == 0)
  {

    vermelho = 255;
    verde = 0;
    azul = 0;
    brilho = 100;
  }
  else if (strcasecmp(comando, "VERDE") == 0)
  {
    vermelho = 0;
    verde = 255;
    azul = 0;
    brilho = 100;
  }
  else if (strcasecmp(comando, "AZUL") == 0)
  {
    vermelho = 0;
    verde = 0;
    azul = 225;
    brilho = 100;
  }
  else if (strcasecmp(comando, "BRANCO") == 0)
  {
    vermelho = 255;
    verde = 255;
    azul = 255;
    brilho = 100;
  }
  else if (strcasecmp(comando, "DESLIGA") == 0)
  {
    vermelho = 0;
    verde = 0;
    azul = 0;
  }
  else if (strcasecmp(comando, "RGB") == 0)
  {
    if (parametro == NULL)
    {
      Serial.println("Use o comando RGB rgb (0 a 255)");
      return;
    }
    char *cursor = parametro;
    int r = lerProximoNumero(cursor);
    int g = lerProximoNumero(cursor);
    int b = lerProximoNumero(cursor);

    vermelho = constrain(r, 0, 255); //*limita o valor ja convertido para int de 0 a 255;
    verde = constrain(g, 0, 255);
    azul = constrain(b, 0, 255);
  }
  else if (strcasecmp(comando, "BRILHO") == 0)
  {
    if (parametro == NULL)
    {
      Serial.println("Use o comando: BRILHO n (0 a 100)"); //* GRB 200 100 50
      return;
    }
    brilho = constrain(atoi(parametro), 0, 100);
  }
  else
  {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Comando: [%s] nao existe", comando);
    Serial.print("Comando nao encontrado");
    return;
  }
  aplicarCor();
}

//! r g b
void aplicarCor()
{
  int r = vermelho * brilho / 100;
  int g = verde * brilho / 100;
  int b = azul * brilho / 100;

  rgbLedWrite(PIN_RGB_LED, r, g, b);

  char bufferMensagem[80];
  snprintf(bufferMensagem, sizeof(bufferMensagem), "Cor: R=%3d G=%3d B=%3d | Brilho: %3d %% | HEX: #%02X%02X%02X",
           vermelho, verde, azul, brilho, r, g, b);
  Serial.println(bufferMensagem);
}

//* o & é usado para informar que vamos trabalhar com o conteúdo do ponteiro
//* a funcao recebe a propria variavel de quem chamou e nao uma copia
//*
//* PARAMETRO      |          ALTERNA O TEXTO APONTADO     |       ALTERA PARA ONDE O PONTEIRO DE FORA APONTA
//* char* p        |             sim(*p = '\0')            |                  nao, pq ele e uma copia
//* const char* p  |             nao                       |                    nao
//* char* &p       |             sim                       |        sim, e a propia variavel passada como parametro


int lerProximoNumero(char* &cursor)
{
  if(cursor == NULL) return 0;//* proteje o estouro de memoria no caso atoi(NULL)
  // atoi converte texto para numero
  int valor = atoi(cursor);
  char *espaco = strchr(cursor, ' ');
  cursor = (espaco != NULL) ? espaco + 1 : NULL;
  return valor;
}
