/* ---------------------------------------------------
 este sketch prende y apaga un pin con los valores
 definidos en timerOn y timerOff
 y si se conecta por BT y 
 se manda un comando 'h' por terminal (SIN LF o CR)
 devuelve la lista de comandos validos. 
 
 Configuracion del BT HC-05:
 NAME: GONZABT
 PSWD: 1234
 UART 38400,0,0
 ROLE: 0 (SLAVE)
------------------------------------------------------*/

#include <Arduino.h>
#include <BlockNot.h> //libreria de timers

#include <SoftwareSerial.h> //para manejar el BT
SoftwareSerial BT1(10, 11); // RX | TX

const int timerOn = 10000;  //tiempo que el pin permanece prendido (en HIGH)
const int timerOff = 5000; //tiempo que el pin permanece apagado (en LOW)
int contador=0;
BlockNot bombaTimerON(timerOn, GLOBAL_RESET);
BlockNot bombaTimerOFF(timerOff);

const int bomba_PIN = 13;

bool bombaON = false;

bool bombaIsOff() { return !bombaON; }

bool bombaIsOn() { return bombaON; }

void turnbombaOn() {
    digitalWrite(bomba_PIN, LOW);
    bombaTimerON.RESET;
    bombaON = true;
    contador ++;
}

void turnbombaOff() {
    digitalWrite(bomba_PIN, HIGH);
    bombaTimerOFF.RESET;
    bombaON = false;
}

void listaComandos() {
  Serial.println("-------------------------");
  Serial.println("Lista de comandos");
  Serial.println("h => lista de comandos ");
  Serial.println("c => contador de veces que se encendió la bomba desde que está prendido ");
  Serial.println("t => tiempo de encendido o reseteo en minutos del Arduino");
// Serial.println(" =>  ");
  Serial.println("-------------------------");
  BT1.println("-------------------------");
  BT1.println("Lista de comandos");
  BT1.println("h => lista de comandos ");
  BT1.println("c => contador de veces que se encendió la bomba desde que está prendido ");
  BT1.println("t => tiempo de encendido o reseteo en minutos del Arduino");
// BT1.println(" =>  ");        
  BT1.println("-------------------------");

}

void setup() {
    BT1.begin(38400);
    Serial.begin(9600);
    pinMode(bomba_PIN, OUTPUT);
    resetAllTimers();
    turnbombaOn();    
    Serial.println(F("\nREADY!\n"));
}

void loop() {

    if (bombaTimerON.TRIGGERED && bombaIsOn()) {    //apaga la bomba
        turnbombaOff();        
    }

    if (bombaTimerOFF.TRIGGERED && bombaIsOff()) {  //enciende la bomba
        turnbombaOn();
        Serial.println("bomba On");
        Serial.print("Desde enciende la bomba>contador :");
        Serial.println(contador);
    }
    BT1.listen();
    if (BT1.available()) {                          //lee BT y envia a Arduino             
        char myChar = BT1.read();        
        Serial.println("-------------------------");
        Serial.print ("valor de myChar a testear> ");       
        Serial.println (myChar);
        Serial.println("-------------------------");
        if (myChar == 'h') listaComandos();
        else if (myChar == 'c'){
          Serial.println("-------------------------");
          Serial.print("respuesta a comando c> ");
          Serial.print("contador: ");
          Serial.println(contador);
          Serial.println("-------------------------");
          BT1.println("-------------------------");
          BT1.print("respuesta a comando c> ");
          BT1.print("contador: ");
          BT1.println(contador);          
          BT1.println("-------------------------");
          }
        else if (myChar == 't'){
          int tiempoEncendido = millis()/1000/60;          
          
          Serial.println("-------------------------");
          Serial.print("respuesta a comando t> ");
          Serial.print("tiempo de encendido en minutos: ");
          Serial.println(tiempoEncendido);
          Serial.println("-------------------------");
          BT1.println("-------------------------");
          BT1.print("respuesta a comando t> ");
          BT1.print("tiempo de encendido en minutos: ");
          BT1.println(tiempoEncendido);          
          BT1.println("-------------------------");
          }
          else {
            Serial.println("-------------------------");
            Serial.println("no escribió un comando valido");
            Serial.println("'h' para lista de comandos'");
            Serial.println("-------------------------");            
            BT1.println("-------------------------");
            BT1.println("no escribió un comando valido");
            BT1.println("'h' para lista de comandos'");
            BT1.println("-------------------------");
            };
      }
     
     if (Serial.available()) {
       char myChar = Serial.read();                      //lee Arduino y envia a BT
        BT1.println("-------------------------");
        BT1.print("Desde Serial Monitor> ");
        BT1.println(myChar);        
        BT1.println("-------------------------");
    }  
}
