/*
  AgruminoAT - AT commands interpreter project for Agrumino board using the Agrumino library.
  Created by Antonello Meloni & Hicham Lafhouli.
*/


#include "AgruminoAt.h"


/////////////////
// Constructor //
/////////////////

AgruminoAt::AgruminoAt() {

}

void AgruminoAt::setup_interpreter(Agrumino agrumino_instance) {

  agrumino = agrumino_instance;

  AT_funz6((char)0, this); //Legge la configurazione dalla flash
  if (config_pointer->sleep_time_sec <= 0 || config_pointer->sleep_time_sec > 4294) {
    config_pointer->sleep_time_sec = 2;
  }

  //Preparazione tabelle comandi ed help
  strcpy(&commands[0][0], "AT");
  strcpy(&help[0][0], "AT ready test");
  command_function[0] = AT_funz0;

  strcpy(&commands[1][0], "AT+Q");          //Esce dall'interprete
  strcpy(&help[1][0], "Quit AT interpreter");
  command_function[1] = AT_funz1;

  strcpy(&commands[2][0], "AT+SLPTIME");    //Visualizza il tempo di sleep_time - Se è presente il parametro: setta il tempo di sleep_time
  strcpy(&help[2][0], "With no parameter: returns sleep time in sec \nWith a numeric parameter within 1 and 4294 sets it");
  command_function[2] = AT_funz2;

  strcpy(&commands[3][0], "AT+BTLEV");      //Visualizza il livello della batteria
  strcpy(&help[3][0], "Return battery level");
  command_function[3] = AT_funz3;

  strcpy(&commands[4][0], "AT+TMP");        //Visualizza la temperatura
  strcpy(&help[4][0], "Return temperature (C°)");
  command_function[4] = AT_funz4;

  strcpy(&commands[5][0], "AT+SAVECFG");    //Salva la configurazione
  strcpy(&help[5][0], "Save configuration to flash memory");
  command_function[5] = AT_funz5;

  strcpy(&commands[6][0], "AT+READCFG");      //Legge la configurazione
  strcpy(&help[6][0], "Read configuration from flash memory");
  command_function[6] = AT_funz6;

  strcpy(&commands[7][0], "AT+HELP");         //Visualizza help
  strcpy(&help[7][0], "Print Help");
  command_function[7] = AT_funz7;

  strcpy(&commands[8][0], "AT+NOP");          //Nessuna operazione -> timeout
  strcpy(&help[8][0], "No operation");
  command_function[8] = AT_funz8;

  strcpy(&commands[9][0], "AT+LUX");          //Visualizza illuminazione
  strcpy(&help[9][0], "Return illuminance");
  command_function[9] = AT_funz9;

  strcpy(&commands[10][0], "AT+BTVOLT");       //Visualizza volt batteria
  strcpy(&help[10][0], "Return battery voltage");
  command_function[10] = AT_funz10;

  strcpy(&commands[11][0], "AT+SOIL");
  strcpy(&help[11][0], "Return soil moisture");
  command_function[11] = AT_funz11;

  strcpy(&commands[12][0], "AT+BRDON");
  strcpy(&help[12][0], "Turn board on");
  command_function[12] = AT_funz12;

  strcpy(&commands[13][0], "AT+BRDOFF");
  strcpy(&help[13][0], "Turn board off");
  command_function[13] = AT_funz13;

  strcpy(&commands[14][0], "AT+BTCHGR");
  strcpy(&help[14][0], "Return if battery is charging");
  command_function[14] = AT_funz14;

  strcpy(&commands[15][0], "AT+LEDON");
  strcpy(&help[15][0], "Turn LED on");
  command_function[15] = AT_funz15;

  strcpy(&commands[16][0], "AT+LEDOFF");
  strcpy(&help[16][0], "Turn LED off");
  command_function[16] = AT_funz16;

  strcpy(&commands[17][0], "AT+MSQSRV");
  strcpy(&help[17][0], "Print/Set Mosquitto Server address");
  command_function[17] = AT_funz17;

  strcpy(&commands[18][0], "AT+SSID");
  strcpy(&help[18][0], "Print/Set wifi access point SSID");
  command_function[18] = AT_funz18;

  strcpy(&commands[19][0], "AT+PSWD");
  strcpy(&help[19][0], "Print/Set wifi access point password");
  command_function[19] = AT_funz19;

  strcpy(&commands[20][0], "AT+PUBCH");
  strcpy(&help[20][0], "Print/Set Mosquitto Server pub channel");
  command_function[20] = AT_funz20;

  strcpy(&commands[21][0], "AT+SUBCH");
  strcpy(&help[21][0], "Print/Set Mosquitto Server sub channel");
  command_function[21] = AT_funz21;

  strcpy(&commands[22][0], "AT+WIFION");
  strcpy(&help[22][0], "Set WIFI AND Mosquitto client ON");
  command_function[22] = AT_funz22;

  strcpy(&commands[23][0], "AT+WIFIOFF");
  strcpy(&help[23][0], "Set WIFI AND Mosquitto client OFF");
  command_function[23] = AT_funz23;

  strcpy(&commands[24][0], "AT+DPSON");
  strcpy(&help[24][0], "Set deep_sleep ON");
  command_function[24] = AT_funz24;

  strcpy(&commands[25][0], "AT+DPSOFF");
  strcpy(&help[25][0], "Set deep_sleep OFF");
  command_function[25] = AT_funz25;

  //TODO implementare altri comandi -> gestione wifi, collegamento social etc
}


