# 1 "c:\\project\\HerBo\\sources\\sources.ino"
# 2 "c:\\project\\HerBo\\sources\\sources.ino" 2
# 3 "c:\\project\\HerBo\\sources\\sources.ino" 2
# 4 "c:\\project\\HerBo\\sources\\sources.ino" 2

# 6 "c:\\project\\HerBo\\sources\\sources.ino" 2

# 8 "c:\\project\\HerBo\\sources\\sources.ino" 2




  Light* l1;
  Light* l2;
  PhotoPeriod* ph;

  const byte seconds = 0;
  const byte minutes = 50;
  const byte hours = 6;

  const byte day = 26;
  const byte month = 5;
  const int year = 2020;

void setup()
{

  //Initialize serial and wait for port to open:
  SerialUSB.begin(9600);
  while (!SerialUSB);// wait for serial port to connect. Needed for native USB port only

  SerialUSB.println("HerBo Start");

  pinMode((6u), (0x1)); // set the LED pin mode

  //Set PWM Resolution on 10bits;

    analogWriteResolution(10);





 //set_time(get_epoch(year,month,day,hours,minutes,seconds));

 //Initialize Lights
  l1 = new Light(0,true);
  l2 = new Light(4,false);

  ph = new PhotoPeriod(-98.183334,19.033333,-5);
  ph->add_light(l1);
  ph->add_light(l2);

  //Serial.println("Get Next Sunrise");
  //uint32_t ep = get_epoch(year,month,day,0,0,0) + (ph->get_next_sunrise(true,year,month,day,-7,1)*3600);
  //Serial.println(ep);

  init_time_RTCZero();
  set_time_RTCZero(1590472800);
  ph->sync(1590472800);
  //Try


}


void loop() {
  //photoperiod();

}

void sync_sun(time_t epoch)
{


  ph->get_next_sunrise(true,epoch,false);
  ph->get_next_sunrise(true,epoch,true);
  //sync time
  //get next Sunrise
  //is next sunrise passed?
  //is next sunset passed
  //get next sunrise and wait


}
