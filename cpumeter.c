// Michael Rodenhurst, 30/09/12

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <string.h>
#include <stdlib.h>

#define VERSION "1.0"

// We assume these start at GPIO 0
const int number_leds = 6;

void initialise();
void update();
int get_cpu_usage();

int main()
{
    printf("CPU LED Meter, Version %s\n", VERSION);

    //initialise();

int count = 0;
while(1)
    {
        update();
        sleep(1);
        //delay(250);
        count += 1;
        if(count > 10) {
            break;
        }
    }

    return 0;
}
/*
void initialise()
{
    // Verify we are root
    if(geteuid() != 0)
    {
        printf("This program must run as root\n");
        exit(0);
    }

    if(wiringPiSetup() == -1)
    {
        printf("wiringPiSetup() failed!\n");
        exit(1);
    }

    printf("Initialise GPIO pins...\n");

    // Initalise all LED's and default them off
    for(int i=0; i < number_leds; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, 0);
    }

    printf("Done\n");
}
*/
void update()
{
    // Disable all LED's

    //for(int i=0; i < number_leds; i++)
    //    digitalWrite(i, 0);

    float cpu_usage = get_cpu_usage();
    printf("cpu_usage: %f\n", cpu_usage);
    //int leds_lit = cpu_usage/(100/(number_leds+1));

    //for(int i=0; i < leds_lit; i++)
    //    digitalWrite(i, 1);
}

int previous_tjif = 0;
int previous_ijif = 0;

// Return value 0-100
int get_cpu_usage()
{
    char *buf = malloc(48);

    // Read the cpu status from /proc/stat
    FILE *fp = fopen("/proc/stat", "r");
    fread(buf, sizeof(char), 48, fp);
    fclose(fp);

    // Strip the prefixed "cpu  "
    char *cpuline = malloc(48);
    for(int i=5; i<strlen(buf); i++)
        cpuline[i-5] = buf[i];

    cpuline[40] = '\0';

    //for(int i = 0; i < strlen(buf); i++) {
    //    printf("%s", buf[i]);
    //}
    printf("%s", cpuline);

    // Parse the proc/stat information into seperate jiffie containers
    int user_jif, nice_jif, system_jif, idle_jif;
    int iowait_jif;

    char *tokbuf = strtok(cpuline, " ");
    user_jif = atoi(tokbuf);
    tokbuf = strtok(NULL, " ");
    nice_jif = atoi(tokbuf);
    tokbuf = strtok(NULL, " ");
    system_jif = atoi(tokbuf);
    tokbuf = strtok(NULL, " ");
    idle_jif = atoi(tokbuf);
    tokbuf = strtok(NULL, " ");
    iowait_jif = atoi(tokbuf);
    printf("\n%d", user_jif);
    printf("\n%d", nice_jif);
    printf("\n%d", system_jif);
    printf("\n%d", idle_jif);
    printf("\n%d", iowait_jif);

    // Get the workload from that
    int tjif = user_jif + nice_jif + system_jif + idle_jif + iowait_jif;
    printf("\n%d", tjif);

    int delta_total = tjif - previous_tjif;
    printf("\n%d", delta_total);
    
    int delta_idle = idle_jif - previous_ijif;
    printf("\n%d", delta_idle);
    
    int delta_usage = (1000*(delta_total-delta_idle)/(delta_total+5))/10;
    printf("\n%d\n", delta_usage);
    
    previous_tjif = tjif;
    previous_ijif = idle_jif;

    free(cpuline);
    free(buf);

    return delta_usage;
}
