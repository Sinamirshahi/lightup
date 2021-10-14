#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

unsigned long ToUInt(char* str);
char* concat(const char *s1, const char *s2);
int setlight(char *);
int getlight();

int main(int argc, char *argv[] )  {  

       if(argc < 2){  
       int current_light = getlight();
       printf("The brighness is at %d\n",current_light);
       return getlight();
   }  

   if (strcmp(argv[1],"--help")==0){
       printf("\n\n\nUsage:\n\nLightup [percentage] will set the light to that value. ex: lightup 25\n\n");
       printf("You can also increase the light by percentage to a positive or negetive value. ex: lightup -10\n\n");
       printf("Running lightup without any value will return the current brightness\n\n");
       printf("use --no-transition at the end to cancel smooth transition\n\n\n");

       return getlight();
   }
    int transition = 1;
    if (argc >=3 )
        if (strcmp(argv[2], "--no-transition")==0){
            transition = 0;

   }
    if(argv[1][0]==43 || argv[1][0]==45){
        int dest=getlight()+atoi(argv[1])+1;
        if (dest < 0){
            dest = 0;
        } else if (dest>100){
            dest = 100;
        }
        

        
        char sdest[10];
        sprintf(sdest, "%d", dest);
        strcpy(argv[1], sdest);
    }

 
   if(argc < 2){  
       int current_light = getlight();
       printf("The brighness is at %d",current_light);
   }  

   else {  
    int current_light = getlight();
    char svalue[10];
    float time = 0.0;
    if (ToUInt(argv[1]) >= current_light){
        for (int i= current_light; i <= ToUInt(argv[1]); i++){
                    sprintf(svalue, "%d", i);

                    if (transition == 0)
                        sprintf(svalue, "%d", (int)ToUInt(argv[1]));


                   setlight(svalue);
                   if (i >= 100 || transition == 0)
                        break;
                    if (transition == 1)
                        time = 1/(float)(ToUInt(argv[1])-current_light);
                   sleep(time);

        }
    }
    else if (ToUInt(argv[1]) < current_light) {
            for (int i= current_light; i > ToUInt(argv[1]); i--){
                   sprintf(svalue, "%d", i);
                   if (transition == 0)
                        sprintf(svalue, "%d", (int)ToUInt(argv[1]));

                   setlight(svalue);
                   if (i<=0 || transition == 0)
                        break;
                    if (transition == 1)
                        time = 1/(float)(current_light-ToUInt(argv[1]));
                   sleep(time);

        }
    }

    }

    return getlight();
}


int getlight(){

         FILE *fp1;
         char line1[130];
         fp1 = popen( "cat /sys/class/backlight/intel_backlight/max_brightness" , "r");
         fgets( line1, sizeof line1, fp1);
         line1[strcspn(line1, "\n")] = 0;
         pclose(fp1);
         int max_brightness = ToUInt(line1);

         ////
         FILE *fp2;
         char line2[130];
         fp2 = popen( "cat /sys/class/backlight/intel_backlight/brightness" , "r");
         fgets( line2, sizeof line2, fp2);
         line2[strcspn(line2, "\n")] = 0;
         pclose(fp2);
         int brightness = ToUInt(line2);

         float ratio;
         return (int)(((float)brightness / (float) max_brightness) * 100);
}

int setlight(char* value){

         FILE *fp1;
         char line1[130];
         fp1 = popen( "cat /sys/class/backlight/intel_backlight/max_brightness" , "r");
         fgets( line1, sizeof line1, fp1);
         line1[strcspn(line1, "\n")] = 0;
         pclose(fp1);
         int max_brightness = ToUInt(line1);

         float percents,p;

         percents = ToUInt(value);
         p = percents * (float)max_brightness/100;

         if (p>max_brightness)
            p = max_brightness;

         char spercents[10];
         sprintf(spercents, "%d", (int)p);

         FILE *fp;
         char line[130];
         fp = popen( concat(  concat("echo ", spercents)," | sudo tee /sys/class/backlight/intel_backlight/brightness") , "r");
         fgets( line, sizeof line, fp);
         line[strcspn(line, "\n")] = 0;

         int out;
         out = ToUInt(line);
         pclose(fp);
   return (int)((float)out/((float)max_brightness/100));

}


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); 
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}



unsigned long ToUInt(char* str)
{
    unsigned long mult = 1;
    unsigned long re = 0;
    int len = strlen(str);
    for(int i = len -1 ; i >= 0 ; i--)
    {
        re = re + ((int)str[i] -48)*mult;
        mult = mult*10;
    }
    return re;
}
