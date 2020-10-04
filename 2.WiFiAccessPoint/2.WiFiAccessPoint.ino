#include <ESP8266WiFi.h>

const char *ssid = "dxwy";
const char *password = "dxwydxwy";

void setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  // 不设置密码可以不写第二个参数
  WiFi.softAP(ssid, password);

  Serial.print("Access Point: ");
  Serial.println(ssid);              // 打印WIFI名称
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());   // 打印NodeMCU的IP地址
}

void loop()
{
}
