#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//监测座位上是否有人
const char* ssid= "******";//WiFi账号 
const char* password = "********"; //WiFi密码 
const char* mqtt_server = "tcp://broker.emqx.io:1883";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int s1=0;
int s2=0;
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3

// defines variables
float duration;
float distance;
const int trigPin1 = 4;  //D2
const int echoPin1 = 5;  //D1
float tp1;
float tp2;

float duration1;
float distance1;
void setup_wifi() {  //连接WiFi

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void reconnect() {//连接MQTT服务器
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
   digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  Serial.print("echoPin: ");
  Serial.print(echoPin);
  distance= duration*0.034/2.0;
  Serial.print(" ,Distance: ");
  Serial.println(distance);
  if(tp1-distance>5){
     s1=1;
  }else{
     s1=0;
  }
  tp1=distance;
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  Serial.print("echoPin1: ");
  Serial.print(echoPin1);
  distance1= duration1*0.034/2.0;
  Serial.print(" ,Distance: ");
  Serial.println(distance1);
  f(tp2-distance1>5){
     s2=1;
  }else{
     s2=0;
  }
  tp2=distance1;
  snprintf (msg, MSG_BUFFER_SIZE, "d%d%d%d%d%d%d%d%d%d%d",s1, s2,1,1,1,1,1,1,1,1);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("qwerwlw", msg);
  delay(3000);
}
