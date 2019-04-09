#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#pragma warning (disable:4996)

	typedef struct date {
		int year;
		int month;
		int day;
	} date, Date;

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

			
	
			
int main(int argc, const char * argv[]){
	date edf_start_date;
	date edf_end_date;
	int edf_period_days;
	int edf_period_day_hrs;
	int edf_start_hr;
	int edf_end_hr;
	
	//added ok
	/* Event entry to be scheduled */
			typedef struct edf_entry {
				
				char* name;		/* Name of the activity */
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
				
				char* name;
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
		
//ok?
		int insert_entry(char* insrt_name, Date insrt_date, int insrt_time, int insrt_cost, int insrt_type){
				edf_entry_list* temp;
				temp = malloc(sizeof(edf_entry_list));
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
				printf("Entry name   Deadline/Date   Time   Cost   Priority\n");
				printf("==================================================\n");
				printf("%-13s%-16s%-7d%-7d%-8d\n", temp_ptr->entry.name, temp_date_str, temp_ptr->entry.edf_time,temp_ptr->entry.edf_cost,temp_ptr->entry.type_piority);
			}
			else{return;}
			
			
			while(temp_ptr->next_entry!=NULL){
				temp_ptr = temp_ptr->next_entry;
				date2str(temp_ptr->entry.edf_date, temp_date_str);
				printf("%-13s%-16s%-7d%-7d%-8d\n", temp_ptr->entry.name, temp_date_str, temp_ptr->entry.edf_time,temp_ptr->entry.edf_cost,temp_ptr->entry.type_piority);
			}
			
			/*
			if(temp_ptr->next_entry!=NULL){
				temp_ptr = temp_ptr->next_entry;
				date2str(temp_ptr->entry.edf_date, temp_date_str);
				printf("%-13s%-16s%-7d%-7d%-8d\n", temp_ptr->entry.name, temp_date_str, temp_ptr->entry.edf_time,temp_ptr->entry.edf_cost,temp_ptr->entry.type_piority);
				printf("%x\n", entry_head);
				printf("%x\n", temp_ptr);
			}
			if(temp_ptr->next_entry!=NULL){
				printf("%x\n", temp_ptr->next_entry);
			}
			*/
			
			/*
			printf("Head:\n");
			temp_ptr = entry_head;
			date2str(temp_ptr->entry.edf_date, temp_date_str);
			printf("%-13s%-16s%-7d%-7d%-8d\n", temp_ptr->entry.name, temp_date_str, temp_ptr->entry.edf_time,temp_ptr->entry.edf_cost,temp_ptr->entry.type_piority);
			printf("Tail:\n");
			temp_ptr = entry_tail;
			date2str(temp_ptr->entry.edf_date, temp_date_str);
			printf("%-13s%-16s%-7d%-7d%-8d\n", temp_ptr->entry.name, temp_date_str, temp_ptr->entry.edf_time,temp_ptr->entry.edf_cost,temp_ptr->entry.type_piority);
			*/
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
			
			/*
			printf("Head:\n");
			temp_ptr = timetable_head;
			date2str(temp_ptr->timeslot.edf_date, temp_date_str);
			printf("%-16s%-2d:00    %-17s\n", temp_date_str, temp_ptr->timeslot.edf_time,temp_ptr->timeslot.name);
			printf("Tail:\n");
			temp_ptr = timetable_tail;
			date2str(temp_ptr->timeslot.edf_date, temp_date_str);
			printf("%-16s%-2d:00    %-17s\n", temp_date_str, temp_ptr->timeslot.edf_time,temp_ptr->timeslot.name);
			*/
			
			printf("\n");
		}
		
		//added ok
		int add_time_slot(char* insrt_name, Date insrt_date, int insrt_time){
				
				//printf("running add time slot.\n");
				
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
					//edf_timetable_list* temp_ptr = timetable_head;
					//while(temp_ptr.next_slot!=NULL){
					//	temp_ptr = temp_ptr.next_slot;
					//}
					timetable_tail->next_slot = temp;
					timetable_tail = temp;
					return 1;
				}
			}
		
		//added ok
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
				
				/*
				//Unit testing
				char temp_str[20];
				char temp_str2[20];
				date2str(edf_start_date, temp_str);
				date2str(edf_end_date, temp_str2);
				printf("EDF Period added successfully:\n");
				printf("Period starts from %s to %s\n", temp_str, temp_str2);
				printf("Available hours from %d to %d\n", edf_start_hr, edf_end_hr);
				printf("Total days: %d\n", edf_period_days);
				printf("Total hours a day: %d\n", edf_period_day_hrs);
				*/
				
				return;
			}
		
