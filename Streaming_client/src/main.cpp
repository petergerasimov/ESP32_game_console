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

void loop()
{
    GD.ClearColorRGB(255, 0, 0);
    GD.Clear();
    GD.get_inputs();

    String inputJSONMessage;
    String outputJSONMessage;
    StaticJsonBuffer<200> jsonBufferOutput;

    JsonObject& outputJSON = jsonBufferOutput.createObject();
    outputJSON["x"] = GD.inputs.x;
    outputJSON["y"] = GD.inputs.y;
    outputJSON.printTo(outputJSONMessage);

    
    

    if (client.connected())
    {
        webSocketClient.sendData(outputJSONMessage);
        webSocketClient.getData(inputJSONMessage);
        if (inputJSONMessage.length() > 0)
        {
            Serial.print("Received data: ");
            Serial.println(inputJSONMessage);
        }
    }
    else
    {
        Serial.println("Client disconnected.");
    }
    // delay(3000);
    GD.swap();
}
