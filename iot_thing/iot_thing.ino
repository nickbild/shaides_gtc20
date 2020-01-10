/* Nick Bild
 * nick.bild@gmail.com
 * Web server that controls an MP3 player.
 * 
 * Developed for Arduino Nano 33 IoT
 */

#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  Serial.println("Starting Access Point...");

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the WiFi firmware.");
  }

  // Set AP IP address.
  WiFi.config(IPAddress(192, 168, 1, 4));

  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // Start web server (port 80).
  server.begin();

  printWiFiStatus();
}


void loop() {
  // Check for changes in connected devices.
  if (status != WiFi.status()) {
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      Serial.println("Device connected to AP.");
    } else {
      Serial.println("Device disconnected from AP.");
    }
  }

  // Listen for incoming clients.
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected.");
    String currentLine = "";
    while (client.connected()) {
      // Are there still bytes to read?
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          // If the current line is empty, that's two newlines in a row (i.e. end of client request).          
          if (currentLine.length() == 0) {
            // Send response to client.
            
            // HTTP header.
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Response content.
            client.print("Click <a href=\"/music_on\">here</a> turn the music on.<br>");
            client.print("Click <a href=\"/music_off\">here</a> turn the music off.<br>");

            // End of response.
            client.println();
            
            break;
          } else {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;
        }

        // Interpret client request.
        if (currentLine.endsWith("GET /music_on")) {
          Serial.println("Turning music on.");
        }
        if (currentLine.endsWith("GET /music_off")) {
          Serial.println("Turning music off.");
        }
        
      }
    }
    // Close connection.
    client.stop();
    Serial.println("Client disconnected.");
  }
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.print("Point browser to http://");
  Serial.println(ip);
}
