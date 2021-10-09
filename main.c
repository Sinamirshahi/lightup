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

int main(int argc, char *argv[] )  {  

   if(argc < 2){  

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
         printf("the brightnes is at %.0f\n", ((float)brightness / (float) max_brightness) * 100);

   }  

   else {  
      
      // system(concat(concat("sudo enlighten ", argv[1]),"%"));
         FILE *fp1;
        char line1[130];
         fp1 = popen( "cat /sys/class/backlight/intel_backlight/max_brightness" , "r");
         fgets( line1, sizeof line1, fp1);
         line1[strcspn(line1, "\n")] = 0;
         pclose(fp1);
         int max_brightness = ToUInt(line1);

         float percents,p;

         percents = ToUInt(argv[1]);
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
         printf("%0.f", (float)out/((float)max_brightness/100));
         pclose(fp);
   return 0;
}
}  

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
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