config* AgruminoAt::get_config() {
  return config_pointer;
}

Agrumino AgruminoAt::get_Agrumino() {
  return agrumino;
}

Row* AgruminoAt::get_commands() {
  return commands;
}

HelpRow* AgruminoAt::get_help() {
  return help;
}


/**
   Analizza l'input sulla porta seriale
*/
void AgruminoAt::serialIn(char* command, int counter) {
  char ch;
  int cicle = 1;
  int x = 0;

  while (cicle == 1) {
    if (Serial.available()) {
      counter = 1500;
      ch = Serial.read();
      Serial.print((char)ch);
      if (ch == 13 || ch==0 || ch==10 || x > ROW_LENGHT - 2) {
        Serial.println();
        command[x] = (char)0;
        x = 0;
        cicle = 0;
      } else {
        command[x++] = (char)ch;
      }

    }
    delay(20);
    counter--;
    if (counter < 1) {
      cicle = 0;
      strcpy(command, "AT+NOP");
    }
  }
}

/**
   Esegue l'input e interpreta i comandi
*/
void AgruminoAt::atInterpreter() {
  char command[ROW_LENGHT] = "";
  char execute[COMMAND_LENGHT] = "";
  char param[ROW_LENGHT] = "";
  boolean cicle = true;
  boolean endCommand = false;
  int x = 0;

  Serial.println();
  Serial.println("AT interpreter for Agrumino");
  Serial.println("Use AT+Q to quit");
  Serial.println("Use AT+HELP to print a little help");
  Serial.println();

  while (cicle) {

    serialIn(command, 1500);

    while (!endCommand) {
      endCommand = analize(command, execute, param);

      for (x = 0 ; x < COMMANDS_NUM; x++) {
        if (strcmp(execute, &commands[x][0]) == 0) {
          command_function[x](param, this);
          if (x == QUIT || !agrumino.isAttachedToUSB()) {
            cicle = false;
            agrumino.turnBoardOff();
            ESP.reset();
          }
          x = COMMANDS_NUM;
        }
      }
    }
    strcpy(command, "");
    strcpy(param, "");
    strcpy(execute, "");

    endCommand = false;
  }
}

