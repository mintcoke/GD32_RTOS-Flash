/*
 * spi_ecat.c — GD32 SPI0 init for TR8253 ESC
 *   GD32 SPI0 (0x40013000, APB2) = STM32 SPI1, pins PA5/PA6/PA7
 *
 * SPI Mode 3 (CPOL=1, CPHA=1), 27MHz (APB2 108MHz / 4)
 * MISO configured as INPUT (not AF_PP!) to avoid bus conflict
 */

#include "spi_ecat.h"
#include "GD32Evb.h"

void MX_SPI1_Init(void)
{
    /* Enable clocks */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);       /* Alternate function clock */
    rcu_periph_clock_enable(ESC_SPI_CLK);

    /* Configure SPI pins: SCK, MISO, MOSI as Alternate Function Push-Pull */
    gpio_init(ESC_SPI_GPIO, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,
              ESC_SPI_SCK_PIN | ESC_SPI_MOSI_PIN);
    /* MISO: input (slave drives this pin, host reads) */
    gpio_init(ESC_SPI_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,
              ESC_SPI_MISO_PIN);

    /* Configure CS pin as Push-Pull output, initial HIGH (deselected) */
    gpio_init(ESC_CS_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, ESC_CS_PIN);
    gpio_bit_set(ESC_CS_GPIO, ESC_CS_PIN);

    /* De-init then configure SPI1 */
    spi_disable(ESC_SPI);
    spi_i2s_deinit(ESC_SPI);

    spi_parameter_struct spi_init_struct;
    spi_struct_para_init(&spi_init_struct);

    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;  /* CPOL=1, CPHA=1 */
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_4;                 /* PCLK2 / 4 = 27MHz */
    spi_init_struct.endian               = SPI_ENDIAN_MSB;

    spi_init(ESC_SPI, &spi_init_struct);
    spi_enable(ESC_SPI);
}
