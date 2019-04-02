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
// Input module:
void addPeriod(char timetable[][14][30]);
char * addEvent(int id, char * event_name_arr[], Date_Ptr date_arr, int * time_arr, int * duration_arr, int arr_index, FILE * fp, char timetable[][14][30], char * input_status[]);
void addRequest (int * id, char * event_name_arr[][1000], Date date_arr[][1000], int time_arr[][1000], int duration_arr[][1000], FILE * fp, char * command, char * input_event[], char *
input_status[], char timetable[][14][30]);
// Scheduling module:
void FCFS(int id, char * event_name_arr[][1000], int duration_arr[][1000], char timetable[][14][30]); // First-come-first-serve algorithm

/* add period or time slots to the timetable for scheduling events */
void addPeriod(char timetable[][14][30]) {
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

/* add revision (arr_index = 0) / activity (arr_index = 1) / assignment (arr_index = 2) / project (arr_index = 3) */
char * addEvent(int id, char * event_name_arr[], Date_Ptr date_arr, int * time_arr, int * duration_arr, int arr_index, FILE * fp, char timetable[][14][30], char * input_status[]) {
    char event_name[50];
    Date date;
    int time_hour;
    int time_minute;
    int duration;
    char parameter [50] = "";
    char event[50] = "";
    char * return_event;
    int valid = 1; /* 1: valid 0: invalid */

    /* initiliaze the string to be returned (a whole event) */
    if (arr_index == 0) {
        strcpy(event, "addRivision ");
    }
    else if (arr_index == 1){
        strcpy(event, "addActivity ");
    }
    else if (arr_index == 2){
        strcpy(event, "addAssignment ");
    }
    else {
        strcpy(event, "addProject ");
    }

    /* reading and checking (subject code with assignment or project number for addAssignment and addProject respectively) / (subject code or name of event for addRevision and addActivity
respectively) */
    fscanf(fp, "%s", event_name);
    strcat(event, event_name); /* concatenate the event name with the string to be returned */

    /* reading and checking (due date for addAssignment & addProject) / (date for addRevision & addActivity) */
    fscanf(fp, "%s", parameter);
    strcat(event, " ");
    strcat(event, parameter); /* concatenate the date with the string to be returned */
    date.year = atoi(strtok(parameter, "-"));
    /* print error message if the year is out of assumed range (2019) */
    if (date.year != 2019) {
        printf("ID: %d, Invalid year. (Valid year: 2019)\n", id);
        valid = 0;
    }
    date.month = atoi(strtok(NULL, "-"));
    /* print error message if the month is out of assumed range (4) */
    if (date.month != 4) {
        printf("ID: %d, Invalid month. (Valid month: 4)\n", id);
        valid = 0;
    }
    date.day = atoi(strtok(NULL, "-"));
    /* print error message if the day is out of assumed range (8 - 21) */
    if (date.day < 8 || date.day > 21) {
        printf("ID: %d, Invalid day. (Valid day: 8 - 21)\n", id);
        valid = 0;
    }

    /* reading and checking time for addRevision & addActivity */
    if (arr_index == 0 || arr_index == 1) {
        fscanf(fp, "%s", parameter);
        strcat(event, " ");
        strcat(event, parameter); /* concatenate the time with the string to be returned */
        time_hour = atoi(strtok(parameter, ":"));
        /* print error message if the hour is out of assumed range (19 - 22) */
        if (time_hour < 19 || time_hour > 22) {
            printf("ID: %d, Invalid hour (Valid hour: 19 - 22)\n", id);
            valid = 0;
        }
        time_minute = atoi(strtok(NULL, ":"));
        /* print error message if the minute is out of assumed range (0) */
        if (time_minute != 0) {
            printf("ID: %d, Invalid minute (Valid minute: 00)\n", id);
            valid = 0;
        }
        /* print error message if the timeslot for input date and time is not available */
        if (strcmp(timetable[time_hour - 19][date.day - 8], "N/A") != 0) {
            printf("ID: %d, The timeslot for input date and time is not available\n", id);
            valid = 0;
        }
    }

    /* reading and checking duration */
    fscanf(fp, "%s", parameter);
    strcat(event, " ");
    strcat(event, parameter); /* concatenate the duration with the string to be returned */
    duration = atoi(parameter);
    /* print error message if the duration is invalid */
    if (duration <= 0) {
        printf("ID: %d, Invalid duration. (Valid duration: > 0)\n", id);
        valid = 0;
    }

    /* save the parameters of the event if all parameters are valid */
    if (valid == 1) {
        event_name_arr[id] = malloc(strlen(event_name) * sizeof(char));
        strcpy(event_name_arr[id], event_name);
        date_arr[id] = date;
        if (arr_index == 0 || arr_index == 1) {
            time_arr[id] = time_hour;
        }
        duration_arr[id] = duration;
    }
    // else, save the status of the event as "Rejected" */
    else {
        input_status[id] = malloc(9 * sizeof(char));
        strcpy(input_status[id], "Rejected");
    }

    /* return the whole event as a string */
    return_event = malloc(strlen(event) * sizeof(char));
    strcpy(return_event, event);
    return return_event;
}

/* adding events by calling addEvent with different file pointer (stdin for direct input / fp for addBatch) */
void addRequest (int * id, char * event_name_arr[][1000], Date date_arr[][1000], int time_arr[][1000], int duration_arr[][1000], FILE * fp, char * command, char * input_event[], char *
input_status[], char timetable[][14][30]) {
    char * event;
    char * return_event = NULL;

    if (strcmp(command, "addRevision") == 0) {
        return_event = addEvent(*id, event_name_arr[0], date_arr[0], time_arr[0], duration_arr[0], 0, fp, timetable, input_status);
    }
    else if (strcmp(command, "addActivity") == 0) {
        return_event = addEvent(*id, event_name_arr[1], date_arr[1], time_arr[1], duration_arr[1], 1, fp, timetable, input_status);
    }
    else if (strcmp(command, "addAssignment") == 0) {
        return_event = addEvent(*id, event_name_arr[2], date_arr[2], NULL, duration_arr[2], 2, fp, timetable, input_status);
    }
    else if (strcmp(command, "addProject") == 0) {
        return_event = addEvent(*id, event_name_arr[3], date_arr[3], NULL, duration_arr[3], 3, fp, timetable, input_status);
    }

    /* logging the events */
    event = malloc(strlen(return_event) * sizeof(char));
    strcpy(event, return_event);
    free(return_event);
    input_event[*id] = malloc(strlen(event) * sizeof(char));
    strcpy(input_event[*id], event);
    free(event);
    (*id) ++; /* increment the id */
}

void FCFS(int id, char * event_name_arr[][1000], int duration_arr[][1000], char timetable[][14][30]){
    int count;
    for (int i=0;i<id;i++){
        count=0;
        for (int j=0;j<4;j++){
            if (event_name_arr[j][i] != NULL) {
                count=duration_arr[j][i];
                for (int x=0;x<14;x++){
                    for (int y=0;y<4;y++){
                        if (strcmp(timetable[y][x],"N/A")==0){
                            strcpy(timetable[y][x],event_name_arr[j][i]);
                            count--;
                            if (count<=0){
                                break;
                            }
                        }
                    }
                    if (count<=0){
                        break;
                    }
                }
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    int a, b;
    char command [20] = "";
    int id = 0;
    char file_name[50];

    char * input_event[1000] = {NULL}; /* For log file */
    char * input_status[1000] = {NULL}; /* For log file (Accepted / Rejected) */

    char * event_name_arr[4][1000] = {NULL}; /* For addRevision (array_index = 0) or addActivity (array_index = 1) or addAssignment (array index = 2) or addProject (array index = 3) */
    Date date_arr[4][1000]; /* For addRevision (array_index = 0) or addActivity (array_index = 1) or addAssignment (array index = 2) or addProject (array index = 3) */
    int time_arr[2][1000]; /* For addRevision (array_index = 0) or addActivity (array_index = 1) */
    int duration_arr[4][1000]; /* For addRevision (array_index = 0) or addActivity (array_index = 1) or addAssignment (array index = 2) or addProject (array index = 3) */

    char timetable[4][14][30] = {""}; /* a timetable for events in 19:00 to 23:00 from 2019-04-08 to 2019-04-21 ("" means that that period is not added) */

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
            addRequest(&id, event_name_arr, date_arr, time_arr, duration_arr, stdin, command, input_event, input_status, timetable);
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
                addRequest(&id, event_name_arr, date_arr, time_arr, duration_arr, fp, command, input_event, input_status, timetable); /* calling addRequest with the file pointer pointing to the file
opened */
            }
            fclose(fp); /* close the file */
        }
        else if (strcmp(command, "runS3") == 0){
            printf("runS3\n");
            /* TO BE WRITTEN... */
            // Scheduling Modules..
            FCFS(id,event_name_arr,duration_arr,timetable);
            /* CAN BE DELETED!!!!!!!!!! JUST FOR TESTING / DEMONSTRATION ============================== */
            for (a = 0; a < 14; a ++) {
                for (b = 0; b < 4; b ++) {
                    if (strcmp(timetable[b][a], "") != 0) {
                        printf("Day %d Time %d: %s\n", a + 8, b + 19,timetable[b][a]);
                    }
                }
            }
            for (a = 0; a < id; a ++) {
                if (input_event[a] != NULL) {
                    if (input_status[a] == NULL) {
                        printf("ID: %d Event: %s Status: NULL\n", a + 1, input_event[a]);
                    }
                    else {
                        printf("ID: %d Event: %s Status: %s\n", a + 1, input_event[a], input_status[a]);
                    }
                }
            }
            for (a = 0; a < id; a ++) {
                for (b = 0; b < 4; b ++) {
                    if (event_name_arr[b][a] != NULL) {
                        printf("ID: %d Event: %s ", a + 1, event_name_arr[b][a]);
                        printf("Date: %d-%d-%d ", date_arr[b][a].year, date_arr[b][a].month, date_arr[b][a].day);
                        if (b == 0 || b == 1) {
                            printf("Time: %d:00 ", time_arr[b][a]);
                        }
                        printf("Duration: %d\n", duration_arr[b][a]);
                    }
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
