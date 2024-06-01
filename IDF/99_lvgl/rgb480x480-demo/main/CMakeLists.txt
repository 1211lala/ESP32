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
                        "lvgl_checkbox.c"
                       INCLUDE_DIRS
                        "."
                        )
# idf_component_register(SRC_DIRS 
#                         "."
#                        INCLUDE_DIRS
#                         "."
#                         )