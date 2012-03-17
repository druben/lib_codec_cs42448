#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../i2c/lib_I2C.h"
#include "../menu/menu.h"

#ifndef LIB_DEBUG
//#define LIB_DEBUG
#endif

typedef enum CDC_ENABLE_enum {
	DISABLE = 0,
	ENABLE = 1
} CDC_ENABLE_enum;

typedef enum CDC_PWR_NAME_enum {
	PDN = 0,
	PDN_DAC1 = 1,
	PDN_DAC2 = 2,
	PDN_DAC3 = 3,
	PDN_DAC4 = 4,
	PDN_ADC1 = 5,
	PDN_ADC2 = 6,
	PDN_ADC3 = 7
} CDC_PWR_NAME_enum;

typedef enum CDC_AOUTX_enum {
	Aout1 = 1,
	Aout2 = 2,
	Aout3 = 3,
	Aout4 = 4,
	Aout5 = 5,
	Aout6 = 6,
	Aout7 = 7,
	Aout8 = 8
} CDC_AOUTX_enum;

typedef enum CDC_AINX_enum{
	Ain1 = 1,
	Ain2 = 2,
	Ain3 = 3,
	Ain4 = 4,
	Ain5 = 5,
	Ain6 = 6,
	Ain7 = 7,
	Ain8 = 8
} CDC_AINX_enum;

typedef enum MUTEC_POLARITY_enum{
        ACTIVElow = 0,
        ACTIVEhigh=1
} MUTEC_POLARITY_enum;

typedef enum TRANSITION_enum{
        ADC_SNGVOL = 2,
        MUTEADC_SP = 3,
        AMUTE = 4,
        DAC_SNGVOL = 7
} TRANSITION_enum;

typedef enum ADC_DAC_CONTROL_enum{
        AIN6_MUX = 0,
        AIN5_MUX = 1,
        ADC3SINGLE = 2,
        ADC2SINGLE = 3,
        ADC1SINGLE = 4,
        DAC_DEM = 5,
        ADC3_HPF = 6,
        ADC12_HPF = 7
} ADC_DAC_CONTROL_enum;


typedef enum CDC_MASK_enum{
  ADC1_OVFL = 0,
  ADC2_OVFL = 1,
  ADC3_OVFL = 2,
  ADC_CLC = 3,
  DAC_CLK = 4,      
} CDC_MASK_enum;

/*============================================================================
*  MAP is 8 bits (First bit determines if auto increment is enabled)
*    last 7 bits sets the register address that will be read/write.
*
*    Enables the auto increment of the register
*
*    6.1
*/    


/*
*=============================================================================
*(6.2 on Data Sheet)   Chip I.D. and Revision Register
*                      Address 01h                                                                     
*
*/
typedef struct CHIP_REGISTER{
        char chip_register;
        char address;
} chip_REGISTER;

char CDC_chip_register();

/*============================================================================
* (6.3)       Power control address is located at 0x02h                                                                     
*
*                       POWER DOWN ADC PAIRS
* When enabled the respected ADC Channel Pair will remain in a reset state
* The respected x value:
*     ADC1: AIN1/AIN2   Bit #5
*     ADC2: AIN3/AIN4   Bit #6
*     ADC3: AIN5/AIN6   Bit #7
*     (Bits 5-7 on Power Control)
*     Default = 0
*
*                      POWER DOWN DAC PAIRS
*  When enabled the respected DAC Channel Pair will remain in a reset state
*     Any change of these bits need to be made while the DAC are muted or on
*     the power down bit .           
*     DAC1: AOUT1/AOUT2  Bit #1
*     DAC2: AOUT3/AOUT4  Bit #2
*     DAC3: AOUT5/AOUT6  Bit #3
*     DAC4: AOUT7/AOUT8  Bit #4
*     (Bits 1-4 on Power Control)
*     Default = 0
*                     POWER DOWN
* The entire device will enter a low-power state when this function is enabled.
* The contents of the control register are retained in this mode.
*     (Bit 0 on Power Control)             Default = 0
*/ 

typedef struct CDC_PWR{ 
    CDC_ENABLE_enum p_enable[8];
    char power_control;
    char address;         
}CDC_PWR;

     
/*
*=============================================================================
*(6.4 on Data Sheet)   Function Mode 
*                      Address 03h                                                                     
*
*/
typedef struct FUNCTION_MODE{
        char function_mode; 
        char address;        
} FUNCTION_MODE;


