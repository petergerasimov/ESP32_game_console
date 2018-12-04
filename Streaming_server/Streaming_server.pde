import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.AWTException;

import websockets.*;
WebsocketServer ws;

//PImage frame;
Rectangle dimension;
Robot robot;

int curX = 0;
int curY = 0;


void setup() {
  size(480, 272);
  ws= new WebsocketServer(this, 6969, "/stream");
  //smooth(3);
  noSmooth();
  frameRate(1);
  colorMode(RGB);
  imageMode(CORNER);
  background(0);

  dimension  = new Rectangle(displayWidth, displayHeight);
  try {
    robot = new Robot();
  }
  catch (AWTException cause) {
    println(cause);
    exit();
  }
}

PImage currFrame;
JSONObject json = new JSONObject();
int x=0;
int y=0;

void draw() {
  background(0);
  currFrame=grabFrame(dimension, robot);
  currFrame.resize(20, 20);
  image(currFrame, 0, 0, width, height);
  json.setJSONArray("frame", arrToJsonArr(currFrame.pixels));
  
  //for(x=0;x<currFrame.width;x++){
  //  for(y=0;y<currFrame.height;y++){
  //    json.setInt("x", x);
  //    json.setInt("y", y);
      //json.setInt("color", currFrame.pixels[(y*currFrame.width) + x]);
      ws.sendMessage(json.toString());
  //    stroke(currFrame.pixels[(y*currFrame.width) + x]);
  //    rect(x,y,1,1);
      //println(json.toString());
  //  }
  //}
  
  //println(json);
  //robot.mouseMove(curX, curY); //GET DATA FROM SOCKET AND MOVE THE MOUSE
}

PImage grabFrame(Rectangle dim, Robot bot) {
  return new PImage(bot.createScreenCapture(dim));
}

void webSocketServerEvent(String data) {
  JSONObject json = parseJSONObject(data);
  if (json == null) {
    println("JSONObject could not be parsed");
  } else {
    curX = json.getInt("x");
    curY = json.getInt("y");
  }
}

JSONArray arrToJsonArr(int arr[]) {
  JSONArray jsonArr = new JSONArray();
  for (int i = 0; i < arr.length; i++) {
    jsonArr.setInt(i, arr[i]);
  }
  return jsonArr;
}
