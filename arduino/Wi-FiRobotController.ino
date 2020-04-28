/**
 *  Program: Wi-Fi Robot Controller
 *  Author: Marcelo Carvalho
 *  Team Robonáticos
*/

#include <ThingSpeak.h>
#include <WiFi.h>
#define ssid "Frajola"
#define password "0l1v31r@*"//*Alfa#oliveira


//IMPLEMENTANDO WATCHDOG

hw_timer_t *timer = NULL; //faz o controle do temporizador (interrupção por tempo)

//função que o temporizador irá chamar, para reiniciar o ESP32
void IRAM_ATTR resetModule(){
    ets_printf("(watchdog) reiniciar\n"); //imprime no log
    esp_restart(); //reinicia o chip
}
//FIM WATCHDOG



//COLOCAR NUMERO DO CANAL THINGSPEAK
unsigned long counterChannelNumber = 1027998;                // Channel ID
const char * myCounterReadAPIKey = "5YGLN7Z9ZA41RZGY";
const char * myCounterWriteAPIKey = "76I4RG8JAY2EJDBN";

// Read API Key

//1,2 e 3 são campos de canal thingspeak voce pode alterar o nome de acordo com nome que esta no seu canal.
const  int FieldNumber1 = 1;                                 // The field you wish to read
// The field you wish to read
const int FieldNumber2 = 2;
const int FieldNumber3 = 3;
const int FieldNumber4 = 4;
const int FieldNumber5 = 5;
const int FieldNumber6 = 6;
const int FieldNumber7 = 7;
const int FieldNumber8 = 8;

WiFiClient client;
//definição motores

int mtDF = 27;
int mtDT = 33;
int mtEF = 19;
int mtET = 4;
//BOTÕES
byte btn1 = 13;
byte btn2 = 23;
byte btn3 = 36;

//outras variaveis

byte F;
byte D;
byte E;
byte R;
byte RE5s;
byte VE;
byte VD;
byte FRENTE5s;

void setup() {
  ThingSpeak.begin(client);//Parte importante para inciar a transmissao de dados
  Serial.begin(115200);

  delay(100);
  pinMode(2, OUTPUT); //saida digital q aciona o LED
  pinMode(mtDF, OUTPUT);
  pinMode(mtDT, OUTPUT);//CONFIGURA MOTORES
  pinMode(mtEF, OUTPUT);
  pinMode(mtET, OUTPUT);

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP); //CONFIGURA BOTÕES
  pinMode(btn3, INPUT_PULLUP);

  digitalWrite(2, 0); // aqui garantimos que todos LEDS comecem desligados

  WiFi.mode(WIFI_STA);
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.println("");

//CONFIGURANDO WATCHDOG

    //hw_timer_t * timerBegin(uint8_t num, uint16_t divider, bool countUp)
    /*
       num: é a ordem do temporizador. Podemos ter quatro temporizadores, então a ordem pode ser [0,1,2,3].
      divider: É um prescaler (reduz a frequencia por fator). Para fazer um agendador de um segundo, 
      usaremos o divider como 80 (clock principal do ESP32 é 80MHz). Cada instante será T = 1/(80) = 1us
      countUp: True o contador será progressivo 
    */
    timer = timerBegin(0, 80, true); //timerID 0, div 80
    //timer, callback, interrupção de borda
    timerAttachInterrupt(timer, &resetModule, true);
    //timer, tempo (us), repetição
    timerAlarmWrite(timer,55000000, true);// SE O ESP32 FICAR PRESO EM ALGUM WHILE POR MAIS DE 55 SEGUNDOS WATCHDOG INICIA O ESP32 PORQUE ELE PODE TER SOFRIDO UM TRAVAMENTO
    timerAlarmEnable(timer); //habilita a interrupção

    //FIM CONFIG.

