/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-websocket-server-arduino/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

use code to assign STATE
//<hr/>
//  <div class="content">
//    <div class="card">
//      <h2>Save As Default</h2>
//      <p class="state">state: <span id="state">%STATE%</span></p>
//      <p><button id="button" class="button">Toggle</button></p>
//    </div>
//  </div>

  
*********/

//#define DEBUG

// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>

// How many leds are in the strip?

// 

// Data pin that led data will be written out over
#define DATA_PIN 4

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

/*
  #define NUM_LEDS 23
  #define RSINKCAB_ST 0
  #define RSINKCAB_SZ 4
  #define LSINKCAB_ST 4
  #define LSINKCAB_SZ 4
  #define RSTOVECAB_ST 8
  #define RSTOVECAB_SZ 5
  #define LSTOVECAB_ST 13
  #define LSTOVECAB_SZ 4
  #define REFRIGCAB_ST 17
  #define REFRIGCAB_SZ 6
*/
  #define NUM_LEDS 240
  #define RSINKCAB_ST 0
  #define RSINKCAB_SZ 45
  #define LSINKCAB_ST 45
  #define LSINKCAB_SZ 45
  #define RSTOVECAB_ST 90
  #define RSTOVECAB_SZ 37
  #define LSTOVECAB_ST 127
  #define LSTOVECAB_SZ 37
  #define REFRIGCAB_ST 164
  #define REFRIGCAB_SZ 76

 
// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];


  


// Replace with your network credentials
const char* ssid = "HomeNetName";
const char* password = "Password";

const char* PARAM_LSINK = "leftSinkDefault";
const char* PARAM_RSINK = "rightSinkDefault";
const char* PARAM_LSTOVE = "leftStoveDefault";
const char* PARAM_RSTOVE = "rightStoveDefault";
const char* PARAM_REFRIG = "refrigDefault";

String A_Str = "A127,127,127";
String B_Str = "B127,127,127";
String C_Str = "C127,127,127";
String D_Str = "D127,127,127";
String E_Str = "E127,127,127";

