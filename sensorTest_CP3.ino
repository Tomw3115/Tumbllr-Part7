// Contents of of file SensorTest.ino

#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include "sensor.h"

IR IR;
Ultrasonic Ultrasonic;

char Ultrasonic ::measure_flag=0;
unsigned long Ultrasonic::measure_prev_time=0;
static double Ultrasonic::distance_value;

int ir_sense = 0;
unsigned long previous_time = 0;

//add library to control led lights
#include <Adafruit_NeoPixel.h>

//add light controller
Adafruit_NeoPixel lights = Adafruit_NeoPixel(4, 3, NEO_GRB + NEO_KHZ800);

void Left_Receive()
  {
    IR.left_is_obstacle=1;
  }
void Right_Receive()
  {
    IR.right_is_obstacle=2;
  }

void IR::Send()
{
  static unsigned long ir_send_time;
  if (millis() - ir_send_time > 15)
    {
    for (int i = 0; i < 39; i++)
      {
        digitalWrite(IR_SEND_PIN, LOW);
        delayMicroseconds(9);
        digitalWrite(IR_SEND_PIN, HIGH);
        delayMicroseconds(9);
      }
      ir_send_time=millis();
    }
}

void IR::Pin_init()
{
  pinMode(LEFT_RECEIVE_PIN, INPUT_PULLUP);
  pinMode(RIGHT_RECEIVE_PIN, INPUT_PULLUP);
  pinMode(IR_SEND_PIN, OUTPUT);
  attachPCINT(digitalPinToPCINT(LEFT_RECEIVE_PIN), Left_Receive, FALLING);
  attachPCINT(digitalPinToPCINT(RIGHT_RECEIVE_PIN), Right_Receive, FALLING);
}

void IR::Check()
{
    //Serial.println("chacking IR");
    ir_sense = left_is_obstacle + right_is_obstacle;
    //Serial.println(ir_sense);
    left_is_obstacle = 0;
    right_is_obstacle = 0;
}

void IRLights(int sensor)
{
  lights.clear();
  lights.show();
  switch (sensor)
  {
    case 0:
      break;
    case 1:
      lights.setPixelColor(2, 100, 0, 0);
      lights.show();
      delay(10);
      break;
    case 2:
      lights.setPixelColor(3, 100, 0, 0);
      lights.show();
      delay(10);
      break;
    case 3:
      lights.setPixelColor(2, 100, 0, 0);
      lights.setPixelColor(3, 100, 0, 0);
      lights.show();
      delay(10);
      break;
  }
}

void Ultrasonic::Pin_init()
{
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
}

void Ultrasonic::Get_Distance()
{
  if (millis() - get_distance_prev_time > 50)
  {
    delayMicroseconds(1);
    get_distance_prev_time = millis();
    measure_flag = 0;
    attachPCINT(digitalPinToPCINT(ECHO_PIN), Distance_Measure, RISING);
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
  }
}

void Ultrasonic::Distance_Measure()
{
  if (measure_flag == 0)
  {
    measure_prev_time = micros();
    attachPCINT(digitalPinToPCINT(ECHO_PIN), Distance_Measure, FALLING);
    measure_flag = 1;
  }
  else if (measure_flag == 1)
  {
    distance_value = (micros() - measure_prev_time) * 0.017; //340.29 m/s / 2 -> (340.29*100 cm)/(1000*1000 us) / 2 = 0.0170145
    // divide by 2.54 to convert centimeters to inches
    distance_value /= 2.54;
    measure_flag = 2;
    //Serial.println(distance_value);
  }
}

float Ultrasonic::Check()
{
  float us_distance;
  if (measure_flag == 2)
    {us_distance = float(distance_value);}
  else
    {us_distance = 99.0;}
  Serial.println(us_distance);
  return us_distance;
}

void USLights(double dist)
{
  if (dist <= 3)
      {
        lights.setPixelColor(0, 100, 0, 0);  //red
        lights.setPixelColor(1, 100, 0, 0);
      }
  else if (dist >3 && dist <= 6)
    { 
      lights.setPixelColor(0, 110, 100, 12);  //yellow
      lights.setPixelColor(1, 110, 100, 12); 
    }
  else if (dist >6 && dist <= 12)
    {
      lights.setPixelColor(0, 0, 100, 0);  //green
      lights.setPixelColor(1, 0, 100, 0);
    }
  else if (dist > 12)
    {
      lights.setPixelColor(0, 100, 100, 100); //white
      lights.setPixelColor(1, 100, 100, 100);
    }
  lights.show();
  delay(10);
}

void setup() 
{
  
  IR.Pin_init();
  Ultrasonic.Pin_init();
  
  Serial.begin(115200);
  delay(100);

  // code added to the initialization routine for lights 
  lights.begin();  // initialize the lights
  lights.show();   // make sure it is visible
  lights.clear();  // Initialize all pixels to 'off' 
}

void loop() 
{  
  IR.Send();
  Ultrasonic.Get_Distance();
  IR.Check();
  IRLights(ir_sense);
  USLights(Ultrasonic.Check());
  
  delay(100); // repeat every 100 milliseconds
}


