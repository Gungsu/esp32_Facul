#include "ACS_read.h"
#include <Arduino.h>

struct medMv {
  uint16_t values[50];
  uint8_t index;
  uint16_t newValue;
  uint16_t medTotal;
  uint16_t medTotalOLD;

  int sumTot;
  bool flagMedInit;
  bool printNew;

  void calcMed() {
    values[index] = newValue;
    sumTot += newValue;
    if (flagMedInit) {
      if(index==49) {
        sumTot -= values[0];
        index = 0;
      } else {
        sumTot -= values[index+1];
        index++;
      }
      medTotal = sumTot/50;
      if (abs(medTotalOLD-medTotal) > 10) {
        medTotalOLD = medTotal;
        printNew = true;
      } else {
        printNew = false;
      }
    } else {
      if (index>=49) {
        index = 0;
        flagMedInit = true;
      } else {
        index++;
      }
    }
  }
};

medMv MedMv;
hw_timer_t *My_timer = NULL;

void IRAM_ATTR onTimer()
{
  MedMv.newValue = map(analogRead(33), 4095, 0, 0, 9800);
  MedMv.calcMed();
}

void start_acs() {
    My_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(My_timer, &onTimer, true);
    timerAlarmWrite(My_timer, 10000, true); //microseconds
    MedMv.flagMedInit = false;
}

void startTimer() {
    timerAlarmEnable(My_timer); // Just Enable
}

void stopTimer() {
    timerAlarmDisable(My_timer); // Just Enable
}

uint16_t get_medTotal() {
    return MedMv.medTotal;
}