#include "wifi.h"

int wifi_check()
{
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) 
    return(-1);

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) 
    return(-2);
  
  return(1);
}

void wifi_connect(const char* SSID, const char* pass , int &status) {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(SSID, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
}
// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address, WiFiUDP* u) {
  //Serial.println("1");
  
  byte packetBuffer[ NTP_PACKET_SIZE];
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  //Serial.println("2");
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  //Serial.println("3");

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  u->beginPacket(address, 123); //NTP requests are to port 123
  //Serial.println("4");
  u->write(packetBuffer, NTP_PACKET_SIZE);
  //Serial.println("5");
  u->endPacket();
  
  //Serial.println("6");
}

WiBo::WiBo(const char* ssid, const char* password){


    WiFiDrv::pinMode(WIFI_LED_GREEN, OUTPUT);  //GREEN
    WiFiDrv::pinMode(WIFI_LED_RED, OUTPUT);  //RED
    WiFiDrv::pinMode(WIFI_LED_BLUE, OUTPUT);  //BLUE
  
  // check for the WiFi module:
  	if (WiFi.status() == WL_NO_MODULE) 
    {
      Serial.println("No Wifi Module");
      this->_ready = false;
    }
    	

  	else if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION) 
    {
      Serial.println("Wrong firmwareVersion");
      this->_ready = false;
    }

	else 
  	{
		this->_ready = true;
		this->_password = password;
		this->_ssid = ssid;

	}

}

void WiBo::connect_ap()
{
	//is the wifi module Ready (control on class instanciation)
	if(this->_ready)
	{	
				this->_status = WiFi.begin(this->_ssid.c_str(),this->_password.c_str());
	}

}

void WiBo::update_status(){



  if(this->_ready == false)
  {
    WiFiDrv::digitalWrite(WIFI_LED_RED, HIGH);
    WiFiDrv::digitalWrite(WIFI_LED_BLUE, LOW);
    WiFiDrv::digitalWrite(WIFI_LED_GREEN, LOW);
  }
  else
  {
    
    this->_status = WiFi.status();

    switch(this->_status){
      case WL_CONNECTED:
            WiFiDrv::digitalWrite(WIFI_LED_RED, LOW);
            WiFiDrv::digitalWrite(WIFI_LED_BLUE, LOW);
            WiFiDrv::digitalWrite(WIFI_LED_GREEN, HIGH);
      break;
      default:
            WiFiDrv::digitalWrite(WIFI_LED_RED, LOW);
            WiFiDrv::digitalWrite(WIFI_LED_BLUE, HIGH);
            WiFiDrv::digitalWrite(WIFI_LED_GREEN, LOW);
      break;

    }
  }
  
}

bool WiBo::is_connected()
{
  return(this->_status == WL_CONNECTED);
}