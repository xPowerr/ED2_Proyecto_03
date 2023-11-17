// Webserver Proyecto 3 Parqueo 
// Gabriel Carrera 21216 y Luis Pedro González 21513

#include <WiFi.h> // Incluye la biblioteca para manejar conexiones WiFi.
#include <WiFiClient.h> // Incluye la biblioteca para manejar clientes WiFi.
#include <WebServer.h> // Incluye la biblioteca para manejar un servidor web.
#include <ESPmDNS.h> // Incluye la biblioteca para manejar nombres de dominio en la red local.
#include <LiquidCrystal.h> // Incluye la biblioteca para manejar una pantalla LCD.

// Definición de pines para la pantalla LCD 16x2.
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
  
const char* ssid = "ParqueomaticUVG"; // Nombre de la red WiFi.
const char* password = "xPowerr_"; // Contraseña de la red WiFi.
char tivaPA; // Variable para recibir datos desde el  Tiva 1.
char tivaPB; // Variable para recibir datos desde el  Tiva 2.
  
WebServer server(80); // Crea un servidor web en el puerto 80.
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); // Instancia la pantalla LCD con los pines especificados.
  
// Variables booleanas para indicar la ocupación de cada espacio de parqueo.
bool spot1 = false;
bool spot2 = false;
bool spot3 = false;
bool spot4 = false;
bool spot5 = false;
bool spot6 = false;
bool spot7 = false;
bool spot8 = false;
  
// Variables enteras para contar los espacios ocupados en cada área de parqueo.
uint8_t PA1;
uint8_t PA2;
uint8_t PA3;
uint8_t PA4;
uint8_t PB1;
uint8_t PB2;
uint8_t PB3;
uint8_t PB4;
uint8_t parqueoA;
uint8_t parqueoB;
uint8_t available;
  
  // Función para manejar la página principal del servidor web.
  void handleRoot() {
    server.send(200, "text/html", HTML()); // Envía el HTML de la página principal.
  }
  
  // Funciones para actualizar el estado de los espacios de parqueo en la interfaz web.
  void handleColor1() {
    // Si el espacio 1 está vacante (spot1 es false),
    if (spot1 == false) {
        // envía una respuesta con la clase CSS 'vacant' para el espacio 1.
        server.send(200, "text/plain", "parking-spot vacant");
    } else {
        // De lo contrario, envía una respuesta con la clase CSS 'occupied' para el espacio 1.
        server.send(200, "text/plain", "parking-spot occupied");
    }
}