bool ledState = 0;
const int ledPin = 2;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Kitchen Cabinet LED Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
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
    color: #143642;
  }
  h3 {
    font-size: 1.8rem;
    color: black;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
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
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
   .rightsinkgroup{
    ;
   }
.leftsinkgroup{
  ;
}
.rightstovegroup{
  ;
}
.leftstovegroup{
  ;
}
.refriggroup{
  ;
}
.parent {
  margin: 1rem;
  padding: 2rem 2rem;
  text-align: center;
}
.child {
  display: inline-block;
  padding: 1rem 1rem;
  vertical-align: middle;
}
   .slidecontainer {

   }

.slider {
  -webkit-appearance: none;
  height: 15px;
  border-radius: 5px;
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px;
  height: 25px;
  background: #4CAF50;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 25px;
  background: #4CAF50;
  cursor: pointer;
}
  </style>
 
</head>
<body>
  <div class="topnav">
    <h1>Kitchen Cabinet LED Server</h1>
  </div>


<hr/>
<div class="rightsinkgroup">
<h3>RightSink Default: </h3>
<div>
<div class="slidecontainer">
 <p><input oninput="changeAllRightSink()" onchange="changeAllRightSink()" type="range" min="0" max="255" value="127" class="slider" id="rightSinkRangeR">
  RED: <span id="rightSinkR"></span></p>
</div>

<div class="slidecontainer">
 <p><input oninput="changeAllRightSink()" onchange="changeAllRightSink()" type="range" min="0" max="255" value="127" class="slider" id="rightSinkRangeG">
  GREEN: <span id="rightSinkG"></span></p>
</div>

<div class="slidecontainer">
 <p><input oninput="changeAllRightSink()" onchange="changeAllRightSink()" type="range" min="0" max="255" value="127" class="slider" id="rightSinkRangeB">
  BLUE: <span id="rightSinkB"></span></p>
</div>
</div>

<div class="parent">
<div class='child'>

      <p><button id="rightSinkButton" onclick="saveRightSinkDefault()" class="button">Save</button></p>

</div>
<div class='child'>
<div style="width:80px; height: 80px; margin: auto; border: 5px outset black; background: #7F7F7F;"id="rightSinkBack">
</div>
</div>
</div>

</div>
<hr/>

<div class="leftsinkgroup">
<h3>LeftSink Default: </h3>
<div>
<div class="slidecontainer">
 <p><input oninput="changeAllLeftSink()" onchange="changeAllLeftSink()" type="range" min="0" max="255" value="127" class="slider" id="leftSinkRangeR">
  RED: <span id="leftSinkR"></span></p>
</div>

<div class="slidecontainer">
 <p><input oninput="changeAllLeftSink()" onchange="changeAllLeftSink()" type="range" min="0" max="255" value="127" class="slider" id="leftSinkRangeG">
  GREEN: <span id="leftSinkG"></span></p>
</div>

<div class="slidecontainer">
 <p><input oninput="changeAllLeftSink()" onchange="changeAllLeftSink()" type="range" min="0" max="255" value="127" class="slider" id="leftSinkRangeB">
  BLUE: <span id="leftSinkB"></span></p>
</div>
</div>

<div class="parent">
<div class='child'>

      <p><button id="leftSinkButton" onclick="saveLeftSinkDefault()" class="button">Save</button></p>

</div>
<div class='child'>
<div style="width:80px; height: 80px; margin: auto; border: 5px outset black; background: #7F7F7F;"id="leftSinkBack">
</div>
</div>
</div>

</div>
<hr/>

<div class="rightstovegroup">
<h3>RightStove Default:</h3>
<div>
<div class="slidecontainer">
 <p><input oninput="changeAllRightStove()" onchange="changeAllRightStove()" type="range" min="0" max="255" value="127" class="slider" id="rightStoveRangeR">
  RED: <span id="rightStoveR"></span></p>
</div>

<div class="slidecontainer">
 <p><input oninput="changeAllRightStove()" onchange="changeAllRightStove()" type="range" min="0" max="255" value="127" class="slider" id="rightStoveRangeG">
  GREEN: <span id="rightStoveG"></span></p>
</div>

<div class="slidecontainer">
 <p><input oninput="changeAllRightStove()" onchange="changeAllRightStove()" type="range" min="0" max="255" value="127" class="slider" id="rightStoveRangeB">
  BLUE: <span id="rightStoveB"></span></p>
</div>
</div>

<div class="parent">
<div class='child'>

      <p><button id="rightStoveButton" onclick="saveRightStoveDefault()" class="button">Save</button></p>

</div>
<div class='child'>
<div style="width:80px; height: 80px; margin: auto; border: 5px outset black; background: #7F7F7F;"id="rightStoveBack">
</div>
</div>
</div>

</div>
<hr/>
  <div class="leftstovegroup">
<h3>LeftStove Default: </h3>
<div>
<div class="slidecontainer">
 <p><input oninput="changeAllLeftStove()" onchange="changeAllLeftStove()" type="range" min="0" max="255" value="127" class="slider" id="leftStoveRangeR">
  RED: <span id="leftStoveR"></span></p>
</div>

<div class="slidecontainer">
 <p><input oninput="changeAllLeftStove()" onchange="changeAllLeftStove()" type="range" min="0" max="255" value="127" class="slider" id="leftStoveRangeG">
  GREEN: <span id="leftStoveG"></span></p>
</div>

<div class="slidecontainer">
 <p><input oninput="changeAllLeftStove()" onchange="changeAllLeftStove()" type="range" min="0" max="255" value="127" class="slider" id="leftStoveRangeB">
  BLUE: <span id="leftStoveB"></span></p>
</div>
</div>

<div class="parent">
<div class='child'>

      <p><button id="leftStoveButton" onclick="saveLeftStoveDefault()" class="button">Save</button></p>

</div>
<div class='child'>
<div style="width:80px; height: 80px; margin: auto; border: 5px outset black; background: #7F7F7F;"id="leftStoveBack">
</div>
</div>
</div>

</div>
<hr/>
  <div class="refriggroup">
<h3>Refrig Default: </h3>
<div>
<div class="slidecontainer">
 <p><input oninput="changeAllRefrig()" onchange="changeAllRefrig()" type="range" min="0" max="255" value="127" class="slider" id="refrigRangeR">
  RED: <span id="refrigR"></span></p>
</div>

<div class="slidecontainer">
 <p><input oninput="changeAllRefrig()" onchange="changeAllRefrig()" type="range" min="0" max="255" value="127" class="slider" id="refrigRangeG">
  GREEN: <span id="refrigG"></span></p>
</div>

<div class="slidecontainer">
 <p><input oninput="changeAllRefrig()" onchange="changeAllRefrig()" type="range" min="0" max="255" value="127" class="slider" id="refrigRangeB">
  BLUE: <span id="refrigB"></span></p>
</div>
</div>

<div class="parent">
<div class='child'>

      <p><button id="refrigButton" onclick="saveRefrigDefault()" class="button">Save</button></p>

</div>
<div class='child'>
<div style="width:80px; height: 80px; margin: auto; border: 5px outset black; background: #7F7F7F;"id="refrigBack">
</div>
</div>
</div>

</div>

<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
    


  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
   // setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var receivData = event.data;
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
     if(receivData[0]== 'A')
    {
      receivData = receivData.substring(1);
      var strArr = receivData.split(',');
      document.getElementById('rightSinkRangeR').value = parseInt(strArr[0]);
      document.getElementById('rightSinkRangeG').value = parseInt(strArr[1]);
      document.getElementById('rightSinkRangeB').value = parseInt(strArr[2]);
      document.getElementById('rightSinkR').innerHTML = strArr[0];
      document.getElementById('rightSinkG').innerHTML = strArr[1];
      document.getElementById('rightSinkB').innerHTML = strArr[2];
      document.getElementById('rightSinkBack').style.backgroundColor = "rgb(" + parseInt(strArr[0])+ "," + parseInt(strArr[1]) + "," + parseInt(strArr[2]) + ")";    
    }
     if(receivData[0]== 'B')
    {
      receivData = receivData.substring(1);
      var strArr = receivData.split(',');
      document.getElementById('leftSinkRangeR').value = parseInt(strArr[0]);
      document.getElementById('leftSinkRangeG').value = parseInt(strArr[1]);
      document.getElementById('leftSinkRangeB').value = parseInt(strArr[2]);
      document.getElementById('leftSinkR').innerHTML = strArr[0];
      document.getElementById('leftSinkG').innerHTML = strArr[1];
      document.getElementById('leftSinkB').innerHTML = strArr[2];
      document.getElementById('leftSinkBack').style.backgroundColor = "rgb(" + parseInt(strArr[0])+ "," + parseInt(strArr[1]) + "," + parseInt(strArr[2]) + ")";    
    }
     if(receivData[0]== 'C')
    {
      receivData = receivData.substring(1);
      var strArr = receivData.split(',');
      document.getElementById('rightStoveRangeR').value = parseInt(strArr[0]);
      document.getElementById('rightStoveRangeG').value = parseInt(strArr[1]);
      document.getElementById('rightStoveRangeB').value = parseInt(strArr[2]);
      document.getElementById('rightStoveR').innerHTML = strArr[0];
      document.getElementById('rightStoveG').innerHTML = strArr[1];
      document.getElementById('rightStoveB').innerHTML = strArr[2];
      document.getElementById('rightStoveBack').style.backgroundColor = "rgb(" + parseInt(strArr[0])+ "," + parseInt(strArr[1]) + "," + parseInt(strArr[2]) + ")";    
    }
     if(receivData[0]== 'D')
    {
      receivData = receivData.substring(1);
      var strArr = receivData.split(',');
      document.getElementById('leftStoveRangeR').value = parseInt(strArr[0]);
      document.getElementById('leftStoveRangeG').value = parseInt(strArr[1]);
      document.getElementById('leftStoveRangeB').value = parseInt(strArr[2]);
      document.getElementById('leftStoveR').innerHTML = strArr[0];
      document.getElementById('leftStoveG').innerHTML = strArr[1];
      document.getElementById('leftStoveB').innerHTML = strArr[2];
      document.getElementById('leftStoveBack').style.backgroundColor = "rgb(" + parseInt(strArr[0])+ "," + parseInt(strArr[1]) + "," + parseInt(strArr[2]) + ")";    
    }
    if(receivData[0]== 'E')
    {
      receivData = receivData.substring(1);
      var strArr = receivData.split(',');
      document.getElementById('refrigRangeR').value = parseInt(strArr[0]);
      document.getElementById('refrigRangeG').value = parseInt(strArr[1]);
      document.getElementById('refrigRangeB').value = parseInt(strArr[2]);
      document.getElementById('refrigR').innerHTML = strArr[0];
      document.getElementById('refrigG').innerHTML = strArr[1];
      document.getElementById('refrigB').innerHTML = strArr[2];
      document.getElementById('refrigBack').style.backgroundColor = "rgb(" + parseInt(strArr[0])+ "," + parseInt(strArr[1]) + "," + parseInt(strArr[2]) + ")";    
    }
    //document.getElementById('state').innerHTML = state;
  }
  
  function onLoad(event) {
      initWebSocket();

      initPage();
}  

