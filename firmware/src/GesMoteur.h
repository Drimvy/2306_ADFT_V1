#ifndef GesMoteur_h
#define GesMoteur_h

// GesMoteur.h
// C. Mieville    11/09/2023
//
// Fonctions pour la gestion des moteur
//
// Principe : Il controle les moteur en fonction du choix selectionné
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
#include "GestI2cMIKROE-stepper.h"
#include "driver/tmr/drv_tmr_static.h"
#include "peripheral/oc/plib_oc.h"
#include "Mc32Delays.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
#define RapportPWM_pc 50 //rapport cyclique en poucent pour l'intencité de la LED
#define MAX_Step_M2 40 //nombre de STEP max avant de devoir bouger M1
#define MAX_Step_M1 4 //nombre de STEP max avant de devoir bouger M2

//Déclatation de tableau externe
extern uint8_t Step_Positif[4];
extern uint8_t Step_Negatif[4];

//Enumeration machine d'etat Gestion des moteurs
typedef enum
{
	/* Application's state machine's initial state. */
    STATE_MOTEUR_M3, 
    STATE_MOTEUR_M1,
    STATE_MOTEUR_M2,
            
} STATES_MODE_Moteur;

// *****************************************************************************
// *****************************************************************************
// Section: Prototypes
// *****************************************************************************
// *****************************************************************************
//---------------------------------------------------------------------------------	
// Fonction INIT_MOTEUR:  Fonction initalisant les pin pour la gestion des moteurs
void INIT_MOTEUR(void);
//---------------------------------------------------------------------------------	
// Fonction Mode_Normal:  Fonction contenant la machine d'état mode normal
void Mode_Normal(void);
//---------------------------------------------------------------------------------	
// Fonction Mode_Avance_Rapide:  Fonction contenant la machine d'état mode avance rapide
void Mode_Avance_Rapide(void);
//---------------------------------------------------------------------------------	
// Fonction Mode_Avance_Lente:  Fonction contenant la machine d'état mode avance lente
void Mode_Avance_Lente(void);
//---------------------------------------------------------------------------------	
// Fonction Mode_Retour_Lent:  Fonction contenant la machine d'état interface humaine
void Mode_Retour_Lent(void);
//---------------------------------------------------------------------------------	
// Fonction Mode_Rembobinage:  Fonction contenant la machine d'état interface humaine
void Mode_Rembobinage(void);
//---------------------------------------------------------------------------------	
// Fonction OnOff_VentilETLED:  Fonction permettant d'allumer et etteindre la LED et le vetillateur
void OnOff_VentilETLED(bool Etat);

#endif
