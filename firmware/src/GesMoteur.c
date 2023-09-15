// GesMoteur.c
//
//	Description : Gestion des moteurs suivant le mode selectionné
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
#include "GesMoteur.h"


//---------------------------------------------------------------------------------	
// Fonction INIT_MOTEUR
// Description: Fonction permettant d'initialiser les pins necessaire à la gestion 
//              des moteurs
// Entrées: -
// Sorties: -

void INIT_MOTEUR(void)
{
    //Allumer le moteur M3
    Reset_M3On();
    //Allumer le moteur M2
    Reset_M2On();
    //Allumer le moteur M1
    Reset_M1On();
    
    //éteindre le ventillateur et la LED
    OnOff_VentilETLED(0);
    
    //activer les Input pour faire tourner le moteur
    //Moteur 3
    INA1_M3On();
    INB1_M3On();
    INB2_M3On();
    //moteur 2
    INA1_M2On();
    INB1_M2On();
    INB2_M2On();
    //Moteur 1 
    INA1_M1On(); 
    INB1_M1On();
    INB2_M1On();
}


//---------------------------------------------------------------------------------	
// Fonction Mode_Normal
// Description: Fonction permettant la gestion des trois moteur pour effectuer les 
//              mouvement du Mode Normal
//        
// Entrées: -
// Sorties: -

void Mode_Normal(void)
{
    //Déclatation de varable local 
    //Initaliser la machine d'état en static en mode STATE_MOTEUR_M3
    static STATES_MODE_Moteur Val_Mode_Moteur = STATE_MOTEUR_M3;
    //varible compteur de step
    uint8_t Nbr_Step_M2 = 0;
    uint8_t Nbr_Step_M1 = 0;
    
    static int i;
    
    uint8_t Step_Positif[4] = {0x46, 0x44, 0x40, 0x42}; //moteur tourne dans le sens horaire

    switch(Val_Mode_Moteur)
    {
        case STATE_MOTEUR_M3:
            //Si le capteur de fin course du haut est actif 
            if(SwitchUp())
            {
                //Changer d'état (faire tourner le moteur M2)
                Val_Mode_Moteur = STATE_MOTEUR_M2;
            }
            //sinon faire tourner le moteur 
            else 
            {
                
                if(i == 4)
                {
                    i = 0;
                }
                else
                {
                    I2C_WriteGPIO_UnData_PCA95(ID_I2C_M(Moteur_3), Step_Positif[i]);
                    i++;
                    delay_ms(10);
                }             
            }
  
        break;
        
        case STATE_MOTEUR_M2:
            //Si le capteur de fin course du bas est actif 
            if (FinCourse_DownStateGet())
            {
                //éteindre la LED et le ventillateur
                OnOff_VentilETLED(0);
                //Changer d'état (faire tourner le moteur M3)
                Val_Mode_Moteur = STATE_MOTEUR_M3;
            }
            else
            { 
                //allumer la LED et le ventillateur
                OnOff_VentilETLED(1);
                /*Si la PIN est à l'etat haut (Pin servant à simuler le signal 
                qui sera envoyer par le Raspberry pi)*/
                if(Raspb1StateGet())
                {
                    //prendre une photo
                    delay_us(100);//simulation de la prise de photo
                }
                else
                {
                    //envoie des data pour avancer d'un step
                    I2C_WriteGPIO_PCA95( ID_I2C_M(Moteur_2) , Step_Positif, 4);
                    //Si le moteur 2 à avance de moins de 4 step
                    if (Nbr_Step_M2 < 40)
                    {
                        //incrémenter la compteur de step
                        Nbr_Step_M2 ++;
                        //rester dans la machine d'etat à l'etat STATE_MOTEUR_M2
                        Val_Mode_Moteur = STATE_MOTEUR_M2;
                    }
                    else
                    {
                        //mettre la valeur du compteur à 0
                        Nbr_Step_M2 = 0;
                        //Changer d'état (faire tourner le moteur M1)
                        Val_Mode_Moteur = STATE_MOTEUR_M1;
                    }
                }
            }
        break;
        
        case STATE_MOTEUR_M1:
            if (FinCourse_DownStateGet())
            {
                //éteindre la LED et le ventillateur
                OnOff_VentilETLED(0);
                //Changer d'état (faire tourner le moteur M3)
                Val_Mode_Moteur = STATE_MOTEUR_M3;
            }
            else
            { 
                //envoie des data pour avancer d'un step
                I2C_WriteGPIO_PCA95( ID_I2C_M(Moteur_1) , Step_Positif, 4);
                //Si le moteur 1 à avance de moins de 4 step
                if (Nbr_Step_M1 < 4)
                {
                    //incrémenter la compteur de step
                    Nbr_Step_M1 ++;
                    //rester dans la machine d'etat à l'etat STATE_MOTEUR_M1
                    Val_Mode_Moteur = STATE_MOTEUR_M1;
                }
                else
                {
                    //mettre la valeur du compteur à 0
                    Nbr_Step_M1 = 0;
                    //Changer d'état (faire tourner le moteur M2)
                    Val_Mode_Moteur = STATE_MOTEUR_M2;
                }
            }
            
        break;   
    }
    SwitchClearDown();
    SwitchClearUp();
}

