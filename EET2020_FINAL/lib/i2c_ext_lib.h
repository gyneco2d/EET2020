/*
 * i2c_ext_lib.h
 *
 * Created: 02/06/17 
 *  Author: nakamori
 */ 


#ifndef I2C_EXT_LIB_H_
#define I2C_EXT_LIB_H_

extern int32_t i2c_ext_write_reg8(struct i2c_m_sync_desc *i2c, uint8_t reg, uint8_t *buffer, uint8_t length);
extern int32_t i2c_ext_write_reg16(struct i2c_m_sync_desc *i2c, uint16_t reg, uint8_t *buffer, uint8_t length);

#endif /* I2C_EXT_LIB_H_ */
