#include "LittleFS_fun.h"

/*************************************************************************************
 * 函数功能 : 启动文件系统 打印出文件系统管理的存储内存和使用率 并遍历文件系统
 *************************************************************************************/
void fs_mount(void)
{
    LittleFS.begin(false, "/LittleFS", 10, "spiffs");
    // LittleFS.format();
    logoinfo("LittleFS total %dK \r\n", LittleFS.totalBytes() / 1024);
    logoinfo("LittleFS used %.02f%% \r\n\r\n", (float)((float)LittleFS.usedBytes() / LittleFS.totalBytes()) * 100);
    fs_scanfile("/");
}

/*************************************************************************************
 * 函数功能 : 向指定文件中写入数据文件不存在时会自动创造文件，文件存在时会覆盖之前得分数据
 * 参数     : path : 文件的根路径
 *           message : 需要写入的内容
 *************************************************************************************/
int fs_write(const char *path, const char *message)
{
    File file = LittleFS.open(path, FILE_WRITE);
    if (!file)
    {
        return FS_OPEN_ERROE;
    }
    if (file.print(message))
    {
    }
    else
    {
        return FS_WRITE_ERROE;
    }
    file.close();
    return FS_OK;
}

/*************************************************************************************
 * 函数功能 : 向指定文件中读出数据
 * 参数     : path : 文件的根路径
 * 返回值   : 读出的数据
 *************************************************************************************/
String fs_read(const char *path)
{
    String rxbuf;
    File file = LittleFS.open(path);
    if (!file || file.isDirectory())
    {
        return "NULL";
    }
    uint32_t number = file.available();

    rxbuf = file.readString();
    file.close();
    return rxbuf;
}

/*************************************************************************************
 * 函数功能 : 向指定文件的末尾添加数据数据
 * 参数     : path : 文件的根路径
 *           message : 需要添加的数据
 *************************************************************************************/
int fs_append(const char *path, const char *message)
{
    File file = LittleFS.open(path, FILE_APPEND);
    if (!file)
    {
        return FS_OPEN_ERROE;
    }
    if (file.print(message))
    {
    }
    else
    {
        return FS_WRITE_ERROE;
    }
    file.close();
    return FS_OK;
}

/*************************************************************************************
* 函数功能 : 遍历文件系统内的文件
* 参数     : path : 需要查看文件的根路径
/*************************************************************************************/
void fs_scanfile(const char *path)
{
    File dirfile = LittleFS.open(path);

    File file = dirfile.openNextFile();

    while (file)
    {
        if (file.isDirectory())
        {
            logoinfo("%s\r\n", file.path());
            fs_scanfile(file.path());
        }
        else
        {
            logoinfo("%s\t\t\t\tSIZE: %d\r\n", file.path(), file.size());
        }
        file = dirfile.openNextFile();
    }
    file.close();
    dirfile.close();

    Serial.printf("\r\n");
}





int write_fs_config(_wifi_config &config)
{
    String wifiToString = "";

    StaticJsonDocument<500> wifiBuffer;

    wifiBuffer["ssid"] = config.ssid;
    wifiBuffer["password"] = config.password;
    wifiBuffer["ip"] = config.ip;
    wifiBuffer["port"] = config.port;
    wifiBuffer["use_static_ip"] = config.use_static_ip;
    wifiBuffer["subnet"] = config.subnet;
    wifiBuffer["gateway"] = config.gateway;
    wifiBuffer["dns"] = config.dns;
    wifiBuffer["remote_ip"] = config.remote_ip;
    wifiBuffer["remote_port"] = config.remote_port;
    /* 序列化JSON数据并导出字符串 */
    serializeJson(wifiBuffer, wifiToString);
#if 1
    logoinfo("将写入%s的数据如下:\r\n", WIFI_CONFIG_PATH);
    logoinfo("%s\r\n", wifiToString.c_str());
#endif
    if (fs_write(WIFI_CONFIG_PATH, wifiToString.c_str()) != FS_OK)
    {

        logoinfo("数据写入失败!\r\n");
        return -1;
    }
    return 0;
}