// Función para manejar el estado del espacio de Parqueo A2.
void handleColor2() {
    // La lógica es similar a handleColor1, pero para el espacio 2.
    if (spot2 == false) {
        server.send(200, "text/plain", "parking-spot vacant");
    } else {
        server.send(200, "text/plain", "parking-spot occupied");
    }
}
  
  void handleColor3() {
    if (spot3 == false) {
      server.send(200, "text/plain", "parking-spot vacant");
    } else {
      server.send(200, "text/plain", "parking-spot occupied");
    }
  }
  
  void handleColor4() {
    if (spot4 == false) {
      server.send(200, "text/plain", "parking-spot vacant");
    } else {
      server.send(200, "text/plain", "parking-spot occupied");
    }
  }
  
  void handleColor5() {
    if (spot5 == false) {
      server.send(200, "text/plain", "parking-spot vacant");
    } else {
      server.send(200, "text/plain", "parking-spot occupied");
    }
  }
  
  void handleColor6() {
    if (spot6 == false) {
      server.send(200, "text/plain", "parking-spot vacant");
    } else {
      server.send(200, "text/plain", "parking-spot occupied");
    }
  }
  
  void handleColor7() {
    if (spot7 == false) {
      server.send(200, "text/plain", "parking-spot vacant");
    } else {
      server.send(200, "text/plain", "parking-spot occupied");
    }
  }
  
  void handleColor8() {
    if (spot8 == false) {
      server.send(200, "text/plain", "parking-spot vacant");
    } else {
      server.send(200, "text/plain", "parking-spot occupied");
    }
  }
  
  // Función para manejar solicitudes a URLs no encontradas.
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
    Serial.begin(115200); // Inicia la comunicación serial.
    Serial1.begin(115200, SERIAL_8N1, 32, 33, false, 20000UL, 112); // Inicia UART1 con configuración específica.
    Serial2.begin(115200); // Inicia UART2.
    lcd.begin(16, 2); // Inicia la pantalla LCD de 16x2.
    lcd.setCursor(0,0); // Posiciona el cursor en la pantalla LCD.
    lcd.print("Parqueomatic UVG"); // Muestra mensaje inicial en la LCD.
    lcd.setCursor(0,1); // Cambia la posición del cursor en la LCD.
    lcd.print("Disp:"); // Muestra la disponibilidad de parqueos.
    lcd.setCursor(7,1);
    lcd.print("PA:"); // Muestra la disponibilidad en el área PA.
    lcd.setCursor(12,1);
    lcd.print("PB:"); // Muestra la disponibilidad en el área PB.
    WiFi.mode(WIFI_STA); // Configura el módulo WiFi en modo estación.
    WiFi.begin(ssid, password); // Conecta a la red WiFi.
    Serial.println("");

    // Espera a que la conexión WiFi se establezca.
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); // Muestra la dirección IP asignada al ESP32.
  
    // Configura el MDNS.
    if (MDNS.begin("esp32")) {
      Serial.println("MDNS responder started");
    }
  
    // Define las rutas y funciones del servidor web.
    server.on("/", handleRoot);
    server.on("/l1", handleColor1);
    server.on("/l2", handleColor2);
    server.on("/l3", handleColor3);
    server.on("/l4", handleColor4);
    server.on("/l5", handleColor5);
    server.on("/l6", handleColor6);
    server.on("/l7", handleColor7);
    server.on("/l8", handleColor8);
    server.onNotFound(handleNotFound);
  
    server.begin(); // Inicia el servidor web.
    Serial.println("HTTP server started");
  }
  
  void loop(void) {
    server.handleClient(); // Esta línea mantiene activo el servidor web. Procesa las solicitudes entrantes y responde a ellas.
  
    check_tivaPA(); // Esta función verifica el estado actual de los parqueos manejados por el dispositivo Tiva 1.
    check_tivaPB(); // Similar a check_tivaPA, pero para el dispositivo Tiva 2.
  
    check(); // Esta función realiza operaciones adicionales de verificación o actualización del estado del parqueo.
  
    parqueoA = PA1 + PA2 + PA3 + PA4; // Suma los espacios disponibles en el sotano 1 (o área A) y almacena el resultado en 'parqueoA'.
    parqueoB = PB1 + PB2 + PB3 + PB4; // Suma los espacios disponibles en el sotano 2 (o área B) y almacena el resultado en 'parqueoB'.
    available = PA1 + PA2 + PA3 + PA4 + PB1 + PB2 + PB3 + PB4; // Calcula el total de espacios disponibles sumando todos los espacios de ambos sótanos.
  
    lcd.setCursor(5, 1); // Posiciona el cursor en la fila 1, columna 5 de la pantalla LCD.
    lcd.print(String(available)); // Muestra el número total de espacios disponibles en la pantalla LCD.
  
    lcd.setCursor(10,1); // Posiciona el cursor en la fila 1, columna 10 de la pantalla LCD.
    lcd.print(String(parqueoA)); // Muestra los espacios disponibles en el sotano 1 en la pantalla LCD.
  
    lcd.setCursor(15,1); // Posiciona el cursor en la fila 1, columna 15 de la pantalla LCD.
    lcd.print(String(parqueoB)); // Muestra los espacios disponibles en el sotano 2 en la pantalla LCD.
  }
  

