/*********************************OWL-IOT32*********************************                                      
 
	                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                                           |
|                             Oooo                          |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)           
***************************************************************************/
#include "bma250e_driver.h"
#include "board_config.h"
#include "bma2x2.h"
#include "i2c_driver.h"
#include "usart_driver.h"
#include "Systick.h"


i2c_device bma250e_dev = {
	.sof_i2c = &sof_i2c1,
	.slave_addr = BMA2x2_I2C_ADDR1 << 1,
};

extern s32 bma2x2_data_readout_template(void);

extern struct bma2x2_t bma2x2;

void BMA250E_Init(void)
{
	/*Local variables for reading accel x, y and z data*/
	s16	accel_x_s16, accel_y_s16, accel_z_s16 = BMA2x2_INIT_VALUE;
	s32 com_rslt = ERROR;
	
	u8 chip_id = 0;
	#ifdef DEVICE_I2C
	if(I2C_CheckDevice(&sof_i2c1,BMA2x2_I2C_ADDR1 << 1) != 0)
	{
#ifdef USART1_DEBUG
		printf("I2C Bus No addrs: %x BMA250E device!!\r\n",BMA2x2_I2C_ADDR1);
#endif
	}
	#endif
	bma2x2_data_readout_template();
	
	bma2x2_read_reg(0x00,&chip_id, 1);	
#ifdef USART1_DEBUG
		printf("chip_id = %x\r\n",chip_id);
#endif
	
//	while(1);
	while(1)
	{
		com_rslt = bma2x2_init(&bma2x2);
		com_rslt += bma2x2_set_power_mode(BMA2x2_MODE_NORMAL);
		com_rslt += bma2x2_set_bw(0x08);
		/* Read the accel X data*/
		com_rslt += bma2x2_read_accel_x(&accel_x_s16);
		/* Read the accel Y data*/
		com_rslt += bma2x2_read_accel_y(&accel_y_s16);
		/* Read the accel Z data*/
		com_rslt += bma2x2_read_accel_z(&accel_z_s16);
#ifdef USART1_DEBUG
		printf("BMA250E accel: x = %d y = %d z = %d\r\n",accel_x_s16,accel_y_s16,accel_z_s16);
#endif
		accel_x_s16 = 0;
		accel_y_s16 = 0;
		accel_z_s16 = 0;
		
		delay_ms(1000);
		delay_ms(1000);
	}
}