function sendMessage(msg){
    // Wait until the state of the socket is not ready and send the message when it is...
    waitForSocketConnection(websocket, function(){
        console.log("message sent!!!");
        websocket.send(msg);
    });
}

// Make the function wait until the connection is made...
function waitForSocketConnection(socket, callback){
    setTimeout(
        function () {
            if (socket.readyState === 1) {
                console.log("Connection is made")
                if (callback != null){
                    callback();
                }
            } else {
                console.log("wait for connection...")
                waitForSocketConnection(socket, callback);
            }

        }, 5); // wait 5 milisecond for the connection...
}

  
  function toggle(){
   // websocket.send('toggle');
  }

  function initPage(){
     sendMessage('init');
    }
    
  
  function initSliders(){
  
    var r = 127;
    var g = 127;
    var b = 127;
    document.getElementById('rightSinkBack').style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
    document.getElementById('leftSinkBack').style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
    document.getElementById('rightStoveBack').style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
  }

var sliderR = document.getElementById("rightSinkRangeR");
var outputR = document.getElementById("rightSinkR");
var divback = document.getElementById("rightSinkBack");
outputR.innerHTML = sliderR.value;
sliderR.oninput = function() {
  outputR.innerHTML = this.value;
  changeAllRightSink();
}

var sliderG = document.getElementById("rightSinkRangeG");
var outputG = document.getElementById("rightSinkG");
outputG.innerHTML = sliderG.value;
sliderG.oninput = function() {
  outputG.innerHTML = this.value;
  changeAllRightSink();
}

