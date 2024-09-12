#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>

const char* ssid = "UGMURO-1"; //Ubah sesuai wifi anda
const char* password = "ayamgepuk"; //Ubah Password Wifi sesuai milik anda
int x = 90;

ESP8266WebServer server(80);
String webPage = "";
Servo myservo;

void setup() {
  myservo.attach(D2);
  myservo.write(x);
  delay(200);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", []() {
    refreshpage();
    server.send(200, "text/html", webPage);
  });
  server.on("/kiri", []() {
    kiri();
    refreshpage();
    server.send(200, "text/html", webPage);
    Serial.println(x);
  });
  server.on("/kanan", []() {
    kanan();
    refreshpage();
    server.send(200, "text/html", webPage);
    Serial.println(x);
  });
  server.on("/angle", []() {
    server.send(200, "text/plain", String(x));
  });

  server.begin();
  Serial.println("HTTP server started");
  Serial.println(x);
}

void loop() {
  server.handleClient();
}

void kiri() {
  if (x < 180) {
    x += 30;
    if (x > 180) x = 180; // Clamp to 180
  }
}

void kanan() {
  if (x > 0) {
    x -= 30;
    if (x < 0) x = 0; // Clamp to 0
  }
}

void refreshpage() {
  webPage = "";
  webPage += "<html><head><style>";
  webPage += "body { font-family: Arial, sans-serif; background-color: #f0f0f5; margin: 0; padding: 0; text-align: center; }";
  webPage += "h1 { color: #333; margin-top: 20px; }";
  webPage += "h2 { color: #666; }";
  webPage += ".container { background-color: #fff; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); margin: 20px auto; padding: 20px; width: 80%; max-width: 600px; }";
  webPage += "button { background-color: #4CAF50; color: white; padding: 15px 25px; font-size: 18px; margin: 10px; border: none; border-radius: 5px; cursor: pointer; box-shadow: 0 4px #999; transition: background-color 0.3s ease; }";
  webPage += "button:hover { background-color: #45a049; }";
  webPage += "button:active { background-color: #3e8e41; box-shadow: 0 2px #666; transform: translateY(4px); }";
  webPage += "</style></head><body>";
  webPage += "<div class='container'>";
  webPage += "<h1>WebCam Servo Control</h1>";
  webPage += "<button id='leftButton'>Kiri</button>";
  webPage += "<button id='rightButton'>Kanan</button>";
  webPage += "<h2 id='angleDisplay'>Current Angle: " + String(x) + "°</h2>";
  webPage += "</div>";

  // JavaScript for handling button clicks and updating the angle
  webPage += "<script>";
  webPage += "document.getElementById('leftButton').addEventListener('click', function() {";
  webPage += "  fetch('/kiri').then(response => response.text()).then(updateAngle);";
  webPage += "});";
  webPage += "document.getElementById('rightButton').addEventListener('click', function() {";
  webPage += "  fetch('/kanan').then(response => response.text()).then(updateAngle);";
  webPage += "});";
  webPage += "function updateAngle() {";
  webPage += "  fetch('/angle').then(response => response.text()).then(angle => {";
  webPage += "    document.getElementById('angleDisplay').innerText = 'Current Angle: ' + angle + '°';";
  webPage += "  });";
  webPage += "}";
  webPage += "updateAngle();"; // Initial call to update the angle display
  webPage += "</script>";

  webPage += "</body></html>";
}
