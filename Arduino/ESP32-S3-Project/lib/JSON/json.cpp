#include "json.h"

_wifi_config wifi_config;

/* sta模式需要连接的ssd 或者ap模式需要开启的ssd*/
String ssd = "";
/* sta模式下的密码 或者ap模式下的密码 */
String password = "";
/* sta模式下的ip 或者ap模式下的ip */
IPAddress ip;
/* sta模式下的getway 或者ap模式下的getway */
IPAddress getway;
/* sta模式下的subnet 或者ap模式下的subnet */
IPAddress subnet;
/* 设置的tcp端口 */
uint32_t tcp_port = 0;
/* 防火门状态 */
uint8_t door_status = 0;
/* 防火门ID */
uint16_t door_id = 0;
/* 防火门张开角度 */
uint16_t door_angle = 0;

void json_set_wifi_configs(uint8_t esp32_mode)
{
    /* 计算含有一个对象,对象中有两个名值对的内存 */
    const size_t capacity = 1 * JSON_OBJECT_SIZE(5) + 500;
    /* 创建doc对象,并赋予capacity大小的内存 */
    DynamicJsonDocument doc(capacity);
    String config_json = "";
    if (esp32_mode == 1)
    {
        config_json = littlefs_readfile(LittleFS, "/sta_configs.txt");
    }
    else if (esp32_mode == 0)
    {
        config_json = littlefs_readfile(LittleFS, "/ap_configs.txt");
    }

    deserializeJson(doc, config_json); /* 解析json */

    password = doc["password"].as<String>();
    ssd = doc["ssd"].as<String>();
    String ip_1 = doc["ip"].as<String>();
    String getway_1 = doc["getway"].as<String>();
    String subnet_1 = doc["subnet"].as<String>();
    tcp_port = doc["port"].as<int>();
#if 0
    Serial.printf("%s\r\n", config_json.c_str());
    Serial.printf("password: %s\r\n", password);
    Serial.printf("ssd: %s\r\n", ssd);
    Serial.printf("ip: %s\r\n", ip);
    Serial.printf("getway: %s\r\n", getway);
    Serial.printf("subnet: %s\r\n", subnet);
#endif
    ip = ipaddress_ascll_to_int(ip_1.c_str());
    getway = ipaddress_ascll_to_int(getway_1.c_str());
    subnet = ipaddress_ascll_to_int(subnet_1.c_str());

    if (esp32_mode == 1)
    {
        bsp_wifi_sta_init(ssd.c_str(), password.c_str(), ip, getway, subnet);
    }
    else if (esp32_mode == 0)
    {
        bsp_wifi_ap_init(ssd.c_str(), password.c_str(), ip, getway, subnet);
    }
}

void write_wifi_config(_wifi_config config)
{

    String config_buffer = "{\"ssd\":\"" + config.ssd + "\",\"password\":\"" + config.password + "\",\"ip\":\"" + config.ip.toString() + "\",\"getway\": \"" + config.getwey.toString() + "\",\"subnet\": \"" + config.subnet + "\",\"port\": 2100}";
    Serial.printf("%s\r\n",config_buffer.c_str());
    Serial.printf("%s\r\n",config.ip.toString().c_str());
    Serial.printf("%s\r\n",config.getwey.toString().c_str());
    Serial.printf("%s\r\n",config.subnet.toString().c_str());
}

IPAddress ipaddress_ascll_to_int(const char *ipbuf)
{
    uint8_t flag = 1;
    uint8_t count = 0;
    char ip1[5] = {0};
    char ip2[5] = {0};
    char ip3[5] = {0};
    char ip4[5] = {0};

    for (uint8_t i = 0; i < strlen(ipbuf); i++)
    {
        if (ipbuf[i] == '.')
        {
            flag += 1;
            count = 0;
        }
        else
        {
            if (flag == 1)
            {
                ip1[count] = ipbuf[i];
                count += 1;
            }
            if (flag == 2)
            {
                ip2[count] = ipbuf[i];
                count += 1;
            }
            if (flag == 3)
            {
                ip3[count] = ipbuf[i];
                count += 1;
            }
            if (flag == 4)
            {
                ip4[count] = ipbuf[i];
                count += 1;
            }
        }
    }
    ip1[strlen(ip1)] = '\0';
    ip2[strlen(ip2)] = '\0';
    ip3[strlen(ip3)] = '\0';
    ip4[strlen(ip4)] = '\0';

    IPAddress address(atoi(ip1), atoi(ip2), atoi(ip3), atoi(ip4));
    return address;
}