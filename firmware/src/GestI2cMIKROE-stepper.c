// GestI2cMIKROE-stepper.c
//
//	Description : Gestion par l'I2C des module mikro pour driver des moteurs
//	Auteur 		: C-Mieville
//      Création       : 11.09.2023
//      Modifications  :  --
//      Version        :  V1.0
//      
/*--------------------------------------------------------*/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "GestI2cMIKROE-stepper.h"
#include "peripheral\i2c\plib_i2c.h"
#include "peripheral\osc\plib_osc.h"


//---------------------------------------------------------------------------------	
// Fonction I2C_INITMOTEUR
// Description: Fonction permettant d'initialiser les pins necessaire à la gestion 
//              des moteurs
// Entrées: I2C_ID
// Sorties: -
void I2C_InitMoteur( uint32_t ID)  
{
   //Initialiser I2C en mode FAST
   bool Fast = true;
   i2c_init(Fast, ID);
   //configuration de du registre pour l'I2C
   I2C_WriteConfigPCA95(ID);
}


//---------------------------------------------------------------------------------	
// Fonction I2C_WriteConfigPCA95
// Description: Fonction permettant de pointer sur le registre output port
//              (Permet d'initialiser en etat de sortie les broches.)
// Entrées: I2C_ID
// Sorties: -

void I2C_WriteConfigPCA95( uint32_t ID)
{
    //Mise des sorties à leurs valeurs par défaut
    i2c_start(ID);//start   
    i2c_write(PCA9538A_wr ,ID);	// adresse + écriture 
    i2c_write(PCA9538A_pt_Output, ID);	// sélection ptr. output.
    //si on veux configurer le moteur M2
    if (ID == 0xBF805300)
    {
        i2c_write(PCA9538A_OutputInitValue_M2, ID);	// data to port
    }

    else
    {
        i2c_write(PCA9538A_OutputInitValue, ID);	// data to port
    }
    i2c_stop(ID);               //Stop
    
    //Toutes les IO en sorties
    i2c_start(ID);//start    
    i2c_write(PCA9538A_wr ,ID);	// adresse + écriture
    i2c_write(PCA9538A_pt_Config, ID);	// sélection ptr. output.
    i2c_write(PCA9538A_ConfigInitValue, ID);	// data to port
    i2c_stop(ID);               //Stop
            
} // I2C_WriteConfigPCA95

//---------------------------------------------------------------------------------	
// Fonction I2C_WriteGPIO_PCA95
// Description: Fonction permettant de definir l'etat des pin du PCA95 (Low,Hight) 
//              
// Entrées: I2C_ID, valeur du tableau (ex:Step_Positif), taille du tableau
// Sorties: -
void I2C_WriteGPIO_PCA95( uint32_t ID , uint8_t *Step, uint8_t Taille)
{
    int i;                      //initialiser la varible
    i2c_start(ID);              //Start
    i2c_write(PCA9538A_wr, ID);	// adresse + écriture
    i2c_write(PCA9538A_pt_Output, ID);	// sélection ptr. output.
    for ( i = 0; i < Taille; i++) //Ecrire chaque data du tableau
    {
        i2c_write(Step[i], ID); //ecrire les datas pour definir l'etat des pin  
    }    
    i2c_stop(ID);               //Stop
} //I2C_WriteGPIO_PCA95

//---------------------------------------------------------------------------------	
// Fonction I2C_WriteGPIO_PCA95
// Description: Fonction permettant de definir l'etat des pin du PCA95 (Low,Hight) 
//              
// Entrées: I2C_ID, Valeur des Pin du I/O experder souhaité
// Sorties: -
void I2C_Write_Data_PCA95( uint32_t ID , uint8_t Step)
{
    i2c_start(ID);              //Start
    i2c_write(PCA9538A_wr, ID);	// adresse + écriture
    i2c_write(PCA9538A_pt_Output, ID);	// sélection ptr. output.

    i2c_write(Step, ID); //ecrire les datas pour definir l'etat des pin 

    i2c_stop(ID);               //Stop
} //I2C_WriteGPIO_PCA95
 
//---------------------------------------------------------------------------------	
// Fonction ID_I2C_M
// Description: Fonction permettant de definir l'ID des I2C. 
//              
// Entrées: Indiquer le moteur qu'on souhaite piloter
// Sorties: ID de I2C selectionner
uint32_t ID_I2C_M(uint8_t Num_Mot)
{
    uint32_t ID_I2C;    //initialiser la varible
    switch(Num_Mot)
    {
        case 1:
            ID_I2C = I2C_M1;    //I2C ID souhaité correspond à celui du moteur 1 (SDA et SCL 5)
        break;
        
        case 2:
            ID_I2C = I2C_M2;    //I2C ID souhaité correspond à celui du moteur 2 (SDA et SCL 1)
        break;
        
        case 3:
            ID_I2C = I2C_M3;    //I2C ID souhaité correspond à celui du moteur 3 (SDA et SCL 4)
        break;
    }
    return ID_I2C; //retourner la valeur de ID
}

