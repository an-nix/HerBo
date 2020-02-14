#include <SPI.h>
#include <WiFiNINA.h>

#include "wifi.h"

#define CONFIG_PIN 14

WiFiServer server(80);
bool config = 1;
int ssNbr;


void setup() 
{

  	//Initialize serial and wait for port to open:
  	Serial.begin(9600);
  	while (!Serial);// wait for serial port to connect. Needed for native USB port only

  	Serial.println("HerBo Start");

  	pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode
	pinMode(CONFIG_PIN,INPUT_PULLUP);  

	config = !digitalRead(CONFIG_PIN);

	if(config) 
	{
		ssNbr = WiFi.scanNetworks();
		start_ap(server);
	}
}


void loop() {
	if(config) 
	{config_handler();}
	  // scan for existing networks:
	else
	{
		  Serial.println("Scanning available networks...");
  listNetworks();
  delay(10000);
	}
	

 
}

void config_handler()
{
 WiFiClient client = server.available();   // listen for incoming clients

  

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        
		char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
		

			client.print("<select form=\"fm\" id=\"cars\">");
  			for (int thisNet = 0; thisNet < ssNbr; thisNet++) 
			{
				String w = WiFi.SSID(thisNet);
				client.print("<option value=\"" + w + "\">" + w + "</option>");
  			}		
			client.print("</select>");
			client.print("<form id=\"fm\"");
			client.print("<label for=\"pass\">First name:</label>");
			client.print("<input type=\"text\" id=\"fname\" name=\"fname\"><br><br>");
  			client.print("<input type=\"submit\" value=\"Submit\">");
			client.print("</form>");
            client.println();

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

}
