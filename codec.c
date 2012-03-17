#include "codec.h"

#ifdef LIB_DEBUG
int main (void) {
    
    CODEC_info initCDC;
    
    CDC_init(&initCDC);
    
    printf("\n\tCodec address: %X\n\n",initCDC.CDC_address);
    
    CDC_start(&initCDC);
    
    
    //Get code from matt or earl
    //initCDC.status_control
//    error_check.CDC_status[0] = 0;
//    error_check.CDC_status[1] = 0;
//    error_check.CDC_status[2] = 0;
//    error_check.CDC_status[3] = 0;
//    error_check.CDC_status[4] = 0;
//    CDC_error(&error_check);
    
    enum CDC_ENABLE_enum power_state = ENABLE;
    enum CDC_PWR_NAME_enum DAC_ADC = PDN_ADC3;
    CDC_set_power(&initCDC.power_control, power_state, DAC_ADC);
   
    enum CDC_ENABLE_enum Filter_mux = ENABLE;
    enum ADC_DAC_CONTROL_enum bitnum= ADC1SINGLE;
    CDC_set_ADC_DAC_control(&initCDC.ADC_DAC_cont, Filter_mux, bitnum);
   
    enum CDC_ENABLE_enum mute_state = DISABLE;
    enum CDC_AOUTX_enum mute_Aoutx = Aout2;
    CDC_set_mute(&initCDC.Aout_mute, mute_state ,mute_Aoutx);
   
    enum CDC_ENABLE_enum freeze = ENABLE;   
    CDC_set_interface_format(freeze);
   
    enum CDC_AOUTX_enum Aoutx_vol = Aout2;
    uint8_t VOL = 33;  //Volume between 0 and -127 dB 
    CDC_change_output_vol(&initCDC.VOLout_control, VOL, Aoutx_vol);
    
    enum CDC_AINX_enum Ainx_vol = Ain2;
    int8_t inVOL = 24; //in Volume between 24 to -64
    CDC_change_input_vol(&initCDC.VOLin_control, inVOL, Ainx_vol);
    
    enum CDC_ENABLE_enum invert_state = ENABLE;
    enum CDC_AOUTX_enum invert_Aoutx = Aout5;
    CDC_set_inv_out(&initCDC.INV_Aout_control,invert_state,invert_Aoutx);
    
    enum CDC_AINX_enum invert_Ainx = Ain5;
    CDC_set_inv_in(&initCDC.INV_Ain_control,invert_state,invert_Ainx);

    
    enum CDC_ENABLE_enum TC_state = ENABLE;
    enum TRANSITION_enum control = DAC_SNGVOL;
    CDC_set_transition_control(&initCDC.transitionCONTROL, TC_state, control);
    
    enum MUTEC_POLARITY_enum Active = ACTIVEhigh; 
    enum CDC_ENABLE_enum MUTEC = DISABLE;
    CDC_MUTEC_set_control(&initCDC.mutec_control, MUTEC);
    CDC_MUTEC_set_polarity(&initCDC.mutec_control, Active);
    
    int8_t i;
    for (i = 7; i >= 0; i--)
     { //printf("[%d] = %d\n", i, initCDC.mutec_control.mutec_enable[i]);
         
        initCDC.mutec_control.mutec_control <<= 1;
        initCDC.mutec_control.mutec_control += initCDC.mutec_control.mutec_enable[i]; 
        
     }
     //Not returning correct HEX states
     //printf("\nMUTEC stautes: %X\n",(uint8_t)initCDC.mutec_control.mutec_control);
    
    
    system("PAUSE");
    
}*/
#endif

/*=============================================================================
*                   6.1                                                         
*/




/*============================================================================
*                 6.2    
*                 Default for Codec look at DATA sheet section 6.2                                       
*/ 
char CDC_chip_register() {
	return 0x01;
}

