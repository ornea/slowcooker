/*
   ToDo
   Update tick marks based on time since last Reset
   Finish the Farenheit Option so it makes sense
   Check to make sure HEATER is off when probe disconnected i.e. when reading is a NaN
   RC522 MODULE    Uno/Nano     MEGA
  IRQ             N/A          N/A  BR
  NSS             D10          D53  WH/BR
  SCK             D13          D52  GR
  MOSI            D11          D51  WH/GR
  MISO            D12          D50  OR
  GND             GND          GND  WH/OR
  RST             D9           D5   BL
  3.3V            3.3V         3.3V WH/BL

  WEMOS
  IRQ             N/A WH/BL (Blue)
  NSS (SDA)       D8  WH/GR (Black)
  SCK             D5  GR  (Yellow)
  MOSI            D7  WH/OR (Red)
  MISO            D6  BLUE (Green)
  GND             GND OR   (Orange)
  RST             D0  WH/BR (Brown)
  3.3V            VCC BR  (Grey)

  H0 Blue  { 53, 5},
  H1 Pink  { 49, 6},
  H2 Silver  { 48, 7 },
  H3 Red    {47, 8}
  Test ok with Mega
  Tested OK with 6m of cat5

  //Normal run
  //Vcc 4.85v
  //D4 3.36V
  //D3 3.36v
  //D2 84mv
  //D1 84mV
  //RX 3.36V
  //TX 3.36V
  //Res 3.36V
  //A0 60mV
  //D0 84mV
  //D5 84mV
  //D6 84mV
  //D7 85mV
  //D8 85mV
  //3.3v 3.36V


  //Pins good for input D0,D1,D2,D5,D6,D7
  //D0 GPIO16 dont care

  //D1 GPIO05 Dont Care

  //D2 GPIO04 Dont Care

  //D3 GPIO00 can not be low during a reset, but must be low during a reflash
  //D3 GPIO00 can be high during a reset but must be low during a reflash
  //D3 GPIO00 can be used with wemos relay shield
  //D3 pull up

  //D4 GPIO02 can not be low during a reset
  //D4 GPIO02 can be high during a reset and reflash
  //D4 GPIO02 cant be used with wemos relay shield. Will Not reset
  //D4 pull up

  //D5 GPIO14 Dont Care
  //D5 GPIO14 SPI Clock

  //D6 GPIO12 Dont Care
  //D6 GPIO12 SPI MISO

  //D7 GPIO13 (Serial.swap() -> RX) Dont Care
  //D7 GPIO13 SPI MOSI

  //D8 GPIO15 (Serial.swap() -> TX) can not be high during a reset or a reflash
  //D8 GPIO15 (Serial.swap() -> TX) can be low during a reset and reflash
  //D8 pull down

  //TX GPIO01

  //RX GPIO03


*/


//#define USE_MAX6675
#define USE_DHT
//#define USE_ADC

#define SerialSpeed 115200
#define INTERVAL_1_DAY 60 * 60 * 24 *1000 //86400000///24  // 1day => 24*60*60*1000
#define INTERVAL_1_HOUR 60 *60 * 1000
#define INTERVAL_1_MIN 60 * 1000
#define INTERVAL_1_SEC 1000

#define CELSIUS "DEG_C"
#define FARENHEIT "DEG_F"
#define INVERTED "Active Low"
#define NONINVERTED "Active High"
#define MODE_AUTO "Free Run"
#define FORCED_HIGH "Forced High"
#define FORCED_LOW "Forced Low"

#define HEATER_OFF "heater_off"
#define HEATER_ON "heater_on"

#define HEATER D0

#define HEATER_HIGH digitalWrite(HEATER, HIGH)
#define HEATER_LOW digitalWrite(HEATER, LOW)
#define ISHEATERHIGH digitalRead(HEATER)
#define degf(C)  ((C*9/5)+32)//(0°C × 9/5) + 32 = 32°F

#define BUTTON_PREAMPLE "*"

#define STRING_PREAMBLE "@"

#define PID_DATA_PREAMBLE "&"

#define TRUE true
#define FALSE false

const char __SKETCH__[] = __FILE__; //Used to get the current path


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>//Used to send heartbeats
#include <WiFiUdp.h>
#include <ArduinoOTA.h> //Update Flash Over The Air
#include <EEPROM.h>     //Saves various settings 

const byte io_pins [9] =     {D0, D1, D2, D3, D4, D5, D6, D7, D8};
//const byte io_pins[9] =    {16,  5,  4,  0,  2, 14, 12, 13, 15};

