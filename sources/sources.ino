#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>

#include "photo_period.h"
#include "timeduino.h"
#include "BH1750.h"
#include "wifi.h"

#include "hardware_config.h"
#include "HtmlGen.h"


#define STATUS_TIME		1000
#define RECONNECT_TIME 	10000



/*
	WL_NO_SHIELD = 255,
        WL_NO_MODULE = WL_NO_SHIELD,
        WL_IDLE_STATUS = 0,
        WL_NO_SSID_AVAIL,
        WL_SCAN_COMPLETED,
        WL_CONNECTED,
        WL_CONNECT_FAILED,
        WL_CONNECTION_LOST,
        WL_DISCONNECTED,
        WL_AP_LISTENING,
        WL_AP_CONNECTED,
        WL_AP_FAILED
*/


  Light *l1,*l2;
  //Light* l2;
  PhotoPeriod* ph;
  
WifiHandler* wh;
WiFiServer* ws;
int display_timer;



void setup() 
{

  LOGGING_ERROR();
 

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //while (!Serial);// wait for serial port to connect. Needed for native USB port only
  delay(10000);
  LOGGING_STARTING();

  Serial.println("HerBo Start");

  pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode

  //Set PWM Resolution on 10bits;
  #ifdef ADC_10_BITS
    analogWriteResolution(10);
  #else 
    analogWriteResolution(8);
  #endif

/**************Light & PhotoPeriod**************/
  l1 = new Light(0,true);
  l2 = new Light(4,false);

  ph = new PhotoPeriod(-98.183334,19.033333,-5);
  ph->add_light(l1);
  ph->add_light(l2);

  init_time();
  init_alarm(sunrise);
  
  set_time(EPOCH_DEFAULT);
  ph->sync(EPOCH_DEFAULT); 


/**************WIFI**************/
  wh = new WifiHandler("annix_home","welcome @t h0me",WifiMode::Station,STATUS_TIME,RECONNECT_TIME);
  wh->attach_connect(connect);
  wh->attach_disconnect(disconnect);
  //wh->start_server();
  

  //LOGGING_WAITING();
  display_timer = -1000;
}


void loop() {

  //delay(10000);
  int current_millis = millis();

  //WifiHandler loop
  int stat = wh->timed_loop(current_millis);
  wh->loop();
  //PhotoPeriod loop
  ph->loop();

  //status loop
  if(current_millis > display_timer + 1000)
  {
    display_timer = current_millis;
    if(stat == 0)
    {
      LOGGING_ERROR();
    }
    else if(stat==WL_CONNECTED || stat == WL_AP_CONNECTED)
    {
      LOGGING_READY();
    }
    else
    {
      LOGGING_WAITING();
    }
  }

}


void test_callback(WiFiServer* s){
  WiFiClient client = s->available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
          client.println("</html>");
           break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


void led_drive(byte red, byte green, byte blue) {

  WiFiDrv::analogWrite(WIFI_LED_RED, red);
  WiFiDrv::analogWrite(WIFI_LED_BLUE, blue);
  WiFiDrv::analogWrite(WIFI_LED_GREEN, green);

}

void connect()
{
  Serial.println("Connect");
  ws= new WiFiServer(80);
  ws->begin();

}

void disconnect()
{
  Serial.println("DisConnect");
  delete ws;
}

void sunrise() {
  Serial.println("Wake up");
  ph->init_period();

}
