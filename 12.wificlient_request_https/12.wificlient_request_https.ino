#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

//Include the SSL client
#include <WiFiClientSecure.h>

char ssid[] = "dxwy";         // your network SSID (name)
char password[] = "dxwydxwy"; // your network key

//Add a SSL client
WiFiClientSecure client;

String subredditName = "arduino";

long checkRedditDueTime;
int checkRedditDelay = 60000; // 60 x 1000 (1 minute)

void setup()
{

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

String getTitleOfNewestPost(String sub)
{
  String title = "";
  String headers = "";
  String body = "";
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
  bool gotResponse = false;
  long now;
  char host[] = "api.reddit.com";

  if (client.connect(host, 443))
  {
    String URL = "/r/" + sub + "/new?limit=1";
    Serial.println("connected");
    Serial.println(URL);
    client.println("GET " + URL + " HTTP/1.1");
    client.print("Host: ");
    client.println(host);
    client.println("User-Agent: arduino/1.0");
    client.println("");

    now = millis();
    // checking the timeout
    while (millis() - now < 1500)
    {
      while (client.available())
      {
        char c = client.read();
        //Serial.print(c);

        if (finishedHeaders)
        {
          body = body + c;
        }
        else
        {
          if (currentLineIsBlank && c == '\n')
          {
            finishedHeaders = true;
          }
          else
          {
            headers = headers + c;
          }
        }

        if (c == '\n')
        {
          currentLineIsBlank = true;
        }
        else if (c != '\r')
        {
          currentLineIsBlank = false;
        }

        //marking we got a response
        gotResponse = true;
      }
      if (gotResponse)
      {
        Serial.println(body);
        Serial.println();
        // DynamicJsonBuffer jsonBuffer;
        // JsonObject &root = jsonBuffer.parseObject(body);
        // if (root.success())
        // {
        //   if (root.containsKey("data"))
        //   {
        //     JsonObject &post = root["data"]["children"][0];
        //     if (post.containsKey("data"))
        //     {
        //       title = post["data"]["title"].as<String>();
        //     }
        //   }
        // }
        // else
        // {
        //   Serial.println("failed to parse JSON");
        // }
        String info_name_str;
        StaticJsonDocument<200> doc; //声明一个JsonDocument对象
        // DynamicJsonDocument doc(200); //声明一个JsonDocument对象
        DeserializationError error = deserializeJson(doc, body); //反序列化JSON数据
        // if (error.c_str() == "OK") //检查反序列化是否成功
        if (!error) //检查反序列化是否成功
        {
          //读取json节点
          const char *myC = doc["title"]; //等同于const char *myC = doc["myChar"].as<char*>();
          title = doc["kind"].as<String>();
          // JsonObject info = doc["info"];
          Serial.println("输出结点");
          Serial.println(myC);

          // Serial.println(myF, n); //n指定输出小数位数
        }

        break;
      }
    }
  }

  //Serial.println(body);

  return title;
}

void loop()
{
  long now = millis();
  if (now >= checkRedditDueTime)
  {
    Serial.println("---------");
    String title = getTitleOfNewestPost(subredditName);
    if (title != "")
    {
      Serial.println("Most recent post on /r/" + subredditName);
      Serial.println(title);
    }
    else
    {
      Serial.println("Error getting title.");
    }
    Serial.println("---------");
    checkRedditDueTime = now + checkRedditDelay;
  }
}
