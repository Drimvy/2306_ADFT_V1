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
// Description: Fonction permettant d'indiquer la pression d'un bouton
// Entrées: valeur sur la sortie des boutons  
// Sorties: -
void ScanBTN (bool BTN_Plus, bool BTN_Moins, bool BTN_OK, bool SW_UP, bool SW_Down)
{
   // Traitement antirebond sur les boutons
   DoDebounce (&DescrBTN_Plus, BTN_Plus);
   DoDebounce (&DescrBTN_Moins, BTN_Moins);
   DoDebounce (&DescrBTN_OK, BTN_OK);

   //Si le boutons plus est presser
   if(DebounceIsPressed(&DescrBTN_Plus))
   {      
       //clear l'indication bouton appuyer
        DebounceClearPressed(&DescrBTN_Plus);
        //si le bouton == 0
        if(Bouton.Inc == false)
        {
            //mettre à 1 la valeur du bouton
            Bouton.Inc = true;
        }
        else
        {
            //maintenir à 0 la valeur du bouton
            Bouton.Inc = false;
        }    
   }
   //Si le boutons moins est pressé
   if(DebounceIsPressed(&DescrBTN_Moins))
   {      
       //clear l'indication bouton appuyer
        DebounceClearPressed(&DescrBTN_Moins);
        //si le bouton == 0
        if(Bouton.Dec == false)
        {
            //mettre à 1 la valeur du bouton
            Bouton.Dec = true;
        }
        else
        {
            //maintenir à 0 la valeur du bouton
            Bouton.Dec = false;
        }    
   }
   //Si le boutons ok est pressé
   if(DebounceIsPressed(&DescrBTN_OK))
   { 
       //clear l'indication bouton appuyer
        DebounceClearPressed(&DescrBTN_OK);
        //si le bouton == 0
        if(Bouton.OK == false)
        {
            //mettre à 1 la valeur du bouton
            Bouton.OK = true;
        }
        else
        {
            //maintenir à 0 la valeur du bouton
            Bouton.OK = false;
        }    
   }
   
}

//---------------------------------------------------------------------------------	
// Fonction BoutonPlus
// Description: Fonction permettant d'indiquer la valeur du bouton
// Entrées: - 
// Sorties: Valeur du bouton
bool BoutonPlus    (void) {
   return (Bouton.Inc);
}

//---------------------------------------------------------------------------------	
// Fonction BoutonMinus
// Description: Fonction permettant d'indiquer la valeur du bouton
// Entrées: - 
// Sorties: Valeur du bouton
bool BoutonMinus    (void) {
   return (Bouton.Dec);
}

//---------------------------------------------------------------------------------	
// Fonction BoutonIsOK
// Description: Fonction permettant d'indiquer la valeur du bouton
// Entrées: - 
// Sorties: Valeur du bouton
bool BoutonIsOK    (void) {
   return (Bouton.OK);
}


//       BoutonClearPlus    annule indication d'incrément
//---------------------------------------------------------------------------------	
// Fonction BoutonClearPlus
// Description: Fonction permettant pour BoutonClearPlus d'annuler indication 
//              indication de d'incrémenter
// Entrées: - 
// Sorties: -
void BoutonClearPlus   (void) {
   Bouton.Inc = 0;
}


//---------------------------------------------------------------------------------	
// Fonction BoutonClearMinus
// Description: Fonction permettant pour BoutonClearMinus d'annuler indication 
//              indication de décrément
// Entrées: - 
// Sorties: -
void BoutonClearMinus   (void) {
   Bouton.Dec = 0;
}

//---------------------------------------------------------------------------------	
// Fonction BoutonClearOK
// Description: Fonction permettant pour BoutonClearOK d'annuler indication action OK
// Entrées: - 
// Sorties: -
void BoutonClearOK   (void) {
   Bouton.OK = 0;
}

