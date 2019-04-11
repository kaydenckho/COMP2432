#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

/* structure for a date (Year + Month + Day) */
typedef struct date {
    int year;
    int month;
    int day;
}date, Date;

typedef struct date * Date_Ptr;

/* function prototype */
char * validatePeriod(void);
char * validateEvent(int * id, char * type, FILE * fp);
/* First-come-first-serve algorithm */
void FCFS(int id, char * type_arr[1000], Date date_arr[1000], int time_arr[1000], char * event_name_arr[1000], int duration_arr[1000], char timetable[][14][50], int progress_arr[1000], char status_arr[][10]);

/* input validation for addPeriod */
char * validatePeriod(void) {
    char valid[2] = "1"; /* a character to indicate whether the input is valid or not (0: invalid, 1: valid) */
    Date start_date;
    Date end_date;
    int start_time_hour;
    int start_time_minute;
    int end_time_hour;
    int end_time_minute;
    char parameter [20] = "";
    char string[50] = "addPeriod"; /* initialize the string to be copied to return_string */
    char * return_string; /* string to be returned */
    
    /* reading and checking start date */
    scanf("%s", parameter);
    strcat(string, " ");
    strcat(string, parameter); /* concatenate the start date to the string */
    start_date.year = atoi(strtok(parameter, "-"));
    /* invalid if the starting year of period to be added is out of assumed range (2019) */
    if (strcmp(valid, "1") == 0 && start_date.year != 2019) {
        printf("Invalid starting year added. (Valid year: 2019)\n");
        strcpy(valid, "0");
    }
    start_date.month = atoi(strtok(NULL, "-"));
    /* invalid if the starting month of period to be added is out of assumed range (4) */
    if (strcmp(valid, "1") == 0 && start_date.month != 4) {
        printf("Invalid starting month added (Valid month: 4)\n");
        strcpy(valid, "0");
    }
    start_date.day = atoi(strtok(NULL, "-"));
    /* invalid if the starting day of period to be added is out of assumed range (8 - 21) */
    if (strcmp(valid, "1") == 0 && (start_date.day < 8 || start_date.day > 21)) {
        printf("Invalid starting day added (Valid day: 8 - 21)\n");
        strcpy(valid, "0");
    }
    
    /* reading and checking end date */
    scanf("%s", parameter);
    strcat(string, " ");
    strcat(string, parameter); /* concatenate the end date to the string */
    end_date.year = atoi(strtok(parameter, "-"));
    /* invalid if the ending year of period to be added is out of assumed range (2019) */
    if (strcmp(valid, "1") == 0 && end_date.year != 2019) {
        printf("Invalid ending year added. (Valid year: 2019)\n");
        strcpy(valid, "0");
    }
    end_date.month = atoi(strtok(NULL, "-"));
    /* invalid if the ending month of period to be added is out of assumed range (4) */
    if (strcmp(valid, "1") == 0 && end_date.month != 4) {
        printf("Invalid ending month added (Valid month: 4)\n");
        strcpy(valid, "0");
    }
    end_date.day = atoi(strtok(NULL, "-"));
    /* invalid if the ending day of period to be added is out of assumed range (start_date.day - 21) */
    if (strcmp(valid, "1") == 0 && (end_date.day < start_date.day || start_date.day > 21)) {
        printf("Invalid ending day added (Valid day: %d - 21)\n", start_date.day);
        strcpy(valid, "0");
    }
    
    /* reading and checking start time */
    scanf("%s", parameter);
    strcat(string, " ");
    strcat(string, parameter); /* concatenate the start time to the string */
    start_time_hour = atoi(strtok(parameter, ":"));
    /* invalid if the starting hour of period to be added is out of assumed range (19 - 22) */
    if (strcmp(valid, "1") == 0 && (start_time_hour < 19 || start_time_hour > 22)) {
        printf("Invalid starting hour added (Valid hour: 19 - 22)\n");
        strcpy(valid, "0");
    }
    start_time_minute = atoi(strtok(NULL, ":"));
    /* invalid if the starting minute of period to be added is out of assumed range (0) */
    if (strcmp(valid, "1") == 0 && start_time_minute != 0) {
        printf("Invalid starting minute added (Valid minute: 00)\n");
        strcpy(valid, "0");
    }
    
    /* reading and checking end time */
    scanf("%s", parameter);
    strcat(string, " ");
    strcat(string, parameter); /* concatenate the end time to the string */
    end_time_hour = atoi(strtok(parameter, ":"));
    /* invalid if the endinging hour of period to be added is out of assumed range (start_time_hour - 23) */
    if (strcmp(valid, "1") == 0 && (end_time_hour <= start_time_hour || end_time_hour > 23)) {
        printf("Invalid ending hour added (Valid hour: %d - 23)\n", start_time_hour + 1);
        strcpy(valid, "0");
    }
    end_time_minute = atoi(strtok(NULL, ":"));
    /* invalid if the ending minute of period to be added is out of assumed range (0) */
    if (strcmp(valid, "1") == 0 && end_time_minute != 0) {
        printf("Invalid ending minute added (Valid minute: 00)\n");
        strcpy(valid, "0");
    }
    
    strcat(string, " ");
    strcat(string, valid); /* concatenate the indicator (0 / 1) to the string to indicate whether the input is valid or not */
    
    /* return the string as an input to scheduler */
    /* format: [use input] [indicator] */
    return_string = malloc((strlen(string) + 1) * sizeof(char));
    strcpy(return_string, string);
    return return_string;
}

/* input validation for addRevision & addActivity & addAssignment & addProject */
char * validateEvent(int * id, char * type, FILE * fp) {
    char valid[2] = "1"; /* a character to indicate whether the input is valid or not (0: invalid, 1: valid) */
    Date date;
    int time_hour;
    int time_minute;
    int duration;
    char parameter [50] = "";
    char string[50] = "";
    strcpy(string, type); /* initialize the string to be copied to return_string */
    char * return_string; /* string to be returned */
    
    /* reading (subject code with assignment or project number for addAssignment and addProject respectively) / (subject code or name of event for addRevision and addActivity
     respectively) */
    fscanf(fp, "%s", parameter);
    strcat(string, " ");
    strcat(string, parameter); /* concatenate the event name to the string */
    
    /* reading and checking (due date for addAssignment & addProject) / (date for addRevision & addActivity) */
    fscanf(fp, "%s", parameter);
    strcat(string, " ");
    strcat(string, parameter); /* concatenate the date to the string */
    date.year = atoi(strtok(parameter, "-"));
    /* invalid if the year is out of assumed range (2019) */
    if (date.year != 2019) {
        printf("ID: %d, Invalid year. (Valid year: 2019)\n", *id + 1);
        strcpy(valid, "0");
    }
    date.month = atoi(strtok(NULL, "-"));
    /* invalid if the month is out of assumed range (4) */
    if (strcmp(valid, "1") == 0 && date.month != 4) {
        printf("ID: %d, Invalid month. (Valid month: 4)\n", *id + 1);
        strcpy(valid, "0");
    }
    date.day = atoi(strtok(NULL, "-"));
    /* invalid if the day is out of assumed range (8 - 21) */
    if (strcmp(valid, "1") == 0 && (date.day < 8 || date.day > 21)) {
        printf("ID: %d, Invalid day. (Valid day: 8 - 21)\n", *id + 1);
        strcpy(valid, "0");
    }
    
    /* reading and checking time for addRevision & addActivity */
    if (strcmp(type, "addRevision") == 0 || strcmp(type, "addActivity") == 0) {
        fscanf(fp, "%s", parameter);
        strcat(string, " ");
        strcat(string, parameter); /* concatenate the time to the string */
        time_hour = atoi(strtok(parameter, ":"));
        /* invalid if the hour is out of assumed range (19 - 22) */
        if (strcmp(valid, "1") == 0 && (time_hour < 19 || time_hour > 22)) {
            printf("ID: %d, Invalid hour (Valid hour: 19 - 22)\n", *id + 1);
            strcpy(valid, "0");
        }
        time_minute = atoi(strtok(NULL, ":"));
        /* invalid if the minute is out of assumed range (0) */
        if (strcmp(valid, "1") == 0 && time_minute != 0) {
            printf("ID: %d, Invalid minute (Valid minute: 00)\n", *id + 1);
            strcpy(valid, "0");
        }
        
    }
    
    /* reading and checking duration */
    fscanf(fp, "%s", parameter);
    strcat(string, " ");
    strcat(string, parameter); /* concatenate the duration to the string */
    duration = atoi(parameter);
    /* print error message if the duration is invalid */
    if (strcmp(valid, "1") == 0 && duration <= 0) {
        printf("ID: %d, Invalid duration. (Valid duration: > 0)\n", *id + 1);
        strcpy(valid, "0");
    }
    
    (*id) ++; /* increment the id */
    
    strcat(string, " ");
    strcat(string, valid); /* concatenate the indicator (0 / 1) to the string to indicate whether the input is valid or not */
    
    /* return the string as an input to scheduler */
    /* format: [use input] [indicator] */
    return_string = malloc((strlen(string) + 1) * sizeof(char));
    strcpy(return_string, string);
    return return_string;
}

