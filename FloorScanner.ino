//biblioteca responsável pela comunicação com o Cartão SD
#include "SD.h"
#include "ESP8266WiFi.h"
 
//pino ligado ao CS do módulo SD Card
#define CS_PIN  D8
#define Button1_Pin D1
#define Button2_Pin D2

int button1 = 0;
int button2 = 0;

File dataFile = SD.open("LOG.txt", FILE_WRITE);
  
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  pinMode(Button1_Pin, INPUT);
  pinMode(Button2_Pin, INPUT);
  
  Serial.begin(9600);
  Serial.println("Inicializando o cartão SD...");

  // verifica se o cartão SD está presente e se pode ser inicializado
  if (!SD.begin(CS_PIN)) {
    Serial.println("Falha, verifique se o cartão está presente.");
    //programa encerrrado
    return;
  }
  
  //se chegou aqui é porque o cartão foi inicializado corretamente  
  Serial.println("Cartão inicializado.");

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
  digitalWrite(LED_BUILTIN, LOW);
  
}

void wifiscan(){
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    dataFile.print(n);
    dataFile.println(" networks found");
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    for (int i = 0; i < n; ++i) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      // Print SSID and RSSI for each network found
      dataFile.print(i + 1);
      dataFile.print(": ");
      dataFile.print(WiFi.SSID(i));
      dataFile.print(" (");
      dataFile.print(WiFi.RSSI(i));
      dataFile.print(")");
      dataFile.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop(){
  button1 = digitalRead(Button1_Pin);
  if(button1 == 1){
    
    while(button2 == LOW){
        File dataFile = SD.open("LOG.txt", FILE_WRITE);
    // se o arquivo foi aberto corretamente, escreve os dados nele
    if (dataFile) {
        wifiscan();
    }
    // se o arquivo não pôde ser aberto os dados não serão gravados.
    else {
        Serial.println("Falha ao abrir o arquivo LOG.txt");
    }

    //intervalo de espera para uma nova leitura dos dados.
    delay(2000);
    button2 = digitalRead(Button2_Pin);
   }//while

  if(button2 == HIGH){
    dataFile.close();
    while(button2 == HIGH){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
   }//if
  
  button2 = digitalRead(Button2_Pin);
  }
}
