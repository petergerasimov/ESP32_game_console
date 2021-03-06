import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.AWTException;

import java.io.File;
import java.util.Iterator;
import javax.imageio.*;
import javax.imageio.stream.*;

//import javax.imageio.*;
import java.awt.image.*; 

import java.net.*;
import java.io.*;
import java.util.Arrays;



//PImage frame;
DatagramSocket ds; 
Rectangle dimension;
Robot robot;

int curX = 0;
int curY = 0;
int clientPort = 1337; 
int packetSize = 1460;

void setup() {
  size(480, 272);
  //smooth(3);
  noSmooth();
  frameRate(15);
  colorMode(RGB);
  imageMode(CORNER);
  background(0);
  
  try {
    ds = new DatagramSocket();
  } catch (SocketException e) {
    e.printStackTrace();
  }
  
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

int x=0;
int y=0;

int currRow;

void draw() {
  background(0);
  currFrame=grabFrame(dimension, robot);
  currFrame.resize(120, 68);
  image(currFrame, 0, 0, width, height);
  broadcast(currFrame,0.1);
}

PImage grabFrame(Rectangle dim, Robot bot) {
  return new PImage(bot.createScreenCapture(dim));
}


void broadcast(PImage img,float compression) {

  // We need a buffered image to do the JPG encoding
  BufferedImage bimg = new BufferedImage( img.width,img.height, BufferedImage.TYPE_INT_RGB );

  // Transfer pixels from localFrame to the BufferedImage
  img.loadPixels();
  bimg.setRGB( 0, 0, img.width, img.height, img.pixels, 0, img.width);

  // Need these output streams to get image as bytes for UDP communication
  ByteArrayOutputStream baStream  = new ByteArrayOutputStream();
  BufferedOutputStream bos    = new BufferedOutputStream(baStream);
  
  // Turn the BufferedImage into a JPG and put it in the BufferedOutputStream
  // Requires try/catch
  Iterator iter = ImageIO.getImageWritersByFormatName("jpeg");
  ImageWriter writer = (ImageWriter)iter.next();
  ImageWriteParam iwp = writer.getDefaultWriteParam();
  iwp.setCompressionMode(ImageWriteParam.MODE_EXPLICIT);
  iwp.setCompressionQuality(compression);
  try {
    ImageOutputStream ios = ImageIO.createImageOutputStream(bos);
    writer.setOutput(ios);
    IIOImage image = new IIOImage(bimg, null, null);
    writer.write(null, image, iwp);
  } 
  catch (IOException e) {
    e.printStackTrace();
  }

  writer.dispose();
  // Get the byte array, which we will send out via UDP!
  //byte[] packet = new byte[baStream.toByteArray().length+5];
  byte[] sizeOfImage = new byte[5];
  int temp = baStream.toByteArray().length;
  for(int i=0;i<5;i++){
    sizeOfImage[4-i] = (byte)(temp%10); 
    temp/=10;
  }
    
  if((sizeOfImage.length+baStream.toByteArray().length)<=packetSize){
    byte[] fill = new byte[packetSize - (baStream.toByteArray().length + sizeOfImage.length)];
    byte[] packet = connectByteArray(connectByteArray(baStream.toByteArray(),fill),sizeOfImage);
  // Send JPEG data as a datagram

  
    try {
      byte[] ipAddr = new byte[]{(byte)192,(byte)168,(byte)0,(byte)255};
      ds.send(new DatagramPacket(packet,packet.length, InetAddress.getByAddress(ipAddr),clientPort));
    } 
    catch (Exception e) {
      e.printStackTrace();
    }
  }
  else println("Frame too large :" + baStream.toByteArray().length);
}

 byte[] connectByteArray(byte[] a,byte[] b){
 byte[] connected = new byte[a.length+b.length];
 System.arraycopy(a, 0, connected, 0, a.length);
 System.arraycopy(b, 0, connected, a.length, b.length);
 return connected; 
}
