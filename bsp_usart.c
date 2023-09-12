#include "bsp_usart.h"
#include <stdio.h>



void usart_config(uint32_t baudrate)
{
	rcu_periph_clock_enable(RCU_USART0);//ʱ��ʹ��
	rcu_periph_clock_enable(BSP_USART_RX_RCU);//GPIOAʱ��ʹ��
	rcu_periph_clock_enable(BSP_USART_TX_RCU);
	
	gpio_af_set(BSP_USART_TX_PORT,BSP_USART_AF,BSP_USART_TX_PIN);//���ù�������
	gpio_af_set(BSP_USART_RX_PORT,BSP_USART_AF,BSP_USART_RX_PIN);
	
	gpio_mode_set(BSP_USART_TX_PORT,GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_USART_TX_PIN);
	gpio_mode_set(BSP_USART_RX_PORT,GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_USART_RX_PIN);

	gpio_output_options_set(BSP_USART_TX_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_USART_TX_PIN);
	gpio_output_options_set(BSP_USART_RX_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_USART_RX_PIN);
	
	usart_deinit(USART0);//��ʼ��
	usart_baudrate_set(USART0,baudrate);//������
	usart_parity_config(USART0,USART_PM_NONE);//У��λ
	usart_word_length_set(USART0,USART_WL_8BIT);//����λ
	usart_stop_bit_set(USART0,USART_STB_1BIT);//ֹͣλ
	
	usart_enable(USART0);
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);
	
}

void usart_send (uint32_t data)
{
	usart_data_transmit(USART0,(uint32_t)data);
	while(RESET==usart_flag_get(USART0,USART_FLAG_TBE))//��Ϊ�����ڴ�ѭ��
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

