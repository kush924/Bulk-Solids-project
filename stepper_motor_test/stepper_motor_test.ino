int driverPUL = 7;    // PUL- pin
int driverDIR = 8;    // DIR- pin
unsigned long int ms = 50;
unsigned long int count = 0 , precount = 0 ;
int b1 , b2;
int p1=13 , p2=12;  //13 back , 12 fow

void setup() {
  pinMode (driverPUL, OUTPUT);
  pinMode (driverDIR, OUTPUT);
  pinMode (13,INPUT_PULLUP);
  pinMode (12,INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {

  // b1 = digitalRead(p1);
  // b2 = digitalRead(p2);
  //  if (b1==0){
  //   back();
  //  }
  //  if (b2==0){
  //   fow();
  //  }
  fow();
}

void back(){
    digitalWrite(driverDIR,LOW);
    Serial.println("fow");
    moov();
}

void fow(){
    digitalWrite(driverDIR,HIGH);
    Serial.println("back");
    moov();

}
void moov(){
  if((micros()-count)>=ms){
    digitalWrite(driverPUL,HIGH);
    Serial.print("high");
  }//delay(t);
  if((micros()-count)>=(ms*2)){
    digitalWrite(driverPUL,LOW);
    Serial.print("low");
  count=micros();
  }//delay(t);  
}