var sliderB = document.getElementById("rightSinkRangeB");
var outputB = document.getElementById("rightSinkB");
outputB.innerHTML = sliderB.value;
sliderB.oninput = function() {
  outputB.innerHTML = this.value;
  changeAllRightSink();
}

function saveRightSinkDefault() {
    var r = document.getElementById('rightSinkR').innerHTML;
    var g = document.getElementById('rightSinkG').innerHTML;
    var b = document.getElementById('rightSinkB').innerHTML;
    var hexColor =   r + ','+  g + ','+ b ;
    //document.getElementById('rightSinkDefault').innerHTML = hexColor;
    var hexColor =  'sa'+ r + ','+  g + ','+ b ;
    websocket.send(hexColor);  
}

function changeAllRightSink() {
    var r = document.getElementById('rightSinkR').innerHTML;
    var g = document.getElementById('rightSinkG').innerHTML;
    var b = document.getElementById('rightSinkB').innerHTML;
    var hexColor =  'a'+ r + ','+  g + ','+ b ;
    document.getElementById('rightSinkBack').style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
    console.log('Sending hex Color: '+ hexColor);
    websocket.send(hexColor);
}

var sliderLeftSinkR = document.getElementById("leftSinkRangeR");
var outputLeftSinkR = document.getElementById("leftSinkR");
var divbackLeftSink = document.getElementById("leftSinkBack");
outputLeftSinkR.innerHTML = sliderLeftSinkR.value;
sliderLeftSinkR.oninput = function() {
  outputLeftSinkR.innerHTML = this.value;
  changeAllLeftSink();
}

var sliderLeftSinkG = document.getElementById("leftSinkRangeG");
var outputLeftSinkG = document.getElementById("leftSinkG");
outputLeftSinkG.innerHTML = sliderLeftSinkG.value;
sliderLeftSinkG.oninput = function() {
  outputLeftSinkG.innerHTML = this.value;
  changeAllLeftSink();
}

var sliderLeftSinkB = document.getElementById("leftSinkRangeB");
var outputLeftSinkB = document.getElementById("leftSinkB");
outputLeftSinkB.innerHTML = sliderLeftSinkB.value;
sliderLeftSinkB.oninput = function() {
  outputLeftSinkB.innerHTML = this.value;
  changeAllLeftSink();
}

