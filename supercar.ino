#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "uiu";
const char *password = "kogaidan";

ESP8266WebServer server(80);

const int led = 13;

const int in_left1 = 2;
const int in_left2 = 15;
const int in_right1 = 12;
const int in_right2 = 14;

void handleNotFound() {
  digitalWrite ( led, 1 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( led, 0 );
}

void setup() {
  pinMode(led, OUTPUT);
  pinMode(in_left1, OUTPUT);
  pinMode(in_left2, OUTPUT);
  pinMode(in_right1, OUTPUT);
  pinMode(in_right2, OUTPUT);
  
  digitalWrite(led, 0);
  digitalWrite(in_left1, 0);
  digitalWrite(in_left2, 0);
  digitalWrite(in_right1, 0);
  digitalWrite(in_right2, 0);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if (MDNS.begin ("supercar")) {
    Serial.println ( "MDNS responder started" );
  }

  server.on("/", []() {
    server.send ( 200, "text/plain", "i am supercar" );
  });

  server.on("/motor", []() {
    String l = server.arg("l");
    String r = server.arg("r");
    
    analogWrite(in_left1, l.toInt());
    analogWrite(in_left2, 0);
    
    analogWrite(in_right1, r.toInt());
    analogWrite(in_right2, 0);

    server.send(200, "text/plain", l + ", " + r);
  });
  
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop() {
  server.handleClient();
}

