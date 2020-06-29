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
 * \brief I2C�ɂ��Write ���� 8�o�C�g���W�X�^�Ɖϒ��f�[�^�̏������ݗp(7bit �X���[�u�A�h���X�̂�)
 * \param[in] i2c I2C�Ǘ��p�̍\���̕ϐ��̃|�C���^(driver_init.c ���Ő錾�A�����������ς�)  ��) &I2C_0
 * \param[in] reg �f�o�C�X�̃��W�X�^�A�h���X 1�o�C�g����
 * \param[in] buffer �������݃f�[�^�z��(uint8_t�̔z��)�̐擪�A�h���X
 * \param[in] length �f�[�^�̒���(byte)
 * \return ���s����
 * \retval ���l�@���s
 * \retval 0 ����
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
 * \brief I2C�ɂ��Write ���� 16�o�C�g���W�X�^�Ɖϒ��f�[�^�̏������ݗp(7bit �X���[�u�A�h���X�̂�)
 * \param[in] i2c I2C�Ǘ��p�̍\���̕ϐ��̃|�C���^(driver_init.c ���Ő錾�A�����������ς�)  ��) &I2C_0
 * \param[in] reg �f�o�C�X�̃��W�X�^�A�h���X 2�o�C�g����
 * \param[in] buffer �������݃f�[�^�z��(uint8_t�̔z��)�̐擪�A�h���X
 * \param[in] length �f�[�^�̒���(byte)
 * \return ���s����
 * \retval ���l�@���s
 * \retval 0 ����
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
