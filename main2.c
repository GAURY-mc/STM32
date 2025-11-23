#include<string.h>
#include<stdint.h>

const char *pMessage = "Hi there , Good Morning !";

const int value = 200;

char myData[50];

int main(void)
   { 
     for (uint32_t i=0 ; i < strlen(pMessage); i++) {
           myData[i] = *(pMessage+i);
       }
      for(;;);
}
