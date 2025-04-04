// Definición de pines para relés
const int relay1 = D3;  // Relé 1
const int relay2 = D4;  // Relé 2
const int relay3 = D5;  // Relé 3 (adicional)
const int relay4 = D6;  // Relé 4 (adicional)

// Definición de comandos
char cmdS0[] = "\x02" "00S01B" "\x03";  // Comando Status
char cmdS1[] = "\x02" "00S11A" "\x03";  // Comando S1 - Activar Relé 1
char cmdS2[] = "\x02" "00S219" "\x03";  // Comando S2 - Activar Relé 2
char cmdS3[] = "\x02" "00S318" "\x03";  // Comando S3 - Activar Relé 3
char cmdS4[] = "\x02" "00S417" "\x03";  // Comando S4 - Activar Relé 4
char cmdR1[] = "\x02" "00R11B" "\x03";  // Comando R1 - Desactivar Relé 1
char cmdR2[] = "\x02" "00R21A" "\x03";  // Comando R2 - Desactivar Relé 2
char cmdR3[] = "\x02" "00R319" "\x03";  // Comando R3 - Desactivar Relé 3
char cmdR4[] = "\x02" "00R418" "\x03";  // Comando R4 - Desactivar Relé 4
char cmdX0[] = "\x02" "00X016" "\x03";  // Comando X0 - Reiniciar

// Respuestas
char cmdS0_rta[] = "\x02""00S0111134247025""\x03"; // Respuesta Status
char cmdACK[] = "\x02""00ACK""\x03";              // Respuesta ACK
char cmdNAK[] = "\x02""00NAK""\x03";              // Respuesta NAK

// Variables de control
bool debug_mode = true;  // Modo de depuración
unsigned long last_command_time = 0;  // Para control de tiempo entre comandos

void setup() {
  Serial.begin(9600);
  
  // Configurar pines de relés
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  
  // Inicializar relés (HIGH = desactivado)
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  
  if (debug_mode) {
    Serial.println("Sistema iniciado - Esperando comandos");
  }
}

void loop() {
  if (Serial.available() > 0) {
    // Leer comando hasta fin de línea
    String command = Serial.readStringUntil('\n');
    command.trim();  // Eliminar espacios y caracteres no imprimibles
    
    // Registrar tiempo de recepción del comando
    last_command_time = millis();
    
    // Depuración - mostrar comando recibido en hexadecimal
    if (debug_mode) {
      Serial.print("Comando recibido: ");
      for (int i = 0; i < command.length(); i++) {
        Serial.print(command[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    
    // Procesar comando
    if (command == cmdS0) {
      if (debug_mode) Serial.println("Procesando: Comando Status");
      sendResponse(cmdS0_rta);
    } 
    else if (command == cmdS1) {
      if (debug_mode) Serial.println("Procesando: Activar Relé 1");
      digitalWrite(relay1, LOW);
      sendResponse(cmdACK);
    } 
    else if (command == cmdS2) {
      if (debug_mode) Serial.println("Procesando: Activar Relé 2");
      digitalWrite(relay2, LOW);
      sendResponse(cmdACK);
    }
    else if (command == cmdS3) {
      if (debug_mode) Serial.println("Procesando: Activar Relé 3");
      digitalWrite(relay3, LOW);
      sendResponse(cmdACK);
    }
    else if (command == cmdS4) {
      if (debug_mode) Serial.println("Procesando: Activar Relé 4");
      digitalWrite(relay4, LOW);
      sendResponse(cmdACK);
    }
    else if (command == cmdR1) {
      if (debug_mode) Serial.println("Procesando: Desactivar Relé 1");
      digitalWrite(relay1, HIGH);
      sendResponse(cmdACK);
    } 
    else if (command == cmdR2) {
      if (debug_mode) Serial.println("Procesando: Desactivar Relé 2");
      digitalWrite(relay2, HIGH);
      sendResponse(cmdACK);
    }
    else if (command == cmdR3) {
      if (debug_mode) Serial.println("Procesando: Desactivar Relé 3");
      digitalWrite(relay3, HIGH);
      sendResponse(cmdACK);
    }
    else if (command == cmdR4) {
      if (debug_mode) Serial.println("Procesando: Desactivar Relé 4");
      digitalWrite(relay4, HIGH);
      sendResponse(cmdACK);
    }
    else if (command == cmdX0) {
      if (debug_mode) Serial.println("Procesando: Reiniciar sistema");
      sendResponse(cmdACK);
      delay(100);
      ESP.restart();
    }
    else {
      if (debug_mode) Serial.println("Comando no reconocido");
      sendResponse(cmdNAK);
    }
  }
  
  // Aquí puedes añadir otras tareas que el sistema deba hacer cuando no está procesando comandos
  // Por ejemplo, monitorear sensores, controlar estados de relés basados en temporizadores, etc.
}

void sendResponse(const char* response) {
  if (debug_mode) {
    Serial.print("Enviando respuesta: ");
    for (int i = 0; response[i] != '\0'; i++) {
      Serial.print(response[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  
  // Enviar respuesta con salto de línea (tal como parece esperar tu sistema)
  Serial.println(response);
}
