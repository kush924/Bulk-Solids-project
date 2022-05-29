//loadcell
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//loadcell_pins:
const int HX711_dout = 2; //mcu > HX711 dout pin
const int HX711_sck = 3; //mcu > HX711 sck pin

//loadcell_HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);
//loadcell_variables
const int calVal_eepromAdress = 0;
unsigned long t = 0;

//stepper_variables
int driverPUL = 7;    // PUL- pin
int driverDIR = 8;    // DIR- pin
unsigned long int ms = 2930;
unsigned long int count = 0 , precount = 0 ;//precount not in use
int b1 , b2;
int p1=13 , p2=12;  //13 back , 12 fow

void setup() {
 
  Serial.begin(115200); delay(10);
  
//loadcell_begin
  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = -87.74; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    //while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("CLEARDATA");
    Serial.print("LABEL,Date,Time,Timer,");
    Serial.println("Load Cell Value");
  
  }
//loadcell_end

//stepper_begin
  pinMode (driverPUL, OUTPUT);
  pinMode (driverDIR, OUTPUT);
  pinMode (13,INPUT_PULLUP);
  pinMode (12,INPUT_PULLUP);
//stepper_end  
}

void loop() {

//loadcell_begin
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
//      Serial.print("Load_cell output val: ");
      Serial.print("DATA,DATE,TIME,TIMER,");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  // check if last tare operation is complete:
  if (LoadCell.getTareStatus() == true) {
    //Serial.println("Tare complete");
  }
//loadcell_end

//stepper_begin
  b1 = digitalRead(p1);
  b2 = digitalRead(p2);
   if (b1==0){
    back();
   }
   if (b2==0){
    fow();
   }
//stepper_end
}

//stepper_functions
void back(){
    digitalWrite(driverDIR,LOW);
    moov();
}

void fow(){
    digitalWrite(driverDIR,HIGH);
    moov();

}
void moov(){
    if((micros()-count)>=ms){
    digitalWrite(driverPUL,HIGH);
  }
  if((micros()-count)>=(ms*2)){
    digitalWrite(driverPUL,LOW);
  count=micros();
    }
}
