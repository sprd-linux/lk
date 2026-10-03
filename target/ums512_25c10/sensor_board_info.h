#ifndef _SENSOR_BOARD_INFO_H_
#define _SENSOR_BOARD_INFO_H_

/*I2C2 -> AP_IIC0-config 0
  I2C5 -> AP_IIC1-config 1
*/
struct sensor_phypara sensor_board_phylist[]={
	{0x2C , 0x00, 0x80 ,"qmc6308", 0},
	{0x30 , 0x39, 0x10 ,"mmc5603", 0},
};

#endif //_SENSOR_BOARD_INFO_H_