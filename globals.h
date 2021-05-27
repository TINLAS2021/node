#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "Arduino.h"
#include "AX12A.h"

#define NODE_ID 8
#define ID (14u)
#define OUT_TOPIC "bigSnake/output/8"
#define IN_TOPIC "bigSnake/input/8"
int initial_pos = 632;

#define DirectionPin   (10u)
#define BaudRate      (1000000ul)
#define DEVIATION 300

int max_val = DEVIATION;
int min_val = -DEVIATION;

int pos = initial_pos;

const char* ssid = "Tesla IoT";
const char* password = "fsL6HgjN";
const char* mqtt_server = "185.224.91.138";

const char* mqtt_username = "snake_node_";
const char* mqtt_password = "BigSnake1";

#endif _GLOBALS_H
