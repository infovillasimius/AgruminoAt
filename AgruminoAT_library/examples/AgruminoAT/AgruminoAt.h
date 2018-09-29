
/*
  AgruminoAT - AT commands interpreter project for Agrumino board using the Agrumino library.
  Created by Antonello Meloni & Hicham Lafhouli.
*/

#ifndef AgruminoAt_h
#define AgruminoAt_h

#include "Arduino.h"
#include <Agrumino.h>
#include <EEPROM.h>
#include "spi_flash.h"

#define ROW_LENGHT 255
#define HELP_LENGHT 128
#define COMMAND_LENGHT 15
#define COMMANDS_NUM 26
#define QUIT 1

#define EEPROM_SIZE 4096
#define LEN 20

typedef struct {
  uint32_t sleep_time_sec;
  char mqtt_server[LEN];
  char ssid[LEN];
  char password[LEN];
  char channel_sub[LEN];
  char channel_pub[LEN];
  uint8_t ok_start;
  char sleep_mode;
} config;

typedef char Row[COMMAND_LENGHT];
typedef char HelpRow[HELP_LENGHT];


class AgruminoAt {

  public:

    // Constructor
    AgruminoAt();
    void setup_interpreter(Agrumino agrumino_instance);

    // Public methods
    void serialIn(char* command, int counter);
    void atInterpreter();
    boolean analize(char* command, char* execute, char* param);

    config* get_config();
    Agrumino get_Agrumino();
    Row* get_commands();
    HelpRow* get_help();


  private:

    Agrumino agrumino;

    config* config_pointer = new config;

    char commands[COMMANDS_NUM][COMMAND_LENGHT];
    char help[COMMANDS_NUM][HELP_LENGHT];

    void (*command_function[COMMANDS_NUM])(char* , AgruminoAt*);

    // Command methods
    static void AT_funz0(char* param, AgruminoAt* agruminoAt);
    static void AT_funz1(char* param, AgruminoAt* agruminoAt);
    static void AT_funz2(char* param, AgruminoAt* agruminoAt);
    static void AT_funz3(char* param, AgruminoAt* agruminoAt);
    static void AT_funz4(char* param, AgruminoAt* agruminoAt);
    static void AT_funz5(char* param, AgruminoAt* agruminoAt);
    static void AT_funz6(char* param, AgruminoAt* agruminoAt);
    static void AT_funz7(char* param, AgruminoAt* agruminoAt);
    static void AT_funz8(char* param, AgruminoAt* agruminoAt);
    static void AT_funz9(char* param, AgruminoAt* agruminoAt);
    static void AT_funz10(char* param, AgruminoAt* agruminoAt);
    static void AT_funz11(char* param, AgruminoAt* agruminoAt);
    static void AT_funz12(char* param, AgruminoAt* agruminoAt);
    static void AT_funz13(char* param, AgruminoAt* agruminoAt);
    static void AT_funz14(char* param, AgruminoAt* agruminoAt);
    static void AT_funz15(char* param, AgruminoAt* agruminoAt);
    static void AT_funz16(char* param, AgruminoAt* agruminoAt);
    static void AT_funz17(char* param, AgruminoAt* agruminoAt);
    static void AT_funz18(char* param, AgruminoAt* agruminoAt);
    static void AT_funz19(char* param, AgruminoAt* agruminoAt);
    static void AT_funz20(char* param, AgruminoAt* agruminoAt);
    static void AT_funz21(char* param, AgruminoAt* agruminoAt);
    static void AT_funz22(char* param, AgruminoAt* agruminoAt);
    static void AT_funz23(char* param, AgruminoAt* agruminoAt);
    static void AT_funz24(char* param, AgruminoAt* agruminoAt);
    static void AT_funz25(char* param, AgruminoAt* agruminoAt);

};

#endif
