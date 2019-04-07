#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* structure for a date (Year + Month + Day) */
struct date {
    int year;
    int month;
    int day;
};

typedef struct date Date;
typedef struct date * Date_Ptr;

/* function prototype */
char * validatePeriod(void);
char * validateEvent(int * id, char * type, FILE * fp);

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

int main(int argc, const char * argv[]) {
    int a, b, temp;
    int id = 0;
    int parent_id = getpid();
    int pid;
    int fd[2][2]; /* (1 * 2) pipes for communation between 1 pair of module (input module & scheduling module) */
	int edf[2][2];  /*Read write pipe pair for edf algorithm scheduling*/
    char command [20] = "";
    char buffer[50];
    
    /* create pipes */
    for (a = 0; a < 2; a ++) {
        if (pipe(fd[a]) < 0) {
            printf("Pipe creation error\n");
            exit(1);
        }
    }
	
	/* create edf pipes*/
	for (a = 0; a < 2; a ++) {
        if (pipe(fd[a]) < 0) {
            printf("Pipe edf creation error\n");
            exit(1);
        }
    }
    
    pid = fork(); /* create 1 child (Parent: input module, 1st child: scheduling module) */
    
    if (pid < 0) {
        printf("Fork Failed\n");
        exit(1);
    }
    /* child process */
    else if (pid == 0) {
        
        pid = getpid() - parent_id;
        int read_pipe;
        int write_pipe;
		date edf_start_date;
		date edf end_date;
		int edf_period_days;
		int edf_period_day_hrs;
		int edf_start_hr;
		int edf_end_hr;
        
		if(pid == 0){
			/*Assigning pipes and closing unused ends*/
			/*edf[0] is for listening*/
			/*edf[1] is for writing back*/
			read_pipe = edf[0][0];
			close(edf[0][1]);
			write_pipe = edf[1][1];
			close(edf[1][0]);
			
			
			/* Event entry to be scheduled */
			typedef struct edf_entry {
				
				char* name;		/* Name of the activity */
				Date edf_date;		/* Date of deadline or activity */
				int edf_time;		/* Time start hr:0-24 */
				int edf_cost		/* Time cost for the activity in hrs */
				int type_piority;	/* Indicate the type of activity from 1 to 4, e.g. Project = 1 */
				
			} edf_entry;
			
			
			/* List of event entries */
			typedef struct edf_entry_list {
				
				edf_entry entry;
				edf_entry_list* next_entry = NULL;
				
			} edf_entry_list;
			
			edf_entry_list* entry_head = NULL;
			edf_entry_list* entry_tail = NULL;
			
			
			/* Time slot element for timetable list */
			typedef struct edf_timeslot {
				
				char* name;
				Date edf_date;
				int edf_time;
				
			} edf_timeslot;
			
			
			/* A list of time slots to represent the timetable (slots by slots) */
			typedef struct edf_timetable_list{
				
				edf_timeslot timeslot;
				edf_timetable_list* next_slot = NULL;
				
			} edf_timetable_list;
			
			edf_timetable_list* timetable_head = NULL;
			edf_timetable_list* timetable_tail = NULL;
			
			
			//OK // Return no. of days between 2 dates in 2019
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
			
			//OK /*Compare 2 dates if date1 is later than date2    1: Yes  -1: No  0: Identical*/
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
			
			//OK // Return the date of tomorrow 
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
			
			/*Insert an event entry into the list*/  //maybe ok
			int insert_entry(char* insrt_name, Date insrt_date, int insrt_time, int insrt_cost, int insrt_type){
				edf_entry_list* temp = new edf_entry_list;
				temp.enrty.name = insrt_name;
				temp.enrty.edf_date = insrt_date;
				temp.enrty.edf_time = insrt_time;
				temp.enrty.edf_cost = insrt_cost;
				temp.enrty.type_piority = insrt_type;
				
				
				edf_entry_list* entry_list_ptr;
				
				if(entry_head==NULL){
					entry_head = temp;
					entry_tail = temp;
				}
				else{
					entry_list_prev = NULL;
					entry_list_ptr = entry_head;
					while(true){
						/*Date of new entry is later than current*/ //checked
						if(date_cmp(date_entry_list_ptr.entry.edf_date, temp.enrty.edf_date) == -1){
							if(entry_list_ptr.next_entry!=NULL){
								entry_list_prev = entry_list_ptr;
								entry_list_ptr = entry_list_ptr.next_entry;
							}
							else{
								entry_list_ptr.next_entry = temp;
								entry_tail = temp;
								return 1;
							}
						}
						/*Date of new entry is eariler than current*/  //checked
						else if(date_cmp(date_cmpentry_list_ptr.entry.edf_date, temp.enrty.edf_date) == 1){
							if(entry_list_prev != NULL){
								entry_list_prev.next_entry = temp;
								temp.next_entry = entry_list_ptr;
							}
							else{
								temp.next_entry = entry_head;
								entry_head = temp;
							}
							return 1;
							
						}
						/*Date of new entry is identical*/
						else{
							/*Date of new entry is later than current time*/ //checked
							if(temp.enrty.edf_time > entry_list_ptr.enrty.edf_time){
								if(entry_list_ptr.next_entry!=NULL){
									entry_list_prev = entry_list_ptr;
									entry_list_ptr = entry_list_ptr.next_entry;
								}
								else{
									entry_list_ptr.next_entry = temp;
									entry_tail = temp;
									return 1;
								}
							}
							/*Date of new entry is earlier than current time*/
							else if(temp.enrty.edf_time < entry_list_ptr.enrty.edf_time){
								if(entry_list_prev != NULL){
									entry_list_prev.next_entry = temp;
									temp.next_entry = entry_list_ptr;
								}
								else{
									temp.next_entry = entry_head;
									entry_head = temp;
								}
								return 1;
							}
							/*Date of new entry is same to current time*/
							else{
								/*New entry has higher priority*/ //checked
								if(entry_list_ptr.entry.type_piority < temp.enrty.type_piority){
									if(entry_list_prev != NULL){
										entry_list_prev.next_entry = temp;
										temp.next_entry = entry_list_ptr;
									}
									else{
										temp.next_entry = entry_head;
										entry_head = temp;
									}
									return 1;
								}
								/*New entry has lower priority*/  //checked
								else if(entry_list_ptr.entry.type_piority > temp.enrty.type_piority){
									if(entry_list_ptr.next_entry!=NULL){
										entry_list_prev = entry_list_ptr;
										entry_list_ptr = entry_list_ptr.next_entry;
									}
									else{
										entry_list_ptr.next_entry = temp;
										entry_tail = temp;
										return 1;
									}
								}
								/*New entry has same priority*/
								else{
									if(entry_list_ptr.next_entry!=NULL){
										entry_list_prev = entry_list_ptr;
										entry_list_ptr = entry_list_ptr.next_entry;
									}
									else{
										entry_list_ptr.next_entry = temp;
										entry_tail = temp;
										return 1;
									}
								}
							}
						}
					}
				}
			}
			
			
			// Adding a timeslot at the end of the timetable //checked?
			int add_time_slot(char* insrt_name, Date insrt_date, int insrt_time){
				edf_timetable_list* temp;
				temp = malloc(sizeof(edf_timetable_list))
				temp.timeslot.name = insrt_name;
				temp.timeslot.edf_date = insrt_date;
				temp.timeslot.edf_time = insrt_time;
				
				if(entry_head==NULL){
					timetable_head = temp;
					timetable_tail = temp;
				}
				else{
					//edf_timetable_list* temp_ptr = timetable_head;
					//while(temp_ptr.next_slot!=NULL){
					//	temp_ptr = temp_ptr.next_slot;
					//}
					taimetable_tail.next_slot = temp;
					taimetable_tail = taimetable_tail.next_slot;
				}
			}
			
			//OK
			Date str2date(char* date1){
				char *token;
				char *saveptr;
				Date temp;
				temp.year = atoi(strtok_r(date1, "-", &saveptr));
				temp.month = atoi(strtok_r(NULL, "-", &saveptr));
				temp.day = atoi(strtok_r(NULL, "-", &saveptr));
				return temp;
			}
			
			// OK
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
			
			
			//OK
			int str2hr(char* time1){
				char *token;
				char *saveptr;
				int temp = -1;
				temp = atoi(strtok_r(time1, ":", &saveptr));
				return temp;
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
						temp_time++;
					}
					temp_time = edf_start_hr;
					temp_date = date_tmr(temp_date);
				}
				return;
			}
			
			void addProject(char* parameters[]){
				char *name = parameters[0];
				Date temp_date = str2date(parameters[1]);
				int temp_time = 24;
				int temp_cost = atoi(parameters[2]);
				int type = 1;
				insert_entry(name, temp_date, temp_time, temp_cost, type);
			}
			
			void addAssiginment(char* parameters[]){
				char *name = parameters[0];
				Date temp_date = str2date(parameters[1]);
				int temp_time = 24;
				int temp_cost = atoi(parameters[2]);
				int type = 2;
				insert_entry(name, temp_date, temp_time, temp_cost, type);
			}
			
			void addRevision(char* parameters[]){
				char *name = parameters[0];
				Date temp_date = str2date(parameters[1]);
				int temp_time = str2hr(parameters[2]);
				int temp_cost = atoi(parameters[3]);
				int type = 3;
				insert_entry(name, temp_date, temp_time, temp_cost, type);
			}
			
			void addActivity(char* parameters[]){
				char *name = parameters[0];
				Date temp_date = str2date(parameters[1]);
				int temp_time = str2hr(parameters[2]);
				int temp_cost = atoi(parameters[3]);
				int type = 4;
				insert_entry(name, temp_date, temp_time, temp_cost, type);
			}
			
			
			
			void EDF_RunS3(){
				edf_entry_list* current_PA = NULL;
				int PA_r_cost = 0;
				edf_entry_list* current_RA = NULL;
				int RA_r_cost = 0;
				edf_timetable_list* current_slot = NULL;
				
				int i = 1;
				int total_slots = edf_period_days*edf_period_day_hrs;
				
				void getNextPA(edf_entry_list* current){
					edf_entry_list* temp = current;
					int found = 0;
					while(found==0&&temp!=NULL){
						temp = temp.next_entry;
						if(temp!=NULL&&temp.entry.type_piority<3){
							found=1;
						}
					}
					if(found==0){
						current = NULL;
					}
					else{
						current = temp;
					}
					return;
				}
				
				void getNextRA(edf_entry_list* current){
					edf_entry_list* temp = current;
					int found = 0;
					while(found==0&&temp!=NULL){
						temp = temp.next_entry;
						if(temp!=NULL&&temp.entry.type_piority>2){
							found=1;
						}
					}
					if(found==0){
						current = NULL;
					}
					else{
						current = temp;
					}
					return;
				}
				
				void assign_slot(edf_timetable_list* location,char *name){
					location.timeslot.name = name;
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
					while(found==0&&temp.next_entry!=NULL){
						if(temp.entry.type_piority<3){
							current_PA = temp;
							found=1;
						}
						temp = temp.next_entry;
					}
					found = 0;
					while(found==0&&temp.next_entry!=NULL){
						if(temp.entry.type_piority>2){
							current_RA = temp;
							found=1;
						}
						temp = temp.next_entry;
					}
					
					//There is at least 1 Project or assignment
					current_slot = timetable_head;
					if(current_PA!=NULL){
						PA_r_cost = current_PA.entry.edf_cost;
					}
					//There is at least 1 revision / activity
					if(current_RA!=NULL){
						RA_r_cost = current_RA.entry.edf_cost;
					}
				}
				
				
				
				
				
				// Assign until out of slots
				while(i<=total_slots){
					//There's still at least 1 PA / 1 RA
					if(current_PA!=NULL&&current_RA!=NULL){
						//On same date
						if(date_cmp(current_PA.entry.edf.date,current_RA.entry.edf_date)==0){
							if(current_slot.timeslot.edf_time==current_RA.entry.edf_time||RA_r_time<current_RA.entry.edf_cost){
								// Scheduler accepted adding RA
								if(RA_r_time<current_RA.entry.edf_cost){
									assign_slot(current_slot, current_RA.entry.name);
									RA_r_cost-=1;
									//A R/A is finished
									if(RA_r_cost<=0){
										getNextRA(current_RA);
									}
								}
								// Scheduler haven't accepted adding RA
								else{
									
									int days_til_dead = date_days(current_PA.entry.edf_date, current_slot.edf_date)-1;
									int cost_left = (days_til_dead*edf_period_day_hrs) + (edf_end_hr-current_slot.edf_time)
									//Still have time to finish P/A
									if(cost_left-current_RA.entry.edf_cost >= PA_r_cost){
										assign_slot(current_slot, current_RA.entry.name);
										RA_r_cost-=1;
										//A R/A is finished
										if(RA_r_cost<=0){
											getNextRA(current_RA);
										}
									}
									//No time to finish P/A
									else{
										getNextRA(current_RA);
									}
								}
							}
						}
						//Not on same date
						else{
							assign_slot(current_slot, current_PA.entry.name);
							PA_r_cost-=1;
							//A P/A is finished
							if(PA_r_cost<=0){
								getNextPA(current_PA);
							}
						}
					}
					else if(current_PA!=NULL&&current_RA==NULL){
						assign_slot(current_slot, current_PA.entry.name);
						PA_r_cost-=1;
						//A P/A is finished
						if(PA_r_cost<=0){
							getNextPA(current_PA);
						}
					}
					else if(current_PA==NULL&&current_RA!=NULL){
						//timeslot date is same
						if(date_cmp(current_slot.timeslot.edf.date,current_RA.entry.edf_date)==0){
							//time is same
							if(current_slot.timeslot.edf_time==current_RA.entry.edf_time||RA_r_time<current_RA.entry.edf_cost){
								// Scheduler accepted adding RA
								assign_slot(current_slot, current_RA.entry.name);
								RA_r_cost-=1;
								//A R/A is finished
								if(RA_r_cost<=0){
									getNextRA(current_RA);
								}
							}
						}
						else{
							getNextRA(current_RA);
						}
					}
					//Both ptr is null and no more entires need to be scheduled;
					else{
						break;
					}
					current_slot = current_slot.next_slot;
					i++;
				}
			}
			
			//Process a command in EDF
			void command_handler(char* input){
				char *command;
				char *parameters[5];
				char *token;
				char *saveptr;
				int i = 0;
				
				command = strtok_r(input, " ", &saveptr);	//Extract a token
				token = strtok_r(NULL, " ", &saveptr);
				while(token != NULL && i < 5){
					parameters[i] = token;
					token = strtok_r(NULL, " ", &saveptr);
					i++;
				}
				i--;
				if(atoi(parameters[i]) == 0){
					printf("Scheduler received invalid message from pipe! Nothing need to do.\n");
					return;
				}
				//Add period
				if(strcmp(command,"addPeiod")==0){
					addPeriod(parameters);
				}
				//Add project
				else if(strcmp(command,"addProject")==0){
					addProject(parameters);
				}
				//Add assignment
				else if(strcmp(command,"addAssignment")==0){
					addAssignment(parameters);
				}
				//Add revision
				else if(strcmp(command,"addRevision")==0){
					addRevision(parameters);
				}
				//Add activity
				else if(strcmp(command,"addActivity")==0){
					addActivity(parameters);
				}
				// Start scheduling
				else if(strcmp(command,"runS3")==0){
					if(strcmp(parameters[0],"EDF")==0){
						EDF_RunS3();
					}
					else{
						printf("EDF Scheduler received runS3 from pipe! But nothing need to do.\n");
					}
				}
				// Exit program 
				else if(strcmp(command,"exitS3")==0){
					exit(0);
				}
				else{
					printf("Scheduler received message from pipe! But nothing need to do.\n");
				}
				return;
			}
			
			
			while(true){
				n = read(read_pipe, buffer, 50);
				if(n <= 0){
					printf("EDF Read error. Quitting.");
					break;
				}
				if(strlen(message) > 1){
					command_handler(buffer);
					
					memset(message,0,sizeof(buffer));
				}
			}
			
			
		}
		
		
        /* 1st child: schduling module */
        if (pid == 1) {
            read_pipe = 1; /* fd[1] is the pipe for scheduling module to read from input module */
            write_pipe = 0; /* fd[0] is the pipe for scheduling module to write to input module */
            
            /* close unused pipe end */
            for (a = 0; a < 2; a ++) {
                if (a != read_pipe && a != write_pipe) {
                    close(fd[a][0]);
                    close(fd[a][1]);
                }
                else if (a == read_pipe) close(fd[a][1]);
                else close(fd[a][0]);
            }
            
            char * type_arr [1000] = {NULL}; /* addRevision / addActivity / addAssignment / addProject */
            char * status_arr[1000] = {NULL}; /* Accepted / Rejected */
            char * event_name_arr[1000] = {NULL}; /* For addRevision (subject code) / addActivity (name of the event) / addAssignment (subject code with assignment number) / addProject (subject code with project number) */
            Date date_arr[1000]; /* For addRevision (date) / addActivity (date) / addAssignment (due date) / addProject (due date) */
            int time_arr[1000]; /* For addRevision (time) or addActivity (time) */
            for (a = 0; a < 1000; a ++) time_arr[a] = -1; /* -1 means not applicable e.g. addAssignment & addProject */
            int duration_arr[1000]; /* For addRevision / addActivity / addAssignment / addProject */
            int progress_arr[1000]; /* For addAssignment / addProject (% of completion) */
            for (a = 0; a < 1000; a ++) progress_arr[a] = -1; /* -1 means not applicable e.g. invalid input events */
            
            char timetable[4][14][50] = {""}; /* a timetable for events in 19:00 to 23:00 ([4]) from 2019-04-08 to 2019-04-21 ([14]) ("" means that that period is not added) */
            
            char string[50] = "";
            /* wait for response until the write end of input module is closed (stop reading) */
            while((a = read(fd[1][0], buffer, 50)) > 0) {
                buffer[a] = 0;
                
                /* read the first 5 characters to determine to type of command */
                for (b = 0; b < 5; b ++) command[b] = buffer[b];
                command[b] = 0;
                
                /* addPeriod */
                if (strcmp(command, "addPe") == 0) {
                    
                    int start_day, end_day, start_hour, end_hour;
                    
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
                            strcpy(timetable[b][a], "N/A"); //* N/A means that that period is added but with no assignment */
                        }
                    }
                    
                }
                /* addRevision / addActivity / addAssignment / addProject */
                else if (strcmp(command, "addRe") == 0 || strcmp(command, "addAc") == 0 || strcmp (command, "addAs") == 0 || strcmp (command, "addPr") == 0) {
                    
                    /* saving the type into the type_arr */
                    if (strcmp (command, "addRe") == 0) {
                        type_arr[id] = malloc(strlen("addRevision") * sizeof(char));
                        strcpy(type_arr[id], "addRevision");
                    }
                    else if (strcmp (command, "addAc") == 0) {
                        type_arr[id] = malloc(strlen("addActivity") * sizeof(char));
                        strcpy(type_arr[id], "addActivity");
                    }
                    else if (strcmp (command, "addAs") == 0) {
                        type_arr[id] = malloc(strlen("addAssignment") * sizeof(char));
                        strcpy(type_arr[id], "addAssignment");
                    }
                    else if (strcmp (command, "addPr") == 0) {
                        type_arr[id] = malloc(strlen("addProject") * sizeof(char));
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
                        
                        /* the input the invalid the the input date and time is out of the "period range" entered */
                        if (strcmp(timetable[time_arr[id] - 19][date_arr[id].day - 8], "N/A") != 0) {
                            printf("ID: %d, The timeslot for input date and time is not available\n", id + 1);
                            buffer[a - 1] = '0'; /* set the indicator to 0 to indicate that the input is invalid */
                        }
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
                    
                    /* the status of the invalid input is set to "Rejected" */
                    if (buffer[a - 1] == '0') {
                        status_arr[id] = malloc(strlen("Rejected") * sizeof(char));
                        strcpy(status_arr[id], "Rejected");
                    }
                    
                    id ++; /* increment the id */

                    write(fd[write_pipe][1], "OK", 2); /* for synchronization */
                    
                }
                /* runS3 */
                else if (strcmp(command, "runS3") == 0) {
                    /* CAN BE DELETED!!!!!!!!!! JUST FOR TESTING / DEMONSTRATION ============================== */
                    /* TO BE WRITTEN... */
                    for (a = 0; a < 14; a ++) {
                        for (b = 0; b < 4; b ++) {
                            if (strcmp(timetable[b][a], "") != 0) printf("Day 2019-4-%d Time %d:00 : %s\n", a + 8, b + 19, timetable[b][a]);
                        }
                    }
                    for (a = 0; a < id; a ++) {
                        if (status_arr[a] == NULL) {
                            printf("ID: %d Type: %s Status: NULL Progress: %d%% Event_name: %s Date: %d-%d-%d Time: %d:00 Duration: %d\n", a + 1, type_arr[a], progress_arr[a], event_name_arr[a], date_arr[a].year, date_arr[a].month, date_arr[a].day, time_arr[a], duration_arr[a]);
                        }
                        else {
                            printf("ID: %d Type: %s Status: %s Progress: %d%% Event_name: %s Date: %d-%d-%d Time: %d:00 Duration: %d\n", a + 1, type_arr[a], status_arr[a], progress_arr[a], event_name_arr[a], date_arr[a].year, date_arr[a].month, date_arr[a].day, time_arr[a], duration_arr[a]);
                        }
                    }
                    
                    write(fd[write_pipe][1], "OK", 2); /* for synchronization */
                    
                    /* CAN BE DELETED!!!!!!!!!! JUST FOR TESTING / DEMONSTRATION ============================== */
                }
            }
            
            /* close all the pipes end when the input module stops writing (program ends) */
            close(fd[read_pipe][0]);
            close(fd[write_pipe][1]);
            
            /* free all allocated space */
            for (a = 0; a < 1000; a ++) {
                if (type_arr[id] != NULL) free(type_arr[id]);
                if (status_arr[id] != NULL) free(status_arr[id]);
                if (event_name_arr[id] != NULL) free(event_name_arr[id]);
            }
        }
        
        exit(0);
    }
    /* Parent process: Input module */
    else {
        
        /* close unused pipe end */
        /* Parent uses pipes of even id to read */
        /* Parent uses pipes of odd id to write */
        for (a = 0; a < 2; a ++) {
            if (a % 2 == 0) close(fd[a][1]);
            else close(fd[a][0]);
        }
        
        char input[50] = ""; /* the string to be sent to the scheduler  (format: [user input] [indicator]) (indicator: '0': invalid / '1': valid) */
        char * return_string;
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
                    write(fd[1][1], input, strlen(input));
                }
            }
            else if (strcmp(command, "addRevision") == 0 || strcmp(command, "addActivity") == 0 || strcmp(command, "addAssignment") == 0 || strcmp(command, "addProject") == 0) {
                return_string = validateEvent(&id, command, stdin);
                strcpy(input, return_string);
                free(return_string); /* free the allocated space */
                write(fd[1][1], input, strlen(input)); /* send the input to scheduler */
                read(fd[0][0], buffer, 50); /* for synchronization */
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
                    write(fd[1][1], input, strlen(input)); /* send the input to scheduler */
                    read(fd[0][0], buffer, 50); /* for synchronization */
                }
                fclose(fp); /* close the file */
            }
            else if (strcmp(command, "runS3") == 0) {
                /* CAN BE DELETED!!!!!!!!!! JUST FOR TESTING / DEMONSTRATION ============================== */
                /* TO BE WRITTEN... */
                strcpy(input, command);
                write(fd[1][1], input, strlen(input)); /* send the input to scheduler */
                read(fd[0][0], buffer, 50); /* for synchronization */
                /* CAN BE DELETED!!!!!!!!!! JUST FOR TESTING / DEMONSTRATION ============================== */
            }
            else if (strcmp(command, "exitS3") == 0) {
                continue;
            }
            else {
                printf("invalid command\n");
            }
        }
        printf("Bye-bye!\n"); /* end the program properly */
        
        /* close all the pipes end when the input module stops writing (program ends) */
        for (a = 0; a < 2; a ++) {
            if (a % 2 == 0) close(fd[a][0]);
            else close(fd[a][1]);
        }
        
        while (wait(NULL) > 0); // wait for all child processes to finish
        exit(0);
   }
}
