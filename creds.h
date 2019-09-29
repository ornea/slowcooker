#define credentials
#define indx_SSID 0
#define indx_KEY 1

#define URL_Heartbeat "http://192.168.0.200/cgi-bin/heartbeat.pl?"
#define numAccessPoints 5
char access_point[numAccessPoints][2][20] = //this is not inline with the rules for size of ssid and key
{
  {"ssid0", "key0"},
  {"ssid1", "key1"},
  {"ssid2", "key2"},
  {"ssid3", "key3"},
  {"ssid4", "key4"},
};