////ZERA TODOS CANAIS NA NUVEM DO THINGSPEAK
//  ThingSpeak.writeField(counterChannelNumber, 1, 0, myCounterReadAPIKey);
//    ThingSpeak.writeField(counterChannelNumber, 2, 0, myCounterReadAPIKey);
//      ThingSpeak.writeField(counterChannelNumber, 3, 0, myCounterReadAPIKey);
//        ThingSpeak.writeField(counterChannelNumber, 4, 0, myCounterReadAPIKey);
//          ThingSpeak.writeField(counterChannelNumber, 5, 0, myCounterReadAPIKey);
//            ThingSpeak.writeField(counterChannelNumber, 6, 0, myCounterReadAPIKey);
//              ThingSpeak.writeField(counterChannelNumber, 7, 0, myCounterReadAPIKey);
//                ThingSpeak.writeField(counterChannelNumber, 8, 0, myCounterReadAPIKey);

}

void loop() {

  btn1 = digitalRead(13);
  btn2 = digitalRead(23);
  btn3 = digitalRead(36);

 
  //RECEBE INFORMAÇÃO DO THINGSPEAK E ACENDE LED NA PORTA DOIS. RESERVA VARIAVEL VALOR RECEBIDO

  F = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
  D = ThingSpeak.readLongField(counterChannelNumber, FieldNumber2, myCounterReadAPIKey);
  E = ThingSpeak.readLongField(counterChannelNumber, FieldNumber3, myCounterReadAPIKey);
  R = ThingSpeak.readLongField(counterChannelNumber, FieldNumber4, myCounterReadAPIKey);
  RE5s = ThingSpeak.readLongField(counterChannelNumber, FieldNumber5, myCounterReadAPIKey);
  VE = ThingSpeak.readLongField(counterChannelNumber, FieldNumber6, myCounterReadAPIKey);
  VD = ThingSpeak.readLongField(counterChannelNumber, FieldNumber7, myCounterReadAPIKey);
FRENTE5s = ThingSpeak.readLongField(counterChannelNumber, FieldNumber8, myCounterReadAPIKey);

  Serial.println(F);
  digitalWrite(2, F);
 

  //CONDIÇÕES RECEBEDIAS DA NUVEM SÃO EXECUTADAS DE ACORDO COM A VARIAVEL F(FRENTE) POR 1,5 SEGUNDOS. APOS ISSO ENVIAR INFORMAÇÃO PARA NUVEM ZERANDO A VARIAVEL

  if (F == 1 )

  {

    Serial.print("frente");
    Serial.println(F);
    digitalWrite(2, F);

    digitalWrite(mtDF, HIGH);
    digitalWrite(mtDT, LOW);
    digitalWrite(mtEF, HIGH);
    digitalWrite(mtET, LOW);
    delay(1500);


    digitalWrite(mtDF, LOW);
    digitalWrite(mtDT, LOW);
    digitalWrite(mtEF, LOW);
    digitalWrite(mtET, LOW);
    delay(500);


    //ATUALIZANDO PARAMETROS DA NUVEM
    ThingSpeak.writeField(counterChannelNumber, 1, 0, myCounterReadAPIKey);

    Serial.println("aguardando atualizar nuvem");

    F = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 1, 0, myCounterReadAPIKey);

    F = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
    delay(10000);
        
    F = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 1, 0, myCounterReadAPIKey);
    while (F == 1)
      F = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 1, 0, myCounterReadAPIKey);

