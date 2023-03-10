#ifndef ALL_DEF_H
#define ALL_DEF_H

//this file defines definitions for everything

const int vpin = 35;  //vertical
const int hpin = 34;  //horizontal
const int bpin = 15;  //button

const int play_led = 12;
const int rec_led = 13;

String filename;
long file_number=0;

const int headerSize = 44;
byte header[headerSize];

QueueHandle_t audioPlayQueue = NULL;
QueueHandle_t audioRecordQueue = NULL;

QueueHandle_t cmd_queue;

SemaphoreHandle_t Semaphore_Controls = NULL;

//enum menu_type {first, second, third, fourth, fifth, none};
enum menu_type {welcome, file_select, language_select,none};

//these are the commands from the joystick
enum cmd {mid,jleft, jright, up, down, push};

menu_type which_menu;
cmd current_cmd;


#endif
