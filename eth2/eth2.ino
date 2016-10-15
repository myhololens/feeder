#include <Ethernet.h>
boolean reading = false;
 
// Setup the static network info for you arduino
byte ip[] = { 192, 168, 1, 20 };   // IP Address
byte subnet[] = { 255, 255, 255, 0 }; // Subnet Mask
byte gateway[] = { 192, 168, 1, 1 }; // Gateway
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC Address
 
EthernetServer server = EthernetServer(80); // Port 80
String HTTPget = "";
 
void setup()
{
  // pin 10, 11, 12 and 13 are used by the ethernet shield
  Ethernet.begin(mac, ip, gateway, subnet); // setup ethernet with params from above
  server.begin();
}
 
void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) 
  {
    // send http reponse header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    // process request.
    processClient(client);
  }
}
 
void processClient(EthernetClient client)
{
  // http request will end with a blank line
  boolean lineIsBlank = true;
 
  while (client.connected())
  {
    if (client.available())
    {
      char c = client.read();
 
      if(reading && c == ' ') reading = false;
      if(c == '?') reading = true; // ? in GET request was found, start reading the info
 
      //check that we are reading, and ignore the '?' in the URL, then append the get parameter into a single string
      if(reading && c != '?') HTTPget += c;
 
      if (c == '\n' && lineIsBlank)  break;
 
      if (c == '\n') lineIsBlank = true;
      else if (c != '\r') lineIsBlank = false;
 
    }
  }
  client.print(HTTPget);
  delay(1); // give the web browser a moment to recieve
  client.stop(); // close connection
  HTTPget = ""; // clear out the get param we saved
}
