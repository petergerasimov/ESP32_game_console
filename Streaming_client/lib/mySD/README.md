# microSD_ESP32


Hello everyone.

To be investigating to put microSD reader, to have in library for ESP32 and FT81x to make work the assets and images. At the moment go very well. I find from this link http://www.iotsharing.com/2017/05/how-to-use-arduino-esp32-to-store-data-to-sdcard.html Sharing a library to work very well with ESP32, also it is very configurable, we can put the analog pins in any of them to make the SPI bus constructor for the reader.

Until today the tests are very good. the reader module used is this;
<img src="https://www.waveshare.com/img/devkit/accBoard/Micro-SD-Storage-Board/Micro-SD-Storage-Board-4.jpg" alt="Placa" />

<img src="https://www.waveshare.com/img/devkit/accBoard/Micro-SD-Storage-Board/Micro-SD-Storage-Board-2.jpg" alt="Placa" />

Buy to eBay https://www.ebay.es/itm/Micro-SD-Card-Development-Kit-Storage-Memory-Board-SDIO-and-SPI-Interfaces/250885836925?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2060353.m2749.l2649] manufactured by WareShare, good reader. Library running attached in post or https://github.com/nhatuan84/esp32-micro-sdcard


# The constructor is:
Class SD:
- SD.begin(uint8_t cs , int8_t mosi , int8_t miso , int8_t sck): initialize library with SPI pins
- for example;
- while (!card.init(SPI_HALF_SPEED, 17, 27, 26, 16))   

//ATENCION A LA DEFINICION DE LOS PINES // 17=CS, 27=MOSI, 26=MISO y 16=SCLK}

* 100% recommended. I will carry out tests to adapt to the GD2ESP library
