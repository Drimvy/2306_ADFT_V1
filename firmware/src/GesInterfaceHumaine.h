#ifndef GesInterfaceHumaine_h
#define GesInterfaceHumaine_h

// GesInterfaceHumaine.h
// C. Mieville    11/09/2023
//
// Fonctions pour la gestion du choix de mode de fonctionnement du système
//
// Principe : Il est necessaire pour le choix du mode de fonctionnement des 
//            moteurs. Les leds clignotent quand le mode est pas encore selectonner,   
//            une fois le mode choisi(BTN_OK), la led reste allumé.
//            
//---------------------------------------------------------------------------


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "GesBouton.h"
#include "GesMoteur.h"


// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
#define Val_Compteur 50 //periode de 1s pour le clignottement des LED et un r.c. de 50% 

//Enumeration machine d'etat interface humaine
typedef enum
{
	/* Application's state machine's initial state. */
    STATE_SELECT_MODE_NORMAL,
    STATE_SELECT_AVANCE_RAPIDE,
    STATE_SELECT_AVANCE_LENTE,
    STATE_SELECT_RETOUR_LENT,
    STATE_SELECT_REMBOBINAGE, 

    STATE_MODE_NORMAL,
    STATE_MODE_AVANCE_RAPIDE,
    STATE_MODE_AVANCE_LENTE,
    STATE_MODE_RETOUR_LENT,
    STATE_MODE_REMBOBINAGE,

} STATES_MODE;

// *****************************************************************************
// *****************************************************************************
// Section: Prototypes
// *****************************************************************************
// *****************************************************************************
//---------------------------------------------------------------------------------	
// Fonction GEST_Mode:  Fonction contenant la machine d'état interface humaine
void GEST_Mode(void);

#endif