long tme = millis(); //tempo inicial do loop

    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog) 

    Serial.print("tempo passado dentro do loop (ms) = ");
    tme = millis() - tme; //calcula o tempo (atual - inicial)
    Serial.println(tme);


    {
      Serial.println("Nuvem atualizada");
      delay(50);
    }
  }

  //CONDIÇÕES RECEBEDIAS DA NUVEM SÃO EXECUTADAS DE ACORDO COM A VARIAVEL D(DIREITA) POR 1,5 SEGUNDOS. APOS ISSO ENVIAR INFORMAÇÃO PARA NUVEM ZERANDO A VARIAVEL


  else if (D == 1)

  { Serial.println("Direita");
    digitalWrite(2, D);


    digitalWrite(mtDF, 1);
    digitalWrite(mtDT, 0);
    digitalWrite(mtEF, 0);
    digitalWrite(mtET, 1);
    delay(1000);


    digitalWrite(mtDF, LOW);
    digitalWrite(mtDT, LOW);
    digitalWrite(mtEF, LOW);
    digitalWrite(mtET, LOW);
    delay(500);

    //ATUALIZANDO PARAMETROS DA NUVEM
    ThingSpeak.writeField(counterChannelNumber, 2, 0, myCounterReadAPIKey);

    Serial.println("aguardando atualizar nuvem");

    D = ThingSpeak.readLongField(counterChannelNumber, FieldNumber2, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 2, 0, myCounterReadAPIKey);

    D = ThingSpeak.readLongField(counterChannelNumber, FieldNumber2, myCounterReadAPIKey);
    delay(10000);

long tme = millis(); //tempo inicial do WATCHDOG

    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog) 

    Serial.print("tempo passado dentro do loop (ms) = ");
    tme = millis() - tme; //calcula o tempo (atual - inicial)
    Serial.println(tme);




    D = ThingSpeak.readLongField(counterChannelNumber, FieldNumber2, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 2, 0, myCounterReadAPIKey);
    while (D == 1)
      D = ThingSpeak.readLongField(counterChannelNumber, FieldNumber2, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 2, 0, myCounterReadAPIKey);
    {
      Serial.println("Nuvem atualizada");
      delay(50);
    }
  }

  //CONDIÇÕES RECEBEDIAS DA NUVEM SÃO EXECUTADAS DE ACORDO COM A VARIAVEL E(ESQUERDO) POR 1,5 SEGUNDOS. APOS I ENVIA INFORMAÇÃO PARA NUVEM ZERANDO A VARIAVEL
  else if (E == 1)

  { Serial.println("ESQUERDA");
    digitalWrite(2, E);
    delay(1000);
    digitalWrite(mtDF, 0);
    digitalWrite(mtDT, HIGH);
    digitalWrite(mtEF, HIGH);
    digitalWrite(mtET, 0);
    delay(1000);


    digitalWrite(mtDF, LOW);
    digitalWrite(mtDT, LOW);
    digitalWrite(mtEF, LOW);
    digitalWrite(mtET, LOW);
    delay(500);

    //ATUALIZANDO PARAMETROS DA NUVEM
    ThingSpeak.writeField(counterChannelNumber, 3, 0, myCounterReadAPIKey);

    Serial.println("aguardando atualizar nuvem");

    E = ThingSpeak.readLongField(counterChannelNumber, FieldNumber3, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 3, 0, myCounterReadAPIKey);

    E = ThingSpeak.readLongField(counterChannelNumber, FieldNumber3, myCounterReadAPIKey);
    delay(10000);

long tme = millis(); //tempo inicial do WATCHDOG

    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog) 

    Serial.print("tempo passado dentro do loop (ms) = ");
    tme = millis() - tme; //calcula o tempo (atual - inicial)
    Serial.println(tme);

    E = ThingSpeak.readLongField(counterChannelNumber, FieldNumber3, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 3, 0, myCounterReadAPIKey);
    while (E == 1)
      E = ThingSpeak.readLongField(counterChannelNumber, FieldNumber3, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 3, 0, myCounterReadAPIKey);
    {
      Serial.println("Nuvem atualizada");
      delay(50);
    }
  }
  //CONDIÇÕES RECEBEDIAS DA NUVEM SÃO EXECUTADAS DE ACORDO COM A VARIAVEL R(RÉ) POR 1,5 SEGUNDOS. APOS ISSO ENVIA INFORMAÇÃO PARA NUVEM ZERANDO A VARIAVEL




  else if (R == 1)

  { Serial.println("RÉ");
    digitalWrite(2, R);

    digitalWrite(mtDF, LOW);
    digitalWrite(mtDT, HIGH);
    digitalWrite(mtEF, LOW);
    digitalWrite(mtET, HIGH);
    delay(1500);

    digitalWrite(mtDF, LOW);
    digitalWrite(mtDT, LOW);
    digitalWrite(mtEF, LOW);
    digitalWrite(mtET, LOW);
    delay(500);
    //ATUALIZANDO PARAMETROS DA NUVEM
    ThingSpeak.writeField(counterChannelNumber, 4, 0, myCounterReadAPIKey);

    Serial.println("aguardando atualizar nuvem");

    R = ThingSpeak.readLongField(counterChannelNumber, FieldNumber4, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 4, 0, myCounterReadAPIKey);

    R = ThingSpeak.readLongField(counterChannelNumber, FieldNumber4, myCounterReadAPIKey);
    delay(10000);


long tme = millis(); //tempo inicial do WATCHDOG

    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog) 

    Serial.print("tempo passado dentro do loop (ms) = ");
    tme = millis() - tme; //calcula o tempo (atual - inicial)
    Serial.println(tme);


    R = ThingSpeak.readLongField(counterChannelNumber, FieldNumber4, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 4, 0, myCounterReadAPIKey);
    while (R == 1)
      R = ThingSpeak.readLongField(counterChannelNumber, FieldNumber4, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 4, 0, myCounterReadAPIKey);
    {
      Serial.println("Nuvem atualizada");
      delay(50);
    }
  }


  else if  (VD == 1) //SE USUARIO QUISER ANDAR ATÉ ENCONTRAR PAREDE E VIRAR PARA ESQUERDA ...ROVER FARA AÇÃO ABAIXO

