// Webserver Proyecto 3 Parqueo 
// Gabriel Carrera 21216 y Luis Pedro González 21513

#include <WiFi.h> //Librería para manejar el Wifi
#include <WiFiClient.h> //Librería para manejar Clientes
#include <WebServer.h> //Librería para manejar el servidor
#include <ESPmDNS.h> //Librería para asegurarse que la IP no cambie
#include <LiquidCrystal.h> //Librería para la pantalla LCD

//Definicion de pines de la pantalla 16x2 (8 bits)
#define RS 25
#define EN 26
//#define D0 32
//#define D1 33
//#define D2 25
//#define D3 26
#define D4 27
#define D5 14
#define D6 19
#define D7 18

const char* ssid = "ParqueomaticUVG"; //Nombre de la red
const char* password = "xPowerr_"; //Contraseña
char tiva1; //Variable para recibir del serial de la tiva 1disponibles
char tiva2; //Variable para recibir del serial de la tiva 2

WebServer server(80); //Definimos objeto para la librería Webserver en el puerto 80 ya con ese hace el request de http
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); //Definimos el objeto para la librería LCD y asignamos los pines

//Variables booleanas para los parqueos
bool spot1 = false;
bool spot2 = false;
bool spot3 = false;
bool spot4 = false;
bool spot5 = false;
bool spot6 = false;
bool spot7 = false;
bool spot8 = false;


//Variables enteras para los totales de los parqueos
uint8_t s1;
uint8_t s2;
uint8_t s3;
uint8_t s4;
uint8_t s5;
uint8_t s6;
uint8_t s7;
uint8_t s8;
uint8_t parqueoA;
uint8_t parqueoB;
uint8_t available;

//Función para que cuando se este en la pagina principal se ejecute la siguiente función
void handleRoot() {
  server.send(200, "text/html", HTML()); //Código de la página web
}

