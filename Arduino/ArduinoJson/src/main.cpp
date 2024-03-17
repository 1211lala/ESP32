/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-09-15 10:12:39
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-15 18:28:29
 * @FilePath: \ESP32_JSON\src\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <Arduino.h>
#include "ArduinoJson.h"

/***
 *  https://oktools.net/json  json格式化网站
 *  https://arduinojson.org/v6/assistant/#/step1  计算json arduino官网
 *
 */
void setup()
{
  Serial.begin(115200);

  Serial.printf("\r\n****************************************************\r\n");
  /**

    {
        "name":"liuao",
        "year":23
    }
  */
  String json1 = "{\"name\":\"liuao\",\"year\":23}";     /* 建立json变量 */
  const size_t capacity1 = 1 * JSON_OBJECT_SIZE(2) + 30; /* 计算含有一个对象,对象中有两个名值对的内存 */
  DynamicJsonDocument doc1(capacity1);                   /* 创建doc对象,并赋予capacity大小的内存 */

  deserializeJson(doc1, json1); /* 解析json */

  String name = doc1["name"].as<String>();
  int year = doc1["year"].as<int>();

  Serial.printf("%s\r\n", name);
  Serial.printf("%d\r\n", year);

  Serial.printf("\r\n****************************************************\r\n");
  /*
  [
    {
      "name":"liuao",
      "hight":173
    },
    {
      "year":13
    }
  ]
  */
  String json2 = "[{\"name\":\"liuao\",\"hight\":173},{\"year\":13}]";
  /* 计算含有一个数组,数组中有两个对象的内存，一个对象有两个名值对 一个对象有一个名值对的内存 */
  const size_t capacity2 = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(1) + 60;
  DynamicJsonDocument doc2(capacity2);

  deserializeJson(doc2, json2);

  Serial.printf("%s\r\n", doc2[0]["name"].as<String>().c_str());
  Serial.printf("%d\r\n", doc2[0]["hight"].as<int>());
  Serial.printf("%d\r\n", doc2[1]["year"].as<int>());

  Serial.printf("\r\n****************************************************\r\n");
  /**
    {
       "date":"2023.9.15.9.34.15",
       "wight":70,
       "num":[
               {
                 "num1":{
                           "number":17
                         },
                 "num2":{
                            "number":16
                         }
               }
                {  
                  "num3":{
                            "number":15
                          }
                }
             ]
    }
  */
  String json3 = "{\"date\":\"2023.9.15.9.34.15\",\"wight\":70,\"num\":[{\"num1\":{\"number\":17},\"num2\":{\"number\":16}},{\"num3\":{\"number\":15}}]}";
  const size_t capacity3 =  JSON_ARRAY_SIZE(2) + 4*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 90;
  DynamicJsonDocument doc3(capacity3);

  deserializeJson(doc3, json3);
  Serial.printf("%s\r\n", doc3["date"].as<String>().c_str());
  Serial.printf("%d\r\n", doc3["wight"].as<int>());

  Serial.printf("%d\r\n", doc3["num"][0]["num1"]["number"].as<int>());
  Serial.printf("%d\r\n", doc3["num"][0]["num2"]["number"].as<int>());
  Serial.printf("%d\r\n", doc3["num"][1]["num3"]["number"].as<int>());
}
void loop()
{

}