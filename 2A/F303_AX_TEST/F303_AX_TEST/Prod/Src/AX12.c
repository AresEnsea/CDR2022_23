#include <AX12.h>


void AX12_Init(AX12* ax12, UART_HandleTypeDef* uart, uint8_t ID, uint8_t BR){

	/******************************************
	 * Initialisation des paramètre du module *
	 ******************************************/

	ax12->AX_Uart = uart;
	ax12->m_ID = ID;
	ax12->m_BR = BR;

}


void AX12_ReProg(AX12* ax12 ,uint8_t ID, uint8_t BR){
	/***********************************
	 * Configuration de l'ID du module *
	 ***********************************/

	//Envoie de la requete d'écriture
	AX12_sendTram1Byte(ax12,REG_ID,ID);

	/**************************************
	 * Réception de la réponse du module  *
	 **************************************/
	HAL_HalfDuplex_EnableReceiver(ax12->AX_Uart);
	uint8_t data[SIZE_PING+1];
	if(HAL_UART_Receive(ax12->AX_Uart,data,SIZE_PING+1,500) == HAL_TIMEOUT){
		data[4] = 128;
	}
	HAL_HalfDuplex_EnableTransmitter(ax12->AX_Uart);

	/*************************************************************
	 * Nous vérifions que le module à bien enregistrer la donnée *
	 *************************************************************/
	if(data[5] == 0){
		ax12->m_ID=ID;
	}

	/****************************************
	 * Configuration du baud rate du module *
	 ****************************************/
	AX12_sendTram1Byte(ax12,REG_baudRate,BR);

	//Envoie de la requete d'écriture
	HAL_HalfDuplex_EnableReceiver(ax12->AX_Uart);

	/**************************************
	 * Réception de la réponse du module  *
	 **************************************/
	if(HAL_UART_Receive(ax12->AX_Uart,data,SIZE_PING,500) == HAL_TIMEOUT){
		data[4] = 128;
	}
	HAL_HalfDuplex_EnableTransmitter(ax12->AX_Uart);

	if(data[5] == 0){
		ax12->m_BR=BR;
	}
}


void AX12_setRangeAngle(AX12* ax12,float min, float max){
	uint16_t dataMin = min/STEP_ANGLE;
	uint16_t dataMax = max/STEP_ANGLE;
	AX12_sendTram2Byte(ax12,REG_CWAngle,dataMin);
	AX12_sendTram2Byte(ax12,REG_CCWAngle,dataMax);

}

void AX12_setWheelMode(AX12* ax12){
	AX12_sendTram2Byte(ax12,REG_CWAngle,0);
	AX12_sendTram2Byte(ax12,REG_CCWAngle,0);

}

void AX12_setMaxTemp(AX12* ax12,uint8_t temp){
	AX12_sendTram1Byte(ax12,REG_temp,temp);
}


void AX12_setRangeVolt(AX12* ax12,uint8_t min, uint8_t max){
	min*=10;
	max*=10;
	AX12_sendTram1Byte(ax12,REG_minVolt,min);
	AX12_sendTram1Byte(ax12,REG_maxVolt,max);
}


void AX12_setMaxTorque(AX12* ax12,float max){
	uint16_t data = (uint16_t)max/TORQUE_PERCENT;
	AX12_sendTram2Byte(ax12,REG_torque,data);
}


uint16_t AX12_getModelNumber(AX12* ax12){
	return AX12_readTram2byte(ax12,REG_number);
}


uint8_t AX12_getFirmwareVersion(AX12* ax12){
	return AX12_readTram1byte(ax12,REG_firmware);
}

uint16_t AX12_getPosition(AX12* ax12){
	return AX12_readTram2byte(ax12,REG_presentPosition);
}

void AX12_TorqueE_D(AX12* ax12, uint8_t state){
	AX12_sendTram1Byte(ax12,REG_torque_E_D,state);
}


void AX12_LED_O_N(AX12* ax12,uint8_t state){
	AX12_sendTram1Byte(ax12,REG_LED_E_D,state);
}


void AX12_setPosition(AX12* ax12, float angle){
	uint16_t data = (uint16_t)angle/STEP_ANGLE;
	AX12_sendTram2Byte(ax12,REG_goalPosition,data);
}


void AX12_setMovingSpeed(AX12* ax12, float rpm){
	uint16_t data = (uint16_t)rpm/STEP_SPEED;
	AX12_sendTram2Byte(ax12,REG_limitSpeed,data);
}


void AX12_EEPROM_Lock(AX12* ax12 , uint8_t state){
	AX12_sendTram1Byte(ax12,REG_lockEEPROM,state);
}



