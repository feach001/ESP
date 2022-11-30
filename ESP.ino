/** Programm welches automatisch einer Linie folgt. Das ESP verhält sich so, dass es sich mit der schwarzen Linie zwischen beiden
Sensoren bewegt **/
const int SensorL = 0;
const int SensorR = 1;

const int SPEEDR = 255; //Unterschiedliche Geschwindigkeiten für beide Motoren. 
const int SPEEDL = 255;

const int SENSORGRENZE = 3000;

int lMotorPin1 = 2;
int lMotorPin2 = 3;
int rMotorPin1 = 18;
int rMotorPin2 = 19;

void setup() {
  pinMode(lMotorPin1, OUTPUT);
  pinMode(lMotorPin2, OUTPUT);
  pinMode(rMotorPin1, OUTPUT);
  pinMode(rMotorPin2, OUTPUT);
  Serial.begin(115200);
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
