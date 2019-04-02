#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* structure for a date (Year + Month + Day) */
struct date {
    int year;
    int month;
    int day;
};

typedef struct date Date;
typedef struct date * Date_Ptr;

/* function prototype */
void addPeiord(char timetable[][14][50]);
void addEvent(int * id, char * type, char * status_arr[], char * event_name_arr[], Date date_arr[], int time_arr[], int duration_arr[], char timetable[][14][50], FILE * fp);

/* add period or time slots to the timetable for scheduling events */
void addPeriod(char timetable[][14][50]) {
    Date start_date;
    Date end_date;
    int start_time_hour;
    int start_time_minute;
    int end_time_hour;
    int end_time_minute;
    char parameter [20] = "";
    int a, b;
    
    /* reading and checking start date */
    scanf("%s", parameter);
    start_date.year = atoi(strtok(parameter, "-"));
    /* return if the starting year of period to be added is out of assumed range (2019) */
    if (start_date.year != 2019) {
        printf("Invalid starting year added. (Valid year: 2019)\n");
        for (a = 0; a < 3; a++) scanf("%s", parameter); /* deal with remaining 3 inputs before return */
        return;
    }
    start_date.month = atoi(strtok(NULL, "-"));
    /* return if the starting month of period to be added is out of assumed range (4) */
    if (start_date.month != 4) {
        printf("Invalid starting month added (Valid month: 4)\n");
        for (a = 0; a < 3; a++) scanf("%s", parameter); /* deal with remaining 3 inputs before return */
        return;
    }
    start_date.day = atoi(strtok(NULL, "-"));
    /* return if the starting day of period to be added is out of assumed range (8 - 21) */
    if (start_date.day < 8 || start_date.day > 21) {
        printf("Invalid starting day added (Valid day: 8 - 21)\n");
        for (a = 0; a < 3; a++) scanf("%s", parameter); /* deal with remaining 3 inputs before return */
        return;
    }
    
    /* reading and checking end date */
    scanf("%s", parameter);
    end_date.year = atoi(strtok(parameter, "-"));
    /* return if the ending year of period to be added is out of assumed range (2019) */
    if (end_date.year != 2019) {
        printf("Invalid ending year added. (Valid year: 2019)\n");
        for (a = 0; a < 2; a++) scanf("%s", parameter); /* deal with remaining 2 inputs before return */
        return;
    }
    end_date.month = atoi(strtok(NULL, "-"));
    /* return if the ending month of period to be added is out of assumed range (4) */
    if (end_date.month != 4) {
        printf("Invalid ending month added (Valid month: 4)\n");
        for (a = 0; a < 2; a++) scanf("%s", parameter); /* deal with remaining 2 inputs before return */
        return;
    }
    end_date.day = atoi(strtok(NULL, "-"));
    /* return if the ending day of period to be added is out of assumed range (start_date.day - 21) */
    if (end_date.day < start_date.day || start_date.day > 21) {
        printf("Invalid ending day added (Valid day: %d - 21)\n", start_date.day);
        for (a = 0; a < 2; a++) scanf("%s", parameter); /* deal with remaining 2 inputs before return */
        return;
    }
    
    
    /* reading and checking start time */
    scanf("%s", parameter);
    start_time_hour = atoi(strtok(parameter, ":"));
    /* return if the starting hour of period to be added is out of assumed range (19 - 22) */
    if (start_time_hour < 19 || start_time_hour > 22) {
        printf("Invalid starting hour added (Valid hour: 19 - 22)\n");
        for (a = 0; a < 1; a++) scanf("%s", parameter); /* deal with remaining 1 inputs before return */
        return;
    }
    start_time_minute = atoi(strtok(NULL, ":"));
    /* return if the starting minute of period to be added is out of assumed range (0) */
    if (start_time_minute != 0) {
        printf("Invalid starting minute added (Valid minute: 00)\n");
        for (a = 0; a < 1; a++) scanf("%s", parameter); /* deal with remaining 1 inputs before return */
        return;
    }
    
    /* reading and checking end time */
    scanf("%s", parameter);
    end_time_hour = atoi(strtok(parameter, ":"));
    /* return if the endinging hour of period to be added is out of assumed range (start_time_hour - 23) */
    if (end_time_hour <= start_time_hour || end_time_hour > 23) {
        printf("Invalid ending hour added (Valid hour: %d - 23)\n", start_time_hour + 1);
        return;
    }
    end_time_minute = atoi(strtok(NULL, ":"));
    /* return if the ending minute of period to be added is out of assumed range (0) */
    if (end_time_minute != 0) {
        printf("Invalid ending minute added (Valid minute: 00)\n");
        return;
    }
    
    /* add the valid timeslots or period to the timetable if all parameters are valid */
    for (a = start_date.day - 8; a <= end_date.day - 8; a++) {
        for (b = start_time_hour - 19; b <= end_time_hour - 20; b++) {
            strcpy(timetable[b][a], "N/A"); /* N/A means that that period is added but with no assignment */
        }
    }
    
}

