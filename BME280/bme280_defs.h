/*
 * bme280_defs.h
 *
 *  Created on: 5 Sep 2019
 *      Author: rato
 */

#ifndef BME280_DEFS_H_
#define BME280_DEFS_H_

#define BME280_DEVICE_I2C_ADDRESS (0xEC>>1)

/* * * * * * * * * * * * * * * * * *
 * BME280 MEMORY MAP
 * * * * * * * * * * * * * * * * * */

#define	BME_DIG_T1_7_0      0x88    // read only
#define	BME_DIG_T1_15_8	    0x89    // read only
#define	BME_DIG_T2_7_0	    0x8A    // read only
#define	BME_DIG_T2_15_8	    0x8B    // read only
#define	BME_DIG_T3_7_0	    0x8C    // read only
#define	BME_DIG_T3_15_8	    0x8D    // read only
#define	BME_DIG_P1_7_0	    0x8E    // read only
#define	BME_DIG_P1_15_8	    0x8F    // read only
#define	BME_DIG_P2_7_0	    0x90    // read only
#define	BME_DIG_P2_15_8	    0x91    // read only
#define	BME_DIG_P3_7_0	    0x92    // read only
#define	BME_DIG_P3_15_8	    0x93    // read only
#define	BME_DIG_P4_7_0	    0x94    // read only
#define	BME_DIG_P4_15_8	    0x95    // read only
#define	BME_DIG_P5_7_0	    0x96    // read only
#define	BME_DIG_P5_15_8	    0x97    // read only
#define	BME_DIG_P6_7_0	    0x98    // read only
#define	BME_DIG_P6_15_8	    0x99    // read only
#define	BME_DIG_P7_7_0	    0x9A    // read only
#define	BME_DIG_P7_15_8	    0x9B    // read only
#define	BME_DIG_P8_7_0	    0x9C    // read only
#define	BME_DIG_P8_15_8	    0x9D    // read only
#define	BME_DIG_P9_7_0	    0x9E    // read only
#define	BME_DIG_P9_15_8	    0x9F    // read only


#define	BME_DIG_H1_7_0	    0xA1    // read only
#define	BME_DIG_H2_7_0      0xE1    // read only
#define	BME_DIG_H2_15_8     0xE2    // read only
#define	BME_DIG_H3_7_0      0xE3    // read only
#define	BME_DIG_H4_11_4     0xE4    // read only
#define	BME_DIG_H4_3_0      0xE5    // read only
#define	BME_DIG_H5_3_0      0xE5    // read only
#define	BME_DIG_H5_11_4     0xE6    // read only
#define	BME_DIG_H6          0xE7    // read only


#define	BME_CHIP_ID	    0xD0    //read only
#define	BME_RESET	    0xE0    //write only
#define	BME_CTRL_HUM	0xF2    // write / read
#define	BME_STATUS	    0xF3    // read only
#define	BME_CTRL_MEAS	0xF4    // write / read
#define	BME_CONFIG	    0xF5    // write / read

#define	BME_PRESS_MSB	0xF7    // read only
#define	BME_PRESS_LSB	0xF8    // read only
#define	BME_PRESS_XLSB	0xF9    // read only
#define	BME_TEMP_MSB	0xFA    // read only
#define	BME_TEMP_LSB	0xFB    // read only
#define	BME_TEMP_XLSB	0xFC    // read only
#define	BME_HUM_MSB	    0xFD    // read only
#define	BME_HUM_LSB	    0xFE    // read only

/* BME280 bits positions */

#define BME_CTRL_HUM_OSRH_H_Pos   0x00
#define BME_CTRL_MEAS_MODE_Pos    0x00
#define BME_CTRL_MEAS_OSRP_S_Pos  0x02
#define BME_CTRL_MEAS_OSRT_S_Pos  0x05
#define BME_CONFIG_SPI3W_EN_Pos   0x00
#define BME_CONFIG_FILTER_Pos     0x02
#define BME_CONFIG_T_SB_Pos       0x05
#define BME_STATUS_IM_UPDATE_Pos  0x00
#define BME_STATUS_MEASURING_Pos  0x03
#define	BME_PRESS_XLSB_Pos	      0x04
#define	BME_TEMP_XLSB_Pos	      0x04

/* BME280 bit masks */
#define BME_CTRL_HUM_OSRS_H_Mask   0x07
#define BME_CTRL_MEAS_MODE_Mask    0x03
#define BME_CTRL_MEAS_OSRS_P_Mask  0x1C
#define BME_CTRL_MEAS_OSRS_T_Mask  0xE0
#define BME_CONFIG_SPI3W_EN_Mask   0x01
#define BME_CONFIG_FILTER_Mask     0x1C
#define BME_CONFIG_T_SB_Mask       0xE0
#define BME_STATUS_IM_UPDATE_Mask  0x01
#define BME_STATUS_MEASURING_Mask  0x08
#define	BME_PRESS_XLSB_Mask	       0xF0
#define	BME_TEMP_XLSB_Mask	       0xF0



/*********************************************
 *
 *  CTRL_MEAS REGISTER SETTINGS
 *
 *********************************************/
/* BME280 Chip mode */
#define BME_MODE_SLEEP			   0x00
#define BME_MODE_FORCED            0x10
#define BME_NORMAL_NORMAL          0x11


/* BME280 Pressure oversampling settings macros */
#define BME_PRESSURE_OVERSAMPLING_OFF  0x00
#define BME_PRESSURE_OVERSAMPLING_x1   0x01
#define BME_PRESSURE_OVERSAMPLING_x2   0x02
#define BME_PRESSURE_OVERSAMPLING_x4   0x03
#define BME_PRESSURE_OVERSAMPLING_x8   0x04
#define BME_PRESSURE_OVERSAMPLING_x16  0x05

/* BME280 Temperature oversampling settings macros */
#define BME_TEMPERATURE_OVERSAMPLING_OFF  0x00
#define BME_TEMPERATURE_OVERSAMPLING_x1   0x01
#define BME_TEMPERATURE_OVERSAMPLING_x2   0x02
#define BME_TEMPERATURE_OVERSAMPLING_x4   0x03
#define BME_TEMPERATURE_OVERSAMPLING_x8   0x04
#define BME_TEMPERATURE_OVERSAMPLING_x16  0x05

/*********************************************
 *
 *  CTRL_HUM REGISTER SETTINGS
 *
 *********************************************/

/* BME280 Humidity oversampling settings macros */
#define BME_HUMIDITY_OVERSAMPLING_OFF  0x00
#define BME_HUMIDITY_OVERSAMPLING_x1   0x01
#define BME_HUMIDITY_OVERSAMPLING_x2   0x02
#define BME_HUMIDITY_OVERSAMPLING_x4   0x03
#define BME_HUMIDITY_OVERSAMPLING_x8   0x04
#define BME_HUMIDITY_OVERSAMPLING_x16  0x05


/*********************************************
 *
 *  CONFIG REGISTER SETTINGS
 *
 *********************************************/
#define BME_T_STANDBY_0_5US				0x00
#define BME_T_STANDBY_62_5MS			0x01
#define BME_T_STANDBY_125MS				0x02
#define BME_T_STANDBY_250MS             0x04
#define BME_T_STANDBY_1000MS            0x05
#define BME_T_STANDBY_10MS              0x06
#define BME_T_STANDBY_20MS              0x07

#endif /* BME280_DEFS_H_ */
