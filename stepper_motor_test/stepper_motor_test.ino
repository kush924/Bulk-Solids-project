int driverPUL = 7;    // PUL- pin
int driverDIR = 8;    // DIR- pin
int t = 6;
int count = 0 , precount = 0 ;
int val1 , val2;
void setup() {
  pinMode (driverPUL, OUTPUT);
  pinMode (driverDIR, OUTPUT);
  pinMode (13,INPUT_PULLUP);
  pinMode (12,INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  count = micros();

  val1 = digitalRead(13);
  val2 = digitalRead(12);
   if (val1==0){
    fow();
   }
   if (val2==0){
    back();
   }
}

void fow(){
    digitalWrite(driverDIR,LOW);
    moov();
}

void back(){
    digitalWrite(driverDIR,HIGH);
    moov();

}
void moov(){
    if(count - precount >= t){
    digitalWrite(driverPUL,HIGH);
    }
    if(count - precount >= 2*t){
    digitalWrite(driverPUL,LOW);
    precount = count;
    }
}
