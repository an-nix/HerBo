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

WiBo::WiBo(const char* ssid, const char* password,int port){



  
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
    this->_server = new WiFiServer(80);

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



int keyIndex = 0;                // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

String ConfigurationWebServer::serve() {

	unsigned int i = strlen(_body);
	Serial.print(_body);
	char* content = fill_content();
	i += strlen(content);
	Serial.print(content);
	char* ret = new char[i];
	sprintf(ret,_body,content);
	Serial.print(ret);
	return ret;

}

char* ConfigurationWebServer::fill_content() {

	String rets = "Bonjour";
	int i = rets.length() + 1;
	char* ret = new char[i];
	rets.toCharArray(ret,i);
	return ret;

}

WifiHandler::WifiHandler(const char* ssid, const char* pass,WifiMode mode, uint32_t status_rearm, uint32_t connect_rearm) {
  
  // check for the WiFi module
	if (WiFi.status() == WL_NO_MODULE) {
		Serial.println("Communication with WiFi module failed!");
		this->__ready = false;
    }

  //check firmware version
	else if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION) {
		Serial.println("Please upgrade the firmware");
    this->__ready = false;
	}

  //Wireless Module is ready
  else {
    this->__ready = true;
	  this->__status = WL_IDLE_STATUS;
    this->__mode = mode;
    this->__ssid = ssid;
    this->__pass = pass;
    this->__status_timer = 0;
    this->__connect_timer = 0;
    this->__status_rearm_timer = status_rearm;
    this->__connect_rearm_timer = connect_rearm;
    this->__on_connect = NULL;
    this->__on_disconnect = NULL;
    this->__previous_state = false;
  }


}


int WifiHandler::connect() {

  if(this->__ready)
  {
    if(this->__mode == WifiMode::AP)
    {
      this->__status = WiFi.beginAP(this->__ssid.c_str(),this->__pass.c_str());
    }
    
    else if (this->__mode == WifiMode::Station)
    {
      this->__status = WiFi.begin(this->__ssid.c_str(),this->__pass.c_str());
    }
  }

  return(this->__status);
}

bool WifiHandler::is_connected() {
  if(this->__mode == WifiMode::AP)
    return(this->__status == WL_AP_LISTENING || this->__status == WL_AP_CONNECTED);
  else
    return(this->__status == WL_CONNECTED);
}

int WifiHandler::timed_loop(uint32_t current_time) {

 if(this->__ready)
 {
  if(current_time > this->__status_timer)
    {
      //w->update_status();
      this->__status = WiFi.status();
      this->__status_timer = current_time + this->__status_rearm_timer;
      
      bool tcon = this->is_connected();
      if(this->__previous_state != tcon)
        {
          if(tcon)
          {
            this->__on_connect();
          }
          else
          {
            this->__on_disconnect();
          }
          
          this->__previous_state = tcon;          
        }

    }

    if(current_time > this->__connect_timer && !(this->is_connected()))
    {
      this->connect();
      this->__connect_timer = current_time + this->__connect_rearm_timer;
    }
 
    return(this->__status);
  }
  else
  {
    return(-1);
  }
  
}

void WifiHandler::loop() {
	//__client_handler(__server);
}

void WifiHandler::attach_connect(void (*callback)()) {
  this->__on_connect = callback;
}

void WifiHandler::attach_disconnect(void (*callback)()) {
  this->__on_disconnect = callback;
}