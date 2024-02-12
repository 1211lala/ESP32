#include "littlefs_api.h"
#include "dirent.h"

void littlefs_start_get_info(fs::LittleFSFS &littlefs)
{
    LittleFS.begin(false, "/littlefs", 10, "spiffs");

    Serial.printf("(%d) littlefs total %d chars\r\n", millis(), littlefs.totalBytes());
    Serial.printf("(%d) littlefs used %d chars\r\n", millis(), littlefs.usedBytes());
    littlefs_scanFile(littlefs, "/");
}

/*
    向指定文件中写入数据
*/
int littlefs_writeFile(fs::LittleFSFS &littlefs, const char *path, const char *message)
{
    File file = littlefs.open(path, FILE_WRITE);
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

/*
    向指定文件中读出数据
    返回读出的数据大小
*/
int littlefs_readFile(fs::LittleFSFS &littlefs, const char *path, char *rxbuf)
{
    File file = littlefs.open(path);
    if (!file || file.isDirectory())
    {
        return FS_OPEN_ERROE;
    }
    uint32_t number = file.available();

    if (file.readBytes(rxbuf, number))
    {
    }
    else
    {
        return FS_READ_ERROR;
    }
    file.close();
    return number;
}

String littlefs_readfile(fs::LittleFSFS &littlefs, const char *path)
{
    String rxbuf;
    File file = littlefs.open(path);
    if (!file || file.isDirectory())
    {
        return "NULL";
    }
    uint32_t number = file.available();

    rxbuf = file.readString();
    file.close();
    return rxbuf;
}
/*
    向指定文件中添加数据
*/
int littlefs_appendFile(fs::LittleFSFS &littlefs, const char *path, const char *message)
{
    File file = littlefs.open(path, FILE_APPEND);
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

/*
    查看目录下的文件支持深度查看
*/
void littlefs_scanFile(fs::LittleFSFS &littlefs, const char *path)
{
    File dirfile = littlefs.open(path);

    File file = dirfile.openNextFile();

    while (file)
    {
        if (file.isDirectory())
        {
            Serial.printf("%s\r\n", file.path());
            littlefs_scanFile(littlefs, file.path());
        }
        else
        {
            Serial.printf("%s \tsize: %d\r\n", file.path(), file.size());
        }
        file = dirfile.openNextFile();
    }
    file.close();
    dirfile.close();
}

void littlefs_fileTest(void)
{

    LittleFS.mkdir("/DIR0");
    LittleFS.mkdir("/DIR1");
    littlefs_writeFile(LittleFS, "/DIR1/1.txt", "192.168.8.1");
    littlefs_writeFile(LittleFS, "/DIR1/2.txt", "192.168.8.2");
    littlefs_writeFile(LittleFS, "/DIR1/3.txt", "192.168.8.3");
    littlefs_writeFile(LittleFS, "/DIR1/4.txt", "192.168.8.4");
    littlefs_writeFile(LittleFS, "/DIR1/5.txt", "192.168.8.5");
    littlefs_writeFile(LittleFS, "/DIR1/6.txt", "192.168.8.6");
    LittleFS.mkdir("/DIR2");
    littlefs_writeFile(LittleFS, "/DIR2/1.txt", "192.168.8.1");
    littlefs_writeFile(LittleFS, "/DIR2/2.txt", "192.168.8.2");
    littlefs_writeFile(LittleFS, "/DIR2/3.txt", "192.168.8.3");
    littlefs_writeFile(LittleFS, "/DIR2/4.txt", "192.168.8.4");
    littlefs_writeFile(LittleFS, "/DIR2/5.txt", "192.168.8.5");
    littlefs_writeFile(LittleFS, "/DIR2/6.txt", "192.168.8.6");

    littlefs_writeFile(LittleFS, "/configs.txt", "192.168.8.33");
    littlefs_writeFile(LittleFS, "/parammeter.txt", "192.168.8.33");
    littlefs_writeFile(LittleFS, "/readme.txt", "文件系测试");

    Serial.printf("\r\n文件已经创建完成如下:\r\n\r\n");
    littlefs_scanFile(LittleFS, "/");

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
    littlefs_scanFile(LittleFS, "/");

    Serial.printf("进行格式化.......\r\n\r\n");
    LittleFS.format();
    Serial.printf("格式化完成\r\n\r\n");
}