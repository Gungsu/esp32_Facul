#include <Arduino.h>

struct medMv;

void IRAM_ATTR onTimer();

void start_acs();

uint16_t get_medTotal();

void startTimer();

void stopTimer();