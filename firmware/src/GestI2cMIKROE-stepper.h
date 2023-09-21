#ifndef GestI2cMIKROE_stepper_H
#define GestI2cMIKROE_stepper_H

// GestI2cMIKROE_stepper.h
// C. Mieville    11/09/2023
//
// Fonctions pour controler en I2C les module MIKRO
//
// Principe : Il permet d'envoyer des trames en I2C, pour faire avancer ou  
//            reculer les moteurs
//            
//---------------------------------------------------------------------------

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include "GesMoteur.h"
#include "Mc32_I2cUtilCCS.h"


// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
// Définition pour PCA9538A (composant permettant de gerer l'I2C pour controler les drivers)
//adresse resd/write
#define PCA9538A_rd      0xE1         // PCA9538A address for read
#define PCA9538A_wr      0xE0         // PCA9538A address for write
//Pointeur de registre
#define PCA9538A_pt_Input   0x00        // adr. registre Input port
#define PCA9538A_pt_Output  0x01        // adr. registre output port
#define PCA9538A_pt_Config  0x03        // adr. registre configuration port

#define PCA9538A_OutputInitValue 0x46   //Valeur de sortie des pin du I/O expender M1 et M3
#define PCA9538A_ConfigInitValue 0x00   //Valeur pour configurer toute les pin du I/O expender en sortie
#define PCA9538A_OutputInitValue_M2 0x02    //Valeur initialiser pour le mode fonctionnement M2 (Half step)

#define I2C_M1  I2C_ID_4 //ID 1
#define I2C_M2  I2C_ID_1 //ID 2
#define I2C_M3  I2C_ID_5 //ID 3

// *****************************************************************************
// *****************************************************************************
// Section: Prototypes
// *****************************************************************************
// *****************************************************************************
//---------------------------------------------------------------------------------	
// Fonction I2C_InitMoteur:  Fonction initalisant l'I2C
void I2C_InitMoteur(uint32_t ID);
//---------------------------------------------------------------------------------	
// Fonction I2C_WriteConfigPCA95:  Fonction pointant sur le registre qu'on souhaite utiliser
void I2C_WriteConfigPCA95(uint32_t ID);
//---------------------------------------------------------------------------------	
// Fonction I2C_WriteGPIO_PCA95:  Fonction permettant de definir l'etat des pins
void I2C_WriteGPIO_PCA95( uint32_t ID , uint8_t *Step, uint8_t Taille);
//---------------------------------------------------------------------------------	
// Fonction I2C_WriteGPIO_PCA95:  Fonction permettant de definir l'etat des pins (envoie que d'un octet)
void I2C_Write_Data_PCA95( uint32_t ID , uint8_t Step);

//---------------------------------------------------------------------------------	
// Fonction ID_I2C_M:  Fonction retournant la valeur de I2C ID
uint32_t ID_I2C_M(uint8_t Num_Mot);

#endif
// Fonction ID_I2C_M:  Fonction retournant la valeur de I2C ID
