// Webserver Proyecto 3 Parqueo 
// Gabriel Carrera 21216 y Luis Pedro González 21513

#include <LiquidCrystal.h> //Librería para la pantalla LCD

//Definicion de pines de la pantalla 16x2 (8 bits)
#define RS 34
#define EN 35
//#define D0 32
//#define D1 33
//#define D2 25
//#define D3 26
#define D4 27
#define D5 14
#define D6 12
#define D7 13

char tiva1; //Variable para recibir del serial de la tiva 1
char tiva2; //Variable para recibir del serial de la tiva 2

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); //Definimos el objeto para la librería LCD y asignamos los pines

//Variables booleanas para los parqueos
bool parqueo1 = false;
bool parqueo2 = false;
bool parqueo3 = false;
bool parqueo4 = false;
bool parqueo5 = false;
bool parqueo6 = false;
bool parqueo7 = false;
bool parqueo8 = false;

//Variables enteras para los totales de los parqueos
uint8_t s1;
uint8_t s2;
uint8_t s3;
uint8_t s4;
uint8_t s5;
uint8_t s6;
uint8_t s7;
uint8_t s8;
uint8_t sotano1;
uint8_t sotano2;
uint8_t available;


void setup(void) {
  Serial.begin(115200); //Iniciar comunicación serial para ver en la terminal
  Serial1.begin(115200, SERIAL_8N1, 32, 33, false, 20000UL, 112); //Habilitar el UART1 y cambiar los pines
  Serial2.begin(115200); //Habilitar el UART2
  lcd.begin(16, 2); //Setup de la LCD de 16x2
  lcd.setCursor(0,0); //Cursor en 0,0
  lcd.print("Parqueomatic"); //Escribir parqueomatic
  lcd.setCursor(12,0); //Cursor 12,0
  lcd.print("P:"); //Escribir p de parqueos totales
  lcd.setCursor(0,1); //Cursor en 0,1
  lcd.print("S1:"); //Escribir S1
  lcd.setCursor(8,1); //Cursor 8,1
  lcd.print("S2:"); //Escribir S2
  Serial.println("");
}

void loop(void) {
  check_tiva1(); //Llamar a función para revisar los parqueos de la tiva 1
  check(); //Llamar para poder realizar suma de los parqueos
  sotano1 = s1 + s2 + s3 + s4; //Suma de parqueos en sotano 1
  available = s1 + s2 + s3 + s4; //Suma de disponible totales
  lcd.setCursor(13, 0); //Cursor en 13,0
  lcd.print(String(available)); //Mostrar parqueos totales available
  lcd.setCursor(3,1); //Cursor en 3,1
  lcd.print(String(sotano1)); //Mostrar los parqueos available en el sotano 1
}

//Función para chequear la información de la Tiva1
void check_tiva1(){
  tiva1 = Serial2.read(); //Leer de UART2

  //Chequear para los parqueos
  if (tiva1 == 0){
    parqueo1 = false;
    parqueo2 = false;
    parqueo3 = false;
    parqueo4 = false;
  }
  else if (tiva1 == 1){
    parqueo1 = true;
    parqueo2 = false;
    parqueo3 = false;
    parqueo4 = false;
  }
  else if (tiva1 == 2){
    parqueo1 = false;
    parqueo2 = true;
    parqueo3 = false;
    parqueo4 = false;
  }
  else if (tiva1 == 3){
    parqueo1 = true;
    parqueo2 = true;
    parqueo3 = false;
    parqueo4 = false;
  }
  else if (tiva1 == 4){
    parqueo1 = false;
    parqueo2 = false;
    parqueo3 = true;
    parqueo4 = false;
  }
  else if (tiva1 == 5){
    parqueo1 = true;
    parqueo2 = false;
    parqueo3 = true;
    parqueo4 = false;
  }
  else if (tiva1 == 6){
    parqueo1 = false;
    parqueo2 = true;
    parqueo3 = true;
    parqueo4 = false;
  }
  else if (tiva1 == 7){
    parqueo1 = true;
    parqueo2 = true;
    parqueo3 = true;
    parqueo4 = false;
  }
  else if (tiva1 == 8){
    parqueo1 = false;
    parqueo2 = false;
    parqueo3 = false;
    parqueo4 = true;
  }
  else if (tiva1 == 9){
    parqueo1 = true;
    parqueo2 = false;
    parqueo3 = false;
    parqueo4 = true;
  }
  else if (tiva1 == 10){
    parqueo1 = false;
    parqueo2 = true;
    parqueo3 = false;
    parqueo4 = true;
  }
  else if (tiva1 == 11){
    parqueo1 = true;
    parqueo2 = true;
    parqueo3 = false;
    parqueo4 = true;
  }
  else if (tiva1 == 12){
    parqueo1 = false;
    parqueo2 = false;
    parqueo3 = true;
    parqueo4 = true;
  }
  else if (tiva1 == 13){
    parqueo1 = true;
    parqueo2 = false;
    parqueo3 = true;
    parqueo4 = true;
  }
  else if (tiva1 == 14){
    parqueo1 = false;
    parqueo2 = true;
    parqueo3 = true;
    parqueo4 = true;
  }
  else if (tiva1 == 15){
    parqueo1 = true;
    parqueo2 = true;
    parqueo3 = true;
    parqueo4 = true;
  }
}


//Función para las sumas
void check(){
  //Revisar si están ocupados los parqueos
  if (parqueo1 == true){
    s1 = 1;
  }
  else{
    s1 = 0;
  }
  if (parqueo2 == true){
    s2 = 1;
  }
  else{
    s2 = 0;
  }
  if (parqueo3 == true){
    s3 = 1;
  }
  else{
    s3 = 0;
  }
  if (parqueo4 == true){
    s4 = 1;
  }
  else{
    s4 = 0;
  }
}