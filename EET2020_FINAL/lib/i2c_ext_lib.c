/*
 * i2c_lib.c
 *
 * Created: 02/06/17
 *  Author: nakamori
 */ 


#include <hal_i2c_m_sync.h>
#include <utils.h>
#include <string.h>
#include <stdlib.h>
#include "i2c_ext_lib.h"


/**
 * \brief I2CによるWrite 処理 8バイトレジスタと可変長データの書き込み用(7bit スレーブアドレスのみ)
 * \param[in] i2c I2C管理用の構造体変数のポインタ(driver_init.c 内で宣言、初期化処理済み)  例) &I2C_0
 * \param[in] reg デバイスのレジスタアドレス 1バイト限定
 * \param[in] buffer 書き込みデータ配列(uint8_tの配列)の先頭アドレス
 * \param[in] length データの長さ(byte)
 * \return 実行結果
 * \retval 負値　失敗
 * \retval 0 成功
 */
int32_t i2c_ext_write_reg8(struct i2c_m_sync_desc *i2c, uint8_t reg, uint8_t *buffer, uint8_t length)
{
	uint8_t *pbuf = (uint8_t *)malloc(length+1);
	if (!pbuf) {
		return ERR_NO_MEMORY;
	}
	pbuf[0] = reg;
	memcpy(&pbuf[1], buffer, length);
	
	struct _i2c_m_msg msg;
	msg.addr   = i2c->slave_addr;
	msg.len    = length+1;
	msg.flags  = I2C_M_STOP;
	msg.buffer = pbuf;
	
	return i2c_m_sync_transfer(i2c, &msg);
}


/**
 * \brief I2CによるWrite 処理 16バイトレジスタと可変長データの書き込み用(7bit スレーブアドレスのみ)
 * \param[in] i2c I2C管理用の構造体変数のポインタ(driver_init.c 内で宣言、初期化処理済み)  例) &I2C_0
 * \param[in] reg デバイスのレジスタアドレス 2バイト限定
 * \param[in] buffer 書き込みデータ配列(uint8_tの配列)の先頭アドレス
 * \param[in] length データの長さ(byte)
 * \return 実行結果
 * \retval 負値　失敗
 * \retval 0 成功
 */
int32_t i2c_ext_write_reg16(struct i2c_m_sync_desc *i2c, uint16_t reg, uint8_t *buffer, uint8_t length)
{
	uint8_t *pbuf = (uint8_t *)malloc(length+2);
	if (!pbuf) {
		return ERR_NO_MEMORY;
	}
	pbuf[0] = reg & 0xFF;
	pbuf[1] = reg >> 8;
	memcpy(&pbuf[2], buffer, length);
	
	struct _i2c_m_msg msg;
	msg.addr   = i2c->slave_addr;
	msg.len    = length+2;
	msg.flags  = I2C_M_STOP;
	msg.buffer = pbuf;
	
	return i2c_m_sync_transfer(i2c, &msg);
}
