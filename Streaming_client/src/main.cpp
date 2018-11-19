#include "GD23ZESP32.h" //GD2 should work but it doesn't

#include <EEPROM.h>
#include <SPI.h>
//#include <string>

//Networking
#include <WiFi.h>
#include "WebSocketClient.h"
#include "ArduinoJson.h"

const char *ssid = "Nika";
const char *password = "MnogoHubavaParolaZaWIFI";

char path[] = "/stream";
char host[] = "192.168.0.104";

WebSocketClient webSocketClient;
WiFiClient client;

void setup()
{
    GD.begin(0);
    GD.self_calibrate();
    Serial.begin(9600);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // delay(5000);

    if (client.connect(host, 80))
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
int colorPixel=0;

String inputJSONMessage="";
String outputJSONMessage="";


void loop()
{
    // GD.ClearColorRGB(255, 0, 0);

    GD.Clear();
    GD.get_inputs();

    // StaticJsonBuffer<200> jsonBufferOutput; //THIS TAKES UP ALL OF THE RAM
    // JsonObject& outputJSONObject = jsonBufferOutput.createObject();
    // outputJSONObject["x"] = GD.inputs.x;
    // outputJSONObject["y"] = GD.inputs.y;
    // outputJSONObject.printTo(outputJSONMessage);

    if (client.connected())
    {
        webSocketClient.sendData(outputJSONMessage);

        webSocketClient.getData(inputJSONMessage);

        if (inputJSONMessage.length() > 0)
        {
            StaticJsonBuffer<200> inputJSONBuffer;
            JsonObject& inputJSONObject = inputJSONBuffer.parseObject(inputJSONMessage);
            xPixel=inputJSONObject["x"];
            yPixel=inputJSONObject["y"];
            colorPixel=inputJSONObject["color"];

            GD.Begin(POINTS);
            GD.ColorRGB(colorPixel);
            GD.Vertex2ii(xPixel, yPixel);

            // GD.Begin(RECTS);
            // GD.ColorRGB(colorPixel);
            // GD.Vertex2ii(0, 0);
            // GD.Vertex2ii(479, 271);


            // Serial.println(String(xFrame) + ":" + String(yFrame));
            // JsonArray& frame = inputJSONObject["frame"];
        }
    }
    else
    {
        Serial.println("Client disconnected.");
    }
    GD.swap();
}
