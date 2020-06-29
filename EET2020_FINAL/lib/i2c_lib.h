/*
 * i2c_lib.h
 *
 * Created: 2020/06/17 16:27:33
 *  Author: gyneco2d
 */ 


#ifndef I2C_LIB_H_
#define I2C_LIB_H_

void i2c_init(void);
int32_t i2c_write_single(int16_t addr, uint8_t reg, uint8_t data);
int32_t i2c_write_multi(int16_t addr, uint8_t reg, uint8_t *pbuf, uint8_t length);
int32_t i2c_read(int16_t addr, uint8_t reg, uint8_t *pbuf, uint8_t length);

#endif /* I2C_LIB_H_ */