/* add revision / activity / assignment / project */
void addEvent(int * id, char * type, char * status_arr[], char * event_name_arr[], Date date_arr[], int time_arr[], int duration_arr[], char timetable[][14][50], FILE * fp) {
    char event_name[50];
    Date date;
    int time_hour;
    int time_minute;
    int duration;
    char parameter [50] = "";
    int valid = 1; /* 1: valid 0: invalid */
    
    /* reading (subject code with assignment or project number for addAssignment and addProject respectively) / (subject code or name of event for addRevision and addActivity
     respectively) */
    fscanf(fp, "%s", event_name);
    
    /* reading and checking (due date for addAssignment & addProject) / (date for addRevision & addActivity) */
    fscanf(fp, "%s", parameter);
    date.year = atoi(strtok(parameter, "-"));
    /* print error message if the year is out of assumed range (2019) */
    if (date.year != 2019) {
        printf("ID: %d, Invalid year. (Valid year: 2019)\n", *id);
        valid = 0;
    }
    date.month = atoi(strtok(NULL, "-"));
    /* print error message if the month is out of assumed range (4) */
    if (date.month != 4) {
        printf("ID: %d, Invalid month. (Valid month: 4)\n", *id);
        valid = 0;
    }
    date.day = atoi(strtok(NULL, "-"));
    /* print error message if the day is out of assumed range (8 - 21) */
    if (date.day < 8 || date.day > 21) {
        printf("ID: %d, Invalid day. (Valid day: 8 - 21)\n", *id);
        valid = 0;
    }
    
    /* reading and checking time for addRevision & addActivity */
    if (strcmp(type, "addRevision") == 0 || strcmp(type, "addActivity") == 0) {
        fscanf(fp, "%s", parameter);
        time_hour = atoi(strtok(parameter, ":"));
        /* print error message if the hour is out of assumed range (19 - 22) */
        if (time_hour < 19 || time_hour > 22) {
            printf("ID: %d, Invalid hour (Valid hour: 19 - 22)\n", *id);
            valid = 0;
        }
        time_minute = atoi(strtok(NULL, ":"));
        /* print error message if the minute is out of assumed range (0) */
        if (time_minute != 0) {
            printf("ID: %d, Invalid minute (Valid minute: 00)\n", *id);
            valid = 0;
        }
        /* print error message if the timeslot for input date and time is not available */
        if (strcmp(timetable[time_hour - 19][date.day - 8], "N/A") != 0) {
            printf("ID: %d, The timeslot for input date and time is not available\n", *id);
            valid = 0;
        }
    }
    
    /* reading and checking duration */
    fscanf(fp, "%s", parameter);
    duration = atoi(parameter);
    /* print error message if the duration is invalid */
    if (duration <= 0) {
        printf("ID: %d, Invalid duration. (Valid duration: > 0)\n", *id);
        valid = 0;
    }
    
    /* save the status of the event as "Rejected" if the input is invalid */
    if (valid == 0) {
        status_arr[*id] = malloc(strlen("Rejected") * sizeof(char));
        strcpy(status_arr[*id], "Rejected");
    }
    
    /* save the parameters of the event */
    event_name_arr[*id] = malloc(strlen(event_name) * sizeof(char));
    strcpy(event_name_arr[*id], event_name);
    date_arr[*id] = date;
    if (strcmp(type, "addRevision") == 0 || strcmp(type, "addActivity") == 0) {
        time_arr[*id] = time_hour;
    }
    duration_arr[*id] = duration;
    
    (*id) ++; /* increment the id */
}


