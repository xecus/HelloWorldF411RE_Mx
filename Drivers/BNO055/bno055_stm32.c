#include "bno055_stm32.h"

static I2C_HandleTypeDef *_bno055_i2c;


void bno055_assignI2C(I2C_HandleTypeDef *i2cHandle) {
    _bno055_i2c = i2cHandle;
}

void bno055_delay(int time) {
    HAL_Delay(time);
}

/*
void bno055_writeData(uint8_t reg, uint8_t data) {
    uint8_t buf[2] = {reg, data};
    HAL_I2C_Master_Transmit(_bno055_i2c, BNO055_I2C_ADDR << 1, buf, 2, BNO055_WRITE_TIMEOUT);
}

void bno055_readData(uint8_t reg, uint8_t *data, uint8_t len) {
    HAL_I2C_Mem_Read(_bno055_i2c, BNO055_I2C_ADDR << 1, reg, I2C_MEMADD_SIZE_8BIT, data, len, BNO055_READ_TIMEOUT);
}
*/

void bno055_writeData(uint8_t reg, uint8_t data) {
  uint8_t txdata[2] = {reg, data};
  uint8_t status;
  status = HAL_I2C_Master_Transmit(_bno055_i2c, BNO055_I2C_ADDR << 1,
                                   txdata, sizeof(txdata), 10);
  if (status == HAL_OK) {
    return;
  }

  if (status == HAL_ERROR) {
    printf("HAL_I2C_Master_Transmit HAL_ERROR\r\n");
  } else if (status == HAL_TIMEOUT) {
    printf("HAL_I2C_Master_Transmit HAL_TIMEOUT\r\n");
  } else if (status == HAL_BUSY) {
    printf("HAL_I2C_Master_Transmit HAL_BUSY\r\n");
  } else {
    printf("Unknown status data %d", status);
  }

  uint32_t error = HAL_I2C_GetError(_bno055_i2c);
  if (error == HAL_I2C_ERROR_NONE) {
    return;
  } else if (error == HAL_I2C_ERROR_BERR) {
    printf("HAL_I2C_ERROR_BERR\r\n");
  } else if (error == HAL_I2C_ERROR_ARLO) {
    printf("HAL_I2C_ERROR_ARLO\r\n");
  } else if (error == HAL_I2C_ERROR_AF) {
    printf("HAL_I2C_ERROR_AF\r\n");
  } else if (error == HAL_I2C_ERROR_OVR) {
    printf("HAL_I2C_ERROR_OVR\r\n");
  } else if (error == HAL_I2C_ERROR_DMA) {
    printf("HAL_I2C_ERROR_DMA\r\n");
  } else if (error == HAL_I2C_ERROR_TIMEOUT) {
    printf("HAL_I2C_ERROR_TIMEOUT\r\n");
  }

  HAL_I2C_StateTypeDef state = HAL_I2C_GetState(_bno055_i2c);
  if (state == HAL_I2C_STATE_RESET) {
    printf("HAL_I2C_STATE_RESET\r\n");
  } else if (state == HAL_I2C_STATE_READY) {
    printf("HAL_I2C_STATE_RESET\r\n");
  } else if (state == HAL_I2C_STATE_BUSY) {
    printf("HAL_I2C_STATE_BUSY\r\n");
  } else if (state == HAL_I2C_STATE_BUSY_TX) {
    printf("HAL_I2C_STATE_BUSY_TX\r\n");
  } else if (state == HAL_I2C_STATE_BUSY_RX) {
    printf("HAL_I2C_STATE_BUSY_RX\r\n");
  } else if (state == HAL_I2C_STATE_LISTEN) {
    printf("HAL_I2C_STATE_LISTEN\r\n");
  } else if (state == HAL_I2C_STATE_BUSY_TX_LISTEN) {
    printf("HAL_I2C_STATE_BUSY_TX_LISTEN\r\n");
  } else if (state == HAL_I2C_STATE_BUSY_RX_LISTEN) {
    printf("HAL_I2C_STATE_BUSY_RX_LISTEN\r\n");
  } else if (state == HAL_I2C_STATE_ABORT) {
    printf("HAL_I2C_STATE_ABORT\r\n");
  } else if (state == HAL_I2C_STATE_TIMEOUT) {
    printf("HAL_I2C_STATE_TIMEOUT\r\n");
  } else if (state == HAL_I2C_STATE_ERROR) {
    printf("HAL_I2C_STATE_ERROR\r\n");
  }
  // while (HAL_I2C_GetState(_bno055_i2c) != HAL_I2C_STATE_READY) {}
  // return;
}

