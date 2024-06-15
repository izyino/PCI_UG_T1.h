
//
// Exemplo n.2 de utilização da biblioteca library PCI_UG_T1.h
// via Wi-Fi, com o servidor web do ESP-32 definido como access-point
// ------------------------------------------------------------------
// Pressupõe um motor de passo 28byj-48 (motor n.0) conectado a CN1
// Rede Wi-Fi criada com o nome PCI_UG_T1 senha 00000000, acessível
// pelo IP 192.168.4.1 ou outro, informado pelo monitor serial
// ------------------------------------------------------------------
//

#include <WiFi.h>
#include <PCI_UG_T1.h>

PCI_UG_T1 x(2, 0);   //indica motor de passo no modo 2 em CN1 

const char* ssid     = "PCI_UG_T1";
const char* password = "00000000";

#define LED_BUILTIN 0

WiFiServer server(80);

String header;

String html="<html><head><meta charset='utf-8'><title></title></head><body><table border='0' width='600' id='table1' cellspacing='4' cellpadding='4' height='400'><tr><td colspan='6' align='center'><p align='center'><b><font size='4'>CONTROLE DO MOTOR DE PASSO<br>PARA TESTE DA PCI_UG_T1 VIA Wi-Fi</font></b></p></td></tr><tr><td colspan='6' align='center'>&nbsp;</td></tr><tr><td colspan='6' align='center' bgcolor='#FFCCFF'><font size='2'><b>VELOCIDADE</b></font></td></tr><tr><td align='center' bgcolor='#CV3' width='100'><a href='/V3'><font size='4'>&nbsp;&nbsp;3&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV5' width='100'><a href='/V5'><font size='4'>&nbsp;&nbsp;5&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV10' width='100'><a href='/V10'><font size='4'>&nbsp;&nbsp;10&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV20' width='100'><a href='/V20'><font size='4'>&nbsp;&nbsp;20&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV40' width='100'><a href='/V40'><font size='4'>&nbsp;&nbsp;40&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV80' width='100'><a href='/V80'><font size='4'>&nbsp;&nbsp;80&nbsp;&nbsp;</font></td></a></tr><tr><td colspan='6' align='center'>&nbsp;</td></tr><tr><td colspan='6' align='center' bgcolor='#CCFF99'><p align='center'><font size='2'><b>DIREÇÃO</b></font></td></tr><tr><td colspan='3' align='center' bgcolor='#CDE' width='300'><a href='/DE'><font size='4'>&nbsp;&nbsp;&nbsp;&nbsp;ESQUERDA&nbsp;&nbsp;&nbsp;&nbsp;</font></td></a><td colspan='3' align='center' bgcolor='#CDD' width='300'><a href='/DD'><font size='4'>&nbsp;&nbsp;&nbsp;&nbsp;DIREITA&nbsp;&nbsp;&nbsp;&nbsp;</font></td></a></tr><tr><td colspan='6' align='center'>&nbsp;</td></tr><tr><td colspan='6' align='center' bgcolor='#99CCFF'><p align='center'><font size='2'><b>MOVIMENTO</b></font></td></tr><tr><td align='center' bgcolor='#C15' width='100'><a href='/C15'><font size='4'>&nbsp;15º&nbsp;</font></td></a><td align='center' bgcolor='#C30' width='100'><a href='/C30'><font size='4'>&nbsp;30º&nbsp;</font></td></a><td align='center' bgcolor='#C45' width='100'><a href='/C45'><font size='4'>&nbsp;45º&nbsp;</font></td></a><td align='center' bgcolor='#C90' width='100'><a href='/C90'><font size='4'>&nbsp;90º&nbsp;</font></td></a><td align='center' bgcolor='#CCP' width='100'><a href='/CCP'><font size='2'>ANDA<br>PARA</font></td></a><td align='center' bgcolor='#1VS' width='100'><a href='/1VS'><font size='2'>UMA DOSE<br>VIA SENSOR</font></td></a></tr></table></body></html>";
String copyhtml;

uint32_t nsteps;   //steps a percorrer
int      graus=30; //graus a girar (999=contínuo, 0=para)
int      vel=5;    //velocidade de deslocamento sendo máxima 3 sentido 'D'; máxima 5 sentido 'E'
char     sent='D'; //sentido esquerda ou direita (E ou D)

