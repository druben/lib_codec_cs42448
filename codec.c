


#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "codec.h"

//#include "lib_I2C.h"



int main (void) {
    
    CODEC_info initCDC;
    
    CDC_initialization(&initCDC);
    
    printf("\n\tCodec address: %X\n\n",initCDC.CDC_address);
    
    CDC_start(&initCDC);
    
    
    //Get code from matt or earl
    //initCDC.status_control
//    error_check.CDC_status[0]=0;
//    error_check.CDC_status[1]=0;
//    error_check.CDC_status[2]=0;
//    error_check.CDC_status[3]=0;
//    error_check.CDC_status[4]=0;
//    CDC_error(&error_check);
    
    enum CDC_ENABLE power_state = ENABLE;
    enum CDC_PWR_NAME DAC_ADC = PDN_ADC3;
    CDC_SET_POWER(&initCDC.power_control, power_state, DAC_ADC);
   
    enum CDC_ENABLE Filter_mux = ENABLE;
    enum ADC_DAC_CONTROL bitnum= ADC1SINGLE;
    set_ADC_DACcontrol(&initCDC.ADC_DAC_cont, Filter_mux, bitnum);
   
    enum CDC_ENABLE mute_state = DISABLE;
    enum CDC_AOUTx mute_Aoutx = Aout2;
    CDC_SET_MUTE(&initCDC.Aout_mute, mute_state ,mute_Aoutx);
   
    enum CDC_ENABLE freeze = ENABLE;   
    INTERFACE_format(freeze);
   
    enum CDC_AOUTx Aoutx_vol = Aout2;
    uint8_t VOL = 33;  //Volume between 0 and -127 dB 
    Change_VOLout(&initCDC.VOLout_control, VOL, Aoutx_vol);
    
    enum CDC_AINx Ainx_vol = Ain2;
    int8_t inVOL = 24; //in Volume between 24 to -64
    Change_VOLin(&initCDC.VOLin_control, inVOL, Ainx_vol);
    
    enum CDC_ENABLE invert_state = ENABLE;
    enum CDC_AOUTx invert_Aoutx = Aout5;
    CDC_SET_INV_OUT(&initCDC.INV_Aout_control,invert_state,invert_Aoutx);
    
    enum CDC_AINx invert_Ainx = Ain5;
    CDC_SET_INV_IN(&initCDC.INV_Ain_control,invert_state,invert_Ainx);

    
    enum CDC_ENABLE TC_state = ENABLE;
    enum TRANSITION_enum control = DAC_SNGVOL;
    set_TRANSITIONcontrol(&initCDC.transitionCONTROL, TC_state, control);
    
    enum MUTEC_polarity Active = ACTIVEhigh; 
    enum CDC_ENABLE MUTEC = DISABLE;
    set_MUTEC_control(&initCDC.mutec_control, MUTEC);
    set_MUTEC_Polarity(&initCDC.mutec_control, Active);
    
    int8_t i;
    for (i = 7; i >= 0; i--)
     { //printf("[%d] = %d\n", i, initCDC.mutec_control.mutec_enable[i]);
         
        initCDC.mutec_control.mutec_control <<= 1;
        initCDC.mutec_control.mutec_control += initCDC.mutec_control.mutec_enable[i]; 
        
     }
     //Not returning correct HEX states
     //printf("\nMUTEC stautes: %X\n",(uint8_t)initCDC.mutec_control.mutec_control);
    
    
    system("PAUSE");
    
}
/*=============================================================================
*                   6.1                                                         
*/




/*============================================================================
*                 6.2    
*                 Default for Codec look at DATA sheet section 6.2                                       
*/ 
char CHIP_register(){
 
 char chip_register = 0x01;
     
}
/*=============================================================================
*                   6.3                                                           
*/

