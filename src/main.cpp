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
      if (medTotalOLD != medTotal) {
        int64_t res = abs(medTotalOLD - medTotal);
        printNew = (res>30)?true:false;
        if(printNew) medTotalOLD = medTotal;
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

// put function declarations here:
#define MOTOR_SOB 32
#define MOTOR_DESC 23
#define BUT_RECRAVAR 4  //PNP01
#define LEVER_AUTO 16  // PNP02

void IRAM_ATTR onTimer();

medMv MedMv;
hw_timer_t *My_timer = NULL;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Setar interrupcao
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, 10000, true); //microseconds
  timerAlarmEnable(My_timer); // Just Enable

  pinMode(MOTOR_SOB, OUTPUT);
  pinMode(MOTOR_DESC, OUTPUT);
  pinMode(BUT_RECRAVAR, INPUT);
  pinMode(LEVER_AUTO, INPUT);

  // Setar __init__ de struct
  MedMv.flagMedInit = false;

  Serial.print("\nIniciei...\n");
}

int readButtons() {
  uint16_t resultValue = 0;
  if (!digitalRead(LEVER_AUTO)) {
    resultValue += 1;
  }
  if (!digitalRead(BUT_RECRAVAR)) {
    resultValue += 2;
  }
  return resultValue;
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(analogRead(33));
  switch (readButtons())
  {
  case 0:
    digitalWrite(MOTOR_SOB, 1);
    digitalWrite(MOTOR_DESC, 1);
    break;
  case 1:  //conf desligado
    digitalWrite(MOTOR_SOB, 1);
    digitalWrite(MOTOR_DESC, 1);
    break;
  case 2: //ativado para subir
    digitalWrite(MOTOR_DESC, 1);
    digitalWrite(MOTOR_SOB, 0);
    break;
  case 3: //ativado para descer
    digitalWrite(MOTOR_SOB, 1);
    digitalWrite(MOTOR_DESC, 0);
    break;
  default:
    break;
  }
  if (MedMv.printNew)
      Serial.println(MedMv.medTotal);
  //Serial.println(readButtons());
  //delay(500);
}

// put function definitions here:
void IRAM_ATTR onTimer()
{
  MedMv.newValue = map(analogRead(33), 4095, 0, 3300, 280); //1861 //1208
  MedMv.calcMed();
}