void setup() {
  Serial.begin(115200);
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

void loop(){
  WiFiClient client = server.available();
  if (client) {
    if (((graus==555)||(graus==999))&&(x.xsteps[0]>0)){x.xsteps[0]=2;} //se chegou comando via browser, aborta movimento
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //quando recebe um comando do browser, emite um beep de 0.2s e pisca o led 10 vezes
            x.bdur=200;x.bfreq=2000;x.binter=0;x.bnum=1;
            x.ldur=50;x.linter=25;x.lnum=10;
            
            //
            // atualiza as variáveis vel, sent e graus
            // conforme os clicks que foram dados na página lá no cliente
            //

            if (header.indexOf("GET /V3")  >= 0) {vel=3;}
            if (header.indexOf("GET /V5")  >= 0) {vel=5;}
            if (header.indexOf("GET /V10") >= 0) {vel=10;}
            if (header.indexOf("GET /V20") >= 0) {vel=20;}
            if (header.indexOf("GET /V40") >= 0) {vel=40;}
            if (header.indexOf("GET /V80") >= 0) {vel=80;}
            
            if (header.indexOf("GET /DE") >= 0) {sent='E';}
            if (header.indexOf("GET /DD") >= 0) {sent='D';}

            if (header.indexOf("GET /C15") >= 0) {graus=15;}
            if (header.indexOf("GET /C30") >= 0) {graus=30;}
            if (header.indexOf("GET /C45") >= 0) {graus=45;}
            if (header.indexOf("GET /C90") >= 0) {graus=90;}
            if (header.indexOf("GET /CCP") >= 0) {if (graus==999){graus=0;}else{graus=999;}}
            if (header.indexOf("GET /1VS") >= 0) {graus=555;}

            copyhtml=html;

            //
            // atualiza as cores de destaque das 
            // variáveis vel, sent e graus conforme os valores atuais
            //
            
            if (vel==3) {copyhtml.replace("#CV3", "#FF9999");}else{copyhtml.replace("#CV3", "#FFCCFF");}
            if (vel==5) {copyhtml.replace("#CV5", "#FF9999");}else{copyhtml.replace("#CV5", "#FFCCFF");}
            if (vel==10){copyhtml.replace("#CV10","#FF9999");}else{copyhtml.replace("#CV10","#FFCCFF");}
            if (vel==20){copyhtml.replace("#CV20","#FF9999");}else{copyhtml.replace("#CV20","#FFCCFF");}
            if (vel==40){copyhtml.replace("#CV40","#FF9999");}else{copyhtml.replace("#CV40","#FFCCFF");}
            if (vel==80){copyhtml.replace("#CV80","#FF9999");}else{copyhtml.replace("#CV80","#FFCCFF");}

            if (sent=='E') {copyhtml.replace("#CDE", "#33CC33");}else{copyhtml.replace("#CDE", "#CCFF99");}
            if (sent=='D') {copyhtml.replace("#CDD", "#33CC33");}else{copyhtml.replace("#DDD", "#CCFF99");}

            if (graus==15) {copyhtml.replace("#C15", "#0099FF");}else{copyhtml.replace("#C15", "#99CCFF");}
            if (graus==30) {copyhtml.replace("#C30", "#0099FF");}else{copyhtml.replace("#C30", "#99CCFF");}
            if (graus==45) {copyhtml.replace("#C45", "#0099FF");}else{copyhtml.replace("#C45", "#99CCFF");}
            if (graus==90) {copyhtml.replace("#C90", "#0099FF");}else{copyhtml.replace("#C90", "#99CCFF");}
            if (graus==999){copyhtml.replace("#CCP", "#0099FF");}else{copyhtml.replace("#CCP", "#99CCFF");}
            if (graus==555){copyhtml.replace("#1VS", "#0099FF");}else{copyhtml.replace("#1VS", "#99CCFF");}

            // envia a página com os botões atualizados
            client.println(copyhtml);

            //starta o motor de acordo com o comando recebido
            if (graus!=555){
              while(x.xsteps[0]>0){}
              if (graus==999){nsteps=2048000000;}
              if (graus!=999){nsteps=map(graus,0,360,0,2048);}
              if (sent=='D'){x.runStep(0, nsteps, vel, true);}else{x.runStep(0, nsteps, vel, false);}
            }  

            //se foi para rodar uma dose baseado só no sensor, executa
            if (graus==555){
              Serial.println("");
              bool dir;
              if (sent=='D'){dir=true;}else{dir=false;}
              while (analogRead(34)<3900){
                Serial.print(analogRead(34));Serial.print(",");
                while(x.xsteps[0]>0){};
                x.runStep(0, 6, vel, dir); //gira aprox 1 grau por vez até o início de uma divisão (preto)
              }  
              Serial.println("");
              while(x.xsteps[0]>0){};
              while (analogRead(34)>3900){
                Serial.print(analogRead(34));Serial.print(",");
                while(x.xsteps[0]>0){};
                x.runStep(0, 6, vel, dir); //gira aprox 1 grau por vez até o fim da divisão (sai do preto)
              }
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
}
