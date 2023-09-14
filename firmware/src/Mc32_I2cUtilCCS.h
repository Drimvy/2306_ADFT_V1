#ifndef MC32_I2CUTILCCS_H
#define MC32_I2CUTILCCS_H
//--------------------------------------------------------
//	Mc32_I2cUtilCCS.h
//--------------------------------------------------------
//	Description :	Adaptation des fonctions I2C afin
//			de pouvoir utiliser la même syntaxe 
//			que sous CCS.
//
//	Auteur 		: 	C. HUBER
//      Date            :       22.05.2014
//	Version		:	V1.6
//	Compilateur	:	XC32 V1.33 & Harmony V1.00
// Modifications :
//      CHR 19.03.2015  Migration sur plib_i2c de Harmony 1.00   CHR
//      CME 11.09.2023  Ajout de la possibiliter de travailler avec plusieurs I2C
/*--------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>


//------------------------------------------------------------------------------
// i2c_init
//
// Initialisationde l'I2c
//   si bool Fast = false   LOW speed
//   si bool Fast = true    HIGH speed
//------------------------------------------------------------------------------

void i2c_init(bool Fast, uint32_t id);
void i2c_start(uint32_t id);
void i2c_reStart(uint32_t id);
bool i2c_write( uint8_t data, uint32_t id );

//------------------------------------------------------------------------------
// i2c_read()
//
// Syntaxe : 	data = i2c_read (ack);
// La fonction retourne l'octet lu.
// Le paramètre ack :
// 1 signifie qu'il faut effectuer l'acquittement.
// 0 signifie qu'il ne faut pas effectuer l'acquittement.
//
// Remarque : réalisée à partir du code de l'exemple
//

uint8_t i2c_read(bool ackTodo,uint32_t id);

void i2c_stop(uint32_t id);


#endif