function saveLeftSinkDefault() {
    var r = document.getElementById('leftSinkR').innerHTML;
    var g = document.getElementById('leftSinkG').innerHTML;
    var b = document.getElementById('leftSinkB').innerHTML;
    var hexColor =   r + ','+  g + ','+ b ;
  //  document.getElementById('leftSinkDefault').innerHTML = hexColor;
    var hexColor =  'sb'+ r + ','+  g + ','+ b ;
    websocket.send(hexColor);  
}

function changeAllLeftSink() {
    var r = document.getElementById('leftSinkR').innerHTML;
    var g = document.getElementById('leftSinkG').innerHTML;
    var b = document.getElementById('leftSinkB').innerHTML;
    var hexColor =  'b'+ r + ','+  g + ','+ b ;
    document.getElementById('leftSinkBack').style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
    websocket.send(hexColor);
}

var sliderRightStoveR = document.getElementById("rightStoveRangeR");
var outputRightStoveR = document.getElementById("rightStoveR");
var divbackRightStove = document.getElementById("rightStoveBack");
outputRightStoveR.innerHTML = sliderRightStoveR.value;
sliderRightStoveR.oninput = function() {
  outputRightStoveR.innerHTML = this.value;
  changeAllRightStove();
}

var sliderRightStoveG = document.getElementById("rightStoveRangeG");
var outputRightStoveG = document.getElementById("rightStoveG");
outputRightStoveG.innerHTML = sliderRightStoveG.value;
sliderRightStoveG.oninput = function() {
  outputRightStoveG.innerHTML = this.value;
  changeAllRightStove();
}

var sliderRightStoveB = document.getElementById("rightStoveRangeB");
var outputRightStoveB = document.getElementById("rightStoveB");
outputRightStoveB.innerHTML = sliderRightStoveB.value;
sliderRightStoveB.oninput = function() {
  outputRightStoveB.innerHTML = this.value;
  changeAllRightStove();
}

function saveRightStoveDefault() {
    var r = document.getElementById('rightStoveR').innerHTML;
    var g = document.getElementById('rightStoveG').innerHTML;
    var b = document.getElementById('rightStoveB').innerHTML;
    var hexColor =   r + ','+  g + ','+ b ;
   // document.getElementById('rightStoveDefault').innerHTML = hexColor;
    var hexColor =  'sc'+ r + ','+  g + ','+ b ;
    websocket.send(hexColor);  
}

function changeAllRightStove() {
    var r = document.getElementById('rightStoveR').innerHTML;
    var g = document.getElementById('rightStoveG').innerHTML;
    var b = document.getElementById('rightStoveB').innerHTML;
    var hexColor =  'c'+ r + ','+  g + ','+ b ;
    document.getElementById('rightStoveBack').style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
    websocket.send(hexColor);
}
  
var sliderLeftStoveR = document.getElementById("leftStoveRangeR");
var outputLeftStoveR = document.getElementById("leftStoveR");
var divbackLeftStove = document.getElementById("leftStoveBack");
outputLeftStoveR.innerHTML = sliderLeftStoveR.value;
sliderLeftStoveR.oninput = function() {
  outputLeftStoveR.innerHTML = this.value;
  changeAllLeftStove();
}

  
var sliderLeftStoveG = document.getElementById("leftStoveRangeG");
var outputLeftStoveG = document.getElementById("leftStoveG");
var divbackLeftStove = document.getElementById("leftStoveBack");
outputLeftStoveG.innerHTML = sliderLeftStoveG.value;
sliderLeftStoveG.oninput = function() {
  outputLeftStoveG.innerHTML = this.value;
  changeAllLeftStove();
}
  
var sliderLeftStoveB = document.getElementById("leftStoveRangeB");
var outputLeftStoveB = document.getElementById("leftStoveB");
var divbackLeftStove = document.getElementById("leftStoveBack");
outputLeftStoveB.innerHTML = sliderLeftStoveB.value;
sliderLeftStoveB.oninput = function() {
  outputLeftStoveB.innerHTML = this.value;
  changeAllLeftStove();
}

function saveLeftStoveDefault() {
    var r = document.getElementById('leftStoveR').innerHTML;
    var g = document.getElementById('leftStoveG').innerHTML;
    var b = document.getElementById('leftStoveB').innerHTML;
    var hexColor =   r + ','+  g + ','+ b ;
    //document.getElementById('leftStoveDefault').innerHTML = hexColor;
    var hexColor =  'sd'+ r + ','+  g + ','+ b ;
    websocket.send(hexColor);  
}