void AX12_sendData(AX12* ax12 ,uint8_t size ,uint8_t instruction ,uint8_t nbParametre ,... ){

	/****************************************
	 * Création d'une liste qui contien 	*
	 * tous les paramètre après nbParametre *
	 ****************************************/
	va_list ap;
	va_start(ap,nbParametre);

	/**************************
	 * Création de la requête *
	 **************************/
	uint8_t data[size];
	data[0] = 0xFF;
	data[1] = 0xFF;
	data[2] = ax12->m_ID;
	data[3] = 2+nbParametre;
	data[4] = instruction;

	/************************************
	 * nous implémentons les paramètres *
	 * (registre, paramètre, ...)	    *
	 ************************************/
	for(uint8_t i = 0; i < nbParametre;i++){
		data[5+i] = (uint8_t)va_arg(ap,int);
	}
	/****************************
	 * nous détruisons la liste *
	 ****************************/
	va_end(ap);

	/********************************
	 * nous vérifions quel checksum *
	 * nous allons calculer 		*
	 ********************************/
	switch(nbParametre){
	case(0):data[size-1] = AX12_calculChecksum(data[2],data[3],data[4],0,0,0);break;
	case(1):data[size-1] = AX12_calculChecksum(data[2],data[3],data[4],data[5],0,0);break;
	case(2):data[size-1] = AX12_calculChecksum(data[2],data[3],data[4],data[5],data[6],0);break;
	case(3):data[size-1] = AX12_calculChecksum(data[2],data[3],data[4],data[5],data[6],data[7]);break;
	default : break;
	}

	/************************
	 * envoie de la requête *
	 ************************/
	HAL_UART_Transmit(ax12->AX_Uart, (uint8_t*)&data, size, 500);
}


uint8_t AX12_calculChecksum(uint8_t id, uint8_t length,uint8_t instruction,uint8_t reg,uint8_t param1, uint8_t param2){
	uint16_t checksum = id + length + instruction + reg + param1 + param2;
	checksum=~checksum;
	checksum&=0xFF;
	return (uint8_t)checksum;
}

void AX12_sendTram1Byte(AX12* ax12, uint8_t reg,uint8_t data){
	AX12_sendData(ax12 ,SIZE_WRITE_8, write ,2,reg,data);
}

void AX12_sendTram2Byte(AX12* ax12,uint8_t reg ,uint16_t data){
	uint8_t data1 = (uint8_t)(data>>8);
	uint8_t data2 = (uint8_t)data&0xFF;
	AX12_sendData(ax12 ,SIZE_WRITE_16, write ,3,reg,data2,data1);
}

uint8_t AX12_readTram1byte(AX12* ax12, uint8_t reg){
	uint8_t data[SIZE_GET_READ_8];

	/***********************************
	 * envoie de la requête de lecture *
	 ***********************************/
	AX12_sendData(ax12 ,SIZE_REQUEST_READ_8, read ,2,reg,1);

	/*************************
	 * réception des données *
	 *************************/
	HAL_HalfDuplex_EnableReceiver(ax12->AX_Uart);
	HAL_UART_Receive(ax12->AX_Uart,data,SIZE_GET_READ_8,500);
	HAL_HalfDuplex_EnableTransmitter(ax12->AX_Uart);

	return data[5];
}

uint16_t AX12_readTram2byte(AX12* ax12, uint8_t reg){
	uint8_t data[SIZE_GET_READ_16];

	/***********************************
	 * envoie de la requête de lecture *
	 ***********************************/
	AX12_sendData(ax12 ,SIZE_REQUEST_READ_16, read ,2,reg,2);

	/*************************
	 * réception des données *
	 *************************/
	HAL_HalfDuplex_EnableReceiver(ax12->AX_Uart);
	HAL_UART_Receive(ax12->AX_Uart,data,SIZE_GET_READ_16,500);
	HAL_HalfDuplex_EnableTransmitter(ax12->AX_Uart);

	uint16_t dataReturn = (data[6]<<8) + data[5];
	return (dataReturn);
}

int8_t AX12_pingModule(AX12* ax12){
	uint8_t data[SIZE_PING];

	/********************************
	 * envoie de la requête de ping *
	 ********************************/
	AX12_sendData(ax12 ,SIZE_PING, ping ,0);

	/*************************
	 * réception des données *
	 *************************/
	HAL_HalfDuplex_EnableReceiver(ax12->AX_Uart);
	if(HAL_UART_Receive(ax12->AX_Uart,data,SIZE_PING,500) == HAL_TIMEOUT){
		data[4] = 128;
	}
	HAL_HalfDuplex_EnableTransmitter(ax12->AX_Uart);

	/*******************************************
	 * nous renvoyons l'erreur s'il y en a une *
	 *******************************************/
	return AX12_errorControl(data[4]);
}

int8_t AX12_errorControl(uint8_t err){
	uint8_t error = 0;
	for(uint8_t i = 0; i < 8;i++){
		switch(err&(1<<i)){
		case(1): error-=1;break;
		case(2): error-=2;break;
		case(4): error-=4;break;
		case(8): error-=8;break;
		case(16): error-=16;break;
		case(32): error-=32;break;
		case(64): error-=64;break;
		case(128): error-=128;break;
		default: break;
		}
	}
	return error;
}