void CDC_SET_POWER(CDC_PWR* CDC_power, CDC_ENABLE en, CDC_PWR_NAME name){

     CDC_power->p_enable[(int)name]=en;
     
     
     int8_t i;
     
     for (i = 7; i >= 0; i--)
     { //printf("Codec[%d] = %d\n", i, CDC_power->p_enable[i]);
         
        CDC_power->power_control <<= 1;
        CDC_power->power_control += CDC_power->p_enable[i]; 
     }
        
       //printf("\nPower Status (in HEX): %X\n",(uint8_t)(CDC_power->power_control));
       
}
/*============================================================================
*                   6.4
*        from Data cheet :11111xx0                                                                   
*/

char FUNCTION_mode(){
     char function_mode = 0xFE;     
}
/*============================================================================
*                   6.5                                                          
*/

char INTERFACE_format(CDC_ENABLE en){
     
     uint8_t FREEZE = (uint8_t)en;
     
     //printf("\nFREEZE = %d\n",FREEZE);
     
     if(FREEZE == 1)
               FREEZE <<= 7;
     
     char interface_format =FREEZE+0x36; //0x36 = 0110110
     
     //printf("Interface Format staues: %X\n",(uint8_t)interface_format);
     
}
/*============================================================================
*                   6.6                                                          
*/


