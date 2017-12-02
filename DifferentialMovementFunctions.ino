
//    PID myPID(&current position, &output variable, &required position, Kp, Ki, Kd, DIRECT);
double j = 0, fi;

PID myPID(&fi, &errAdd, &j, Kp, Ki, Kd, DIRECT);
//PID myPIDy(&delta, &errAdd_y, &Setpointdelta, Kpy, Kiy, Kdy, DIRECT);
boolean MoveStraight(double X1, double Y1, double X2, double Y2) //The bot should already be in the required orentation
{

  requiredOrentation = atan2((Y2 - Y1), (X2 - X1)) * 180 / PI;
  double b = ( ((Y2 - Y1) * X1) - ((X2 - X1) * Y1) ) / (X2 - X1);
  double a = (Y2 - Y1) / (X2 - X1);
  double c = sqrt(1 + (a * a));

  
  if (DistanceSq (X1, Y1, X2, Y2) < 2 * MinRampDistSq)
  {
    myPID.SetOutputLimits(-MinSpeed, MinSpeed);
    myPID.SetMode(AUTOMATIC);
    //    myPIDy.SetOutputLimits(-MinSpeed, MinSpeed);
    //    myPIDy.SetMode(AUTOMATIC);
    
    while (!(((x < X2 + err) && (x > X2 - err)) && ((y < Y2 + err) && (y > Y2 - err))))
    {
      cal();
      //    delta=sign(requiredOrentation-atan2((y-Y1),(x-X1)) )*fabs(y+(a*x)+(b)/c);
      requiredOrentation = atan2((Y2 - y), (X2 - x)) * 180 / PI;
      fi = Rotate(requiredOrentation, q);
      myPID.Compute();
      //    myPIDy.Compute();
      Usb.Task();
      if (PS3.PS3Connected || PS3.PS3NavigationConnected) {

        if ( PS3.getButtonClick(SELECT)) {
        return true;}

      }
#if enable_debug

#endif
      check_for_obstacle();
      if (manual_controlled) 
      {
        break;
        
      }
      int lvalue = (int)(-MinSpeed - errAdd); //Serial.print(delta);
      int rvalue = (int)(-MinSpeed + errAdd); //-errAdd_y
      ST.motor(1, -lvalue); ST.motor(2, -rvalue);

    }
    long a = millis();
    float prevq;
    stopMotors();
    while (q != prevq && (millis() - a) < 1500) {
      prevq = q;  //calc of final position
      cal();
    }
  }
  else     //Ramping part
  {
#if enable_debug
    Serial.println(" Ramping partttttttttttttttttttttttttttttttttttttttttttt:");
#endif
    myPID.SetOutputLimits(-speedMaxDrive, speedMaxDrive);
    myPID.SetMode(AUTOMATIC);
    //    myPIDy.SetOutputLimits(-speedMaxDrive, speedMaxDrive);
    //    myPIDy.SetMode(AUTOMATIC);
    long distFrmStrt = DistanceSq (x, y, X2, Y2);
    prevDist = 2147483646;
    while (!(((x < X2 + err) && (x > X2 - err)) && ((y < Y2 + err) && (y > Y2 - err))))
    {
      cal();
      dist = DistanceSq (x, y, X2, Y2);
      if (dist > prevDist) {

#if enable_debug
       
#endif
        break;
      }
      //delta=sign(requiredOrentation-atan2((y-Y1),(x-X1)) )*fabs(y+(a*x)+(b)/c);
      requiredOrentation = atan2((Y2 - y), (X2 - x)) * 180 / PI;
      fi = Rotate(requiredOrentation, q);
      myPID.Compute();
      //    myPIDy.Compute();
      Usb.Task();
      if (PS3.PS3Connected || PS3.PS3NavigationConnected) {

        if ( PS3.getButtonClick(SELECT)) {
        return true;}

      }
#if enable_debug
      
#endif

      int TempMaxSpeed = speedMaxDrive;

      if (distFrmStrt - dist < MinRampDistSq) {
        TempMaxSpeed = (unsigned)(MinSpeed + (speedMaxDrive - MinSpeed) * (distFrmStrt - dist) / MinRampDistSq);
      }
      if (dist <= MinRampDistSq) {
        TempMaxSpeed = (unsigned)(MinSpeed + (speedMaxDrive - MinSpeed) * dist / MinRampDistSq);

      }
      if (TempMaxSpeed < 0) {
        TempMaxSpeed = 0;
      }
#if enable_debug
     
#endif
      int lvalue = (int)(-TempMaxSpeed - errAdd * TempMaxSpeed / speedMaxDrive); //Serial.print(delta);
      int rvalue = (int)(-TempMaxSpeed + errAdd * TempMaxSpeed / speedMaxDrive); //-errAdd_y
      //    Serial.print(" lvalue:");
      //    Serial.print(lvalue);
      //    Serial.print(" rvalue:");
      //    Serial.print(rvalue);
      ST.motor(1, -lvalue); ST.motor(2, -rvalue);
      prevDist = dist;
      check_for_obstacle();
      if (manual_controlled) break;
    }

    long a = millis();
    float prevq;
    stopMotors();
    while (q != prevq && (millis() - a) < 1500) {
      prevq = q;  //calc of final position
      cal();
    }


  }

  return false;
}
boolean Turn(double qr, double err)
{
  if ((qr - q > 0 && qr - q < 180) || (qr - q < 0 && qr - q < -180))
  { double prevDiff, curDiff;
    while (q < qr - err || q > qr + err)
    { //rotate anticlockwise
      //      curDiff = fabs(q - qr);
      //      if (curDiff > prevDiff && sign(q) == sign(qr)) break;
      //      prevDiff = curDiff;
      ST.motor(1, speedMaxTurn); ST.motor(2, -speedMaxTurn);
      cal();
      check_for_obstacle();
      Usb.Task();
      if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
        if ( PS3.getButtonClick(SELECT)) 
        return true;
      }
      if (qr > (180 - err) || qr < (-180 + err)) {
        if (q > (180 - err) || q < (-180 + err)) break;
      }
      if (manual_controlled) break;
    }
    long a = millis();
    float prevq;
    stopMotors();
    while (q != prevq && (millis() - a) < 1500) {
      prevq = q;  //calc of final position
      cal();
    }
  }

  else
  {
    double prevDiff, curDiff;
    while (q > qr + err || q < qr - err)
    { //rotate clockwise
      //      curDiff = fabs(q - qr);
      //      if (curDiff > prevDiff && sign(q) == sign(qr)) break;
      //      prevDiff = curDiff;
      ST.motor(1, -speedMaxTurn); ST.motor(2, speedMaxTurn);
      cal();
      check_for_obstacle();
      Usb.Task();
      if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
        if ( PS3.getButtonClick(SELECT)) 
        return true;
      }
      if (qr > (180 - err) || qr < (-180 + err)) {
        if (q > (180 - err) || q < (-180 + err)) break;
      }
      if (manual_controlled) break;
    }
    long a = millis();
    float prevq;
    stopMotors();
    while (q != prevq && (millis() - a) < 1500) {
      prevq = q;  //calc of final position
      cal();
    }
  }
  return false;
}

int sign (double a)
{
  if (a < 0)return -1;
  else if (a > 0) return 1;
  else return 0;
}
boolean Goto(double X2, double Y2)
{ manual_controlled=false;
  stopMotors();
  requiredOrentation = atan2((Y2 - y), (X2 - x)) * 180 / PI;
  double Initial_Orientation=requiredOrentation;
  //Serial.print("requiredOrentation:");
  //Serial.println(requiredOrentation);
  
  if(Turn(requiredOrentation, 0.5)) {return true;}

  if(MoveStraight(x, y, X2, Y2)){return true;}
//  if(Turn(Initial_Orientation, 0.5)) {return true;}
  return false;

}

double DistanceSq (double x1, double y1, double x2, double y2)
{
  return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

