#ifndef __CFG_MODE_HEADER__
#define __CFG_MODE_HEADER__

#include <WiFiNINA.h>


#define APSSID "HerBo"
#define PASS "HerBoPass"



void start_ap(WiFiServer&);
void printWiFiStatus();
int listNetworks();
void printEncryptionType(int thisType);
void printMacAddress(byte mac[]);

#endif
