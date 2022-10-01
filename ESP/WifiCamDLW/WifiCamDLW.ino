#include "WifiCam.hpp"
#include <WiFi.h>

static const char* WIFI_SSID = "HUY-IPHONE12PRO";
static const char* WIFI_PASS = "huyiphone";

esp32cam::Resolution initialResolution;

WebServer server(80);

void
setup()
{
  //LED indicator
  pinMode(33,OUTPUT);
  digitalWrite(33,1);
  Serial.begin(115200);
  Serial.println();
  delay(2000);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);


  while (WiFi.status() != WL_CONNECTED) {
  delay(200);
  digitalWrite(33,0);
  Serial.print(".");
  delay(200);
  digitalWrite(33,1);
  }
  
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi failure");
    delay(5000);
    ESP.restart();
  }
  Serial.println("WiFi connected");
  digitalWrite(33,0);
  {
    using namespace esp32cam;

    initialResolution = Resolution::find(1024, 768);

    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(initialResolution);
    cfg.setJpeg(80);

    
    bool ok = Camera.begin(cfg);
    if (!ok) {
      Serial.println("camera initialize failure");
      delay(5000);
      ESP.restart();
    }
    Serial.println("camera initialize success");
  }

  Serial.println("camera starting");
  Serial.print("http://");
  Serial.println(WiFi.localIP());

  addRequestHandlers();
  server.begin();
}

void
loop()
{
  server.handleClient();
}
