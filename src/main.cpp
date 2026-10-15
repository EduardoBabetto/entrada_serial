
#include <Arduino.h>

void leituraBloqueante();
void leituraNaoBloqueante();
void processarFrase(char* texto);
int lerProximoNumero(char*& cursor);
void aplicarCor();

int vermelho = 0;
int verde = 0;
int azul = 0;
int brilho = 0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  //* readStringUntil espera no máximo esse tempo (ms) pelo '\n'(ENTER)
  //* O padrão é 1000ms
}


void loop() {
  leituraNaoBloqueante();
}

void leituraNaoBloqueante(){
  const int TAMANHO_MAX = 50; //* tamanho do vetor (incluir no cálculo o '/0')
  static char frase[TAMANHO_MAX];
  static int position = 0;

  while(Serial.available() > 0){
    char c = Serial.read();

    if(c == '\r') continue;
    //* o continue pula o restanto do while naquela volta e passa para a leitura 
    //* do próximo caractere. O '\r' é lido e simplesmente descartado, sem entrar no vetor.

    if(c == '\n'){
      frase[position] = '\0';
      if(position > 0) processarFrase(frase);
      position = 0;
    }
    else if(position < TAMANHO_MAX -1){
      frase[position] = c;
      position++;
    }
  }
}


//*VERMELHO
//* VERDE
//* AZUL
//* BRANCO
//* DESLIGA
//* RGB r g b (ex.: RGB 200 125 98)
//* BRILHO n (0 a 100%)
void processarFrase(char* texto){
  Serial.printf("Recebido: [%s] \n\r", texto);

  char* comando = texto;
  char* parametro = NULL; //* NULL -> '\0'

  char* espaco = strchr(texto, ' ');
  
  if(espaco != NULL){   // Se ele encontrou um espaço...
    parametro = espaco + 1;
    *espaco = '\0';
  }

  if(strcasecmp (comando, "VERMELHO") == 0) {   //* strcasecmp = compara a ordem léxica das palavras não ligando para Letras maiúsculas e minúsculas
    vermelho = 255;
    verde = 0;
    azul = 0;
    brilho = 100;
  }
  else if(strcasecmp (comando, "VERDE") == 0) {   
    vermelho = 0;
    verde = 255;
    azul = 0;
    brilho = 100;
  }
  else if(strcasecmp (comando, "AZUL") == 0) {   
    vermelho = 0;
    verde = 0;
    azul = 255;
    brilho = 100;
  }
  else if(strcasecmp (comando, "BRANCO") == 0) {   
    vermelho = 255;
    verde = 255;
    azul = 255;
    brilho = 100;
  }
  else if(strcasecmp (comando, "DESLIGA") == 0) {   
    vermelho = 0;
    verde = 0;
    azul = 0;
  }
  else if(strcasecmp (comando, "RGB") == 0) {   
    if(parametro == NULL){
      Serial.println("Use o comando: RGB r g b (0 a 255)");
      return;
    }

    char* cursor = parametro;
    
    int r = lerProximoNumero(cursor);
    int g = lerProximoNumero(cursor);
    int b = lerProximoNumero(cursor);

    vermelho = constrain(r, 0, 255); //* limita o valor já convertido para int de 0 a 255;
    verde = constrain(g, 0, 255);
    azul = constrain(b, 0, 255);
  }
  else if(strcasecmp (comando, "BRILHO") == 0) {   
    if(parametro == NULL){
      Serial.println("Use o comando: BRILHO n (0 a 100%)");
      return;
    }

    brilho = constrain(atoi(parametro), 0, 100);
  }
  else{
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Comando: [%s] não existe", comando);
    Serial.println(buffer);
    return;
  }
  aplicarCor();
}

void aplicarCor(){
  int r = vermelho * brilho / 100;
  int g = verde * brilho / 100;
  int b = azul * brilho / 100;

  rgbLedWrite(PIN_RGB_LED, r, g, b);

  char bufferMensagem[80];
  snprintf(bufferMensagem, sizeof(bufferMensagem), "Cor: R= %3d G= %3d B= %3d | Brilho: %3d %% | HEX: #%02X%02X%02X",
            vermelho, verde, azul, brilho, r, g, b);
 
  Serial.println(bufferMensagem);
}

//* cursor é uma referencia (&) para um ponteiro char (char*)
//* a função recebe a própria variável de quem chamou e não uma cópia
//*
//* |  PARAMETRO         |  ALTERA O TEXTO APONTADO   |  ALTERA PARA ONDE O PONTEIRO DE FORA APONTA           |
//* |    char* p         |    sim (*p = '\0')         |     não, porque ele é uma cópia                       |
//* |    const char* p   |    não                     |     não                                               |
//* |    char* &p        |    sim                     |     sim, é a própria variável passada como parametro  |

int lerProximoNumero(char*& cursor){    
  
  if(cursor == NULL) return 0; //* proteje o estouro de memória no caso atoi(NULL)
  
  int valor = atoi(cursor);   //* atoi pega o valor inserido e converte em inteiro.
  char* espaco = strchr(cursor, ' ');
  cursor = (espaco != NULL) ? espaco + 1 : NULL;
  return valor;
}  


void leituraBloqueante(){
    //* available() diz quantos bytes já chegaram e estão esperando a leitura
  if(Serial.available() > 0){
    
    String texto = Serial.readStringUntil('\n');
    
    //* trim() remove do fim do texto os espaços, '/r', '/t', '\n' que alguns terminais enviam
    texto.trim();
    
    if(texto.length() == 0) return;
    Serial.printf("Seu nome é %s \n\r", texto.c_str());
    Serial.printf("Quantidade de caracteres: %u \n\r", texto.length());

    Serial.printf("Primeira letra: %c \n\r", texto.charAt(0));
    Serial.printf("Ultima letra: %c \n\r", texto[  texto.length() -1] );

    String maiusculo = texto;
    maiusculo.toUpperCase();
    Serial.printf("Em maiusculas: %s \n\r", maiusculo.c_str());

    if(maiusculo == "EDUARDO"){
      Serial.println("Este é o seu nome correto");
    }
  }
}