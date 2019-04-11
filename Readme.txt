**Readme.txt**
=============================================
Project title: Study Scheduling System (S3)
Group 20
Group Members:
Cheung Ka Ho 17047931D
Ho Kin Ling 17056079D
Ngan Ting Cheuk 17061434D
So Hiu Tung 17057557D
Yan Ho Wang 17057122D

This S3 system should be compiled and executed under the “Apollo” linux server of Computing department. 
The compiler we are using is "gcc".

First, open “cmd” and connect to “Apollo” using ssh client by typing “ssh <Your studentID>@csdoor.comp.polyu.edu.hk”.

After logging in by typing the password, type “Apollo” to connect to “Apollo” linux server.

Then, type “pico s3.c” to create a C source file.

Copy the source code of s3 system into s3.c and exit.

Compile s3.c by typing “gcc s3.c -o s3”.

Type "./s3" to run the program.

A welcome message "~~Welcome to s3~~" will be shown.

You can start typing instruction to the system.

**Instruction set**

1. addPeriod [start date] [end date] [start time] [end time]
Example: addPeriod 2019-04-08 2019-04-21 19:00 23:00
Usage: 
It is to specify the period (dates of start and end) and the time slots [start time and end time]
for scheduling events. Date and time format is year-month-day and hour (YYYY-MM-DD and hh:mm, 
simply take the hour and ignore the minute (i.e. mm=00) and use the 24-hour). Actually, 
this should be the first command for the application.

2. addAssignment [subject code with assignment number] [due date] [duration]
Example: addAssignment COMP2432A1 2019-04-18 12
Usage:
[addAssignment] is to add a subject assignment to the scheduler.
Follows by the due date and hours required to finish the assignment.
The format [YYYY-MM-DD] is that YYYY is year, MM is month and DD is day.
Then the [duration] is the number of hours that required to finish the assignment.
Since there are only 4 timeslots per day that a task could be allocated,
for a task whose duration is more than 4 hours it could be allocated to the timeslots of the next coming days.
However, for assignment, you may not finish it at one time.
For example, there is an assignment which needs 4 hours to complete and should be done before Day_6.
You may separate the assignment into 4 sections (require 4 individual timeslot).
In the other word, you divide that one request into four different requests.
Do the first section in Day_1, Timeslot_1, the second section in Day_1, Timeslot_4,
the third section in Day_3, Timeslot_2 and finally complete the fourth section in Day_5, Timeslot_3.
Moreover, that may cause some assignments which could not be fully completed,
i.e. less than 100 percent. If so, you should indicate the percentage of completion in the report. 

3. addProject [subject code with project number] [due date] [duration]
Example: addProject COMP2422P1 2019-04-20 26
Usage:
[addProject] is similar to [addAssignment]. It is to add a subject project to the scheduler.
Follows by the due date in the format [YYYY-MM-DD]. Then the duration is specified (hours).
If you are considering to use "priority" as one of the algorithms that to be implemented in your application,
[addProject] would have a higher priority than [addAssignment].

4. addRevision [subject code] [date and time] [duration]
Example: addRevision COMP2000 2019-04-14 19:00 2
Usage:
[addRevision] is to add a revision section to the scheduler. Revision date in the format [YYYY-MM-DD hh:mm].
Then the duration is specified (number of hours required).
Unlike the previous two, this task should be done in one off.
If the task cannot be done in one go, simply reject it.
For considering "priority", the order looks like the following.
[addProject] > [addAssignment] > [addRevision] > [addActivity].

5. addActivity [name of the event] [date and time of the event] [duration]
Example: addActivity Meeting 2019-04-18 20:00 2
Usage:
[addActivity] is similar to [addRevision]. Instead of the "subject code", here we use "name of the event".
Then also it follows by the date and time of the event in the format [YYYY-MM-DD hh:mm]. And then, the duration.

6. addBatch [filename]
Example: addBatch S3_tasks_01.dat
Usage: [addBatch] allows user to prepare a text file which contains multiple lines of different requests and import into the program.

7. runS3 [algorithm] [filename]
Example: runS3 FCFS S3_report_fcfs_01.dat
Usage: [runS3] is to generate a schedule with the algorithm specified (in the example "FCFS" means " first come first served").
 There are other algorithms that can be used:
- PR, priority
- SJF, shortest job first
- SRT, shortest remaining time
- RR X, round robin with X is the number of time slots (quantum) to be assigned in each round.
- etc
For the output file, for example S3_report_fcfs_01.dat,
it is just a text file that include the time table and the analysis report (Output Module and Analyzer Module).
In addition, the acceptance or rejection of a request should be recorded in a corresponding log file (stored in Input Module).

8. exitS3
Usage: [exitS3] is to terminate the program properly.

**Note**
The maximum period of this system is from 2019-04-08 19:00 to 2019-04-21 23:00.
One time slot is assumed as one hour, and inputs are simply to indicate hours.
The only two scheduling algorithm used is First-Come-First-Serve(FCFS) and Earliest Deadline First(EDF).
Assignment will still be scheduled accordingly even the [due date] has passed.

**DEMO**

~~WELCOME TO S3~~
Please enter:
> addPeriod 2019-04-08 2019-04-21 19:00 23:00
Please enter:
> addAssignment COMP2432A1 2019-04-18 12
Please enter:
> addProject COMP2422P1 2019-04-20 26
Please enter:
> addRevision COMP2000 2019-04-14 19:00 2
Please enter:
> addActivity Meeting 2019-04-18 20:00 2
Please enter:
> addBatch S3_tasks_00.dat
Please enter:
> runS3 FCFS S3_report_fcfs_01.dat
Please enter:
> runS3 EDF S3_report_edf_02.dat
Please enter:
> exitS3
Bye-bye!

The output files (.dat) and (.log) will be generated in the current directory.