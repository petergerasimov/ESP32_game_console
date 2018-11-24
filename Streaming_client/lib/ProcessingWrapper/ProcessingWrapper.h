#include <new>
#include "GD23ZESP32.h"

#define draw() loop()

#define MouseX GD.inputs.x
#define MouseY GD.inputs.y

namespace Processing{
        uint16_t width,height;
        void size(int,int);
        void background();
        class Rectangle{
            public:
                float width,height,x,y;
                Rectangle();
                Rectangle(float,float,float,float);
                Rectangle(float,float,float,float,float);
        }rect;

};