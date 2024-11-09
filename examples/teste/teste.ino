
#include <WiFi.h>
#include <EEPROM.h>
#include <ESPAsyncWebServer.h>
//#include <WebServer.h>
//#include <WiFiClient.h>  
//#include <WiFiAP.h>
#include <PCI_UG_T1.h>

#include "ini_rede.h"

AsyncWebServer server(80);
//WebServer server(80);

PCI_UG_T1 x(2, 0);      //indica motor de passo no modo 2 em CN1 

const char* xssid = "PCI_UG_T1";
const char* xpass = "00000000";

String   ssid, pass;
bool     fim=false;
int      n,k;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(128);
  x.begin();
  x.bdur=200;x.bfreq=2000;x.binter=0;x.bnum=1;
  x.ldur=50;x.linter=25;x.lnum=10;

  dumpflash();  //mostra o estado antes dos 128 bytes da memória flash  

  //***********************************************************************************************
  initRede(); //inicializa ssid e password. Se fot a primeira vez, escolhe a rede/senha a ser usada 
  //***********************************************************************************************

  Serial.println("");
  Serial.print("Conectando na rede: ");Serial.println(ssid);
  Serial.print("Com a senha.......: ");Serial.println(pass);
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("Rede WiFi conectada");
  Serial.println("");

  IPAddress X=WiFi.localIP();
  X[3]=99;IPAddress local_IP=X;
  X[3]=1; IPAddress gateway=X;
  IPAddress subnet(255, 255, 0, 0);
  IPAddress primaryDNS(1, 1, 1, 1);
  IPAddress secondaryDNS(1, 0, 0, 1);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Falha na configuração da estação");
  }
 
  IPAddress IP = WiFi.localIP();
  Serial.println("-----------------------------------------");
  Serial.print("Rede: ");Serial.print(ssid);Serial.println(" conectada");
  Serial.print("Acesso pelo IP fixo: ");Serial.println(IP);
  Serial.println("-----------------------------------------");
  server.begin();
}


void loop()
{
}  


void initRede()
{
//  server.handleClient();

  for (k=0; k<128; k++){EEPROM.write(k, 0x00);}EEPROM.commit();
  WiFi.softAP(xssid, xpass);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("");    
  Serial.println("-----------------------------------------");
  Serial.println("Rede: PCI_UG_T1");
  Serial.println("Senha: 00000000");
  Serial.print("Acesso pelo IP: ");Serial.println(IP);
  Serial.print("Vcc no momento (5vcc): ");Serial.print(float(analogRead(32))/3103.0*5.0,1);Serial.println(" volts");
  Serial.println("-----------------------------------------");
  Serial.println("");
  server.begin();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", ini_rede); 
            Serial.println("Enviou pagina!");});
  
//  server.on("/", raiz);
//  server.on("/leredes", leredes);
//  server.on("/grava", gravarede);
  while (!fim){}
}


//void raiz(){
//  x.ldur=50;x.linter=25;x.lnum=10;
//  String enviapg = ini_rede;
//  server.send(200, "text/html", enviapg);
//}


//void leredes()
//{
//  x.ldur=50;x.linter=25;x.lnum=10;
//  n = WiFi.scanNetworks();
//  if (n>12){n=12;}
//  String parmValues="";
//  for (int k=0;  k<n; k++){parmValues += "" + WiFi.SSID(k) + "" + "&";}
//  server.send(200, "text/plane", parmValues);
//}

                  
//void gravarede()
//{
//  x.ldur=50;x.linter=25;x.lnum=10;
//  ssid = server.arg(String(0));
//  pass = server.arg(String(1));
//    
//  EEPROM.write(124, ssid.length());
//  EEPROM.write(125, pass.length());
//            
//  for (k=0; k<=ssid.length()+1; k++){EEPROM.write(k, ssid.charAt(k));}
//  for (k=0; k<=pass.length()+1; k++){EEPROM.write(k+32, pass.charAt(k));}
//  
//  EEPROM.write(126, 0x55);EEPROM.write(127, 0xAA);
//  EEPROM.commit();
//
//  ssid="";pass="";
//  for (k=0; k<EEPROM.read(124); k++){ssid=ssid+char(EEPROM.read(k));}
//  for (k=0; k<EEPROM.read(125); k++){pass=pass+char(EEPROM.read(k+32));}
//
//  fim=true;
//}  


void dumpflash()
{
  uint8_t xbyte;
  uint16_t addr, k; char hexVal[5];
  Serial.println("");
  for (addr=0; addr<127; addr=addr+16){
    sprintf(hexVal, "%04d", addr);
    Serial.print(hexVal);Serial.print("  ");  
    sprintf(hexVal, "%04X", addr);
    Serial.print(hexVal);Serial.print("  ");  
    for (k=0; k<16; k=k+1){
      xbyte=EEPROM.read(addr+k);
      sprintf(hexVal, "%02X", (xbyte));Serial.print(hexVal);Serial.print(" ");
    }
    Serial.print(" ");
    for (k=0; k<16; k=k+1){
      xbyte=EEPROM.read(addr+k);         
      Serial.write(xbyte>31 && xbyte != 127 ? char (xbyte) : '.');
    }
    Serial.println("");
  }
  Serial.println("");
}  


/////////////////////////////////////////// fim //////////////////////////////////////////////////
