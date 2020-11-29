#include "dma_conf.h"
#include "pro_conf.h"
#include <string.h>


static void dma_com_conf(void);


static DMA_InitTypeDef dma_s;

/************************************************
 * @brief DMA 通用配置
 ************************************************/
static void dma_com_conf( void )
{
    dma_s.DMA_Mode                  = DMA_Mode_Normal;
    dma_s.DMA_FIFOMode              = DMA_FIFOMode_Disable;
    dma_s.DMA_FIFOThreshold         = DMA_FIFOThreshold_Full;
    dma_s.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
    dma_s.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
    dma_s.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
    dma_s.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;
}


/************************************************
 * @brief 启动DMA memory to memory 传输
 * 
 * @param DMAx 
 * @param src       源地址
 * @param dst       目标地址
 * @param stream    DMA 流
 * @param size      DMA 传输size
 * @param ch        DMA 传输通道
 * @param prio      DMA 传输优先级
 ************************************************/
void dma_mem2mem ( 
    DMA_TypeDef *DMAx, 
    DMA_Stream_TypeDef *stream, 
    uint32_t ch, 
    uint32_t prio,
    uint8_t *src, 
    uint8_t *dst, 
    uint32_t size
)
{
    /* 等待当前DMA流传输完成 */
    while (DMA_GetCmdStatus( stream ) == ENABLE);

    if ( DMAx == DMA1 )
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA1, ENABLE );
    else if ( DMAx == DMA2 )
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2, ENABLE );

    DMA_DeInit( stream );

    dma_com_conf();

    dma_s.DMA_Channel               = ch;
    dma_s.DMA_Priority              = prio;
    

    dma_s.DMA_BufferSize            = size;
    dma_s.DMA_Memory0BaseAddr       = (uint32_t)src;
    dma_s.DMA_PeripheralBaseAddr    = (uint32_t)dst;

    dma_s.DMA_DIR                   = DMA_DIR_MemoryToMemory;
    dma_s.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    dma_s.DMA_PeripheralInc         = DMA_PeripheralInc_Enable;

    DMA_Init( stream, &dma_s );
    DMA_Cmd( stream, ENABLE );
}


void dma_test(void)
{
    uint8_t buf_src[512];
    uint8_t buf_dst[512];

    RCC_AHB2PeriphClockCmd( RCC_AHB2Periph_RNG, ENABLE );
    RNG_Cmd( ENABLE );

    for ( uint16_t i = 0; i < 512; i++ ) 
        buf_src[i] = RNG_GetRandomNumber()%0xFF;

    MEMSET( buf_dst, 0, sizeof(buf_dst) );

    dma_mem2mem(
        DMA1, DMA1_Stream0, DMA_Channel_1, 
        DMA_Priority_VeryHigh,
        buf_src, buf_dst, 512
    );

    while( DMA_GetCurrDataCounter( DMA1_Stream1 ) );
    DMA_Cmd( DMA1_Stream1, DISABLE );

    if ( MEMCMP( buf_src, buf_src, 512 ) == 0 ) {
        DEBUG_PRINT( "DMA transmited ok\n" );
    } else
        DEBUG_PRINT( "DMA transmited error\n" );

}
MSH_CMD_EXPORT(dma_test, dma_test);