{


  {
    Serial.println(" ANDAR ATE ENCONTRAR PAREDE E VIRAR ESQUERDA");
    digitalWrite(2, VD);
    delay(1000);
    btn1 = digitalRead(13);
    btn2 = digitalRead(23);
    btn3 = digitalRead(36);
Serial.println(btn1);
Serial.println(btn2);

    while (( btn2 != 1)|| (btn2 !=1))
    {       btn1 = digitalRead(13);
           btn2 = digitalRead(23); //
           btn3 = digitalRead(36);
Serial.println(btn1);
Serial.println(btn2);
  
    digitalWrite(mtDF, 1);
      digitalWrite(mtDT, 0);
      digitalWrite(mtEF, 1); //ANDA ATÉ ENCOTRAR PAREDE
      digitalWrite(mtET, 0);}
   
  {
      digitalWrite(mtDF, 0); //vai para trás
      digitalWrite(mtDT, 0);
      digitalWrite(mtEF, 0);
      digitalWrite(mtET, 0);
    }delay(500);
    {
      digitalWrite(mtDF, 0); //vai para trás
      digitalWrite(mtDT, 1);
      digitalWrite(mtEF, 0);
      digitalWrite(mtET, 1);
    }
    delay(600);

    {
      digitalWrite(mtDF, 1);  /// vai para frente
      digitalWrite(mtDT, 0);
      digitalWrite(mtEF, 1);
      digitalWrite(mtET, 0);
    }
    delay(700);

    {
      digitalWrite(mtDF, 0);
      digitalWrite(mtDT, 1);// vai ´para tras
      digitalWrite(mtEF, 0);
      digitalWrite(mtET, 1);
    }
    delay(900);

     {
      digitalWrite(mtDF, 0); //vai para trás
      digitalWrite(mtDT, 0);
      digitalWrite(mtEF, 0);
      digitalWrite(mtET, 0);
    }delay(500);

    {
      digitalWrite(mtDF, 1);
      digitalWrite(mtDT, 0);
      digitalWrite(mtEF, 0);// vira esquerda
      digitalWrite(mtET, 1);
      delay(1000);

    }
    digitalWrite(mtDF, LOW);
    digitalWrite(mtDT, LOW);
    digitalWrite(mtEF, LOW);  //pára
    digitalWrite(mtET, LOW);
    delay(500);
  }




  ThingSpeak.writeField(counterChannelNumber, 7, 0, myCounterReadAPIKey);

  Serial.println("aguardando atualizar nuvem");

  VD = ThingSpeak.readLongField(counterChannelNumber, FieldNumber7, myCounterReadAPIKey);
  ThingSpeak.writeField(counterChannelNumber, 7, 0, myCounterReadAPIKey);

  VD = ThingSpeak.readLongField(counterChannelNumber, FieldNumber7, myCounterReadAPIKey);
  delay(10000);

long tme = millis(); //tempo inicial do WATCHDOG

    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog) 

    Serial.print("tempo passado dentro do loop (ms) = ");
    tme = millis() - tme; //calcula o tempo (atual - inicial)
    Serial.println(tme);

  VD = ThingSpeak.readLongField(counterChannelNumber, FieldNumber7, myCounterReadAPIKey);
  ThingSpeak.writeField(counterChannelNumber, 7, 0, myCounterReadAPIKey);
  while (VD == 1)
    VD = ThingSpeak.readLongField(counterChannelNumber, FieldNumber7, myCounterReadAPIKey);
  ThingSpeak.writeField(counterChannelNumber, 7, 0, myCounterReadAPIKey);
  {
    Serial.println("Nuvem atualizada");
    delay(50);
 
  }


 
}

  else if  (VE == 1) //SE USUARIO QUISER ANDAR ATÉ ENCONTRAR PAREDE E VIRAR PARA ESQUERDA ...ROVER FARA AÇÃO ABAIXO

