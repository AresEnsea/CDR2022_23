/**
 * @file config.h
 * Ce fichier regroupe un grand nombre de constantes utilisées à diverses 
 * endroits dans le projet. Certaines permettent de configuer le fonctionnement
 * du robot, d'autres l'activation ou non des affichages de debogage dans la 
 * console via un port USB serie.
 */
#include <math.h>

typedef int bool;
#define true 1
#define false 0


/******************************* Robot Select ********************************/
//#define ISPHOBOS
#define ISDEIMOS


/**************************** Config Age of bots *****************************/
#define MATCH_DURATION 100


/***************************** Config propulsion *****************************/
#define DEBUG_PROPULSION(format, args...) printf(format, ##args)
//#define DEBUG_PROPULSION(format, args...) HAL_Delay(20);

#define STEPS_PER_REV       200
#define MICROSTEPS          32

#ifdef ISPHOBOS
	#define WHEEL_RADIUS    38.6
	#define ENTRAXE_MOTOR   120.5
    #define MAX_CORRECTION  40.0
	#define MAX_MOTOR_SPEED 400
#endif
#ifdef ISDEIMOS
	#define WHEEL_RADIUS    27.15
	#define ENTRAXE_MOTOR   112.32
	#define MAX_CORRECTION  80.0
	#define MAX_MOTOR_SPEED 800
#endif


/****************************** Config odometry ******************************/
//#define DEBUG_ODOMETRY(format, args...) printf(format, ##args)
#define DEBUG_ODOMETRY(format, args...) {;}

#define TICKS_PER_REV 8192

#ifdef ISPHOBOS
	#define ODOMETRY_RADIUS  26.2255
	#define ENTRAXE_ODOMETRY 200.165586
#endif
#ifdef ISDEIMOS
	#define ODOMETRY_RADIUS  26.9811
//27.245
	#define ENTRAXE_ODOMETRY 179.68306
#endif


/*********************************** Lidar ***********************************/

#define LIDAR_THRESHOLD 40
#define LIDAR_MAX_DISTANCE 60
#define LIDAR_TIMEOUT 1000

/******************************** Timer names ********************************/
#define R_ENCODER htim1
#define L_ENCODER htim3
#define R_MOTOR_TIMER htim8
#define L_MOTOR_TIMER htim11
