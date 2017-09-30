/*
 * @filename: stpm33.c
 * @Author	: nghiaphung
 * @Date	: 24/9/2017
 * @Email	: ducnghia318@gmail.com
 */
 
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "stpm33.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
#define STPM33_CS_PORT         GPIOA
#define STPM33_SCK_PORT        GPIOA
#define STPM33_MOSI_PORT       GPIOA
#define STPM33_MISO_PORT       GPIOA
#define STPM33_SYN_PORT        GPIOB

#define STPM33_CS_PIN          GPIO_Pin_4
#define STPM33_SCK_PIN         GPIO_Pin_5
#define STPM33_MOSI_PIN        GPIO_Pin_7
#define STPM33_MISO_PIN        GPIO_Pin_6
#define STPM33_SYN_PIN         GPIO_Pin_1

#define CRC_8                  (0x07)
#define STPM33_FRAME_LEN       (5)
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/

/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/
uint8_t Stpm33_Transfer(uint8_t pData);
void Crc8Calc(uint8_t u8Data);
uint8_t CalcCRC8(uint8_t *pBuf);
void Stpm33_Enable(void);
void Stpm33_Disable (void);
void delay(uint32_t pTime);
/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
extern void Stpm33_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef  SPI_InitStruct;
    /* initialize PA4 as CS */
    GPIO_InitStruct.GPIO_Pin   = STPM33_CS_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STPM33_CS_PORT, &GPIO_InitStruct);
    /* initialize PA5 as SCK */
    GPIO_InitStruct.GPIO_Pin   = STPM33_SCK_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STPM33_SCK_PORT, &GPIO_InitStruct);
    /* initialize PA6 as MISO */
    GPIO_InitStruct.GPIO_Pin   = STPM33_MISO_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(STPM33_MISO_PORT, &GPIO_InitStruct);
    /* initialize PA7 as MOSI */
    GPIO_InitStruct.GPIO_Pin   = STPM33_MOSI_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STPM33_MOSI_PORT, &GPIO_InitStruct);
    /* initialize PB1 as SYN */
    GPIO_InitStruct.GPIO_Pin   = STPM33_SYN_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STPM33_SYN_PORT, &GPIO_InitStruct);
    /* initialize SPI */
    SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial     = 7;
    SPI_Init(SPI1, &SPI_InitStruct);
    SPI_Cmd(SPI1,ENABLE);
    GPIO_SetBits(STPM33_SYN_PORT, STPM33_SYN_PIN);
    GPIO_SetBits(STPM33_CS_PORT, STPM33_CS_PIN);
    
}

uint32_t	Stpm33_ReadRegister(uint8_t pAddr)
{
    uint8_t vBuffer[5];
    uint8_t vBufferRecv[5];
	uint32_t vTemp = 0;
	uint8_t i = 0;
	vBuffer[0] = pAddr;
	vBuffer[1]	= 0xFF;
	vBuffer[2]	= 0xAA;
	vBuffer[3]	= 0x55;
	vBuffer[4]	= CalcCRC8(vBuffer);
	Stpm33_Enable();
	delay(350);
	for (i = 0; i < 5; i++){
			vBufferRecv[i] = Stpm33_Transfer(vBuffer[i]);
	}
	Stpm33_Disable();
	delay(350*2);
	vBuffer[0] = 0xFF;
	vBuffer[1]	= 0xFF;
	vBuffer[2]	= 0xAA;
	vBuffer[3]	= 0x55;
	vBuffer[4]	= CalcCRC8(vBuffer);
	Stpm33_Enable();
	delay(350);
	for (i = 0; i < 5; i++){
			vBufferRecv[i] = Stpm33_Transfer(vBuffer[i]);
	}
	Stpm33_Disable();
	vTemp = (vBufferRecv[3] << 24) + (vBufferRecv[2] << 16) + (vBufferRecv[1] << 8) + vBufferRecv[0];
	
	return vTemp;
}

 
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
void Crc8Calc(uint8_t u8Data) {
    uint8_t CRC_u8Checksum;
	uint8_t loc_u8Idx;
	uint8_t loc_u8Temp;
	loc_u8Idx = 0;
	while (loc_u8Idx < 8) {
		loc_u8Temp = u8Data ^ CRC_u8Checksum;
		CRC_u8Checksum <<= 1;
		if (loc_u8Temp & 0x80) {
			CRC_u8Checksum ^= CRC_8;
		}
		u8Data <<= 1;
		loc_u8Idx++;
	}
}

uint8_t CalcCRC8(uint8_t *pBuf) {
    uint8_t CRC_u8Checksum;
	uint8_t i;
	CRC_u8Checksum = 0x00;
	for (i = 0; i < STPM33_FRAME_LEN - 1; i++) {
		Crc8Calc(pBuf[i]);
	}
	return CRC_u8Checksum;
}

void Stpm33_Enable(void)
{
    GPIO_ResetBits(STPM33_CS_PORT, STPM33_CS_PIN);
}

void Stpm33_Disable (void)
{
    GPIO_SetBits(STPM33_CS_PORT, STPM33_CS_PIN);
}

uint8_t Stpm33_Transfer(uint8_t pData)
{
    // Write data to be transmitted to the SPI data register
	SPI1->DR = pData;
	// Wait until transmit complete
	while (!(SPI1->SR & (SPI_I2S_FLAG_TXE)));
	// Wait until receive complete
	while (!(SPI1->SR & (SPI_I2S_FLAG_RXNE)));
	// Wait until SPI is not busy anymore
	while (SPI1->SR & (SPI_I2S_FLAG_BSY));
	// Return received data from SPI data register
	return SPI1->DR;
}

void delay(uint32_t pTime)
{
	while(pTime--);
}

