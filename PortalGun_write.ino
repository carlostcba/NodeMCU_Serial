#include <ESP8266WiFi.h>
#include <Wiegand.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

const char* ssid = "Equipos-LaSalle";        // Nombre de la red Wifi
const char* password = "Lasalle300";  // Contraseña del Wifi
const char* host = "192.168.1.202";  // Dirección IP de tu PC con XAMPP
const char* user = "portalgun";     // Usuario de la base de datos MySQL
const char* dbPassword = "@LaSalle2599"; // Contraseña de la base de datos MySQL
const char* database = "rondinls";  // Nombre de la base de datos MySQL
const int D0_PIN = 5;  // Pin D0 del lector RFID conectado al pin D1 del ESP8266 (GPIO5)
const int D1_PIN = 4;  // Pin D1 del lector RFID conectado al pin D2 del ESP8266 (GPIO4)
const int READ_RFID = 15;  // BUZZER RFID Read OK conectado al pin D8 del ESP8266 (GPI15)
const int LED_WIFI = 13;  // LED WIFI OK conectado al pin D7 del ESP8266 (GPI13)
const int LED_MYSQL = 12;  // LED MYSQL Read OK conectado al pin D6 del ESP8266 (GPI12)

WIEGAND wg;
WiFiClient client;
MySQL_Connection conn((Client *)&client);

bool connectWiFi() {
  Serial.println("Conectando a la red WiFi...");
  WiFi.begin(ssid, password);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 10) {
    delay(500);
    Serial.print(".");
    retries++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("Conexión WiFi establecida");
    pinMode(LED_WIFI, OUTPUT);
    digitalWrite(LED_WIFI, HIGH);
    delay(500);
    digitalWrite(LED_WIFI, LOW);
    Serial.print("LED Wifi conectado");
    Serial.println("");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("");
    Serial.println("Error al conectar a la red WiFi");
    digitalWrite(LED_WIFI, HIGH);
    Serial.print("LED Wifi desconectado");
    return false;
  }
}

bool connectMySQL() {
  Serial.println("Conectando a la base de datos MySQL...");
  IPAddress serverIP;
  if (!serverIP.fromString(host)) {
    Serial.println("Error al convertir la dirección IP");
    return false;
  }
  if (conn.connect(serverIP, 3306, const_cast<char*>(user), const_cast<char*>(dbPassword), const_cast<char*>(database))) {

    Serial.println("Conexión a la base de datos MySQL establecida");
    pinMode(LED_MYSQL, OUTPUT);
    digitalWrite(LED_MYSQL, HIGH);
    delay(500);
    digitalWrite(LED_MYSQL, LOW);
    Serial.print("LED MYSQL conectada");
    return true;
  } else {
    Serial.println("Error al conectar a la base de datos MySQL");
    digitalWrite(LED_MYSQL, HIGH);
    Serial.print("LED MYSQL desconectada");
    return false;
  }
}

void setup() {
  Serial.begin(115200);
  wg.begin(D0_PIN, D1_PIN);
  delay(100);
  Serial.println();
  Serial.println("Inicializando Rondin.");
  pinMode(READ_RFID, OUTPUT);
  digitalWrite(READ_RFID, LOW);

  // Conectar WiFi
  while (!connectWiFi()) {
    delay(1000);
  }

  // Conectar MySQL
  while (!connectMySQL()) {
    delay(1000);
  }
}

void loop() {
  // Verificar si la conexión WiFi está activa
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("¡Conexión WiFi perdida!");
    while (!connectWiFi()) {
      delay(1000);
    }
  }

  // Verificar si la conexión MySQL está activa
  if (!conn.connected()) {
    Serial.println("¡Conexión MySQL perdida!");
    while (!connectMySQL()) {
      delay(1000);
    }
  }

  // Leyendo tarjeta RFID
  if (wg.available()) {
    unsigned long cardCode = wg.getCode();
    // Imprimir los datos de la tarjeta RFID
    Serial.println(cardCode);
    // Activar buzzer una vez cuando lee la tarjeta
    digitalWrite(READ_RFID, HIGH);
    delay(500);
    digitalWrite(READ_RFID, LOW);
    delay(50);
    Serial.print("LED RFID se ha leido la tarjeta");
    delay(1000);

    // Insertar el código de tarjeta en la base de datos MySQL
    char query[128];
    sprintf(query, "INSERT INTO registros (tarjeta) VALUES ('%lu')", cardCode);
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    cur_mem->execute(query);
    delete cur_mem;
    


    Serial.println("Solicitud completada");
    // Activar buzzer dos veces cuando guarda el registro
    digitalWrite(READ_RFID, HIGH);
    delay(500);
    digitalWrite(READ_RFID, LOW);
    delay(50);
    digitalWrite(READ_RFID, HIGH);
    delay(200);
    digitalWrite(READ_RFID, LOW);
    delay(50);
    Serial.print("LED RFID se guardo el registro");

    delay(5000);  // Espera 5 segundos antes de realizar otra solicitud
  }
}
