#include "fatfs_sd.h"

/*****************************************************************************************************
    https://github.com/espressif/arduino-esp32/blob/master/libraries/SD/examples/SD_Test/SD_Test.ino
*****************************************************************************************************/

/******************************************************************************
 * 函数描述: spi总线初始化SD卡并挂载fatfs,在初始化前一定要设置为fat32模式
 * 参  数1:
 *******************************************************************************/
int sd_mount_fatfs(void)
{
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS, SPI, 4000000U, "/sd", 5, false))
    {
        loginfo("sd卡挂载失败\r\n");
        return ESP_FAIL;
    }
    switch (SD.cardType())
    {
    case CARD_NONE:
        loginfo("No SD card attached\n");
        break;
    case CARD_MMC:
        loginfo("MMC\n");
        break;
    case CARD_SD:
        loginfo("SDSC\n");
        break;
    case CARD_SDHC:
        loginfo("SDHC\n");
        break;
    default:
        loginfo("UNKNOWN\n");
        break;
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    loginfo("SD Card Size: %lluMB\n", cardSize);

    loginfo("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    loginfo("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));

    return ESP_OK;
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(fs, file.path(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

/******************************************************************************
 * 函数描述: 检测文件是否存在
 * 参  数1: 文件路径
 * 返  回:  文件的大小/ (-X 不存在)
 *******************************************************************************/
int isexist(const char *path)
{
    FRESULT ret;
    FILINFO fno;
    ret = f_stat(path, &fno);
    if (ret == FR_OK)
    {
        return (int)fno.fsize;
    }
    else
    {
        loginfo("%s不存在\n", path);
    }
    return -FR_NO_FILE;
}

/******************************************************************************
 * 函数描述: 向指定文件中写数据
 * 参  数1: 文件路径
 * 参  数2: 需要写入的数据
 * 返  回:  实际写入的数据(字节)
 *******************************************************************************/
int fatfs_write(const char *path, char *buffer, uint32_t size)
{
    UINT writeSize = 0;
    FIL fil;
    FRESULT fr;
    fr = f_open(&fil, path, FA_CREATE_ALWAYS | FA_WRITE);
    if (fr != FR_OK)
    {
        loginfo("failed to open %s    error code(%d)\n", path, fr);
        return -FR_NO_FILE;
    }
    fr = f_write(&fil, (char *)buffer, size, &writeSize);
    if (fr != FR_OK)
    {
        loginfo("failed to write %s    error code(%d)\n", path, fr);
        return -FR_NO_FILE;
    }
    f_close(&fil);
    return writeSize;
}

/******************************************************************************
 * 函数描述: 读取文件内容，一定要在读取成功(return > 0)后使用free(), 未成功不用在内部free
 * 参  数1: 文件路径
 * 参  数2: 缓存buffer
 * 参  数3: 所允许分配的最大字节数
 * 返  回:  实际读取的字节数(字节)
 *
    char * buffer = NULL;
    int cnt = fatfs_read("/hello.txt", &buffer, 200);
    loginfo("%d\n", cnt);
    if(cnt > 0)
    {
        buffer[cnt]  = '\0';
        loginfo("read = %s\r\n", buffer);
        free(buffer);
    }
 *******************************************************************************/
int fatfs_read(const char *path, char **buffer, uint32_t max_size)
{
    UINT readSize = 0;
    FIL fil;
    FRESULT fr;
    uint32_t fileSize = isexist(path);
    if (fileSize > max_size)
    {
        loginfo("%s文件太大, 以及超过分配的最大内存", path);
        return -1;
    }

    fr = f_open(&fil, path, FA_READ);
    if (fr != FR_OK)
    {
        loginfo("failed to open %s    error code(%d)", path, fr);
        return -FR_NO_FILE;
    }
    *buffer = (char *)malloc(fileSize + 1);
    fr = f_read(&fil, *buffer, fileSize, &readSize);
    if (fr != FR_OK)
    {
        // free(*buffer);
        loginfo("failed to read %s    error code(%d)", path, fr);
        return -FR_NO_FILE;
    }
    f_close(&fil);
    return fileSize;
}