function changeAllLeftStove() {
    var r = document.getElementById('leftStoveR').innerHTML;
    var g = document.getElementById('leftStoveG').innerHTML;
    var b = document.getElementById('leftStoveB').innerHTML;
    var hexColor =  'd'+ r + ','+  g + ','+ b ;
    document.getElementById('leftStoveBack').style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
    websocket.send(hexColor);
}

var sliderRefrigR = document.getElementById("refrigRangeR");
var outputRefrigR = document.getElementById("refrigR");
var divbackRefrig = document.getElementById("refrigBack");
outputRefrigR.innerHTML = sliderRefrigR.value;
sliderRefrigR.oninput = function() {
  outputRefrigR.innerHTML = this.value;
  changeAllRefrig();
}

  
var sliderRefrigG = document.getElementById("refrigRangeG");
var outputRefrigG = document.getElementById("refrigG");
var divbackRefrig = document.getElementById("refrigBack");
outputRefrigG.innerHTML = sliderRefrigG.value;
sliderRefrigG.oninput = function() {
  outputRefrigG.innerHTML = this.value;
  changeAllRefrig();
}
  
var sliderRefrigB = document.getElementById("refrigRangeB");
var outputRefrigB = document.getElementById("refrigB");
var divbackRefrig = document.getElementById("refrigBack");
outputRefrigB.innerHTML = sliderRefrigB.value;
sliderRefrigB.oninput = function() {
  outputRefrigB.innerHTML = this.value;
  changeAllRefrig();
}

function saveRefrigDefault() {
    var r = document.getElementById('refrigR').innerHTML;
    var g = document.getElementById('refrigG').innerHTML;
    var b = document.getElementById('refrigB').innerHTML;
    var hexColor =   r + ','+  g + ','+ b ;
    //document.getElementById('refrigDefault').innerHTML = hexColor;
    var hexColor =  'se'+ r + ','+  g + ','+ b ;
    websocket.send(hexColor);  
}

function changeAllRefrig() {
    var r = document.getElementById('refrigR').innerHTML;
    var g = document.getElementById('refrigG').innerHTML;
    var b = document.getElementById('refrigB').innerHTML;
    var hexColor =  'e'+ r + ','+  g + ','+ b ;
    document.getElementById('refrigBack').style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
    websocket.send(hexColor);
}
</script>
<iframe style="display:none" name="hidden-form"></iframe>
</body>
</html>
)rawliteral";

void notifyClients() {
  ws.textAll(String(ledState));
}


void fillRightSinkCabinet( uint8_t r,uint8_t g,uint8_t b)
{
   fill_solid( &leds[RSINKCAB_ST],RSINKCAB_SZ, CRGB(g,r,b));     //right sink cabinet
}

void fillLeftSinkCabinet( uint8_t r,uint8_t g,uint8_t b)
{
   fill_solid( &leds[LSINKCAB_ST],LSINKCAB_SZ, CRGB(g,r,b));     //right sink cabinet
}

void fillRightStoveCabinet( uint8_t r,uint8_t g,uint8_t b)
{
   fill_solid( &leds[RSTOVECAB_ST],RSTOVECAB_SZ, CRGB(g,r,b));     //right sink cabinet
}

void fillLeftStoveCabinet( uint8_t r,uint8_t g,uint8_t b)
{
   fill_solid( &leds[LSTOVECAB_ST],LSTOVECAB_SZ, CRGB(g,r,b));     //right sink cabinet
}

void fillRefrigCabinet( uint8_t r,uint8_t g,uint8_t b)
{
   fill_solid( &leds[REFRIGCAB_ST],REFRIGCAB_SZ, CRGB(g,r,b));     //right sink cabinet
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}








String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}