{


  {
    Serial.println(" ANDAR ATE ENCONTRAR PAREDE E VIRAR ESQUERDA");
    digitalWrite(2, VE);
    delay(1000);
    btn1 = digitalRead(13);
    btn2 = digitalRead(23);
    btn3 = digitalRead(36);
Serial.println(btn1);
Serial.println(btn2);

    while (( btn2 != 1)|| (btn2 !=1))
    {       btn1 = digitalRead(13);
           btn2 = digitalRead(23); //
           btn3 = digitalRead(36);
Serial.println(btn1);
Serial.println(btn2);
  
    digitalWrite(mtDF, 1);
      digitalWrite(mtDT, 0);
      digitalWrite(mtEF, 1); //ANDA ATÉ ENCOTRAR PAREDE
      digitalWrite(mtET, 0);}
   
  {
      digitalWrite(mtDF, 0); //vai para trás
      digitalWrite(mtDT, 0);
      digitalWrite(mtEF, 0);
      digitalWrite(mtET, 0);
    }delay(500);
    {
      digitalWrite(mtDF, 0); //vai para trás
      digitalWrite(mtDT, 1);
      digitalWrite(mtEF, 0);
      digitalWrite(mtET, 1);
    }
    delay(600);

    {
      digitalWrite(mtDF, 1);  /// vai para frente
      digitalWrite(mtDT, 0);
      digitalWrite(mtEF, 1);
      digitalWrite(mtET, 0);
    }
    delay(700);

    {
      digitalWrite(mtDF, 0);
      digitalWrite(mtDT, 1);// vai ´para tras
      digitalWrite(mtEF, 0);
      digitalWrite(mtET, 1);
    }
    delay(900);

     {
      digitalWrite(mtDF, 0); //vai para trás
      digitalWrite(mtDT, 0);
      digitalWrite(mtEF, 0);
      digitalWrite(mtET, 0);
    }delay(500);

    {
      digitalWrite(mtDF, 0);
      digitalWrite(mtDT, HIGH);
      digitalWrite(mtEF, HIGH);// vira esquerda
      digitalWrite(mtET, 0);
      delay(1000);

    }
    digitalWrite(mtDF, LOW);
    digitalWrite(mtDT, LOW);
    digitalWrite(mtEF, LOW);  //pára
    digitalWrite(mtET, LOW);
    delay(500);
  }




  ThingSpeak.writeField(counterChannelNumber, 6, 0, myCounterReadAPIKey);

  Serial.println("aguardando atualizar nuvem");

  VE = ThingSpeak.readLongField(counterChannelNumber, FieldNumber6, myCounterReadAPIKey);
  ThingSpeak.writeField(counterChannelNumber, 6, 0, myCounterReadAPIKey);

  VE = ThingSpeak.readLongField(counterChannelNumber, FieldNumber6, myCounterReadAPIKey);
  delay(10000);

long tme = millis(); //tempo inicial do WATCHDOG

    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog) 

    Serial.print("tempo passado dentro do loop (ms) = ");
    tme = millis() - tme; //calcula o tempo (atual - inicial)
    Serial.println(tme);


  VE = ThingSpeak.readLongField(counterChannelNumber, FieldNumber6, myCounterReadAPIKey);
  ThingSpeak.writeField(counterChannelNumber, 6, 0, myCounterReadAPIKey);
  while (VE == 1)
    VE = ThingSpeak.readLongField(counterChannelNumber, FieldNumber6, myCounterReadAPIKey);
  ThingSpeak.writeField(counterChannelNumber, 6, 0, myCounterReadAPIKey);
  {
    Serial.println("Nuvem atualizada");
    delay(50);
    Serial.println("presa nesse caraio");
  }


 
}