// Código HTML, CSS y Javascript para una página web de gestión de parqueo
String HTML() {
    // Iniciando el documento HTML con una declaración DOCTYPE y abriendo la etiqueta html
    String ptr = "<!DOCTYPE html> <html>\n";

    // Sección head con metadatos para diseño responsivo y conjunto de caracteres
    ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr += "<meta charset=\"UTF-8\">\n";

    // Título de la página
    ptr += "<title>PARQUEO</title>\n";

    // Estilos CSS internos para definir la apariencia de la página
    ptr += "  <style>\n";
    // Estilo para el cuerpo de la página
    ptr += "    body {\n";
    ptr += "      font-family: Arial, sans-serif;\n"; // Tipo de fuente
    ptr += "      background-color: #FFFFFF;\n"; // Color de fondo
    ptr += "      color: #000000;\n"; // Color del texto
    ptr += "      margin: 0;\n"; // Margen
    ptr += "      padding: 20px;\n"; // Relleno
    ptr += "    }\n";
    // Estilo para los encabezados h1 y h2
    ptr += "    h1, h2 {\n";
    ptr += "      text-align: center;\n"; // Alineación del texto
    ptr += "    }\n";
    // Estilo para el layout de los espacios de parqueo
    ptr += "    .parking-lot {\n";
    ptr += "      display: flex;\n"; // Uso de Flexbox
    ptr += "      justify-content: center;\n"; // Alineación horizontal
    ptr += "      flex-wrap: wrap;\n"; // Permitir múltiples líneas
    ptr += "    }\n";
    // Estilo para cada espacio de parqueo
    ptr += "    .parking-spot {\n";
    ptr += "      width: 22%;\n"; // Ancho
    ptr += "      height: 320px;\n"; // Altura
    ptr += "      border-radius: 20px;\n"; // Bordes redondeados
    ptr += "      margin: 20px;\n"; // Margen
    ptr += "      text-align: center;\n"; // Alineación del texto
    ptr += "      display: flex;\n"; // Uso de Flexbox
    ptr += "      align-items: center;\n"; // Alineación vertical
    ptr += "      justify-content: center;\n"; // Alineación horizontal
    ptr += "      font-size: 26px;\n"; // Tamaño de fuente
    ptr += "      font-weight: bold;\n"; // Peso de la fuente
    ptr += "      cursor: pointer;\n"; // Estilo del cursor
    ptr += "      transition: background-color 0.3s ease;\n"; // Transición suave de color
    ptr += "    }\n";
    // Estilo para espacios vacantes
    ptr += "    .vacant {\n";
    ptr += "      background-color: #FF0000;\n"; // Color de fondo para vacante
    ptr += "    }\n";
    // Estilo para espacios ocupados
    ptr += "    .occupied {\n";
    ptr += "      background-color: #30FF24;\n"; // Color de fondo para ocupado
    ptr += "    }\n";
    ptr += "  </style>\n";
    ptr += "</head>\n";

    // Cuerpo de la página
    ptr += "<body>\n";
    // Encabezado principal
    ptr += "  <h1>Parqueo-matic</h1>\n";
    // Encabezado secundario para el parqueo A
    ptr += "  <h2>PARQUEO A</h2>\n";
    // División para los espacios del parqueo A
    ptr += "  <div class=\"parking-lot\">\n";
    // Cada div representa un espacio de parqueo
    ptr += "<div id=\"spot1\" class=\"parking-spot\">PA1 &#128664;</div>\n";
    ptr += "<div id=\"spot2\" class=\"parking-spot\">PA2 &#128664;</div>\n";
    ptr += "<div id=\"spot3\" class=\"parking-spot\">PA3 &#128664;</div>\n";
    ptr += "<div id=\"spot4\" class=\"parking-spot\">PA4 &#128664;</div>\n";
    ptr += "  </div>\n";
    // Encabezado secundario para el parqueo B
    ptr += "  <h2>PARQUEO B</h2>\n";
    // División para los espacios del parqueo B
    ptr += "  <div class=\"parking-lot\">\n";
    // Cada div representa un espacio de parqueo
    ptr += "<div id=\"spot5\" class=\"parking-spot\">PB1 &#128664;</div>\n";
    ptr += "<div id=\"spot6\" class=\"parking-spot\">PB2 &#128664;</div>\n";
    ptr += "<div id=\"spot7\" class=\"parking-spot\">PB3 &#128664;</div>\n";
    ptr += "<div id=\"spot8\" class=\"parking-spot\">PB4 &#128664;</div>\n";
    ptr += "  </div>\n";

    // Script de JavaScript para actualizar los colores de los espacios de parqueo
    ptr += "<script>\n";
    // Funciones para actualizar el color de cada espacio de parqueo
    ptr += "function updateColor1() {\n";
    ptr += "var xhttp = new XMLHttpRequest();\n"; // Creando un objeto XMLHttpRequest
    ptr += "xhttp.onreadystatechange = function() {\n"; // Evento para manejar los cambios de estado
    ptr += "if (this.readyState == 4 && this.status == 200) {\n"; // Verificación del estado de la solicitud
    ptr += "document.getElementById('spot1').className = this.responseText;\n"; // Actualización del color basado en la respuesta
    ptr += "}\n";
    ptr += "};\n";
    ptr += "xhttp.open('GET', '/l1', true);\n"; // Configuración de la solicitud GET
    ptr += "xhttp.send();\n"; // Enviando la solicitud
    ptr += "setTimeout(updateColor1, 500);}\n"; // Estableciendo un temporizador para actualización periódica
    // Repetición de las funciones para cada espacio de parqueo (updateColor2, updateColor3, etc.)
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
    ptr +="setTimeout(updateColor8, 500);}\n";    ptr += "updateColor1();\n"; // Iniciando la primera actualización de color para cada espacio
    ptr += "updateColor2();\n";
    ptr += "updateColor3();\n";
    ptr += "updateColor4();\n";
    ptr += "updateColor5();\n";
    ptr += "updateColor6();\n";
    ptr += "updateColor7();\n";
    ptr += "updateColor8();\n";
    ptr += "</script>\n";
    ptr += "</body>\n";
    ptr += "</html>";
    return ptr;
}