/**
   Analizza il testo inserito in input ed estrae i comandi da eseguire
   ed i relativi parametri se presenti;
   se i comandi da eseguire sono terminati restituisce vero altrimenti
   restituisce falso
*/
boolean AgruminoAt::analize(char* command, char* execute, char* param) {
  int x = 0;
  int y = 0;
  int z = 0;
  int comStatus = 0;
  char newCommand[ROW_LENGHT] = "";

  for (x = 0; x < ROW_LENGHT; x++) {

    if (comStatus == 0) { //status 0 - single command mode
      if (command[x] == 61) {
        comStatus = 3;
        execute[x] = (char)0;
      } else if (command[x] == 59) {
        comStatus = 2;
        execute[x] = (char)0;
      } else if (command[x] == 0) {
        execute[x] = (char)0;
        strcpy(command, "");
        strcpy(param, "");
        return true;
      } else {
        execute[x] = command[x];
      }

    } else if (comStatus == 1) { //string parameter mode
      if (command[x] == 34 ) {
        comStatus = 3;
        param[z] = (char)0;
        /*x++;*/
      } else if (command[x] == 0) {
        param[z] = (char)0;
        strcpy(command, "");
        return true;
      } else {
      param[z++] = command[x];
      }

    } else if (comStatus == 2) { //multi command mode

      if (command[x] == 0) {
        newCommand[y] = (char)0;
        strcpy(command, &commands[0][0]);
        strcat(command, newCommand);
        return false;
      }
      newCommand[y++] = command[x];

    } else if (comStatus == 3) { //numeric integer parameter mode

      if (command[x] == 34 ) {
        comStatus = 1;
      } else if (command[x] == 59) {
        comStatus = 2;
        param[z] = (char)0;
      } else if (command[x] == 0) {
        param[z] = (char)0;
        return true;
      }
      if ((command[x] > 47 && command[x] < 58) || command[x] == 46) {
        param[z++] = command[x];
      }

    }
  }
  return false;
}




/////////////////////
// Command methods //
/////////////////////

/**
   AT
*/
void AgruminoAt::AT_funz0(char* param, AgruminoAt* agruminoAt) {
  Serial.println("OK");
  Serial.println();
}


/**
   AT+Q
*/
void AgruminoAt::AT_funz1(char* param, AgruminoAt* agruminoAt) {
  Serial.println("Quitting");
  Serial.println("OK");
  Serial.println();
}


/**
   AT+SLPTIME
*/
void AgruminoAt::AT_funz2(char* param, AgruminoAt* agruminoAt) {
  Agrumino agrumino = agruminoAt->get_Agrumino();
  if (strlen(param) > 0 && (atoi(param) <= 0 || atoi(param) > 4294)) {
    Serial.println("Invalid parameter value!");
    Serial.println("Error");
    Serial.println();
  } else {
    if (strlen(param) > 0) {
      agruminoAt->get_config()->sleep_time_sec = atoi(param);
    }
    Serial.println("+SLPTIME:< " + String(agruminoAt->get_config()->sleep_time_sec) + " >");
    Serial.println("OK");
    Serial.println();
  }
}


/**
   AT+BTLEV
*/
void AgruminoAt::AT_funz3(char* param, AgruminoAt* agruminoAt) {
  Agrumino agrumino = agruminoAt->get_Agrumino();
  Serial.println("+BTLEV:< " + String(agrumino.readBatteryLevel()) + "%>");
  Serial.println("OK");
  Serial.println();
}


/**
   AT+TMP
*/
void AgruminoAt::AT_funz4(char* param, AgruminoAt* agruminoAt) {
  Agrumino agrumino = agruminoAt->get_Agrumino();
  Serial.println("+TMP:< " + String(agrumino.readTempC()) + " °C>");
  Serial.println("OK");
  Serial.println();
}

/**
   AT+SAVECFG
*/
void AgruminoAt::AT_funz5(char* param, AgruminoAt* agruminoAt) {
  config* config_pointer = agruminoAt->get_config();
  Serial.println("Writing configuration to flash");
  EEPROM.begin(EEPROM_SIZE);

  uint8_t * byteStorage = (uint8_t *)config_pointer;
  for (size_t i = 0; i < sizeof(config); i++) {
    EEPROM.write(i, byteStorage[i]);
    //Serial.println(byteStorage[i]);
  }
  EEPROM.commit();
  EEPROM.end();
  Serial.println("OK");
  Serial.println();
}


/**
   AT+READCFG
*/
void AgruminoAt::AT_funz6(char* param, AgruminoAt* agruminoAt) {
  config* config_pointer = agruminoAt->get_config();

  Serial.println("Reading configuration from flash");
  EEPROM.begin(EEPROM_SIZE);
  uint8_t * byteStorageRead = (uint8_t *)config_pointer;
  for (size_t i = 0; i < sizeof(config); i++) {
    byteStorageRead[i] = EEPROM.read(i);
  }

  EEPROM.end();
  Serial.println("OK");
  Serial.println();
}

