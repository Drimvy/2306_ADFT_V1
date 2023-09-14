/*--------------------------------------------------------*/
//      Mc32_I2cUtilCCS.c
/*--------------------------------------------------------*/
//	Description :	utilitaire I2C compatible syntaxe CCS
//                      Obtenu par modification de l'exemple harmony
//
//	Auteur 		: 	C. HUBER
//
//	Version		:	V1.2
//	Compilateur	:	XC32 V1.40 & Harmony V1.06
// Modifications :
//      CHR 19.03.2015  Migration sur plib_i2c de Harmony 1.00   CHR
//      CHR 12.04.2016  adaptaion détails pour plib_i2c de Harmony 1.06   CHR
//		SCA 04.04.2017  Compléments commentaires i2c_init HighFrequencyEnable/Disable
//      CME 11.09.2023  Ajout de la possibiliter de travailler avec plusieurs I2C
//
/*--------------------------------------------------------*/

#include "app.h"
#include "Mc32_I2cUtilCCS.h"
#include "peripheral\i2c\plib_i2c.h"
#include "peripheral\osc\plib_osc.h"



// KIT 32MX795F512L Constants
//#define KIT_I2C_BUS   I2C_ID_3
#define I2C_CLOCK_FAST 400000
#define I2C_CLOCK_SLOW 100000




//------------------------------------------------------------------------------
// i2c_init
//
// Initialisationde l'I2c
//      si bool Fast = false  LOW speed   100 KHz
//      si bool Fast = true   HIGH speed  400 KHz
//------------------------------------------------------------------------------
// Adaptation plib_i2c  : 19.03.2015 CHR
// CHR 12.04.2016 reprise principe init du driver I2C de Harmony 1.06
// = pas OK reprise ancien principe            

uint32_t I2cConReg, I2cBrg;     // pour controle de la configuration

void i2c_init( bool Fast, uint32_t ID)
{
 
    PLIB_I2C_Disable(ID);      // Ajout CHR

    // LOW frequency is enabled (**NOTE** PLIB function logic reverted)
    // A 100k et 400kHz, on devrait activer le "slope control" 
    // (cf. § I2C datasheet PIC32). Toutefois, le LM92 a des problèmes 
    // d'incompatibilité avec les flancs trop lents => désactivé
    // Voir application note 
    // "AN-2113 Applying I2C Compatible Temperature Sensors in Systems with Slow Clock Edges"
    PLIB_I2C_HighFrequencyEnable(ID);
    if (Fast)  {
       PLIB_I2C_BaudRateSet(ID,
               SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_1), I2C_CLOCK_FAST);
    } else {
        PLIB_I2C_BaudRateSet(ID,
               SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_1), I2C_CLOCK_SLOW);
    }

    // selon driver. Voir commentaire PLIB_I2C_HighFrequencyEnable ci-dessus
    // PLIB_I2C_HighFrequencyDisable(KIT_I2C_BUS);   

    PLIB_I2C_SlaveClockStretchingEnable(ID);  // ajout CHR

    PLIB_I2C_Enable(ID);

    I2cConReg = I2C3CON;
    I2cBrg = I2C3BRG;
}


//------------------------------------------------------------------------------
// i2c_start()
//
// Début la transaction I2C master
//
// Adaptation plib_i2c  : 19.03.2015 CHR

void i2c_start(uint32_t ID)
{
  
    // int DebugCode = 0;

    // Wait for the bus to be idle, then start the transfer
    while(PLIB_I2C_BusIsIdle(ID) == false);

     /* Check for recieve overflow */
    if ( PLIB_I2C_ReceiverOverflowHasOccurred(ID))
    {
        PLIB_I2C_ReceiverOverflowClear(ID);
    }

    /* Check for transmit overflow */
    if (PLIB_I2C_TransmitterOverflowHasOccurred(ID))
    {
        PLIB_I2C_TransmitterOverflowClear(ID);
    }

    PLIB_I2C_MasterStart(ID);

    if (PLIB_I2C_ArbitrationLossHasOccurred(ID));
    {
        // DBPRINTF("Error: Bus collision during transfer Start\n");
        // DebugCode = 1;
        // Handel bus collision
        PLIB_I2C_ArbitrationLossClear(ID);
    }

    // Wait for the signal to complete
    while (PLIB_I2C_StartWasDetected(ID) == false);
    
 } // end i2c_start

