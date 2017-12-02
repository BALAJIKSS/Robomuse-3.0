int counter = 0;
int i = 0;

void check_for_obstacle()
{
  if (digitalReadFast(command_) && counter == 1)
  {
#if enable_debug
    
#endif
    long a = millis();
//    / STOP MOTORS
    stopMotors();
    float prevq;
    while (q != prevq && (millis() - a) < 1500) 
    {
      prevq = q;  //calc of final position
      cal();
    }
//    BLINK LED STRIP
    blinkled(3);
    red();
    Serial.println('k');
    while (!Serial.available());
//    GET INT
    i = Serial.parseInt();
    if (i == 0)
    { boolean b = false;
      while (1)
      { cal();
        b = PS3_update();
        ST.motor(1, left);
        ST.motor(2, right);
        if (b) break;
      }
      long a = millis();
      float prevq;
      stopMotors();
      while (q != prevq && (millis() - a) < 1500) {
        prevq = q;  //calc of final position
        cal();
      }
      requiredOrentation = atan2((Y2 - y), (X2 - x)) * 180 / PI;
     manual_controlled=true;
     counter=0;
    }
    else {blinkled(10);counter=0;}
   // (continues turn / drive)

  }
  else if (digitalReadFast(command_)) counter++;
  else {
    counter = 0;
  }
blue();
}
