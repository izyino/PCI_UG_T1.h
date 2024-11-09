
//
// Exemplo n.2 de utilizaÃ§Ã£o da biblioteca library PCI_UG_T1.h
// via Wi-Fi, com o servidor web do ESP-32 definido como access-point
// ------------------------------------------------------------------
// PressupÃµe um motor de passo 28byj-48 (motor n.0) conectado a CN1
// Rede Wi-Fi criada com o nome PCI_UG_T1 senha 00000000, acessÃ­vel
// pelo IP 192.168.4.1 ou outro, informado pelo monitor serial
// ------------------------------------------------------------------
//

#include <WiFi.h>
#include <PCI_UG_T1.h>

PCI_UG_T1 x(2, 0);   //indica motor de passo no modo 2 em CN1 

const char* ssid     = "PCI_UG_T1";
const char* password = "00000000";

int i, k, n;
char c;

WiFiServer server(80);

String header;

String p1="<html><head><meta charset='utf-8'><title></title></head><body><table border='0' width='600' id='table1' cellspacing='4' cellpadding='8' height='400'><tr><td colspan='4' align='center' bgcolor='#CCFF99'><p align='center'><b><font size='4'>REDES WiFi<br>AO ALCANCE DO ESP32 DA PCI_UG_T1</font></b></p></td></tr><tr><td colspan='4' align='center'></td></tr>";
String p2="<tr><td align='center' bgcolor='#FFCCFF' width='100'>#AAA</td><td align='center' bgcolor='#FFCCFF' width='300'><p align='left'>#BBB</td><td align='center' bgcolor='#FFCCFF' width='100'>#CCC</td><td align='center' bgcolor='#FFCCFF' width='100'>#DDD</td></tr>";
String p3="<tr><td colspan='4' align='center'></td></tr><form action='/ok'><tr><td colspan='4' align='center' bgcolor='#CCFF99'><b><font size='2'>REDE ESCOLHIDA</font></b></td></tr><tr><td align='center' bgcolor='#FFFF99'><b><font size='2'>nº</font></b></td><td colspan='2' align='center' bgcolor='#FFFF99'><font size='2'><b>senha</b></font></td><td align='center' rowspan='2' bgcolor='#FFCC66'><font size='2'><input type='submit' value='Submit'></font></td></tr><tr><td align='center' bgcolor='#CCFFFF'><input type='text' id='rsenha' name='rsenha' size='7'></td><td colspan='2' align='center' bgcolor='#CCFFFF'><input type='text' id='rsenha' name='rsenha' size='40'></td></tr></form></table></body></html>";
String savP2=p2;

String copyhtml;

uint32_t nsteps;   //steps a percorrer
int      graus=30; //graus a girar (999=contÃ­nuo, 0=para)
int      vel=5;    //velocidade de deslocamento sendo mÃ¡xima 3 sentido 'D'; mÃ¡xima 5 sentido 'E'
char     sent='D'; //sentido esquerda ou direita (E ou D)

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(32, INPUT);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("-----------------------------------------");
  Serial.println("Rede: PCI_UG_T1");
  Serial.println("Senha: 00000000");
  Serial.print("Acesso pelo IP: ");Serial.println(IP);
  Serial.print("Vcc no momento (5vcc): ");Serial.print(float(analogRead(32))/3103.0*5.0,1);Serial.println(" volts");
  Serial.println("-----------------------------------------");
  server.begin();
  x.begin();
}

void loop()
{
  WiFiClient client = server.available();
  if (client) {
    if (((graus==555)||(graus==999))&&(x.xsteps[0]>0)){x.xsteps[0]=2;} //se chegou comando via browser, aborta movimento
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
          
            n = WiFi.scanNetworks();
            client.print(p1);
                        
            if (n > 0) {
              for (i = 0; i < n; i++) {
                p2=savP2;
                String x=String(i+1);
                if ((i+1)<10){x="0"+x;}
                p2.replace("#AAA", x);
                p2.replace("#BBB", WiFi.SSID(i));
                p2.replace("#CCC", String(WiFi.RSSI(i)));
                if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN){p2.replace("#DDD", "aberta");}else{p2.replace("#DDD", "*");}

                client.print(p2);
              }
              delay(10);
            }
            client.print(p3);
          }

          // The HTTP response ends with another blank line
          client.println();
          break;
        } else { // if you got a newline, then clear currentLine
         currentLine = "";
        }} else if (c != '\r') {  // if you got anything else but a carriage return character,
      currentLine += c;      // add it to the end of the currentLine
      }
    }
  }
  // Clear the header variable
  header = "";
  // Close the connection
  client.stop();
}
