#ifndef __CFG_MODE_HEADER__
#define __CFG_MODE_HEADER__

#include <WiFiNINA.h>


#define APSSID "HerBo"
#define PASS "HerBoPass"


class ConfigurationWebServer {

	private:
	 const char* _body = R"(
		 <body>%s</body>
	 )"; 

	 public:
	 	String serve();
	
		char* fill_content();

};

class WifiHandler
{
	private:
		WiFiServer* __server;
		int __status;
		int __nwNbr;
		void (*__client_handler) (WiFiServer*);

	public:
		WifiHandler(void (*)(WiFiServer*));
		int start_access_point(const char*, const char*);
		int start_server();
		int list_networks();
		void do_client_things();
};

void printWiFiStatus();
int listNetworks();
void printEncryptionType(int thisType);
void printMacAddress(byte mac[]);

#endif
