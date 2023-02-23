#define sig A0   // pin signal

#define sC 38  // pines de 'control'
#define sB 43
#define sA 42

#define ft0 0  // lecturas de los fototransistores
#define ft1 0
#define ft2 0
#define ft3 0
#define ft4 0
#define ft5 0
#define ft6 0
#define ft7 0



void setup(){
  Serial.begin(9600);
  
  pinMode(sC,OUTPUT);
  pinMode(sB,OUTPUT);
  pinMode(sA,OUTPUT);
  
}

void loop(){

  // Serial.println(lectura(0));

  Serial.println(lectura(1));

/*
  for (byte i=0; i<8; i++){
    Serial.print("Lectura del ft ");
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(lectura(i));
    delay(50);
  }
  */

  
  
  

}


int lectura(byte ft){
  switch (ft){
    case 0: binario(0); delay(10); return analogRead(sig); break;
    case 1: binario(1); delay(10); return analogRead(sig); break;
    case 2: binario(2); delay(10); return analogRead(sig); break;
    case 3: binario(3); delay(10); return analogRead(sig); break;
    case 4: binario(4); delay(10); return analogRead(sig); break;
    case 5: binario(5); delay(10); return analogRead(sig); break;
    case 6: binario(6); delay(10); return analogRead(sig); break;
    case 7: binario(7); delay(10); return analogRead(sig); break;
  }
}


void binario(byte pin){
  switch (pin){
    case 0: 
      digitalWrite(sC,0); 
      digitalWrite(sB,0); 
      digitalWrite(sA,0); break;
    case 1:
      digitalWrite(sC,0); 
      digitalWrite(sB,0); 
      digitalWrite(sA,1); break;
    case 2:
      digitalWrite(sC,0); 
      digitalWrite(sB,1); 
      digitalWrite(sA,0); break;
    case 3:
      digitalWrite(sC,0); 
      digitalWrite(sB,1); 
      digitalWrite(sA,1); break;
    case 4: 
      digitalWrite(sC,1); 
      digitalWrite(sB,0); 
      digitalWrite(sA,0); break;
    case 5: 
      digitalWrite(sC,1); 
      digitalWrite(sB,0); 
      digitalWrite(sA,1); break;
    case 6: 
      digitalWrite(sC,1); 
      digitalWrite(sB,1); 
      digitalWrite(sA,0); break;
    case 7: 
      digitalWrite(sC,1); 
      digitalWrite(sB,1); 
      digitalWrite(sA,1); break;
  }
}