int ticks = 0;  //Keeps track of how many times the PID loop has been executed.

//The following info is included in the Build Info page
const String strSummary =  "<br><br>Summary<br>PID Slow Cooker <br>controller<br>By Justin Richards";

//byte data[MAX_LEN];
unsigned long next_heartbeat_Millis = 0;  //Time to send a http request to webserver for remote logging if needed
unsigned long previous_PID_Millis = 0;    //Keeps track of timming for PID loop execution
unsigned long previous_ON_Millis = 0;     //PWM implemented in software
unsigned long previous_OFF_Millis = 0;
unsigned long previousyieldMillis = 0;    //If probe is disconnected, ensures that utility functions Wifi etc are run periodically in the background
unsigned long smMillis = 0;

//PID variables
float error = 0;
float pTerm = 0;
float iTerm = 0;
float dTerm = 0;

float sensorValue = 0;
float currentHeat = 0;
float currentHumidity = 0;

int CV;  //Control Variable
enum sm //state machine enumerated states
{
  standby, stage1vectoring, stage1sustain, stage2vectoring, stage2sustain, cyclecomplete
};
int sm = 0;

enum PID //used as index into PID array of floats
{
  sp1, sp2, sp1td, sp2td, kp, ki, kd, mvmax, mvmin, cal, tc
};

enum modes
{
  free_run, forced_high, forced_low, invalid
};

//int heating_stage = stage1;
typedef struct APP_CONFIG //AppConfig
{
  char E_sign[16] = "AABbCcDdEe68"; //This is a signature that is used to determine
  //if EEPROM is to be updated with the hardcoded
  //values or continue using previously saved values
  //must be at the start of the struct
#ifdef real_credentials
  #error
#endif

#if __has_include("creds.h") 
//# include "myinclude.h"
 #error
#endif

#include "creds.h" //contains SSID/KEY pairs
//#ifdef credentials
//  #error
//#endif
  //Default Configs as EP End Point

  char EpHostname[15] = "slowcook"; //End Point Hostname
  char EpControlHostname[15] = "slowcooker"; //If we need to send UDP messages to another controller. Not used here

  //the requirement to statically seems to have been mitigated by WiFi.persistent(false); in _init.h
  uint8_t EpIP[4] = {172, 16, 10, 220}; //I have issues when a Cisco Router issues the IP address.
  uint8_t EpMASK[4] = {255, 255, 0, 0}; //So when in that environment I statically assign
  uint8_t EpGW[4] = {172, 16, 0, 1};
  uint8_t EpDNS[4] = {172, 16, 0, 1};

  //Default Configs as AP Access Point so it can be configured for a different network
  char ApSSID[32] = "slowcook"; //Max 32 Chars
  char ApKey[64];
  uint8_t ApIP[4] = {192, 168, 12, 1};

  uint8_t ApMASK[4] = {255, 255, 255, 0};
  uint8_t ApGW[4] = {192, 168, 12, 1};

  //               sp1 , sp2 , sp1td, sp2td,  kp , ki  , kd , mvmax , mvmin, cal,  tc (timeconstant)
  float PID[11] = {72.5, 57.2, 4    , 360    ,  0.5 , 0.4 , 0.5 , 140.0 ,   0  ,  0 , 5000}; //
  uint8_t Mode = 0;
  struct FLAGS
  {
    unsigned char b7: 1;
    unsigned char bIsCelsius: 1;
    unsigned char bIsOutputInverted : 1; //Reverses the logic
    unsigned char xbIsModeAuto : 1; //'1' Normal mode
    unsigned char xbIsOutForcedHigh : 1; //'1' forces Output HIGH
    unsigned char bUseStaticIP : 1; //When Cisco Router is doing DHCP
    unsigned char bIsDHCPEnabled : 1; //Not used
    unsigned char bInConfigMode : 1; //Not used
    //  } Flags = { 0, 1, 0, 1, 0, 1, 0, 0};//Leftmost is b7 use static
  } Flags = { 0, 1, 0, 1, 0, 0, 0, 0};//Leftmost is b7 use dhcp
  // */// Flag structure
} ;

APP_CONFIG AppConfig;

ESP8266WebServer server(80); //instantiate server object of type class ESP8266WebServer
//WiFiClient client;
HTTPClient http;  //instantiate http object of type class HTTPClient
#include "helpers.h"
#include "mdns_query.h"
#include "hbeat.h"
#include "pid.h"
#include "webSocketEvent.h"