/*=============================================================================
*                   6.3                                                           
*/
void CDC_set_power(CODEC_info* info, CDC_ENABLE_enum en, CDC_PWR_NAME_enum name) {

	 CDC_PWR* CDC_power = &(info->power_control);
     CDC_power->p_enable[(int)name] = en;
     
     int8_t i;  
     for (i = 7; i >= 0; i--)
     { //printf("Codec[%d] = %d\n", i, CDC_power->p_enable[i]);
         
        CDC_power->power_control <<= 1;
        CDC_power->power_control += CDC_power->p_enable[i]; 
     }
        
       //printf("\nPower Status (in HEX): %X\n",(uint8_t)(CDC_power->power_control));
     char temp[2];
     temp[0] = CDC_power->address;
     temp[1] = CDC_power->power_control;
     
     I2CSendData(info->address, &(temp[0]), 2);
}


/*============================================================================
*                   6.4
*        from Data sheet :11111xx0                                                                   
*/

char CDC_function_mode() {
     return 0xFE;     
}
/*============================================================================
*                   6.5                                                          
*/

char CDC_set_interface_format(CDC_ENABLE_enum en) {
     
     uint8_t FREEZE = (uint8_t)en;
     
     //printf("\nFREEZE = %d\n",FREEZE);
     
     if(FREEZE == 1)
               FREEZE <<= 7;
     
     return (FREEZE + 0x36); //0x36 = 0110110
     
     //printf("Interface Format staues: %X\n",(uint8_t)interface_format);
     
}
/*============================================================================
*                   6.6                                                          
*/


void CDC_set_ADC_DAC_control(ADC_DAC_control* ADcontrol, CDC_ENABLE_enum en, ADC_DAC_CONTROL_enum bit) {
     
     ADcontrol->enable_ADC_DAC_cont[(int)bit] = en;
     
     int8_t i;
     
     for (i = 7; i >= 0; i--)
     {   //printf("[%d] = %d\n", i, Tcontrol->enable_Tcontrol[i]);
       
        ADcontrol->ADC_DAC_cont <<= 1;
        ADcontrol->ADC_DAC_cont += ADcontrol->enable_ADC_DAC_cont[i]; 
     }
     //printf("\nADC Control & DAC DE-emphasis control: (in HEX): %X\n",(uint8_t)(ADcontrol->ADC_DAC_cont));
     
}
/*============================================================================
*                   6.7  
*                                                   
*/                  
void CDC_set_transition_control(CODEC_info* info, CDC_ENABLE_enum en, TRANSITION_enum bit) {
     
     TRANSITION_control* Tcontrol = &(info->transitionCONTROL);
     Tcontrol->enable_Tcontrol[(int)bit] = en;
     
     //printf("\nChanging bit#%d in Transition control\n",bit);
     
     int8_t i;
     for (i = 7; i >= 0; i--)
     {   //printf("[%d] = %d\n", i, Tcontrol->enable_Tcontrol[i]);
       
        Tcontrol->transitionCONTROL <<= 1;
        Tcontrol->transitionCONTROL += Tcontrol->enable_Tcontrol[i]; 
     }
     //printf("Mute Status (in HEX): %X\n",(uint8_t)(Tcontrol->transitionCONTROL));
     char temp[2];
     temp[0] = Tcontrol->address;
     temp[1] = Tcontrol->transitionCONTROL;
     
     I2CSendData(info->address, &(temp[0]), 2);
}
/*============================================================================
*                   6.8                                                          
*/
void CDC_set_mute(CODEC_info* info, CDC_ENABLE_enum en, CDC_AOUTX_enum Aoutx) {
     CDC_MUTE* CDC_mute = &(info->Aout_mute);
     CDC_mute->m_enable[(int)(Aoutx-1)] = en;
     
      //printf("\nAout%d is muted\n",Aoutx);
     
     int8_t i;
     for (i = 7; i >= 0; i--)
     {   //printf("Codec[%d] = %d\n", i, CDC_mute->m_enable[i]);
        CDC_mute->Aout_mute <<= 1;
        CDC_mute->Aout_mute += CDC_mute->m_enable[i]; 
     }
     //printf("Mute Status (in HEX): %X\n",(uint8_t)(CDC_mute->Aout_mute));
     
     char temp[2];
     temp[0] = info->Aout_mute.address;
     temp[1] = CDC_mute->Aout_mute;
     
     I2CSendData(info->address, &(temp[0]), 2);
}

