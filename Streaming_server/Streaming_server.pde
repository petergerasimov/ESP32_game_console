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
  size(1280, 720);
  ws= new WebsocketServer(this, 80, "/stream");
  //smooth(3);
  noSmooth();
  frameRate(60);

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

void draw() {
  currFrame=grabFrame(dimension, robot);
  currFrame.resize(2,2);
  image(currFrame, 0, 0, width, height);
  json.setInt("width",currFrame.width);
  json.setInt("height",currFrame.height);
  json.setJSONArray("frame", arrToJsonArr(currFrame.pixels));
  println(json);
  //robot.mouseMove(curX, curY); //GET DATA FROM SOCKET AND MOVE THE MOUSE
  ws.sendMessage(json.toString());
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

JSONArray arrToJsonArr(int arr[]){
  JSONArray jsonArr = new JSONArray();
  for (int i = 0; i < arr.length; i++) {
    jsonArr.setInt(i, arr[i]);
  }
  return jsonArr;
}

void printColor(int p){
  int r = (p>>16) & 0xff;
  int g = (p>>8) & 0xff;
  int b = p & 0xff;
  println(r + ":" + g + ":" + b);
}
