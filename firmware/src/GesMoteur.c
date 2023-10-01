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


static uint16_t Nbr_Step_M2 = 500; //500
uint8_t Step_Positif[4] = {0x46, 0x44, 0x40, 0x42}; //moteur tourne dans le sens horaire
uint8_t Half_Step_Positif[8] = {0x46,0x06, 0x44, 0x44, 0x40, 0x00, 0x42, 0x42}; //moteur tourne dans le sens horaire
uint8_t Quarter_Step_Positif[16] = {0x06,0x46,0x06,0x44,0x04,0x44,0x44,0x40,0x00,0x40,0x00,0x42,0x02,0x42,0x42,0x46}; //moteur tourne dans le sens horaire
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
    Reset_M2Off();//fonctionne normalement
    //Allumer le moteur M1
    Reset_M1On();
    
    //éteindre le ventillateur et la LED
    OnOff_VentilETLED(0);
    
    //initaialiser les pin pour le maintien du moteur 
    //Moteur 3 mettre toute ces pins à 1
    INA1_M3On();
    INB1_M3On();
    INB2_M3On();
    
    //moteur 2 initaialiser les pin 
    CLK_M2Off();
    Enable_M2On();//normal operation mode
    Rot_Dir_M2Off();//sense anti horaire
    
    //Moteur 1 mettre toute ces pins à 1
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
    Rot_Dir_M2Off();//sense anti horaire
    //Déclatation de varable local 
    //Initaliser la machine d'état en static en mode STATE_MOTEUR_M3
    static STATES_MODE_Moteur Val_Mode_Moteur = STATE_MOTEUR_M3;
    //varible compteur de step
    
    static uint8_t Nbr_Step_M1 = 0;
    
    //valeur pour init de la structure
    static uint8_t Etape_Step_M1 = 6;
    static uint8_t Etape_Step_M2 = 0;
    static uint8_t Etape_Step_M3 = 0;
    
    
    switch(Val_Mode_Moteur)
    {
        case STATE_MOTEUR_M3:
            //Si le capteur de fin course du haut est actif 
            if(FinCourse_UpStateGet())
            {
                //Changer d'état (faire tourner le moteur M2)
                Val_Mode_Moteur = STATE_MOTEUR_M2;

            }
            //sinon faire tourner le moteur 
            else 
            {
                //modifier l'etat des pin INA1, INB1 et INB2
                Quarter_Step_M3(Etape_Step_M3);
                //moifier la valeur des port de I/O expender (controler via I2C))
                I2C_Write_Data_PCA95(ID_I2C_M(Moteur_3), Quarter_Step_Positif[Etape_Step_M3]);
                //incréementer la valeur du compteur
                Etape_Step_M3++;
                //si la valeur de  Val_HalfStep est supérieur au égale à 16
                //le moteur à fait un step complet
                if (Etape_Step_M3 >= NBR_ETAPE_STEP_M3)
                {
                    // remettre la valeur du compteurs à 0
                    Etape_Step_M3 = 0;
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
                if(Raspb2StateGet())
                {
                    //mettre le signal du clock à l'état haut
                    CLK_M2On();
                   //Si le moteur 2 à avance de moins de 4 step
                    if (Nbr_Step_M2 > 0)
                    {
                        //décréementer la valeur du compteur
                        Etape_Step_M2++;
                        //si la valeur de  Val_HalfStep est supérieur au égale à 8
                        //le moteur à fait un step complet
                        if (Etape_Step_M2 >= NBR_ETAPE_STEP_M2)
                        {
                            // remettre la valeur du compteurs à 0
                            Etape_Step_M2 = 0;
                            //incrémenter la compteur de step
                            Nbr_Step_M2 --;
                        } 
                    }
                    else
                    {
                        //mettre la valeur du compteur à 0
                        Nbr_Step_M2 = MAX_Step_M2;
                        //éteindre la LED et le ventillateur
                        OnOff_VentilETLED(0);
                        //Changer d'état (faire tourner le moteur M1)
                        Val_Mode_Moteur = STATE_MOTEUR_M1;
                    } 
                    //attendre 500 us (pour que le système est le temps de lire le flanc positif du clock)
                    delay_us(500);
                    //mettre le signal clock à 0
                    CLK_M2Off();
                }
                else
                {
                    //prendre une photo
                    delay_ms(1000);//simulation de la prise de photo
                }
            }
        break;
        case STATE_MOTEUR_M1:
            //Si le moteur 1 à avance de moins de 4 step
            if (Nbr_Step_M1 < MAX_Step_M1)
            {
                //modifier l'etat des pin INA1, INB1 et INB2
                Quarter_Step_M1(Etape_Step_M1);
                //moifier la valeur des port de I/O expender (controler via I2C))
                I2C_Write_Data_PCA95(ID_I2C_M(Moteur_1), Quarter_Step_Positif[Etape_Step_M1]);
                //incréementer la valeur du compteur
                Etape_Step_M1++;
                //si la valeur de  Val_HalfStep est supérieur au égale à 16
                //le moteur à fait un step complet
                if (Etape_Step_M1 >= NBR_ETAPE_STEP_M1)
                {
                    // remettre la valeur du compteurs à 0
                    Etape_Step_M1 = 0;
                    //incrémenter la compteur de step
                    Nbr_Step_M1 ++;
                }          
            }
            else
            {
                //mettre la valeur du compteur à 0
                Nbr_Step_M1 = 0;
                //Changer d'état (faire tourner le moteur M2)
                Val_Mode_Moteur = STATE_MOTEUR_M2;
            }
        
//        case STATE_MOTEUR_M1:
//            //Si le moteur 1 à avance de moins de 4 step
//            if (Nbr_Step_M1 < MAX_Step_M1)
//            {
//                if ( Etape_Step_M1 == 0)
//                {
//                    //incréementer la valeur du compteur
//                    Etape_Step_M1 = 16;  
//                }
//                //incréementer la valeur du compteur
//                Etape_Step_M1 --;
//                //modifier l'etat des pin INA1, INB1 et INB2
//                Quarter_Step_M1(Etape_Step_M1);
//                //moifier la valeur des port de I/O expender (controler via I2C))
//                I2C_Write_Data_PCA95(ID_I2C_M(Moteur_1), Quarter_Step_Positif[Etape_Step_M1]);
//                
//                //si la valeur de  Val_HalfStep est supérieur au égale à 16
//                //le moteur à fait un step complet
//                if (Etape_Step_M1 <= 0)
//                {
//                    Nbr_Step_M1 ++;
//                }  
//            
//            }
//            else
//            {
//                //mettre la valeur du compteur à 0
//                Nbr_Step_M1 = 0;
//                //Changer d'état (faire tourner le moteur M2)
//                Val_Mode_Moteur = STATE_MOTEUR_M2;
//            }
            
            
        break;   
    }
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
    Rot_Dir_M2Off();//sense anti horaire
    //Déclatation de varable local 
    //Initaliser la machine d'état en static en mode STATE_MOTEUR_M3
    static STATES_MODE_Moteur Val_Mode_Moteur_AL = STATE_MOTEUR_M3;

    static uint8_t Nbr_Step_M1 = 1;
    
    //valeur pour init de la structure
    static uint8_t Etape_Step_M1 = 0;
    static uint8_t Etape_Step_M3 = 0;
    
    
    switch(Val_Mode_Moteur_AL)
    {
        case STATE_MOTEUR_M3:
            //Si le capteur de fin course du haut est actif 
            if(FinCourse_UpStateGet())
            {
                //Changer d'état (faire tourner le moteur M1)
                Val_Mode_Moteur_AL = STATE_MOTEUR_M1;

            }
            //sinon faire tourner le moteur 
            else 
            {
                //modifier l'etat des pin INA1, INB1 et INB2
                Quarter_Step_M3(Etape_Step_M3);
                //moifier la valeur des port de I/O expender (controler via I2C))
                I2C_Write_Data_PCA95(ID_I2C_M(Moteur_3), Quarter_Step_Positif[Etape_Step_M3]);
                //incréementer la valeur du compteur
                Etape_Step_M3++;
                //si la valeur de  Val_HalfStep est supérieur au égale à 16
                //le moteur à fait un step complet
                if (Etape_Step_M3 >= NBR_ETAPE_STEP_M3)
                {
                    // remettre la valeur du compteurs à 0
                    Etape_Step_M3 = 0;
                }          
            }
        break;
        
        
        
        case STATE_MOTEUR_M1:
            //Si le moteur 1 à avance de moins de 4 step
            if (FinCourse_DownStateGet() != 1)
            {
                //modifier l'etat des pin INA1, INB1 et INB2
                Quarter_Step_M1(Etape_Step_M1);
                //moifier la valeur des port de I/O expender (controler via I2C))
                I2C_Write_Data_PCA95(ID_I2C_M(Moteur_1), Quarter_Step_Positif[Etape_Step_M1]);
                //incréementer la valeur du compteur
                Etape_Step_M1++;
                //si la valeur de  Val_HalfStep est supérieur au égale à 16
                //le moteur à fait un step complet
                if (Etape_Step_M1 >= NBR_ETAPE_STEP_M1)
                {
                    // remettre la valeur du compteurs à 0
                    Etape_Step_M1 = 0;
                    //incrémenter la compteur de step
                    Nbr_Step_M1 ++;
                }          
            }
            //Si le capteur de fin course du bas est actif 
            else
            {
                //Changer d'état (faire tourner le moteur M3)
                Val_Mode_Moteur_AL = STATE_MOTEUR_M3;
            }
            
            
        break; 
    }
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
    Rot_Dir_M2Off();//sense anti horaire
    //Déclatation de varable local 
    //Initaliser la machine d'état en static en mode STATE_MOTEUR_M3
    static STATES_MODE_Moteur Val_Mode_Moteur_AL = STATE_MOTEUR_M3;

    static uint8_t Nbr_Step_M1 = 1;
    
    //valeur pour init de la structure
    static uint8_t Etape_Step_M1 = 0;
    static uint8_t Etape_Step_M2 = 0;
    static uint8_t Etape_Step_M3 = 0;
    
    
    switch(Val_Mode_Moteur_AL)
    {
        case STATE_MOTEUR_M3:
            //Si le capteur de fin course du haut est actif 
            if(FinCourse_UpStateGet())
            {
                //Changer d'état (faire tourner le moteur M2)
                Val_Mode_Moteur_AL = STATE_MOTEUR_M2;

            }
            //sinon faire tourner le moteur 
            else 
            {
                //modifier l'etat des pin INA1, INB1 et INB2
                Quarter_Step_M3(Etape_Step_M3);
                //moifier la valeur des port de I/O expender (controler via I2C))
                I2C_Write_Data_PCA95(ID_I2C_M(Moteur_3), Quarter_Step_Positif[Etape_Step_M3]);
                //incréementer la valeur du compteur
                Etape_Step_M3++;
                //si la valeur de  Val_HalfStep est supérieur au égale à 16
                //le moteur à fait un step complet
                if (Etape_Step_M3 >= NBR_ETAPE_STEP_M3)
                {
                    // remettre la valeur du compteurs à 0
                    Etape_Step_M3 = 0;
                }          
            }
        break;
        
        case STATE_MOTEUR_M2:
            //Si le capteur de fin course du bas est actif 
            if (FinCourse_DownStateGet())
            {
                //Changer d'état (faire tourner le moteur M3)
                Val_Mode_Moteur_AL = STATE_MOTEUR_M3;
            }
            else
            { 
            //mettre le signal du clock à l'état haut
            CLK_M2On();
            //Si le moteur 2 à avance de moins de 4 step
                if (Nbr_Step_M2 > 0)
                {
                    //décréementer la valeur du compteur
                    Etape_Step_M2++;
                    //si la valeur de  Val_HalfStep est supérieur au égale à 8
                    //le moteur à fait un step complet
                    if (Etape_Step_M2 >= NBR_ETAPE_STEP_M2)
                    {
                        // remettre la valeur du compteurs à 0
                        Etape_Step_M2 = 0;
                        //incrémenter la compteur de step
                        Nbr_Step_M2 --;
                    } 
                }
                else
                {
                    //mettre la valeur du compteur à 0
                    Nbr_Step_M2 = MAX_Step_M2;
                    //Changer d'état (faire tourner le moteur M1)
                    Val_Mode_Moteur_AL = STATE_MOTEUR_M1;
                }
                //attendre 500 us (pour que le système est le temps de lire le flanc positif du clock)
                delay_us(500);
                //mettre le signal clock à 0
                CLK_M2Off();           
            }
        break;
        
        case STATE_MOTEUR_M1:
            //Si le moteur 1 à avance de moins de 4 step
            if (Nbr_Step_M1 < MAX_Step_M1)
            {
                //modifier l'etat des pin INA1, INB1 et INB2
                Quarter_Step_M1(Etape_Step_M1);
                //moifier la valeur des port de I/O expender (controler via I2C))
                I2C_Write_Data_PCA95(ID_I2C_M(Moteur_1), Quarter_Step_Positif[Etape_Step_M1]);
                //incréementer la valeur du compteur
                Etape_Step_M1++;
                //si la valeur de  Val_HalfStep est supérieur au égale à 16
                //le moteur à fait un step complet
                if (Etape_Step_M1 >= NBR_ETAPE_STEP_M1)
                {
                    // remettre la valeur du compteurs à 0
                    Etape_Step_M1 = 0;
                    //incrémenter la compteur de step
                    Nbr_Step_M1 ++;
                }          
            }
            else
            {
                //mettre la valeur du compteur à 0
                Nbr_Step_M1 = 0;
                //Changer d'état (faire tourner le moteur M2)
                Val_Mode_Moteur_AL = STATE_MOTEUR_M2;
            }
            
            
        break;   
    }
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
    Rot_Dir_M2On();//sense anti horaire
    //Déclatation de varable local 
    //Initaliser la machine d'état en static en mode STATE_MOTEUR_M3
    static STATES_MODE_Moteur Val_Mode_Moteur_RL = STATE_MOTEUR_M1;
    //varible compteur de step
    static uint8_t Nbr_Step_M1 = 0;
    static uint8_t MAX_Step = 2;
    //valeur pour init de la structure
    static uint8_t Etape_Step_M1 = 16;
    static uint8_t Etape_Step_M2 = 0;
    static uint8_t Etape_Step_M3 = 16;
    
    
    switch(Val_Mode_Moteur_RL)
    {
        case STATE_MOTEUR_M3:
            //Si le capteur de fin course du haut est actif 
            if(FinCourse_DownStateGet())
            {
                //Changer d'état (faire tourner le moteur M2)
                Val_Mode_Moteur_RL = STATE_MOTEUR_M2;

            }
            //sinon faire tourner le moteur 
            else 
            {
                if ( Etape_Step_M3 == 0)
                {
                    //incréementer la valeur du compteur
                    Etape_Step_M3 = 16;  
                }
                //incréementer la valeur du compteur
                Etape_Step_M3 --;
                //modifier l'etat des pin INA1, INB1 et INB2
                Quarter_Step_M3(Etape_Step_M1);
                //moifier la valeur des port de I/O expender (controler via I2C))
                I2C_Write_Data_PCA95(ID_I2C_M(Moteur_3), Quarter_Step_Positif[Etape_Step_M3]);
                        
            }
        break;
        
        case STATE_MOTEUR_M2:
            //Si le capteur de fin course du bas est actif 
            if (FinCourse_UpStateGet())
            {
                //Changer d'état (faire tourner le moteur M3)
                Val_Mode_Moteur_RL = STATE_MOTEUR_M3;
            }
            else
            { 
            //mettre le signal du clock à l'état haut
            CLK_M2On();
            //Si le moteur 2 à avance de moins de 4 step
                if (Nbr_Step_M2 > 0)
                {
                    //décréementer la valeur du compteur
                    Etape_Step_M2++;
                    //si la valeur de  Val_HalfStep est supérieur au égale à 8
                    //le moteur à fait un step complet
                    if (Etape_Step_M2 >= NBR_ETAPE_STEP_M2)
                    {
                        // remettre la valeur du compteurs à 0
                        Etape_Step_M2 = 0;
                        //incrémenter la compteur de step
                        Nbr_Step_M2 --;
                    } 
                }
                else
                {
                    //mettre la valeur du compteur à 0
                    Nbr_Step_M2 = MAX_Step_M2;
                    //éteindre la LED et le ventillateur
                    OnOff_VentilETLED(0);
                    //Changer d'état (faire tourner le moteur M1)
                    Val_Mode_Moteur_RL = STATE_MOTEUR_M1;
                }
                //attendre 500 us (pour que le système est le temps de lire le flanc positif du clock)
                delay_us(500);
                //mettre le signal clock à 0
                CLK_M2Off();            
            }
        break;
        
        case STATE_MOTEUR_M1:
            //Si le moteur 1 à avance de moins de 4 step
            if (Nbr_Step_M1 <= MAX_Step)
            {
                if ( Etape_Step_M1 == 0)
                {
                    //incréementer la valeur du compteur
                    Etape_Step_M1 = 16;  
                }
                //incréementer la valeur du compteur
                Etape_Step_M1 --;
                //modifier l'etat des pin INA1, INB1 et INB2
                Quarter_Step_M1(Etape_Step_M1);
                //moifier la valeur des port de I/O expender (controler via I2C))
                I2C_Write_Data_PCA95(ID_I2C_M(Moteur_1), Quarter_Step_Positif[Etape_Step_M1]);
                
                //si la valeur de  Val_HalfStep est supérieur au égale à 16
                //le moteur à fait un step complet
                if (Etape_Step_M1 <= 0)
                {
                    Nbr_Step_M1 ++;
                }  
            
            }
            else
            {
                MAX_Step = 1;
                //mettre la valeur du compteur à 0
                Nbr_Step_M1 = 1;
                //Changer d'état (faire tourner le moteur M2)
                Val_Mode_Moteur_RL = STATE_MOTEUR_M2;
            }
            
            
        break;   
    
    }
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

    //Déclatation de varable local 
    //Initaliser la machine d'état en static en mode STATE_MOTEUR_M3
    static STATES_MODE_Moteur Val_Mode_Moteur_RL = STATE_MOTEUR_M1;


    //valeur pour init de la structure
    static uint8_t Etape_Step_M1 = 16;

    static uint8_t Etape_Step_M3 = 16;
    
    
    switch(Val_Mode_Moteur_RL)
    {
        case STATE_MOTEUR_M3:
            //Si le capteur de fin course du haut est actif 
            if(FinCourse_DownStateGet())
            {
                //Changer d'état (faire tourner le moteur M2)
                Val_Mode_Moteur_RL = STATE_MOTEUR_M1;

            }
            //sinon faire tourner le moteur 
            else 
            {
                if ( Etape_Step_M3 == 0)
                {
                    //incréementer la valeur du compteur
                    Etape_Step_M3 = 16;  
                }
                //incréementer la valeur du compteur
                Etape_Step_M3 --;
                //modifier l'etat des pin INA1, INB1 et INB2
                Quarter_Step_M3(Etape_Step_M1);
                //moifier la valeur des port de I/O expender (controler via I2C))
                I2C_Write_Data_PCA95(ID_I2C_M(Moteur_3), Quarter_Step_Positif[Etape_Step_M3]);
                        
            }
        break;
        
        
        case STATE_MOTEUR_M1:
            //Si le moteur 1 à avance de moins de 4 step
            if (FinCourse_UpStateGet() != 1)
            {
                if ( Etape_Step_M1 == 0)
                {
                    //incréementer la valeur du compteur
                    Etape_Step_M1 = 16;  
                }
                //incréementer la valeur du compteur
                Etape_Step_M1 --;
                //modifier l'etat des pin INA1, INB1 et INB2
                Quarter_Step_M1(Etape_Step_M1);
                //moifier la valeur des port de I/O expender (controler via I2C))
                I2C_Write_Data_PCA95(ID_I2C_M(Moteur_1), Quarter_Step_Positif[Etape_Step_M1]);
                
                 
            
            }
            else
            {
                //Changer d'état (faire tourner le moteur M3)
                Val_Mode_Moteur_RL = STATE_MOTEUR_M3;
            }
            
            
        break;   
    
    }
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
    //initailiser la varable
    uint32_t PeriodeTimer3 = 0;
    //Obtention de la periode du timer 
    PeriodeTimer3 = DRV_TMR1_PeriodValueGet(); 
    //Alumer la LED et le ventillateur
    if (Etat == 1)
    {
        //allumer le ventillateur pour refroidire la LED (signal inversé)
        CTRL_VentilOff();
        //Calcul du rapport de la pulse, (inversion du PWM ), 
        //entrer la valeur du PWM voulu en poucent sur le define: RapportPWM_pc
        PeriodeTimer3 = PeriodeTimer3 * (100-RapCyclPWM_pc)/100;
        //MAJ de la pulse du PWM 
        DRV_OC0_PulseWidthSet( PeriodeTimer3);
    }
    else
    {
        //allumer la pin controlant le ventillateur pour arreter refroidire la LED (signal inversé)
        CTRL_VentilOn(); 
        //Calcul du rapport de la pulse, (inversion du PWM ), 
        //entrer la valeur du PWM voulu en poucent sur le define: RapportPWM_pc
        PeriodeTimer3 = PeriodeTimer3 * (100-0)/100;
        //MAJ de la pulse du PWM 
        DRV_OC0_PulseWidthSet( PeriodeTimer3);
    }
}
//---------------------------------------------------------------------------------	
// Fonction Half_Step_M3
// Description: Fonction permettant de mettreà jour la valeur des pins INA1, INB1 
//              et INB2. Cela permettra de faire tourner le moteur
//
// Entrées: numéro d'étape de la table de vériter pour tourner le moteur en mode halfstep
// Sorties: -
void Half_Step_M3(uint8_t Numero_Step_M3)
{
    switch (Numero_Step_M3)
    {
        case 0:
            INA1_M3On();
            //INA2_M3On();
            INB1_M3On();
            INB2_M3On();
        break;
        
        case 1:
            INA1_M3Off();
            //INA2_M3Off();
        break;
        
        case 2:
            INA1_M3On();
        break;
        
        case 3:
            INB1_M3Off();
            INB2_M3Off();
            
        break;
        
        case 4:
            INB1_M3On();
            INB2_M3On();
        break;
        
        case 5:
            INA1_M3Off();
            //INA2_M3Off();;
        break;
        
        case 6:
            INA1_M3On();
            //INA2_M3On();;
        break;
        
        case 7:
            INB1_M3Off();
            INB2_M3Off();
        break;
    }
}
//---------------------------------------------------------------------------------	
// Fonction Half_Step_M3
// Description: Fonction permettant de mettre à jour la valeur des pins INA1, INB1 
//              et INB2. Cela permettra de faire tourner le moteur
//
// Entrées: numéro d'étape de la table de vériter pour tourner le moteur en mode quarter step
// Sorties: -
void Quarter_Step_M3(uint8_t Numero_Step_M3)
{
    switch (Numero_Step_M3)
    {
        case 0:
            INA1_M3On();
            //INA2_M3Off();
            INB1_M3On();
            INB2_M3Off();
        break;
        
        case 1:
            INA1_M3Off();
            //INA2_M3On();
            INB1_M3On();
            INB2_M3On();
        break;
        
        case 2:
            INA1_M3Off();
            //INA2_M3Off();
            INB1_M3On();
            INB2_M3On();
        break;
        
        case 3:
            INA1_M3Off();
            //INA2_M3On();
            INB1_M3On();
            INB2_M3On();
        break;
        
        case 4:
            INA1_M3On();
            //INA2_M3Off();
            INB1_M3On();
            INB2_M3Off();
        break;
        
        case 5:
            INA1_M3On();
            //INA2_M3On();
            INB1_M3Off();
            INB2_M3On();
        break;
        
        case 6:
            INA1_M3On();
            //INA2_M3On();
            INB1_M3Off();
            INB2_M3Off();
        break;
        
        case 7:
            INA1_M3On();
            //INA2_M3On();
            INB1_M3Off();
            INB2_M3On();
        break;
        
        case 8:
            INA1_M3On();
            //INA2_M3Off();
            INB1_M3On();
            INB2_M3Off();
        break;
        
        case 9:
            INA1_M3Off();
            //INA2_M3On();
            INB1_M3On();
            INB2_M3On();
        break;
        
        case 10:
            INA1_M3Off();
            //INA2_M3Off();
            INB1_M3On();
            INB2_M3On();
        break;
        
        case 11:
            INA1_M3Off();
            //INA2_M3On();
            INB1_M3On();
            INB2_M3On();
        break;
        
         case 12:
            INA1_M3On();
            //INA2_M3Off();
            INB1_M3On();
            INB2_M3Off();
        break;
        
        case 13:
            INA1_M3On();
            //INA2_M3On();
            INB1_M3Off();
            INB2_M3On();
        break;
        
        case 14:
            INA1_M3On();
            //INA2_M3On();
            INB1_M3Off();
            INB2_M3Off();
        break;
        
        case 15:
            INA1_M3On();
            //INA2_M3On();
            INB1_M3Off();
            INB2_M3On();
        break;
    }
}
//---------------------------------------------------------------------------------	
// Fonction Half_Step_M1
// Description: Fonction permettant de mettreà jour la valeur des pins INA1, INB1 
//              et INB2. Cela permettra de faire tourner le moteur
//
// Entrées: numéro d'étape de la table de vériter pour tourner le moteur en mode halfstep
// Sorties: -
void Half_Step_M1(uint8_t Numero_Step_M1)
{
    switch (Numero_Step_M1)
    {
        case 0:
            INA1_M1On();
            //INA2_M1On();
            INB1_M1On();
            INB2_M1On();
        break;
        
        case 1:
            INA1_M1Off();
            //INA2_M3Off();
        break;
        
        case 2:
            INA1_M1On();

        break;
        
        case 3:

            INB1_M1Off();
            INB2_M1Off();
            
        break;
        
        case 4:
            INB1_M1On();
            INB2_M1On();
        break;
        
        case 5:
            INA1_M1Off();
            //INA2_M3Off();;
        break;
        
        case 6:
            INA1_M1On();
            //INA2_M3On();;
        break;
        
        case 7:
            INB1_M1Off();
            INB2_M1Off();
        break;
    }
}
//---------------------------------------------------------------------------------	
// Fonction Quarter_Step_M1
// Description: Fonction permettant de mettre à jour la valeur des pins INA1, INB1 
//              et INB2. Cela permettra de faire tourner le moteur
//
// Entrées: numéro d'étape de la table de vériter pour tourner le moteur en mode quarter step
// Sorties: -
void Quarter_Step_M1(uint8_t Numero_Step_M1)
{
    switch (Numero_Step_M1)
    {
        case 0:
            INA1_M1On();
            //INA2_M3Off();
            INB1_M1On();
            INB2_M1Off();
        break;
        
        case 1:
            INA1_M1Off();
            //INA2_M3On();
            INB1_M1On();
            INB2_M1On();
        break;
        
        case 2:
            INA1_M1Off();
            //INA2_M3Off();
            INB1_M1On();
            INB2_M1On();
        break;
        
        case 3:
            INA1_M1Off();
            //INA2_M3On();
            INB1_M1On();
            INB2_M1On();
        break;
        
        case 4:
            INA1_M1On();
            //INA2_M3Off();
            INB1_M1On();
            INB2_M1Off();
        break;
        
        case 5:
            INA1_M1On();
            //INA2_M3On();
            INB1_M1Off();
            INB2_M1On();
        break;
        
        case 6:
            INA1_M1On();
            //INA2_M3On();
            INB1_M1Off();
            INB2_M1Off();
        break;
        
        case 7:
            INA1_M1On();
            //INA2_M3On();
            INB1_M1Off();
            INB2_M1On();
        break;
        
        case 8:
            INA1_M1On();
            //INA2_M3Off();
            INB1_M1On();
            INB2_M1Off();
        break;
        
        case 9:
            INA1_M1Off();
            //INA2_M3On();
            INB1_M1On();
            INB2_M1On();
        break;
        
        case 10:
            INA1_M1Off();
            //INA2_M3Off();
            INB1_M1On();
            INB2_M1On();
        break;
        
        case 11:
            INA1_M1Off();
            //INA2_M3On();
            INB1_M1On();
            INB2_M1On();
        break;
        
         case 12:
            INA1_M1On();
            //INA2_M3Off();
            INB1_M1On();
            INB2_M1Off();
        break;
        
        case 13:
            INA1_M1On();
            //INA2_M3On();
            INB1_M1Off();
            INB2_M1On();
        break;
        
        case 14:
            INA1_M1On();
            //INA2_M3On();
            INB1_M1Off();
            INB2_M1Off();
        break;
        
        case 15:
            INA1_M1On();
            //INA2_M3On();
            INB1_M1Off();
            INB2_M1On();
        break;
    }
}