//---------------------------------------------------------------------------------	
// Fonction Mode_Avance_Rapide
// Description: Fonction permettant la gestion des trois moteur pour effectuer une 
//              avance rapide
//        
// Entrées: -
// Sorties: -

void Mode_Avance_Rapide(void)
{
    
}

//---------------------------------------------------------------------------------	
// Fonction Mode_Avance_Lente
// Description: Fonction permettant la gestion des trois moteur pour effectuer un 
//              avance lente
//        
// Entrées: -
// Sorties: -

void Mode_Avance_Lente(void)
{
    
}

//---------------------------------------------------------------------------------	
// Fonction Mode_Retour_Lent
// Description: Fonction permettant la gestion des trois moteur pour effectuer un 
//              retour lent
//        
// Entrées: -
// Sorties: -

void Mode_Retour_Lent(void)
{
    
}

//---------------------------------------------------------------------------------	
// Fonction Mode_Rembobinage
// Description: Fonction permettant la gestion des trois moteur pour effectuer un 
//              rembobinage
//        
// Entrées: -
// Sorties: -

void Mode_Rembobinage(void)
{
    
}
//---------------------------------------------------------------------------------	
// Fonction OnOff_VentilETLED
// Description: Fonction permettant d'éteindre ou allumer la ventillation et la LED 
//              de puissance. Dans le cas de la LED, son intensité lumineuse et géré
//              par un signal PWM     
//                          
// Entrées: entrer un 1 pour enclancher la led et la ventillation, et un 0 pour tout
//          éteindre (LED et Ventillaton)
// Sorties: -
void OnOff_VentilETLED(bool Etat)
{
    uint32_t PeriodeTimer3 = 0;
    //Alumer la LED et le ventillateur
    if (Etat == 1)
    {
        //allumer le ventillateur pour refroidire la LED (signal inversé)
        CTRL_VentilOff();
        //start OC
        DRV_OC0_Start();
        //Obtention de la periode du timer 
        PeriodeTimer3 = DRV_TMR1_PeriodValueGet();  
        //Calcul du rapport de la pulse, (inversion du PWM ), 
        //entrer la valeur du PWM voulu en poucent sur le define: RapportPWM_pc
        PeriodeTimer3 = PeriodeTimer3 * (100-RapportPWM_pc)/100;
        //MAJ de la pulse du PWM 
        DRV_OC0_PulseWidthSet( PeriodeTimer3);
    }
    else
    {
        //Stop OC (Stopper le PWM)
        DRV_OC0_Stop();
        //allumer la pin controlant le ventillateur pour arreter refroidire la LED (signal inversé)
        CTRL_VentilOn();
    }
}
