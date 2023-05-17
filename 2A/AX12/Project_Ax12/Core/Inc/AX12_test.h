#ifndef INC_AX12_TEST_H_
#define INC_AX12_TEST_H_

#include "main.h"
#include <stdlib.h>
#include <stdarg.h>

#define TRUE 1
#define FALSE 0

#define STEP_ANGLE 0.293255132
#define TORQUE_PERCENT 0.09775171065
#define STEP_SPEED 0.1114369501

#define SIZE_WRITE_8 8
#define SIZE_WRITE_16 9
#define SIZE_REQUEST_READ_8 8
#define SIZE_REQUEST_READ_16 8
#define SIZE_GET_READ_8 7
#define SIZE_GET_READ_16 8
#define SIZE_REQUEST_PING 6
#define SIZE_PING 6

#define BR_1M 		1
#define BR_500K 	3
#define BR_400K 	4
#define BR_250K 	7
#define BR_200K 	9
#define BR_115200 	16
#define BR_57600 	34
#define BR_19200 	103
#define BR_9600 	207

enum instruction{
	Ping 			= 1,	//0x01
	Read			= 2,	//0x02
	Write 			= 3,	//0x03
	Reg_Write		= 4,	//0x04
	Action			= 5,	//0x05
	Factory_Reset 	= 6,	//0x06
	Reboot			= 8,	//0x08
	Sync_Write		= 131,	//0x83
	Bulk_Read		= 146	//0x92
};

enum regEEPROM{
	REG_ModelNumber = 0,		//2Byte	R
	REG_Firmware 	= 2,	//1Byte		R
	REG_ID 			= 3,	//1Byte		RW
	REG_BaudRate 	= 4,	//1Byte		RW
	REG_DelayTime 	= 5,	//1Byte		RW
	REG_CWAngle 	= 6, 		//2Byte	RW
	REG_CCWAngle 	= 8, 		//2Byte	RW
	REG_temp 		= 11,	//1Byte		RW
	REG_minVolt		= 12,	//1Byte		RW
	REG_maxVolt		= 13,	//1Byte		RW
	REG_MaxTorque 	= 14,		//2Byte	RW
	REG_status 		= 16,	//1Byte		RW
	REG_AlarmLED	= 17,	//1Byte		RW
	REG_shutdown	= 18	//1Byte		RW
};

enum regRAM{
	REG_EnableTorque 	= 24,	//1Byte		RW
	REG_LED				= 25,	//1Byte		RW
	REG_CWMargin		= 26,	//1Byte		RW
	REG_CCWMargin		= 27,	//1Byte		RW
	REG_CWSlop			= 28,	//1Byte		RW
	REG_CCWSlop 		= 29,	//1Byte		RW
	REG_goalPosition	= 30,		//2Byte	RW
	REG_limitSpeed		= 32,		//2Byte	RW
	REG_torqueLimit		= 34,		//2Byte	RW
	REG_presentPosition	= 36,		//2Byte	R
	REG_presentSpeed	= 38,		//2Byte	R
	REG_presentLoad		= 40,		//2Byte	R
	REG_presentVolt		= 42,	//1Byte		R
	REG_presentTemp		= 43,	//1Byte		R
	REG_registered		= 44,	//1Byte		R
	REG_moving			= 46,	//1Byte		R
	REG_lockEEPROM		= 47,	//1Byte		RW
	REG_Punch 			= 48		//2Byte	RW
};

/**
 * @brief AX12 : Structure qui contiens l'adresse et le baudrate du module
 */
typedef struct AX12{
	UART_HandleTypeDef* AX_Uart;
	uint8_t m_ID;
	uint8_t m_BR;
} AX12;

void AX12_Init(AX12* ax12, UART_HandleTypeDef* uart, uint8_t ID, uint8_t BR);

/* Set Register*/
void AX12_setID(AX12* ax12, uint8_t id);

void AX12_setBaudRate(AX12 *ax12, uint8_t BR);

void AX12_setDelayTime(AX12* ax12, uint16_t Timeout);

void AX12_setRangeAngle(AX12* ax12,float min, float max);

void AX12_setTemp(AX12* ax12,uint8_t temp);

void AX12_setRangeVolt(AX12* ax12,uint8_t min, uint8_t max);

void AX12_setMaxTorque(AX12* ax12,float max);

void AX12_setStatus(AX12* ax12,uint8_t status);

void AX12_setAlarmLed(AX12* ax12,uint8_t status);

void AX12_setShutdown(AX12* ax12,uint8_t status);

void AX12_setLED(AX12* ax12,uint8_t state);

void AX12_setPosition(AX12* ax12, float angle);

void AX12_setMovingSpeed(AX12* ax12, float rpm);

void AX12_setEEPROM_Lock(AX12* ax12 , uint8_t state);

/* Read Register*/
uint16_t AX12_getModelNumber(AX12* ax12);

uint16_t AX12_getFirmware(AX12* ax12);

uint16_t AX12_getID(AX12* ax12);

uint16_t AX12_getBaudRate(AX12* ax12);

uint16_t AX12_getDelayTime(AX12* ax12);

uint16_t AX12_getPosition(AX12* ax12);


/**********************************************
 * Création des fonctions pour lire et écrire *
 **********************************************/

void AX12_sendTram1Byte(AX12* ax12, uint8_t reg,uint8_t data);

void AX12_sendTram2Byte(AX12* ax12,uint8_t reg ,uint16_t data);

uint16_t AX12_SendTram(AX12* ax12, uint8_t instruction, uint8_t nbParametre, ...);

uint8_t AX12_checkSum(uint8_t data[], uint8_t nbParametre);

uint8_t AX12_calculChecksum(uint8_t id, uint8_t length,uint8_t instruction,uint8_t reg,uint8_t param1, uint8_t param2);


#endif /* INC_AX12_TEST_H_ */
