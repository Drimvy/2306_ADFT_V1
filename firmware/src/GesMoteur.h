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
#include "Mc32Delays.h"
#include "driver/tmr/drv_tmr_static.h"
#include "peripheral/oc/plib_oc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
#define RapCyclPWM_pc 100 //rapport cyclique en poucent pour controler l'intencité de la LED
#define MAX_Step_M2 250 //nombre de STEP max pour M2 avant de devoir faire tourner M1
#define MAX_Step_M1 1 //nombre de STEP max pour M1 avant de devoir bouger M2
#define NBR_ETAPE_STEP_M3 16 //defini le nomre d'etape avant que le moteur réalise pour faire un step
#define NBR_ETAPE_STEP_M2 8 //defini le nomre d'etape avant que le moteur réalise pour faire un step
#define NBR_ETAPE_STEP_M1 16 //defini le nomre d'etape avant que le moteur réalise pour faire un step

//Déclatation de tableau externe
//tableau pour réaliser des STEP
extern uint8_t Step_Positif[4];
//tableau pour réaliser des Half STEP
extern uint8_t Half_Step_Positif[8];
//tableau pour réaliser des Quarter STEP
extern uint8_t Quarter_Step_Positif[16];

//Enumeration: machine d'etat Gestion des moteurs
typedef enum
{
	/* Application's state machine's initial state. */
    STATE_MOTEUR_M3, 
    STATE_MOTEUR_M1,
    STATE_MOTEUR_M2,
            
} STATES_MODE_Moteur;

//Structure permettant de compter à quel étape sont les moteurs sont pour faire un step
typedef struct {
            uint8_t M1;             // moteur 1
            uint8_t M2;             // moteur 2
            uint8_t M3;             // Moteur 3

} S_VAL_STEP;

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
//---------------------------------------------------------------------------------	
// Fonction Half_Step_M3: Fonction permettant la configuration des pin pour avancer en half step pour M3
void Half_Step_M3(uint8_t Numero_StepM3);
//---------------------------------------------------------------------------------	
// Fonction Quarter_Step_M3: Fonction permettant la configuration des pin pour avancer en quarter step pour M3
void Quarter_Step_M3(uint8_t Numero_StepM3);
//---------------------------------------------------------------------------------	
// Fonction Half_Step_M1: Fonction permettant la configuration des pin pour avancer en half step pour M1
void Half_Step_M1(uint8_t Numero_StepM1);
//---------------------------------------------------------------------------------	
// Fonction Quarter_Step_M1: Fonction permettant la configuration des pin pour avancer en quarter step pour M1
void Quarter_Step_M1(uint8_t Numero_StepM1);
#endif
