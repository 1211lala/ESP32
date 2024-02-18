# idf_component_register(SRCS "file1.c" "fileN.c" 
#                     INCLUDE_DIRS "路径1" "路径N")  

# 我只有一个源文件，则内容为：  
idf_component_register(SRCS "ledPin.c" 
                    INCLUDE_DIRS ".")
