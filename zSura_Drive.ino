#define enable_debug 0
#include <Sabertooth.h>
#include <Encoder.h>
//BALAJI
#include <PID_v1.h>
//#include <Psabertooth.h>/
#include <digitalWriteFast.h>
#include <PS3BT.h>
//Psabertooth l(2, 38400); // prev. used baud rate

Sabertooth ST(128, Serial2);
Encoder  myEnc(18, 19);//ADK
//PIN 18 19 interupt
Encoder myEnc2(2, 3);//ADK PIN 2 3 interupt
//   avoid using pins with LEDs attached
#define command_ 7


float loldPosition  = 0;
float roldPosition  = 0;
float lnewPosition  = 0;
float rnewPosition  = 0;

int speed = 0;
double dd = 0, dr = 0, dl = 0, dq = 0, W = 85, x = 0, y = 0, q = 0;
long speedMaxTurn = 55;
int MinSpeed = 25; //DeadSpeed
long speedMaxDrive = 75;
int stopMotor = 0;
long left, right;
double Kp = 1, Ki = 0, Kd = 0;
double Kpy = 2, Kiy = 0, Kdy = 0;
double errAdd, delta, requiredOrentation;
double errAdd_y;
double Setpoint = 0;
double Setpointdelta = 0;
int MinRampDistSq = 100 * 100;
long dist, prevDist = 2147483646;
int err = 10; // box err
double X2, Y2;  //Final position
boolean manual_controlled = false;
USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch


void setup() {
  Serial2.begin(38400);
  Serial.begin(115200);

  pinModeFast(command_, INPUT);
#if enable_debug == 0
  Serial.println ("connect ps3");
#endif

  PS3_init();
  Led_init();

}
void reset_coordinate()
{
  x = 0;  y = 0; q = 0;
  loldPosition = 0;
  roldPosition = 0;
  myEnc.write(0);
  myEnc2.write(0);
}
void cal(void)
{
  lnewPosition = float(myEnc.read());
  dl = (float(lnewPosition) - float(loldPosition)) * .1;
  if (lnewPosition != loldPosition) {
    loldPosition = lnewPosition;
  }

  rnewPosition = float(myEnc2.read());
  dr = (float(rnewPosition) - float(roldPosition)) * .1;
  if (rnewPosition != roldPosition) {
    roldPosition = rnewPosition;
  }

  //Serial.print("l=");
  //Serial.print(lnewPosition);
  //Serial.print('\t');
  //Serial.print("r=");
  //Serial.print(rnewPosition);
  //330.6cm in 20000 tick    *1.415608      4= 120cm   //331.24952939
  dd = ((dr + dl) * 120) / (200 * 77.5);
  dq = float(dr - dl) / W;
  x = x + dd * cos(((q + dq / 2) * 3.14) / 180);
  y = y + dd * sin(((q + dq / 2) * 3.14) / 180);
  q = q + dq;
  if (q > 180) q = q - 360;
  if (q <= -180) q = q + 360;
#if enable_debug
  
#endif
}