//Función que llama javascript para que cuando se este en la pagina principal se ejecute la siguiente función
void handleColor1(){
  if (spot1 == false) {
    server.send(200, "text/plain", "parking-spot vacant"); //Si el parqueo esta vacío enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  } else {
    server.send(200, "text/plain", "parking-spot occupied"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  }
}

void handleColor2(){
  if (spot2 == false) {
    server.send(200, "text/plain", "parking-spot vacant"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  } else {
    server.send(200, "text/plain", "parking-spot occupied"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  }
}

void handleColor3(){
  if (spot3 == false) {
    server.send(200, "text/plain", "parking-spot vacant"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  } else {
    server.send(200, "text/plain", "parking-spot occupied"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  }
}

void handleColor4(){
  if (spot4 == false) {
    server.send(200, "text/plain", "parking-spot vacant"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  } else {
    server.send(200, "text/plain", "parking-spot occupied"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  }
}

void handleColor5(){
  if (spot5 == false) {
    server.send(200, "text/plain", "parking-spot vacant"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  } else {
    server.send(200, "text/plain", "parking-spot occupied"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  }
}

void handleColor6(){
  if (spot6 == false) {
    server.send(200, "text/plain", "parking-spot vacant"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  } else {
    server.send(200, "text/plain", "parking-spot occupied"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  }
}

void handleColor7(){
  if (spot7 == false) {
    server.send(200, "text/plain", "parking-spot vacant"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  } else {
    server.send(200, "text/plain", "parking-spot occupied"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  }
}

void handleColor8(){
  if (spot8 == false) {
    server.send(200, "text/plain", "parking-spot vacant"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  } else {
    server.send(200, "text/plain", "parking-spot occupied"); //Si el parqueo esta ocupado enviar un texto a javascript con la clase de css en HTML para asignarle al id correspondiente
  }
}

//Funcion a llamar si el cliente escribe una direccion que no existe
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200); //Iniciar comunicación serial para ver en la terminal
  Serial1.begin(115200, SERIAL_8N1, 32, 33, false, 20000UL, 112); //Habilitar el UART1 y cambiar los pins
  Serial2.begin(115200); //Habilitar el UART2
  lcd.begin(16, 2); //Setup de la LCD de 16x2
  lcd.setCursor(0,0); //Cursor en 0,0
  lcd.print("Parqueomatic UVG"); //Escribir parqueomatic
  lcd.setCursor(0,1); //Cursor en 0,1
  lcd.print("Disp:"); //Escribir S1
  lcd.setCursor(7,1); //Cursor en 0,1
  lcd.print("PA:"); //Escribir PA
  lcd.setCursor(12,1); //Cursor 8,1
  lcd.print("PB:"); //Escribir PB
  WiFi.mode(WIFI_STA); //Wifi como station mode para unirse a una red que ya existe
  WiFi.begin(ssid, password); //Iniciar la conexión 
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //Escribir la IP para acceder a la página

  //Iniciar multi DNS
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  //Definir las funciones a llamar cuando se haga el request de ciertas direcciones
  server.on("/", handleRoot);
  server.on("/l1", handleColor1);
  server.on("/l2", handleColor2);
  server.on("/l3", handleColor3);
  server.on("/l4", handleColor4);
  server.on("/l5", handleColor5);
  server.on("/l6", handleColor6);
  server.on("/l7", handleColor7);
  server.on("/l8", handleColor8);

  server.onNotFound(handleNotFound); //Cuando se pida una direccion inexistente llamar a la funcion indicada

  server.begin(); //Iniciar el servidor
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient(); //Mantener el servidor 
  check_tiva1(); //Llamar a función para revisar los parqueos de la tiva 1
  check_tiva2(); //Llamar a función para revisar los parqueos de la tiva 2
  check(); //Llamar para poder realizar suma de los parqueos
  parqueoA = s1 + s2 + s3 + s4; //Suma de parqueos en sotano 1
  parqueoB = s5 + s6 + s7 + s8; //Suma de parqueos en sotano 2
  available = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8; //Suma de available totales
  lcd.setCursor(5, 1); //Cursor en 13,0
  lcd.print(String(available)); //Mostrar parqueos totales dispnibles
  lcd.setCursor(10,1); //Cursor en 3,1
  lcd.print(String(parqueoA)); //Mostrar los parqueos dispnibles en el sotano 1
  lcd.setCursor(15,1); //Cursor en 12,1
  lcd.print(String(parqueoB)); //Mostrar los parqueos available en el sotano 2
}

//Código HTML, CSS y Javascript para la página WEB
String HTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<meta charset=\"UTF-8\">\n";
  ptr +="<title>PARQUEO</title>\n";
  ptr +="  <style>\n";
  ptr +="    body {\n";
  ptr +="      font-family: Arial, sans-serif;\n";
  ptr +="      background-color: #FFFFFF;\n";
  ptr +="      color: #000000;\n";
  ptr +="      margin: 0;\n";
  ptr +="      padding: 20px;\n";
  ptr +="    }\n";
  ptr +="    h1, h2 {\n";
  ptr +="      text-align: center;\n";
  ptr +="    }\n";
  ptr +="    .parking-lot {\n";
  ptr +="      display: flex;\n";
  ptr +="      justify-content: center;\n";
  ptr +="      flex-wrap: wrap;\n";
  ptr +="    }\n";
  ptr +="    .parking-spot {\n";
  ptr +="      width: 22%;\n";
  ptr +="      height: 320px;\n";
  ptr +="      border-radius: 20px;\n";
  ptr +="      margin: 20px;\n";
  ptr +="      text-align: center;\n";
  ptr +="      display: flex;\n";
  ptr +="      align-items: center;\n";
  ptr +="      justify-content: center;\n";
  ptr +="      font-size: 26px;\n";
  ptr +="      font-weight: bold;\n";
  ptr +="      cursor: pointer;\n";
  ptr +="      transition: background-color 0.3s ease;\n";
  ptr +="    }\n";
  ptr +="    .vacant {\n";
  ptr +="      background-color: #FF0000;\n";
  ptr +="    }\n";
  ptr +="    .occupied {\n";
  ptr +="      background-color: #30FF24;\n";
  ptr +="    }\n";
  ptr +="  </style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="  <h1>Parqueo-matic</h1>\n";
  ptr +="<title>PARQUEO A</title>\n";
  ptr +="  <h2>PARQUEO A</h2>\n";
  ptr +="  <div class=\"parking-lot\">\n";
  ptr +="<div id=\"spot1\" class=\"parking-spot\">PA1 &#128664;</div>\n";
  ptr +="<div id=\"spot2\" class=\"parking-spot\">PA2 &#128664;</div>\n";
  ptr +="<div id=\"spot3\" class=\"parking-spot\">PA3 &#128664;</div>\n";
  ptr +="<div id=\"spot4\" class=\"parking-spot\">PA4 &#128664;</div>\n";
  ptr +="  </div>\n";
  ptr +="<title>PARQUEO B</title>\n";
  ptr +="  <h2>PARQUEO B</h2>\n";
  ptr +="  <div class=\"parking-lot\">\n";
  ptr +="<div id=\"spot5\" class=\"parking-spot\">PB1 &#128664;</div>\n";
  ptr +="<div id=\"spot6\" class=\"parking-spot\">PB2 &#128664;</div>\n";
  ptr +="<div id=\"spot7\" class=\"parking-spot\">PB3 &#128664;</div>\n";
  ptr +="<div id=\"spot8\" class=\"parking-spot\">PB4 &#128664;</div>\n";
  ptr +="  </div>\n";
  ptr +="<script>\n";
  ptr +="function updateColor1() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById('spot1').className = this.responseText;\n";
  ptr +="}\n";
  ptr +="};\n";
  ptr +="xhttp.open('GET', '/l1', true);\n";
  ptr +="xhttp.send();\n";
  ptr +="setTimeout(updateColor1, 500);}\n";
  ptr +="function updateColor2() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById('spot2').className = this.responseText;\n";
  ptr +="}\n";
  ptr +="};\n";
  ptr +="xhttp.open('GET', '/l2', true);\n";
  ptr +="xhttp.send();\n";
  ptr +="setTimeout(updateColor2, 500);}\n";
  ptr +="function updateColor3() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById('spot3').className = this.responseText;\n";
  ptr +="}\n";
  ptr +="};\n";
  ptr +="xhttp.open('GET', '/l3', true);\n";
  ptr +="xhttp.send();\n";
  ptr +="setTimeout(updateColor3, 500);}\n";
  ptr +="function updateColor4() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById('spot4').className = this.responseText;\n";
  ptr +="}\n";
  ptr +="};\n";
  ptr +="xhttp.open('GET', '/l4', true);\n";
  ptr +="xhttp.send();\n";
  ptr +="setTimeout(updateColor4, 500);}\n";
  ptr +="function updateColor5() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById('spot5').className = this.responseText;\n";
  ptr +="}\n";
  ptr +="};\n";
  ptr +="xhttp.open('GET', '/l5', true);\n";
  ptr +="xhttp.send();\n";
  ptr +="setTimeout(updateColor5, 500);}\n";
  ptr +="function updateColor6() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById('spot6').className = this.responseText;\n";
  ptr +="}\n";
  ptr +="};\n";
  ptr +="xhttp.open('GET', '/l6', true);\n";
  ptr +="xhttp.send();\n";
  ptr +="setTimeout(updateColor6, 500);}\n";
  ptr +="function updateColor7() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById('spot7').className = this.responseText;\n";
  ptr +="}\n";
  ptr +="};\n";
  ptr +="xhttp.open('GET', '/l7', true);\n";
  ptr +="xhttp.send();\n";
  ptr +="setTimeout(updateColor7, 500);}\n";
  ptr +="function updateColor8() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById('spot8').className = this.responseText;\n";
  ptr +="}\n";
  ptr +="};\n";
  ptr +="xhttp.open('GET', '/l8', true);\n";
  ptr +="xhttp.send();\n";
  ptr +="setTimeout(updateColor8, 500);}\n";
  ptr +="updateColor1();\n";
  ptr +="updateColor2();\n";
  ptr +="updateColor3();\n";
  ptr +="updateColor4();\n";
  ptr +="updateColor5();\n";
  ptr +="updateColor6();\n";
  ptr +="updateColor7();\n";
  ptr +="updateColor8();\n";
  ptr +="</script>\n";
  ptr +="</body>\n";
  ptr +="</html>";
  return ptr;
}

//Función para chequear la información de la Tiva1
void check_tiva1(){
  tiva1 = Serial2.read(); //Leer de UART2

  //Chequear para los parqueos
  if (tiva1 == 15){
    spot1 = false;
    spot2 = false;
    spot3 = false;
    spot4 = false;
  }
  else if (tiva1 == 14){
    spot1 = true;
    spot2 = false;
    spot3 = false;
    spot4 = false;
  }
  else if (tiva1 == 13){
    spot1 = false;
    spot2 = true;
    spot3 = false;
    spot4 = false;
  }
  else if (tiva1 == 12){
    spot1 = true;
    spot2 = true;
    spot3 = false;
    spot4 = false;
  }
  else if (tiva1 == 11){
    spot1 = false;
    spot2 = false;
    spot3 = true;
    spot4 = false;
  }
  else if (tiva1 == 10){
    spot1 = true;
    spot2 = false;
    spot3 = true;
    spot4 = false;
  }
  else if (tiva1 == 9){
    spot1 = false;
    spot2 = true;
    spot3 = true;
    spot4 = false;
  }
  else if (tiva1 == 8){
    spot1 = true;
    spot2 = true;
    spot3 = true;
    spot4 = false;
  }
  else if (tiva1 == 7){
    spot1 = false;
    spot2 = false;
    spot3 = false;
    spot4 = true;
  }
  else if (tiva1 == 6){
    spot1 = true;
    spot2 = false;
    spot3 = false;
    spot4 = true;
  }
  else if (tiva1 == 5){
    spot1 = false;
    spot2 = true;
    spot3 = false;
    spot4 = true;
  }
  else if (tiva1 == 4){
    spot1 = true;
    spot2 = true;
    spot3 = false;
    spot4 = true;
  }
  else if (tiva1 == 3){
    spot1 = false;
    spot2 = false;
    spot3 = true;
    spot4 = true;
  }
  else if (tiva1 == 2){
    spot1 = true;
    spot2 = false;
    spot3 = true;
    spot4 = true;
  }
  else if (tiva1 == 1){
    spot1 = false;
    spot2 = true;
    spot3 = true;
    spot4 = true;
  }
  else if (tiva1 == 0){
    spot1 = true;
    spot2 = true;
    spot3 = true;
    spot4 = true;
  }
}

void check_tiva2(){
  tiva2 = Serial1.read(); //Leer del UART1

  //Chequear para los parqueos
  if (tiva2 == 15){
    spot5 = false;
    spot6 = false;
    spot7 = false;
    spot8 = false;
  }
  else if (tiva2 == 14){
    spot5 = true;
    spot6 = false;
    spot7 = false;
    spot8 = false;
  }
  else if (tiva2 == 13){
    spot5 = false;
    spot6 = true;
    spot7 = false;
    spot8 = false;
  }
  else if (tiva2 == 12){
    spot5 = true;
    spot6 = true;
    spot7 = false;
    spot8 = false;
  }
  else if (tiva2 == 11){
    spot5 = false;
    spot6 = false;
    spot7 = true;
    spot8 = false;
  }
  else if (tiva2 == 10){
    spot5 = true;
    spot6 = false;
    spot7 = true;
    spot8 = false;
  }
  else if (tiva2 == 9){
    spot5 = false;
    spot6 = true;
    spot7 = true;
    spot8 = false;
  }
  else if (tiva2 == 8){
    spot5 = true;
    spot6 = true;
    spot7 = true;
    spot8 = false;
  }
  else if (tiva2 == 7){
    spot5 = false;
    spot6 = false;
    spot7 = false;
    spot8 = true;
  }
  else if (tiva2 == 6){
    spot5 = true;
    spot6 = false;
    spot7 = false;
    spot8 = true;
  }
  else if (tiva2 == 5){
    spot5 = false;
    spot6 = true;
    spot7 = false;
    spot8 = true;
  }
  else if (tiva2 == 4){
    spot5 = true;
    spot6 = true;
    spot7 = false;
    spot8 = true;
  }
  else if (tiva2 == 3){
    spot5 = false;
    spot6 = false;
    spot7 = true;
    spot8 = true;
  }
  else if (tiva2 == 2){
    spot5 = true;
    spot6 = false;
    spot7 = true;
    spot8 = true;
  }
  else if (tiva2 == 1){
    spot5 = false;
    spot6 = true;
    spot7 = true;
    spot8 = true;
  }
  else if (tiva2 == 0){
    spot5 = true;
    spot6 = true;
    spot7 = true;
    spot8 = true;
  }
}

//Función para las sumas
void check(){
  //Revisar si están ocupados los parqueos
  if (spot1 == true){
    s1 = 1;
  }
  else{
    s1 = 0;
  }
  if (spot2 == true){
    s2 = 1;
  }
  else{
    s2 = 0;
  }
  if (spot3 == true){
    s3 = 1;
  }
  else{
    s3 = 0;
  }
  if (spot4 == true){
    s4 = 1;
  }
  else{
    s4 = 0;
  }
  if (spot5 == true){
    s5 = 1;
  }
  else{
    s5 = 0;
  }
  if (spot6 == true){
    s6 = 1;
  }
  else{
    s6 = 0;
  }
  if (spot7 == true){
    s7 = 1;
  }
  else{
    s7 = 0;
  }
  if (spot8 == true){
    s8 = 1;
  }
  else{
    s8 = 0;
  }
}