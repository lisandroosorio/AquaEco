/***************************************************
 This example uses software solution to calibration the ph meter,
not the potentiometer. So it is more easy to use and calibrate.
 This is for SEN0161 and SEN0169.

 Created 2016-8-11
 By youyou from DFrobot <youyou.yu@dfrobot.com>

 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

#include <EEPROM.h>
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}
#define ReceivedBufferLength 20
char receivedBuffer[ReceivedBufferLength+1]; // store the serial command
byte receivedBufferIndex = 0; 
#define SCOUNT 30 // sum of sample point
int analogBuffer[SCOUNT]; //store the sample voltage
int analogBufferIndex = 0;
#define SlopeValueAddress 0 // (slope of the ph probe)store atthe beginning of the EEPROM. The slope is a float number,occupies4 bytes.
#define InterceptValueAddress (SlopeValueAddress+4)
float slopeValue, interceptValue, averageVoltage;
boolean enterCalibrationFlag = 0;
#define SensorPin A0
#define VREF 5000 //for arduino uno, the ADC reference is the power(AVCC), that is 5000mV
#include <FastLED.h>
#define LED_PIN     7
#define NUM_LEDS    7
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>
CRGB leds[NUM_LEDS];
Servo servo;
int angle = 10;
//temp pin 4
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


float fakeData[] = {
  65,//temp
  33,//turb
  0 //phph
  };

String commands[] = {
  ",",
  "",
  ","
};

float voltageTurb;
String data="";

void readCharacteristicValues()
{
 EEPROM_read(SlopeValueAddress, slopeValue);
 EEPROM_read(InterceptValueAddress, interceptValue);
 if(EEPROM.read(SlopeValueAddress)==0xFF && EEPROM.read(SlopeValueAddress+1)==0xFF && EEPROM.read(SlopeValueAddress+2)==0xFF && EEPROM.read(SlopeValueAddress+3)==0xFF)
 {
 slopeValue = 3.5; // If the EEPROM is new, the recommendatory slope is 3.5. 
 EEPROM_write(SlopeValueAddress, slopeValue);
 }
 if(EEPROM.read(InterceptValueAddress)==0xFF && EEPROM.read(InterceptValueAddress+1)==0xFF && EEPROM.read(InterceptValueAddress+2)==0xFF && EEPROM.read(InterceptValueAddress+3)==0xFF)
 {
 interceptValue = 0; // If the EEPROM is new, the recommendatory intercept is 0.
 EEPROM_write(InterceptValueAddress, interceptValue);
 }
} 


void turnOnLED(){

   leds[0] = CRGB(255, 255, 255);
  FastLED.show();
  
  leds[1] = CRGB(255, 255, 255);
  FastLED.show();
  
  leds[2] = CRGB(255, 255, 255);
  FastLED.show();

  leds[3] = CRGB(255, 255, 255);
  FastLED.show();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo.attach(8);
  servo.write(angle);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  turnOnLED();
  readCharacteristicValues(); //read the slope and intercept of the ph probe
  
  sensors.begin();
}
float calculatePH(){
 float test =0;
 for(int i=0;i<300;i++)
 {
    test +=analogRead(SensorPin)/1024.0*VREF; //read the voltage and store into the buffer,every 40ms
  
 }

 test = test/300;
 
  test=test/1000.0*slopeValue+interceptValue;

  return test;
}

float calculateTurb(){

  
  int i=0;

 // Serial.print(i);
  voltageTurb = 0;
  for(i=0;i<1000;i++)
  {
    
     int sensorValue = analogRead(A1);// read the input on analog pin 0:
 
      voltageTurb += sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):

    
  }
  float finalt = voltageTurb/1000;

  finalt =  -1120.4*square(finalt)+5742.3*finalt-4352.9; 
  if(finalt<0)
    finalt=0;
  if(finalt>3500)
    finalt=3500;
    
 
  return finalt;


  
}

float calculateTemp(){

    float temperatureToDivide=0;
    int i=0;
    for(i=0;i<10;i++)
    {
       sensors.requestTemperatures(); 
       temperatureToDivide = sensors.getTempFByIndex(0);
       
    }
  
  float toSend = temperatureToDivide/10;
  return temperatureToDivide;
    
  
}

void runServo(){

   for(angle = 10; angle < 180; angle++)  
  {                                  
    servo.write(angle);               
    delay(15);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 10; angle--)    
  {                                
    servo.write(angle);           
    delay(5);       
  } 
}
void loop() {
  // put your main code here, to run repeatedly:

    
    runServo();
    fakeData[0]= calculateTemp();
    fakeData[1]= calculateTurb();
    fakeData[2]= calculatePH();
    data=commands[1]+ fakeData[0]+""+commands[0] + "" + fakeData[1] +commands[0]+fakeData[2] + "\n";

    Serial.print(data);
  
  //for(int i=0;i<1000;++i){
    //delay(10000);
//    while(!Serial.available()==0){
//      data = Serial.readStringUntil('\n');
//      if(data.substring(0,3)=="ESP"){
//        //Serial.print("ignr ");
//        Serial.print(data);
//        Serial.print("\n");
//      }
//    }
      while(!Serial.available()){}

      String response = Serial.readStringUntil('\n');
      Serial.println(response);

      

    
  //}
}
