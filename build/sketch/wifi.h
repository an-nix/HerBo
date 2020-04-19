#ifndef __CFG_MODE_HEADER__
#define __CFG_MODE_HEADER__

#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include "timeduino.h"
#include <utility/wifi_drv.h>


#define APSSID "HerBo"
#define PASS "HerBoPass"

#define WIFI_LED_GREEN 25
#define WIFI_LED_RED 26
#define WIFI_LED_BLUE 27

#define NTP_PACKET_SIZE 48 


#define CONNECT_TRY		5


int wifi_check();
void wifi_connect(const char* SSID, const char* pass , int &status);
unsigned long sendNTPpacket(IPAddress& address, WiFiUDP* u);




class WiBo {

	private:
		bool _ready;
		String _ssid;
		String _password;
		int _status;
		WiFiServer* _server;
		

	public:
		WiBo(const char* ssid, const char* password,int port);
		void connect_ap();
		void update_status();
		bool is_connected();
		bool start_server(int port);

		
};




class ConfigurationWebServer {

	private:
	 const char* _body = R"(
		 <body>%s</body>
	 )"; 

	 public:
	 	String serve();
	
		char* fill_content();

};

enum WifiMode{
	Station = 0,
	AP = 1
};

class ServerHandler {

	private:
		WiFiServer* __server;
		void (*__client_handler) (WiFiServer*);

	public:
		ServerHandler(int port,void (*)(WiFiServer*));
		void loop();
		
};

class WifiHandler
{
	private:

		bool __ready;
		int __status;

		WifiMode __mode;
		String __ssid;
		String __pass;
		
		//periodic tasks 
		uint32_t __connect_timer;
		uint32_t __connect_rearm_timer;
		uint32_t __status_timer;
		uint32_t __status_rearm_timer;

		bool __previous_state;

		//event
		void (*__on_connect) ();
		void (*__on_disconnect) ();

		bool is_connected();

	public:

		WifiHandler(const char* ssid, const char* pass, WifiMode mode,uint32_t status_rearm, uint32_t timer_rearm);							
		int connect();
		void loop();	
		int timed_loop(uint32_t current_time);
		
		void attach_connect(void (*)());
		void attach_disconnect(void (*)());
		void attach_wifiserver(WiFiServer*);
};

void printWiFiStatus();
int listNetworks();
void printEncryptionType(int thisType);
void printMacAddress(byte mac[]);


#endif
