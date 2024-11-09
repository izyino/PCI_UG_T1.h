const char ini_rede[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <title>Inicializa rede WiFi a ser utilizada</title>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <style>
      input {
        caret-color: red;
      }

      body {
        margin: 10px;
        width: 100vw;
        background: #ecf0f3;
        display: flex;
        align-items: center;
        text-align: center;
        justify-content: center;
        place-items: center;
        overflow: hidden;
        font-family: Arial, Helvetica, sans-serif;
        overflow-y: visible;
      }

    h1 {
      font-size: 18px;
      font-weight: 600;
      text-align: center;
      color: #3594ff;
    }


    h2 {
      font-size: 1.2rem;
      text-align: center;
      color: #1da1f2;
    }

    h3 {
      font-size: 1.2rem;
      text-align: center;
      line-height: 36px;
      color: #1da1f2;
    }

    p {
    font-size: 12px;
    text-indent: 10px;
    text-align: justify;
    }

      .painel {
        margin-top:5px;
        margin-left:10px;
        margin-button:10px;
        margin-right:10px;
        position: relative;
        width: 300px;
        height: 550px;
        border-radius: 20px;
        padding: 20px;
        box-sizing: border-box;
        background: #ecf0f3;
        box-shadow: 14px 14px 20px #cbced1, -14px -14px 20px white;
      }

      .inputs {
        text-align: center;
        margin-top: 30px;
        margin-bottom: 5px;
        width: 100%;
      }

      label,
      input,
      button {
        text-align: center;
        display: block;
        width:100%;
        padding: 0;
        border: none;
        outline: none;
        box-sizing: border-box;
      }

      label {
        margin-bottom: 0px;
      }

      label:nth-of-type(2) {
        margin-top: 5px;
      }

      input::placeholder {
        color: gray;
      }


      .titulo {
        font-size: 14px;
        text-align: center;
        font-weight: 600;
        padding: 2px;
        border: none;
        outline: none;
        color: black;
      }

      input {
        background: #ecf0f3;
        padding: 5px;
        padding-left: 20px;;
        margin-top: 5px;
        height: 50px;
        font-size: 30px;
        border-radius: 50px;
        box-shadow: inset 6px 6px 6px #cbced1, inset -6px -6px 6px white;
      }
      .i200 {
        height: 36px;
        width: 72px;    //260px;
      }

      button {
        color: #707070;
        margin-top: 13px;
        background: #9dc9ff;
        height: 70px;
        width: 100%;
        padding-top: 0px;
        border-radius: 25px;
        cursor: pointer;
        font-weight: 900;
        font-size: 20px;
        box-shadow: 6px 6px 6px #cbced1, -6px -6px 6px white;
        transition: 0.5s;
      }
    
      button:hover {
        box-shadow: none;
      }

      .knob {
        color: white;
        font-size: 1.8rem;
        align-items: center;
        background: #9dc9ff;
        line-height: 50px;
        height: 50px;
        width: 50px;
        margin: 5px;
        cursor: pointer;
        font-weight: 900;
        box-shadow: 6px 6px 6px #cbced1, -6px -6px 6px white;
        transition: 0.5s;
      }

      .knob:hover {
        box-shadow: none;
      }

      .knob1 {
        margin: 0;  
        color: white;
        font-size: 1.8rem;
        text-align: center;
        background: #9dc9ff;
        line-height: 50px; 
        height: 50px;
        width: 50px;
        margin: 5px;
        cursor: pointer;
        font-weight: 900;
        box-shadow: 6px 6px 6px #cbced1, -6px -6px 6px white;
        transition: 0.5s;
      }

      .knob1:hover {
        box-shadow: none;
      }

     
    .ib {
      display:flex;
      flex-direction: row;
      align-items: center;
      text-align: center;
      justify-content: center;
      place-items: center;
    }

      a {
        position: absolute;
        font-size: 8px;
        text-align: center;
        bottom: 4px;
        right: 4px;
        padding: 6px;
        text-decoration: none;
        color: black;
        border-radius: 5px;
      }

      h2 {
        font-size: 8px;
        text-align: center;
        bottom: 4px;
        right: 4px;
        padding: 6px;
        text-decoration: none;
        color: black;
      }
    </style>
  
  </head>
  
  <body onload="leredes()"> <!- Lê as redes disponíveis assim que carregar o HTML ->
  <main>
  
    <section id="inicial" class="painel" style="display:block">
    <h1>NOMEDOPRODUTO<br>SUBNOMESUBNOME<br></h1>
      <div class="button">
        <button title="Define o programa de alimentação - Tela 1" onclick="mudapag('inicial','pref1')" class="button">Programa<br>de alimentação<label class=titulo></label></button>
        <button title="Altera as configurações e as preferências - Tela 2" onclick="mudapag('inicial','pref2')" class="button">Configurações<br>e preferências<label class=titulo></label></button>
        <button title="" onclick="" class="button"><label class=titulo><br></label></button>
        <button title="" onclick="" class="button"><label class=titulo><br></label></button>
        <button title="" onclick="" class="button"><label class=titulo><br></label></button>        
      </div>
      <h2><br>NOMENOME - Rev. <span id="vers0">x.y</span></h2>
    </section>

    
    <section id="pref1" class="painel" style="display:none">
      <table border="0" width="100%" id="table1" cellspacing="0" cellpadding="0">
        <tr>
          <td><h3>TELA 1<h3></td>
          <td class="ib">
            <button type="button" title="Confirma a programação abaixo" class="knob" onclick="gravarede()" class="button">&#9658;</button>
            <button type="button" title="Não altera nada e volta para a tela inicial" class="knob" onclick="mudapag('pref1','inicial')" class="button">&#8617;</button>
          </td>
        </tr>
      </table>
      <form class="inputs" >
        <label class=titulo>01</label><div class="ib"><p id="rede00"/></p></div>
        <label class=titulo>02</label><div class="ib"><p id="rede01"/></p></div>
        <label class=titulo>03</label><div class="ib"><p id="rede02"/></p></div>
        <label class=titulo>04</label><div class="ib"><p id="rede03"/></p></div>
        <label class=titulo>05</label><div class="ib"><p id="rede04"/></p></div>
        <label class=titulo>06</label><div class="ib"><p id="rede05"/></p></div>
        <label class=titulo>07</label><div class="ib"><p id="rede06"/></p></div>
        <label class=titulo>08</label><div class="ib"><p id="rede07"/></p></div>
        <label class=titulo>09</label><div class="ib"><p id="rede08"/></p></div>
        <label class=titulo>10</label><div class="ib"><p id="rede09"/></p></div>
        <label class=titulo>11</label><div class="ib"><p id="rede10"/></p></div>
        <label class=titulo>12</label><div class="ib"><p id="rede11"/></p></div>
      </form>
      <h2><br>NOMENOME - Rev. <span id="vers1">x.y</span></h2>
    </section>


    <section id="pref2" class="painel" style="display:none">
      <table border="0" width="100%" id="table2" cellspacing="0" cellpadding="0">
        <tr>
          <td><h3>TELA 2<h3></td>
          <td class="ib">
            <button type="button" title="Confirma os valores abaixo" class="knob" onclick="gravarede()" class="button">&#9658;</button>
            <button type="button" title="Não altera nada e volta para a tela inicial" class="knob" onclick="mudapag('pref2','inicial')" class="button">&#8617;</button>
          </td>
        </tr>
      </table>
      <form class="inputs" >
        <label class=titulo>PARÂMETRO 1<br>xxxxxxxxxxxxxx</label><div class="ib"><input class="i200" type="number" id="parm1" step="1" min="0" max="255" /></div>
        <label class=titulo>PARÂMETRO 2<br>xxxxxxxxxxxxxx</label><div class="ib"><input class="i200" type="number" id="parm2" step="1" min="0" max="255" /></div>
        <label class=titulo>PARÂMETRO 3<br>xxxxxxxxxxxxxx</label><div class="ib"><input class="i200" type="number" id="parm3" step="1" min="0" max="255" /></div>
        <label class=titulo>PARÂMETRO 4<br>xxxxxxxxxxxxxx</label><div class="ib"><input class="i200" type="number" id="parm4" step="1" min="0" max="255" /></div>        
       </form>
    <h2><br><br><br><br>NOMENOME - Rev. <span id="vers2">x.y</span></h2>
    </section>


    <script> 
    // ===================== leitura das preferências atuais ===========================================
      function leredes() {
        var redes;
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            redes = this.responseText.split("&");

            document.getElementById("rede00").value=redes[0];
            document.getElementById("rede01").value=redes[1];
            document.getElementById("rede02").value=redes[2];
            document.getElementById("rede03").value=redes[3];
            document.getElementById("rede04").value=redes[4];
            document.getElementById("rede05").value=redes[5];
            document.getElementById("rede06").value=redes[6];
            document.getElementById("rede07").value=redes[7];
            document.getElementById("rede08").value=redes[8];
            document.getElementById("rede09").value=redes[9];
            document.getElementById("rede10").value=redes[10];
            document.getElementById("rede11").value=redes[11];

          };  
        };

        xhttp.open("GET", "leredes", true);
        xhttp.send();
      };


    // ===================== muda página (da página que vem, para a página que vai) ===================
      function mudapag(xvem, xvai) {
        var vemx = document.getElementById(xvem);
        var vaix = document.getElementById(xvai);
        vaix.style.display = "block";
        vemx.style.display = "none";
      };


    // ===================== grava preferências ===========================================
    function gravarede() {
      var msg ="";

      var redes[0] = document.getElementById("rede00").value;
      var redes[1] = document.getElementById("rede01").value;
      var redes[2] = document.getElementById("rede02").value;
      var redes[3] = document.getElementById("rede03").value;
      var redes[4] = document.getElementById("rede04").value;
      var redes[5] = document.getElementById("rede05").value;
      var redes[6] = document.getElementById("rede06").value;
      var redes[7] = document.getElementById("rede07").value;
      var redes[8] = document.getElementById("rede08").value;
      var redes[9] = document.getElementById("rede09").value;
      var redes[10] = document.getElementById("rede10").value;
      var redes[11] = document.getElementById("rede11").value;
      
      msg = "0="+redes[0]+"&1="+redes[1]+"&2="+redes[2]+"&3="+redes[3]+"&4="+redes[4]+"&5="+redes[5]+"&6="+redes[6]+"&7="+redes[7]+"&8="+redes[8]+"&9="+redes[9]+"&10="+redes[10]+"&11="+redes[11];
      
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() { // The readystatechange event is fired whenever the readyState property of the XMLHttpRequest changes.
        if (this.readyState == 4 && this.status == 200) { // 4 = The operation is complete.
          document.getElementById("mensagem").innerHTML = this.responseText;
        };
      };
      xhttp.open("GET", "gravarede?"+msg, true);
      xhttp.send();
    };

    </script>

  </body>
  
</html>
)=====";
