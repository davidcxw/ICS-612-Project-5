// David Wang, Michael Omori
// Inspired by Michael Rodenhurst, 30/09/12

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <string.h>
#include <stdlib.h>

#define VERSION "1.0"

// We assume these start at GPIO 0
const int number_leds = 8;

void initialise();
void update();
int get_cpu_usage();

int main()
{

int count = 0;
while(1)
    {
        update();
        sleep(1);
        count += 1;
        if(count > 100) {
            break;
        }
    }

    return 0;
}

void update()
{
    float cpu_usage = get_cpu_usage();
    int leds_lit = 10 * cpu_usage/(100/(number_leds));
    FILE *fp;
    fp = fopen("/sys/devices/virtual/LedBlink/LedBlink/period", "w");
    fprintf(fp, "%d", leds_lit);
    fclose(fp);
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

    // Get the workload from that
    int tjif = user_jif + nice_jif + system_jif + idle_jif + iowait_jif;

    int delta_total = tjif - previous_tjif;
    
    int delta_idle = idle_jif - previous_ijif;
    
    int delta_usage = (1000*(delta_total-delta_idle)/(delta_total+5))/10;
    
    previous_tjif = tjif;
    previous_ijif = idle_jif;

    free(cpuline);
    free(buf);

    return delta_usage;
}
