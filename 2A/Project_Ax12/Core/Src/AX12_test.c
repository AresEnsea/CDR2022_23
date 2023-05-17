#include "AX12_test.h"
uint8_t returnData[16];
uint16_t don;

void AX12_Init(AX12* ax12, UART_HandleTypeDef* uart, uint8_t ID, uint8_t BR)
{
	/******************************************
	 * Initialisation des paramètre du module *
	 ******************************************/
	ax12->AX_Uart = uart;
	ax12->m_ID = ID;
	ax12->m_BR = BR;
}

/*********************************************************
 * Création des fonctions pour saisir les registres 	 *
 *********************************************************/

/*
* Control Table of EEPROM Area *
*/
void AX12_setID(AX12* ax12, uint8_t id){
	AX12_sendTram1Byte(ax12, REG_ID, id);
}

void AX12_setBaudRate(AX12 *ax12, uint8_t BR){
	AX12_sendTram1Byte(ax12, REG_BaudRate, BR);
}

void AX12_setDelayTime(AX12* ax12, uint16_t Timeout){
	AX12_sendTram1Byte(ax12, REG_DelayTime, Timeout);
}

void AX12_setRangeAngle(AX12* ax12,float min, float max){
	uint16_t dataMin = min/STEP_ANGLE;
	uint16_t dataMax = max/STEP_ANGLE;
	AX12_sendTram2Byte(ax12, REG_CWAngle, dataMin);
	AX12_sendTram2Byte(ax12, REG_CCWAngle, dataMax);
}

void AX12_setTemp(AX12* ax12,uint8_t temp){
	AX12_sendTram1Byte(ax12, REG_temp, temp);
}

void AX12_setRangeVolt(AX12* ax12,uint8_t min, uint8_t max){
	min*=10;
	max*=10;
	AX12_sendTram1Byte(ax12, REG_minVolt, min);
	AX12_sendTram1Byte(ax12, REG_maxVolt, max);
}

void AX12_setMaxTorque(AX12* ax12,float max){
	uint16_t data = (uint16_t)max/TORQUE_PERCENT;
	AX12_sendTram2Byte(ax12, REG_MaxTorque, data);
}

void AX12_setStatus(AX12* ax12,uint8_t status){
	AX12_sendTram1Byte(ax12, REG_status, status);
}

void AX12_setAlarmLed(AX12* ax12,uint8_t status){
	AX12_sendTram1Byte(ax12, REG_AlarmLED, status);
}

void AX12_setShutdown(AX12* ax12,uint8_t status){
	AX12_sendTram1Byte(ax12, REG_shutdown, status);
}

/*
* Control Table of RAM Area *
*/

void AX12_setEnableTorque(AX12* ax12,uint8_t state){
	AX12_sendTram1Byte(ax12, REG_EnableTorque, state);
}

void AX12_setLED(AX12* ax12,uint8_t state){
	AX12_sendTram1Byte(ax12, REG_LED, state);
}

void AX12_setPosition(AX12* ax12, float angle){
	uint16_t data = (uint16_t)angle/STEP_ANGLE;
	AX12_sendTram2Byte(ax12, REG_goalPosition,data);
}

void AX12_setMovingSpeed(AX12* ax12, float rpm){
	uint16_t data = (uint16_t)rpm/STEP_SPEED;
	AX12_sendTram2Byte(ax12,REG_limitSpeed,data);
}

void AX12_setEEPROM_Lock(AX12* ax12 , uint8_t state){
	AX12_sendTram1Byte(ax12,REG_lockEEPROM,state);
}

/*********************************************************
 * Création des fonctions pour obtenir les registres 	 *
 *********************************************************/
uint16_t AX12_getModelNumber(AX12* ax12){
	AX12_SendTram(ax12, Read, 2, REG_ModelNumber, 2);
	return 1;
}

uint16_t AX12_getFirmware(AX12* ax12){
	AX12_SendTram(ax12, Read, 2, REG_Firmware, 1);
	return 1;
}

uint16_t AX12_getID(AX12* ax12){
	AX12_SendTram(ax12, Read, 2, REG_ID, 1);
	return 1;
}

uint16_t AX12_getBaudRate(AX12* ax12){
	AX12_SendTram(ax12, Read, 2, REG_BaudRate, 1);
	return 1;
}

