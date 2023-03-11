# esp32_voice_memo
 

An ESP32 voice memo recorder with azure powered speech to text and language translation… looks like we can finally finish building that great big tower we started on all those centuries ago.

In this video, I give an explanation of how I made this, including a description of the code used. The YouTube instrucitonal video is at https://youtu.be/l0EJ3O9tus8 

## The pin connections are as follows:
__SD Card (SPI bus):__   SCK -> 18, MOSI -> 23, MISO -> 19, CS -> 5

__Joystick:__ Vertical -> 35, Horizontal -> 34, button -> 15

__MAX98357 Amplifier (I2S):__  DIN -> 14, BCLK -> 26, LRC -> 27, GAIN -> GND

__MEMS INMP441 Microphone (I2S):__  WS -> 25, SD -> 33, SCK -> 32, L/R -> GND

__OLED:__  SDA -> 21, SCK-> 22

__LEDs:__  Green -> 12, Red -> 13

## Websites referenced in this video.
Dronebot workshop article on I2S: https://dronebotworkshop.com/esp32-i2s/ 
Eric Nam’s tutorial on wav recording with MEMS microphone: https://youtu.be/qmruNKeIN-o 
Eric Nam’s github code: https://github.com/0015/ThatProject/tree/master/ESP32_MICROPHONE/ESP32_INMP441_RECORDING 
Chris from Atomic14’s voice memo recorder: https://youtu.be/bVru6M862HY 
Chris from Atomic14’ github code: https://github.com/atomic14/esp32_sdcard_audio 
ESP32 Audio library: https://github.com/schreibfaul1/ESP32-audioI2S 
Xtronical I2S: https://www.xtronical.com/i2s-ep3/ 
Eric Nam’s tutorial on speech to text with Google: https://youtu.be/RZVzZfndIS0 
Azure tutorial: https://microsoftlearning.github.io/AI-900-AIFundamentals/instructions/04b-translate-text-and-speech.html 


## Parts List:

ESP32
https://www.aliexpress.com/item/1005001929935550.html

INMP441 Omnidirectional MEMS I2S microphone
https://www.aliexpress.com/item/4000045517597.html

MAX98357 I2S Audio Amplifier
https://www.aliexpress.com/item/1005003417970948.html

OLED screen
https://www.aliexpress.com/item/32957309383.html 

Joystick
https://www.aliexpress.com/item/32672164557.html

Breadboard
https://www.aliexpress.com/item/1742546890.html

Jumper wires
https://www.aliexpress.com/item/1005003219096948.html 

