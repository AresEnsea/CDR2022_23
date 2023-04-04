#ifndef SERVOAX12_H_
#define SERVOAX12_H_

#include "usart.h"
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

#define BR_1M 1
#define BR_500K 3
#define BR_400K 4
#define BR_250K 7
#define BR_200K 9
#define BR_115200 16
#define BR_57600 34
#define BR_19200 103
#define BR_9600 207

enum instruction{
	ping =1,
	read,
	write,
	reg_write,
	action,
	reset,
	reboot=8
};

enum regEEPROM{
	REG_number = 0,
	REG_firmware = 2,
	REG_ID,
	REG_baudRate,
	REG_delayTime,
	REG_CWAngle, // Clockwise
	REG_CCWAngle = 8, //Counter Clockwise
	REG_temp = 11,
	REG_minVolt,
	REG_maxVolt,
	REG_torque,
	REG_status = 16,
	REG_LED,
	REG_shutdown
};

enum regRAM{
	REG_torque_E_D = 24,
	REG_LED_E_D,
	REG_CWMargin,
	REG_CCWMargin,
	REG_CWSlop,
	REG_CCWSlop,
	REG_goalPosition,
	REG_limitSpeed=32,
	REG_torqueLimit=34,
	REG_presentPosition=36,
	REG_presentSpeed=38,
	REG_presentLoad=40,
	REG_presentVolt=42,
	REG_presentTemp,
	REG_registered,
	REG_moving=46,
	REG_lockEEPROM,
	REG_Punch
};

/**
 * @brief AX12 : Structure qui contiens l'adresse et le baudrate du module
 */
typedef struct AX12{
	UART_HandleTypeDef* AX_Uart;
	uint8_t m_ID;
	uint8_t m_BR;
} AX12;

/**
 * @brief Initialisation du module
 * @param uart : liaisons série utilisé pour communiquer avec le module
 * @param ID : adresse du module
 * @param BR : vitesse de communication du module
 * @return l'adresse de la structure
 */
void AX12_Init(AX12* ax12, UART_HandleTypeDef* uart, uint8_t ID, uint8_t BR);

/**
 * @brief Attribution d'une nouvelle adresse et baudrate au module
 * @param ax12 : structure AX12
 * @param ID : nouvelle ID du module
 * @param BR : nouvelle vitesse de communication
 */
void AX12_ReProg(AX12* ax12 ,uint8_t ID, uint8_t BR);

/**
 * @brief définis une échelle d'angle que peut attendre le module
 * @param ax12 : structure AX12
 * @param min : angle minimal 0° min
 * @param man : angle maximal 300° max
 */
void AX12_setRangeAngle(AX12* ax12,float min, float max);

/**
 * @brief permet d'activer le mode roue libre
 * @param ax12 : structure AX12
 */
void AX12_setWheelMode(AX12* ax12);

/**
 * @brief définis la température maximale
 * @param ax12 : structure AX12
 * @param temp : température maximal
 */
void AX12_setMaxTemp(AX12* ax12,uint8_t temp);

/**
 * @brief définis une échelle de tensions
 * @param ax12 : structure AX12
 * @param min : tensions minimales en V limité 6V
 * @param man : tensions maximales en V limité 12V
 */
void AX12_setRangeVolt(AX12* ax12,uint8_t min, uint8_t max);
/**
 * @brief définis le couple maximal du module
 * @param ax12 : structure AX12
 * @param max : couple maximale en pourcentage
 */
void AX12_setMaxTorque(AX12* ax12,float max);

/**
 * @brief activation du couple ou non
 * @param ax12 : structure AX12
 * @param state : TRUE activation du couple
 * 				  FALSE désactivation du couple
 */
void AX12_TorqueE_D(AX12* ax12, uint8_t state);

/**
 * @brief activation de la LED
 * @param ax12 : structure AX12
 * @param state : TRUE LED allumer
 * 				  FALSE LED eteinte
 */
void AX12_LED_O_N(AX12* ax12,uint8_t state);

/**
 * @brief récupérer le numéro de série du module
 * @param ax12 : structure AX12
 * @return le numéro de série
 */
