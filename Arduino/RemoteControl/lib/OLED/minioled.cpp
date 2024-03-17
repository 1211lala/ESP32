#include "minioled.h"
#include "oledfnot.h"

uint8_t OLED_RAM[64][6];

void OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
    uint8_t send_buf[3];

    Wire.beginTransmission(0x3c);
    send_buf[0] = mode;
    send_buf[1] = dat;
    Wire.write(send_buf, 2);
    Wire.endTransmission();
}

/**
 * 函数功能: OLED初始化
 * 说    明:
 */
void Oled_Init(uint8_t mode)
{
    Wire.begin(21, 22);
    Wire.setClock(1000000);
    delay(200);
    OLED_WR_Byte(0xAE, OLED_CMD); /*display off*/
    OLED_WR_Byte(0x00, OLED_CMD); /*set lower column address*/
    OLED_WR_Byte(0x12, OLED_CMD); /*set higher column address*/
    OLED_WR_Byte(0x40, OLED_CMD); /*set display start line*/
    OLED_WR_Byte(0xB0, OLED_CMD); /*set page address*/
    OLED_WR_Byte(0x81, OLED_CMD); /*contract control*/
    OLED_WR_Byte(0xff, OLED_CMD); /*128*/
    OLED_WR_Byte(0xA1, OLED_CMD); /*set segment remap*/
    OLED_WR_Byte(0xA6, OLED_CMD); /*normal / reverse*/
    OLED_WR_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
    OLED_WR_Byte(0x2F, OLED_CMD); /*duty = 1/48*/
    OLED_WR_Byte(0xC8, OLED_CMD); /*Com scan direction*/
    OLED_WR_Byte(0xD3, OLED_CMD); /*set display offset*/
    OLED_WR_Byte(0x00, OLED_CMD);
    OLED_WR_Byte(0xD5, OLED_CMD); /*set osc division*/
    OLED_WR_Byte(0x80, OLED_CMD);
    OLED_WR_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
    OLED_WR_Byte(0x21, OLED_CMD);
    OLED_WR_Byte(0xDA, OLED_CMD); /*set COM pins*/
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xdb, OLED_CMD); /*set vcomh*/
    OLED_WR_Byte(0x40, OLED_CMD);
    OLED_WR_Byte(0x8d, OLED_CMD); /*set charge pump enable*/
    OLED_WR_Byte(0x14, OLED_CMD);
    OLED_Clear(mode);
    OLED_Refresh();
    OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}

/**
 * 函数功能: 画点函数
 * 输入参数: x 坐标的x轴 范围:0-64
 * 输入参数: y 坐标的y轴 范围:0-48
 * 输入参数: t 填充/清空 1:填充 0:清空
 * 说    明:	无
 */
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    if(x < 0 || x > 64) return;
    if(y < 0 || y > 48) return;
    
    uint8_t i, j;
    i = y / 8; /* 算出数据在第几行 */
    j = y % 8; /* 算出数据在第几行的具体哪一位*/
    if (t)
    {
        OLED_RAM[x][i] |= 1 << j;
    }
    else
    {
        OLED_RAM[x][i] &= ~(1 << j);
    }
}
/**
 * 函数功能: OLED刷屏实际写数据函数
 * 输入参数: 无
 * 输入参数: 无
 * 说    明:	无
 */
void OLED_Refresh(void)
{
    uint8_t buf[1 + 64];

    for (uint8_t i = 0; i < 6; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); /* 设置行起始地址 */
        OLED_WR_Byte(0x00, OLED_CMD);     /* 设置低列起始地址 只取0x00的低四位 */
        OLED_WR_Byte(0x12, OLED_CMD);     /* 设置高列起始地址 只取0x10的低四位*/

        buf[0] = OLED_DATA;
        for (uint8_t n = 1; n < 64 + 1; n++)
        {
            buf[n] = OLED_RAM[n - 1][i];
        }

        Wire.beginTransmission(0x3c);
        Wire.write(buf, 64 + 1);
        Wire.endTransmission();
    }
}
/**
 * 函数功能: OLED屏幕清屏函数
 * 输入参数: mode 全屏点亮刷屏/全屏熄灭刷屏  0xff:亮  0:灭
 * 说    明:	无
 */
void OLED_Clear(uint8_t mode)
{
    uint8_t i, j;

    for (i = 0; i < hang / 8; i++)
    {
        for (j = 0; j < 64; j++)
        {
            OLED_RAM[j][i] = mode;
        }
    }
}
/**
 * 函数功能: 显示一个英文字符
 * 输入参数: x 坐标的x轴 范围:0-127
 * 输入参数: y 坐标的y轴 范围:0-31
 * 输入参数: chr 需要显示的字符
 * 输入参数: char_size 显示字符的大小
 * 输入参数: mode 正常显示/反色显示 1 正常显示 0 反色显示
 * 说    明: 兼容 8*6的字体
 */