void set_ADC_DACcontrol(ADC_DAC_control* ADcontrol, CDC_ENABLE en, ADC_DAC_CONTROL bit){
     
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
void set_TRANSITIONcontrol(TRANSITION_control* Tcontrol, CDC_ENABLE en, TRANSITION_enum bit){
     
     Tcontrol->enable_Tcontrol[(int)bit]=en;
     
     //printf("\nChanging bit#%d in Transition control\n",bit);
     
     int8_t i;
     
     for (i = 7; i >= 0; i--)
     {   //printf("[%d] = %d\n", i, Tcontrol->enable_Tcontrol[i]);
       
        Tcontrol->transitionCONTROL <<= 1;
        Tcontrol->transitionCONTROL += Tcontrol->enable_Tcontrol[i]; 
     }
     //printf("Mute Status (in HEX): %X\n",(uint8_t)(Tcontrol->transitionCONTROL));  
}
/*============================================================================
*                   6.8                                                          
*/
void CDC_SET_MUTE(CDC_MUTE* CDC_mute, CDC_ENABLE en, CDC_AOUTx Aoutx){
     
     CDC_mute->m_enable[(int)(Aoutx-1)]=en;
     
      //printf("\nAout%d is muted\n",Aoutx);
     
     int8_t i;
     
     for (i = 7; i >= 0; i--)
     {   //printf("Codec[%d] = %d\n", i, CDC_mute->m_enable[i]);
       
        CDC_mute->Aout_mute <<= 1;
        CDC_mute->Aout_mute += CDC_mute->m_enable[i]; 
     }
     //printf("Mute Status (in HEX): %X\n",(uint8_t)(CDC_mute->Aout_mute));      
}

/*============================================================================
*                   6.9                                                          
*/
char voutDEC2BIN(int DB_VOLUME){
 
    uint8_t start_dB=0;
    char Codec_dB = start_dB+2*DB_VOLUME;
         
    return Codec_dB;
}

void Change_VOLout(Aoutx_VOL* VOL_control, int DB_VOL, CDC_AOUTx Aoutx){

     VOL_control->VOLout[(int)Aoutx]= (uint8_t)(voutDEC2BIN(DB_VOL));
     
     //printf("\nAout%d has a volume of -%d (dB) or %X in HEX\n\n", Aoutx, DB_VOL, VOL_control->VOLout[(int)Aoutx]);
     
     int8_t i;
     
     for (i = 7; i >= 0; i--)
     {   //printf("Aout%d = %d dB\n", i, VOL_control->VOLout[i]);
       
        VOL_control->VOLout_control[i]= (uint8_t)(VOL_control->VOLout[i]);
       //printf("[%d] = %u\n", i, (uint8_t)(VOL_control->VOLout_control[i]));
     }   
}

/*============================================================================
*                   6.10 & 6.12                                                          
*/
void CDC_SET_INV_OUT(CDC_INV* CDC_inv, CDC_ENABLE en, CDC_AOUTx Aoutx){
     
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

void CDC_SET_INV_IN(CDC_INV* CDC_inv, CDC_ENABLE en, CDC_AINx Ainx){
     
     CDC_inv->INV_Ain_enable[(int)(Ainx-1)] = en;
     
     //printf("\nInvert Ain%d\n",Ainx);
     
     int8_t i;
     
     for (i = 7; i >= 0; i--)
     {   //printf("Bit[%d] = %d\n", i, CDC_inv->INV_Ain_enable[i]);
       
        CDC_inv->INV_Ain <<= 1;
        CDC_inv->INV_Ain += CDC_inv->INV_Ain_enable[i]; 
     }
     //printf("Ain inverter Status (in HEX): %X\n\n",(uint8_t)(CDC_inv->INV_Ain));  
}
/*============================================================================
*                   6.11                                                        
*/

char vinDEC2BIN(int DB_VOLUME){
 
    int8_t start_dB=0;
    
    char Codec_dB;
    
    if(DB_VOLUME > 24){
         DB_VOLUME = 24;
         printf("Max Vin is 24 dB");
         Codec_dB = start_dB+DB_VOLUME*2;
         return Codec_dB;        
    }
    
    else
        Codec_dB = start_dB+DB_VOLUME*2;
        return Codec_dB;
    
}

void Change_VOLin(Ainx_VOL* VOL_control, int DB_VOL, CDC_AINx Ainx){

     VOL_control->VOLin[(int)Ainx]= (int8_t)(vinDEC2BIN(DB_VOL));
     
     //printf("\nAin%d has a volume of %d (dB) or %X in HEX\n\n", Ainx, DB_VOL, VOL_control->VOLin[(int)Ainx]);
     
     int8_t i;
     
     for (i = 7; i >= 0; i--)
     {   //printf("Ain%d = %d dB\t", i, VOL_control->VOLin[i]);
       
        VOL_control->VOLin_control[i] = (uint8_t)(VOL_control->VOLin[i]);
      // printf("[%d] = %u\n", i, (uint8_t)(VOL_control->VOLin_control[i]));
     }   
}

/*============================================================================
*                   6.13                                                        
*/

char CDC_interupt(){
     char CDC_interupt = 0x04;    
}

/*============================================================================
*                   6.14                                                        
*/

void CDC_error(STATUS* errors){
     
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

void set_status_mask(STATUS* MASKstatus, CDC_ENABLE en, MASK set_interupt){
 
     MASKstatus->MASK_status_enable [(int)set_interupt] = en;
     
     int8_t i;
     
     for (i = 7; i >= 0; i--)
     {   //printf("Bit[%d] = %d\n", i, MASKstatus->MASK_status_enable[i]);
       
        MASKstatus->MASK_status <<= 1;
        MASKstatus->MASK_status += MASKstatus->MASK_status_enable[i]; 
     }
     //printf("Status Mask interupt control: %X\n\n",(uint8_t)(MASKstatus->MASK_status));
     
     
}
/*============================================================================
*                   6.16                                                       
*/

void set_MUTEC_control(MUTEC_control* Mcontrol, CDC_ENABLE en){
     
     Mcontrol->mutec_enable[1] = (int)en;
     
     
}

void set_MUTEC_Polarity(MUTEC_control* Mcontrol, MUTEC_polarity active){
     
     Mcontrol->mutec_enable[0] = (int)active;
}
/*===========================================================================
*          Initializing the Codec with information and Coresponding address                                                       
*/

int8_t CDC_initialization(CODEC_info* codec_info){
       
       codec_info->CDC_address = 0x48; //Codec address of 1001000
       
       //initializing the addresses
       (codec_info->chip_register).address = 0x01;
       (codec_info->power_control).address = 0x02;
       (codec_info->function_mode).address = 0x03;
       (codec_info->interface_format).address = 0x04;
       (codec_info->ADC_DAC_cont).address = 0x05;
       (codec_info->transitionCONTROL).address = 0x06;
       (codec_info->Aout_mute).address = 0x07;
       (codec_info->VOLout_control).Aout1_address = 0x08;
       (codec_info->VOLout_control).Aout2_address = 0x09;
       (codec_info->VOLout_control).Aout3_address = 0x0A;
       (codec_info->VOLout_control).Aout4_address = 0x0B;
       (codec_info->VOLout_control).Aout5_address = 0x0C;
       (codec_info->VOLout_control).Aout6_address = 0x0D;
       (codec_info->VOLout_control).Aout7_address = 0x0E;
       (codec_info->VOLout_control).Aout8_address = 0x0F;
       (codec_info->INV_Aout_control).DACinv_address = 0x10;
       (codec_info->VOLin_control).Ain1_address = 0x11;
       (codec_info->VOLin_control).Ain2_address = 0x12;
       (codec_info->VOLin_control).Ain3_address = 0x13;
       (codec_info->VOLin_control).Ain4_address = 0x14;
       (codec_info->VOLin_control).Ain5_address = 0x15;
       (codec_info->VOLin_control).Ain6_address = 0x16; 
       (codec_info->INV_Ain_control).ADCinv_address = 0x17;
       (codec_info->CDC_interupt).address = 0x18;
       (codec_info->status_control).error_address = 0x19;
       (codec_info->MASK_status_control).mask_address = 0x1A;
       (codec_info->mutec_control).address = 0x1B;
       
       //6.2
       (codec_info->chip_register).chip_register = CHIP_register();
       
       //6.3
       (codec_info->power_control).p_enable[0]=0;
       (codec_info->power_control).p_enable[1]=0;
       (codec_info->power_control).p_enable[2]=0;
       (codec_info->power_control).p_enable[3]=0;
       (codec_info->power_control).p_enable[4]=0;
       (codec_info->power_control).p_enable[5]=0;
       (codec_info->power_control).p_enable[6]=0;
       (codec_info->power_control).p_enable[7]=0;
       
       //6.4
       (codec_info->function_mode).function_mode = FUNCTION_mode();
       
       //6.5
       (codec_info->interface_format).interface_format = 0x36; //From Data sheet
       
       //6.6
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[0]=0;
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[1]=0;
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[2]=1;//To run the ADC in 
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[3]=1;//Single endded
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[4]=1;//inputs
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[5]=0;
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[6]=0;
        (codec_info->ADC_DAC_cont).enable_ADC_DAC_cont[7]=0;
       
       //6.7
       (codec_info->transitionCONTROL).enable_Tcontrol[0]=0;//Must stay 0 //For ADC Soft Ramp
       (codec_info->transitionCONTROL).enable_Tcontrol[1]=1;//Must stay 1 //See Data sheet.
       (codec_info->transitionCONTROL).enable_Tcontrol[2]=0;
       (codec_info->transitionCONTROL).enable_Tcontrol[3]=0;
       (codec_info->transitionCONTROL).enable_Tcontrol[4]=0;
       (codec_info->transitionCONTROL).enable_Tcontrol[5]=0;//Must stay 0 //For DAC Soft Ramp
       (codec_info->transitionCONTROL).enable_Tcontrol[6]=1;//Must stay 1 //See Data sheet.
       (codec_info->transitionCONTROL).enable_Tcontrol[7]=0;
       
       //6.8
        (codec_info->Aout_mute).m_enable[0]=0;
        (codec_info->Aout_mute).m_enable[1]=0;
        (codec_info->Aout_mute).m_enable[2]=0;
        (codec_info->Aout_mute).m_enable[3]=0;
        (codec_info->Aout_mute).m_enable[4]=0;
        (codec_info->Aout_mute).m_enable[5]=0;
        (codec_info->Aout_mute).m_enable[6]=0;
        (codec_info->Aout_mute).m_enable[7]=0;
       
       //6.9
        (codec_info->VOLout_control).VOLout[0]=0;
        (codec_info->VOLout_control).VOLout[1]=0;
        (codec_info->VOLout_control).VOLout[2]=0;
        (codec_info->VOLout_control).VOLout[3]=0;
        (codec_info->VOLout_control).VOLout[4]=0;
        (codec_info->VOLout_control).VOLout[5]=0;
        (codec_info->VOLout_control).VOLout[6]=0;
        (codec_info->VOLout_control).VOLout[7]=0; 
       
       //6.10
        (codec_info->INV_Aout_control).INV_Aout_enable[0]=0;
        (codec_info->INV_Aout_control).INV_Aout_enable[1]=0;
        (codec_info->INV_Aout_control).INV_Aout_enable[2]=0;
        (codec_info->INV_Aout_control).INV_Aout_enable[3]=0;
        (codec_info->INV_Aout_control).INV_Aout_enable[4]=0;
        (codec_info->INV_Aout_control).INV_Aout_enable[5]=0;
        (codec_info->INV_Aout_control).INV_Aout_enable[6]=0;
        (codec_info->INV_Aout_control).INV_Aout_enable[7]=0;
       
       //6.11
        (codec_info->VOLin_control).VOLin[0]=0;
        (codec_info->VOLin_control).VOLin[1]=0;
        (codec_info->VOLin_control).VOLin[2]=0;
        (codec_info->VOLin_control).VOLin[3]=0;
        (codec_info->VOLin_control).VOLin[4]=0;
        (codec_info->VOLin_control).VOLin[5]=0;
        (codec_info->VOLin_control).VOLin[6]=0;
        (codec_info->VOLin_control).VOLin[7]=0;
       
       //6.12
        (codec_info->INV_Ain_control).INV_Ain_enable[0]=0;
        (codec_info->INV_Ain_control).INV_Ain_enable[1]=0;
        (codec_info->INV_Ain_control).INV_Ain_enable[2]=0;
        (codec_info->INV_Ain_control).INV_Ain_enable[3]=0;
        (codec_info->INV_Ain_control).INV_Ain_enable[4]=0;
        (codec_info->INV_Ain_control).INV_Ain_enable[5]=0;
        (codec_info->INV_Ain_control).INV_Ain_enable[6]=0;
        (codec_info->INV_Ain_control).INV_Ain_enable[7]=0;
       //6.13
       (codec_info->CDC_interupt).CDC_interupt = CDC_interupt();
       
       //6.14
       //error: no initialization
       
       //6.15
       (codec_info->MASK_status_control).MASK_status_enable [0] = 0;
       (codec_info->MASK_status_control).MASK_status_enable [1] = 0;
       (codec_info->MASK_status_control).MASK_status_enable [2] = 0;
       (codec_info->MASK_status_control).MASK_status_enable [3] = 0;
       (codec_info->MASK_status_control).MASK_status_enable [4] = 0;
       (codec_info->MASK_status_control).MASK_status_enable [5] = 0;
       (codec_info->MASK_status_control).MASK_status_enable [6] = 0;
       (codec_info->MASK_status_control).MASK_status_enable [7] = 0;
       
       //6.16
        (codec_info->mutec_control).mutec_enable[0]=0;
        (codec_info->mutec_control).mutec_enable[1]=0;
        (codec_info->mutec_control).mutec_enable[2]=0;
        (codec_info->mutec_control).mutec_enable[3]=0;
        (codec_info->mutec_control).mutec_enable[4]=0;
        (codec_info->mutec_control).mutec_enable[5]=0;
        (codec_info->mutec_control).mutec_enable[6]=0;
        (codec_info->mutec_control).mutec_enable[7]=0;
}

int8_t CDC_start(CODEC_info* codec_info){
 
            
            
            
 
            
}

void I2CSendData( uint8_t slave_address, char *ptr_data, uint8_t length) {
	
   printf("Sent it boss!!!\n\n");
   
   
}

void I2CReadData( uint8_t slave_address, char *ptr_data, uint8_t length) {
	
   printf("Read it boss!!!\n\n");
   
   
}
