# esp8266-netprobe

A simple way to automatically monitor internet connection status. 
This application will send your IP address to Google Spreadsheet (with the help of Apps Script). The spreadsheet also contains date of when the last data was written in -> in case of internet connection failure you can calculate precisely the time of outage.

This device is mainly purposed to be used in locations without static IP. In case of unplanned IP address change you will not lose the ability to connect there.

It uses [ipify.org](https://api.ipify.org/) to determine ip address and [HTTPSRedirect](https://github.com/electronicsguy/ESP8266/tree/master/HTTPSRedirect) library.

### Requirements
- NodeMcu devboard / esp8266 / WeMos
- Google Account
- [HTTPSRedirect](https://github.com/electronicsguy/ESP8266/tree/master/HTTPSRedirect) library

### Instructions

1. Change the configuration section of the code (*esp8266-netprobe/esp8266-netprobe.ino*) (wifi settings, choose spreadsheet api key (use random string))
2. Create new spredsheet on your Google Drive and copy its ID (the long alphanumeric string in the URL)
3. Open Script editor in the spreadsheet (Data -> Script editor)
4. Copy the example code (*appsscript.gs*) and paste the spreadsheet id and previously choosen api key
5. Save the script as web app (Publish -> Save as a web app). Select New project version, execute the app as 'Me' and 'Anyone, even anonymous' can access the app
6. Copy&paste the shown web app URL (everything behind '.google.com') to the .ino file 
7. Upload code to esp8266

### TODO
- LED light to show success/failure during sending data to Google
- Ethernet connection
- web interface