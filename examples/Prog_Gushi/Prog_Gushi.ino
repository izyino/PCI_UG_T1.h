/***
  * 
  * Exemplo n.2 de utilização da biblioteca library PCI_UG_T1.h
  * via Wi-Fi, com o servidor web do ESP-32 definido como access-point
  * ------------------------------------------------------------------
  * Pressupõe um motor de passo 28byj-48 (motor n.0) conectado a CN1
  * Rede Wi-Fi criada com o nome PCI_UG_T1 senha 12345678, acessível
  * pelo IP 192.168.4.1 ou outro, informado pelo monitor serial
  * ------------------------------------------------------------------
  *
  * https://randomnerdtutorials.com/esp32-wi-fi-manager-asyncwebserver/
 ***/

// Import required libraries
#include <WiFi.h>
//#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <PCI_UG_T1.h>

PCI_UG_T1 motor(2, 0); // indica motor de passo no modo 2 em CN1 e motor(es) DC em CN2

// Replace with your network credentials
const char *ssid = "web_server";
const char *password = "12345678";

bool ledState = 0;
const int ledPin = 2;
int velocidade, angulo, sentido, steps;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//AsyncWebSocket ws("/ws");

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 123);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="pt-BR">
<head>
  <title>PCI UG 28BYJ-48</title>
  <meta charset="utf-8"/>
  <meta name="viewport" conteudo="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #000000;
  }
  .cabec {
    overflow: hidden;
    background-color: #0F8B8D;
  }
  body {
    margin: 0;
  }
  .conteudo {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .cartao {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .botao {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /.botao:hover {background-color: #0f8b8d}/
   .botao:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<meta name="viewport" conteudo="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="cabec">
    <h1>PCI UG 28BYJ-48</h1>
  </div>
  <div class="conteudo">
    <div class="cartao">
    <form action="">
      <h2>Parâmetros do motor</h2>
        <label>Tempo por passo (ms): </label>
        <input type="number" list="velocidade"  id="Velocidade">
        <datalist id="velocidade">
            <option value=3>
            <option value=5>
            <option value=10>
            <option value=20>
            <option value=40>
        </datalist><br>
        <label>Ângulo (°): </label>
        <input type="number" list="angulo" id="Angulo">
        <datalist id="angulo">
            <option value=15>
            <option value=30>
            <option value=45>
            <option value=90>
            <option value=180>
        </datalist><br>
        <label>Sentido: </label>
        <input type="text" list="sentido" id="Sentido">
        <datalist id="sentido">
            <option value="horário">
            <option value="anti-horário">
        </datalist>
    </form>
      <p id="mensagem"></p>
      <button class="botao" type="button" onclick="girar()" id="botao">GIRAR</button>
    </div>
  </div>
<script>
  function girar() {
    var xhttp = new XMLHttpRequest();
    var velocidade = document.getElementById('Velocidade').value;
    var angulo = document.getElementById('Angulo').value;
    var giro = (document.getElementById('Sentido').value=="horário")?0:1;
 xhttp.onreadystatechange = function () {
 if (this.readyState == 4 && this.status == 200) {
  document.getElementById("mensagem").innerHTML = this.responseText;}};
 var msg="&v="+velocidade+"&a="+angulo+"&g="+giro;
 console.log("giro?" + msg);
 xhttp.open("GET", "girar?" + msg, true);
 xhttp.send();
  }
</script>
</body>
</html>
)rawliteral";

void setup()
{
  Serial.begin(115200);
  delay(500);
  motor.begin();
  Serial.println("Setting soft access point mode!");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", html); 
            Serial.println("Enviou pagina!");});
  server.on("/girar", HTTP_GET, [](AsyncWebServerRequest *request) { // 
    AsyncWebParameter *p = request->getParam("v");
    velocidade = p->value().toInt();
    p = request->getParam("a");
    angulo = p->value().toInt();
    p = request->getParam("g");
    sentido = p->value().toInt();
    Serial.print("Velocidade: ");
    Serial.print(velocidade);
    Serial.print(", angulo: ");
    Serial.print(angulo);
    Serial.print(", sentido: ");
    if(sentido==0)
    {
      Serial.println("horario");
    } else
    {
      Serial.println("anti-horario");
    }

    steps = map(angulo, 0, 360, 0, 1024);
    motor.runStep(0, steps, velocidade, sentido);
    Serial.print("Girando ");
    Serial.print(steps);
    Serial.print(" passos, com velocidade ");
    Serial.print(velocidade);
    Serial.print(" e sentido ");
    Serial.println(sentido);
});
}

void loop()
{
}
