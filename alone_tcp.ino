#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else // ESP32
 #include <WiFi.h>
#endif

const char* ssid = "nothing1";
const char* password = "somu123456";

WiFiServer server(80);

// PIN LED
const int ledPin = 16;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        client.flush();

        if (request.indexOf("TOGGLE_LED") != -1) {
          ledState = !ledState;
          digitalWrite(ledPin, ledState);
        }

        // Send response to client
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("");
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");
        client.println("<p>LED State: " + String(ledState ? "ON" : "OFF") + "</p>");
        client.println("</html>");
        break;
      }
    }
    delay(1);
    client.stop();
  }
}
