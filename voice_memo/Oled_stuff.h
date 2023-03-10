#ifndef OLED_STUFF_
#define OLED_STUFF_


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
//#include <Fonts/ArialUnicodeMS_Regular_16.h>
//#include <Fonts/
#include "menu.h"
#include "i2s_record_memo.h"
#include "i2s_play_memo.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void uploadFile(); //this uploads the file for speecah to text
void translate(int lang);

const int DT = 1;

void testdrawrect();

void testfillrect();      // Draw rectangles (filled)

void testdrawcircle();    // Draw circles (outlines)

void testfillcircle();    // Draw circles (filled)

void testdrawroundrect(); // Draw rounded rectangles (outlines)

void testfillroundrect(); // Draw rounded rectangles (filled)

void testdrawtriangle();  // Draw triangles (outlines)

void testfilltriangle();  // Draw triangles (filled)


void initDisplay(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
//    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
  display.setRotation(0);
  display.setTextColor(WHITE);
  //to invert use display.setTextColor(BLACK,WHITE);
  display.setTextSize(1);
  display.setCursor(0, 17);
}

void display_menu(menu_type t1 ){

  menu_struct *menu2disp;
//  Serial.println("getting menu");
  menu2disp = get_menu_address(t1);
//  Serial.println("got menu");
  uint16_t pos = (*menu2disp).pos;
//  Serial.println("pos = " + String(pos));
  uint16_t numoptions = (*menu2disp).numoptions;

  uint16_t numdisp = (numoptions< pos+3)?numoptions:(pos+3);

  uint16_t strt=0;

  
  
  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 17);

  if(pos>0) strt=pos-1; //display the item before pos at the top of the menu

  if(pos+1 == numoptions && numoptions > 2) strt = pos-2; 

  numdisp = (numoptions<= strt+3)?numoptions:(strt+3);
//  Serial.println("mumdisp = " + String(numdisp));
  for(uint16_t i = strt; i<numdisp; i++){
//    Serial.println("here");
    uint16_t i1 = (*menu2disp).str_index[i] + 1; //index of the first '\n'
//    Serial.println("not crashed");    
    uint16_t i2 = (*menu2disp).str_index[i+1] - 0; //index of the second '\n'
//    Serial.println("i1 = " + String(i1) + ", i2 = " + String(i2));
    //str = '\noptions\n'
    String str = (*menu2disp).menu_options.substring(i1,i2);
//    Serial.println(str);
    display.setTextColor(WHITE);
    if (pos == i){
      display.println(">"+str);
    } else {
      display.println(str);
    }
    
//    Serial.println(str);
    display.display();
    delay(50);
  }
  
}

void do_stuff(String str){
  //do_stuff("PLAY")
//  Serial.println(str);
  //this is a function that does stuff when the menu option is selected.
  //semaphore so functions can use the joystick. We are displaying something other
  //than a menu to the oled, so we might want to use the joystick controls.
  xSemaphoreTake(Semaphore_Controls,  portMAX_DELAY);
  while(!digitalRead(bpin)){
    //wait for button to go hight, turn off that is.
  }
  
  if(strcmp(str.c_str(), "RECORD")==0){
    Serial.println("in record");
    file.close();
    filename =  String("/vm_") + String(file_number) + String(".wav");
    //Serial.println(filename.c_str());
    file_number++;
    
    file = SD.open(filename.c_str(), FILE_WRITE);
    file.write(header, headerSize);
    i2s_record();
    
    set_fs_menu_options(SD); //set the menu options
  }

  if(str.indexOf(".wav")!=-1 ){ //we are playing a song.
    Serial.println(str);
    filename = str;
    str = "PLAY";
    
  }

  if(strcmp(str.c_str(), "PLAY")==0){
    i2s_play();
  }

  if(strcmp(str.c_str(), "french")==0){
    
    uploadFile();
    delay(100);
    translate(0);    
  }

  if(strcmp(str.c_str(), "italian")==0){
    
    uploadFile();
    delay(100);
    translate(1);    
  }

  if(strcmp(str.c_str(), "chinese")==0){
    
    uploadFile();
    delay(100);
    translate(2);    
  }
  
  if(strcmp(str.c_str(), "english")==0){
    
    uploadFile();
    // translate();    
  }
    
    if(strcmp(str.c_str(),"drawrect")==0){
      testdrawrect();
//      delay(2000);
    }
  
    if(strcmp(str.c_str(),"fillrect")==0){
      testfillrect();      // Draw rectangles (filled)
  //    delay(DT);
    }
    
    if(strcmp(str.c_str(),"drawcircle")==0){
      testdrawcircle();    // Draw circles (outlines)
  //    delay(2000);
    }
  
    if(strcmp(str.c_str(),"fillcircle")==0){
      testfillcircle();    // Draw circles (filled)
  //    delay(2000);
    }
  
    if(strcmp(str.c_str(),"drawroundrect")==0){
      testdrawroundrect(); // Draw rounded rectangles (outlines)
  //    delay(2000);
    }
  
    if(strcmp(str.c_str(),"fillroundrect")==0){
      testfillroundrect(); // Draw rounded rectangles (filled)     
  //    delay(2000);
    }
    
    if(strcmp(str.c_str(),"drawtriangle")==0){
      testdrawtriangle();  // Draw triangles (outlines)
  //    delay(2000);
    }
  
    if(strcmp(str.c_str(),"filltriangle")==0){
      testfilltriangle();  // Draw triangles (filled)      
  //    delay(2000);
    }

  //give back control of the joystick.
  xSemaphoreGive(Semaphore_Controls);
  

}

void testdrawrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(DT);
}

void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(DT);
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(DT);
}

void testfillcircle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(DT);
}

void testdrawroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(DT);
}

void testfillroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawtriangle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(DT);
}

void testfilltriangle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(DT);
}

#endif
