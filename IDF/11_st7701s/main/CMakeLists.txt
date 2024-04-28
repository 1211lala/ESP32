idf_component_register(SRC_DIRS 
                        "."
                        INCLUDE_DIRS 
                        ""

                        REQUIRES gt911
                        REQUIRES st7701s
                        )




# idf_component_register(SRCS "p_spi_st7701s.c" "p_i2c_gt911.c" "main.c"
#                     INCLUDE_DIRS ".")