void initCabinets()
{
  String strValue = readFile(SPIFFS, "/rightSinkDefault.txt");
  char * pEnd;
  uint8_t redVal = (uint8_t) strtol((const char *) &strValue[0], &pEnd, 10);
  uint8_t greenVal = (uint8_t) strtol( pEnd+1, &pEnd, 10);
  uint8_t blueVal = (uint8_t) strtol( pEnd+1, NULL, 10);

  fillRightSinkCabinet( redVal,greenVal,blueVal);
  
  Serial.println(redVal);
  Serial.println(greenVal);
  Serial.println(blueVal);
  A_Str = 'A'+strValue; // use "A" to initRightSink

  // save string to value send later
  //ws.textAll(String(strValue));

 //----------------------------- 

  strValue = readFile(SPIFFS, "/leftSinkDefault.txt");
  redVal = (uint8_t) strtol((const char *) &strValue[0], &pEnd, 10);
  greenVal = (uint8_t) strtol( pEnd+1, &pEnd, 10);
  blueVal = (uint8_t) strtol( pEnd+1, NULL, 10);

  fillLeftSinkCabinet( redVal,greenVal,blueVal);

  
  Serial.println(redVal);
  Serial.println(greenVal);
  Serial.println(blueVal);
  B_Str = 'B'+strValue; // use "B" to initLeftSink
  //ws.textAll(String(strValue));

 //----------------------------- 
  strValue = readFile(SPIFFS, "/rightStoveDefault.txt");
  redVal = (uint8_t) strtol((const char *) &strValue[0], &pEnd, 10);
  greenVal = (uint8_t) strtol( pEnd+1, &pEnd, 10);
  blueVal = (uint8_t) strtol( pEnd+1, NULL, 10);

  fillRightStoveCabinet( redVal,greenVal,blueVal);

  
  Serial.println(redVal);
  Serial.println(greenVal);
  Serial.println(blueVal);
  C_Str = 'C'+strValue; // use "C" to initRightStove
  //ws.textAll(String(strValue));

 //----------------------------- 
  strValue = readFile(SPIFFS, "/leftStoveDefault.txt");
  redVal = (uint8_t) strtol((const char *) &strValue[0], &pEnd, 10);
  greenVal = (uint8_t) strtol( pEnd+1, &pEnd, 10);
  blueVal = (uint8_t) strtol( pEnd+1, NULL, 10);

  fillLeftStoveCabinet( redVal,greenVal,blueVal);

  
  Serial.println(redVal);
  Serial.println(greenVal);
  Serial.println(blueVal);
  D_Str = 'D'+strValue; // use "D" to initLeftStove
  //ws.textAll(String(strValue));

 //-----------------------------  
  strValue = readFile(SPIFFS, "/refrigDefault.txt");
  redVal = (uint8_t) strtol((const char *) &strValue[0], &pEnd, 10);
  greenVal = (uint8_t) strtol( pEnd+1, &pEnd, 10);
  blueVal = (uint8_t) strtol( pEnd+1, NULL, 10);

  fillRefrigCabinet( redVal,greenVal,blueVal);
  FastLED.show(); 
  
  Serial.println(redVal);
  Serial.println(greenVal);
  Serial.println(blueVal);
  E_Str = 'E'+strValue; // use "E" to initRefrig
 // ws.textAll(String(strValue));
  
}

