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
#include "application/demo.h"
#include "mcc_generated_files/system.h"

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


