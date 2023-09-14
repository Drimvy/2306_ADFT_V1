// GesBouton.c
//
//	Description : Gestion des boutons pour l'interface humaine
//	Auteur 		: C-Mieville
//      Cr�ation       : 11.09.2023
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
// Description: Fonction permettant d'initialiser la valeur des boutons � 0
// Entr�es: -
// Sorties: -

void BoutonInit (void)
{
   // Initialisation des descripteurs de touches des boutons
   DebounceInit(&DescrBTN_Plus);
   DebounceInit(&DescrBTN_Moins);
   DebounceInit(&DescrBTN_OK);
   
   // Init de la structure Bouton
    Bouton.Inc = 0;             // �v�nement incr�ment  
    Bouton.Dec = 0;             // �v�nement d�cr�ment 
    Bouton.OK = 0;              // �v�nement action OK 
 } // InitBouton

//---------------------------------------------------------------------------------	
// Fonction ScanBTN
// Description: Fonction permettant d'intiquer la pression d'un bouton
// Entr�es: valeur sur la sortie des boutons  
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


//       BoutonPlus       true indique un nouveau incr�ment
bool BoutonPlus    (void) {
   return (Bouton.Inc);
}

//       BoutonMinus      true indique un nouveau d�cr�ment
bool BoutonMinus    (void) {
   return (Bouton.Dec);
}

//       BoutonIsOK         true indique action OK
bool BoutonIsOK    (void) {
   return (Bouton.OK);
}


//       BoutonClearPlus    annule indication d'incr�ment
void BoutonClearPlus   (void) {
   Bouton.Inc = 0;
}

//       BoutonClearMinus   annule indication de d�cr�ment
void BoutonClearMinus   (void) {
   Bouton.Dec = 0;
}

//       BoutonClearOK      annule indication action OK
void BoutonClearOK   (void) {
   Bouton.OK = 0;
}

