#include <Arduino.h>
#include "ACS_read.h"

// put function declarations here:
#define MOTOR_SOB 32
#define MOTOR_DESC 23
#define BUT_RECRAVAR 4  //PNP01
#define LEVER_AUTO 16  // PNP02

bool bloqueioMotor;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Setar interrupcao
  start_acs();

  pinMode(MOTOR_SOB, OUTPUT);
  pinMode(MOTOR_DESC, OUTPUT);
  pinMode(BUT_RECRAVAR, INPUT);
  pinMode(LEVER_AUTO, INPUT);

  startTimer();

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

  delay(1000);
  Serial.println(String(get_medTotal()));
}