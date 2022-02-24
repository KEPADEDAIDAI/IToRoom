#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <MFRC522.h>
#include <Servo.h>

/*
门口部分
*/
const char* ssid = "****";//WiFi账号 
const char* password = "********";//WiFi密码 
const char* mqtt_server = "tcp://broker.emqx.io:1883";

Servo myServo;  
int pos=0;    
int num=0;
int sign1=0;
int sign2=0;
bool subscribe1=false;
bool subscribe2=false;
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
#define SCREEN_WIDTH 128 // OLED显示宽度（像素）
#define SCREEN_HEIGHT 32 // OLED显示高度（像素）

#define OLED_MOSI   2  //DI   ------ D4
#define OLED_CLK    0  //D0  ------- D3
#define OLED_DC    16  //DC  ------- D0
#define OLED_CS    9  //CS  ------- D6
#define OLED_RESET 15  //RES -------D8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
int cnt = 0;
int led = 10;
#define RST_PIN         5           // 配置Adafruit针脚
#define SS_PIN          4
MFRC522 mfrc522(SS_PIN, RST_PIN);   // 创建新的RFID实例
MFRC522::MIFARE_Key key;
int x[10]={0,28,56,84,112,0,28,56,84,112};
int y[10]={17,17,17,17,17,25,25,25,25,25};
bool z[10];
void setup() {
  Serial.begin(9600);
  myServo.attach(1); //tx   
  SPI.begin();        // SPI开始
  mfrc522.PCD_Init(); // 创建 MFRC522 对象
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // 不要继续，一直循环
  }
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  display.clearDisplay();  // 清空缓冲区
  display.display();
  conect_wifi();
}
void open(){
  myServo.write(90); 
  delay(5000);
  myServo.write(0); 
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if((char)payload[0] =='d'){//座位情况
       for (int i = 1; i < 11; i++) {
          if((char)payload[i] =='0'){
               z[i-1]=false;
          }else{
               z[i-1]=true;
          }
       }  
       draw();
  }
  if((char)payload[0] =='o'){
       open();
  }
}

void reconnect() {
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
      client.subscribe("fjhyjk123");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
     reconnect();
  }
  client.loop();
  RFID();
}
void RFID(){
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent()) //判断是不是新卡片
        return;
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())// 判断是否选择成功
        return;
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));
    byte blockAddr      = 57;
    byte trailerBlock   = 59; //密匙
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);
    //使用密匙解密
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    // Read data from the block 读取数据
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);//读取存储的代码
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();
    num=check(buffer, 16);
    if(num!=0)
    {
      if(num==1){
        if(sign1%2!=0){
           welcome();
            snprintf (msg, MSG_BUFFER_SIZE, "o%d%d%d",0,0,0);
            Serial.print("Publish message: ");
            Serial.println(msg);
            client.publish("qwerwlw", msg);
           cnt++; 
        }else{
           bye();
        }
        sign1++;
      }else{
        if(sign2%2!=0){
          welcome();
          snprintf (msg, MSG_BUFFER_SIZE, "o%d%d%d",0,0,1);
          Serial.print("Publish message: ");
          Serial.println(msg);
          client.publish("qwerwlw", msg);
           cnt++; 
        }else{
           bye();
        }
        sign2++;
      }
    }
    else{
      Serial.println("false");
      fail();
    }
    draw();
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
}
void welcome()
{
    snprintf (msg, MSG_BUFFER_SIZE, "o%d%d%d",0,0,0);
    Serial.print("Publish message: ");
    Serial.println(msg);
   client.publish("qwerwlw", msg);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Welcome!!!");
    display.display();
    delay(3000);
}
void fail(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Permission denied");
  display.display();
  light();
}
void light()
{
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
}
void conect_wifi(){
  WiFi.begin(ssid, password);
  delay(2000);
  int i = 0;                                   // 这一段程序语句用于检查WiFi是否连接成功
  while (WiFi.status() != WL_CONNECTED) {      // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。 
    display.clearDisplay();
    display.setTextSize(1);             // 正常1:1像素比例
    display.setTextColor(WHITE);        // 绘制白色文本
    display.setCursor(0, 0);
    display.print("Connecting to ");
    display.println(ssid);
    i++;
    if(i%3==1) display.print(".");
    else if(i%3==2) display.print("..");
    else display.print("...");
    display.display();
    delay(1000);
  }
  display.clearDisplay();
  display.setTextSize(1);             // 正常1:1像素比例
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("connect success");
  display.print("ip address: ");
  display.print(WiFi.localIP());
  display.display();
  delay(3000);
}
void Printcnt(){
  display.clearDisplay();             //清空屏幕
  display.setTextSize(1);             // 正常1:1像素比例
  display.setTextColor(WHITE);        // 绘制白色文本
  display.setCursor(0, 15);         // 设置位置
  display.print("doday's num : ");
  display.print(cnt);
  display.display(); // 显示
}
void draw(){
    display.clearDisplay();
    display.setTextSize(1);             // 正常1:1像素比例
    display.setTextColor(WHITE);        // 绘制白色文本
    display.setCursor(0,5);         // 设置位置
    display.print("today's num: ");
    display.print(cnt);
    for(int i=0;i<10;i++){
      if(z[i]){
        display.fillRect(x[i],y[i],10,5,SSD1306_WHITE);
      }else{
        display.drawRect(x[i],y[i],10,5,SSD1306_WHITE);
      }
    }
    display.display();
    delay(20);
}
void bye(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("goodbye!!!");
    // display.println("Learning:3h");
    // display.println("Number  :22");
    // display.println("pay     :15");
    display.display();
    open();
}
int check(byte *buffer, byte bufferSize)
{
  if(bufferSize < 2) return 0;
  if(buffer[0]!=0xaa || buffer[1]!=0xbb || buffer[2]!= 0xcc) return 1;
  if(buffer[0]!=0x00 || buffer[1]!=0x00 || buffer[2]!= 0x00) return 2;
  return 0;
}
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
