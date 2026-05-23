#include "uart.h"

/* 全局变量定义 */
uint8_t  rs485_rx_buf[RS485_RX_BUF_SIZE];
volatile uint16_t rs485_rx_len = 0;

/*!
    \brief      初始化所有串口 (RS232用于打印, RS485用于声纳)
*/
void UART_Init(void)
{
    /* 1. 使能时钟 */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
		rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_USART1); // GD32的USART1对应引脚PA2,PA3
    rcu_periph_clock_enable(RCU_USART2); // GD32的USART2对应引脚PB10,PB11

    /* ================= 2. 配置 RS232 (调试打印) ================= */
    // PA2: TX
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    // PA3: RX
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    usart_deinit(USART1);
    usart_baudrate_set(USART1, 115200U);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_enable(USART1);

    /* ================= 3. 配置 RS485 (声纳通讯) ================= */
    // PA5: 485_DE (收发控制)
    gpio_init(RS485_DE_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, RS485_DE_PIN);
    RS485_RX_EN(); // 默认置为接收模式

    // PB10: TX
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    // PB11: RX
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    usart_deinit(USART2);
    usart_baudrate_set(USART2, 115200U); // 传感器出厂默认波特率
    usart_word_length_set(USART2, USART_WL_8BIT);
    usart_stop_bit_set(USART2, USART_STB_1BIT);
    usart_parity_config(USART2, USART_PM_NONE);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);

    /* 使能 USART2 接收中断 */
    nvic_irq_enable(USART2_IRQn, 0, 0);
    usart_interrupt_enable(USART2, USART_INT_RBNE);
    
    usart_enable(USART2);
}

/*!
    \brief      RS485 发送数据函数
*/
void RS485_SendData(uint8_t *data, uint16_t len)
{
    RS485_TX_EN(); // 切换为发送模式
    for(uint16_t i = 0; i < len; i++) {
        usart_data_transmit(USART2, data[i]);
        while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
    }
    while(RESET == usart_flag_get(USART2, USART_FLAG_TC)); // 等待发送完成
    RS485_RX_EN(); // 切换回接收模式
}

/*!
    \brief      重定向 printf 到 USART1 (RS232)
*/
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART1, (uint8_t)ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}

/*!
    \brief      USART2 (RS485) 中断服务函数
*/
void USART2_IRQHandler(void)
{
    /* 使用 usart_flag_get 直接读取硬件状态，绕过中断使能位的限制 */
    if((RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE)) || 
       (RESET != usart_flag_get(USART2, USART_FLAG_ORERR))) 
    {
        /* 读取数据寄存器，强行清空 RXNE 和 ORERR 标志位 */
        uint8_t data = (uint8_t)usart_data_receive(USART2);
        
        if(rs485_rx_len < RS485_RX_BUF_SIZE) {
            rs485_rx_buf[rs485_rx_len++] = data;
        }
    }
}
