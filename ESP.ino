#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <RemoteXY.h>

// RemoteXY BT settings
#define REMOTEXY_BLUETOOTH_NAME "ESP_32_C3"
#define REMOTEXY_ACCESS_PASSWORD "bestesesp123"

// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 94 bytes
{ 255,5,0,0,0,87,0,16,29,1,2,1,3,71,29,11,2,26,31,31,
83,101,110,115,111,114,0,80,104,111,110,101,0,10,48,38,74,19,19,4,
36,8,32,71,79,32,0,24,83,84,79,80,0,2,1,3,88,29,8,2,
26,31,31,77,111,116,46,79,78,0,77,111,116,46,111,102,102,0,4,48,
10,3,10,64,2,26,4,48,40,3,10,64,2,26 };

struct{// input variables
uint8_t switch_1; // = 1 bei Sensor, = 0 bei Phone         Sensor Switch
uint8_t pushSwitch_Engine; // = 1 bei Go, Stop = 0         Stop / Go Switch
uint8_t switch_2; // = 1 bei Mot.ON, = 0 bei Mot.OFF       Motor Switch
int8_t slider_links; // =-100..100 slider position
int8_t slider_rechts; // =-100..100 slider position
uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

const int SensorL = 0;
const int SensorR = 1;

const int SPEEDR = 255; //Unterschiedliche Geschwindigkeiten für beide Motoren. 
const int SPEEDL = 255;

const int SENSORGRENZE = 3000;

int lMotorPin1 = 2; //left Motor
int lMotorPin2 = 3;
int rMotorPin1 = 18;  //right Motor
int rMotorPin2 = 19;

void setup() {
  RemoteXY_Init();
  Serial.begin(115200);
  //PinMode for left Motor
  pinMode(lMotorPin1, OUTPUT);
  pinMode(lMotorPin2, OUTPUT);
  //PinMode for right Motor
  pinMode(rMotorPin1, OUTPUT);
  pinMode(rMotorPin2, OUTPUT);
}

void Vorwaertsfahren()
{
  analogWrite(lMotorPin1, 0);
  analogWrite(lMotorPin2, SPEEDR);
  analogWrite(rMotorPin1, 0);
  analogWrite(rMotorPin2, SPEEDL);
}

void linksfahren()
{
  analogWrite(lMotorPin1, 0);
  analogWrite(lMotorPin2, SPEEDR);
  analogWrite(rMotorPin1, 0);
  analogWrite(rMotorPin2, 0);
}

void rechtsfahren()
{
  analogWrite(lMotorPin1, 0);
  analogWrite(lMotorPin2, 0);
  analogWrite(rMotorPin1, 0);
  analogWrite(rMotorPin2, SPEEDL);
}

void stopp()
{
  analogWrite(lMotorPin1, 0);
  analogWrite(lMotorPin2, 0);
  analogWrite(rMotorPin1, 0);
  analogWrite(rMotorPin2, 0);
}

void loop() {
  RemoteXY_Handler();
  if(RemoteXY.switch_2 == 0){                                             // Falls der Switch auf Phone ist
    if(RemoteXY.slider_links >= 0){
      analogWrite(lMotorPin1, 0);
      analogWrite(lMotorPin2, RemoteXY.slider_links * 2);
    }
    else if(RemoteXY.slider_links < 0){
      analogWrite(lMotorPin1, RemoteXY.slider_links * -2);
      analogWrite(lMotorPin2, 0);
    }
    if(RemoteXY.slider_rechts >= 0){
      analogWrite(rMotorPin1, 0);
      analogWrite(rMotorPin2, RemoteXY.slider_rechts * 2.3);
    }
    else if(RemoteXY.slider_rechts < 0){
      analogWrite(rMotorPin1, RemoteXY.slider_rechts * -2.3);
      analogWrite(rMotorPin2, 0);
    }
  }
  else if(RemoteXY.switch_2 == 1 and RemoteXY.pushSwitch_Engine == 1){  //Falls der Switch auf Sensor ist und der Button auf Go
    Serial.println(analogRead(SensorL));
    Serial.println(analogRead(SensorR));
    //Falls beide Sensoren weiß erkennen, vorwärtsfahren
    if (analogRead(SensorR) <= SENSORGRENZE && analogRead(SensorL) <= SENSORGRENZE)
    {   
      Vorwaertsfahren();
    }
    //Falls der linke Sensor Schwarz erkennt, nach rechts fahren
    if(analogRead(SensorL) >= SENSORGRENZE)
    {
      rechtsfahren();
    }
    //Falls der rechte Sensor Schwarz erkennt, nach links fahren
    if (analogRead(SensorR) >= SENSORGRENZE)
    {
      linksfahren();
    }
    //Falls auf beiden Schwarz erkennt wird, anhalten.
    if (analogRead(SensorR) >= SENSORGRENZE && analogRead(SensorL) >= SENSORGRENZE)
    {
      stopp();
    }
  }
} 
