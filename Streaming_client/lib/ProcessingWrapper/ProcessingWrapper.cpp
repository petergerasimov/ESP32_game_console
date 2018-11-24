#include "ProcessingWrapper.h"

void Processing::size(int width,int height){
    Processing::width=width;
    Processing::height=height;
    GD.begin(0);
}

void Processing::background(uint32_t rgb){
    GD.swap();
    GD.get_inputs();
    GD.ClearColorRGB(rgb);
    GD.Clear();
}

void Processing::background(uint8_t r,uint8_t g,uint8_t b){
    GD.swap();
    GD.get_inputs();
    GD.ClearColorRGB(r,g,b);
    GD.Clear();
}

Processing::Rectangle::Rectangle(){
    x=0;
    y=0;
    width=0;
    height=0;
}

Processing::Rectangle::Rectangle(float x,float y,float width,float height){
    
}