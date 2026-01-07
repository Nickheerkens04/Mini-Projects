#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#define NEOPIXEL_PIN 5 //safer pin choice
#define NUMPIXELS 1

Adafruit_NeoPixel pixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

DNSServer dnsServer; //activates dns sever for webpage redirect
AsyncWebServer server(80); //activates webserver to host webpage
const char* ssid = "Free WiFi"; //configures our ap to be a global variable

void showColor(uint8_t r, uint8_t g, uint8_t b) {
  pixel.clear();
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
}

void showSuccess() { showColor(0, 255, 0); } //green 
void showFailure() { showColor(255, 0, 0); } //red color 
void showWorking() { showColor(255, 100, 0); } //orange 

//our Html code for our server
const char HTML_CONTENT[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta content="text/html; charset=ISO-8859-1"
http-equiv="content-type">
<meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
<title>ESP32 Demo</title>
<style>
body { background-color: #0067B3  ; font-family: Arial, Helvetica, Sans-Serif; Color: #FFFFFF; }
</style>
</head>
<body>
<center>
<h1 style="color:#FFFFFF; font-family:verdana;font-family: verdana;padding-top: 10px;padding-bottom: 10px;font-size: 36px">ESP32 Captive Portal</h1>
<h2 style="color:#FFFFFF;font-family: Verdana;font: caption;font-size: 27px;padding-top: 10px;padding-bottom: 10px;">Give Your WiFi Credentials</h2>
<FORM action="/auth" method="post">
<P ><label style="font-family:Times New Roman">SSID</label><br><input maxlength="30px" type='text' id="ssid_wifi" name="ssid" placeholder='Enter WiFi SSID' style="width: 400px; padding: 5px 10px ; margin: 8px 0; border : 2px solid #3498db; border-radius: 4px; box-sizing:border-box" ><br></P>
<P><label style="font-family:Times New Roman">PASSKEY</label><br><input maxlength="30px" type = "text" id="pass_wifi" name="passkey"  placeholder = "Enter WiFi PASSKEY" style="width: 400px; padding: 5px 10px ; margin: 8px 0; border : 2px solid #3498db; border-radius: 4px; box-sizing:border-box" ><br><P>
<input type="checkbox" name="configure" value="change"> Change IP Settings </P>
<BR>
<INPUT type="submit"><INPUT type="reset">
<style>
input[type="reset"]{background-color: #3498DB; border: none; color: white; padding:  15px 48px; text-align: center; text-decoration: none;display: inline-block; font-size: 16px;}
input[type="submit"]{background-color: #3498DB; border: none; color: white; padding:  15px 48px;text-align: center; text-decoration: none;display: inline-block;font-size: 16px;}
</style>
</FORM>
</center>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pixel.begin();
  showWorking();
  
  delay(200);

  Serial.println("Starting Access Point...");

  bool apStarted = WiFi.softAP(ssid);

  if (apStarted) {
    Serial.println("Access Point Started!");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
    showSuccess();
  } else {
    Serial.println("Failed to start Access Point!");
    showFailure();
  }

  dnsServer.start(53, "*", WiFi.softAPIP()); //catches all for DNS Server

//configuring our routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("/auth");
  });

  server.on("/auth", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", HTML_CONTENT);
  });

  // Captive portal probes
  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("/auth");
  });
  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("/auth");
  });
  server.on("/redirect", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("/auth");
  });

  server.on("/auth", HTTP_POST, [](AsyncWebServerRequest *request){
    String ssid = "";
    String pass = "";
    String configure = "no";

    if (request->hasParam("ssid", true)) {
      ssid = request->getParam("ssid", true)->value();
    }
    
    if (request->hasParam("passkey", true)) {
      pass = request->getParam("passkey", true)->value();
    }

    if (request->hasParam("configure", true)) {
      configure = "yes";
    }

    String response = "<h2>Received WiFi Credentials</h2>";
    response += "SSID: " + ssid + "<br>";
    response += "PASSKEY: " + pass + "<br>";
    response += "Change IP Settings: " + configure + "<br>";

    request->send(200, "text/html", response);
});
    
  server.onNotFound([](AsyncWebServerRequest *request){
    request->redirect("/auth");
  });

//starting the server
  server.begin();
}

bool lastConnected = false;

void loop() {
  dnsServer.processNextRequest();

  bool connected = WiFi.softAPgetStationNum() > 0;
  if (connected != lastConnected) {
    if (connected) showSuccess();
    else showFailure();
    lastConnected = connected;

  }
}