uint16_t AX12_getDelayTime(AX12* ax12){
	AX12_SendTram(ax12, Read, 2, REG_DelayTime, 1);
	return 1;
}

uint16_t AX12_getPosition(AX12* ax12){
	don = AX12_SendTram(ax12, Read, 2, REG_presentPosition, 2);
	uint16_t angle = don * STEP_ANGLE;
	return angle;
}

uint16_t AX12_getSpeed(AX12* ax12){
	AX12_SendTram(ax12, Read, 2, REG_presentSpeed, 2);
	return 1;
}

uint16_t AX12_getLoad(AX12* ax12){
	AX12_SendTram(ax12, Read, 2, REG_presentLoad, 2);
	return 1;
}

uint16_t AX12_getVolt(AX12* ax12){
	AX12_SendTram(ax12, Read, 2, REG_presentVolt, 1);
	return 1;
}

uint16_t AX12_getTemp(AX12* ax12){
	AX12_SendTram(ax12, Read, 2, REG_presentTemp, 1);
	return 1;
}

/**********************************************
 * Création des fonctions pour lire et écrire *
 **********************************************/
/* Envoyer la Trame de 1 Byte*/
void AX12_sendTram1Byte(AX12* ax12, uint8_t reg,uint8_t data){
	AX12_SendTram(ax12 ,Write ,2 ,reg ,data);
}


/* Envoyer la Trame de 2 Byte*/
void AX12_sendTram2Byte(AX12* ax12,uint8_t reg ,uint16_t data){
	uint8_t data1 = (uint8_t)(data>>8);
	uint8_t data2 = (uint8_t)data&0xFF;
	don = AX12_SendTram(ax12 ,Write ,3 ,reg ,data2 ,data1);
}


/* Création et Envoye de la Trame */
uint16_t AX12_SendTram(AX12* ax12, uint8_t instruction, uint8_t nbParametre, ...)
{
	/****************************************
	 * Création d'une liste qui contien 	*
	 * tous les paramètre après nbParametre *
	 ****************************************/
	int pdeux;
	int pun;
	va_list Parametre;
	va_start(Parametre, nbParametre);
	uint8_t size = 6 + nbParametre;

	/**************************
	 * Création de la requête *
	 **************************/
	uint8_t data[8];
	data[0] = 0xFF;
	data[1] = 0xFF;
	data[2] = ax12->m_ID;
	data[3] = nbParametre + 2;
	data[4] = instruction;

	/************************************
	 * nous implémentons les paramètres *
	 * (registre, paramètre, ...)	    *
	 ************************************/
	for(uint8_t i = 0; i < nbParametre;i++){
		data[5+i] = (uint8_t)va_arg(Parametre,int);}

	/****************************
	 * nous détruisons la liste *
	 ****************************/
	va_end(Parametre);
	data[size - 1] = AX12_checkSum(data, nbParametre);

	/************************
	 * envoie de la requête *
	 ************************/
	HAL_UART_Transmit(ax12->AX_Uart, (uint8_t*)&data, size, HAL_MAX_DELAY);
	//HAL_Delay(100);
	//HAL_UART_Receive_DMA(ax12->AX_Uart, returnData, 16);
	if(instruction == Read)
	{
		uint8_t longueurData = 6+data[6];
		uint16_t finalData;
		//uint8_t returnData[longueurData];
		HAL_UART_Receive_DMA(ax12->AX_Uart, returnData, 20);
		for(int i = 0; i < longueurData; i++)
		{
			if(returnData[i] == 0xFF && returnData[i+1] == 0xFF && returnData[i+2] == ax12->m_ID && returnData[i+4] == 0x00) // returnData[i+3] == data[size-2]+2
			{
				pdeux = i + 5;
				pun = i + 6;
				finalData = (returnData[pun]<<8) + (returnData[pdeux]);
			}
		}
		return finalData;
	}
}

uint8_t AX12_checkSum(uint8_t* data, uint8_t nbParametre)
{
	uint16_t checksum = 0;
	for(uint8_t i = 2; i <= 4 + nbParametre;i++)
	{
		checksum += data[i];
	}
	checksum=~checksum;
	checksum&=0xFF;
	return (uint8_t)checksum;
}