/**
   AT+HELP
*/
void AgruminoAt::AT_funz7(char* param, AgruminoAt* agruminoAt) {
  Serial.println("\nAT interpreter for Agrumino. \nList of AT Commands:");
  for (int x = 0 ; x < COMMANDS_NUM; x++) {
    Serial.print(agruminoAt->get_commands()[x]);
    Serial.print(" - ");
    Serial.println(agruminoAt->get_help()[x]);
  }
  Serial.println();
  Serial.println("Commands may be entered one per line or separated by a semicolon:");
  Serial.println("in the second case only the first cmd must start with [AT] and the others with [+].");
  Serial.println("All lines must end with a [CR] or a [LR].");
  Serial.println("A parameter can be send after the command and a [=] char.");
  Serial.println("A string parameter has to be sorrounded by quotation marks [\"], a numeric one must not.");
}


/**
   AT+NOP
*/
void AgruminoAt::AT_funz8(char* param, AgruminoAt* agruminoAt) {
  //timeout -> nop
}


/**
   AT+LUX
*/
void AgruminoAt::AT_funz9(char* param, AgruminoAt* agruminoAt) {
  Agrumino agrumino = agruminoAt->get_Agrumino();
  Serial.println("+LUX:< " + String(agrumino.readLux()) + " >");
  Serial.println("OK");
  Serial.println();
}


/**
   AT+BTVOLT
*/
void AgruminoAt::AT_funz10(char* param, AgruminoAt* agruminoAt) {
  Agrumino agrumino = agruminoAt->get_Agrumino();
  Serial.println("+BTVOLT:< " + String(agrumino.readBatteryVoltage()) + " >");
  Serial.println("OK");
  Serial.println();
}


/**
   AT+SOIL
*/
void AgruminoAt::AT_funz11(char* param, AgruminoAt* agruminoAt) {
  Agrumino agrumino = agruminoAt->get_Agrumino();
  Serial.println("+SOIL:< " + String(agrumino.readSoil()) + " >");
  Serial.println("OK");
  Serial.println();
}


/**
   AT+BRDON
*/
void AgruminoAt::AT_funz12(char* param, AgruminoAt* agruminoAt) {
  Agrumino agrumino = agruminoAt->get_Agrumino();
  Serial.println("+BRDON");
  if (!agrumino.isBoardOn()) {
    agrumino.turnBoardOn();
  }
  if (!agrumino.isBoardOn()) {
    Serial.println("Error");
  } else {
    Serial.println("OK");
  }
  Serial.println();
}


/**
   AT+BRDOFF
*/
void AgruminoAt::AT_funz13(char* param, AgruminoAt* agruminoAt) {
  Agrumino agrumino = agruminoAt->get_Agrumino();
  Serial.println("+BRDOFF");
  if (agrumino.isBoardOn()) {
    agrumino.turnBoardOff();
  }
  if (agrumino.isBoardOn()) {
    Serial.println("Error");
  } else {
    Serial.println("OK");
  }
  Serial.println();
}


/**
   AT+BTCHGR
*/
void AgruminoAt::AT_funz14(char* param, AgruminoAt* agruminoAt) {
  Agrumino agrumino = agruminoAt->get_Agrumino();
  Serial.print("+BTCHGR:< ");
  if (agrumino.isBatteryCharging()) {
    Serial.print("Battery is Charging");
  } else {
    Serial.print("Battery is not Charging");
  }
  Serial.println(" >");
  Serial.println("OK");
  Serial.println();
}


/**
   AT+LEDON
*/
void AgruminoAt::AT_funz15(char* param, AgruminoAt* agruminoAt) {
  Agrumino agrumino = agruminoAt->get_Agrumino();
  Serial.println("+LEDON");
  if (!agrumino.isLedOn()) {
    agrumino.turnLedOn();
  }
  if (!agrumino.isLedOn()) {
    Serial.println("Error");
  } else {
    Serial.println("OK");
  }
  Serial.println();
}


