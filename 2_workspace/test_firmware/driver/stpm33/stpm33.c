
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "stpm33.h"
#include "../lcd/lcd.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
#define STPM33_CS_PORT         GPIOA
#define STPM33_SCK_PORT        GPIOA
#define STPM33_MOSI_PORT       GPIOA
#define STPM33_MISO_PORT       GPIOA
#define STPM33_SYN_PORT        GPIOB
#define STPM33_EN_PORT         GPIOA

#define STPM33_CS_PIN          GPIO_Pin_4
#define STPM33_SCK_PIN         GPIO_Pin_5
#define STPM33_MOSI_PIN        GPIO_Pin_7
#define STPM33_MISO_PIN        GPIO_Pin_6
#define STPM33_SYN_PIN         GPIO_Pin_1
#define STPM33_EN_PIN          GPIO_Pin_11

#define CRC_8                  (0x07)
#define STPM33_FRAME_LEN       (5)

/* registers */
/* refer to STPM3x datasheet and AN470 */
#define STPM_DFE_CR_1_ADDRESS          0x18 //GAIN1
#define STPM_DFE_CR_2_ADDRESS          0x1A //GAIN2
#define STPM_DSP_CR_3_ADDRESS          0x04 //auto latch
#define STPM_DSP_CR_5_ADDRESS          0x08 //CHV1
#define STPM_DSP_CR_6_ADDRESS          0x0A //CHC1
#define STPM_DSP_CR_7_ADDRESS          0x0C //CHV2
#define STPM_DSP_CR_8_ADDRESS          0x0E //CHC2
#define STPM_DSP_REG_14_ADDRESS        0x48 //C1 RMS and V1 RMS
#define STPM_DSP_REG_15_ADDRESS        0x4A //C2 RMS and V2 RMS
#define STPM_PH1_REG_5_ADDRESS         0x5C //PH1 active power
#define STPM_PH1_REG_7_ADDRESS         0x60 //PH1 reactive power
#define STPM_PH2_REG_5_ADDRESS         0x74
#define STPM_PH2_REG_7_ADDRESS         78

#define STPM_SUBMASK_POWER_ACTIVE      ((uint32_t)0x1FFFFFFF)
#define STPM_SUBMASK_POWER_REACTIVE    ((uint32_t)0x1FFFFFFF)
#define STPM_SUBMASK_CURRENT_RMS       ((uint32_t)0xFFFF8000)
#define STPM_SUBMASK_VOLTAGE_RMS       ((uint32_t)0x00007FFF)
#define STPM_CURRENT_SHIFT             15

/* refer to source code metrology of stpm33 */
#define STPM_VOL_FACT_CH1              116274
#define STPM_CUR_FACT_CH1              25934
#define STPM_POW_FACT_CH1              30154605
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/
uint8_t 	stmBufRecv[5];
uint8_t 	stmBufSend[5];
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
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STPM33_CS_PORT, &GPIO_InitStruct);
    /* initialize PA5 as SCK */
    GPIO_InitStruct.GPIO_Pin   = STPM33_SCK_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(STPM33_SCK_PORT, &GPIO_InitStruct);
    /* initialize PA6 as MISO */
    GPIO_InitStruct.GPIO_Pin   = STPM33_MISO_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(STPM33_MISO_PORT, &GPIO_InitStruct);
    /* initialize PA7 as MOSI */
    GPIO_InitStruct.GPIO_Pin   = STPM33_MOSI_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(STPM33_MOSI_PORT, &GPIO_InitStruct);
    /* initialize PB1 as SYN */
    GPIO_InitStruct.GPIO_Pin   = STPM33_SYN_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STPM33_SYN_PORT, &GPIO_InitStruct);
    /* initialize PA11 as EN */
    GPIO_InitStruct.GPIO_Pin   = STPM33_EN_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STPM33_EN_PORT, &GPIO_InitStruct);
    GPIO_PinLockConfig(STPM33_MISO_PORT, STPM33_MISO_PIN);
    GPIO_PinLockConfig(STPM33_MOSI_PORT, STPM33_MOSI_PIN);
    GPIO_PinLockConfig(STPM33_SCK_PORT, STPM33_SCK_PIN);
    /* initialize SPI */
    SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
    //SPI_InitStruct.SPI_CRCPolynomial     = 7;
    SPI_Init(SPI1, &SPI_InitStruct);
    SPI_Cmd(SPI1,ENABLE);
    GPIO_SetBits(STPM33_SYN_PORT, STPM33_SYN_PIN);
    
    Stpm33_Enable();
    GPIO_SetBits(STPM33_EN_PORT, STPM33_EN_PIN);
    delay1(200);
    GPIO_ResetBits(STPM33_EN_PORT, STPM33_EN_PIN);
    delay1(200);
    
    Stpm33_Disable();
  
}

