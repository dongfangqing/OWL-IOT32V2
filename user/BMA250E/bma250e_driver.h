#ifndef _BMA250E_DRIVER_H_
#define _BMA250E_DRIVER_H_


#include "stm32f10x.h"

#include "i2c_driver.h"


extern i2c_device bma250e_dev;

void BMA250E_Init(void);



#endif

