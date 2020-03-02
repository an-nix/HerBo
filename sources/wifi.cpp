#include "wifi.h"


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

WifiHandler::WifiHandler(void (*callback)(WiFiServer*)) {
	__server = new WiFiServer(80);
	 __status = WL_IDLE_STATUS;
	 __client_handler = callback;
}

// by default the local IP address of will be 192.168.4.1
// you can override it with the following:
// WiFi.config(IPAddress(10, 0, 0, 1));
// Return 0 => OK
// Return -1 => Generic Error
// Return -2 => Module not present

int WifiHandler::start_access_point(const char* ssid,const char* pass) {
	
  // check for the WiFi module:
	if (WiFi.status() == WL_NO_MODULE) {
		Serial.println("Communication with WiFi module failed!");
		return(-2);
    }

	String fv = WiFi.firmwareVersion();
	if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
		Serial.println("Please upgrade the firmware");
	}

  // print the network name (SSID);
	Serial.print("Creating access point named: ");
	Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
	if (WiFi.beginAP(ssid, pass) != WL_AP_LISTENING) {
		Serial.println("Creating access point failed");
		return(-1);
	}
	return(0);
}

int WifiHandler::start_server() {
	// wait 10 seconds for connection:
	delay(10000);

	// start the web server on port 80
	__server->begin();
}

void WifiHandler::do_client_things() {
	__client_handler(__server);
}

int WifiHandler::list_networks() {

  int numSsid = WiFi.scanNetworks();
  return(numSsid);
}

