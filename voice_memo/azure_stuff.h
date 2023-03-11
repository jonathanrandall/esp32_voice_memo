#ifndef AZURE_STUFF_
#define AZURE_STUFF_

#include <WiFi.h>
#include <HTTPClient.h>
#include "Oled_stuff.h"
#include <ArduinoJson.h>
//#include "C:\Users\jonny\Documents\Arduino\libraries\ArduinoJson-5.13.5\ArduinoJson.h"

const char *ssid = "WiFi-C5BF";
const char *password = "***";
bool reconnect = false;
const char *text;


void wifi_connect(){
  if(reconnect){
    WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    
    Serial.print(".");
  }

  //display.println(WiFi.localIP());
  //display.display();

  
  Serial.println(WiFi.localIP());
  delay(100);
  }
  
}

void wifi_disconnect(){
  if(reconnect){
    WiFi.disconnect(true);
  }
 
  delay(10);
}


//const int jsonSize = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(20) + 3 * JSON_OBJECT_SIZE(1) + 6 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 20 * JSON_OBJECT_SIZE(4) + 2 * JSON_OBJECT_SIZE(6);
StaticJsonDocument<512> doc;


void display_ready(){
  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
  display.setRotation(0);
  display.setTextColor(WHITE);
  //to invert use display.setTextColor(BLACK,WHITE);
  display.setTextSize(1);
  display.setCursor(0, 17);
  //display.setFont(&ArialUnicodeMS_Regular_16);
  //display.println("中文字符串");
  //display.display();
}

void translate(int lang ){
  wifi_connect();
 
  //french 0
  //italian 1
  //chinese 2
  String uri = "https://api.cognitive.microsofttranslator.com/translate?api-version=3.0&from=en&to=fr&to=it&to=zh-Hans";//detect";
  //uri = "https://australiaeast.stt.speech.microsoft.com/speech/recognition/conversation/cognitiveservices/v1?language=en-US";
 
  String endpoint = "https://api.cognitive.microsofttranslator.com/";
  String subscriptionKey = "***";
  String location="australiaeast";
  String to_post;
  HTTPClient client;
  client.begin(uri);
  client.addHeader("Ocp-Apim-Subscription-Key", subscriptionKey);
  client.addHeader("Ocp-Apim-Subscription-Region", location);
  client.addHeader("Content-Type","application/json");
  //client.addHeader("Content-Length", String(to_post.length()));
  // client.addHeader("Content-Type", "audio/wav");
  //text = "Hello";"{'text': 'Hello'}";
  to_post = String("[{'text': '") + String(text) + String("'}]");
  Serial.println(String(text));
  Serial.println("to_post");
  Serial.println(to_post);
  
  char *tmp1 = (char *) calloc(to_post.length()+1, sizeof(char));
  for(int i = 0; i<to_post.length(); i++){
    tmp1[i] = (to_post.c_str())[i];
  }
  Serial.print("tmp1: ");
  Serial.println(tmp1);
  int httpResponseCode = client.sendRequest("POST", (uint8_t *)tmp1, to_post.length());
  Serial.print("httpResponseCode : ");
  Serial.println(httpResponseCode);

   if(httpResponseCode == 200){
    display_ready();
    String response = client.getString();
    Serial.println("==================== Transcription ====================");
    //display.println(response);
    
    Serial.println(response);

    deserializeJson(doc, response);

    //[{"translations":[{"text":"Bonjour","to":"fr"},{"text":"Ciao","to":"it"},{"text":"你","to":"zh-Hans"}]}]
    text = doc[0]["translations"][lang]["text"];    
    //display.display();
    Serial.println(text);
    
    display.print(text);
    
    display.display();
    
    Serial.println("====================      End      ====================");//
    while(digitalRead(bpin)){
      //wait for button press.
    }
  }else{
    text = "Error";
    Serial.println("Error");
    Serial.println(client.getString());
  }
  client.end();  
   wifi_disconnect();
   if(tmp1){
     free(tmp1);
   }
}

void uploadFile(){
  
  wifi_connect();
  menu_struct *menu;
  menu = &fs_menu;
  uint16_t p1 = (*menu).pos;
  uint16_t i1 = (*menu).str_index[p1] + 1; //index of the first '\n'    
  uint16_t i2 = (*menu).str_index[p1+1]; //index of the second '\n'
  filename ="/" + ((*menu).menu_options.substring(i1,i2));

  file = SD.open(filename, FILE_READ);
  if(!file){
    file = SD.open("/"+filename, FILE_READ);
    if(!file){
      Serial.println(filename);
      Serial.println("FILE IS NOT AVAILABLE!");
      return;// "FILE IS NOT AVAILABLE!";
    }
  }

  Serial.println("===> Upload FILE to Node.js Server");
  //const char *text;

  String endpoint = "https://myresource4.cognitiveservices.azure.com/";
  String subscriptionKey = "****";
  
  String uri = "https://australiaeast.stt.speech.microsoft.com/speech/recognition/conversation/cognitiveservices/v1?language=en-US";
  String location="australiaeast";

  HTTPClient client;
  client.begin(uri);
  client.addHeader("Ocp-Apim-Subscription-Key", subscriptionKey);
  client.addHeader("Ocp-Apim-Subscription-Region", location);
  client.addHeader("Content-Type", "audio/wav");
  int httpResponseCode = client.sendRequest("POST", &file, file.size());
  Serial.print("httpResponseCode : ");
  Serial.println(httpResponseCode);
  

  if(httpResponseCode == 200){
    display_ready();
    String response = client.getString();
    Serial.println(response);
    //{"RecognitionStatus":"Success","Offset":5100000,"Duration":72400000,"DisplayText":"We are from Earth. We come in peace."}
    Serial.println("==================== Transcription ====================");
    //display.println(response);
    // Parse the json response: Arduino assistant
    deserializeJson(doc, response);
    //JsonObject& doc = jsonBuffer.parseObject(response);


    // if (!doc.success()) {
    //   Serial.println("JSON parsing failed!");
      
    // }
    text = doc["DisplayText"];    
    //display.display();
    
    Serial.println(text);
    display.println(text);
    display.display();

      

    Serial.println("====================      End      ====================");
    while(digitalRead(bpin)){
      
      //wait for button press.
    }
  }else{
    text = "Error";
    Serial.println("Error");
  }
  display.stopscroll();
  
  file.close();
  client.end();
  // return text;
   wifi_disconnect();
   
}

//*/
#endif