void bno055_readData(uint8_t reg, uint8_t *data, uint8_t len) {

  uint8_t status;
  /*
  status = HAL_I2C_Master_Transmit(_bno055_i2c, BNO055_I2C_ADDR << 1, &reg, 1, 100);
  if (status != HAL_OK) {
	  uint32_t error = HAL_I2C_GetError(_bno055_i2c);
	  if (error == HAL_I2C_ERROR_NONE) {

	  } else if (error == HAL_I2C_ERROR_BERR) {
	    printf("HAL_I2C_ERROR_BERR\r\n");
	  } else if (error == HAL_I2C_ERROR_ARLO) {
	    printf("HAL_I2C_ERROR_ARLO\r\n");
	  } else if (error == HAL_I2C_ERROR_AF) {
	    printf("HAL_I2C_ERROR_AF\r\n");
	  } else if (error == HAL_I2C_ERROR_OVR) {
	    printf("HAL_I2C_ERROR_OVR\r\n");
	  } else if (error == HAL_I2C_ERROR_DMA) {
	    printf("HAL_I2C_ERROR_DMA\r\n");
	  } else if (error == HAL_I2C_ERROR_TIMEOUT) {
	    printf("HAL_I2C_ERROR_TIMEOUT\r\n");
	  }
  }
  */

  /*
  status = HAL_I2C_Master_Receive(_bno055_i2c, BNO055_I2C_ADDR << 1, data, len, 100);
  if (status != HAL_OK) {
	  uint32_t error = HAL_I2C_GetError(_bno055_i2c);
	  if (error == HAL_I2C_ERROR_NONE) {

	  } else if (error == HAL_I2C_ERROR_BERR) {
	    printf("HAL_I2C_ERROR_BERR\r\n");
	  } else if (error == HAL_I2C_ERROR_ARLO) {
	    printf("HAL_I2C_ERROR_ARLO\r\n");
	  } else if (error == HAL_I2C_ERROR_AF) {
	    printf("HAL_I2C_ERROR_AF\r\n");
	  } else if (error == HAL_I2C_ERROR_OVR) {
	    printf("HAL_I2C_ERROR_OVR\r\n");
	  } else if (error == HAL_I2C_ERROR_DMA) {
	    printf("HAL_I2C_ERROR_DMA\r\n");
	  } else if (error == HAL_I2C_ERROR_TIMEOUT) {
	    printf("HAL_I2C_ERROR_TIMEOUT\r\n");
	  }
  }
  */


  for (int i=0;i<len;i++) {


	  uint8_t tmp = reg + i;

	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);

	  status = HAL_I2C_Master_Transmit(_bno055_i2c, BNO055_I2C_ADDR << 1, &tmp, 1, 100);
	  if (status != HAL_OK) {
		  uint32_t error = HAL_I2C_GetError(_bno055_i2c);
		  if (error == HAL_I2C_ERROR_NONE) {

		  } else if (error == HAL_I2C_ERROR_BERR) {
		    printf("HAL_I2C_ERROR_BERR\r\n");
		  } else if (error == HAL_I2C_ERROR_ARLO) {
		    printf("HAL_I2C_ERROR_ARLO\r\n");
		  } else if (error == HAL_I2C_ERROR_AF) {
		    printf("HAL_I2C_ERROR_AF\r\n");
		  } else if (error == HAL_I2C_ERROR_OVR) {
		    printf("HAL_I2C_ERROR_OVR\r\n");
		  } else if (error == HAL_I2C_ERROR_DMA) {
		    printf("HAL_I2C_ERROR_DMA\r\n");
		  } else if (error == HAL_I2C_ERROR_TIMEOUT) {
		    printf("HAL_I2C_ERROR_TIMEOUT\r\n");
		  }
	  }

	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);

	  status = HAL_I2C_Master_Receive(_bno055_i2c, BNO055_I2C_ADDR << 1, data + i, 1, 100);
	  if (status != HAL_OK) {
		  uint32_t error = HAL_I2C_GetError(_bno055_i2c);
		  if (error == HAL_I2C_ERROR_NONE) {

		  } else if (error == HAL_I2C_ERROR_BERR) {
		    printf("HAL_I2C_ERROR_BERR\r\n");
		  } else if (error == HAL_I2C_ERROR_ARLO) {
		    printf("HAL_I2C_ERROR_ARLO\r\n");
		  } else if (error == HAL_I2C_ERROR_AF) {
		    printf("HAL_I2C_ERROR_AF\r\n");
		  } else if (error == HAL_I2C_ERROR_OVR) {
		    printf("HAL_I2C_ERROR_OVR\r\n");
		  } else if (error == HAL_I2C_ERROR_DMA) {
		    printf("HAL_I2C_ERROR_DMA\r\n");
		  } else if (error == HAL_I2C_ERROR_TIMEOUT) {
		    printf("HAL_I2C_ERROR_TIMEOUT\r\n");
		  }
	  }
  }



  //for (int i=0;i<len;i++) printf("0x%02X ", data[i]);
  //printf("\r\n");

	/*
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(_bno055_i2c, BNO055_I2C_ADDR_LO<<1, reg, I2C_MEMADD_SIZE_8BIT, data, len, 100);
	if (status != HAL_OK) {
		  uint32_t error = HAL_I2C_GetError(_bno055_i2c);
		  if (error == HAL_I2C_ERROR_NONE) {

		  } else if (error == HAL_I2C_ERROR_BERR) {
		    printf("HAL_I2C_ERROR_BERR\r\n");
		  } else if (error == HAL_I2C_ERROR_ARLO) {
		    printf("HAL_I2C_ERROR_ARLO\r\n");
		  } else if (error == HAL_I2C_ERROR_AF) {
		    printf("HAL_I2C_ERROR_AF\r\n");
		  } else if (error == HAL_I2C_ERROR_OVR) {
		    printf("HAL_I2C_ERROR_OVR\r\n");
		  } else if (error == HAL_I2C_ERROR_DMA) {
		    printf("HAL_I2C_ERROR_DMA\r\n");
		  } else if (error == HAL_I2C_ERROR_TIMEOUT) {
		    printf("HAL_I2C_ERROR_TIMEOUT\r\n");
		  }
	}
	*/
}
