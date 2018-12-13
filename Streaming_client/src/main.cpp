#include "GD23ZESP32.h" //GD2 should work but it doesn't
#include <vector>
#include <EEPROM.h>
#include <SPI.h>
// #include <string>

#include <WiFi.h>
#include <WiFiUdp.h>
// #include "transports/spidev.h"


using namespace std;

char host[] = "192.168.96.104";
int port=1337;

WiFiUDP udp;
uint8_t buffer[1317];
// static GDTransport GDTR;

void connectToWiFi(char *ssid,char *password){
    Serial.print("WIFI status = ");
    Serial.println(WiFi.getMode());
    WiFi.disconnect(true);
    delay(1000);
    WiFi.mode(WIFI_STA);
    delay(1000);
    Serial.print("WIFI status = ");
    Serial.println(WiFi.getMode());


    //TODO asdasdasdasdasd 
    WiFi.begin(ssid , password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup(){
    
    GD.begin(0);
    GD.self_calibrate();
    Serial.begin(9600);
    connectToWiFi("Nika","MnogoHubavaParolaZaWIFI");

    udp.begin(port);
}

void loop(){
    GD.Clear();
    GD.get_inputs();
    udp.parsePacket();
    
    if(udp.read(buffer, 1317) > 0){
        GD.cmd_loadimage(0, 0);

        int buffSize = sizeof(buffer);

        int offset = 512;
        for(int i=0;i<buffSize;i+=offset){
            offset=min(512, buffSize-i);
            uint8_t buff_buff[offset];
            for(int j=0;j<offset;j++){
                buff_buff[j] = buffer[j+i];
            }
            GD.copyram(buff_buff , (offset + 3) & ~3);
        }
        
        GD.Begin(BITMAPS);
        GD.Vertex2ii(0, 0);
    }
    GD.swap();
    
}

