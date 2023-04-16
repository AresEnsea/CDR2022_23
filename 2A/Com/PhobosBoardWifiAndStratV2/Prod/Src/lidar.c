#include "lidar.h"

int lidar_timeSinceData[16];
uint8_t lidar_distances[16];

#define TRAME_BOUND		0xFF
#define DEFAULT_ROI 13

uint8_t newDataset = 0;
uint8_t trameStarted = 0;
uint8_t NROI;
uint16_t trameIndex = 0;
int tpsMesure = 0;

uint16_t distanceList[300] = {0};
uint8_t captorList[300] = {0};
uint8_t roiList[300] = {0};
uint8_t maxMesure = 0;
uint8_t mesuresSent;


// Le lidar est sur l'UART 4

void lidar_initialize() {
	for (int i=0; i<16; i++) {
		lidar_distances[i] = 255;
		lidar_timeSinceData[i] = LIDAR_TIMEOUT;
	}
}

void lidar_updateDistance(int index, uint8_t dist) {
	/*lidar_distances[index] = dist;
	lidar_timeSinceData[index] = 0;*/
	//printf("%d -> %dcm\r\n", index, dist);

}

int lidar_getDistance(Direction dir) {
	int minDist = 1000;
	/*if (dir == FORWARD) {
		for (int i=4; i<=8; i++) {
			if (lidar_timeSinceData[i] <= LIDAR_TIMEOUT && lidar_distances[i] < minDist) {
				minDist = lidar_distances[i];
			}
		}
	} else {
		int i=0;
		if (lidar_timeSinceData[i] <= LIDAR_TIMEOUT && lidar_distances[i] < minDist) {
			minDist = lidar_distances[i];
		}
		for (i=12; i<=15; i++) {
			if (lidar_timeSinceData[i] <= LIDAR_TIMEOUT && lidar_distances[i] < minDist) {
				minDist = lidar_distances[i];
			}
		}
	}*/
	readTrame();

	for(int i = 0; i<maxMesure;i++){
		if((int)distanceList[i] < minDist){
			minDist = (int)distanceList[i];
		}
	}
	return minDist;
}

bool lidar_pathIsClear(Direction dir) {
	/*if (dir == FORWARD) {
		return lidar_frontIsClear();
	} else {
		return lidar_backIsClear();
	}*/
}

bool lidar_frontIsClear() {
	for (int i=4; i<=8; i++) {
		if (lidar_distances[i] < LIDAR_THRESHOLD && lidar_timeSinceData[i] <= LIDAR_TIMEOUT) {
			return false;
		}
	}
	return true;
}

bool lidar_backIsClear() {
	for (int i=12; i<=15; i++) {
		if (lidar_distances[i] < LIDAR_THRESHOLD && lidar_timeSinceData[i] <= LIDAR_TIMEOUT) {
			return false;
		}
	}
	if (lidar_distances[0] < LIDAR_THRESHOLD && lidar_timeSinceData[0] <= LIDAR_TIMEOUT) {
		return false;
	}
	return true;
}

void lidar_incrementTime(int ms) {
	for (int i=0; i<16; i++) {
		if (lidar_timeSinceData[i] <= LIDAR_TIMEOUT) {
			lidar_timeSinceData[i] += ms;
		}
	}
}


