// GesInterfaceHumaine.c
//
//	Description : Gestion du choix du mode de fonctionnement du système
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
#include "GesInterfaceHumaine.h"

//---------------------------------------------------------------------------------	
// Fonction GEST_Mode
// Description: Fonction permettant le choix du mode de fonctionnement du système
//        STATE_SELECT_MODE_XXX--> permete de choisire le mode 
//        STATE_MODE_XXX--> permet d'entrer dans le mode selectionner, les moteur 
//                          vont commencer à tourner
// Entrées: -
// Sorties: -
void GEST_Mode(void)
{
    //Déclatation de varable local statique
    //Initaliser la machine d'état au mode séléction normal
    static STATES_MODE valSTATES_Mode = STATE_SELECT_MODE_NORMAL;
    //Intitialiser le compteur pour le clignottement de la LED à 0
    static uint16_t Compteur_Clignot = 0;
    
    switch(valSTATES_Mode)
    {
        case STATE_SELECT_MODE_NORMAL:
            //Si le compteur = 0
            if (Compteur_Clignot == 0)
            {
                //Toggle la LED mode normal 
                LED_NormalToggle();
                //mettre à jour la valeur du compteur
                Compteur_Clignot = Val_Compteur ;
            }
            else
            {
                //incrémenter la valeur du compteur
                Compteur_Clignot--;
            }
            //Si un des boutons est actif
            if (BoutonPlus ()|| BoutonMinus ()||BoutonIsOK ())
            {
                //Si le bouton + est appuyer
                if (BoutonPlus ())
                {
                    //changer de mode (avanace rapide)
                    valSTATES_Mode = STATE_SELECT_AVANCE_RAPIDE;
                }
                //Si le bouton - est appuyer
                if (BoutonMinus ())
                {
                    //changer de mode (rembobinage)
                    valSTATES_Mode = STATE_SELECT_REMBOBINAGE;
                }
                if (BoutonIsOK ())
                {
                    //selectionner Mode normal
                    valSTATES_Mode = STATE_MODE_NORMAL;
                }
                //éteindre la LED
                LED_NormalOff();
            } 
        break;
        
        case STATE_SELECT_AVANCE_RAPIDE:
            //Si le compteur = 0
            if (Compteur_Clignot == 0)
            {
                //Toggle la LED mode avance rapide
                LED_ARToggle();
                //mettre à jour la valeur du compteur
                Compteur_Clignot = Val_Compteur;
            }
            else
            {
                //incrémenter la valeur du compteur
                Compteur_Clignot--;
            }
            //Si un des boutons est actif
            if (BoutonPlus ()|| BoutonMinus ()||BoutonIsOK ())
            {
                //Si le bouton + est appuyer
                if (BoutonPlus ())
                {
                    //changer de mode (Avance lente)
                    valSTATES_Mode = STATE_SELECT_AVANCE_LENTE;
                }
                //Si le bouton - est appuyer
                if (BoutonMinus ())
                {
                    //changer de mode(Normal)
                    valSTATES_Mode = STATE_SELECT_MODE_NORMAL;
                }
                //Si le bouton OK est appuyer
                if (BoutonIsOK ())
                {
                    //selectionner Mode avance rapide
                    valSTATES_Mode = STATE_MODE_AVANCE_RAPIDE;
                }
                //éteindre la LED
                LED_AROff();
            }            
        break;
        
        case STATE_SELECT_AVANCE_LENTE:
            //Si le compteur = 0
            if (Compteur_Clignot == 0)
            {
                //Toggle la LED avance lente
                LED_ALToggle();
                //mettre à jour la valeur du compteur
                Compteur_Clignot = Val_Compteur;
            }
            else
            {
                //incrémenter la valeur du compteur
                Compteur_Clignot--;
            }
            //Si un des boutons est actif
            if (BoutonPlus ()|| BoutonMinus ()||BoutonIsOK ())
            {
                //Si le bouton + est appuyer
                if (BoutonPlus ())
                {
                    //changer de mode (Retour lent)
                    valSTATES_Mode = STATE_SELECT_RETOUR_LENT;
                }
                //Si le bouton - est appuyer
                if (BoutonMinus ())
                {
                    //changer de mode (Avance rapide)
                    valSTATES_Mode = STATE_SELECT_AVANCE_RAPIDE;
                }
                //Si le bouton OK est appuyer
                if (BoutonIsOK ())
                {
                    //selectionner avance lente 
                    valSTATES_Mode = STATE_MODE_AVANCE_LENTE;
                }
                //éteindre la LED
                LED_ALOff();
            }
        break;
        
        case STATE_SELECT_RETOUR_LENT:
            //Si le compteur = 0
            if (Compteur_Clignot == 0)
            {
                //Toggle la LED 
                LED_RLToggle();
                //mettre à jour la valeur du compteur
                Compteur_Clignot = Val_Compteur;
            }
            else
            {
                //incrémenter la valeur du compteur
                Compteur_Clignot--;
            }
            //Si un des boutons est actif
            if (BoutonPlus ()|| BoutonMinus ()||BoutonIsOK ())
            {
                //Si le bouton + est appuyer
                if (BoutonPlus ())
                {
                    //changer de mode (Rembobinage)
                    valSTATES_Mode = STATE_SELECT_REMBOBINAGE;
                }
                //Si le bouton - est appuyer
                if (BoutonMinus ())
                {
                    //changer de mode(Avance rapide)
                    valSTATES_Mode = STATE_SELECT_AVANCE_LENTE;
                }
                //Si le bouton OK est appuyer
                if (BoutonIsOK ())
                {
                    //selectionner retour lent
                    valSTATES_Mode = STATE_MODE_RETOUR_LENT;
                }
                //éteindre la LED
                LED_RLOff();
            }
        break;
        
        case STATE_SELECT_REMBOBINAGE:
            //Si le compteur = 0
            if (Compteur_Clignot == 0)
            {
                //Toggle la LED
                LED_RembobinageToggle();
                //mettre à jour la valeur du compteur
                Compteur_Clignot = Val_Compteur;
            }
            else
            {
                //incrémenter la valeur du compteur
                Compteur_Clignot--;
            }
            //Si un des boutons est actif
            if (BoutonPlus ()|| BoutonMinus ()||BoutonIsOK ())
            {
                //Si le bouton + est appuyer
                if (BoutonPlus ())
                {
                    //changer de mode (mode normal)
                    valSTATES_Mode = STATE_SELECT_MODE_NORMAL;
                }
                //Si le bouton - est appuyer
                if (BoutonMinus ())
                {
                    //changer de mode (Retour lent)
                    valSTATES_Mode = STATE_SELECT_RETOUR_LENT;
                }
                //Si le bouton OK est appuyer
                if (BoutonIsOK ())
                {
                    //selectionner Mode rembobinage
                    valSTATES_Mode = STATE_MODE_REMBOBINAGE;
                }
                //éteindre la LED
                LED_RembobinageOff();
            }
        break;
        
        case STATE_MODE_NORMAL:
            //allumer la LED
            LED_NormalOn();
            //Si un des boutons d'incrémentation ou d'décrementation est actif
            if (BoutonPlus ()|| BoutonMinus ())
            {
                //Eteindre la LED et son refroidissement
                OnOff_VentilETLED(0);
                //Si le bouton + est appuyer
                if (BoutonPlus ())
                {
                    //changer de mode (avance rapide)
                    valSTATES_Mode = STATE_SELECT_AVANCE_RAPIDE;
                }
                //Si le bouton - est appuyer
                if (BoutonMinus ())
                {
                    //changer de mode (mode rembobinage)
                    valSTATES_Mode = STATE_SELECT_REMBOBINAGE;
                }
                //éteindre la LED
                LED_NormalOff();
            }
            else
            {
                //fonction permettant de faire tourner le système dans le mode mormal
                Mode_Normal();
            }
            
        break;
        
        case STATE_MODE_AVANCE_RAPIDE:
            //allumer la LED
            LED_AROn();
            //Si un des boutons d'incrémentation ou d'décrementation est actif
            if (BoutonPlus ()|| BoutonMinus ())
            {
                //Eteindre la LED et son refroidissement
                OnOff_VentilETLED(0);
                //Si le bouton + est appuyer
                if (BoutonPlus ())
                {
                    //changer de mode (avance lente)
                    valSTATES_Mode = STATE_SELECT_AVANCE_LENTE;
                }
                //Si le bouton - est appuyer
                if (BoutonMinus ())
                {
                    //changer de mode (mode normal)
                    valSTATES_Mode = STATE_SELECT_MODE_NORMAL;
                }
                //éteindre la LED
                LED_AROff();
            }
            else
            {
                //fonction permettant de faire tourner le système dans le mode avance rapide
                //Mode_Avance_Rapide();
            }
            
        break;
        
        case STATE_MODE_AVANCE_LENTE:
            //allumer la LED
            LED_ALOn();
            //Si un des boutons d'incrémentation ou d'décrementation est actif
            if (BoutonPlus ()|| BoutonMinus ())
            {
                //Eteindre la LED et son refroidissement
                OnOff_VentilETLED(0);
                //Si le bouton + est appuyer
                if (BoutonPlus ())
                {
                    //changer de mode (retour lent)
                    valSTATES_Mode = STATE_SELECT_RETOUR_LENT;
                }
                //Si le bouton - est appuyer
                if (BoutonMinus ())
                {
                    //changer de mode (avance rapide)
                    valSTATES_Mode = STATE_SELECT_AVANCE_RAPIDE;
                }
                //éteindre la LED
                LED_ALOff();
            }
            else
            {
                //fonction permettant de faire tourner le système dans le mode avance lente
                //Mode_Avance_Lente();
            }
            
        break;
        
        case STATE_MODE_RETOUR_LENT:
            //allumer la LED
            LED_RLOn();
            //Si un des boutons d'incrémentation ou d'décrementation est actif
            if (BoutonPlus ()|| BoutonMinus ())
            {
                //Eteindre la LED et son refroidissement
                OnOff_VentilETLED(0);
                //Si le bouton + est appuyer
                if (BoutonPlus ())
                {
                    //changer de mode (mode rembobinage)
                    valSTATES_Mode = STATE_SELECT_REMBOBINAGE;
                }
                //Si le bouton - est appuyer
                if (BoutonMinus ())
                {
                    //changer de mode (avance lente)
                    valSTATES_Mode = STATE_SELECT_AVANCE_LENTE;
                }
                //éteindre la LED
                LED_RLOff();
            }
            else
            {
                //fonction permettant de faire tourner le système dans le mode retour lent
                //Mode_Retour_Lent();
            }
            
        break;
        
        case STATE_MODE_REMBOBINAGE:
            //allumer la LED
            LED_RembobinageOn();
            //Si un des boutons d'incrémentation ou d'décrementation est actif
            if (BoutonPlus ()|| BoutonMinus ())
            {
                //Eteindre la LED et son refroidissement
                OnOff_VentilETLED(0);
                //Si le bouton + est appuyer
                if (BoutonPlus ())
                {
                    //changer de mode (mode normal)
                    valSTATES_Mode = STATE_SELECT_MODE_NORMAL;
                }
                //Si le bouton - est appuyer
                if (BoutonMinus ())
                {
                    //changer de mode (retour lent)
                    valSTATES_Mode = STATE_SELECT_RETOUR_LENT;
                }
                //éteindre la LED
                LED_RembobinageOff();
            }
            else
            {
                //fonction permettant de faire tourner le système dans le mode rembobinage
                //Mode_Rembobinage();
            }
            
        break;
            
    }
    //clear la valeur des boutons (mettre les bouton à 0))
    BoutonClearPlus();
    BoutonClearMinus();
    BoutonClearOK();
}


