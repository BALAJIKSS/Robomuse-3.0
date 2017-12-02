#include <Arduino.h>
#include <Ping.h>
#include <digitalWriteFast.h>

Ping ping1 = Ping(11);
Ping ping2 = Ping(7);
Ping ping3 = Ping(3);
Ping ping4 = Ping(2);

#define led 13
#define command_ 9
#define enable_debug 0

void setup() {
#if enable_debug
  Serial.begin(115200);
#endif
  pinModeFast(led, OUTPUT);
  pinModeFast(command_, OUTPUT);
  digitalWriteFast(led, 0);
  digitalWriteFast(command_, 0);
  delay(1000);
}
float d1, d2, d3, d4;
void loop() {
  ping1.fire();
  d1 = ping1.centimeters();
  delay(30);
 ping2.fire();
  d2 = ping2.centimeters();
  delay(30);
  ping3.fire();
  d3 = ping3.centimeters();
  delay(30);
  ping4.fire();
  d4 = ping4.centimeters();
  delay(30);
  #if enable_debug  
  Serial.print(d1);
  Serial.print("  ");
  Serial.print(d2);
  Serial.print("  ");
  Serial.print(d3);
  Serial.print("  ");
  Serial.print(d4);
#endif
int dist2=80;
if((d1>10 && d1<=dist2)||(d2>10 && d2<=dist2)||(d3>10 && d3<=dist2)||(d4>10 && d4<=dist2))
     { 
      #if enable_debug
       Serial.print("detected..");
      #endif 
       obstacle_detected();
          
       
     }
     else 
     {
      digitalWriteFast(command_,0);
      digitalWriteFast(led,0);
     }
     #if enable_debug
  Serial.println();
  #endif

}

void obstacle_detected()
{
  digitalWriteFast(led,1);
  digitalWriteFast(command_,1);
  delay(20);

}