// Función para chequear la información de la tivaPA
void check_tivaPA(){
  tivaPA = Serial2.read(); // Leer datos de la Tiva 1 usando UART2

  // Evaluar el valor de 'tivaPA' para determinar el estado de los parqueos 1 a 4
  if (tivaPA == 15){
    spot1 = false; // Parqueo A1 libre
    spot2 = false; // Parqueo A2 libre
    spot3 = false; // Parqueo A3 libre
    spot4 = false; // Parqueo A4 libre
  }
  else if (tivaPA == 14){
    spot1 = true;  // Parqueo A1 ocupado
    spot2 = false; // Parqueo A2 libre
    spot3 = false; // Parqueo A3 libre
    spot4 = false; // Parqueo A4 libre
  }
  else if (tivaPA == 13){
    spot1 = false; // Parqueo A1 libre
    spot2 = true;  // Parqueo A2 ocupado
    spot3 = false; // Parqueo A3 libre
    spot4 = false; // Parqueo A4 libre
  }
  else if (tivaPA == 12){
    spot1 = true;  // Parqueo A1 ocupado
    spot2 = true;  // Parqueo A2 ocupado
    spot3 = false; // Parqueo A3 libre
    spot4 = false; // Parqueo A4 libre
  }
  else if (tivaPA == 11){
    spot1 = false; // Parqueo A1 libre
    spot2 = false; // Parqueo A2 libre
    spot3 = true;  // Parqueo A3 ocupado
    spot4 = false; // Parqueo A4 libre
  }
  else if (tivaPA == 10){
    spot1 = true;  // Parqueo A1 ocupado
    spot2 = false; // Parqueo A2 libre
    spot3 = true;  // Parqueo A3 ocupado
    spot4 = false; // Parqueo A4 libre
  }
  else if (tivaPA == 9){
    spot1 = false; // Parqueo A1 libre
    spot2 = true;  // Parqueo A2 ocupado
    spot3 = true;  // Parqueo A3 ocupado
    spot4 = false; // Parqueo A4 libre
  }
  else if (tivaPA == 8){
    spot1 = true;  // Parqueo A1 ocupado
    spot2 = true;  // Parqueo A2 ocupado
    spot3 = true;  // Parqueo A3 ocupado
    spot4 = false; // Parqueo A4 libre
  }
  else if (tivaPA == 7){
    spot1 = false; // Parqueo A1 libre
    spot2 = false; // Parqueo A2 libre
    spot3 = false; // Parqueo A3 libre
    spot4 = true;  // Parqueo A4 ocupado
  }
  else if (tivaPA == 6){
    spot1 = true;  // Parqueo A1 ocupado
    spot2 = false; // Parqueo A2 libre
    spot3 = false; // Parqueo A3 libre
    spot4 = true;  // Parqueo A4 ocupado
  }
  else if (tivaPA == 5){
    spot1 = false; // Parqueo A1 libre
    spot2 = true;  // Parqueo A2 ocupado
    spot3 = false; // Parqueo A3 libre
    spot4 = true;  // Parqueo A4 ocupado
  }
  else if (tivaPA == 4){
    spot1 = true;  // Parqueo A1 ocupado
    spot2 = true;  // Parqueo A2 ocupado
    spot3 = false; // Parqueo A3 libre
    spot4 = true;  // Parqueo A4 ocupado
  }
  else if (tivaPA == 3){
    spot1 = false; // Parqueo A1 libre
    spot2 = false; // Parqueo A2 libre
    spot3 = true;  // Parqueo A3 ocupado
    spot4 = true;  // Parqueo A4 ocupado
  }
  else if (tivaPA == 2){
    spot1 = true;  // Parqueo A1 ocupado
    spot2 = false; // Parqueo A2 libre
    spot3 = true;  // Parqueo A3 ocupado
    spot4 = true;  // Parqueo A4 ocupado
  }
  else if (tivaPA == 1){
    spot1 = false; // Parqueo A1 libre
    spot2 = true;  // Parqueo A2 ocupado
    spot3 = true;  // Parqueo A3 ocupado
    spot4 = true;  // Parqueo A4 ocupado
  }
  else if (tivaPA == 0){
    spot1 = true;  // Parqueo A1 ocupado
    spot2 = true;  // Parqueo A2 ocupado
    spot3 = true;  // Parqueo A3 ocupado
    spot4 = true;  // Parqueo A4 ocupado
  }
}