//ok		
	void addProject(char* parameters[]){
				char *name = parameters[0];
				Date temp_date = str2date(parameters[1]);
				int temp_time = 24;
				int temp_cost = atoi(parameters[2]);
				int type = 1;
				insert_entry(name, temp_date, temp_time, temp_cost, type);
			}
//ok		
	void addAssignment(char* parameters[]){
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
						//printf("No more PA\n");
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
						//A R/A is finished
						if(RA_r_cost<=0){
							//Get next until it is not on the same slot
							do{
								getNextRA(&current_RA);
							}while(current_RA!=NULL && date_cmp(current_slot->timeslot.edf_date,current_RA->entry.edf_date)==0 && 
							current_RA->entry.edf_time<=current_slot->timeslot.edf_time);							
							if(current_RA!=NULL){
								RA_r_cost = current_RA->entry.edf_cost;
							}
						}
					}
					else{
						getNextRA(&current_RA);
						if(current_RA!=NULL){
							RA_r_cost = current_RA->entry.edf_cost;
							if(date_cmp(current_slot->timeslot.edf_date,current_RA->entry.edf_date)==0 && current_RA->entry.edf_time<=current_slot->timeslot.edf_time){
								time_check();							
							}
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
					while(found==0&&temp->next_entry!=NULL){
						//If entry is assignment or project
						if(temp->entry.type_piority<3){
							current_PA = temp;
							found=1;
						}
						temp = temp->next_entry;
					}
					
					found = 0;
					temp = entry_head;
					while(found==0&&temp->next_entry!=NULL){
						//If entry is revision / activity
						if(temp->entry.type_piority>2){
							current_RA = temp;
							found=1;
						}
						temp = temp->next_entry;
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
							//printf("Activity is today.\n");
							//Activity start on this slot or in progress
							if((current_slot->timeslot.edf_time==current_RA->entry.edf_time)||(RA_r_cost<current_RA->entry.edf_cost)){
								//printf("Activity in starts now or in progress.\n");
								// RA in progress
								if(RA_r_cost<(current_RA->entry.edf_cost)){
									//printf("Activity in progress.\n");
									assign_slot(current_slot, current_RA->entry.name);
									RA_r_cost-=1;
									//A R/A is finished
									if(RA_r_cost<=0){
										do{
											getNextRA(&current_RA);
										}while(current_RA!=NULL && date_cmp(current_slot->timeslot.edf_date,current_RA->entry.edf_date)==0 && 
										current_RA->entry.edf_time<=current_slot->timeslot.edf_time);
										
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
								assign_slot(current_slot, current_PA->entry.name);
								PA_r_cost-=1;
								//printf("Current PA cost left %d\n", PA_r_cost);
								//A P/A is finished
								if(PA_r_cost<=0){
									//printf("Current PA finished, finding next PA.\n");
									do{
										getNextPA(&current_PA);
									}while(current_PA!=NULL && date_cmp(current_slot->timeslot.edf_date,current_PA->entry.edf_date)>=1 && 
									current_slot->timeslot.edf_time==(edf_end_hr-1));
										
									
									if(current_PA!=NULL){
										PA_r_cost = current_PA->entry.edf_cost;
										//printf("Next PA is %s\n", current_PA->entry.name);
										//printf("Next PA cost is %d\n", PA_r_cost);
									}
									/*
									else{
										printf("No more PA\n");
									}
									*/								
								}
							}
						}
						//Not on same date
						else{
							assign_slot(current_slot, current_PA->entry.name);
							PA_r_cost-=1;
							//printf("Current PA cost left %d\n", PA_r_cost);
							//A P/A is finished
							if(PA_r_cost<=0){
								//printf("Current PA finished, finding next PA.\n");
								do{
									getNextPA(&current_PA);
								}while(current_PA!=NULL && date_cmp(current_slot->timeslot.edf_date,current_PA->entry.edf_date)>=1 && 
								current_slot->timeslot.edf_time==(edf_end_hr-1));
								
								if(current_PA!=NULL){
									PA_r_cost = current_PA->entry.edf_cost;
									//printf("Next PA is %s\n", current_PA->entry.name);
									//printf("Next PA cost is %d\n", PA_r_cost);
								}
								else{
									//printf("No more PA\n");
								}
							}
						}
					}
					else if(current_PA!=NULL&&current_RA==NULL){
						assign_slot(current_slot, current_PA->entry.name);
						PA_r_cost-=1;
						//A P/A is finished
						if(PA_r_cost<=0){
							//printf("Current PA finished, finding next PA.\n");
							do{
								getNextPA(&current_PA);
							}while(current_PA!=NULL && date_cmp(current_slot->timeslot.edf_date,current_PA->entry.edf_date)>=1 && 
							current_slot->timeslot.edf_time==(edf_end_hr-1));
							if(current_PA!=NULL){
								PA_r_cost = current_PA->entry.edf_cost;
								//printf("Next PA is %s\n", current_PA->entry.name);
								//printf("Next PA cost is %d\n", PA_r_cost);
							}
							
						}
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
									do{
										getNextRA(&current_RA);
									}while(current_RA!=NULL && date_cmp(current_slot->timeslot.edf_date,current_RA->entry.edf_date)==0 && 
									current_RA->entry.edf_time<=current_slot->timeslot.edf_time);
										
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
	
	void command_handler(char* input){
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
				
				
				if(atoi(parameters[i]) == 0){
					printf("Scheduler received invalid message from pipe! Nothing need to do.\n");
					return;
				}
				
				//printf("The input is valid\n");
				//Add period
				if(strcmp(command,"addPeiod")==0){
					//printf("Running add period.\n");
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
	
	
	void aa(){
		char y[] = "2019-01-08";
		char a[] = "2019-01-09";
		char x[] = "2019-02-08";
		
		Date c = str2date(a);
		Date d = str2date(y);
		Date e = str2date(x);
		char b[25];
		date2str(c,b);
		//c = date_tmr(c);
		
		printf("c is %d-%d-%d\n", c.year, c.month, c.day);
		printf("%s\n",b);
		
		//printf("%d\n",date_cmp(c,c));  // 0
		//printf("%d\n",date_cmp(c,e));  //-1
		//printf("%d\n",date_cmp(c,d));  // 1
		
		printf("%d\n",date_days(d,e)); //2
		printf("%d\n",date_days(e,d)); //2
	}
	
	
	
	char input_test[] = "addPeiod 2019-04-08 2019-04-21 19:00 23:00 1";
	command_handler(input_test);
	char input_test2[] = "addProject COMP2422P1 2019-04-20 26 1";
	command_handler(input_test2);
	char input_test3[] = "addAssignment COMP2432A1 2019-04-18 12 1";
	command_handler(input_test3);
	char input_test4[] = "addRevision COMP2000 2019-04-14 19:00 2 1";
	command_handler(input_test4);
	
	char input_test5[] = "addActivity Meeting 2019-04-18 20:00 2 1";
	command_handler(input_test5);
	
	print_entries();
	//print_slots();
	char input_test6[] = "runS3 EDF S3_report_fcfs_01.dat 1";
	command_handler(input_test6);
	
	print_slots();
	printf("Program end.\n");
	//aa();
	return 0;
}

