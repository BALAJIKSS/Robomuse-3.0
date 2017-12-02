#define redpin 13
#define bluepin 11
void Led_init()
{
     pinMode(bluepin, OUTPUT);
   pinMode(redpin, OUTPUT);
  digitalWrite(redpin, 0);
    digitalWrite(bluepin, 1);
}

void blinkled(int i){
  for (int a=0;a<i;a++)
  {
    red();
    delay(100);
    pink();
    delay(100);
  }
}


void red()
{
    digitalWrite(redpin, 1);
    digitalWrite(bluepin, 0);
}
void blue()
{
    digitalWrite(redpin, 0);
    digitalWrite(bluepin, 1);
}
void pink()
{
  
    digitalWrite(redpin, 1);
    digitalWrite(bluepin, 1);
}
void led_off()
{

  
    digitalWrite(redpin, 0);
    digitalWrite(bluepin, 0);

}


