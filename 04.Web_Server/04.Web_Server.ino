#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>

const char *ssid = "dxwy";
const char *password = "dxwydxwy";

ESP8266WiFiMulti wifiMulti;                 // 建立ESP8266WiFiMulti对象
ESP8266WebServer esp8266_server(80);        // 建立ESP8266WebServer对象，网络服务器标准http端口号为80

void setup(void){
  Serial.begin(9600);          // 启动串口通讯
 
  //通过addAp函数存储  WiFi名称       WiFi密码
  wifiMulti.addAP(ssid, password);          // 这三条语句通过调用函数addAP来记录3个不同的WiFi网络信息。
  wifiMulti.addAP("taichi-maker2", "87654321");
  wifiMulti.addAP("taichi-maker3", "13572468");
  
  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
    Serial.print(i++); Serial.print(' ');    // 将会连接信号最强的那一个WiFi信号。
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                             // 此处while循环判断是否跳出循环的条件。
 
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');                     // WiFi连接成功后
  Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址
  

  esp8266_server.begin();
  esp8266_server.on("/", handleRoot);
  esp8266_server.onNotFound(handleNotFound);        
  Serial.println("HTTP esp8266_server started");//  告知用户ESP8266网络服务功能已经启动
}
void loop(void){
  esp8266_server.handleClient();     // 处理http服务器访问
}

void handleRoot() {   //处理网站根目录“/”的访问请求 
  esp8266_server.send(200, "text/plain", "Hello from ESP8266");   // NodeMCU将调用此函数。
}

// 设置处理404情况的函数'handleNotFound'
void handleNotFound(){                                        // 当浏览器请求的网络资源无法在服务器找到时，
  esp8266_server.send(404, "text/plain", "404: Not found");   // NodeMCU将调用此函数。
}
