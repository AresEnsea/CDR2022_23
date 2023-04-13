/**
 * @file lidar.h
 */
#ifndef _LIDAR_
#define _LIDAR_

#include "config.h"
#include "propulsion.h"
#include "serial.h"

extern uint8_t lidar_distances[16];
extern int lidar_timeSinceData[16];

extern uint8_t newDataset;
extern uint8_t trameStarted;
extern uint8_t NROI;
extern uint16_t trameIndex;
extern int tpsMesure;

extern uint16_t distanceList[300];
extern uint8_t captorList[300];
extern uint8_t roiList[300];
extern uint8_t maxMesure;

void lidar_initialize();

void lidar_updateDistance(int index, uint8_t dist);
int lidar_getDistance(Direction dir);

bool lidar_pathIsClear(Direction dir);
bool lidar_frontIsClear();
bool lidar_backIsClear();

void lidar_incrementTime(int ms);


void readTrame(void);
void readHeader(uint16_t trameIndex);
void readValue(uint16_t usedIndex,uint16_t trameIndex);
void trameStatus(void);
uint8_t setFrameIndex(uint16_t * trameIndex);

#endif