if (FRENTE5s == 1 )

  {

    Serial.print("frente por 5 segundos");
    Serial.println(FRENTE5s);
    digitalWrite(2, FRENTE5s);

    digitalWrite(mtDF, HIGH);
    digitalWrite(mtDT, LOW);
    digitalWrite(mtEF, HIGH);
    digitalWrite(mtET, LOW);
    delay(5000);


    digitalWrite(mtDF, LOW);
    digitalWrite(mtDT, LOW);
    digitalWrite(mtEF, LOW);
    digitalWrite(mtET, LOW);
    delay(500);


    //ATUALIZANDO PARAMETROS DA NUVEM
    ThingSpeak.writeField(counterChannelNumber, 8, 0, myCounterReadAPIKey);

    Serial.println("aguardando atualizar nuvem");

    FRENTE5s = ThingSpeak.readLongField(counterChannelNumber, FieldNumber8, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 8, 0, myCounterReadAPIKey);

    FRENTE5s = ThingSpeak.readLongField(counterChannelNumber, FieldNumber8, myCounterReadAPIKey);
    delay(10000);

long tme = millis(); //tempo inicial do WATCHDOG

    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog) 

    Serial.print("tempo passado dentro do loop (ms) = ");
    tme = millis() - tme; //calcula o tempo (atual - inicial)
    Serial.println(tme);

    FRENTE5s= ThingSpeak.readLongField(counterChannelNumber, FieldNumber8, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 8, 0, myCounterReadAPIKey);
    while (FRENTE5s == 1)
     FRENTE5s = ThingSpeak.readLongField(counterChannelNumber, FieldNumber8, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 8, 0, myCounterReadAPIKey);
    {
      Serial.println("Nuvem atualizada");
      delay(50);
    }
  }


if (RE5s == 1 )

  {

    Serial.print("RÉ por 5 segundos");
    Serial.println(RE5s);
    digitalWrite(2, RE5s);

    digitalWrite(mtDF, 0);
    digitalWrite(mtDT, 1);
    digitalWrite(mtEF, 0);
    digitalWrite(mtET, 1);
    delay(5000);


    digitalWrite(mtDF, LOW);
    digitalWrite(mtDT, LOW);
    digitalWrite(mtEF, LOW);
    digitalWrite(mtET, LOW);
    delay(500);


    //ATUALIZANDO PARAMETROS DA NUVEM
    ThingSpeak.writeField(counterChannelNumber, 5, 0, myCounterReadAPIKey);

    Serial.println("aguardando atualizar nuvem");

    FRENTE5s = ThingSpeak.readLongField(counterChannelNumber, FieldNumber5, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 5, 0, myCounterReadAPIKey);

    FRENTE5s = ThingSpeak.readLongField(counterChannelNumber, FieldNumber5, myCounterReadAPIKey);
    delay(10000);

long tme = millis(); //tempo inicial do WATCHDOG

    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog) 

    Serial.print("tempo passado dentro do loop (ms) = ");
    tme = millis() - tme; //calcula o tempo (atual - inicial)
    Serial.println(tme);

    FRENTE5s= ThingSpeak.readLongField(counterChannelNumber, FieldNumber5, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 5, 0, myCounterReadAPIKey);
    while (FRENTE5s == 1)
     FRENTE5s = ThingSpeak.readLongField(counterChannelNumber, FieldNumber5, myCounterReadAPIKey);
    ThingSpeak.writeField(counterChannelNumber, 5, 0, myCounterReadAPIKey);
    {
      Serial.println("Nuvem atualizada");
      delay(50);
    }
  }
}
