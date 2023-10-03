#include <WiFi.h>

#define PORT  1610  
#define MAX_CLIENTS 2

const char *ssid = "ESP32_AP";
const char *password = "112233445";
IPAddress local_IP(192, 168, 6, 16);
IPAddress gateway(192, 168, 6, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiServer server(PORT);
WiFiClient clients[MAX_CLIENTS];
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Setting soft-AP configuration ... ");

  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Telnet server started");
  server.begin();
}

void loop() {
  uint8_t i;
  // kiem tra co client moi ket noi khong
  if (server.hasClient())  {
    for (i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i] || !clients[i].connected())
      { if (clients[i]) clients[i].stop();
        clients[i] = server.available();
        Serial.print("New client: "); Serial1.print(i);
        continue;
      }
    }
    
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  // Kiem tra neu so client ket noi MAX_CLIENTS
  // co client, client duoc ket noi va o trang thai available
  // doc du lieu tu client, va gui lai du lieu cho client do.
  for (i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] && clients[i].connected()) {
      if (clients[i].available()) {
        String line = clients[i].readStringUntil('\r');

//        Serial.println(line); // AP2 gửi từ PING
        Serial.print("Server receive from Client:");
        Serial.println(line);
        //Gui thong tin hoai dap cho client
        String resp = String(line.reserve(line.length() - 1));
        Serial.print(" Then, response back to client:");
        Serial.println(resp);
//        clients[i].write(resp.c_str());
        clients[i].write("received");
        Serial.println();
      }
    }
  }
}
