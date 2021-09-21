
#define RED PF_1
#define BLUE PF_2
#define GREEN PF_3
#define PUSH1 PF_4
#define PUSH2 PF_0

// Variables
int cont1 = 0;
int cont2 = 0;

int led1[] = {PD_0, PD_1, PD_2, PD_3, PE_1, PE_2, PE_3, PA_7};
int led2[] = {PB_3, PC_4, PC_5, PC_6, PC_7, PD_6, PD_7, PA_2};

int flag = 0;
int state1 = 0;
int state2 = 0;
int var = 0;
int var2 = 0;
int act1 = 0;
int act2 = 0;
int temp1 = 0;
int temp2 = 0;
int pj1 = 0;

// declaración de funciones
void semaforo (void);  
void juego (void);

void setup() {  
  flag = 1;
  cont1 = 0;
  cont2 = 0;
  // Semaforo              
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT); 
  // botones juego
  pinMode (PUSH1, INPUT_PULLUP);
  pinMode (PUSH2, INPUT_PULLUP); 
  // config de leds     
  for (int i=0; i<8; i++){
    pinMode(led1[i], OUTPUT);
    digitalWrite(led1[i], LOW);
    }
  for (int i=0; i<8; i++){
    pinMode(led2[i], OUTPUT);
    digitalWrite(led2[i], LOW);
    }
  }

void loop() {
  state1 = digitalRead(PUSH1);
  state2 = digitalRead(PUSH2);
  
  if (state1 == HIGH){
    var = 1;
    }
  if (state2 == HIGH){
    var2 = 1;
    }
  if ((cont1 == 1 || cont2 == 1) && flag == 1){
    semaforo();
    flag = 0;
    }
  if (state1 == LOW && var == 1){
    cont1++;
    var = 0;
    if (flag == 0){
      juego();
    }
  }
  if (state2 == LOW && var2 == 1){
    cont2++;
    var2 = 0;
    if (flag == 0){
      juego();
    }
  }
}
/////////////////////////////////////////////////////////////
//                   secuencia de semaforo                 //
/////////////////////////////////////////////////////////////
void semaforo (void){
  digitalWrite(BLUE, LOW);  
  digitalWrite(RED, HIGH);    
  digitalWrite(GREEN, LOW);
  delay(1000); 
  digitalWrite(RED, HIGH);    
  digitalWrite(GREEN, HIGH);
  delay(1000); 
  digitalWrite(RED, LOW);    
  digitalWrite(GREEN, HIGH);
  delay(1000);
  digitalWrite(GREEN, LOW);
  }

/////////////////////////////////////////////////////////////
//                   secuencia de juego                    //
/////////////////////////////////////////////////////////////
void juego (void){
  act1 = 1;
  act2 = 1;
  if (act1 == 1){
    temp1 = cont1 - 1;
    digitalWrite(led1[cont1], HIGH);
    delay(100);
    }
  if (act2 == 1){
    temp2 = cont2 -1;
    digitalWrite(led2[cont2], HIGH);
    delay(100);
    }
  if (cont1 >= 8){
    cont1 = 0;
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, HIGH); 
    delay(500);
    setup();
    }
  if (cont2 >= 8){
    cont2 = 0;
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, HIGH); 
    delay(500);
    setup();
    } 
 }
