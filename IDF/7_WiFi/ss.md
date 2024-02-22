
							
		switch(system_process)
		{
			case(0):        
                /* 获取UART3的数据 */
                /* 在这个地方清零就避免了在读取传感器值时传感器被拔走无法读取值进不去下一个流程的问题 */
                uart3_strat_flag = 0;
                ASK_K5S_ID();
                HAL_Delay(100);
                if(uart3_strat_flag != 3)	/* 说明数据接失败了 填入错误标志 */
                {
                    uart3_get_id = 0xff;
                    uart3_get_point = 0xff;
                    uart3_get_data = 0xffff;
                }
                /* 将串口三的上接的传感器ID，数据写入发送缓存  */
                write_sensor_data(uart3_get_id, uart3_get_point, uart3_get_data);
                break;
			case(1):
                /* 获取WK2124接口一的传感器数据 */
                WK_Get_All_Data(1, &wk_uart1_id, &wk_uart1_point, &wk_uart1_data);
                /* 将WK2124接口一的上接的传感器ID，数据写入发送缓存  */
                write_sensor_data(wk_uart1_id, wk_uart1_point, wk_uart1_data);
                break;
			case(2):
                WK_Get_All_Data(2, &wk_uart2_id, &wk_uart2_point, &wk_uart2_data);
                write_sensor_data(wk_uart2_id, wk_uart2_point, wk_uart2_data);
                break;
			case(3):
                WK_Get_All_Data(3, &wk_uart3_id, &wk_uart3_point, &wk_uart3_data);
                write_sensor_data(wk_uart3_id, wk_uart3_point, wk_uart3_data);
                break;
			case(4):
                WK_Get_All_Data(4, &wk_uart4_id, &wk_uart4_point, &wk_uart4_data);
                write_sensor_data(wk_uart4_id, wk_uart4_point, wk_uart4_data);
                break;
			case(5):
                temp1_data = SHT20_1_TEMP();
                hum1_data = SHT20_1_HUM();
                temp2_data = SHT20_2_TEMP();
                hum2_data = SHT20_2_HUM();
        }