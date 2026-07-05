#include "e290_demo.h"
#include "pan_rf.h"

/* 指示传输是否完成的标志 */
extern struct RxDoneMsg RxDoneParams;

/* us级延时函数 */
void TIM_Delay_us(uint32_t us)
{
    uint32_t start = __HAL_TIM_GET_COUNTER(&htim2);
    while ((__HAL_TIM_GET_COUNTER(&htim2) - start) < us);
}

/* 检测发送完成 */
bool e290_demo_check_rx_done( uint8_t *buffer, uint8_t * length  , double *rssi)
{
	bool ret = false;
	
	if( RxDoneParams.Size != 0 )
	{
		memcpy( buffer ,  RxDoneParams.Payload , RxDoneParams.Size );
		*length = RxDoneParams.Size ;
		RxDoneParams.Size = 0;
		
		*rssi = rf_get_rssi();
		
		ret = true;
	}
	
	return ret;
}


/* 使用显示菜单用户配置参数重新配置模组 */
// void e290_demo_menu_config( menu_config_t  *config )
// {
//     int8_t power_select;
    
//     rf_set_freq(config->frequency_mhz* 1000000 );
//     rf_set_bw(config->ChirpIoT_bw);
//     rf_set_sf(config->ChirpIoT_sf);
//     rf_set_code_rate(config->ChirpIoT_cr);
    
//     switch( config->tx_power )
// 	{
// 		case 27: 
// 			power_select = 18; 
// 			break;
		
// 		case 24:
// 			power_select = 13; 
// 			break;			
		
// 		case 20:
// 			power_select = 11; 
// 			break;	

// 		default:
// 			power_select = 22; 
// 			break;
// 	}
    
//     rf_set_tx_power(power_select);
// }

/* 模组切换到接收状态 */
void e290_demo_receive(void)
{
    uint32_t cnt = 0;
    uint8_t i;
    
    rf_enter_continous_rx();
    rf_irq_process();
    
    if(rf_get_recv_flag() == RADIO_FLAG_RXDONE)
        {
            /* 一定要有这里的清除状态标志操作 */
            rf_set_recv_flag(RADIO_FLAG_IDLE);
            printf("Rx : SNR: %f ,RSSI: %d \r\n", RxDoneParams.Snr, RxDoneParams.Rssi);//
            
            for(i = 0; i < RxDoneParams.Size; i++)
            {
                printf("0x%02x ", RxDoneParams.Payload[i]);
            }
            printf("\r\n");
            
            cnt ++;
            printf("###Rx cnt %d##\r\n", cnt);
        }
        
        if((rf_get_recv_flag() == RADIO_FLAG_RXTIMEOUT) || (rf_get_recv_flag() == RADIO_FLAG_RXERR))
        {
            rf_set_recv_flag(RADIO_FLAG_IDLE);
            printf("Rx err\r\n");
        }
}

/* 发送函数 */
void e290_demo_transmit( uint8_t *buffer , uint8_t length )
{
    /* 进入连续发送 */
    rf_enter_continous_tx();
	rf_continous_tx_send_data( buffer , length );
    // gpio_led_tx_on();
}

/* 模组轮询任务 (状态机) */
void e290_demo_task(void)
{
    rf_irq_process();
    
    if( RADIO_FLAG_RXDONE== rf_get_recv_flag())  
    {
        // gpio_led_rx_on();
        
        rf_set_recv_flag(RADIO_FLAG_IDLE);

        rf_recv_packet(RxDoneParams.Payload);
    }
    else if(RADIO_FLAG_TXDONE == rf_get_recv_flag())
    {
        // gpio_led_tx_off();
    }
    else 
    {
        // gpio_led_rx_off();
    }
}