void readTrame(void){


	if(newDataset){
	uint16_t usedIndex = bufferIndex;
	uint16_t usedStart = trameIndex;

		if(setFrameIndex(&usedStart)){
			//HAL_UART_Transmit(&huart2, sideTrameText, 32, 100);
			readHeader(usedStart);
			//HAL_UART_Transmit(&huart2, middleTrameText, 32, 100);
			readValue(usedIndex, usedStart);
			//HAL_UART_Transmit(&huart2, sideTrameText, 32, 100);
		}
		newDataset = 0;

		/*Pour eviter de remplir le buffer en entier.
		 * Situe ici pour eviter de perdre la position de fin de trame lors de l'etape de lecture des mesures*/
		if(bufferIndex >2000){
			bufferIndex = 0;
		}
	}
}
void readHeader(uint16_t trameIndex){
	//uint8_t value[20];
		uint8_t ncaptActifs = buffer[trameIndex];

		//Pour eviter les erreurs de lecture de trame ne lisant pas le header
		if((ncaptActifs <= 0 || ncaptActifs >16) || (buffer[trameIndex+1]>14)){
			NROI = DEFAULT_ROI;
		}
		else{
			//NROI = buffer[trameIndex+1];
			NROI = DEFAULT_ROI; // Not the right value for debug purposes
		}

		mesuresSent = buffer[trameIndex+2];
		/*int size = sprintf((char*)value, "%d mesures recues\r\n", (int)mesuresSent);
		HAL_UART_Transmit(&huart2, &"Nous avons NcaptActifs = ", 25, 100);
		HAL_UART_Transmit(&huart2, &chiffres[ncaptActifs], 1, 100);
		HAL_UART_Transmit(&huart2, &", qui utlisent ", 15, 100);
		HAL_UART_Transmit(&huart2, &chiffres[NROI], 1, 100);
		HAL_UART_Transmit(&huart2, &" ROIs differents en \n\r", 22, 100);
		HAL_UART_Transmit(&huart2, value, strlen(value), 100);*/
}
void readValue(uint16_t usedIndex, uint16_t trameIndex){

	int i = trameIndex + 3;

	uint8_t Nmesure = 0;
	//uint8_t value[30];
	uint8_t Ncapteur; uint8_t indiceROI; uint16_t distance;
	while(i < (usedIndex - 3) && Nmesure < mesuresSent){
		//Selon le code du systeme de detection : "Pour que le premier element de la chaine de caracteres ne soit pas '\0'"
		// Donc il faut enlever l'offset sur buffer[i]
		indiceROI = (buffer[i]-1)%NROI;
		Ncapteur  = (buffer[i]-1 -indiceROI)/NROI;
		distance  = (buffer[i+1]<<8) + buffer[i+2];
		/*sprintf((char*)value, " , Distance \t\t = %d\r\n", (int)distance);

		HAL_UART_Transmit(&huart2, startContent, 8, 100);			//"Ncapt : "
		HAL_UART_Transmit(&huart2, &chiffres[Ncapteur], 1, 100);
		HAL_UART_Transmit(&huart2, middleContent, 10, 100);			//" , nROI : "
		HAL_UART_Transmit(&huart2, &chiffres[indiceROI], 1, 100);
		HAL_UART_Transmit(&huart2, value, strlen(value), 100);		//" , Distance \t\t = %d\r\n"
		*/
		captorList[Nmesure]=Ncapteur; roiList[Nmesure]=indiceROI; distanceList[Nmesure]=distance;

		i += 3;
		Nmesure++;
	}
	maxMesure = Nmesure;
}
/**
  * @brief Modifies the state of the incoming trame
  */
void trameStatus(void){
	//End of Trame
	if(trameStarted && (bufferIndex >= 3) && DataAcquiered){
		if((buffer[bufferIndex - 3] == TRAME_BOUND) && (buffer[bufferIndex - 2] == TRAME_BOUND) && (buffer[bufferIndex - 1] == TRAME_BOUND)){
			trameStarted = 0;

			newDataset = 1;
			/*bufferIndex = 0;
			buffer[0]=255;*/
			/*if(bufferIndex> 2000){
				bufferIndex = 0;
			}*/
		}
	}
	//Start of Trame
	else if(!trameStarted && (bufferIndex >= 3) && DataAcquiered){
		if((buffer[bufferIndex - 3] == TRAME_BOUND) && (buffer[bufferIndex - 2] == TRAME_BOUND) && (buffer[bufferIndex - 1] == TRAME_BOUND)){
			trameStarted = 1;
			trameIndex = bufferIndex;
			//trameIndex = 0;

		}
	}
	DataAcquiered = 0;
}

/**
  * @brief Adjust the value of the index stating the start of the frame
  * @return the success of the adjustment, 1:success 0:failure
  */
uint8_t setFrameIndex(uint16_t * trameIndex){
	uint16_t bound = *trameIndex;
	while(bound<20000){
		if((buffer[bound - 3] == TRAME_BOUND)	//0xFF
		&& (buffer[bound - 2] == TRAME_BOUND)	//0xFF
		&& (buffer[bound - 1] == TRAME_BOUND)	//0xFF
		&& (buffer[bound - 0] != TRAME_BOUND)){	//NcaptActifs

			*trameIndex = bound;
			return 1;
		}
		else{
			bound++;
		}
	}
	return 0;

}