/*
*============================================================================
*(6.5 on DATA Sheet)      Interface Formats 
*                         Address 04h                                                     
*
*/
typedef struct INTERFACE_FORMAT{
        char interface_format;
        char address;       
} INTERFACE_FORMAT;


/*
*============================================================================
*(6.6 on Data Sheet)       ADC Control & DAC De-Emphasis
*                          Address 05h
*                                                    
*/
typedef struct ADC_DAC_control{
        CDC_ENABLE_enum enable_ADC_DAC_cont [8];
        char ADC_DAC_cont;
        char address;
} ADC_DAC_control;


/*
*============================================================================
*(6.7 on Data Sheet)       Transition Control                                                      
*                           Address 06h
*
*/
typedef struct TRANSITION_CONTROL{
        CDC_ENABLE_enum enable_Tcontrol[8];
        char transitionCONTROL;
        char address;
} TRANSITION_control;


/*
*============================================================================
*(6.8)                      DAC Channel Mute
*                           Address 07h                                                 
*
*/
typedef struct CDC_MUTE{ 
    CDC_ENABLE_enum m_enable[8];    
    char   Aout_mute;
    char address;   
}CDC_MUTE;


/*
*=============================================================================
*(6.9)                          AOUTx Volume Control                                              
*                               Address 08h-0hf   For 8 Channels 
*
*/
typedef struct Aoutx_VOL{ 
    uint8_t VOLout[8];
    char VOLout_control [8];
    char Aout1_address;
    char Aout2_address;
    char Aout3_address;
    char Aout4_address;
    char Aout5_address;
    char Aout6_address;
    char Aout7_address;
    char Aout8_address;
}Aoutx_VOL;


/*
*=============================================================================
*(6.10 & 6.12)                 Channel Invert                                             
*                            Address 10h For DAC
*                            Addres 17h for ADC     
*
*/
typedef struct CDC_INV{ 
    CDC_ENABLE_enum INV_Aout_enable[8]; 
    CDC_ENABLE_enum INV_Ain_enable[8];   
    char   INV_Aout;
    char   INV_Ain;  
    char DACinv_address;
    char ADCinv_address; 
}CDC_INV;


/*
*=============================================================================
*(6.11)                          DAC Channel Invert                                             
*                                Address 10h
*
*/
typedef struct Ainx_VOL{
    uint8_t VOLin[8];
    char VOLin_control [6];
    char Ain1_address;
    char Ain2_address;
    char Ain3_address;
    char Ain4_address;
    char Ain5_address;
    char Ain6_address;
}Ainx_VOL;


/*
*=============================================================================
*(6.13)                          Interupt Control                                             
*                                Address 18h
*
*/
typedef struct CDC_INTERUPT{
        char CDC_interupt;
        char address;
} CDC_INTERUPT;


/*===========================================================================
*       6.14                         Status
*       6.15                         Status Mask
*/
typedef struct STATUS{
        CDC_ENABLE_enum CDC_status [8];
        CDC_ENABLE_enum MASK_status_enable [8];
        char MASK_status;
        char error_address;
        char mask_address;
} STATUS;


/*===========================================================================
*       6.16                         MUTEC pin control
*
*/
typedef struct MUTEC_control{
        CDC_ENABLE_enum mutec_enable[8];
        char mutec_control;
        char address;
} MUTEC_control;


/*==========================================================================
*                Storing all of the codec information                                                           
*/

typedef struct CODEC_info {
        struct CHIP_REGISTER chip_register;      //6.2
        struct CDC_PWR power_control;            //6.3
        struct FUNCTION_MODE  function_mode;     //6.4
        struct INTERFACE_FORMAT interface_format;//6.5
        struct ADC_DAC_control ADC_DAC_cont;     //6.6
        struct TRANSITION_CONTROL transitionCONTROL; //6.7
        struct CDC_MUTE Aout_mute;               //6.8
        struct Aoutx_VOL VOLout_control;         //6.9
        struct CDC_INV  INV_Aout_control;        //6.10
        struct Ainx_VOL VOLin_control;           //6.11
        struct CDC_INV  INV_Ain_control;         //6.12
        struct CDC_INTERUPT CDC_interupt;        //6.13      
        struct STATUS status_control;            //6.14
        struct STATUS MASK_status_control;       //6.15
        struct MUTEC_control mutec_control;      //6.16
        char CDC_address;
} CODEC_info;





















