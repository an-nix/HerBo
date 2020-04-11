#ifndef __CFG_MODE_HEADER__
#define __CFG_MODE_HEADER__

#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <utility/wifi_drv.h>




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
		

	public:
		WiBo(const char* ssid, const char* password);
		void connect_ap();
		void update_status();
		bool is_connected();

		
};

#endif
