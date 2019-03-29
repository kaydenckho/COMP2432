# COMP2432
COMP2432 - Scheduling system
====================================

Tentative requirement specifications:
====================================
main() as parent process, fork() used for Input, scheduling and generating .dat file in output & analysis stage.
pipe() will be used for tranferring parameters in child processes. i.e. input -> scheduling -> generating .dat file.
Each module in C header(.h) file.

Input module:
====================================
Input: User inputs or file inputs(addBatch)
Output: time_t start period
        time_t end_period
        char[][] tasks, 2D array containing jobs to be scheduled,
        log files which records every user input, pls refer to project PDF file.
        
int addPeriod(..)
int addAssignment(..)
int addProject(..)
int addRevision(..)
int addActivity()
int addBatch(..)

Scheduling module:
====================================
Input: time_t start period
       time_t end_period
       char[][], tasks, 2D array containing jobs to be scheduled,
Output: char[][] schedule, 2D array containing schedule jobs 

  char* FCFS(..)
  char* SJF(..)
  char* RR(..)
  char* EDF(..)
  
#Number of algorithms to be done depends on your progress.

Output module & analyzer module
=====================================
Input: char[][] schedule, 2D array containing schedule jobs 
Output: .dat file containing the timetable and analysis report, format pls refer to the output formats specified in project PDF file.
int printTimeTable(..)
int GenReport(..)
..
