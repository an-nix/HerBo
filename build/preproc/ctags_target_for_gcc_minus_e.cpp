# 1 "c:\\project\\HerBo\\sources\\sources.ino"
# 2 "c:\\project\\HerBo\\sources\\sources.ino" 2
# 3 "c:\\project\\HerBo\\sources\\sources.ino" 2
# 4 "c:\\project\\HerBo\\sources\\sources.ino" 2

# 6 "c:\\project\\HerBo\\sources\\sources.ino" 2

# 8 "c:\\project\\HerBo\\sources\\sources.ino" 2
# 9 "c:\\project\\HerBo\\sources\\sources.ino" 2






  Light *l1,*l2;
  //Light* l2;
  PhotoPeriod* ph;
  WiBo* w;


  int wifi_last_status_mil;
  int wifi_last_reconnect_mil;


void setup()
{

  //Initialize serial and wait for port to open:
  SerialUSB.begin(9600);
  while (!SerialUSB);// wait for serial port to connect. Needed for native USB port only

  SerialUSB.println("HerBo Start");

  pinMode((6u), (0x1)); // set the LED pin mode

  //Set PWM Resolution on 10bits;

    analogWriteResolution(10);




 //Initialize Lights
  l1 = new Light(0,true);
  l2 = new Light(4,false);

  ph = new PhotoPeriod(-98.183334,19.033333,-5);
  ph->add_light(l1);
  ph->add_light(l2);

  init_time_RTCZero();
  set_time_RTCZero(1590472800);
  ph->sync(1590472800);

  w = new WiBo("annix_home","welcome @t h0me");

  int wifi_last_status_mil = 0;
  int wifi_last_reconnect_mil = 0;

  w->connect_ap();
  w->update_status();



}


void loop() {


  int current_millis = millis();

  if((current_millis > wifi_last_reconnect_mil + 10000) &&
        !(w->is_connected()))
  {
    w->connect_ap();
    wifi_last_reconnect_mil = current_millis;
  }

  if(current_millis > wifi_last_status_mil + 1000)
  {
    w->update_status();
    SerialUSB.println(current_millis - (wifi_last_status_mil + 1000));
    wifi_last_status_mil = current_millis;
  }




  //update_led_status(w->_status);
  //photoperiod();
}
