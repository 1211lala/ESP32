
idf_component_register(SRCS 
                        "main.c" 
                        "lvgl_interface.c"
                        "sd_spi_fat.c"
                        "wifi_sta.c"
                        "wifi_mqtt.c"
                        "./lvgl_app/lvglstart.c"
                        "./lvgl_app/font_14.c"
                        "./lvgl_app/font_24.c"
                        "./lvgl_app/font_40.c"
                        "./lvgl_app/font_80.c"
                        "./lvgl_app/font_ch_30.c"
                       INCLUDE_DIRS
                        "."
                        )
