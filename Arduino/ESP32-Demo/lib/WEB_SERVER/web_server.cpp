#include "web_server.h"

/*
    参考连接
    https://blog.csdn.net/Naisu_kun/article/details/107164633
*/

AsyncWebServer WebServer(80);

void web_server_init(void)
{
    WebServer.serveStatic("/", LittleFS, "/wifi_config").setDefaultFile("index.html");
    WebServer.on("/set_wifi_config", HTTP_POST, set_wifi_config_callback);
    WebServer.on("/get_wifi_config", HTTP_GET, get_wifi_config_callback);
    AsyncElegantOTA.begin(&WebServer);
    WebServer.begin();
    logoinfo("web server started!\r\n");
}


void get_wifi_config_callback(AsyncWebServerRequest *request)
{
    logoinfo("收到/set_wifi_config的请求\r\n");

    String wifiToString = "";

    StaticJsonDocument<500> wifiBuffer;

    wifiBuffer["ssid"] = _wc.ssid;
    wifiBuffer["password"] = _wc.password;
    wifiBuffer["ip"] = _wc.ip;
    wifiBuffer["port"] = _wc.port;
    wifiBuffer["use_static_ip"] = _wc.use_static_ip;
    wifiBuffer["subnet"] = _wc.subnet;
    wifiBuffer["gateway"] = _wc.gateway;
    wifiBuffer["dns"] = _wc.dns;
    wifiBuffer["remote_ip"] = _wc.remote_ip;
    wifiBuffer["remote_port"] = _wc.remote_port;
    /* 序列化JSON数据并导出字符串 */
    serializeJson(wifiBuffer, wifiToString);

    request->send(200, "text/plain", wifiToString);
}
void set_wifi_config_callback(AsyncWebServerRequest *request)
{
    logoinfo("收到/set_wifi_config的请求\r\n");
    _wc.ssid = request->arg("ssid");
    _wc.password = request->arg("password");
    _wc.ip = ipaddress_ascll_to_int(request->arg("ip").c_str()).toString();

    if (request->arg("use_static_ip") == "on")
    {
        _wc.use_static_ip = true;
    }
    else
    {
        _wc.use_static_ip = false;
    }
    _wc.port = request->arg("port");
    _wc.gateway = ipaddress_ascll_to_int(request->arg("gateway").c_str()).toString();
    _wc.subnet = ipaddress_ascll_to_int(request->arg("subnet").c_str()).toString();
    _wc.dns = ipaddress_ascll_to_int(request->arg("dns").c_str()).toString();
    _wc.remote_ip = ipaddress_ascll_to_int(request->arg("remote_ip").c_str()).toString();
    _wc.remote_port = request->arg("remote_port");

    write_fs_config(_wc);
    #if 1
    logoinfo("ssid : %s\r\n", _wc.ssid.c_str());
    logoinfo("password : %s\r\n", _wc.password.c_str());
    logoinfo("ip : %s\r\n", _wc.ip.c_str());
    logoinfo("port : %s\r\n", _wc.port.c_str());
    logoinfo("use_static_ip : %d  [1:use  0:no]\r\n", _wc.use_static_ip);
    logoinfo("subnet : %s\r\n", _wc.subnet.c_str());
    logoinfo("gateway : %s\r\n", _wc.gateway.c_str());
    logoinfo("dns : %s\r\n", _wc.dns.c_str());
    logoinfo("remote_ip : %s\r\n", _wc.remote_ip.c_str());
    logoinfo("remote_port : %s\r\n", _wc.remote_port.c_str());
    #endif
    request->send(200, "text/plain", "配置完成将重启...");

    vTaskDelay(1500);
    ESP.restart();
}