int read_fs_config(_wifi_config &config)
{
    String wifiString = "";

    DynamicJsonDocument doc(500);

    wifiString = fs_read(WIFI_CONFIG_PATH);
    if (wifiString == "NULL")
    {
        logoinfo("%s读取失败!\r\n", WIFI_CONFIG_PATH);
        return -1;
    }
    DeserializationError error = deserializeJson(doc, wifiString);
    if (error)
    {
        return -2;
    }
    config.ssid = doc["ssid"].as<String>();
    config.password = doc["password"].as<String>();
    config.ip = doc["ip"].as<String>();
    config.use_static_ip = doc["use_static_ip"].as<bool>();
    config.port = doc["port"].as<String>();
    config.subnet = doc["subnet"].as<String>();
    config.gateway = doc["gateway"].as<String>();
    config.dns = doc["dns"].as<String>();
    config.remote_ip = doc["remote_ip"].as<String>();
    config.remote_port = doc["remote_port"].as<String>();

#if 1
    logoinfo("将读出%s的数据如下:\r\n", WIFI_CONFIG_PATH);
    logoinfo("%s\r\n", wifiString.c_str());
    logoinfo("ssid : %s\r\n", config.ssid.c_str());
    logoinfo("password : %s\r\n", config.password.c_str());
    logoinfo("ip : %s\r\n", config.ip.c_str());
    logoinfo("use_static_ip : %d  [1:y  0:n]\r\n", config.use_static_ip);
    logoinfo("port : %s\r\n", config.port.c_str());
    logoinfo("subnet : %s\r\n", config.subnet.c_str());
    logoinfo("gateway : %s\r\n", config.gateway.c_str());
    logoinfo("dns : %s\r\n", config.dns.c_str());
    logoinfo("remote_ip : %s\r\n", config.remote_ip.c_str());
    logoinfo("remote_port : %s\r\n", config.remote_port.c_str());
#endif
    return 0;
}






void fs_test(void)
{
    LittleFS.mkdir("/DIR0");
    LittleFS.mkdir("/DIR1");
    fs_write("/DIR1/1.txt", "192.168.8.1");
    fs_write("/DIR1/2.txt", "192.168.8.2");
    fs_write("/DIR1/3.txt", "192.168.8.3");
    fs_write("/DIR1/4.txt", "192.168.8.4");
    fs_write("/DIR1/5.txt", "192.168.8.5");
    fs_write("/DIR1/6.txt", "192.168.8.6");
    LittleFS.mkdir("/DIR2");
    fs_write("/DIR2/1.txt", "192.168.8.1");
    fs_write("/DIR2/2.txt", "192.168.8.2");
    fs_write("/DIR2/3.txt", "192.168.8.3");
    fs_write("/DIR2/4.txt", "192.168.8.4");
    fs_write("/DIR2/5.txt", "192.168.8.5");
    fs_write("/DIR2/6.txt", "192.168.8.6");

    fs_write("/configs.txt", "192.168.8.33");
    fs_write("/parammeter.txt", "192.168.8.33");
    fs_write("/readme.txt", "文件系测试");

    Serial.printf("\r\n文件已经创建完成如下:\r\n\r\n");
    fs_scanfile("/");

    Serial.printf("删除DIR0、DIR1文件夹及文件parammeter.txt.......\r\n\r\n");
    LittleFS.rmdir("/DIR0");
    LittleFS.remove("/DIR1/1.txt");
    LittleFS.remove("/DIR1/2.txt");
    LittleFS.remove("/DIR1/3.txt");
    LittleFS.remove("/DIR1/4.txt");
    LittleFS.remove("/DIR1/5.txt");
    LittleFS.remove("/DIR1/6.txt");
    LittleFS.rmdir("/DIR1");

    LittleFS.remove("/parammeter.txt");

    Serial.printf("删除后的目录:\r\n\r\n");
    fs_scanfile("/");

    Serial.printf("进行格式化.......\r\n\r\n");
    LittleFS.format();
    Serial.printf("格式化完成\r\n\r\n");
}