uint16_t AX12_getModelNumber(AX12* ax12);

/**
 * @brief récupérer la version du Firmware du module
 * @param ax12 : structure AX12
 * @return la version du Firmware
 */
uint8_t AX12_getFirmwareVersion(AX12* ax12);

/**
 * @biref récupérer la posistion actuelle du servo
 * @param ax12 : structure AX12
 * @return la position du robot
 */
uint16_t AX12_getPosition(AX12* ax12);

/**
 * @brief Définir une position au robot
 * @param ax12 : structure AX12
 * @param angle : angle qu'on le souhaite attribuer au servo
 */
void AX12_setPosition(AX12* ax12, float angle);

/**
 * @brief Définir la vitesse de déplacement
 * @param ax12 : structure AX12
 * @param rpm : vitesse de déplacement du servo (compris entre 1-114 tr/min)
 */
void AX12_setMovingSpeed(AX12* ax12, float rpm);

/**
 * @brief bloquer les moddification sur EEPROM
 * @param ax12 : structure AX12
 * @param state : TRUE EEPROM non modifiable
 * 				  FALSE EEPROM modifiable
 */
void AX12_EEPROM_Lock(AX12* ax12 , uint8_t state);

/**
 * @brief Envoie une trame qui contient 1 octet
 * @param ax12 : structure AX12
 * @param data : la donnée que l'on souhaite envoyer
 */
void AX12_sendTram1Byte(AX12* ax12, uint8_t reg,uint8_t data);

/**
 * @brief Envoie une trame qui contient 2 octets
 * @param ax12 : structure AX12
 * @param data : la donnée que l'on souhaite envoyer
 */
void AX12_sendTram2Byte(AX12* ax12,uint8_t reg ,uint16_t data);

/**
 * @brief permet de lire un registre
 * @param ax12 : structure AX12
 * @param reg : registre que l'on souhaite lire
 * @return retourne l'octet lue
 */
uint8_t AX12_readTram1byte(AX12* ax12, uint8_t reg);

/**
 * @brief permet de lire un registre
 * @param ax12 : structure AX12
 * @param reg : registre que l'on souhaite lire
 * @return retourne les 2 octets lue
 */
uint16_t AX12_readTram2byte(AX12* ax12, uint8_t reg);

/**
 * @brief permet d'envoyer 1 octet
 * @param ax12 : structure AX12
 * @param data : la donnée que l'on souhaite envoyer
 */
void AX12_sendData(AX12* ax12 ,uint8_t size ,uint8_t instruction ,uint8_t nbParametre ,... );

/**
 * @brief permet de calculer le checksum
 * @param id : Identifiant du capteur
 * @param length : la taille de la trame
 * @param instruction : instruction que l'on souhaite utiliser
 * @param reg : registre que l'on souhaite utilisé
 * @param param1 : donnée
 * @param param2 : donnée égale à 0 si 1 octet attendu par la commande
 * @return le checksum
 */
uint8_t AX12_calculChecksum(uint8_t id, uint8_t length,uint8_t instruction,uint8_t reg,uint8_t param1, uint8_t param2);

/**
 * @brief permet de vérifier si le module est bien connecter
 * @param ax12 : structure AX12
 * @return TRUE : module bien connecter | FALSE : module non connecter
 */
int8_t AX12_pingModule(AX12* ax12);

/**
 * @brief permet de controler si il y a une erreur
 * @param data : la valeur retourné par la fonction readData
 * @return un octet qui qui décrit l'ensemble des erreurs
 *
 *			bit 7 : pas de module connecter
 * 			bit 6 : Erreur d'instruction
 * 			bit 5 : Charge trop importante
 * 			bit 4 : Mauvais calcul du checksum
 * 			bit 3 : Instruction inconnue
 * 			bit 2 : Température maximale atteinte
 * 		    bit 1 : Angle souhaité n'est pas compris dans l'intervalle
 * 			bit 0 : La tension n'est pas comprise dans l'intervalle
 */
uint8_t AX12_Id(AX12* ax12);
int8_t AX12_errorControl(uint8_t data);

#endif /* SERVOAX12_H_ */
