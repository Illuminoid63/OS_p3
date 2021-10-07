# Simulated Stride Scheduler
This is a project that I did for an operating systems class, where I implemented a stride scheduling algorithm.

This program simulates how a stride scheduling algorithm would schedule jobs to run on a CPU using user-defined priorities to determine how often a program runs. I simulated this by just making input text files that would offer commands to my program and then the program handles the command and implements said algorithm.

Commands are as followed:

| opcode | argument 1 | argument 2 | meaning |
| ------ | ---------- | ----------  | ------- |
| newjob | NAME | PRIORITY | A new job with specified PRIORITY and NAME has arrived |
| finish | | | The currently running job has terminated - it is an error if the system is idle |
| interrupt | | | A timer interrupt has occurred - the currently running job's quantum is over |
| block | | | The currently running job has become blocked |
| unblock | NAME | | The named job becomes unblocked - it is an error if it was not blocked |
| runnable | | | Print information about the jobs in the runnable queue |
| running | | | Print information about the currently running job |
| blocked | | | Print information about the jobs on the blocked queue |