/*============================================================================
* (6.3)       Power control address is located at 0x02h                                                                     
*
*                       POWER DOWN ADC PAIRS
* When enabled the respected ADC Channel Pair will remain in a reset state
* The respected x value:
*     ADC1: AIN1/AIN2   Bit #5
*     ADC2: AIN3/AIN4   Bit #6
*     ADC3: AIN5/AIN6   Bit #7
*     (Bits 5-7 on Power Control)
*     Default = 0
*
*                      POWER DOWN DAC PAIRS
*  When enabled the respected DAC Channel Pair will remain in a reset state
*     Any change of these bits need to be made while the DAC are muted or on
*     the power down bit .           
*     DAC1: AOUT1/AOUT2  Bit #1
*     DAC2: AOUT3/AOUT4  Bit #2
*     DAC3: AOUT5/AOUT6  Bit #3
*     DAC4: AOUT7/AOUT8  Bit #4
*     (Bits 1-4 on Power Control)
*     Default = 0
*                     POWER DOWN
* The entire device will enter a low-power state when this function is enabled.
* The contents of the control register are retained in this mode.
*     (Bit 0 on Power Control)             Default = 0
*/ 
void CDC_set_power(CDC_PWR* CDC_power, CDC_ENABLE_enum en, CDC_PWR_NAME_enum name);

     
/*
*=============================================================================
*(6.4 on Data Sheet)   Function Mode 
*                      Address 03h                                                                     
*
*/
char CDC_function_mode();


/*
*============================================================================
*(6.5 on DATA Sheet)      Interface Formats 
*                         Address 04h                                                     
*
*/
char CDC_set_interface_format(CDC_ENABLE_enum en);


/*
*============================================================================
*(6.6 on Data Sheet)       ADC Control & DAC De-Emphasis
*                          Address 05h
*                                                    
*/
void CDC_set_ADC_DAC_control(ADC_DAC_control* ADcontrol, CDC_ENABLE_enum en, ADC_DAC_CONTROL_enum bit);


/*
*============================================================================
*(6.7 on Data Sheet)       Transition Control                                                      
*                           Address 06h
*
*/
void CDC_set_transition_control(TRANSITION_control* Tcontrol, CDC_ENABLE_enum en, TRANSITION_enum bit);


/*
*============================================================================
*(6.8)                      DAC Channel Mute
*                           Address 07h                                                 
*
*/
void CDC_set_mute(CODEC_info* info, CDC_ENABLE_enum en, CDC_AOUTX_enum Aoutx);


/*
*=============================================================================
*(6.9)                          AOUTx Volume Control                                              
*                               Address 08h-0hf   For 8 Channels 
*
*/
void CDC_change_output_vol(Aoutx_VOL* VOL_control, int DB_VOL, CDC_AOUTX_enum Aoutx);


/*
*=============================================================================
*(6.10 & 6.12)                 Channel Invert                                             
*                            Address 10h For DAC
*                            Addres 17h for ADC     
*
*/
void CDC_set_inv_out(CDC_INV* CDC_inv, CDC_ENABLE_enum en, CDC_AOUTX_enum Aoutx);
void CDC_set_inv_in(CDC_INV* CDC_inv, CDC_ENABLE_enum en, CDC_AINX_enum Ainx);


/*
*=============================================================================
*(6.11)                          DAC Channel Invert                                             
*                                Address 10h
*
*/
void CDC_change_input_vol(Ainx_VOL* VOL_control, int DB_VOL, CDC_AINX_enum Ainx);


/*
*=============================================================================
*(6.13)                          Interupt Control                                             
*                                Address 18h
*
*/
char CDC_interupt();


/*===========================================================================
*       6.14                         Status
*       6.15                         Status Mask
*/
void CDC_error(STATUS* errors);
void CDC_set_status_mask(STATUS* MASKstatus, CDC_ENABLE_enum en, CDC_MASK_enum set_interupt);


/*===========================================================================
*       6.16                         MUTEC pin control
*
*/
void CDC_MUTEC_set_control(MUTEC_control* Mcontrol, CDC_ENABLE_enum en);
void CDC_MUTEC_set_polarity(MUTEC_control* Mcontrol, MUTEC_POLARITY_enum active);

/*===========================================================================
 * Initialization routine
 */
int8_t CDC_init(CODEC_info* codec_info);
int8_t CDC_start(CODEC_info* codec_info);

// Only for debug in Dev-C++
#ifdef LIB_DEBUG
void I2CSendData( uint8_t slave_address, char *ptr_data, uint8_t length);
#endif