/*============================================================================
*                   6.9                                                          
*/
// Must input volume in positive dB
char voutDEC2BIN(int DB_VOLUME) {
 
    uint8_t start_dB = 0;
    char Codec_dB = start_dB+2*DB_VOLUME;
         
    return Codec_dB;
}

// Must input volume in positive dB
void CDC_change_output_vol(CODEC_info* info, int DB_VOL, CDC_AOUTX_enum Aoutx) {
	Aoutx_VOL* VOL_control = &(info->VOLout_control);
     VOL_control->VOLout[(int)Aoutx] = (uint8_t)(voutDEC2BIN(DB_VOL));
     
     //printf("\nAout%d has a volume of -%d (dB) or %X in HEX\n\n", Aoutx, DB_VOL, VOL_control->VOLout[(int)Aoutx]);
     
     int8_t i;
     
     for (i = 7; i >= 0; i--)
     {   //printf("Aout%d = %d dB\n", i, VOL_control->VOLout[i]);
       
        VOL_control->VOLout_control[i] = (uint8_t)(VOL_control->VOLout[i]);
       //printf("[%d] = %u\n", i, (uint8_t)(VOL_control->VOLout_control[i]));
     }
     
     char temp[2];
     temp[0] = VOL_control->address[(uint8_t)Aoutx];
     temp[1] = VOL_control->VOLout_control[(uint8_t)Aoutx];
     
     I2CSendData(info->address, &(temp[0]), 2);
}

/*============================================================================
*                   6.10 & 6.12                                                          
*/
void CDC_set_inv_out(CDC_INV* CDC_inv, CDC_ENABLE_enum en, CDC_AOUTX_enum Aoutx) {
     
     CDC_inv->INV_Aout_enable[(int)(Aoutx-1)] = en;
     
    //printf("\nInvert Aout%d\n",Aoutx);
     
     int8_t i;
     for (i = 7; i >= 0; i--)
     {   //printf("BIT[%d] = %d\n", i, CDC_inv->INV_Aout_enable[i]);
       
        CDC_inv->INV_Aout <<= 1;
        CDC_inv->INV_Aout += CDC_inv->INV_Aout_enable[i]; 
     }
     //printf("Aout inverter Status (in HEX): %X\n",(uint8_t)(CDC_inv->INV_Aout)); 
}

void CDC_set_inv_in(CDC_INV* CDC_inv, CDC_ENABLE_enum en, CDC_AINX_enum Ainx) {
     
     CDC_inv->INV_Ain_enable[(int)(Ainx-1)] = en;
     
     //printf("\nInvert Ain%d\n",Ainx);
     
     int8_t i;
     
     for (i = 7; i >= 0; i--) {
     	//printf("Bit[%d] = %d\n", i, CDC_inv->INV_Ain_enable[i]);
       
        CDC_inv->INV_Ain <<= 1;
        CDC_inv->INV_Ain += CDC_inv->INV_Ain_enable[i]; 
     }
     //printf("Ain inverter Status (in HEX): %X\n\n",(uint8_t)(CDC_inv->INV_Ain));  
}
/*============================================================================
*                   6.11                                                        
*/

// Input volume can be positive or negative dB
char vinDEC2BIN(int DB_VOLUME) {
 
    int8_t start_dB = 0;
    
    char Codec_dB;
    
    if(DB_VOLUME > 24) {
         DB_VOLUME = 24;
         //printf("Max Vin is 24 dB");
         Codec_dB = start_dB+DB_VOLUME*2;
         return Codec_dB;        
    }
    
    else
        Codec_dB = start_dB+DB_VOLUME*2;
        return Codec_dB;
    
}

