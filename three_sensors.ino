#include <ESP8266WiFi.h>
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker
#include <FirebaseArduino.h>

// _________________________defining constant
#define FIREBASE_HOST "nodemcu-de561.firebaseio.com"
#define FIREBASE_AUTH "kHRY8t2cmkH0C4lChOj6ro2CfCVATgvoRU0owcVX"
#define WIFI_SSID "Arch_angel"
#define WIFI_PASSWORD "archan1997"
// __________________________variable definied
/******************************************************************************/
const char* ssid     = "Arch_angel"; // Your WiFi ssid
const char* password = "archan1997"; // Your Wifi password;
/********************************************************************************/
// MQTT
// Make sure to update this for your own MQTT Broker!
const char* mqtt_server = "broker.hivemq.com";

// The client id identifies the ESP8266 device. Think of it a bit like a hostname (Or just a name, like Greg).
const char* clientID = "client-1";

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker


//MQTT TOPICS
const char* temperature="archan/temperature";
const char* gas="archan/gas";
const char* flame="archan/flame";
const char* status1="archan/status";
const char* Status1="No Forest fire";
const char* SmokeStatus="No Smoke";
const char* prob="archan/prob";

float Prob=0.0;
float t=0.0;
int sensorValue;
int Flame= 0;
int flamePin=4;
int temp=0;

float t_severe=37.5;
float t_moderate=33.5;
float t_mild=31.0;
float t_curr=0.0;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
  Serial.setTimeout(2000);
  //Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("flame", 0);
  Firebase.set("temperature", 0.0);
  Firebase.set("gas",0);
}
void loop() {
  if (client.connect(clientID)) {
    Serial.println("Connected to MQTT Broker!");
    
    client.publish(temperature,String(t).c_str());
    client.publish(gas,String(SmokeStatus).c_str());
    client.publish(flame,String(Flame).c_str());
    client.publish(status1,String(Status1).c_str());
    client.publish(prob,String(Prob).c_str());
  }
  else 
  {
    Serial.println("Connection to MQTT Broker failed...");
  }
  ReadTemp();
  gasSensor();
  flameSensor();
  prediction();
}
void Buzzerfn()
{
  pinMode(D5,OUTPUT);
  digitalWrite(D5, HIGH);
  delay(200);
  digitalWrite(D5, LOW);
  delay(200);
}
void prediction()
{
  if(Flame==0 || t>t_severe || (t>t_moderate && sensorValue==1))
  {
    Status1="Severe";
    Buzzerfn();
  }
  else if(t>=t_moderate || (t>t_mild && sensorValue==1))
  {
    Status1="Moderate";
    Buzzerfn();
  }
  else if(t>=t_mild || sensorValue==1)
  {
    Status1="Mild";
    Buzzerfn();
  }
  else
  {
    Status1="No Forest fire";
    pinMode(D5,INPUT);
  }
  
 float conf=classify(t,sensorValue,Flame);
 Serial.print("chance of forest fire in %=");
 Prob=conf*100;
 Serial.println(Prob);
 }
 
float trimf(float x,float a, float b, float c){
  float f;
  if(x<=a)
     f=0;
  else if((a<=x)&&(x<=b)) 
     f=(x-a)/(b-a);
  else if((b<=x)&&(x<=c)) 
     f=(c-x)/(c-b);
  else if(c<=x) 
     f=0;
  return f; 
}
float trimf1(float x,float a, float b, float c){
  float f;
  if(x>=b || x>=c) return 0;
  else if(x>a) return (x-a)/(b-a);

  if(x==a) return 0.001;
  
}
// Function for predicting atmospheric comfort from temperature and relative humidity
float classify(float T,float smoke,float flame ){
  float C1 = 1; //severe 
  float C2 = 0.66;//moderate 
  float C3 = 0.33; //mild
  
  if(T>t_severe) T=t_severe; // if temperature is above 35°C, set temperature to maksimum in our range
  if(flame==0) flame=1;
  else flame=0;
  
  // Fuzzy rules
  float w1 = trimf(T,t_moderate,t_severe,t_severe);//severe
  float w2 = trimf(T,t_mild,t_moderate,t_severe);//moderate
  float w3 = trimf(T,t_mild,t_mild,t_moderate);//mild
  float w4 = trimf1(T,t_curr,t_mild,t_moderate);//mild
 
    // Defuzzyfication
  float z = (w1*C1 + w2*C2 + w3*C3+ w4*C3*w4)/(w1+w2+w3+w4);
  if(Status1== "Severe" && z<=0.80)
  {
    z=0.99;
    return z;
  }
  else if(Status1== "Moderate" && z<=0.66)
  {
    z=0.6789;
    return z;
  }
  
  else if(Status1== "Mild" && z<0.33)
  {
    z=0.3769;
    return z;
  }
  return z;
}
void ReadTemp()
{
  if(temp==0)
  {
    t_curr=((analogRead(A0)*330.0)/1023.0);
    t_mild=t_curr+ 2.0 ;
    t_moderate=t_mild+3.0;
    t_severe=t_moderate+2.0;
    temp=1;
  }
  t = (analogRead(A0) * 330.0) / 1023.0;
  Serial.print("Temperature:");
  Serial.println(t);
  Firebase.set("temperature", t);
  
}
void gasSensor()
{
  sensorValue = digitalRead(D0); // read analog input pin 0
  
  Serial.print("Smoke Status: ");
  Serial.println(SmokeStatus);
  Firebase.set("gas", sensorValue);
  if(sensorValue==1)
  {
    SmokeStatus="Critical Smoke Detected";
  }
  else if(sensorValue==0)
  {
    SmokeStatus="No Smoke";
  }
}
void flameSensor()
{
  Flame = digitalRead(flamePin);
  Firebase.set("flame",Flame);
  Serial.print("Flame Sensor Status: ");
  if (Flame== 0)
  {
    Serial.println("Fire!!!");
  }
  else if(Flame==1)
  {
    Serial.println("No Fire");
  }
}
