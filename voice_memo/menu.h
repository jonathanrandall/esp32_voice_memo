#ifndef MENU_STUFF_
#define MENU_STUFF_

#include "global_vars.h"

typedef struct menu_struct {
  menu_type this_menu;
  uint16_t pos =0;
  bool multiple; //does the menu go in multiple directions
  String menu_options; //string containing all options. Options are separated by a '\n'
  uint16_t numoptions;  //total number of menu options
  uint16_t *str_index; //index of option name in string
  menu_type *next_menu; //next menu type
  menu_type *prev_menu;
  //QueueHandle_t cmd_menu; //this will send out menu commands.
} menu_struct;

void set_str_index(menu_struct *menu);
menu_struct *get_menu_address(menu_type t1);

menu_struct welcome_menu;
menu_struct fs_menu; //file select menu
menu_struct language_menu; //language select menu

menu_type welcome_menu_next_menu[] = {none, none, file_select};
menu_type welcome_menu_prev_menu[] = {none, none, none};

menu_type fs_menu_next_menu[] = {language_select};
menu_type fs_menu_prev_menu[] = {welcome};

menu_type language_menu_next_menu[] = {none};
menu_type language_menu_prev_menu[] = {file_select};

void initMenus(){
  Semaphore_Controls = xSemaphoreCreateMutex();
  cmd_queue = xQueueCreate(5, sizeof(cmd));

  welcome_menu.menu_options = "\nSTOP\nRECORD\nPLAY\n";
  welcome_menu.multiple = true;
  welcome_menu.numoptions = 3;
  welcome_menu.this_menu = welcome;
  welcome_menu.next_menu = welcome_menu_next_menu;
  welcome_menu.prev_menu = welcome_menu_prev_menu;
  //welcome_menu.cmd_menu = xQueueCreate(1, sizeof(char *));
  set_str_index(&welcome_menu);

  fs_menu.menu_options = "\nall_files\n";
  fs_menu.multiple = false;
  fs_menu.numoptions = 1;
  fs_menu.this_menu = file_select;
  fs_menu.next_menu = fs_menu_next_menu;
  fs_menu.prev_menu = fs_menu_prev_menu;
  //fs_menu.cmd_menu = xQueueCreate(1, sizeof(char *));
  set_str_index(&fs_menu);

  language_menu.menu_options = "\nenglish\nfrench\nitalian\nchinese\n";
  language_menu.multiple = false;
  language_menu.numoptions = 4;
  language_menu.this_menu = language_select;
  language_menu.next_menu = language_menu_next_menu;
  language_menu.prev_menu = language_menu_prev_menu;
  //language_menu.cmd_menu = xQueueCreate(1, sizeof(char *));
  set_str_index(&language_menu);
}


menu_struct *get_menu_address(menu_type t1){
  //this returns the address of the particular menu
  switch(t1){
    case welcome:
      return &welcome_menu;
      break;
    case file_select:
      return &fs_menu;
      break;
    case language_select:
      return &language_menu;
      break;
    default:
      return &welcome_menu;
  }
}


void set_str_index(menu_struct *menu){
  uint16_t *tmp;
  tmp = (uint16_t *) calloc((*menu).numoptions+1, sizeof(uint16_t));
  int index = 0;
  int j = 0;
  index = ((*menu).menu_options).indexOf('\n',index);
  while(index != -1){
    tmp[j] = index;
    index++; j++;
    index = ((*menu).menu_options).indexOf('\n',index);
  }
  if((*menu).str_index) free((*menu).str_index);
  (*menu).str_index = tmp;
}

//need a function to run the menu


#endif
