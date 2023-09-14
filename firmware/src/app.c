/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/
//Variable de structure
APP_DATA appData;
//Flag indicant une interuption
bool Flag_interrupt = false;
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            //Initaliser la configuration des I2C des MIKROE
            I2C_InitMoteur(ID_I2C_M(Moteur_3));
            I2C_InitMoteur(ID_I2C_M(Moteur_2));
            I2C_InitMoteur(ID_I2C_M(Moteur_1));
            //initaliser les moteur (Pin Reset � 1)
            INIT_MOTEUR();			
            //initialisation des boutons
            BoutonInit();
 
            
            // Activer les timers 
            DRV_TMR0_Start();
            DRV_TMR1_Start();
            //changement d'�tat de la machine d'�tat
            APP_UpdateState( APP_STATE_WAIT);
            break;
        }
        
        case APP_STATE_WAIT :
        {
            //si il ya eu une interuption...
            if ( Flag_interrupt == true)
            {
                //mettre le flag interupt � false
                Flag_interrupt= false;
                //changer d'etat dans la machine d'�tat
                APP_UpdateState(APP_STATE_SERVICE_TASKS);
            }
        }
        break;

        case APP_STATE_SERVICE_TASKS:
        {
            //toggle la LED � chaque fois qui entre dans service task
            LED_TestToggle();
            //Fonction permettant le choix du mode de fonctionnement 
            //GEST_Mode();
            
            //envoie des data pour avancer d'un step
            I2C_WriteGPIO_UnData_PCA95( ID_I2C_M(Moteur_2) , 0x16);
            I2C_WriteGPIO_UnData_PCA95( ID_I2C_M(Moteur_2) , 0x14);
            I2C_WriteGPIO_UnData_PCA95( ID_I2C_M(Moteur_2) , 0x10);
            I2C_WriteGPIO_UnData_PCA95( ID_I2C_M(Moteur_2) , 0x12);
            //I2C_WriteGPIO_PCA95( ID_I2C_M(Moteur_3) , Step_Positif, 4);  
            //changement d'�tat de la machine d'�tat
            APP_UpdateState(APP_STATE_WAIT);
            break;
        }

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}
//---------------------------------------------------------------------------------	
// Fonction APP_UpdateState
// Description: Fonction permettant de mettre � jour la valeur l'�tat de la machine 
//              d'�tat principale.
// Entr�es: Nouevvele etat
// Sorties: -

void APP_UpdateState ( APP_STATES NewState )
{
    appData.state = NewState;
}


/*******************************************************************************
 End of File
 */
