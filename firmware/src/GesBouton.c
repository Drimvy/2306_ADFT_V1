// GesBouton.c
//
//	Description : Gestion des boutons pour l'interface humaine
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
#include "GesBouton.h"
#include "Mc32Debounce.h"

//Declaration des variables de structure
// Descripteur des sinaux
S_SwitchDescriptor DescrBTN_Plus;
S_SwitchDescriptor DescrBTN_Moins;
S_SwitchDescriptor DescrBTN_OK;

// Structure pour le traitement des boutons
S_BTN_Descriptor Bouton;


//---------------------------------------------------------------------------------	
// Fonction BoutonInit
// Description: Fonction permettant d'initialiser la valeur des boutons à 0
// Entrées: -
// Sorties: -

void BoutonInit (void)
{
   // Initialisation des descripteurs de touches des boutons
   DebounceInit(&DescrBTN_Plus);
   DebounceInit(&DescrBTN_Moins);
   DebounceInit(&DescrBTN_OK);
   
   // Init de la structure Bouton
    Bouton.Inc = 0;             // événement incrément  
    Bouton.Dec = 0;             // événement décrément 
    Bouton.OK = 0;              // événement action OK 
 } // InitBouton

//---------------------------------------------------------------------------------	
// Fonction ScanBTN
// Description: Fonction permettant d'intiquer la pression d'un bouton
// Entrées: valeur sur la sortie des boutons  
// Sorties: -
void ScanBTN (bool BTN_Plus, bool BTN_Moins, bool BTN_OK)
{
   // Traitement antirebond sur les boutons
   DoDebounce (&DescrBTN_Plus, BTN_Plus);
   DoDebounce (&DescrBTN_Moins, BTN_Moins);
   DoDebounce (&DescrBTN_OK, BTN_OK);
   
   if(DebounceIsPressed(&DescrBTN_Plus))
   {      
        DebounceClearPressed(&DescrBTN_Plus);
        if(Bouton.Inc == false)
        {
            Bouton.Inc = true;
        }
        else
        {
            Bouton.Inc = false;
        }    
   }
   if(DebounceIsPressed(&DescrBTN_Moins))
   {      
        DebounceClearPressed(&DescrBTN_Moins);
        if(Bouton.Dec == false)
        {
            Bouton.Dec = true;
        }
        else
        {
            Bouton.Dec = false;
        }    
   }
   if(DebounceIsPressed(&DescrBTN_OK))
   {      
        DebounceClearPressed(&DescrBTN_OK);
        if(Bouton.OK == false)
        {
            Bouton.OK = true;
        }
        else
        {
            Bouton.OK = false;
        }    
   }  
}


//       BoutonPlus       true indique un nouveau incrément
bool BoutonPlus    (void) {
   return (Bouton.Inc);
}

//       BoutonMinus      true indique un nouveau décrément
bool BoutonMinus    (void) {
   return (Bouton.Dec);
}

//       BoutonIsOK         true indique action OK
bool BoutonIsOK    (void) {
   return (Bouton.OK);
}


//       BoutonClearPlus    annule indication d'incrément
void BoutonClearPlus   (void) {
   Bouton.Inc = 0;
}

//       BoutonClearMinus   annule indication de décrément
void BoutonClearMinus   (void) {
   Bouton.Dec = 0;
}

//       BoutonClearOK      annule indication action OK
void BoutonClearOK   (void) {
   Bouton.OK = 0;
}

