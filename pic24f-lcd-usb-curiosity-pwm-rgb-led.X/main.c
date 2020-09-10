/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include <stddef.h>
#include "bsp/buttons.h"
#include "bsp/rgb_led3.h"
#include "bsp/led1.h"
#include "bsp/led2.h"
#include "bsp/timer_1ms.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/system.h"

//------------------------------------------------------------------------------
//Application related definitions
//------------------------------------------------------------------------------
#define BUTTON_DEBOUCE_TIME_MS      20

typedef enum
{
    BUTTON_COLOR_RED = 0,
    BUTTON_COLOR_GREEN = 1,
    BUTTON_COLOR_BLUE = 2
} BUTTON_COLOR;

//------------------------------------------------------------------------------
//Private prototypes
//------------------------------------------------------------------------------
static void ButtonS1Debounce(void);
static void ButtonS2Debounce(void);
static void DEMO_Initialize(void);
static void DEMO_Tasks(void);
static void UpdatePotentiometer(void);

//------------------------------------------------------------------------------
//Global variables
//------------------------------------------------------------------------------
static volatile BUTTON_COLOR button_color = BUTTON_COLOR_RED;
static uint16_t potentiometer;
static uint16_t red = 600;
static uint16_t green = 300;
static uint16_t blue = 150;

/*******************************************************************************
  GETTING STARTED
  -----------------------------------------------------------------------------
  To run this demo, please refer to the readme.txt file that is provided with
  this project.  You can find this file attached to the project in the 
  "Documentation" logical folder in the project view in the IDE.
  
  You can also locate the readme.txt file on next to the project folder where
  this demo was extracted.  
  
  The readme.txt contains the details of how to run the demo.
  
  There is an additional hardware.txt file also in the "Documentation" logical
  folder in the project that summarizes the hardware connections of the board.
 ******************************************************************************/

int main(void)
{
    
    SYSTEM_Initialize();
    
    DEMO_Initialize();
    
    while(1)
    {
        DEMO_Tasks();
    }
    
    return 0;
}

//------------------------------------------------------------------------------
//Functions
//------------------------------------------------------------------------------
static void DEMO_Initialize(void)
{
    //Configure the pushbutton pins as digital inputs.
    BUTTON_Enable(BUTTON_S1);
    BUTTON_Enable(BUTTON_S2);
   
    LED1_Off();
    LED2_Off();
    
    RGB_LED3_SetColor(red, green, blue);
    RGB_LED3_On();
    
    
    //Turn on a timer, so to generate periodic interrupts.
    TIMER_SetConfiguration(TIMER_CONFIGURATION_1MS);
    
    //Register the ButtonDebounce() callback function, so it gets called periodically
    //when the timer interrupts occur (in this case at 1:1 rate, so ButtonDebounce()
    //executes once per 1ms).
    TIMER_RequestTick(&ButtonS1Debounce, 1);
    TIMER_RequestTick(&ButtonS2Debounce, 1);
}

static void DEMO_Tasks(void)
{   
    UpdatePotentiometer();
    
    //Use the potentiometer ADC value to set the brightness of the currently
    //selected color channel on the RGB LED.  The "currently selected channel"
    //is manually selected by the user at runtime by pressing the pushbuttons.
    switch(button_color)
    {
        case BUTTON_COLOR_RED:
            red = (potentiometer);
            break;

        case BUTTON_COLOR_GREEN:
            green = (potentiometer);
            break;

        case BUTTON_COLOR_BLUE:
            blue = (potentiometer);
            break;

        default:
            break;
    }

    RGB_LED3_SetColor(red, green, blue);
}

//Helper function that advances the currently selected RGB color channel that
//is to be adjusted next.  This function is called in response to user pushbutton
//press events.
static void ChangeColor(void)
{         
    switch(button_color)
    {
        case BUTTON_COLOR_RED:
            button_color = BUTTON_COLOR_GREEN;
            break;

        case BUTTON_COLOR_GREEN:
            button_color = BUTTON_COLOR_BLUE;
            break;

        case BUTTON_COLOR_BLUE:
            button_color = BUTTON_COLOR_RED;
            break;

        default:
            button_color = BUTTON_COLOR_RED;
            break;
    }
}

//This callback function gets called periodically (1/1ms) by the timer interrupt event
//handler.  This function is used to periodically sample the pushbutton and implements
//a de-bounce algorithm to reject spurious chatter that can occur during press events.
static void ButtonS1Debounce(void)
{
    static uint16_t debounceCounter = 0;
    
    //Sample the button S1 to see if it is currently pressed or not.
    if(BUTTON_IsPressed(BUTTON_S1))
    {
        //The button is currently pressed.  Turn on the general purpose LED.
        LED1_On();
        
        //Check if the de-bounce blanking interval has been satisfied.  If so,
        //advance the RGB color channel user control selector.
        if(debounceCounter == 0)
        {
            ChangeColor();   
        }
        
        //Reset the de-bounce countdown timer, so a new color change operation
        //won't occur until the button is released and remains continuously released 
        //for at least BUTTON_DEBOUCE_TIME_MS.
        debounceCounter = BUTTON_DEBOUCE_TIME_MS;
    }
    else
    {
        //The button is not currently pressed.  Turn off the LED.
        LED1_Off();  
        
        //Allow the de-bounce interval timer to count down, until it reaches 0.
        //Once it reaches 0, the button is effectively "re-armed".
        if(debounceCounter != 0)
        {
            debounceCounter--;
        }
    }  
}

//This callback function gets called periodically (1/1ms) by the timer interrupt event
//handler.  This function is used to periodically sample the pushbutton and implements
//a de-bounce algorithm to reject spurious chatter that can occur during press events.
static void ButtonS2Debounce(void)
{
    static uint16_t debounceCounter = 0;

    //Sample the button S2 to see if it is currently pressed or not.
    if(BUTTON_IsPressed(BUTTON_S2))
    {
        //The button is currently pressed.  Turn on the general purpose LED.
        LED2_On();
        
        //Check if the de-bounce blanking interval has been satisfied.  If so,
        //advance the RGB color channel user control selector.
        if(debounceCounter == 0)
        {
            ChangeColor();   
        }
        
        //Reset the de-bounce countdown timer, so a new color change operation
        //won't occur until the button is released and remains continuously released 
        //for at least BUTTON_DEBOUCE_TIME_MS.
        debounceCounter = BUTTON_DEBOUCE_TIME_MS;
    }
    else
    {
        //The button is not currently pressed.  Turn off the LED.
        LED2_Off(); 
        
        //Allow the de-bounce interval timer to count down, until it reaches 0.
        //Once it reaches 0, the button is effectively "re-armed".
        if(debounceCounter != 0)
        {
            debounceCounter--;
        }
    }    
}

static void UpdatePotentiometer(void)
{
    volatile uint16_t i=0;
   //Enable ADC module
    ADC1_Enable();
    
    //Select the PotentioMeter ADC Channel
    ADC1_ChannelSelect(channel_AN5);
    //Start Sampling
    ADC1_SoftwareTriggerEnable();
    //ADC sampling delay
    for(i=0;i<65535;i++)
    {
        //Do Nothing
    }
    ADC1_SoftwareTriggerDisable();
    //Check if the ADC conversion is completed
    while(!ADC1_IsConversionComplete(channel_AN5))
    {
        //Do Nothing
    }
    // Get the Potentiometer ADC values 
    potentiometer = ADC1_ConversionResultGet(channel_AN5);
}
