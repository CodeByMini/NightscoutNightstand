#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <string.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

const char* wifiName = "yourSSID";
const char* wifiPass = "yourWifiPassword";

const char *host = "http://yournightscoutadress.herokuapp.com/api/v1/entries/current.json";

void setup() {
    Serial.begin(115200);
    delay(10);
    Serial.println();
  
    Serial.print("Connecting to ");
    Serial.println(wifiName);
    u8g2.begin();
    WiFi.begin(wifiName, wifiPass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    HTTPClient http;

    Serial.print("Request Link:");
    Serial.println(host);
  
    http.begin(host);
  
    int httpCode = http.GET();
    String payload = http.getString();

    Serial.print("Response Code:");
    Serial.println(httpCode);
    Serial.println(payload);
  
    if(httpCode == 200){
        DynamicJsonBuffer jsonBuffer(384);
        JsonArray& root = jsonBuffer.parseArray(payload);
        if (!root.success()) {
            Serial.println(F("Parsing failed!"));
            return;
        }
        float sgv = root[0]["sgv"].as<float>();
        sgv = sgv/18;
        char sgvCharAr[20];
        dtostrf(sgv,3,1,sgvCharAr);

        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_fub30_tf );
        u8g2.setCursor(15, 50);
        u8g2.print(sgvCharAr);
        u8g2.sendBuffer();
        delay(1000); 
    }else{
        Serial.println("Error in response");
    } 

    http.end();
  
    delay(600000);
}