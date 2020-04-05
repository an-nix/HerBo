# 1 "c:\\project\\HerBo\\sources\\sources.ino"
# 2 "c:\\project\\HerBo\\sources\\sources.ino" 2
# 3 "c:\\project\\HerBo\\sources\\sources.ino" 2
# 4 "c:\\project\\HerBo\\sources\\sources.ino" 2

# 6 "c:\\project\\HerBo\\sources\\sources.ino" 2

# 8 "c:\\project\\HerBo\\sources\\sources.ino" 2





BH1750 lightMeter;
Light* l1 ;
Light* l2;

int _status;
int steps;
unsigned long next_millis;

void setup()
{

    //Initialize Lights
    l1 = new Light(0,true);
    l2 = new Light(4,false);

   //Initialize serial and wait for port to open:
   SerialUSB.begin(9600);
   while (!SerialUSB);// wait for serial port to connect. Needed for native USB port only

   SerialUSB.println("HerBo Start");

   pinMode((6u), (0x1)); // set the LED pin mode





    const byte seconds = 0;
    const byte minutes = 0;
    const byte hours = 16;


    const byte day = 21;
    const byte month = 3;
    const byte year = 20;


    set_time_RTCZero(get_epoch(year,month,day,hours,minutes,seconds));

    //rtc.attachInterrupt(sunrise);

    digitalWrite(5, (0x1));


  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  // On esp8266 devices you can select SCL and SDA pins using Wire.begin(D4, D3);
  Wire.begin();

  lightMeter.begin();
  SerialUSB.println((reinterpret_cast<const __FlashStringHelper *>(("BH1750 Test"))));

  //Global Variable init 
  next_millis = 0;
  _status = 0;
  steps = 0;

  //Set PWM Resolution on 10bits;

    analogWriteResolution(10);



}


void loop()
{

  switch(_status)
  {
    //Initializing
    //Je sais pas ou je suis, quand je suis
    //Led Blinking
    case 0:
      unsigned long m = millis();
      if(m>next_millis)
        {

          steps++;
          int u = pairiod(steps,250);

          SerialUSB.println(u);
          //analogWrite(LED_1_PIN, 1023 - u);
          //analogWrite(LED_2_PIN, u);
          l1->dim(u);
          l2->dim(u);
          next_millis += 250 * 1;
        }

      break;
  }





  //photoperiod();


}

int pairiod(uint32_t steps,uint32_t length)
{

  double dd = sin(((2 * 3.1415926535897932384626433832795 * steps * 1) / length) + (3*3.1415926535897932384626433832795 / 2 ));
  int ret = floor(1024*dd + 1024);

    ret = map(ret,0,256,0,125);




  return(ret);
}
