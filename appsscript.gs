///////////////////////////
// CONFIGURATION
var ss = SpreadsheetApp.openById("");
var apiKey = "";

// END OF CONFIGURATION
///////////////////////////

function doGet(e) {
  if (!e.parameter.apiKey) {
    return ContentService.createTextOutput("No API key"); 
  }
  
  if (e.parameter.apiKey.toString() != apiKey.toString()) {
    return ContentService.createTextOutput("wrong API key!").setMimeType(ContentService.MimeType.TEXT);
  }

  handleRequest(e);
  
  return ContentService.createTextOutput("OK" + typeof(e.parameter.apiKey)).setMimeType(ContentService.MimeType.TEXT);
}


function handleRequest(e) {
 var cell = ss.getActiveSheet();
 var d = new Date();

 cell.getRange("B2").setValue(e.parameter.ipaddress);
 cell.getRange("B3").setValue(d.toLocaleTimeString());
}