void i2c_reStart(uint32_t ID)
{
   
    //int DebugCode = 0;

   // Pas d'attente bus en Idle

   /* Check for recieve overflow */
   if ( PLIB_I2C_ReceiverOverflowHasOccurred(ID))
   {
       PLIB_I2C_ReceiverOverflowClear(ID);
   }

   /* Check for transmit overflow */
   if (PLIB_I2C_TransmitterOverflowHasOccurred(ID))
   {
        PLIB_I2C_TransmitterOverflowClear(ID);
   }

   // PLIB_I2C_StartClear(KIT_I2C_BUS);
   PLIB_I2C_MasterStartRepeat(ID);

   if (PLIB_I2C_ArbitrationLossHasOccurred(ID));
    {
        // DBPRINTF("Error: Bus collision during transfer Start\n");
        // DebugCode = 1;
        // Handel bus collision
        PLIB_I2C_ArbitrationLossClear(ID);
    }

   // Wait for the signal to complete
   while (PLIB_I2C_StartWasDetected(ID) == false); 
  
} // end i2c_reStart


//------------------------------------------------------------------------------
// i2c_write()
//
// Syntaxe : 	i2c_write (data);
// Le paramètre data est la valeur 8 bits à transmettre.
// En mode maître, la fonction génère le signal d'horloge, en mode esclave elle attend le signal d'horloge du maître.
// En outre cette fonction retourne le bit ACK, 0 = ACK, 1 = NO_ACK.
//
//------------------------------------------------------------------------------
//
// Modification de  BOOL TransmitOneByte( UINT8 data )
// - Ajout retour du bit Ack
// Adaptation plib_i2c  : 19.03.2015 CHR

bool i2c_write( uint8_t data, uint32_t ID )
{
    bool  AckBit;
  
  
    //Wait for the bus to be idle (nécessaire aprés un reStart)
    while(PLIB_I2C_BusIsIdle(ID ) == false);

    // Wait for the transmitter to be ready
    while( PLIB_I2C_TransmitterIsReady(ID ) == false);


    // Transmit the byte
    PLIB_I2C_TransmitterByteSend(ID , data);

    while(PLIB_I2C_TransmitterIsBusy(ID ));              //Wait as long as TBF = 1
    while(!PLIB_I2C_TransmitterByteHasCompleted(ID ));   //Wait as long as TRSTAT == 1

    AckBit = PLIB_I2C_TransmitterByteWasAcknowledged(ID);

    return AckBit; 

} // end i2c_write



//------------------------------------------------------------------------------
// i2c_stop()
//
// termine la transaction I2C master
//
//------------------------------------------------------------------------------
//
// Modification de  void StopTransfer( void )
// - pas de modif sauf le nom de la fonction
// Adaptation plib_i2c  : 19.03.2015 CHR


void i2c_stop(uint32_t ID)
{
  
    // Attente bus au repos
    while(PLIB_I2C_BusIsIdle(ID ) == false);

    PLIB_I2C_MasterStop(ID );

    // Wait for the signal to complete
    while (PLIB_I2C_StopWasDetected(ID) == false);

} // end i2c_stop

//------------------------------------------------------------------------------
// i2c_read()
//
// Syntaxe : 	data = i2c_read (ack);
// La fonction retourne l'octet lu.
// Le paramètre ack :
// 1 (true)  signifie qu'il faut effectuer l'acquittement.
// 0 (false) signifie qu'il ne faut pas effectuer l'acquittement.
//
// Adaptation plib_i2c  : 19.03.2015 CHR

uint8_t i2c_read(bool ackTodo, uint32_t ID)
{
    uint8_t i2cByte;

   
        // BSP_LEDOn(BSP_LED_5);  // provisoire : pour observation

        // ajout idem driver statique I2C de Harmony 1_03
        if ( PLIB_I2C_ReceiverOverflowHasOccurred(ID))
        {
            i2cByte = PLIB_I2C_ReceivedByteGet(ID);
            PLIB_I2C_ReceiverOverflowClear(ID);
        }

        // en relation avec stetching
        PLIB_I2C_SlaveClockRelease(ID);

        // Set Rx enable in MSTR which causes SLAVE to send data
        PLIB_I2C_MasterReceiverClock1Byte(ID);

        // Wait till RBF = 1; Which means data is available in I2C2RCV reg
        while(!PLIB_I2C_ReceivedByteIsAvailable(ID));

        i2cByte = PLIB_I2C_ReceivedByteGet(ID); //Read from I2CxRCV

        while ( PLIB_I2C_MasterReceiverReadyToAcknowledge ( ID ) == false );

         if (ackTodo) {
              PLIB_I2C_ReceivedByteAcknowledge ( ID, true );
         } else {
              PLIB_I2C_ReceivedByteAcknowledge ( ID, false );
         }

        // wait till ACK/NACK sequence is complete i.e ACKEN = 0
        while( PLIB_I2C_MasterReceiverReadyToAcknowledge (ID) == false);

 

        return i2cByte;
    
    
} // end i2c_read