/**
   AT+LEDOFF
*/
void AgruminoAt::AT_funz16(char* param, AgruminoAt* agruminoAt) {
  Agrumino agrumino = agruminoAt->get_Agrumino();
  Serial.println("+LEDOFF");
  if (agrumino.isLedOn()) {
    agrumino.turnLedOff();
  }
  if (agrumino.isLedOn()) {
    Serial.println("Error");
  } else {
    Serial.println("OK");
  }
  Serial.println();
}


/**
   AT+MSQSRV
*/
void AgruminoAt::AT_funz17(char* param, AgruminoAt* agruminoAt) {
  if (strlen(param) > 0 && strlen(param) < LEN) {
    strcpy(agruminoAt->get_config()->mqtt_server, param);
  } else if (strlen(param) > 0) {
    Serial.println("Wrong param lenght!");
    Serial.println();
  }
  Serial.println("+MSQSRV:< " + String(agruminoAt->get_config()->mqtt_server) + " >");
  Serial.println("OK");
  Serial.println();
}



/**
   AT+SSID
*/
void AgruminoAt::AT_funz18(char* param, AgruminoAt* agruminoAt) {
  if (strlen(param) > 0 && strlen(param) < LEN) {
    strcpy(agruminoAt->get_config()->ssid, param);
  } else if (strlen(param) > 0) {
    Serial.println("Wrong param lenght!");
    Serial.println();
  }
  Serial.println("+SSID:< " + String(agruminoAt->get_config()->ssid) + " >");
  Serial.println("OK");
  Serial.println();
}



/**
   AT+PSWD
*/
void AgruminoAt::AT_funz19(char* param, AgruminoAt* agruminoAt) {
  if (strlen(param) > 0 && strlen(param) < LEN ) {
    strcpy(agruminoAt->get_config()->password, param);
  } else if (strlen(param) > 0) {
    Serial.println("Wrong param lenght!");
    Serial.println();
  }
  Serial.println("+PSWD:< " + String(agruminoAt->get_config()->password) + " >");
  Serial.println("OK");
  Serial.println();
}


/**
   AT+PUBCH
*/
void AgruminoAt::AT_funz20(char* param, AgruminoAt* agruminoAt) {
  if (strlen(param) > 0 && strlen(param) < LEN) {
    strcpy(agruminoAt->get_config()->channel_pub, param);
  } else if (strlen(param) > 0) {
    Serial.println("Wrong param lenght!");
    Serial.println();
  }
  Serial.println("+PUBCH:< " + String(agruminoAt->get_config()->channel_pub) + " >");
  Serial.println("OK");
  Serial.println();
}



/**
   AT+SUBCH
*/
void AgruminoAt::AT_funz21(char* param, AgruminoAt* agruminoAt) {
  if (strlen(param) > 0 && strlen(param) < LEN) {
    strcpy(agruminoAt->get_config()->channel_sub, param);
  } else if (strlen(param) > 0) {
    Serial.println("Wrong param lenght!");
    Serial.println();
  }
  Serial.println("+SUBCH:< " + String(agruminoAt->get_config()->channel_sub) + " >");
  Serial.println("OK");
  Serial.println();
}


/**
   AT+WIFION
*/
void AgruminoAt::AT_funz22(char* param, AgruminoAt* agruminoAt) {
  agruminoAt->get_config()->ok_start = 1;
  Serial.println("+WIFION");
  Serial.println("OK");
  Serial.println();
}


/**
   AT+WIFIOFF
*/
void AgruminoAt::AT_funz23(char* param, AgruminoAt* agruminoAt) {
  agruminoAt->get_config()->ok_start = 0;
  Serial.println("+WIFIOFF");
  Serial.println("OK");
  Serial.println();
}


/**
   AT+DPSON
*/
void AgruminoAt::AT_funz24(char* param, AgruminoAt* agruminoAt) {
  agruminoAt->get_config()->sleep_mode = 's';
  Serial.println("+DPSON");
  Serial.println("OK");
  Serial.println();
}


/**
   AT+DPSOFF
*/
void AgruminoAt::AT_funz25(char* param, AgruminoAt* agruminoAt) {
  agruminoAt->get_config()->sleep_mode = 'd';
  Serial.println("+DPSOFF");
  Serial.println("OK");
  Serial.println();
}
