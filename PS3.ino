void PS3_init()
{
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
Serial.print(F("\r\nPS3 Bluetooth Library Started"));
}

boolean PS3_update()
{
  long lhy, l, r;
  boolean sqr;
  Usb.Task();
  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    lhy = PS3.getAnalogHat(LeftHatY);
    r = PS3.getAnalogButton(R2);
    l = PS3.getAnalogButton(L2);
    sqr = PS3.getButtonClick(SQUARE);
    if (sqr) {
#if enable_debug
      Serial.print("sqr");
#endif
      return true;
    }
    lhy = lhy - 127;
    left = (long)-(lhy * speedMaxDrive*2 + (r-l) * speedMaxTurn) / 255;
    right = (long)-(lhy * speedMaxDrive*2 + (l-r) * speedMaxTurn) / 255;
    if (abs(right) < 10)right = 0;
    if (abs(left) < 10)left = 0;
    if (abs(left) >= speedMaxDrive) left = sign(left) * speedMaxDrive;
    if (abs(right) >= speedMaxDrive) right = sign(right) * speedMaxDrive;
#if enable_debug
    Serial.print(left);
    Serial.print(' ');
    Serial.print(right);
    Serial.println();
#endif
  }
  return false;
}

