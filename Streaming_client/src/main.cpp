#include "GD23ZESP32.h" //GD2 should work but it doesn't
#include <EEPROM.h>
#include <SPI.h>
#include <math.h>
#include "soc/rtc.h"
#include <WiFi.h>
#include <WiFiUdp.h>

using namespace std;

#define MTU 1460
#define CHUNK_SIZE 512

char host[] = "192.168.96.104";
int port=1337;


WiFiUDP udp;
uint8_t frame[MTU];
int frameSize = 0;

void connectToWiFi(char*,char*);
void loadJPEG(uint8_t*,int);
void recieveFrame(void*);

void setup(){
    rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);
    GD.begin(0);
    // GD.self_calibrate();
    Serial.begin(9600);
    connectToWiFi("Nika","MnogoHubavaParolaZaWIFI");
    udp.begin(port);
    xTaskCreatePinnedToCore( recieveFrame, "recieverTask",8096, NULL, 1, NULL ,0);
}

void loop(){
    double m=millis();
    GD.Clear();
    GD.get_inputs();
    

    if(frameSize>0){
        GD.cmd_loadimage(0, 0);
        loadJPEG(frame,frameSize);   
        GD.Begin(BITMAPS);

        GD.BitmapSize(NEAREST, BORDER, BORDER, 480, 272);
        GD.cmd_scale(F16(4), F16(4));
        GD.cmd_setmatrix();
        GD.Vertex2ii(0, 0);

        frameSize = 0;
    }

    GD.swap();

    Serial.println("FPS:" + String(1000/(millis()-m)));
}

void connectToWiFi(char *ssid,char *password){
    Serial.print("WIFI status = ");
    Serial.println(WiFi.getMode());
    WiFi.disconnect(true);
    delay(1000);
    WiFi.mode(WIFI_STA);
    delay(1000);
    Serial.print("WIFI status = ");
    Serial.println(WiFi.getMode());

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

void loadJPEG(uint8_t *jpeg,int size){
    int offset = CHUNK_SIZE;
    for(int i=0;i<size;i+=offset){
        offset=min(CHUNK_SIZE, size-i);
        GD.copyram(&jpeg[i] , (offset + 3) & ~3);
    }
}

void recieveFrame(void *p){
    while(true){
        if(frameSize==0){
            udp.parsePacket();
            if(udp.read(frame, MTU) > 0){
                for(int i=0;i<5;i++){
                    frameSize += frame[(sizeof(frame)/sizeof(frame[0]))-5+i]*pow(10,4-i);
                }
            }
        }
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
    

}