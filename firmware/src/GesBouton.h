#ifndef GesBouton_h
#define GesBouton_h

// GesBouton.h
// C. Mieville    11/09/2023
//
// Fonctions pour la gestion des boutons
//
// Principe : Il est necessaire pour connaitre l'etat des boutons en évitant 
//            les erreur lié au rebond des  boutons
//            
//---------------------------------------------------------------------------

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include <stdint.h>


//structure de la valeur des boutons
typedef struct {
            uint8_t Inc : 1;             // événement incrément
            uint8_t Dec : 1;             // événement décrément
            uint8_t OK  : 1;             // événement action OK

} S_BTN_Descriptor;

// *****************************************************************************
// *****************************************************************************
// Section: Prototypes
// *****************************************************************************
// *****************************************************************************
//---------------------------------------------------------------------------------	
// Fonction ScanBTN: Lecture de la valeurs des boutons
void ScanBTN (bool BTN_Plus, bool BTN_Moins, bool BTN_OK);
//---------------------------------------------------------------------------------	
// Fonction BoutonInit: Initialisation de la valeur des boutons
void BoutonInit (void);
//---------------------------------------------------------------------------------	
// Fonction BoutonPlus: Indique la valeur du bouton incrémentation
bool BoutonPlus    (void);
//---------------------------------------------------------------------------------	
// Fonction BoutonMinus: Indique la valeur du bouton décrémentation
bool BoutonMinus    (void);
//---------------------------------------------------------------------------------	
// Fonction BoutonIsOK: Indique la valeur du bouton OK
bool BoutonIsOK    (void);
//---------------------------------------------------------------------------------	
// Fonction BoutonClearPlus: Clear valeur bouton incrémentation
void BoutonClearPlus   (void);
//---------------------------------------------------------------------------------	
// Fonction BoutonClearMinus: Clear valeur bouton décrémentation
void BoutonClearMinus   (void);
//---------------------------------------------------------------------------------	
// Fonction BoutonClearOK: Clear valeur bouton OK
void BoutonClearOK   (void);


#endif
