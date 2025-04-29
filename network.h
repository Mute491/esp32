#define MAX_NETWORKS 10

typedef struct
{
  String ssid;
  uint8_t ch;
  uint8_t bssid[6];
}  _Network;

//struct network beacon packet
void scanForNetworks(_Network * networks);

void start_deauth(_Network wifi);

bool spoofMac(uint8_t mac[]);