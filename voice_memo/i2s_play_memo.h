// most of the code snippets in this file are copied from
//    http://www.xtronical.com/i2s-ep3



#ifndef I2S_PLAY_
#define I2S_PLAY_

#include "i2s_record_memo.h"
#include "Audio.h"

Audio audio;

// I2S Connections
#define I2S_DOUT      14  //orange
#define I2S_BCLK      26   //yellow
#define I2S_LRC       27    //greeen

#define NUM_BYTES_TO_READ_FROM_FILE 1024 

static const i2s_port_t i2s_num = I2S_NUM_0;  // i2s port number    

bool ValidWavData(WavHeader_Struct* Wav);
bool FillI2SBuffer(byte* Samples,uint16_t BytesInBuffer);
uint16_t ReadFile(byte* Samples);
void DumpWAVHeader(WavHeader_Struct* Wav);
void PrintData(const char* Data,uint8_t NumBytes);

void i2sInit_play(){
  static const i2s_config_t i2s_config = 
  {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 44100,                                 // Note, this will be changed later
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,             // high interrupt priority
      .dma_buf_count = 8, //8,                                   // 8 buffers
      .dma_buf_len = 64, //64,                                    // 64 bytes per buffer, so 8K of buffer space
      .use_apll=0,
      .tx_desc_auto_clear= true, 
      .fixed_mclk=0    
  };
      
  // These are the physical wiring connections to our I2S decoder board/chip from the esp32, there are other connections
  // required for the chips mentioned at the top (but not to the ESP32), please visit the page mentioned at the top for
  // further information regarding these other connections.
      
  static const i2s_pin_config_t pin_config = 
  {
      .bck_io_num = I2S_BCLK,                           // The bit clock connectiom, goes to pin 27 of ESP32
      .ws_io_num = I2S_LRC,                             // Word select, also known as word select or left right clock
      .data_out_num = I2S_DOUT,                         // Data out from the ESP32, connect to DIN on 38357A
      .data_in_num = I2S_PIN_NO_CHANGE                  // we are not interested in I2S data into the ESP32
  };

  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
  i2s_set_pin(i2s_num, &pin_config);

}




//------------------------------------------------------------------------------------------------------------------------

//  Global Variables/objects    
    
//File file;                                 // Object for root of SD card directory

uint32_t BytesReadSoFar_1 = 1;
void i2s_play(){

  //install the its driver
  i2sInit_play();

  //turn on the green led
  digitalWrite(play_led, HIGH);
  delay(1000);
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(10); // 0...21
  bool isrunning=true;

  audio.connecttoSD(filename.c_str());

  // if(strcmp(fname,"PLAY")==0)  audio.connecttoSD(filename.c_str());
  //       else if(strcmp(fname, "STOP")==0) audio.stopSong();
  // else audio.connecttoSD(fname);

  int on =(digitalRead(bpin));
  while(!on){  //goes low when button pressed
    on = (digitalRead(bpin));
  }

  while(isrunning){
    audio.loop();
    
      if(!digitalRead(bpin)) audio.stopSong(); //if button is pressed stopped
      if (!audio.isRunning()) {
        if(isrunning){ //give back semaphore
          
          digitalWrite(play_led, LOW);
          isrunning = false;
        }
    }
  }

  //uninstall the i2s driver.
  clear_i2s_driver();  

}


#endif
