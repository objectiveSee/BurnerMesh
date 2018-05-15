/**
 * Code for displaying data on a small OLED display (optiona to boards)
 */


#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <Arduino.h>

void displaySetup();
void displayPrintStats(int connected_node_count);

#endif // _DISPLAY_H_
