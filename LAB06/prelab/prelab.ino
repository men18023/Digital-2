
#define RED PF_1
#define BLUE PF_2
#define GREEN PF_3
#define PUSH1 PF_4
#define PUSH2 PF_0

// Variables
int state1 = 0;
int state2 = 0;
int cont = 0;
int push;
int led[] = {PB_3, PC_4, PC_5, PC_6, PC_7, PD_6, PD_7, PA_2};

void setup() {
  // put your setup code here, to run once:
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  for (int i=0; i<8; i++){
    pinMode(led[i], OUTPUT);

  }
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly: 
  state1 = digitalRead(PUSH1);

  int i = 0;
  int j = 0;
  if (digitalRead(PUSH1) == LOW) {
    for (i = 0; i < 256; i++) {
      for (j = 0; j < 8; j++) {
        if ( ((i >> j) & 1 )  == 1)  {
          digitalWrite(led[j], HIGH);
        }
        else {
          digitalWrite(led[j], LOW);
        }
      }
      delay(250);
    }
  }
  }