void CDC_change_input_vol(CODEC_info* info, int DB_VOL, CDC_AINX_enum Ainx) {
	 Ainx_VOL* VOL_control = &(info->VOLin_control);
     VOL_control->VOLin[(uint8_t)Ainx] = (int8_t)(vinDEC2BIN(DB_VOL));
     
     //printf("\nAin%d has a volume of %d (dB) or %X in HEX\n\n", Ainx, DB_VOL, VOL_control->VOLin[(int)Ainx]);
     
     int8_t i;
     for (i = 7; i >= 0; i--) {
     	//printf("Ain%d = %d dB\t", i, VOL_control->VOLin[i]);
       
        VOL_control->VOLin_control[i] = (uint8_t)(VOL_control->VOLin[i]);
     	//printf("[%d] = %u\n", i, (uint8_t)(VOL_control->VOLin_control[i]));
     }
     char temp[2];
     temp[0] = VOL_control->address[(uint8_t)Ainx];
     temp[1] = VOL_control->VOLin_control[(uint8_t)Ainx];
     
     I2CSendData(info->address, &(temp[0]), 2);
}

/*============================================================================
*                   6.13                                                        
*/

char CDC_interupt() {
     return 0x04;    
}

/*============================================================================
*                   6.14                                                        
*/

void CDC_error(STATUS* errors) {
     
     if(errors->CDC_status[4] == 1)
        printf("\n\t!DAC Clock Error see 6.14.1 in Data sheet!\t\n");
     if(errors->CDC_status[3] == 1)
        printf("\n\t!ADC Clock Error see 6.14.2 in Data sheet!\t\n");
     if(errors->CDC_status[2] == 1)
        printf("\n\t!ADC3 Overflow see 6.14.3 in Data sheet!\t\n");    
     if(errors->CDC_status[1] == 1)
        printf("\n\t!ADC2 Overflow see 6.14.3 in Data sheet!\t\n");
     if(errors->CDC_status[0] == 1)
        printf("\n\t!ADC1 Overflow see 6.14.3 in Data sheet!\t\n");            
                         
     
}
/*============================================================================
*                   6.15                                                        
*/

void CDC_set_status_mask(STATUS* MASKstatus, CDC_ENABLE_enum en, CDC_MASK_enum set_interupt) {
 
     MASKstatus->MASK_status_enable [(int)set_interupt] = en;
     
     int8_t i;
     
     for (i = 7; i >= 0; i--) {   
     	//printf("Bit[%d] = %d\n", i, MASKstatus->MASK_status_enable[i]);
       
        MASKstatus->MASK_status <<= 1;
        MASKstatus->MASK_status += MASKstatus->MASK_status_enable[i]; 
     }
	//printf("Status Mask interupt control: %X\n\n",(uint8_t)(MASKstatus->MASK_status)); 
}


/*============================================================================
*                   6.16                                                       
*/

void CDC_MUTEC_set_control(MUTEC_control* Mcontrol, CDC_ENABLE_enum en) {
     
     Mcontrol->mutec_enable[1] = en;
}

void CDC_MUTEC_set_polarity(MUTEC_control* Mcontrol, MUTEC_POLARITY_enum active) {
     
     Mcontrol->mutec_enable[0] = (CDC_ENABLE_enum)active;
}


/*===========================================================================
*          Initializing the Codec with information and Coresponding address                                                       
*/

