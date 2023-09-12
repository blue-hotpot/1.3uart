#include "bsp_usart.h"
#include <stdio.h>



void usart_config(uint32_t baudrate)
{
	rcu_periph_clock_enable(RCU_USART0);//时钟使能
	rcu_periph_clock_enable(BSP_USART_RX_RCU);//GPIOA时钟使能
	rcu_periph_clock_enable(BSP_USART_TX_RCU);
	
	gpio_af_set(BSP_USART_TX_PORT,BSP_USART_AF,BSP_USART_TX_PIN);//复用功能设置
	gpio_af_set(BSP_USART_RX_PORT,BSP_USART_AF,BSP_USART_RX_PIN);
	
	gpio_mode_set(BSP_USART_TX_PORT,GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_USART_TX_PIN);
	gpio_mode_set(BSP_USART_RX_PORT,GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_USART_RX_PIN);

	gpio_output_options_set(BSP_USART_TX_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_USART_TX_PIN);
	gpio_output_options_set(BSP_USART_RX_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_USART_RX_PIN);
	
	usart_deinit(USART0);//初始化
	usart_baudrate_set(USART0,baudrate);//波特率
	usart_parity_config(USART0,USART_PM_NONE);//校验位
	usart_word_length_set(USART0,USART_WL_8BIT);//数据位
	usart_stop_bit_set(USART0,USART_STB_1BIT);//停止位
	
	usart_enable(USART0);
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);
	
}

void usart_send (uint32_t data)
{
	usart_data_transmit(USART0,(uint32_t)data);
	while(RESET==usart_flag_get(USART0,USART_FLAG_TBE))//不为空则在此循环
	{}
}

void usart_send_string(uint8_t *string)
{
	while(string && *string)
	{
		usart_send(*string++);
	}
}

int fputc (int ch,FILE *f)
{
	usart_send(ch);
	return ch;
}

