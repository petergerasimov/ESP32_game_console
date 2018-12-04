#include "GD23ZESP32.h" //GD2 should work but it doesn't

#include <EEPROM.h>
#include <SPI.h>
#include <cJSON.h>
#include <string>


//Networking
#include <WiFi.h>
#include "WebSocketClient.h"
// #include "ArduinoJson.h"


using namespace std;



const char *ssid = "elsys";
const char *password = "";

char path[] = "/stream";
char host[] = "192.168.97.180";

WebSocketClient webSocketClient;
WiFiClient client;

void setup(){
    
    GD.begin(0);
    GD.self_calibrate();
    Serial.begin(9600);
    //DO NOT TOUCH
    //  This is here to force the ESP32 to reset the WiFi and initialise correctly.
    Serial.print("WIFI status = ");
    Serial.println(WiFi.getMode());
    WiFi.disconnect(true);
    delay(1000);
    WiFi.mode(WIFI_STA);
    delay(1000);
    Serial.print("WIFI status = ");
    Serial.println(WiFi.getMode());
    // End silly stuff !!!


    WiFi.begin(ssid);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // delay(5000);

    if (client.connect(host, 6969))
    {
        Serial.println("Connected");
    }
    else
    {
        Serial.println("Connection failed.");
    }

    webSocketClient.path = path;
    webSocketClient.host = host;

    if (webSocketClient.handshake(client))
    {
        Serial.println("Handshake successful");
    }
    else
    {
        Serial.println("Handshake failed.");
    }
    
}

int xPixel=0;
int yPixel=0;
int colorPixel = 0;


String inputJSONMessage;
String outputJSONMessage = "dab";

void loop(){
    cJSON *inputJSON;

    GD.Clear();
    GD.get_inputs();
    if (client.connected())
    {
        int color = 0;
        webSocketClient.sendData(outputJSONMessage);

        webSocketClient.getData(inputJSONMessage);
        // if (inputJSONMessage.length() > 0)
        // {
        inputJSON = cJSON_Parse(inputJSONMessage.c_str());
        if(inputJSON!=NULL){
            cJSON * frame = cJSON_GetObjectItem(inputJSON, "frame");

            for (int i = 0 ; i <cJSON_GetArraySize(frame) ; i++){
                color = cJSON_GetArrayItem(frame, i)->valueint;
                
                int x = i%30;
                int y = i/30; 

                // GD.Begin(POINTS);
                // GD.ColorRGB(color);
                // GD.Vertex2ii(x,y);

                GD.Begin(RECTS);
                GD.ColorRGB(color);
                GD.Vertex2ii(x*16, y*16);
                GD.Vertex2ii((x+1)*16, (y+1)*16);
            }
            cJSON_Delete(inputJSON);
        }
    }
    else
    {
        Serial.println("Client disconnected.");
    }
    GD.swap();
    
}