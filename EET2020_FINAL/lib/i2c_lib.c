/*
 * i2c_lib.c
 *
 * Created: 2020/06/17 16:27:02
 *  Author: gyneco2d
 */ 


#include <atmel_start.h>
#include "i2c_ext_lib.h"
#include "i2c_lib.h"

#define TRYNUM 10

void i2c_init() {
	i2c_m_sync_enable(&I2C_0);
}

int32_t i2c_write_single(int16_t addr, uint8_t reg, uint8_t data) {
	int ret = -1, count = 1;

	i2c_m_sync_set_slaveaddr(&I2C_0, addr, I2C_M_SEVEN);
	while (count <= TRYNUM && ret != 0) {
		ret = i2c_ext_write_reg8(&I2C_0, reg, &data, 1);
		count++;
	}

	if (count == TRYNUM) {
		/* error occurred */
		return ret;
	}
	return ERR_NONE;
}

int32_t i2c_write_multi(int16_t addr, uint8_t reg, uint8_t  *pbuf, uint8_t length) {
	int ret = -1, count = 1;

	i2c_m_sync_set_slaveaddr(&I2C_0, addr, I2C_M_SEVEN);
	while (count <= TRYNUM && ret != 0) {
		ret = i2c_ext_write_reg8(&I2C_0, reg, pbuf, length);
		count++;
	}

	if (count == TRYNUM) {
		/* error occurred */
		return ret;
	}
	return ERR_NONE;
}

int32_t i2c_read(int16_t addr, uint8_t reg, uint8_t *pbuf, uint8_t length) {
	int ret = -1, count = 1;
	
	i2c_m_sync_set_slaveaddr(&I2C_0, addr, I2C_M_SEVEN);
	while (count <= TRYNUM && ret != 0) {
		ret = i2c_m_sync_cmd_read(&I2C_0, reg, pbuf, length);
		count++;
	}
	
	if (count == TRYNUM) {
		/* error occurred */
		return ret;
	}
	return ERR_NONE;
}
