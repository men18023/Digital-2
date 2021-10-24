
int led1 = PF_2;
int led2 = PF_1;
int led3 = PF_3;
int pushr = PA_2;
int pushl = PA_3;
int pushu = PA_4;
int pushd = PB_6;
int out1 = PC_6;
int out2 = PD_7;
int out3 = PD_6;
int out4 = PC_7;

int r = 0;
int l = 0;
int u = 0;
int d = 0;

void setup() {
  pinMode(pushr,INPUT_PULLUP);
  pinMode(pushl,INPUT_PULLUP);
  pinMode(pushu,INPUT_PULLUP);
  pinMode(pushd,INPUT_PULLUP);

  
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(out1,OUTPUT);
  pinMode(out2,OUTPUT);
  pinMode(out3,OUTPUT);
  pinMode(out4,OUTPUT);

}

void loop() {

    r = !digitalRead(pushr);
    l = !digitalRead(pushl);
    u = !digitalRead(pushu);
    d = !digitalRead(pushd);

    if (r){
      digitalWrite(led1, HIGH);
      digitalWrite(out1, HIGH);
    }
    if (l){
      digitalWrite(led2, HIGH);
      digitalWrite(out2, HIGH);
    }
    if (u){
      digitalWrite(led3, HIGH);
      digitalWrite(out3, HIGH);
    }
    if (d){
      digitalWrite(led2, HIGH);
      digitalWrite(led1, HIGH);
      digitalWrite(out4, HIGH);
    }
    else {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(out1, LOW);
      digitalWrite(out2, LOW);
      digitalWrite(out3, LOW);
      digitalWrite(out4, LOW);

    }
  
}