int angle;
int path;
void loop() {

  while (1)
  {
    //    cal();
    boolean b = PS3_update();

    if (PS3.getButtonClick(UP)) path = 1;
    if (PS3.getButtonClick(DOWN)) path = 2;
    if (PS3.getButtonClick(LEFT)) path = 3;
    if (PS3.getButtonClick(RIGHT)) path = 4;

    if (PS3.getButtonClick(TRIANGLE)) {
      MinSpeed += 5;
      speedMaxDrive += 5;
      speedMaxTurn += 5;
    }
    if (PS3.getButtonClick(CROSS)) {
      MinSpeed -= 5;
      speedMaxDrive -= 5;
      speedMaxTurn -= 5;
    }
    constrain(speedMaxDrive, 0, 125);
    constrain(MinSpeed, 0, speedMaxDrive);
    constrain(speedMaxTurn, 0, speedMaxDrive);
    ST.motor(1, left);
    ST.motor(2, right);
    if (b) break;
  }

  stopMotors();
  delay(100);

  switch (path)
  {
    case 1:
      Serial.println ("Ustarting");
      break;
    case 2: Serial.println ("Dstarting");
      break;
    case 3: Serial.println ("Lstarting");
      break;
    default : Serial.println ("Rstarting");
      break;
  }


#if enable_debug == 1
  Serial.println ("send k");
#endif
  while (!Serial.available() || Serial.read() != 'k');


  reset_coordinate();
#if enable_debug == 1
  cal();
  Serial.println ("reset");
#endif
  boolean Manualtransfer = false;
  switch (path)
  {
    case 1: while (!Manualtransfer)
      {
        while (!(((x < 300 + err) && (x > 300 - err)) && ((y < 0 + err) && (y > 0 - err))))
        {
          if (Goto(300, 0)) {
            Manualtransfer = true;
            break;
          }
        }
                while (!(((x < 300 + err) && (x > 300 - err)) && ((y < 300 + err) && (y > 300 - err))))
        {
          if (Goto(300, 300)) {
            Manualtransfer = true;
            break;
          }
        }
                while (!(((x < 0 + err) && (x > 0 - err)) && ((y < 300 + err) && (y > 300 - err))))
        {
          if (Goto(0, 300)) {
            Manualtransfer = true;
            break;
          }
        }
         while (!(((x < 0 + err) && (x > 0 - err)) && ((y < 0 + err) && (y > 0 - err))))
        {
          if (Goto(0, 0)) {
            Manualtransfer = true;
            break;
          }
        }
      }
      break;
    case 2: while (!Manualtransfer)
      {
        while (!(((x < 200 + err) && (x > 200 - err)) && ((y < 0 + err) && (y > 0 - err))))
        {
          if (Goto(200, 0)) {
            Manualtransfer = true;
            break;
          }
        }
                while (!(((x < 200 + err) && (x > 200 - err)) && ((y < 200 + err) && (y > 200 - err))))
        {
          if (Goto(200, 200)) {
            Manualtransfer = true;
            break;
          }
        }
                while (!(((x < 0 + err) && (x > 0 - err)) && ((y < 200 + err) && (y > 200 - err))))
        {
          if (Goto(0, 200)) {
            Manualtransfer = true;
            break;
          }
        }
       
        while (!(((x < 0 + err) && (x > 0 - err)) && ((y < 0 + err) && (y > 0 - err))))
        {
          if (Goto(0, 0)) {
            Manualtransfer = true;
            break;
          }
        }
      }
      break;
    case 3: while (!Manualtransfer)
      {
       while (!(((x < 300 + err) && (x > 300 - err)) && ((y < 0 + err) && (y > 0 - err))))
        {
          if (Goto(300, 0)) {
            Manualtransfer = true;
            break;
          }
        }
              
        while (!(((x < 0 + err) && (x > 0 - err)) && ((y < 0 + err) && (y > 0 - err))))
        {
          if (Goto(0, 0)) {
            Manualtransfer = true;
            break;
          }
        }
      }
      break;
    default:
      while (!Manualtransfer)
     {
        while (!(((x < 300 + err) && (x > 300 - err)) && ((y < 0 + err) && (y > 0 - err))))
        {
          if (Goto(300, 0)) {
            Manualtransfer = true;
            break;
          }
        }
                while (!(((x < 300 + err) && (x > 300 - err)) && ((y < 100 + err) && (y > 100 - err))))
        {
          if (Goto(300, 100)) {
            Manualtransfer = true;
            break;
          }
        }
                while (!(((x < 0 + err) && (x > 0 - err)) && ((y < 100 + err) && (y > 100 - err))))
        {
          if (Goto(0, 100)) {
            Manualtransfer = true;
            break;
          }
        }
       
        while (!(((x < 0 + err) && (x > 0 - err)) && ((y < 0 + err) && (y > 0 - err))))
        {
          if (Goto(0, 0)) {
            Manualtransfer = true;
            break;
          }
        }
     }
      break;
  }

  Serial.println("completed");
}

void stopMotors()
{
  {
    ST.motor(1, stopMotor);
    ST.motor(2, stopMotor);
  }
  delay(1000);
}



