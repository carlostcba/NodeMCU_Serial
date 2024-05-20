const int relay1 = D3;  // Variable Rele 1
const int relay2 = D4;  // Variable Rele 2

char cmdS0[] = "\x02" "00S01B" "\x03";  // Comando Status
char cmdS1[] = "\x02" "0011SA" "\x03";  // Comando S1 - Activar Rele 1
char cmdS2[] = "\x02" "00S219" "\x03";  // Comando S2 - Activar Rele 2
char cmdR1[] = "\x02" "00R11B" "\x03";  // Comando R1 - Desactivar Rele 1
char cmdR2[] = "\x02" "00R21A" "\x03";  // Comando R2 - Desactivar Rele 2
char cmdX0[] = "\x02" "00X016" "\x03";  // Comando X0 - Reiniciar

// Respuesta Comando Status
char cmdS0_rta[] = "\x02""00S0111134247025""\x03"; // Respuesta Comando Status

void setup() {
  Serial.begin(9600);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
}

void loop() {
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
