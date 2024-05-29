#include <Wiegand.h>

const int D0_PIN = D7;  // Pin D0 del lector RFID conectado al pin D7 del ESP8266
const int D1_PIN = D8;  // Pin D1 del lector RFID conectado al pin D8 del ESP8266
const int relay1 = D5;  // Rele 1 conectado al pin D5 del ESP8266
const int relay2 = D6;  // Rele 2 conectado al pin D6 del ESP8266

char cmdS0[] = "\x02" "00S01B" "\x03";  // Comando Status
char cmdS1[] = "\x02" "0011SA" "\x03";  // Comando S1 - Activar Rele 1
char cmdS2[] = "\x02" "00S219" "\x03";  // Comando S2 - Activar Rele 2
char cmdR1[] = "\x02" "00R11B" "\x03";  // Comando R1 - Desactivar Rele 1
char cmdR2[] = "\x02" "00R21A" "\x03";  // Comando R2 - Desactivar Rele 2
char cmdX0[] = "\x02" "00X016" "\x03";  // Comando X0 - Reiniciar

// Estructura de Respuesta Comando Status
String stx_s0 = "\x02" "00" "S0"; // STX mas el ID de placa y el comando S0
String csta_s0 = "0000" // Variable inicializada en 0000
String data_s0 = "" // Variable inicializada vacia
String etx_s0 = "1B" "\x03"; // CHECKSUM mas ETX


String DDMM1_b = "0001";  // Detector de masa metalica 1 esta accionado
String DDMM2_b = "0002";  // Detector de masa metalica 2 esta accionado
String RELE1_b = "0040";  // Relay 1 activado
String RELE2_b = "0080";  // Relay 1 activado
String TARJ_b = "0100";   // Lectura de Tarjeta
String PULS_b = "0400";   // Pulsador de papel fue accionado

// Armado de respuesta Status
String rta_s0 = "0000"  // Respuesta Inicializada en 0000 


WIEGAND wg1; // Declarar Wiegand 1

void setup() {
  Serial.begin(9600);
  wg1.begin(D0_PIN, D1_PIN);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
}

void loop() {
  // Verifica si hay una tarjeta RFID disponible
  if (wg1.available()) {
    unsigned long cardCode_wg1 = wg1.getCode();
    String cardCodeStr = stx_s0 + String(cardCode_wg1) + "\x03";
    data_s0 = cardCodeStr;
    csta_s0 = TARJ_b;
    // Construir la respuesta dinámica
    String rta_s0 = stx_s0 + csta_s0 + data_s0 + etx_s0;
    sendResponse(rta_s0);
  }

  // Leyendo datos del puerto serie
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == String(cmdS0)) {
      sendResponse(rta_s0);
    } else if (command == String(cmdS1)) {
      digitalWrite(relay1, LOW);
      sendResponse(cmdS1);
    } else if (command == String(cmdS2)) {
      digitalWrite(relay2, LOW);
      sendResponse(cmdS2);
    } else if (command == String(cmdR1)) {
      digitalWrite(relay1, HIGH);
      sendResponse(cmdR1);
    } else if (command == String(cmdR2)) {
      digitalWrite(relay2, HIGH);
      sendResponse(cmdR2);
    } else if (command == String(cmdX0)) {
      sendResponse(cmdX0);
      ESP.restart();
    }
  }
}

void sendResponse(String response) {
  Serial.println(response);
}
