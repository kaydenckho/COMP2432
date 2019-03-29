# COMP2432
COMP2432 - Scheduling system
====================================

Tentative requirement specifications:
\n
main() as parent process, fork() used for Input, scheduling and generating .dat file in output & analysis stage.\n
pipe() will be used for tranferring parameters in child processes. i.e. input -> scheduling -> generating .dat file.\n
Each module in C header(.h) file.\n

Input module:
====================================
Input: User inputs or file inputs(addBatch)\n
Output: time_t start period\n
        time_t end_period\n
        char[][] tasks, 2D array containing jobs to be scheduled,\n
        log files which records every user input, pls refer to project PDF file.\n
        
int addPeriod(..)\n
int addAssignment(..)\n
int addProject(..)\n
int addRevision(..)\n
int addActivity()\n
int addBatch(..)\n

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
int printReport(..)
