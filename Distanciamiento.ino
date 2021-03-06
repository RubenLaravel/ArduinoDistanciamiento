#include <ArduinoJson.h>
#include <ESP8266WiFi.h> 
#include <ESP8266HTTPClient.h>

//ID AS AP 
const char *ssidNode = "KeepDistance";                   
const char *passNode = "KeepDistance";                   

//ID AS STATION
const char *ssidRouter = "LIMPE4699";               
const char *passRouter = "3740CCIR899"; 

const int buzzerPin = 2;

String macNodePost;
String macNode;
String macNodes[] = {"AB:A5:DD","AC:28:C1","37:22:AD","37:22:00","37:22:00","37:22:00","37:22:00","37:22:00","37:22:00","37:22:00"};
long rssiNode;

int elementos = (sizeof(macNodes)/sizeof(macNodes[0]))

String myMac;
String myMacPost;

String foundNode;
int networksFound;

const char* serverAddress = "http://3.81.17.144/api/verificar"; 

boolean arrayIncludeElement(String macNodes[], String macNode) {
  
 for (int b = 0; b < elementos); b++) {
  
    if (macNodePost == macNodes[b]) {
     
          return true;               
         }       
       }
         return false;
    } 
 
//void AlarmFunction() {
   
//   digitalWrite(pinBuzzer,HIGH); //FALTA BUSCAR MOMENTO PARA APAGARLO Y QUE DEJE DE ESTAR ENCENDIDO CUANOD YA NO DETECTE
// }
   
void PostDataToServer() {

 if(WiFi.status()== WL_CONNECTED){ 
    
    Serial.print("Requesting URL: "); 
    Serial.println(serverAddress); //Post Data  
    HTTPClient http; 
    http.begin(serverAddress); 
    http.addHeader("Content-Type", "application/json"); 
    StaticJsonDocument<200> doc;
    doc["mac1"] = foundNode;
    doc["mac2"] = myMacPost;
    String requestBody;
    serializeJson(doc, requestBody);  
    int httpResponseCode = http.POST(requestBody);
    
    if(httpResponseCode>0){
       
      String response = http.getString();                         
      Serial.println(httpResponseCode);   
      Serial.println(response);
     
    }
    
    else {
     
      Serial.printf("Error occurred while sending HTTP POST");
      Serial.print(httpResponseCode);}
    
    http.end();
    
       }
 
   else {
    
    Serial.print("Error Wifi connecting");
        }
   }
 
void ScanResult() {  
  
   digitalWrite(buzzerPin,HIGH); 
   
   for (int i = 0; i < networksFound; i++) {
       //SI myMACPost ESTA DENTRO DEL ARRAY GENERAL ENTONCES SIGUES ELSE SIGUIENTE VALOR DE I ALMACENADO
       macNode = WiFi.BSSIDstr(i);
       macNodePost = macNode.substring(9);
       rssiNode = WiFi.RSSI(i);
       myMac = WiFi.macAddress();
       myMacPost = myMac.substring(9);
                                              // PODRIA DESDE AQUI FILTRARSE POR VALOR DE RSSI PARA QUE SEAN MENOS LOS EVALUADOS
       if (arrayIncludeElement(macNodes, macNode) && rssiNode > -48){    //podria fluctuar -46 a -49
                 foundNode = macNodePost; 
                 Serial.print("  Mant????n la distancia! \n");
                 //AlarmFunction(); 
                 digitalWrite(buzzerPin,LOW);          
                 PostDataToServer();
                 delay(500);
                 break; 
               }                
            }
         }

void setup() {
  
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssidNode,passNode);
  delay(500);
  WiFi.begin(ssidRouter,passRouter);
  while (WiFi.status() != WL_CONNECTED) 
    { 
      delay(500); 
      Serial.print("."); 
    } 
  //WiFi.setAutoReconnect(true); (Podria incluirse para reestablecer conexi??n) 
  Serial.println("WiFi connected");
  pinMode(buzzerPin,OUTPUT);
  digitalWrite(buzzerPin,HIGH);
  
}

void loop() {

  networksFound = WiFi.scanNetworks();
  if (networksFound > 0) {
     ScanResult();
   }
}