int8_t CDC_init(CODEC_info* codec_info) {
       
       codec_info->address = 0x90; //Codec address of 1001 0000
       
       //initializing the addresses
       (codec_info->chip_register).address = 0x01;
       (codec_info->power_control).address = 0x02;
       (codec_info->function_mode).address = 0x03;
       (codec_info->interface_format).address = 0x04;
       (codec_info->ADC_DAC_cont).address = 0x05;
       (codec_info->transitionCONTROL).address = 0x06;
       (codec_info->Aout_mute).address = 0x07;
       (codec_info->VOLout_control).address[0] = 0x08;
       (codec_info->VOLout_control).address[1] = 0x09;
       (codec_info->VOLout_control).address[2] = 0x0A;
       (codec_info->VOLout_control).address[3] = 0x0B;
       (codec_info->VOLout_control).address[4] = 0x0C;
       (codec_info->VOLout_control).address[5] = 0x0D;
       (codec_info->VOLout_control).address[6] = 0x0E;
       (codec_info->VOLout_control).address[7] = 0x0F;
       (codec_info->INV_Aout_control).DACinv_address = 0x10;
       (codec_info->VOLin_control).address[0] = 0x11;
       (codec_info->VOLin_control).address[1] = 0x12;
       (codec_info->VOLin_control).address[2] = 0x13;
       (codec_info->VOLin_control).address[3] = 0x14;
       (codec_info->VOLin_control).address[4] = 0x15;
       (codec_info->VOLin_control).address[5] = 0x16; 
       (codec_info->INV_Ain_control).ADCinv_address = 0x17;
       (codec_info->CDC_interupt).address = 0x18;
       (codec_info->status_control).error_address = 0x19;
       (codec_info->MASK_status_control).mask_address = 0x1A;
       (codec_info->mutec_control).address = 0x1B;
       
       //6.2  TODO:  This should be read from the chip if important
       (codec_info->chip_register).chip_register = CDC_chip_register();
       
       //6.3
       (codec_info->power_control).p_enable[0] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->power_control).p_enable[1] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->power_control).p_enable[2] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->power_control).p_enable[3] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->power_control).p_enable[4] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->power_control).p_enable[5] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->power_control).p_enable[6] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->power_control).p_enable[7] = (CDC_ENABLE_enum)DISABLE;
       
       //6.4
       (codec_info->function_mode).function_mode = CDC_function_mode();
       
       //6.5
       (codec_info->interface_format).interface_format = 0x36; //From Data sheet
       
       //6.6
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[0] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[1] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[2] = (CDC_ENABLE_enum)ENABLE;//To run the ADC in 
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[3] = (CDC_ENABLE_enum)ENABLE;//Single endded
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[4] = (CDC_ENABLE_enum)ENABLE;//inputs
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[5] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[6] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[7] = (CDC_ENABLE_enum)DISABLE;
       
       //6.7
       (codec_info->transitionCONTROL).enable_Tcontrol[0] = (CDC_ENABLE_enum)DISABLE; //Must stay 0 //For ADC Soft Ramp
       (codec_info->transitionCONTROL).enable_Tcontrol[1] = (CDC_ENABLE_enum)ENABLE; //Must stay 1 //See Data sheet.
       (codec_info->transitionCONTROL).enable_Tcontrol[2] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->transitionCONTROL).enable_Tcontrol[3] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->transitionCONTROL).enable_Tcontrol[4] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->transitionCONTROL).enable_Tcontrol[5] = (CDC_ENABLE_enum)DISABLE; //Must stay 0 //For DAC Soft Ramp
       (codec_info->transitionCONTROL).enable_Tcontrol[6] = (CDC_ENABLE_enum)ENABLE; //Must stay 1 //See Data sheet.
       (codec_info->transitionCONTROL).enable_Tcontrol[7] = (CDC_ENABLE_enum)DISABLE;
       
       //6.8
        (codec_info->Aout_mute).m_enable[0] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->Aout_mute).m_enable[1] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->Aout_mute).m_enable[2] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->Aout_mute).m_enable[3] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->Aout_mute).m_enable[4] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->Aout_mute).m_enable[5] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->Aout_mute).m_enable[6] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->Aout_mute).m_enable[7] = (CDC_ENABLE_enum)DISABLE;
       
       //6.9
        (codec_info->VOLout_control).VOLout[0] = 0;
        (codec_info->VOLout_control).VOLout[1] = 0;
        (codec_info->VOLout_control).VOLout[2] = 0;
        (codec_info->VOLout_control).VOLout[3] = 0;
        (codec_info->VOLout_control).VOLout[4] = 0;
        (codec_info->VOLout_control).VOLout[5] = 0;
        (codec_info->VOLout_control).VOLout[6] = 0;
        (codec_info->VOLout_control).VOLout[7] = 0; 
       
       //6.10
        (codec_info->INV_Aout_control).INV_Aout_enable[0] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Aout_control).INV_Aout_enable[1] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Aout_control).INV_Aout_enable[2] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Aout_control).INV_Aout_enable[3] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Aout_control).INV_Aout_enable[4] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Aout_control).INV_Aout_enable[5] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Aout_control).INV_Aout_enable[6] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Aout_control).INV_Aout_enable[7] = (CDC_ENABLE_enum)DISABLE;
       
       //6.11
        (codec_info->VOLin_control).VOLin[0] = 0;
        (codec_info->VOLin_control).VOLin[1] = 0;
        (codec_info->VOLin_control).VOLin[2] = 0;
        (codec_info->VOLin_control).VOLin[3] = 0;
        (codec_info->VOLin_control).VOLin[4] = 0;
        (codec_info->VOLin_control).VOLin[5] = 0;
        (codec_info->VOLin_control).VOLin[6] = 0;
        (codec_info->VOLin_control).VOLin[7] = 0;
       
       //6.12
        (codec_info->INV_Ain_control).INV_Ain_enable[0] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Ain_control).INV_Ain_enable[1] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Ain_control).INV_Ain_enable[2] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Ain_control).INV_Ain_enable[3] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Ain_control).INV_Ain_enable[4] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Ain_control).INV_Ain_enable[5] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Ain_control).INV_Ain_enable[6] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->INV_Ain_control).INV_Ain_enable[7] = (CDC_ENABLE_enum)DISABLE;
       //6.13
       (codec_info->CDC_interupt).CDC_interupt = CDC_interupt();
       
       //6.14
       //error: no initialization
       
       //6.15
       (codec_info->MASK_status_control).MASK_status_enable [0] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->MASK_status_control).MASK_status_enable [1] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->MASK_status_control).MASK_status_enable [2] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->MASK_status_control).MASK_status_enable [3] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->MASK_status_control).MASK_status_enable [4] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->MASK_status_control).MASK_status_enable [5] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->MASK_status_control).MASK_status_enable [6] = (CDC_ENABLE_enum)DISABLE;
       (codec_info->MASK_status_control).MASK_status_enable [7] = (CDC_ENABLE_enum)DISABLE;
       
       //6.16
        (codec_info->mutec_control).mutec_enable[0] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->mutec_control).mutec_enable[1] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->mutec_control).mutec_enable[2] = (CDC_ENABLE_enum)DISABLE; //(2-7 are reserved)
        (codec_info->mutec_control).mutec_enable[3] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->mutec_control).mutec_enable[4] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->mutec_control).mutec_enable[5] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->mutec_control).mutec_enable[6] = (CDC_ENABLE_enum)DISABLE;
        (codec_info->mutec_control).mutec_enable[7] = (CDC_ENABLE_enum)DISABLE;
        
        return 1;
}


int8_t CDC_start(CODEC_info* codec_info) {
	
	CDC_set_mute(codec_info, ENABLE, Aout1);
	CDC_set_mute(codec_info, ENABLE, Aout2);
	delaymycode(30);
	CDC_set_mute(codec_info, DISABLE,Aout1);
	CDC_set_mute(codec_info, DISABLE,Aout2);
	
	
	          
 return 1;
            
}

#ifdef LIB_DEBUG
void I2CSendData( uint8_t slave_address, char *ptr_data, uint8_t length) {
   printf("Sent it boss!!!\n\n");
}

void I2CReadData( uint8_t slave_address, char *ptr_data, uint8_t length) {
	
   printf("Read it boss!!!\n\n");
}

#endif
