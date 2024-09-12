# Web-Based Servo Motor Control with ESP8266

## Overview

This project demonstrates how to control a servo motor using an ESP8266 microcontroller and a web interface. The ESP8266 connects to a Wi-Fi network and serves a web page that allows users to adjust the position of a servo motor by clicking buttons on the page.

## Features

- Connects to a Wi-Fi network.
- Hosts a web server to control the servo motor.
- Provides a web interface with buttons to move the servo motor left or right.
- Displays the current angle of the servo motor on the web page.

## Components

- **ESP8266 Microcontroller** (e.g., NodeMCU or Wemos D1 Mini)
- **Servo Motor**
- **Power Supply** (suitable for the servo motor and ESP8266)

## Installation

1. **Install the Arduino IDE**:

   - Download and install the [Arduino IDE](https://www.arduino.cc/en/software).

2. **Install ESP8266 Board Package**:

   - Open the Arduino IDE, go to **File** > **Preferences**.
   - Add the following URL to the **Additional Boards Manager URLs** field: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`.
   - Go to **Tools** > **Board** > **Boards Manager**.
   - Search for **esp8266** and install the package.

3. **Install Required Libraries**:

   - Install the following libraries using the Library Manager (Sketch > Include Library > Manage Libraries):
     - **ESP8266WiFi**
     - **ESP8266WebServer**
     - **ESP8266mDNS**
     - **Servo**

4. **Upload the Code**:
   - Connect your ESP8266 to your computer.
   - Open the Arduino IDE and select the appropriate board and port.
   - Copy and paste the code provided below into the Arduino IDE and upload it to the ESP8266.

## Code

```cpp
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>

const char* ssid = "UGMURO-1";
const char* password = "ayamgepuk";
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
  webPage += "updateAngle();";
  webPage += "</script>";

  webPage += "</body></html>";
}
```

## Usage

1. Power Up the ESP8266:
   - Connect the ESP8266 to a suitable power source.
2. Access the Web Interface:
   - Open a web browser and enter the IP address displayed in the Serial Monitor after the ESP8266 connects to the Wi-Fi network.
3. Control the Servo Motor:
   - Use the "Kiri" button to move the servo motor to the left.
   - Use the "Kanan" button to move the servo motor to the right.
   - The current angle of the servo motor will be displayed on the web page.

## Troubleshooting

- Ensure the ESP8266 is connected to the correct Wi-Fi network and that the credentials are correct.
- Check the Serial Monitor for any error messages or connection issues.
- Make sure the servo motor is properly connected and powered.

## License

This project is licensed under the MIT License.

## Acknowledgments

- ESP8266WiFi Library
- Servo Library

#### Feel free to modify and expand upon this project to suit your needs. Happy coding!

#### @Copyright 2019 | Veenbotronik