uint32_t	Stpm33_ReadRegister(uint8_t pAddr)
{
    uint8_t vBuffer[5];
    uint8_t vBufferRecv[5];
	uint32_t vTemp = 0;
	uint8_t i = 0;
	vBuffer[0]  = pAddr;
	vBuffer[1]	= 0x05;
	vBuffer[2]	= 0x80;
	vBuffer[3]	= 0x00;
	vBuffer[4]	= CalcCRC8(vBuffer);
    
    // create latch to read data
    GPIO_ResetBits(STPM33_SYN_PORT, STPM33_SYN_PIN);
    delay(350); //delay 4us
    GPIO_SetBits(STPM33_SYN_PORT, STPM33_SYN_PIN);
    
	Stpm33_Enable();
    delay(350);
	for (i = 0; i < 5; i++){
			vBufferRecv[i] = Stpm33_Transfer(vBuffer[i]);
	}
    
	Stpm33_Disable();
    delay(350*2);
	vBuffer[0]  = 0xFF; // dummy read address
	vBuffer[1]	= 0xFF; // dummy write address
	vBuffer[2]	= 0xAA; // dummy data
	vBuffer[3]	= 0x55; // dummy data
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

uint32_t Stpm33_ReadVol (void)
{
    uint32_t raw_data;
    uint64_t cal_data;
    /* read data from DSP_REG 14 */
    raw_data = Stpm33_ReadRegister(STPM_DSP_REG_14_ADDRESS);
    /* raw voltage read from STPM33 */
    raw_data = raw_data & STPM_SUBMASK_VOLTAGE_RMS;
    /* Calculate real values with factors */
    cal_data = (uint64_t)raw_data * STPM_VOL_FACT_CH1;
    /* multiply by 10 to have mili vol */
    cal_data = cal_data * 10;
    /* Shift calcul result to 15 bits ( resolution of Reg inside metrology block)*/
    cal_data >>= 15;
    return (uint32_t)cal_data;
}
 
uint32_t Stpm33_ReadCur (void)
{
    uint32_t raw_data;
    uint64_t cal_data;
    /* read data from DSP_REG 14 */
    raw_data = Stpm33_ReadRegister(STPM_DSP_REG_14_ADDRESS);
    /* raw voltage read from STPM33 */
    raw_data = (raw_data & STPM_SUBMASK_CURRENT_RMS) >> STPM_CURRENT_SHIFT;
    /* Calculate real values with factors */
    cal_data = (uint64_t)(raw_data * STPM_CUR_FACT_CH1);
    /* multiply by 10 to have mili ampe */
    cal_data = cal_data * 10;
    /* Shift calcul result to 17 bits ( resolution of Reg inside metrology block)*/
    cal_data >>= 17;  
    return (uint32_t)cal_data;   
}

uint32_t Stpm33_ReadPowerActive (void)
{
    uint32_t raw_data;
    uint64_t cal_data; 
    /* read data from PH1_REG5 */
    raw_data = Stpm33_ReadRegister(STPM_PH1_REG_5_ADDRESS);
    /* raw active power read from STPM33 */
    raw_data = raw_data & STPM_SUBMASK_POWER_ACTIVE;
    raw_data <<= 4;  // handle sign extension
    raw_data >>= 4;
    /* Calculate real values with factors */
    cal_data = (uint64_t)raw_data * STPM_POW_FACT_CH1;    
    /* multiply by 10 to have mili */
    cal_data = cal_data * 10;
    /* Shift calcul result to 28 bits ( resolution of Reg inside metrology block)*/
    cal_data >>= 28;
    return (uint32_t)cal_data;     
}

uint32_t Stpm33_ReadPowerReactive (void)
{
    uint32_t raw_data;
    uint64_t cal_data; 
    /* read data from PH1_REG5 */
    raw_data = Stpm33_ReadRegister(STPM_PH1_REG_7_ADDRESS);
    /* raw active power read from STPM33 */
    raw_data = raw_data & STPM_SUBMASK_POWER_REACTIVE;
    raw_data <<= 4;  // handle sign extension
    raw_data >>= 4;
    /* Calculate real values with factors */
    cal_data = (uint64_t)raw_data * STPM_POW_FACT_CH1;    
    /* multiply by 10 to have mili */
    cal_data = cal_data * 10;
    /* Shift calcul result to 28 bits ( resolution of Reg inside metrology block)*/
    cal_data >>= 28;
    return (uint32_t)cal_data;     
}


void Stpm33_Calib (void)
{
    /* set current gain */
    /* write to 16 bit [31:16] of DFE_CR1, set bit 26 and 27, the other set as default */
    Stpm33_WriteRegister((STPM_DFE_CR_1_ADDRESS + 1) , 0x0327);
    
    /* calib CHV1 */
    Stpm33_WriteRegister(STPM_DSP_CR_5_ADDRESS, 0x0741);
    /* calib CHC1 */
    Stpm33_WriteRegister(STPM_DSP_CR_6_ADDRESS, 0x0985); 
}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/

uint8_t CRC_u8Checksum;
void Crc8Calc(uint8_t u8Data) {
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
	//while (!(SPI1->SR & (SPI_I2S_FLAG_RXNE)));
	// Wait until SPI is not busy anymore
	while (SPI1->SR & (SPI_I2S_FLAG_BSY));
	// Return received data from SPI data register
	return SPI1->DR;
    
}

void delay(uint32_t pTime)
{
	while(pTime--);
}

void Stpm33_WriteRegister (uint8_t pAddr, uint16_t pData)
{
    uint8_t vBuffer[5];
    uint8_t vBufferRecv[5];
    uint8_t i = 0;
    /* send reg addr and data to write */
    vBuffer[0] = 0x00; //dummy addr to read
    vBuffer[1] = pAddr;
    vBuffer[2] = pData & 0xFF;
    vBuffer[3] = (pData >> 8) & 0xFF;
    vBuffer[4] = CalcCRC8(vBuffer);
	Stpm33_Enable();
	delay(350);
	for (i = 0; i < 5; i++)
    {
        vBufferRecv[i] = Stpm33_Transfer(vBuffer[i]);
	}
	Stpm33_Disable();
    
//	uint8_t i = 0;
//	stmBufSend[0] = 0x00;
//	stmBufSend[1]	= pAddr;
//	stmBufSend[2]	= (pData & 0xFF);
//	stmBufSend[3]	= pData >> 8;
//	stmBufSend[4]	= CalcCRC8(stmBufSend);
//	Stpm33_Enable();
//	delay(350);
//	for (i = 0; i < 5; i++){
//			stmBufRecv[i] = Stpm33_Transfer(stmBufSend[i]);
//	}
//	Stpm33_Disable();
}
