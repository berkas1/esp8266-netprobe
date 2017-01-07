#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "HTTPSRedirect.h"
// HTTPSRedirect can be obtained from
// https://github.com/electronicsguy/ESP8266/tree/master/HTTPSRedirect


////////////////////////////////////////////
// CONFIGURATION
////////////////////////////////////////////

// Wi-Fi settings
const char* ssid = "";
const char* password = "";

// pause between sending data (defualt 10 minutes)
const int _mydelay = 600;

// URL of Apps script web app -> use everything after .google.com
// example : "/macros/s/AKfycdfghjdjdsjgjghjghdjSUgZaei_QIshfdU_Fhkgha-bvzsdfg1/exec"
const char* googleURL = "";

// your api key - simple protection for your Apps scrip -> choose random string
const char* googleApiKey = "";

// probe ID - every request is identified by ID, so it can be managed automatically 
const char* probeid = "netprobe";

// LED indication
// flashes green or red light when connection was successful or unsuccessful
const bool ledEnabled = false;
const int ledGreen = 4; // nodemcu D2
const int ledRed = 5; // nodemcu D1

////////////////////////////////////////////
// END OF CONFIGURATION
// please DO NOT change code bellow
////////////////////////////////////////////


const char* googleHost = "script.google.com";
String ipAddress = "";

void getIpAddress() {
    const char* ipifyFingerprint = "8F 6E 82 AD B9 CD 51 85 E6 D3 7B CD 5D 1F 57 82 BE B5 2C 66";
    const char* ipifyAddress = "api.ipify.org";

    WiFiClientSecure client;
    Serial.print("Connecting to: ");
    Serial.println(ipifyAddress);
    if (!client.connect(ipifyAddress, 443)) {
      Serial.println("Error: connection failed.");
      ipAddress = "ipify unavailable";
      return;
    }

    // verify certificate fingerprint
    if (client.verify(ipifyFingerprint, ipifyAddress)) {
      Serial.println("SSL verify: OK");
    } else {
      Serial.println("SSL verify: FAIL");
    }

    Serial.print("Sending HTTP GET request...");
    client.print(
                String("GET ") + "/" + " HTTP/1.1\r\n" +
                 "Host: " + ipifyAddress + "\r\n" +
                 "User-Agent: esp8266-netprobe\r\n" +
                 "Connection: close\r\n\r\n"
             );
    Serial.println("Request sent");

    // reading headers
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }

    // reading IP address
    ipAddress = client.readStringUntil('\n');
}

int makeRequestGoogle() {
    int status = 0;
    // fingerprint of script.google.com
    const char* fingerprint = "0D 9A 55 12 4E A0 73 BE DD 1C 02 36 B5 D1 BA 91 66 A6 42 39";
    const char* googleRedirHost = "script.googleusercontent.com";

    HTTPSRedirect client(443);
    Serial.print("Connecting to: ");
    Serial.println(googleHost);

    bool flag = false;
    for (int i=0; i<5; i++) {
        int retval = client.connect(googleHost, 443);
        if (retval == 1) {
            flag = true;
            break;
        } else {
            Serial.println("Connection failed. Retrying...");
        }
    }

    Serial.flush();
    if (!flag) {
       Serial.print("Could not connect to server: ");
       Serial.println(googleHost);
       Serial.println("Exiting...");
       status = 1;
       return status;
    }

    if (client.verify(fingerprint, googleHost)) {
        Serial.println("SSL verify: OK");
    } else {
        Serial.println("SSL verify: FAIL");
        status = 2;
    }

    if (client.printRedir(googleURL + String("?apiKey=") + googleApiKey +
                "&ipaddress=" + ipAddress + "&probeid=" + probeid, googleHost, googleRedirHost)) {
        if (status != 2) {
            status = 0;
        }
    } else {
        status = 3;
    }

    // control LED indication
    if (!ledEnabled) {
        return status;
    }

    // LED blinking control
    switch (status) {
        case 0:
            digitalWrite(ledGreen, HIGH);
            delay(1500);
            digitalWrite(ledGreen, LOW);
        case 1:
            digitalWrite(ledRed, HIGH);
            delay(1500);
            digitalWrite(ledRed, LOW);
        case 2:
            digitalWrite(ledRed, HIGH);
            delay(500);
            digitalWrite(ledRed, LOW);
            digitalWrite(ledGreen, HIGH);
            delay(500);
            digitalWrite(ledGreen, LOW);
            digitalWrite(ledRed, HIGH);
            delay(500);
            digitalWrite(ledRed, LOW);
    }

    return status;

    // with help of HTTPSRedirect
    // https://github.com/electronicsguy/ESP8266/HTTPSRedirect/
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (ledEnabled) {
      pinMode(ledGreen, OUTPUT);
      pinMode(ledRed, OUTPUT);
      digitalWrite(ledGreen, LOW);
      digitalWrite(ledRed, LOW);
      Serial.println("LED light indication enabled");
  }
}

void loop() {
    getIpAddress();
    Serial.println("Your IP: " + ipAddress);
    Serial.println("Request status: " + makeRequestGoogle());
    delay(_mydelay * 1000);
}
