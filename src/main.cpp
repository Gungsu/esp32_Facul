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

// put function declarations here:
#define MOTOR_SOB 32
#define MOTOR_DESC 23
#define BUT_RECRAVAR 4  //PNP01
#define LEVER_AUTO 16  // PNP02

void IRAM_ATTR onTimer();

medMv MedMv;
hw_timer_t *My_timer = NULL;
bool bloqueioMotor;

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

  if (MedMv.printNew) {
    int valorEmCorrente = (-3.179*MedMv.medTotal+9800);
    if (valorEmCorrente>500) {
      bloqueioMotor = true;
    } else if (valorEmCorrente<100) {
      bloqueioMotor = false;
    }
    Serial.print(MedMv.medTotal);
    Serial.print(" ");
    Serial.println(valorEmCorrente); //O valor mostrado considera com 2 casas decimais
  };
}

// put function definitions here:
void IRAM_ATTR onTimer()
{
  MedMv.newValue = map(analogRead(33), 4095, 0, 3300, 280);
  MedMv.calcMed();
}