/* First-come-first-serve algorithm */
void FCFS(int id, char * type_arr[1000] ,Date date_arr[1000], int time_arr[1000], char * event_name_arr[1000], int duration_arr[1000], char timetable[][14][50], int progress_arr[1000], char status_arr[][10]){
    int count=0;  /* duration of each task */
    int i,x,y;  /* for loop counters */
    for (i=0;i<id;i++){
        if (strcmp(status_arr[i], "") == 0) {
            /* Reject the request if the date and time is out of the period range */
            if (strcmp(type_arr[i],"addRevision")==0 || strcmp(type_arr[i],"addActivity")==0){
                if (strcmp(timetable[time_arr[i] - 19][date_arr[i].day - 8], "N/A") !=0) {
                    strcpy(status_arr[i],"Rejected");
                    progress_arr[i] = 0;
                }
            }
            if (status_arr[i] == NULL || strcmp(status_arr[i],"Rejected") != 0) {
                count=duration_arr[i];
                for (x=0;x<14;x++){
                    for (y=0;y<4;y++){
                        if (strcmp(timetable[y][x],"N/A")==0 && count>0){    /* if the timeslot is available */
                            if(strcmp(type_arr[i],"addRevision")!=0 && strcmp(type_arr[i],"addActivity")!=0){
                                strcpy(timetable[y][x],event_name_arr[i]);   /* assign task for the timeslot */
                                count--;   /*  duration of task by reduced by 1 after scheduled for a timeslot */
                                if (count==0){
                                    progress_arr[i]=100;             /* update progress record in main scheduler */
                                    strcpy(status_arr[i],"Accepted");          /* update status record in main scheduler */
                                }
                            }
                            else if(x==(date_arr[i].day-8) && y>=(time_arr[i]-19) && y<=(time_arr[i]-19+duration_arr[i])){
                                strcpy(timetable[y][x],event_name_arr[i]);   /* assign task for the timeslot */
                                count--;   /*  duration of task by reduced by 1 after scheduled for a timeslot */
                                if (count==0){
                                    progress_arr[i]=100;             /* update progress record in main scheduler */
                                    strcpy(status_arr[i],"Accepted");          /* update status record in main scheduler */
                                }
                            }
                        }
                    }
                }
                if (count>0){    /* if the task is not fully scheduled*/
                    if (count<duration_arr[i]) {
                        if (strcmp(type_arr[i],"addRevision")!=0 && strcmp(type_arr[i],"addActivity")!=0){
                            float prog = (float)(duration_arr[i]-count) / (float)duration_arr[i] *100; /*calculate percentage of completion */
                            progress_arr[i]=(int)prog;     /* update progress record in main scheduler */
                            strcpy(status_arr[i],"Accepted");       /* update status record in main scheduler */
                        }
                        else {
                            progress_arr[i]=0;  /* Progress = 0 if task cannot be scheduled */
                            strcpy(status_arr[i],"Rejected"); /* update status to "Rejected" in main scheduler if the task cannot be scheduled */
                        }
                    }
                    else {
                        progress_arr[i]=0;  /* Progress = 0 if task cannot be scheduled */
                        strcpy(status_arr[i],"Rejected"); /* update status to "Rejected" in main scheduler if the task cannot be scheduled */
                    }
                }
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    int a, b, temp;
    int id = 0;
    int parent_id = getpid();
    int pid;
    int fd[4][2]; /* (2 * 2) pipes for communication between 2 pairs of modules (1.input module & scheduling module 2. scheduling module & output&analyzer module) */
    char command [20] = "";
    char buffer[50] = "";
    char syn[10] = ""; /* only used for synchronization */
    /* create pipes */
    for (a = 0; a < 4; a ++) {
        if (pipe(fd[a]) < 0) {
            printf("Pipe creation error\n");
            exit(1);
        }
    }
    
    /* create 2 children (Parent: input module, 1st child: scheduling module, 2st child: output&analyzer module) */
    for(a = 0; a < 2; a ++){
        if (getpid() == parent_id) pid = fork();
    }
    if (pid < 0) {
        printf("Fork Failed\n");
        exit(1);
    }
    /* child process */
    else if (pid == 0) {
        pid = getpid() - parent_id;
        
        /* 1st child: schduling module */
        if (pid == 1) {
            int i_read_pipe = 1; /* fd[1] is the pipe for scheduling module to read from input module */
            int i_write_pipe = 0; /* fd[0] is the pipe for scheduling module to write to input module */
            
            int oa_read_pipe = 2; /* fd[2] is the pipe for scheduling module to read from output&analyzer module */
            int oa_write_pipe = 3; /* fd[3] is the pipe for scheduling module to write to output&analyzer module */
            
            /* close unused pipe ends */
            for (a = 0; a < 4; a ++) {
                if (a != i_read_pipe && a != i_write_pipe && a != oa_read_pipe && a != oa_write_pipe) {
                    close(fd[a][0]);
                    close(fd[a][1]);
                }
                else if (a == i_read_pipe || a == oa_read_pipe) close(fd[a][1]);
                else close(fd[a][0]);
            }
            
            int start_day, end_day, start_hour, end_hour;
            char * type_arr [1000] = {NULL}; /* addRevision / addActivity / addAssignment / addProject */
            char status_arr[1000][10] = {""}; /* Accepted / Rejected */
            char * event_name_arr[1000] = {NULL}; /* For addRevision (subject code) / addActivity (name of the event) / addAssignment (subject code with assignment number) / addProject (subject code with project number) */
            Date date_arr[1000]; /* For addRevision (date) / addActivity (date) / addAssignment (due date) / addProject (due date) */
            int time_arr[1000]; /* For addRevision (time) or addActivity (time) */
            for (a = 0; a < 1000; a ++) time_arr[a] = -1; /* -1 means not applicable e.g. addAssignment & addProject */
            int duration_arr[1000]; /* For addRevision / addActivity / addAssignment / addProject */
            int progress_arr[1000]; /* For addAssignment / addProject (% of completion) */
            
            char timetable[4][14][50] = {""}; /* a timetable for events in 19:00 to 23:00 ([4]) from 2019-04-08 to 2019-04-21 ([14]) ("" means that that period is not added) */
            
            char string[50] = "";
            
            char request[1000][50] = {""}; /* request list to be sent to output & analyzer module when "runS3" */
            
						/////EDF
			date edf_start_date;
			date edf_end_date;
			int edf_period_days;
			int edf_period_day_hrs;
			int edf_start_hr;
			int edf_end_hr;
			
			
			
			//EDF functions
			
			Date str2date(char *date1) {
				char *token;
				char *saveptr;
				Date temp;

				temp.year = atoi(strtok_r(date1, "-", &saveptr));
				temp.month = atoi(strtok_r(NULL, "-", &saveptr));
				temp.day = atoi(strtok_r(NULL, "-", &saveptr));
				return temp;
			}

			void date2str(Date date1,char *str) {
				char month[3];
				char day[3];
				if(date1.month<10){
					sprintf(month, "0%d", date1.month);
				}
				else{
					sprintf(month, "%d", date1.month);
				}
				if(date1.day<10){
					sprintf(day, "0%d", date1.day);
				}
				else{
					sprintf(day, "%d", date1.day);
				}
				sprintf(str, "%d-%s-%s", date1.year, month, day);
			}
	
			int str2hr(char* time1){
				char *token;
				char *saveptr;
				int temp = -1;
				temp = atoi(strtok_r(time1, ":", &saveptr));
				return temp;
			}

			Date date_tmr(Date date1){
				int max_days_arr[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
				Date temp = date1;
				if(date1.day != max_days_arr[date1.month-1]){
					temp.day +=1;
					return temp;
				}
				else{
					if(date1.month != 12){
						temp.month+=1;
						temp.day=1;
						return temp;
					}
					else{
						temp.year+=1;
						temp.month=1;
						temp.day=1;
						return temp;
					}
				}
			}

			int date_cmp(Date date1, Date date2){
				if(date1.year > date2.year){
					return 1;
				}
				else if(date1.year < date2.year){
					return -1;
				}
				else{
					if(date1.month > date2.month){
						return 1;
					}
					else if(date1.month < date2.month){
						return -1;
					}
					else{
						if(date1.day > date2.day){
							return 1;
						}
						else if(date1.day < date2.day){
							return -1;
						}
						else{
							return 0;
						}
					}
					
				}
			}
			
			bool date_inrange(Date date1, Date date2, Date date3){
				if(date_cmp(date2, date1)>=0 && date_cmp(date2, date3)<=0){
					return true;
				}
				else{
					return false;
				}
			}
			
			int date_days(Date date1, Date date2){
				int i = 0;
				int j =0;
				int days = 0;
				Date temp1;
				Date temp2;
				if(date_cmp(date1, date2)==1){
					temp2 = date1;
					temp1 = date2;
				}
				else if(date_cmp(date1, date2)==-1){
					temp1 = date1;
					temp2 = date2;
				}
				else{
					return 1;
				}
				
				if(temp1.year==2019 || temp2.year==2019 ){
					if(temp1.month!=temp2.month){
						for(i=temp1.month+1;i<temp2.month;i++){
							if(i<=7){
								if(i!=2){
									if(i%2==1){
										days+=31;
									}
									else{
										days+=30;
									}
								}
								else{
									days+=28;
								}
							}
							else{
								if(i%2==1){
									days+=30;
								}
								else{
									days+=31;
								}
							}
						}
						i = temp1.month;
						
						if(i<=7){
							if(i!=2){
								if(i%2==1){
									days+=31-temp1.day+1;
								}
								else{
									days+=30-temp1.day+1;
								}
							}
							else{
								days+=28-temp1.day+1;
							}
						}
						else{
							if(i%2==1){
								days+=30-temp1.day+1;
							}
							else{
								days+=31-temp1.day+1;
							}
						}
						days += temp2.day;
					}
					else{
						days += temp2.day-temp1.day+1;
					}
	
				}
				else{
					return -1;
				}
				
				return days;
			}
			
			
			
			/* Event entry to be scheduled */
			typedef struct edf_entry {
				
				int id;				/* ID of the event name */
				char *name;			/* Name of the activity */
				Date edf_date;		/* Date of deadline or activity */
				int edf_time;		/* Time start hr:0-24 */
				int edf_cost;		/* Time cost for the activity in hrs */
				int type_piority;	/* Indicate the type of activity from 1 to 4, e.g. Project = 1 */
				
			} edf_entry;
			
			
			/* List of event entries */
			typedef struct edf_entry_list {
				
				edf_entry entry;
				struct edf_entry_list * next_entry;
				
			} edf_entry_list;
			
			edf_entry_list* entry_head = NULL;
			edf_entry_list* entry_tail = NULL;
			
			
			/* Time slot element for timetable list */
			typedef struct edf_timeslot {
				
				char *name;
				Date edf_date;
				int edf_time;
				
			} edf_timeslot;
			
			
			/* A list of time slots to represent the timetable (slots by slots) */
			typedef struct edf_timetable_list{
				
				edf_timeslot timeslot;
				struct edf_timetable_list * next_slot;
				
			} edf_timetable_list;
			
			edf_timetable_list* timetable_head = NULL;
			edf_timetable_list* timetable_tail = NULL;
			
			int insert_entry(int id, char* insrt_name, Date insrt_date, int insrt_time, int insrt_cost, int insrt_type){
				edf_entry_list* temp;
				temp = malloc(sizeof(edf_entry_list));
				temp->entry.id = id;
				temp->entry.name = insrt_name;
				temp->entry.edf_date = insrt_date;
				temp->entry.edf_time = insrt_time;
				temp->entry.edf_cost = insrt_cost;
				temp->entry.type_piority = insrt_type;
				temp->next_entry = NULL;
				
				
				edf_entry_list* entry_list_ptr;
				
				if(entry_head==NULL){
					entry_head = temp;
					entry_tail = temp;
				}
				else{
					edf_entry_list* entry_list_prev = NULL;
					entry_list_ptr = entry_head;
					while(true){
						/*Date of new entry is later than current*/ //checked
						if(date_cmp(entry_list_ptr->entry.edf_date, temp->entry.edf_date) == -1){
							if(entry_list_ptr->next_entry!=NULL){
								entry_list_prev = entry_list_ptr;
								entry_list_ptr = entry_list_ptr->next_entry;
							}
							else{
								entry_list_ptr->next_entry = temp;
								entry_tail = temp;
								return 1;
							}
						}
						/*Date of new entry is eariler than current*/  //checked
						else if(date_cmp(entry_list_ptr->entry.edf_date, temp->entry.edf_date) == 1){
							if(entry_list_prev != NULL){
								entry_list_prev->next_entry = temp;
								temp->next_entry = entry_list_ptr;
							}
							else{
								temp->next_entry = entry_head;
								entry_head = temp;							
							}
							return 1;
							
						}
						/*Date of new entry is identical*/
						else{
							/*Date of new entry is later than current time*/ //checked
							if(temp->entry.edf_time > entry_list_ptr->entry.edf_time){
								if(entry_list_ptr->next_entry!=NULL){
									entry_list_prev = entry_list_ptr;
									entry_list_ptr = entry_list_ptr->next_entry;
								}
								else{
									entry_list_ptr->next_entry = temp;
									entry_tail = temp;
									return 1;
								}
							}
							/*Date of new entry is earlier than current time*/
							else if(temp->entry.edf_time < entry_list_ptr->entry.edf_time){
								if(entry_list_prev != NULL){
									entry_list_prev->next_entry = temp;
									temp->next_entry = entry_list_ptr;
								}
								else{
									temp->next_entry = entry_head;
									entry_head = temp;
								}
								return 1;
							}
							/*Date of new entry is same to current time*/
							else{
								/*New entry has higher priority*/ //checked
								if(entry_list_ptr->entry.type_piority < temp->entry.type_piority){
									if(entry_list_prev != NULL){
										entry_list_prev->next_entry = temp;
										temp->next_entry = entry_list_ptr;
									}
									else{
										temp->next_entry = entry_head;
										entry_head = temp;
									}
									return 1;
								}
								/*New entry has lower priority*/  //checked
								else if(entry_list_ptr->entry.type_piority > temp->entry.type_piority){
									if(entry_list_ptr->next_entry!=NULL){
										entry_list_prev = entry_list_ptr;
										entry_list_ptr = entry_list_ptr->next_entry;
									}
									else{
										entry_list_ptr->next_entry = temp;
										entry_tail = temp;
										return 1;
									}
								}
								/*New entry has same priority*/
								else{
									if(entry_list_ptr->next_entry!=NULL){
										entry_list_prev = entry_list_ptr;
										entry_list_ptr = entry_list_ptr->next_entry;
									}
									else{
										entry_list_ptr->next_entry = temp;
										entry_tail = temp;
										return 1;
									}
								}
							}
						}
					}
				}
			}
			
			void print_entries(){
			char temp_date_str[20];
			edf_entry_list* temp_ptr = NULL;
			if(entry_head != NULL){
				temp_ptr = entry_head;
				date2str(temp_ptr->entry.edf_date, temp_date_str);
				printf("ID  Entry name   Deadline/Date   Time   Cost   Priority\n");
				printf("==================================================\n");
				printf("%3d %-13s%-16s%-7d%-7d%-8d\n", temp_ptr->entry.id, temp_ptr->entry.name, temp_date_str, temp_ptr->entry.edf_time,temp_ptr->entry.edf_cost,temp_ptr->entry.type_piority);
			}
			else{return;}
			
			
			while(temp_ptr->next_entry!=NULL){
				temp_ptr = temp_ptr->next_entry;
				date2str(temp_ptr->entry.edf_date, temp_date_str);
				printf("%3d %-13s%-16s%-7d%-7d%-8d\n", temp_ptr->entry.id, temp_ptr->entry.name, temp_date_str, temp_ptr->entry.edf_time,temp_ptr->entry.edf_cost,temp_ptr->entry.type_piority);
			}		
			printf("\n");
		}
		
		void print_slots(){
			char temp_date_str[20];
			edf_timetable_list* temp_ptr = NULL;
			if(timetable_head != NULL){
				temp_ptr = timetable_head;
				date2str(temp_ptr->timeslot.edf_date, temp_date_str);
				printf("EDF Schedule\n");
				printf("==================================================\n");
				printf("%-16s%-2d:00    %-17s\n", temp_date_str, temp_ptr->timeslot.edf_time,temp_ptr->timeslot.name);
			}
			else{return;}
			
			
			while(temp_ptr->next_slot!=NULL){
				temp_ptr = temp_ptr->next_slot;
				date2str(temp_ptr->timeslot.edf_date, temp_date_str);
				printf("%-16s%-2d:00    %-17s\n", temp_date_str, temp_ptr->timeslot.edf_time,temp_ptr->timeslot.name);
			}
						
			printf("\n");
		}
			
			int add_time_slot(char* insrt_name, Date insrt_date, int insrt_time){
				edf_timetable_list* temp;
				temp = malloc(sizeof(edf_timetable_list));
				temp->timeslot.name = insrt_name;
				temp->timeslot.edf_date = insrt_date;
				temp->timeslot.edf_time = insrt_time;
				temp->next_slot = NULL;
				
				if(timetable_head==NULL){
					timetable_head = temp;
					timetable_tail = temp;
					return 1;
				}
				else{
					timetable_tail->next_slot = temp;
					timetable_tail = temp;
					return 1;
				}
			}
			
			void addPeriod(char* parameters[]){
				int i = 0;
				int j = 0;
				edf_start_date = str2date(parameters[0]);
				edf_end_date = str2date(parameters[1]);
				edf_start_hr = str2hr(parameters[2]);
				edf_end_hr = str2hr(parameters[3]);		
				edf_period_days = date_days(edf_start_date, edf_end_date);		
				edf_period_day_hrs = edf_end_hr - edf_start_hr;
				Date temp_date = edf_start_date;
				
				int temp_time = edf_start_hr;
				for(i=1; i<= edf_period_days; i++){
					for(j=1; j<=edf_period_day_hrs; j++){
						add_time_slot("N/A", temp_date, temp_time);
						//char temp_str3[20];
						//date2str((*timetable_tail).timeslot.edf_date, temp_str3);
						//printf("%s %s %d\n", (*timetable_tail).timeslot.name, temp_str, (*timetable_tail).timeslot.edf_time);
						temp_time++;
					}
					temp_time = edf_start_hr;
					temp_date = date_tmr(temp_date);
				}
				return;
			}
			
			void addProject(int id, char* parameters[]){
				char *name; 
				name = malloc(sizeof(char)*20);
				strcpy(name,parameters[0]);
				Date temp_date = str2date(parameters[1]);
				int temp_time = 24;
				int temp_cost = atoi(parameters[2]);
				int type = 1;
				insert_entry(id, name, temp_date, temp_time, temp_cost, type);
			}
			void addAssignment(int id, char* parameters[]){
				char *name; 
				name = malloc(sizeof(char)*20);
				strcpy(name,parameters[0]);
				Date temp_date = str2date(parameters[1]);
				int temp_time = 24;
				int temp_cost = atoi(parameters[2]);
				int type = 2;
				insert_entry(id, name, temp_date, temp_time, temp_cost, type);
			}
			void addRevision(int id, char* parameters[]){
				char *name; 
				name = malloc(sizeof(char)*20);
				strcpy(name,parameters[0]);
				Date temp_date = str2date(parameters[1]);
				int temp_time = str2hr(parameters[2]);
				int temp_cost = atoi(parameters[3]);
				int type = 3;
				insert_entry(id, name, temp_date, temp_time, temp_cost, type);
			}
			void addActivity(int id, char* parameters[]){
				char *name; 
				name = malloc(sizeof(char)*20);
				strcpy(name,parameters[0]);
				Date temp_date = str2date(parameters[1]);
				int temp_time = str2hr(parameters[2]);
				int temp_cost = atoi(parameters[3]);
				int type = 4;
				insert_entry(id, name, temp_date, temp_time, temp_cost, type);
			}
			
			void EDF_RunS3(){
				edf_entry_list* current_PA = NULL;
				int PA_r_cost = 0;
				edf_entry_list* current_RA = NULL;
				int RA_r_cost = 0;
				edf_timetable_list* current_slot = NULL;
				
				int i = 1;
				int total_slots = edf_period_days*edf_period_day_hrs;
				
				void getNextPA(edf_entry_list** current){
					edf_entry_list* temp = *current;
					int found = 0;
					while(found==0&&temp->next_entry!=NULL){
						temp = temp->next_entry;
						if(temp->entry.type_piority<3){
							found=1;
						}
					}
					if(found==0){
						*current = NULL;
					}
					else{
						*current = temp;	
					}
				}
				
				void getNextRA(edf_entry_list** current){
					edf_entry_list* temp = *current;
					int found = 0;
					while(found==0&&temp->next_entry!=NULL){
						temp = temp->next_entry;
						if(temp!=NULL&&temp->entry.type_piority>2){
							found=1;
						}
					}
					if(found==0){
						*current = NULL;
					}
					else{
						*current = temp;
					}
					return;
				}
				
				void assign_slot(edf_timetable_list* location,char *name){
					location->timeslot.name = name;
				}
				
				
				
				bool validate_RA(){
					
					if((date_cmp(current_slot->timeslot.edf_date,current_RA->entry.edf_date)==0 && current_RA->entry.edf_time>current_slot->timeslot.edf_time) 
					|| date_cmp(current_RA->entry.edf_date,current_slot->timeslot.edf_date)==1){
						
						if(date_inrange(edf_start_date, current_RA->entry.edf_date, edf_end_date)){
							
							if(current_RA->entry.edf_time>=edf_start_hr && current_RA->entry.edf_time+current_RA->entry.edf_cost<=edf_end_hr){
								return true;
							}						
						}
					}
					return false;
				}
				
				bool validate_PA(){
					//PA on same day but not last slot OR Later than today   
					if(  ((date_cmp(current_PA->entry.edf_date, current_slot->timeslot.edf_date)==0 && current_slot->timeslot.edf_time!=edf_end_hr-1) ||	
							date_cmp(current_PA->entry.edf_date, current_slot->timeslot.edf_date)==1)
						//AND NOT out of period range
						&& date_inrange(edf_start_date, current_PA->entry.edf_date, edf_end_date)
					){
						return true;
					}
					else{
						return false;
					}
				}
				
				void time_check(){
					//printf("Activity starts now.\n");
					int days_til_dead = (date_days(current_PA->entry.edf_date, current_slot->timeslot.edf_date)-1);
					//printf("Activity encountered: days til dead is: %d\n",days_til_dead);
					int cost_left = (days_til_dead*edf_period_day_hrs) + (edf_end_hr -(current_slot->timeslot.edf_time));
					//printf("Activity encountered: cost left is: %d\n",cost_left);
					//Still have time to finish P/A
					if((cost_left-(current_RA->entry.edf_cost)) >= PA_r_cost){
						//printf("u can join current activity\n");
						assign_slot(current_slot, current_RA->entry.name);
						RA_r_cost-=1;
						strcpy(status_arr[current_RA->entry.id],"Accepted");
						//A R/A is finished
						if(RA_r_cost<=0){
							//Get next until it is not on the same slot
							progress_arr[current_RA->entry.id] = 100;
							do{
								getNextRA(&current_RA);
								if(current_RA!=NULL && !validate_RA()){
									strcpy(status_arr[current_RA->entry.id],"Rejected");
									progress_arr[current_RA->entry.id] = 0;
									//printf("RA ID %d: is rejected (can't validate)\n", current_RA->entry.id);
								}
							}while(current_RA!=NULL && !validate_RA());							
							if(current_RA!=NULL){
								RA_r_cost = current_RA->entry.edf_cost;
							}
						}
					}
					else{
						//printf("u don't have time to join current activity\n");
						strcpy(status_arr[current_RA->entry.id],"Rejected");
						progress_arr[current_RA->entry.id] = 0;
						getNextRA(&current_RA);
						if(current_RA!=NULL){
							RA_r_cost = current_RA->entry.edf_cost;
							if(date_cmp(current_slot->timeslot.edf_date,current_RA->entry.edf_date)==0 && current_RA->entry.edf_time<=current_slot->timeslot.edf_time){
								time_check();							
							}
						}
					}
				}
				
				void assign_PA(){
					assign_slot(current_slot, current_PA->entry.name);
						if(PA_r_cost==current_PA->entry.edf_cost){
							strcpy(status_arr[current_PA->entry.id],"Accepted");
						}
						PA_r_cost-=1;
						//A P/A is finished
						if(PA_r_cost<=0 || (date_cmp(current_PA->entry.edf_date,current_slot->timeslot.edf_date)==0 && current_slot->timeslot.edf_time==edf_end_hr-1)){
							//printf("Current PA finished, finding next PA.\n");
							float temp = 1;
							temp = ((temp*current_PA->entry.edf_cost-PA_r_cost)/current_PA->entry.edf_cost)*100;
							progress_arr[current_PA->entry.id] = (int)temp;
							//printf("%d has completed\n", (int) temp);
							do{
								getNextPA(&current_PA);
								if(current_PA!=NULL && !validate_PA()){
									strcpy(status_arr[current_PA->entry.id],"Rejected");
									progress_arr[current_PA->entry.id] = 0;
									//printf("PA ID%d : is rejected\n", current_PA->entry.id);
								}
							}while(current_PA!=NULL && !validate_PA());
								
							
							if(current_PA!=NULL){
								PA_r_cost = current_PA->entry.edf_cost;
							}				
						}
				}
				
				
				// Check the presence of entries and period
				if(entry_head == NULL || timetable_head==NULL){
					printf("Entries or period missing.\n");
					return;
				}
				else{
					//Get the first PA / RA
					edf_entry_list* temp = entry_head;
					int found = 0;
					while(found==0&&temp!=NULL){
						if(temp->entry.type_piority<3){
							current_PA = temp;
							found=1;
						}
						temp = temp->next_entry;
					}
					while((current_PA!=NULL) 
						&& 
						!date_inrange(edf_start_date,current_PA->entry.edf_date,edf_end_date) 
						){
						strcpy(status_arr[current_PA->entry.id],"Rejected");
						progress_arr[current_PA->entry.id] = 0;
						//printf("start PA ID%d : is rejected\n", current_PA->entry.id);
						getNextPA(&current_PA);
					}
					
					
					found = 0;
					temp = entry_head;
					
					while(found==0&&temp!=NULL){
						if(temp->entry.type_piority>2){
							current_RA = temp;
							found=1;
						}
						temp = temp->next_entry;
					}
		
					while((current_RA!=NULL)
						&&
						(
							(!date_inrange(edf_start_date,current_RA->entry.edf_date, edf_end_date))
							||
							((current_RA->entry.edf_time<edf_start_hr)||(current_RA->entry.edf_time+current_RA->entry.edf_cost>edf_end_hr))
						)
						
						){
						strcpy(status_arr[current_RA->entry.id],"Rejected");
						progress_arr[current_RA->entry.id] = 0;
						//printf("start RA ID%d : is rejected\n", current_RA->entry.id);
						getNextRA(&current_RA);
					}
					
					
					//There is at least 1 Project or assignment
					
					current_slot = timetable_head;
					
					//Initializing cost of current entry
					if(current_PA!=NULL){
						PA_r_cost = current_PA->entry.edf_cost;
					}
					//There is at least 1 revision / activity
					if(current_RA!=NULL){
						RA_r_cost = current_RA->entry.edf_cost;
					}
				}
				
				
				
				
				
				// Assign until out of slots
				while(i<=total_slots){
					//printf("i=%d;\n",i);
					//There's still at least 1 PA / 1 RA
					if(current_PA!=NULL&&current_RA!=NULL){
						//2 entries on same date
						if(date_cmp(current_slot->timeslot.edf_date,current_RA->entry.edf_date)==0){
							//Activity start on this slot or in progress
							if((current_slot->timeslot.edf_time==current_RA->entry.edf_time)||(RA_r_cost<current_RA->entry.edf_cost)){
								// RA in progress
								if(RA_r_cost<(current_RA->entry.edf_cost)){
									//printf("Activity in progress.\n");
									assign_slot(current_slot, current_RA->entry.name);
									RA_r_cost-=1;
									//A R/A is finished
									if(RA_r_cost<=0){
										strcpy(status_arr[current_RA->entry.id],"Accepted");
										progress_arr[current_RA->entry.id] = 100;
										do{
											getNextRA(&current_RA);
											if(current_RA!=NULL && !validate_RA()){
												strcpy(status_arr[current_RA->entry.id],"Rejected");
												progress_arr[current_RA->entry.id] = 0;
												//printf("RA ID %d: is rejected (can't validate)\n", current_RA->entry.id);
											}
										}while(current_RA!=NULL && !validate_RA());										
										if(current_RA!=NULL){
											RA_r_cost = current_RA->entry.edf_cost;
										}	
									}
								}
								// RA starts at this slot
								else{
									time_check();
								}
							}
							//Activity not on this slot
							else{
								assign_PA();
							}
						}
						//Not on same date
						else{
							assign_PA();
						}			
					}
					else if(current_PA!=NULL&&current_RA==NULL){
						assign_PA();
					}
					else if(current_PA==NULL&&current_RA!=NULL){
						//printf("No more PA and start scheduling RAs\n");
						//While RA is expired						
						if(date_cmp(current_slot->timeslot.edf_date,current_RA->entry.edf_date)==0){
							//time is same
							if( (current_slot->timeslot.edf_time==current_RA->entry.edf_time)||(RA_r_cost < current_RA->entry.edf_cost)){
								// Scheduler accepted adding RA
								assign_slot(current_slot, current_RA->entry.name);
								RA_r_cost-=1;
								//A R/A is finished
								if(RA_r_cost<=0){
									strcpy(status_arr[current_RA->entry.id],"Accepted");
									progress_arr[current_RA->entry.id] = 100;
									do{
										getNextRA(&current_RA);
										if(current_RA!=NULL && !validate_RA()){
											strcpy(status_arr[current_RA->entry.id],"Rejected");
											progress_arr[current_RA->entry.id] = 0;											
										}
									}while(current_RA!=NULL && !validate_RA());										
									if(current_RA!=NULL){
										RA_r_cost = current_RA->entry.edf_cost;
									}
								}
							}
						}
					}
					//Both ptr is null and no more entries need to be scheduled;
					else{
						break;
					}
					current_slot = current_slot->next_slot;
					i++;
				}
			}
			
			
			void print_timetable_edf(){
				int i, j = 0;				
				for(i=0; i < 14; i++){
					for(j=0; j <4; j++){
						//printf("Timetable print : Day=%d  Slot=%d is %s\n",i,j,timetable[j][i]);
					}
				}
			}
			
			//char timetable2[4][14][50] = {""};
			//!!!Only for 14 days 4hrs timetables!!!
			void import_EDF_2_timetable(){
				edf_timetable_list* temp_ptr;
				if(timetable_head!=NULL){
					temp_ptr = timetable_head;
				}
				else{
					printf("EDF period haven't added.\n");
				}
				int i, j = 0;				
				for(i=0; i < 14; i++){
					for(j=0; j <4; j++){
						if(temp_ptr!=NULL && strcmp(temp_ptr->timeslot.name,"N/A")!=0){
							strncpy(timetable[j][i], temp_ptr->timeslot.name, sizeof(timetable[j][i])-1);
							//printf("i=%d j=%d imported name : %s\n", i,j,timetable[j][i]);
						}
						else{
							strcpy(timetable[j][i],"N/A");
						}
						temp_ptr = temp_ptr->next_slot;
					}
				}
			}
			
			
			void command_handler(int id, char* input){
				char *command;
				char *parameters[5];
				char *token;
				char *saveptr = NULL;
				int i = 0;
				
				//printf("command handler running\n");
				//printf("Input is %s\n", input);
				command = strtok_r(input, " ", &saveptr);	//Extract a token
				//printf("Command is %s\n", command);
				token = strtok_r(NULL, " ", &saveptr);
				
				while(token != NULL && i < 5){
					//printf("token %d is %s\n", i,token);
					parameters[i] = token;
					token = strtok_r(NULL, " ", &saveptr);
					i++;
				}
				//printf("Parameter read done.\n", token);
				i--;
				//printf("i is now %d.\n", i);
				
				if(strcmp(command,"runS3")!=0){
					if(atoi(parameters[i]) == 0){
						//printf("Scheduler received invalid message from pipe! Nothing need to do.\n");
						return;
					}
				}
				
				
				//printf("The input is valid\n");
				//Add period
				if(strcmp(command,"addPeriod")==0){
					//printf("Running add period.\n");
					timetable_head = NULL;
					timetable_tail = NULL;
					addPeriod(parameters);
				}
				
				//Add project
				else if(strcmp(command,"addProject")==0){
					addProject(id, parameters);
				}
				
				//Add assignment
				else if(strcmp(command,"addAssignment")==0){
					addAssignment(id, parameters);
				}
				
				//Add revision
				else if(strcmp(command,"addRevision")==0){
					addRevision(id, parameters);
				}
				
				//Add activity
				else if(strcmp(command,"addActivity")==0){
					addActivity(id, parameters);
				}			
				// Start scheduling
				else if(strcmp(command,"runS3")==0){
					if(strcmp(parameters[0],"EDF")==0){
						//printf("EDF command handler: Running S3 EDF.\n");
						//print_entries();
						EDF_RunS3();
						//print_slots();
						import_EDF_2_timetable();
						//print_timetable_edf();
					}
					/*
					else{
						printf("EDF Scheduler received runS3 from pipe! But nothing need to do.\n");
					}
					*/
				}
				/*
				// Exit program 
				else if(strcmp(command,"exitS3")==0){
					exit(0);
				}
				*/
				else{
					printf("EDF module unable to resolve this command.\n");
				}
				return;
			}
			////EDF
			
			
			
            write(fd[i_write_pipe][1], "OK", 2); /* for synchronization (the scheduling module is ready to read new data from input module in the beginning of the program) */
            
            /* read user inputs until the write end of input module is closed */
            while((a = read(fd[i_read_pipe][0], buffer, 50)) > 0) {
                write(fd[i_write_pipe][1], "OK", 2); /* for synchronization (the input module can now write new data to the pipe as the scheduling module finishes reading old data in the pipe) */
                buffer[a] = 0;
                
                /* read the first 5 characters to determine to type of command */
                for (b = 0; b < 5; b ++) command[b] = buffer[b];
                command[b] = 0;
                
				char edf_input[50];
				strcpy(edf_input, buffer);
				//printf("EDF received command: %s\n", edf_input);
				
				if(strcmp(command, "runS3") != 0){
					command_handler(id, edf_input);
				}
				
                /* addPeriod */
                if (strcmp(command, "addPe") == 0) {
                    
                    /* saving the starting day */
                    for (a = 0; a < 2; a ++) string[a] = buffer[a + 18];
                    string[a] = 0;
                    start_day = atoi(string);
                    
                    /* saving the ending day */
                    for (a = 0; a < 2; a ++) string[a] = buffer[a + 29];
                    string[a] = 0;
                    end_day = atoi(string);
                    
                    /* saving the starting hour */
                    for (a = 0; a < 2; a ++) string[a] = buffer[a + 32];
                    string[a] = 0;
                    start_hour = atoi(string);
                    
                    /* saving the ending hour */
                    for (a = 0; a < 2; a ++) string[a] = buffer[a + 38];
                    string[a] = 0;
                    end_hour = atoi(string);
                    
                    /* add the timeslots or period to the timetable */
                    for (a = start_day - 8; a <= end_day - 8; a ++) {
                        for (b = start_hour - 19; b < end_hour - 19; b++) {
                            strcpy(timetable[b][a], "N/A"); /* N/A means that that period is added but with no assignment */
                        }
                    }
                    
                }
                /* addRevision / addActivity / addAssignment / addProject */
                else if (strcmp(command, "addRe") == 0 || strcmp(command, "addAc") == 0 || strcmp (command, "addAs") == 0 || strcmp (command, "addPr") == 0) {
                    
                    /* saving the type into the type_arr */
                    if (strcmp (command, "addRe") == 0) {
                        type_arr[id] = malloc((strlen("addRevision") + 1) * sizeof(char));
                        strcpy(type_arr[id], "addRevision");
                    }
                    else if (strcmp (command, "addAc") == 0) {
                        type_arr[id] = malloc((strlen("addActivity") + 1) * sizeof(char));
                        strcpy(type_arr[id], "addActivity");
                    }
                    else if (strcmp (command, "addAs") == 0) {
                        type_arr[id] = malloc((strlen("addAssignment") + 1) * sizeof(char));
                        strcpy(type_arr[id], "addAssignment");
                    }
                    else if (strcmp (command, "addPr") == 0) {
                        type_arr[id] = malloc((strlen("addProject") + 1) * sizeof(char));
                        strcpy(type_arr[id], "addProject");
                    }
                    
                    /* calculate the length of the string for the duration */
                    temp = 1;
                    b = a - 4;
                    while (buffer[b --] != ' ') temp++;
                    
                    /* saving the duration into the duration_arr */
                    
                    for (b = 0; b < temp; b ++) string[b] = buffer[a - 2 - temp + b];
                    string[b] = 0;
                    duration_arr[id] = atoi(string);
                    
                    /* addRevision / addActivity */
                    
                    if (strcmp (command, "addRe") == 0 || strcmp (command, "addAc") == 0) {
                        
                        /* saving the time into the time_arr */
                        for (b = 0; b < 2; b ++) string[b] = buffer[a - 8 - temp + b];
                        string[b] = 0;
                        time_arr[id] = atoi(string);
                        
                        /* saving the day into the date_arr */
                        for (b = 0; b < 2; b ++) string[b] = buffer[a - 11 - temp + b];
                        string[b] = 0;
                        date_arr[id].day = atoi(string);
                        
                        /* saving the month into the date_arr */
                        for (b = 0; b < 2; b ++) string[b] = buffer[a - 14 - temp + b];
                        string[b] = 0;
                        date_arr[id].month = atoi(string);
                        
                        /* saving the year into the date_arr */
                        for (b = 0; b < 4; b ++) string[b] = buffer[a - 19 - temp + b];
                        string[b] = 0;
                        date_arr[id].year = atoi(string);
                        
                        /* saving the event name into the event_name_arr */
                        for (b = 0; b <= a - 33 - temp; b ++) string[b] = buffer[b + 12];
                        string[b] = 0;
                        event_name_arr[id] = malloc((strlen(string) + 1) * sizeof(char));
                        strcpy(event_name_arr[id], string);
                        
                    }
                    else {
                        /* saving the time into the time_arr */
                        for (b = 0; b < 2; b ++) string[b] = buffer [a - 5 - temp + b];
                        string[b] = 0;
                        date_arr[id].day = atoi(string);
                        
                        /* saving the month into the date_arr */
                        for (b = 0; b < 2; b ++) string[b] = buffer[a - 8 - temp + b];
                        string[b] = 0;
                        date_arr[id].month = atoi(string);
                        
                        /* saving the year into the date_arr */
                        for (b = 0; b < 4; b ++) string[b] = buffer[a - 13 - temp + b];
                        string[b] = 0;
                        date_arr[id].year = atoi(string);
                        
                        /* saving the event name into the event_name_arr */
                        if (strcmp (command, "addAs") == 0) for (b = 0; b <= a - 29 - temp; b ++) string[b] = buffer[b + 14];
                        else for (b = 0; b <= a - 26 - temp; b ++) string[b] = buffer[b + 11];
                        string[b] = 0;
                        event_name_arr[id] = malloc((strlen(string) + 1) * sizeof(char));
                        strcpy(event_name_arr[id], string);
                    }
                    
                    /* the status of the invalid input is set to "Invalid" */
                    if (buffer[a - 1] == '0') {
                        strcpy(status_arr[id], "Invalid");
                        progress_arr[id] = 0;
                    }
                    
                    buffer[a - 2] = 0;
                    strcpy(request[id], buffer); /* copy the event to the request list */
                    
                    id ++; /* increment the id */
                    
                }
                /* runS3 */
                else if (strcmp(command, "runS3") == 0) {
                    
                    /* reading the algorithm */
                    for (a = 0; a < 3; a ++) string[a] = buffer[a + 6];
                    string[a] = 0;
                    
                    if (strcmp(string, "FCF") == 0) {
                        FCFS(id,type_arr,date_arr,time_arr,event_name_arr,duration_arr,timetable,progress_arr,status_arr);
                    }
                    else if (strcmp(string, "EDF") == 0) {
                        command_handler(id, edf_input);
                    }
                    
                    read(fd[oa_read_pipe][0], syn, 10); /* for synchronization (wait until the output & analyzer module finishes reading old data in the pipe) */
                    write(fd[oa_write_pipe][1], buffer, strlen(buffer)); /* send the user input as a string to the output & analyzer module */
                    
                    read(fd[oa_read_pipe][0], syn, 10); /* for synchronization (wait until the output & analyzer module finishes reading old data in the pipe) */
                    write(fd[oa_write_pipe][1], &id, sizeof(id));
                    
                    read(fd[oa_read_pipe][0], syn, 10); /* for synchronization (wait until the output & analyzer module finishes reading old data in the pipe) */
                    write(fd[oa_write_pipe][1], request, 1000*50*sizeof(char));
                    
                    read(fd[oa_read_pipe][0], syn, 10); /* for synchronization (wait until the output & analyzer module finishes reading old data in the pipe) */
                    write(fd[oa_write_pipe][1], status_arr, 1000*10*sizeof(char));
					
					read(fd[oa_read_pipe][0], syn, 10); /* for synchronization (wait until the output & analyzer module finishes reading old data in the pipe) */
                    write(fd[oa_write_pipe][1], progress_arr, 1000*sizeof(int));
                    
                    read(fd[oa_read_pipe][0], syn, 10); /* for synchronization (wait until the output & analyzer module finishes reading old data in the pipe) */
                    write(fd[oa_write_pipe][1], timetable, 4*14*50*sizeof(char)); /* send the time table to the output & analyzer module */
                    
                    /* reset the status of valid requests for next scheduling */
                    for (a = 0; a < 1000; a ++) {
                        if (strcmp(status_arr[a],"Invalid") != 0) {
                            strcpy(status_arr[a],"");
                        }
                    }
                    
                    /* reset the timetable for next scheduling */
                    for (a = 0; a < 14; a ++) {
                        for (b = 0; b < 4; b ++) {
                            if (strcmp(timetable[b][a], "") != 0) strcpy(timetable[b][a], "N/A");
                        }
                    }
                }
            }
            
            /* close all the pipes ends when the input module stops writing (process ends) */
            close(fd[i_read_pipe][0]);
            close(fd[i_write_pipe][1]);
            close(fd[oa_read_pipe][0]);
            close(fd[oa_write_pipe][1]);
            
            /* free all allocated space */
            for (a = 0; a < 1000; a ++) {
                if (type_arr[id] != NULL) free(type_arr[id]);
                if (event_name_arr[id] != NULL) free(event_name_arr[id]);
            }
        }
        /* end of 1st child 1st child (Parent: input module, 1st child: scheduling module) */
        
        /* 2nd child (Parent: input module, 2st child: output&analyzer module) */
        if (pid==2){
			char * filename="";
			char * filename2="";
			char a_name[5]="";
			int NA=0; /*counting time slots used (how many N/A)*/
			int acc=0;/*number of accepted*/
			int rej=0;/*number of rejected*/
			int X=0; /*counting time slots used (how many unused period)*/
			int c=0; /*counting status*/
            int id=0;
            int count=6;
            int s_read_pipe = 3; /* fd[3] is the pipe for output&analyzer module to read from scheduling module */
            int s_write_pipe = 2; /* fd[2] is the pipe for output&analyzer module to write to scheduling module */
            FILE * fp1;
            FILE * fp2;
            /* close unused pipe ends */
            for (a = 0; a < 4; a ++) {
                if (a != s_read_pipe && a != s_write_pipe) {
                    close(fd[a][0]);
                    close(fd[a][1]);
                }
                else if (a == s_read_pipe) close(fd[a][1]);
                else close(fd[a][0]);
            }
            
            char timetable[4][14][50] = {""};
            char request[1000][50] = {""};
            char status_arr[1000][10] = {""};
			int progress_arr[1000];
            
            write(fd[s_write_pipe][1], "OK", 2); /* for synchronization (the output & analyzer module is ready to read new data from scheduling module in the beginning of the program) */
            
            /* read user inputs until the write end of scheduling module is closed */
            while((a = read(fd[s_read_pipe][0], buffer, 50)) > 0 ){
                write(fd[s_write_pipe][1], "OK", 2); /* for synchronization (the scheduling module can now write the time to the pipe as the output & analyzer module finishes reading old data in the pipe) */
                read(fd[s_read_pipe][0], &id, sizeof(id));
                write(fd[s_write_pipe][1], "OK", 2); /* for synchronization (the scheduling module can now write the time to the pipe as the output & analyzer module finishes reading old data in the pipe) */
                read(fd[s_read_pipe][0], request, 1000*50*sizeof(char));
                write(fd[s_write_pipe][1], "OK", 2); /* for synchronization (the scheduling module can now write the time to the pipe as the output & analyzer module finishes reading old data in the pipe) */
                read(fd[s_read_pipe][0], status_arr, 1000*10*sizeof(char));
                write(fd[s_write_pipe][1], "OK", 2); /* for synchronization (the scheduling module can now write the time to the pipe as the output & analyzer module finishes reading old data in the pipe) */
                read(fd[s_read_pipe][0], progress_arr, 1000*sizeof(int));
                write(fd[s_write_pipe][1], "OK", 2); /* for synchronization (the scheduling module can now write the time to the pipe as the output & analyzer module finishes reading old data in the pipe) */
				read(fd[s_read_pipe][0],timetable,4*14*50*sizeof(char));
                write(fd[s_write_pipe][1], "OK", 2); /* for synchronization (the scheduling module can now write the time to the pipe as the output & analyzer module finishes reading old data in the pipe) */
                
				/* Initializing */
				NA=0; /*counting time slots used (how many N/A)*/
				acc=0;/*number of accepted*/
				rej=0;/*number of rejected*/
				X=0; /*counting time slots used (how many unused period)*/
				c=0; /*counting status*/
  			    memset(a_name, 0, sizeof(a_name));
				count=6;
				
				while(buffer[count]!=' '){
					a_name[count-6]=buffer[count];
                    count++;
                }
				if(strcmp(a_name,"FCFS")==0){
					filename="S3_report_FCFS.dat";
					filename2="S3_FCFS.log";
				}
				else{
					filename="S3_report_EDF.dat";
					filename2="S3_EDF.log";
				}
				fp1= fopen(filename,"w+");
				
                fprintf(fp1,"Alice Timetable\r\n");
                fprintf(fp1,"2019-04-08 to 2019-04-21\r\n");
                fprintf(fp1,"Algorithm: ");
                
				
				fprintf(fp1,"%s",a_name);
                fprintf(fp1,"\r\n\r\n");
                fprintf(fp1,"%-20s","Date");
                fprintf(fp1,"%-20s","19:00");
                fprintf(fp1,"%-20s","20:00");
                fprintf(fp1,"%-20s","21:00");
                fprintf(fp1,"%-20s\r\n","22:00");
                for (a = 0; a < 14; a ++) {
                    for (b = 0; b < 5; b ++) {
                        if(b==0){
                            if(a<2){
                                fprintf(fp1, "%s%-12d", "2019-4-0",a + 8);
                            }
                            else{
                                fprintf(fp1, "%s%-13d", "2019-4-",a + 8);
                            }
                        }
                        else{
                            if (strcmp(timetable[b-1][a], "") != 0) {
                                fprintf(fp1, "%-20s", timetable[b-1][a]);
                            }
                            else{
                                fprintf(fp1,"%-20s","X");
                            }
                        }
                    }
                    fprintf(fp1,"\r\n");
                    
                }
                fprintf(fp1,"\r\n***Summary Report***\r\n\r\n");
                fprintf(fp1,"Algorithm used: ");
                fprintf(fp1,"%s",a_name);
                fprintf(fp1,"\r\nThere are %d requests.\r\n",id);
				
                while(c<id){
                    if(strcmp(status_arr[c],"Accepted")==0){acc++;}
                    else if(strcmp(status_arr[c],"Rejected")==0){rej++;}
                    c++;
                }
                fprintf(fp1,"Number of request accepted: %d\r\n",acc);
                fprintf(fp1,"Number of request rejected: %d\r\n\r\n",rej);
				
				for (a = 0; a < 14; a ++) {
                    for (b = 0; b < 4; b ++) {
						if(strcmp(timetable[b][a],"N/A")==0){NA++;}
						else if(strcmp(timetable[b][a],"")==0){X++;}
					}
				}
				fprintf(fp1,"Number of time slots used: %d (%d",4*14-X-NA,((4*14-X-NA)*100/(4*14-X)*100)/100);
				fprintf(fp1,"%)");
				
				
                fclose(fp1);
				fp2= fopen(filename2,"w+");
				fprintf(fp2,"***Log File - %s***\r\n",a_name);
				fprintf(fp2,"%-8s","ID");
				fprintf(fp2,"%-43s","Event");
				fprintf(fp2,"%-s\r\n","Accepted/Rejected");
				fprintf(fp2,"-------------------------------------------------------------\r\n");
				for(count=0;count<id;count++){
					fprintf(fp2,"000%-5d",count+1);
					if(progress_arr[count]!=100&&progress_arr[count]!=0){
						fprintf(fp2,"%s(%-d",request[count],progress_arr[count]);
						fprintf(fp2,"%-7s","%)");
					}
					else{fprintf(fp2,"%-45s",request[count]);}
					fprintf(fp2,"%-s\r\n",status_arr[count]);
				}
				fclose(fp2);
				
            }
            
            /* close all the pipe ends when the scheduling module stops writing (process ends) */
            close(fd[s_read_pipe][0]);
            close(fd[s_write_pipe][1]);
        }
        /* end of 2nd child (Parent: input module, 2st child: output&analyzer module)  */
    }
    /* Parent process: Input module */
    else {
        int s_read_pipe = 0; /* fd[0] is the pipe for input module to read from scheduling module */
        int s_write_pipe = 1; /* fd[1] is the pipe for input module to write to scheduling module */
        
        /* close unused pipe ends */
        for (a = 0; a < 4; a ++) {
            if (a != s_read_pipe && a != s_write_pipe) {
                close(fd[a][0]);
                close(fd[a][1]);
            }
            else if (a == s_read_pipe) close(fd[a][1]);
            else close(fd[a][0]);
        }
        
        char input[50] = ""; /* the string to be sent to the scheduler  (format: [user input] [indicator]) (indicator: '0': invalid / '1': valid) */
        char * return_string;
        char algorithm[50];
        char file_name[50];
        
        printf("   ~~WELCOME TO S3~~\n"); /* start the program properly */
        
        while (strcmp(command, "exitS3") != 0) {
            
            /* prompt until the user enter "exitS3" */
            printf("Please enter:\n");
            scanf("%s", command); /* scanning the first string in an input line */
            
            /* call different functions according to the first string in the input line */
            if (strcmp(command, "addPeriod") == 0) {
                return_string = validatePeriod();
                strcpy(input, return_string);
                free(return_string); /* free the allocated space */
                /* send the input to scheduler only if the input for addPeriod is valid */
                if (input[44] == '1') {
                    read(fd[s_read_pipe][0], syn, 10); /* for synchronization (wait until the scheduling module finishes reading old data in the pipe) */
                    write(fd[s_write_pipe][1], input, strlen(input));
                }
            }
            else if (strcmp(command, "addRevision") == 0 || strcmp(command, "addActivity") == 0 || strcmp(command, "addAssignment") == 0 || strcmp(command, "addProject") == 0) {
                return_string = validateEvent(&id, command, stdin);
                strcpy(input, return_string);
                free(return_string); /* free the allocated space */
                read(fd[s_read_pipe][0], syn, 10); /* for synchronization (wait until the scheduling module finishes reading old data in the pipe) */
                write(fd[s_write_pipe][1], input, strlen(input)); /* send the input to scheduler */
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
                    return_string = validateEvent(&id, command, fp);
                    strcpy(input, return_string);
                    free(return_string); /* free the allocated space */
                    read(fd[s_read_pipe][0], syn, 10); /* for synchronization (wait until the scheduling module finishes reading old data in the pipe) */
                    write(fd[s_write_pipe][1], input, strlen(input)); /* send the input to scheduler */
                }
                fclose(fp); /* close the file */
            }
            else if (strcmp(command, "runS3") == 0) {
                strcpy(input, command);
                strcat(input, " ");
                scanf("%s", algorithm); /* scan algorithm */
                if (strcmp(algorithm, "FCFS") != 0 && strcmp(algorithm, "EDF") != 0) {
                    printf("Invalid algorithm (Valid: FCFS / EDF)\n");
                }
                strcat(input, algorithm); /* concatenate the algorithm to the input */
                strcat(input, " ");
                scanf("%s", file_name); /* scan filename */
                strcat(input, file_name); /* concatenate the filename to the input */
                if (strcmp(algorithm, "FCFS") == 0 || strcmp(algorithm, "EDF") == 0) {
                    read(fd[s_read_pipe][0], syn, 10); /* for synchronization (wait until the scheduling module finishes reading old data in the pipe) */
                    write(fd[s_write_pipe][1], input, strlen(input)); /* send the input to scheduler */
                }
            }
            else if (strcmp(command, "exitS3") == 0) {
                continue;
            }
            else {
                printf("invalid command\n");
            }
        }
        printf("Bye-bye!\n"); /* end the program properly */
        
        /* close all the pipe ends when the input module stops writing (process ends) */
        close(fd[s_read_pipe][0]);
        close(fd[s_write_pipe][1]);
        
        while(wait(NULL)>0);
        exit(0);
    }
    /* end of parent process (input module) */
}