void sendStrings(){
  ws.textAll(A_Str);
  ws.textAll(B_Str);
  ws.textAll(C_Str);
  ws.textAll(D_Str);
  ws.textAll(E_Str);
  
  }

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "init") == 0) {
      sendStrings();
    }
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients();
    }
    if(data[0] == 'a'){  // check for right sink
      char * pEnd;
      uint8_t redVal = (uint8_t) strtol((const char *) &data[1], &pEnd, 10);
      uint8_t greenVal = (uint8_t) strtol( pEnd+1, &pEnd, 10);
      uint8_t blueVal = (uint8_t) strtol( pEnd+1, NULL, 10);
      fillRightSinkCabinet(redVal,greenVal,blueVal);
      FastLED.show(); 
    }
    if(data[0] == 'b'){  // check for left sink
      char * pEnd;
      uint8_t redVal = (uint8_t) strtol((const char *) &data[1], &pEnd, 10);
      uint8_t greenVal = (uint8_t) strtol( pEnd+1, &pEnd, 10);
      uint8_t blueVal = (uint8_t) strtol( pEnd+1, NULL, 10);
      fillLeftSinkCabinet(redVal,greenVal ,blueVal);
      FastLED.show(); 
    }
    if(data[0] == 'c'){  // check for right stove
      char * pEnd;
      uint8_t redVal = (uint8_t) strtol((const char *) &data[1], &pEnd, 10);
      uint8_t greenVal = (uint8_t) strtol( pEnd+1, &pEnd, 10);
      uint8_t blueVal = (uint8_t) strtol( pEnd+1, NULL, 10);
#ifdef DEBUG      
      Serial.println(redVal);
      Serial.println(greenVal);
      Serial.println(blueVal);
#endif
      fillRightStoveCabinet(redVal,greenVal ,blueVal);
      FastLED.show(); 
    }
    if(data[0] == 'd'){  // check for left stove
      char * pEnd;
      uint8_t redVal = (uint8_t) strtol((const char *) &data[1], &pEnd, 10);
      uint8_t greenVal = (uint8_t) strtol( pEnd+1, &pEnd, 10);
      uint8_t blueVal = (uint8_t) strtol( pEnd+1, NULL, 10);
#ifdef DEBUG      
      Serial.println(redVal);
      Serial.println(greenVal);
      Serial.println(blueVal);
#endif
      fillLeftStoveCabinet(redVal,greenVal ,blueVal);
      FastLED.show(); 
    }    
    if(data[0] == 'e'){  // check for left stove
      char * pEnd;
      uint8_t redVal = (uint8_t) strtol((const char *) &data[1], &pEnd, 10);
      uint8_t greenVal = (uint8_t) strtol( pEnd+1, &pEnd, 10);
      uint8_t blueVal = (uint8_t) strtol( pEnd+1, NULL, 10);
#ifdef DEBUG      
      Serial.println(redVal);
      Serial.println(greenVal);
      Serial.println(blueVal);
#endif
      fillRefrigCabinet(redVal,greenVal ,blueVal);
      FastLED.show(); 
    }
    if(data[0] == 's' && data[1] == 'a'){  // save rightSink value
      uint8_t * triplet = data + 2;
      writeFile(SPIFFS, "/rightSinkDefault.txt", (char *)triplet );
    }    
    if(data[0] == 's' && data[1] == 'b'){  // save refrig value
      uint8_t * triplet = data + 2;
      writeFile(SPIFFS, "/leftSinkDefault.txt", (char *)triplet );
    }    
    if(data[0] == 's' && data[1] == 'c'){  // save refrig value
      uint8_t * triplet = data + 2;
      writeFile(SPIFFS, "/rightStoveDefault.txt", (char *)triplet );
    }    
    if(data[0] == 's' && data[1] == 'd'){  // save refrig value
      uint8_t * triplet = data + 2;
      writeFile(SPIFFS, "/leftStoveDefault.txt", (char *)triplet );
    }
    if(data[0] == 's' && data[1] == 'e'){  // save refrig value
      uint8_t * triplet = data + 2;
      writeFile(SPIFFS, "/refrigDefault.txt", (char *)triplet );
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  if(var == "refrigDefault"){
    return readFile(SPIFFS, "/refrigDefault.txt");
  }
}

void setup(){
 // setCpuFrequencyMhz(40);
    // Serial port for debugging purposes
  Serial.begin(115200);

  if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
  }
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);

  FastLED.setBrightness(BRIGHTNESS);


  
  initCabinets();
  FastLED.show(); 
  delay(1000);

  /*
  String datastr = "225,0,0";
  char * triplet = &datastr[0];
  writeFile(SPIFFS, "/rightSinkDefault.txt", triplet );
  datastr="225,0,255";
  writeFile(SPIFFS, "/leftSinkDefault.txt", triplet );
  datastr="0,0,255";
  writeFile(SPIFFS, "/rightStoveDefault.txt", triplet );
  datastr="225,30,30";
  writeFile(SPIFFS, "/leftStoveDefault.txt", triplet );
  datastr="60,125,255";
  writeFile(SPIFFS, "/refrigDefault.txt", triplet );
  */
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

//  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
//  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
//    String inputMessage;
//    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
//    if (request->hasParam(PARAM_REFRIG)) {
//      inputMessage = request->getParam(PARAM_REFRIG)->value();
//      writeFile(SPIFFS, "/refrigDefault.txt", inputMessage.c_str());
//    }
//    else {
//      inputMessage = "No message sent";
//    }
//    Serial.println(inputMessage);
//    request->send(200, "text/text", inputMessage);
//  });

  
  server.onNotFound(notFound);

  // Start server
  server.begin();
  
 
  
}

void loop() {
  
  
  ws.cleanupClients();
 // digitalWrite(ledPin, ledState);
//ws.textAll("A255,0,0");
//delay(3000);
//ws.textAll("A0,255,0");
//delay(3000);
  // To access your stored values on inputString, inputInt, inputFloat
 // String yourInputString = readFile(SPIFFS, "/refrigDefault.txt");
  //Serial.print("*** Your refrigDefault: ");
 // Serial.println(yourInputString);

}
