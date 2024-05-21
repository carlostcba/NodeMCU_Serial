#include <Wiegand.h>

const int D0_PIN = D7;  // Pin D0 del lector RFID conectado al pin D1 del ESP8266
const int D1_PIN = D8;  // Pin D1 del lector RFID conectado al pin D2 del ESP8266
const int relay1 = D5;  // Rele 1 conectado al pin D3 del ESP8266 (GPIO5)
const int relay2 = D6;  // Rele 2 conectado al pin D4 del ESP8266

char cmdS0[] = "\x02" "00S01B" "\x03";  // Comando Status
char cmdS1[] = "\x02" "0011SA" "\x03";  // Comando S1 - Activar Rele 1
char cmdS2[] = "\x02" "00S219" "\x03";  // Comando S2 - Activar Rele 2
char cmdR1[] = "\x02" "00R11B" "\x03";  // Comando R1 - Desactivar Rele 1
char cmdR2[] = "\x02" "00R21A" "\x03";  // Comando R2 - Desactivar Rele 2
char cmdX0[] = "\x02" "00X016" "\x03";  // Comando X0 - Reiniciar

//Estructura de Respuesta Comando Status
char stx_s0[] = "\x02" "00";
char etx_s0[] = "1B" "\x03";

char cmdS0_rta[] = "\x02""00S0111134247025""\x03"; // Respuesta Comando Status

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
  // Leyendo tarjeta RFID
  if (wg1.available()) {
    unsigned long cardCode_wg1 = wg1.getCode();
    // Construir la cadena completa antes de imprimirla
    String cardCodeStr = "\x02" "00S00800" + String(cardCode_wg1) + "\x03" + '\n';
    Serial.print(cardCodeStr);
  }
  // Leyendo datos del puerto serie
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == cmdS0) {
      sendResponse(cmdS0_rta);
    } else if (command == cmdS1) {
      digitalWrite(relay1, LOW);
      sendResponse(cmdS1);
    } else if (command == cmdS2) {
      digitalWrite(relay2, LOW);
      sendResponse(cmdS1);
    } else if (command == cmdR1) {
      digitalWrite(relay1, HIGH);
      sendResponse(cmdS1);
    } else if (command == cmdR2) {
      digitalWrite(relay2, HIGH);
      sendResponse(cmdS1);
    } else if (command == cmdX0) {
      sendResponse(cmdX0);
      ESP.restart();
    }
  }
}

void sendResponse(String response) {
  Serial.println(response);
}
