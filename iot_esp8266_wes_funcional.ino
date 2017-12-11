//Equipo de investigación aplicada
//Profesor Roberto Mauricio Pérez Fuentes - Ingeniero en Informática
//Alumno Javier Ignacio Valdés Valdés - Estudiante de Ing. en Informatica - Técnico Electrónico Industrial
//Alumno Diego Ernesto Aburto Olave - Estudiante de Ing. en Informatica - Dibujante Técnico


#include <ESP8266WiFi.h>
#include <DHT.h>

String apiKey = "7YI6JCV6L0";
//String apiKey = "";
const char* ssid = "test1";
//const char* ssid = "test1";
const char* password = "mexico123";
//const char* password = "testroot";
//const char* server = "api.thingspeak.com";
const char* server = "solivsol.com";


//Configuraciones de los perifericos
#define DHTPIN 14     // what digital pin we're connected to
#define DHTTYPE DHT21   // DHT 21 AM2301
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  //Representacion de los estados del arduino en el serial com
  Serial.begin(9600);
  delay(500);
  
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
    Serial.print("No conectado");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //Inicialización de dht
   dht.begin();

   //Led para decir que esta encendido el dipositivo
   //pinMode(12,OUTPUT);
   //Led  para demostrar conexion wifi
   //pinMode(13,OUTPUT);
  
}
void loop() {
  //Led 12 para decir que equipo essta encendido
  //digitalWrite(13,HIGH);
  //digitalWrite(12,HIGH);
  
  //Lectura de variables fisicas
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  //Accion en caso de que la lectura sea erronea
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //Calculos de temperatura en grados celsius
  float hic = dht.computeHeatIndex(t, h, false);

  /*Metadatos y estructura necesaria para la creacion de un archivo html comprensible porparte del webservice*/
  if (client.connect(server,80)) {
    String postStr = apiKey;
    /*
    postStr +="&field1=";
    postStr += String(luminosidad);
    postStr +="&field2=";
    postStr += String((int)temperatura);
    postStr +="&field3=";
    postStr += String((int)humedad);
    */
    /*
    postStr +="&field1=";
    postStr += String((int)t);
    postStr +="&field2=";
    postStr += String((int)h);
    */

    /*Nombre de variable para el servicio web BB.DD*/
    postStr +="&temperatura=";
    postStr += String((int)t);
    /*Nombre de variable para el servicio web BB.DD*/
    postStr +="&humedad=";
    postStr += String((int)h);
    
    postStr += "\r\n\r\n";
    client.print("POST /Sensor.php HTTP/1.1\n");
    //client.print("Host: api.thingspeak.com\n");
    client.print("Host: solivsol.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println("% send to Thingspeak");
  }
  client.stop();
  Serial.println("Waiting…");
  delay(5000);
}
