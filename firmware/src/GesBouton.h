#ifndef GesBouton_h
#define GesBouton_h

// GesBouton.h
// C. Mieville    11/09/2023
//
// Fonctions pour la gestion des boutons
//
// Principe : Il est necessaire pour connaitre l'etat des boutons en �vitant 
//            les erreur li� au rebond des  boutons
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
            uint8_t Inc : 1;             // �v�nement incr�ment
            uint8_t Dec : 1;             // �v�nement d�cr�ment
            uint8_t OK  : 1;             // �v�nement action OK

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
// Fonction BoutonPlus: Indique la valeur du bouton incr�mentation
bool BoutonPlus    (void);
//---------------------------------------------------------------------------------	
// Fonction BoutonMinus: Indique la valeur du bouton d�cr�mentation
bool BoutonMinus    (void);
//---------------------------------------------------------------------------------	
// Fonction BoutonIsOK: Indique la valeur du bouton OK
bool BoutonIsOK    (void);
//---------------------------------------------------------------------------------	
// Fonction BoutonClearPlus: Clear valeur bouton incr�mentation
void BoutonClearPlus   (void);
//---------------------------------------------------------------------------------	
// Fonction BoutonClearMinus: Clear valeur bouton d�cr�mentation
void BoutonClearMinus   (void);
//---------------------------------------------------------------------------------	
// Fonction BoutonClearOK: Clear valeur bouton OK
void BoutonClearOK   (void);


#endif
