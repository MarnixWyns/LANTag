#include <EtherCard.h>
#include <string.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[380];

//OLED I2C Adres: 0x3C

void startup();
void printIPNM();
void printGWDNS();
void connectivityError();

void setup () {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 

  Serial.println(F("Starting LANTAG..."));

  startup();

 
    if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) {
      Serial.println(F("Failed to access Ethernet controller"));
      connectivityError();
      }
   
    Serial.println(F("Passed ethernet initialization"));

    Serial.println(F("Setting up DHCP"));
 
    if (!ether.dhcpSetup()){
      Serial.println(F("DHCP failed"));
      connectivityError();
     }
    
}

void loop () {
    
    printIPNM();
    Serial.println(F("Print IP NM"));
    delay(2000);

    printGWDNS();
    Serial.println(F("Print GW DNS"));
    delay(2000);
}


void startup(){
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(12,8);
    display.clearDisplay();
    display.println(F("LANTAG"));
    display.display();
    delay(2000);
};


void printIPNM(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.clearDisplay();
    
    char devIP[] = "IP: ";

    char parseIP[4];
    ether.makeNetStr(parseIP, ether.myip, 4, '.', 10);
    
    char* endIP; 
    endIP = (char*) malloc(20);
    strcpy(endIP, devIP);
    strcat(endIP, parseIP);
    
    char* devNM = "NM: ";
    char parseNM[4];
    ether.makeNetStr(parseNM, ether.netmask, 4, '.', 10);

    char * endNM; 
    endNM = (char*) malloc(15);
    strcpy(endNM, devNM);
    strcat(endNM, parseNM);

    display.setCursor(0,4);
    display.println(endIP);
    display.setCursor(0,22);
    display.println(endNM);

    display.display();

    free(endNM);
    free(endIP);
}

void printGWDNS(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.clearDisplay();
    
    char* devGW = "GW: ";
    char parseGW[4];
    ether.makeNetStr(parseGW, ether.gwip, 4, '.', 10);
    
    char * endGW; 
    //endGW = malloc(1 + strlen(devGW) + strlen(devGW) + 3);
    endGW = (char*) malloc(20);
    strcpy(endGW, devGW);
    strcat(endGW, parseGW);
    
    char* devDNS = "DNS: ";
    char parseDNS[4];
    ether.makeNetStr(parseDNS, ether.dnsip, 4, '.', 10);

    char* endDNS;
    endDNS = (char*) malloc(15);
    strcpy(endDNS, devDNS);
    strcat(endDNS, parseDNS);


    display.setCursor(0,4);
    display.println(endGW);
    display.setCursor(0,22);
    display.println(endDNS);
    display.display();

    free(endDNS);
    free(endGW);
}

void connectivityError(){
    for (int i = 0; i < 2000; i++){
    display.setTextSize(2);
    display.setTextColor(WHITE);

    display.setCursor(12,0);
    display.clearDisplay();
    display.println(F("Cable\n Error"));
    display.display();
    delay(2000);

    
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(12,0);
    display.clearDisplay();
    display.println(F("Check\n Cable"));
    display.display();
    delay(2000);
    }
}