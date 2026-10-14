#include <Arduino.h>

void leituraBloqueada();
void leituraNaoBloqueante();
void processarFrase(const char*);

void setup() {
 Serial.begin(115200);
 Serial.setTimeout(2000);
 //* readStringUntil espera no maximo esse tempo (ms) peli \n
 //* O padrao e 100ms

 Serial.println();
 Serial.println("Digite seu nome e pressione ENTER: ");
}

void loop() {
  leituraNaoBloqueante();
}

void leituraBloqueada() {

}

void leituraNaoBloqueante() {
  const int TAMANHO_MAX = 50; //* tamanho do vetor(incluir '/0')
  static char frase[TAMANHO_MAX];
  static int position = 0;
  
  while(Serial.available() > 0){
    char c = Serial.read();
    if(c=='\r') continue;
    //* o continue pula restante do while naquela volta e passa pra leitura
    //* do proximo caractere. O '\r' e lido ee simplesmente descartado, sem entrar
    //* no vetor
    if(c =='\n'){
      frase[position] = '\0';
      if(position> 0) processarFrase(frase);
      position = 0;
    }
    else if (position < TAMANHO_MAX - 1) {
      frase[position] = c;
      position++;
    }
  }
}

void processarFrase(const char* texto){
  char espaco = 32;
    Serial.print(espaco);
    Serial.print(espaco);
    Serial.print(espaco);
    Serial.print(espaco);
    Serial.print(espaco);

    Serial.printf("Recebido: [%s]\r\n",texto);

    if(strcmp(texto, "Ola")==0){
      Serial.println("Ola para voce tambem!");
    }
}
