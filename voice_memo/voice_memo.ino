
//#include "Arduino.h"
//#include <driver/i2s.h>
//#include <SPIFFS.h>
//#include "SD.h"
//#include "FS.h"

//subscriptions key 567989cce4c248419ff643135bd84d1f
//region australiaeast
//endpoint https://myresource4.cognitiveservices.azure.com/

#include "azure_stuff.h"
//#include "Oled_stuff.h"
//#include "i2s_play_memo.h"
#include "global_vars.h"

uint16_t vert;
uint16_t horiz;
uint8_t button;
//i2s_zero_dma_buffer((i2s_port_t) m_i2s_num);
// void CreateQueues(){
//     audioPlayQueue = xQueueCreate(2, sizeof(char *));
//     audioRecordQueue = xQueueCreate(2, sizeof(char *));
// }

void get_joystick_data(void *args){
  cmd next_cmd;
  while(true){

    xSemaphoreTake(Semaphore_Controls,  portMAX_DELAY);

    vert = 0x0000 | analogRead(vpin); //12 bit ADC, 0 to 4095
    
    horiz = 0x0000 | analogRead(hpin);
    button = digitalRead(bpin);
    if(button==0){
      Serial.println("button");
      next_cmd = push; 
    } else if(vert > 3072){
      Serial.println("up");
      next_cmd = up;
    } else if(vert<1024){
      Serial.println("down");
      next_cmd = down;
    } else if(horiz > 3072){
      Serial.println("left");
      next_cmd = jleft;
    } else if(horiz <1024) {
      Serial.println("right");
      next_cmd = jright;
    } else {
      next_cmd = mid;
    }
    //send the command to the controller
    xQueueSend(cmd_queue, &next_cmd, portMAX_DELAY);
    xSemaphoreGive(Semaphore_Controls);
    vTaskDelay(600 / portTICK_PERIOD_MS);
  }
   
}

void run_menu(void *params){

  menu_struct *menu;
  
  cmd prev_cmd;

//  unsigned long time_since_last_change=0;

  prev_cmd = mid;
  //initialise the current menu to the first menu
  menu = &welcome_menu;
  menu_type t1;

  while(true){
    cmd rec_cmd; 
    String do_it;
    uint16_t p1;
    uint16_t i1; //index of the first '\n'    
    uint16_t i2 ; //index of the second '\n'
    
    xQueueReceive(cmd_queue, &rec_cmd, portMAX_DELAY);
    if(prev_cmd != rec_cmd){// || (millis()-time_since_last_change)>2000){ //are we receiving a new command
      switch(rec_cmd){
        case up:
          if((*menu).pos>0) { //if pos=0, already at the top.
            (*menu).pos--;
            display_menu((*menu).this_menu);
          }
          break;
        case down:
          if((*menu).pos<(*menu).numoptions-1) { //check if not at bottom
            (*menu).pos++;
            display_menu((*menu).this_menu);
          }
          break;
        case jright:
          if((*menu).multiple) {
            t1 = (*menu).next_menu[(*menu).pos]; //gets the name of the previous menu
          } else{
            t1 = (*menu).next_menu[0];
          }
          
          if(t1!=none){
            display_menu(t1);
            menu = get_menu_address(t1);
          }
          
          break;
        case jleft:
          if((*menu).multiple) {
            t1 = (*menu).prev_menu[(*menu).pos]; //gets the name of the previous menu
          } else{
            t1 = (*menu).prev_menu[0];
          }
          if(t1!=none){
            display_menu(t1);
            menu = get_menu_address(t1);
          }
          break;
        case push: //we have recieved a "push" enum from the joystick controls
          //extract the hightlighted instruction
          
          p1 = (*menu).pos;
          i1 = (*menu).str_index[p1] + 1; //index of the first '\n'    
          i2 = (*menu).str_index[p1+1]; //index of the second '\n'
          do_it = ((*menu).menu_options.substring(i1,i2));
          do_stuff(do_it.c_str());
          //xQueueSend((*menu).cmd_menu, &do_it, portMAX_DELAY);
          
          delay(100);
          //display_menu((*menu).this_menu);
          break;
        case mid:
          display_menu((*menu).this_menu);
          break;
      }
    }
    prev_cmd = rec_cmd;

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }

  
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(play_led, OUTPUT);
  pinMode(rec_led, OUTPUT);

  //turn on the red and green leds for 1s.

  digitalWrite(play_led, HIGH);
  digitalWrite(rec_led, HIGH);
  delay(1000);
  digitalWrite(play_led, LOW);
  digitalWrite(rec_led, LOW);

  //initialise the SD card. (connects to SPI)
  SDInit();

  //initialise the wave header for recording. Note FLASH_RECORD_SIZE is default for 20s.
  wavHeader(header, FLASH_RECORD_SIZE);

  //initialise menus
  initMenus();
  set_fs_menu_options(SD);

  latestFileSD(SD,"/");
  //CreateQueues();
  
  filename = String("/vm_") + String(file_number) + String(".wav");

  file_number++;
  // Serial.println("file_number = " + String(file_number));
    
  // i2sInit();
  delay(100);
  initDisplay();
  delay(20);

  // Wi-Fi connection
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    
    Serial.print(".");
  }

 
  
  Serial.println(WiFi.localIP());
  wifi_disconnect();

  delay(100);
  display_menu(welcome);
  //initAudio();

 xTaskCreatePinnedToCore(
       run_menu, /* Function to implement the task */
       "rm",    /* Name of the task */
       1024*4,            /* Stack size in words */
       NULL,            /* Task input parameter */
       2,               /* Priority of the task */
       NULL,     /* Task handle. */
       1);              /* Core where the task should run */
 
 delay(200);
 
 xTaskCreatePinnedToCore(
       get_joystick_data, /* Function to implement the task */
       "gjd",    /* Name of the task */
       1024*2,            /* Stack size in words */
       NULL,            /* Task input parameter */
       2,               /* Priority of the task */
       NULL,     /* Task handle. */
       1);              /* Core where the task should run */
 



  clear_i2s_driver();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //do_stuff(filename.c_str());
  //delay(15000);
  vTaskDelete(NULL);

}