int main(int argc, const char * argv[]) {
    int a, b;
    char command [20] = "";
    int id = 0;
    char file_name[50];
    
    char * type_arr [1000] = {NULL}; /* addRevision / addActivity / addAssignment / addProject */
    char * status_arr[1000] = {NULL}; /* Accepted / Rejected */
    char * event_name_arr[1000] = {NULL}; /* For addRevision (subject code) / addActivity (name of the event) / addAssignment (subject code with assignment number) / addProject (subject code with project number) */
    Date date_arr[1000]; /* For addRevision (date) / addActivity (date) / addAssignment (due date) / addProject (due date) */
    int time_arr[1000]; /* For addRevision (time) or addActivity (time) */
    for (a = 0; a < 1000; a ++) time_arr[a] = -1; /* -1 means not applicable */
    int duration_arr[1000]; /* For addRevision / addActivity / addAssignment / addProject */
    
    char timetable[4][14][50] = {""}; /* a timetable for events in 19:00 to 23:00 ([4]) from 2019-04-08 to 2019-04-21 ([14]) ("" means that that period is not added) */
    
    printf("   ~~WELCOME TO S3~~\n"); /* start the program properly */
    
    /* prompt until the user enter "exitS3" */
    while (strcmp(command, "exitS3") != 0) {
        printf("Please enter:\n");
        scanf("%s", command); /* scanning the first string in an input line */
        /* call different functions according to the first string in the input line */
        if (strcmp(command, "addPeriod") == 0) {
            addPeriod(timetable);
        }
        else if (strcmp(command, "addRevision") == 0 || strcmp(command, "addActivity") == 0 || strcmp(command, "addAssignment") == 0 || strcmp(command, "addProject") == 0) {
            type_arr[id] = malloc(strlen(command) * sizeof(char));
            strcpy(type_arr[id], command);
            addEvent(&id, type_arr[id], status_arr, event_name_arr, date_arr, time_arr, duration_arr, timetable, stdin);
        }
        else if (strcmp(command, "addBatch") == 0) {
            scanf("%s", file_name);
            FILE *fp;
            fp = fopen(file_name, "r"); /* open a file */
            
            /* error in opening file */
            if (fp == NULL)
            {
                printf("Error in opening input file\n");
                exit(1);
            }
            
            /* loop until there is no input in the file opened */
            while (fscanf(fp, "%s", command) == 1) {
                type_arr[id] = malloc(strlen(command) * sizeof(char));
                strcpy(type_arr[id], command);
                addEvent(&id, type_arr[id], status_arr, event_name_arr, date_arr, time_arr, duration_arr, timetable, fp);
            }
            fclose(fp); /* close the file */
        }
        else if (strcmp(command, "runS3") == 0){
            printf("runS3\n");
            /* TO BE WRITTEN... */
            /* CAN BE DELETED!!!!!!!!!! JUST FOR TESTING / DEMONSTRATION ============================== */
            for (a = 0; a < 14; a ++) {
                for (b = 0; b < 4; b ++) {
                    if (strcmp(timetable[b][a], "") != 0) {
                        printf("Day 2019-4-%d Time %d:00 : %s\n", a + 8, b + 19, timetable[b][a]);
                    }
                }
            }
            for (a = 0; a < id; a ++) {
                if (status_arr[a] == NULL) {
                    printf("ID: %d Type: %s Status: NULL Event_name: %s Date: %d-%d-%d Time: %d:00 Duration: %d\n", a + 1, type_arr[a], event_name_arr[a], date_arr[a].year, date_arr[a].month, date_arr[a].day, time_arr[a], duration_arr[a]);
                }
                else {
                    printf("ID: %d Type: %s Status: %s Event_name: %s Date: %d-%d-%d Time: %d:00 Duration: %d\n", a + 1, type_arr[a], status_arr[a], event_name_arr[a], date_arr[a].year, date_arr[a].month, date_arr[a].day, time_arr[a], duration_arr[a]);
                }
            }
            /* CAN BE DELETED!!!!!!!!!! JUST FOR TESTING / DEMONSTRATION ============================== */
        }
        else {
            continue;
        }
    }
    printf("Bye-bye!\n"); /* end the program properly */
}