void check_tivaPB(){
  tivaPB = Serial1.read(); // Leer datos de la Tiva 2 usando UART1

  // Evaluar el valor de 'tivaPB' para determinar el estado de los parqueos 5 a 8
  if (tivaPB == 15){
    spot5 = false; // Parqueo B1 libre
    spot6 = false; // Parqueo B2 libre
    spot7 = false; // Parqueo B3 libre
    spot8 = false; // Parqueo B4 libre
  }
  else if (tivaPB == 14){
    spot5 = true;  // Parqueo B1 ocupado
    spot6 = false; // Parqueo B2 libre
    spot7 = false; // Parqueo B3 libre
    spot8 = false; // Parqueo B4 libre
  }
  else if (tivaPB == 13){
    spot5 = false; // Parqueo B1 libre
    spot6 = true;  // Parqueo B2 ocupado
    spot7 = false; // Parqueo B3 libre
    spot8 = false; // Parqueo B4 libre
  }
  else if (tivaPB == 12){
    spot5 = true;  // Parqueo B1 ocupado
    spot6 = true;  // Parqueo B2 ocupado
    spot7 = false; // Parqueo B3 libre
    spot8 = false; // Parqueo B4 libre
  }
  else if (tivaPB == 11){
    spot5 = false; // Parqueo B1 libre
    spot6 = false; // Parqueo B2 libre
    spot7 = true;  // Parqueo B3 ocupado
    spot8 = false; // Parqueo B4 libre
  }
  else if (tivaPB == 10){
    spot5 = true;  // Parqueo B1 ocupado
    spot6 = false; // Parqueo B2 libre
    spot7 = true;  // Parqueo B3 ocupado
    spot8 = false; // Parqueo B4 libre
  }
  else if (tivaPB == 9){
    spot5 = false; // Parqueo B1 libre
    spot6 = true;  // Parqueo B2 ocupado
    spot7 = true;  // Parqueo B3 ocupado
    spot8 = false; // Parqueo B4 libre
  }
  else if (tivaPB == 8){
    spot5 = true;  // Parqueo B1 ocupado
    spot6 = true;  // Parqueo B2 ocupado
    spot7 = true;  // Parqueo B3 ocupado
    spot8 = false; // Parqueo B4 libre
  }
  else if (tivaPB == 7){
    spot5 = false; // Parqueo B1 libre
    spot6 = false; // Parqueo B2 libre
    spot7 = false; // Parqueo B3 libre
    spot8 = true;  // Parqueo B4 ocupado
  }
  else if (tivaPB == 6){
    spot5 = true;  // Parqueo B1 ocupado
    spot6 = false; // Parqueo B2 libre
    spot7 = false; // Parqueo B3 libre
    spot8 = true;  // Parqueo B4 ocupado
  }
  else if (tivaPB == 5){
    spot5 = false; // Parqueo B1 libre
    spot6 = true;  // Parqueo B2 ocupado
    spot7 = false; // Parqueo B3 libre
    spot8 = true;  // Parqueo B4 ocupado
  }
  else if (tivaPB == 4){
    spot5 = true;  // Parqueo B1 ocupado
    spot6 = true;  // Parqueo B2 ocupado
    spot7 = false; // Parqueo B3 libre
    spot8 = true;  // Parqueo B4 ocupado
  }
  else if (tivaPB == 3){
    spot5 = false; // Parqueo B1 libre
    spot6 = false; // Parqueo B2 libre
    spot7 = true;  // Parqueo B3 ocupado
    spot8 = true;  // Parqueo B4 ocupado
  }
  else if (tivaPB == 2){
    spot5 = true;  // Parqueo B1 ocupado
    spot6 = false; // Parqueo B2 libre
    spot7 = true;  // Parqueo B3 ocupado
    spot8 = true;  // Parqueo B4 ocupado
  }
  else if (tivaPB == 1){
    spot5 = false; // Parqueo B1 libre
    spot6 = true;  // Parqueo B2 ocupado
    spot7 = true;  // Parqueo B3 ocupado
    spot8 = true;  // Parqueo B4 ocupado
  }
  else if (tivaPB == 0){
    spot5 = true;  // Parqueo B1 ocupado
    spot6 = true;  // Parqueo B2 ocupado
    spot7 = true;  // Parqueo B3 ocupado
    spot8 = true;  // Parqueo B4 ocupado
  }
}

// Función para verificar el estado de cada parqueo
void check(){
  // Revisar si están ocupados los parqueos y asignar 1 si están ocupados o 0 si están libres
  if (spot1 == true){ PA1 = 1; } else{ PA1 = 0; }
  if (spot2 == true){ PA2 = 1; } else{ PA2 = 0; }
  if (spot3 == true){ PA3 = 1; } else{ PA3 = 0; }
  if (spot4 == true){ PA4 = 1; } else{ PA4 = 0; }
  if (spot5 == true){ PB1 = 1; } else{ PB1 = 0; }
  if (spot6 == true){ PB2 = 1; } else{ PB2 = 0; }
  if (spot7 == true){ PB3 = 1; } else{ PB3 = 0; }
  if (spot8 == true){ PB4 = 1; } else{ PB4 = 0; }
}