void OLED_Show_Char(int x, uint8_t y, uint8_t chr, uint8_t char_size, uint8_t mode)
{
    char ram_size, size1, chr1, array, x0, y0;
    x0 = x, y0 = y;

    ram_size = char_size / 8;
    if (char_size % 8)
    {
        ram_size += 1;
    }
    ram_size = ram_size * (char_size / 2); /* 这个/2就是得出这个字体的宽度是多少 如12*6 32*16, 如果要改成长和宽一样的字体就要改成 /1 同理推出其他的字体 */

    if (char_size == 8)
        ram_size = 6; /* 为了兼容 8*6 的字体 */

    chr1 = chr - ' '; /* 计算偏移后的值 */

    for (char i = 0; i < ram_size; i++)
    {
        switch (char_size)
        {
        case (8):
            array = ascll_0806[chr1][i];
            break;
        case (12):
            array = ascll_1206[chr1][i];
        case (16):
            array = ascll_1608[chr1][i];
            break;
        case (24):
            array = ascll_2412[chr1][i];
            break;
        case (32):
            break;
        }

        for (char j = 0; j < 8; j++) /* 写入一个字节的数据*/
        {
            if (array & 0x01) /* 按位读取字节数据*/
            {
                OLED_DrawPoint(x, y, mode);
            }
            else
            {
                OLED_DrawPoint(x, y, !mode);
            }
            array = array >> 1; /* 移位操作 */
            y += 1;             /* 显示的y轴位置加一 */
        }

        x += 1;                                      /* x轴向右加一 */
        if ((size1 != 8) && ((x - x0) == size1 / 2)) /* 如果这一行数据显示完了,开始写入下一行的数据 */
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0; /* y的值加8*/
    }
}

/**
 * 函数功能: 显示英文字符串
 * 输入参数: x 坐标的x轴 范围:0-64
 * 输入参数: y 坐标的y轴 范围:0-48
 * 输入参数: str 需要显示的字符串
 * 输入参数: char_size 显示字符串的大小
 * 输入参数: mode 正常显示/反色显示 1 正常显示 0 反色显示
 * 说    明: 兼容 8*6的字体
 */
void OLED_Show_String(int x, uint8_t y, uint8_t *str, uint8_t str_size, uint8_t mode)
{
    while ((*str >= ' ') && (*str <= '~'))
    {
        OLED_Show_Char(x, y, *str, str_size, mode);
        if (str_size == 8)
            x += 6;
        else
            x += str_size / 2;
        str++;
    }
}

/**
 * 函数功能: 画直线函数
 * 输入参数: x0 坐标的x轴 范围:0-64
 * 输入参数: y0 坐标的y轴 范围:0-48
 * 输入参数: x1 坐标的x轴 范围:0-64
 * 输入参数: y1 坐标的y轴 范围:0-48
 * 说    明: 无
 */
void OLED_Draw_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    uint8_t dm = 0;
    uint8_t lx = abs(x1 - x0);
    uint8_t ly = abs(y1 - y0);
    float x = 0;
    ;
    float y = 0;

    if (lx >= ly)
    {
        dm = lx;
    }
    else
        dm = ly;

    float dx = (float)(x1 - x0) / dm;
    float dy = (float)(y1 - y0) / dm;

    x = (float)x0 + 0.5;
    y = (float)y0 + 0.5;

    for (uint8_t i = 0; i < dm; i++)
    {
        OLED_DrawPoint(x, y, 1);
        x += dx;
        y += dy;
    }
}

/**
 * 函数功能: 显示图片
 * 输入参数: x 图片的x轴起点
 * 输入参数: y 图片的y轴起点
 * 输入参数: bmp_x 图片的x长度
 * 输入参数: bmp_y 图片的y长度
 * 说    明: 无
 */
void OLED_ShowPicture(uint8_t x, uint8_t y, uint8_t bmp_x, uint8_t bmp_y, uint8_t BMP[], uint8_t mode)
{
    uint16_t j = 0;

    uint8_t x0 = x, y0 = y;

    bmp_y = bmp_y / 8 + ((bmp_y % 8) ? 1 : 0);

    for (uint8_t n = 0; n < bmp_y; n++)
    {
        for (uint8_t i = 0; i < bmp_x; i++)
        {
            uint8_t temp = BMP[j];
            j++;
            for (uint8_t m = 0; m < 8; m++)
            {
                if (temp & 0x01)
                    OLED_DrawPoint(x, y, mode);
                else
                    OLED_DrawPoint(x, y, !mode);
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == bmp_x)
            {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
}