void SDInit(){
  pinMode(SD_CS, OUTPUT);      
  digitalWrite(SD_CS, HIGH); 
  
  // Initialize SPI bus for microSD Card
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  
  // Start microSD Card
  if(!SD.begin(SD_CS))
  {
    Serial.println("Error accessing microSD card!");
    while(true); 
  }
}

void SPIFFSInit(){
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS initialisation failed!");
    while(1) yield();
  }

  SPIFFS.remove(filename);
  file = SPIFFS.open(filename, FILE_WRITE);
  if(!file){
    Serial.println("File is not available!");
  }

  byte header[headerSize];
  wavHeader(header, FLASH_RECORD_SIZE);

  file.write(header, headerSize);
  listSPIFFS();
}



void example_disp_buf(uint8_t* buf, int length)
{
    printf("======\n");
    for (int i = 0; i < length; i++) {
        printf("%02x ", buf[i]);
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("======\n");
}




void listSPIFFS(void) {
  Serial.println(F("\r\nListing SPIFFS files:"));
  static const char line[] PROGMEM =  "=================================================";

  Serial.println(FPSTR(line));
  Serial.println(F("  File name                              Size"));
  Serial.println(FPSTR(line));

  fs::File root = SPIFFS.open("/");
  if (!root) {
    Serial.println(F("Failed to open directory"));
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(F("Not a directory"));
    return;
  }

  fs::File file = root.openNextFile();
  while (file) {

    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.print(fileName);
    } else {
      String fileName = file.name();
      Serial.print("  " + fileName);
      // File path can be 31 characters maximum in SPIFFS
      int spaces = 33 - fileName.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      String fileSize = (String) file.size();
      spaces = 10 - fileSize.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      Serial.println(fileSize + " bytes");
    }

    file = root.openNextFile();
  }

  Serial.println(FPSTR(line));
  Serial.println();
  delay(1000);
}

void listSD(void) {
  Serial.println(F("\r\nListing SPIFFS files:"));
  static const char line[] PROGMEM =  "=================================================";

  Serial.println(FPSTR(line));
  Serial.println(F("  File name                              Size"));
  Serial.println(FPSTR(line));

  fs::File root = SD.open("/");
  if (!root) {
    Serial.println(F("Failed to open directory"));
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(F("Not a directory"));
    return;
  }

  fs::File file = root.openNextFile();
  while (file) {

    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.print(fileName);
    } else {
      String fileName = file.name();
      Serial.print("  " + fileName);
      // File path can be 31 characters maximum in SPIFFS
      int spaces = 33 - fileName.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      String fileSize = (String) file.size();
      spaces = 10 - fileSize.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      Serial.println(fileSize + " bytes");
    }

    file = root.openNextFile();
  }

  Serial.println(FPSTR(line));
  Serial.println();
  delay(1000);
}
