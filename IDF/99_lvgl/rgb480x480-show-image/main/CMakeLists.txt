idf_component_register(SRCS 
                        "main.c" 
                        "st7701s.c"  
                        "gt911.c"
                        "lvgl_interface.c"
                        "p_wifi_sta.c"
                        "lvgl_obj.c"
                        "lvgl_bar.c"
                        "lvgl_arc.c"
                        "lvglstart.c"
                        "sd_spi_fat.c"
                        "p_mqtt.c"
                       INCLUDE_DIRS
                        "."
                        )
# idf_component_register(SRC_DIRS 
#                         "."
#                        INCLUDE_DIRS
#                         "."
#                         )

# 将 ../spiffs_data 的内容以 spiffs or fatfs or littlefs 的格式烧录进 storage 分区内

# spiffs_create_partition_image(storage ../data FLASH_IN_PROJECT)
# fatfs_create_partition_image(storage ../data FLASH_IN_PROJECT)
# littlefs_create_partition_image(storage ../data FLASH_IN_PROJECT)