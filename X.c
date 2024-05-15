#1 string handling functions
concatenate_strings() {
    str1="Hello"
    str2="World"
    concatenated="$str1 $str2"
    echo "Concatenated string: $concatenated"
}

string_length() {
    str="Hello, World!"
    length=${#str}
    echo "Length of the string \"$str\" is $length"
}

substring_extraction() {
    str="Hello, World!"
    substring="${str:7:5}"
    echo "Substring of \"$str\" starting from index 7: $substring"
}

substring_replacement() {
    str="Hello, World!"
    new_str="${str/Hello/Hi}"
    echo "After Replacement: $new_str"
}

uppercase_conversion() {
    str="hello, world!"
    uppercase="${str^^}"
    echo "Uppercase string: $uppercase"
}

lowercase_conversion() {
    str="HELLO, WORLD!"
    lowercase="${str,,}"
    echo "Lowercase string: $lowercase"
}

echo "String Handling Function Demo"
echo "-----------------------------"
concatenate_strings
string_length
substring_extraction
substring_replacement
uppercase_conversion
lowercase_conversion

#2 arithmetic functions
add() {
    result=$(( $1 + $2 ))
    echo "Addition: $result"
}

subtract() {
    result=$(( $1 - $2 ))
    echo "Subtraction: $result"
}

multiply() {
    result=$(( $1 * $2))
    echo "Multiplication: $result"
}

divide() {
    if [ $2 -eq 0 ]; then
        echo "Cannot divide by zero"
    else
        result=$(awk "BEGIN {printf \"%.2f\", $1 / $2}")
        echo "Division: $result"
    fi
}

remainder() {
    result=$(( $1 % $2))
    echo "Remainder: $result"
}

echo "Arithmetic Function Demo"
echo "-----------------------"

read -p "Enter two numbers: " num1 num2

add $num1 $num2
subtract $num1 $num2
multiply $num1 $num2
divide $num1 $num2
remainder $num1 $num2

#3 ss palindrome, prime, fibonacci

#!/bin/bash
# Function to calculate factorial
factorial() {
 if [ $1 -eq 0 -o $1 -eq 1 ]; then
 echo 1
 else
 echo $(( $1 * $(factorial $(( $1 - 1 ))) ))
 fi
}
# Function to check if a number is even or odd
evenodd() {
 if [ $(( $1 % 2 )) -eq 0 ]; then
 echo "Even"
 else
 echo "Odd"
 fi
}
# Function to check if a number is prime
isprime() {
 for (( i=2; i<=$1/2; i++ )); do
 if [ $(( $1 % $i )) -eq 0 ]; then
 echo "Not Prime"
 return
 fi
 done
 echo "Prime"
}
# Function to print Fibonacci series
fibonacci() {
 a=0
 b=1
 echo -n "$a $b "
 for (( i=2; i<$1; i++ )); do
 c=$((a + b))
 echo -n "$c "
 a=$b
 b=$c
 done
 echo
}
# Function to print multiplication table
table() {
 for (( i=1; i<=10; i++ )); do
 echo "$1 x $i = $(( $1 * $i ))"
 done
}
#Function to check if a number is palindrome
is_palindrome() {
    num=$1
    reverse=0
    while [ $num -gt 0 ]; do
        remainder=$((num%10))
        reverse=$((reverse * 10 + remainder))
        num=$((num / 10))
    done
    if [ $1 -eq $reverse ]; then
        echo "Palindrome"
    else
        echo "Not Palindrome"
    fi
}
# Main script
read -p "Enter a number: " num
echo "Factorial: $(factorial $num)"
echo "Even/Odd: $(evenodd $num)"
echo "Prime or Not Prime: $(isprime $num)"
echo "Fibonacci Series up to $num terms: $(fibonacci $num)"
echo "Multiplication Table of $num:"
table $num
echo "Palindrome Check: $(is_palindrome $num)"


#4 producer cons and reader
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

char buffer[BUFFER_SIZE];
sem_t mutex, full, empty;

void *producer(void *arg);
void *consumer(void *arg);
void insert_item(char item);
char remove_item();

int main() {
   
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);

    
    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;
}

void *producer(void *arg) {
    for (int i = 0; i < 10; ++i) {
        char item = 'A' + i;
        sem_wait(&empty);  
        sem_wait(&mutex);  
        insert_item(item); 
        sem_post(&mutex);  
        sem_post(&full);   
        printf("Produced:%c\n",item);
        sleep(1);         
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    for (int i = 0; i < 10; ++i) {
        sem_wait(&full);   
        sem_wait(&mutex);
        char item = remove_item(); 
        sem_post(&mutex);  
        sem_post(&empty);  
        printf("Consumed: %c\n", item);
        sleep(1);   
    }
    pthread_exit(NULL);
}

void insert_item(char item) {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (buffer[i] == 0) {
            buffer[i] = item;
            return;
        }
    }
}

char remove_item() {
    char item = buffer[0];
    for (int i = 1; i < BUFFER_SIZE; ++i) {
        buffer[i - 1] = buffer[i];
    }
    buffer[BUFFER_SIZE - 1] = 0;
    return item;
}
-------------
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

sem_t wrt;
pthread_mutex_t mutex;
int cnt = 1;
int numreader = 0;

void *writer(void *wno)
{
    sem_wait(&wrt);
    cnt = cnt*2;
    printf("Writer %d modified cnt to %d\n",(*((int *)wno)),cnt);
    sem_post(&wrt);

}
void *reader(void *rno)
{ 
    pthread_mutex_lock(&mutex);
    numreader++;
    if(numreader == 1) {
        sem_wait(&wrt);
    }
    pthread_mutex_unlock(&mutex);
    printf("Reader %d: read cnt as %d\n",*((int *)rno),cnt);
    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) {
        sem_post(&wrt);
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{

    pthread_t read[10],write[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    int a[10] = {1,2,3,4,5,6,7,8,9,10};

    for(int i = 0; i < 10; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}


#5 Reader and Dining

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

sem_t wrt;
pthread_mutex_t mutex;
int cnt = 1;
int numreader = 0;

void *writer(void *wno)
{
    sem_wait(&wrt);
    cnt = cnt*2;
    printf("Writer %d modified cnt to %d\n",(*((int *)wno)),cnt);
    sem_post(&wrt);

}
void *reader(void *rno)
{ 
    pthread_mutex_lock(&mutex);
    numreader++;
    if(numreader == 1) {
        sem_wait(&wrt);
    }
    pthread_mutex_unlock(&mutex);
    printf("Reader %d: read cnt as %d\n",*((int *)rno),cnt);
    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) {
        sem_post(&wrt);
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{

    pthread_t read[10],write[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    int a[10] = {1,2,3,4,5,6,7,8,9,10};

    for(int i = 0; i < 10; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
------------------
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N
int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };
sem_t mutex;
sem_t S[N];
void test(int phnum)
{
    if (state[phnum] == HUNGRY
        && state[LEFT] != EATING
        && state[RIGHT] != EATING) {
        state[phnum] = EATING;
        sleep(2);
        printf("Philosopher %d takes fork %d and %d\n",
        phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is Eating\n", phnum + 1);
        sem_post(&S[phnum]);
    }
}
 
void take_fork(int phnum)
{
    sem_wait(&mutex);
    state[phnum] = HUNGRY;
    printf("Philosopher %d is Hungry\n", phnum + 1);
    test(phnum);
    sem_post(&mutex);
    sem_wait(&S[phnum]);
    sleep(1);
}
void put_fork(int phnum)
{
    sem_wait(&mutex);
    state[phnum] = THINKING;
    printf("Philosopher %d putting fork %d and %d down\n",
    phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}
void* philosopher(void* num)
{
    while (1) {
        int* i = num;
        sleep(1);
        take_fork(*i);
        sleep(0);
        put_fork(*i);
    }
}
int main()
{
    int i;
    pthread_t thread_id[N];
    sem_init(&mutex, 0, 1);
    for (i = 0; i < N; i++)
        sem_init(&S[i], 0, 0);
    for (i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL,
        philosopher, &phil[i]);
        printf("Philosopher %d is thinking\n", i + 1);
    }
    for (i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);
}

#6 FCFS and RR
#include <stdio.h>

#define MAX_PROCESSES 10

struct Process {
    int pid;
    int arrival;
    int burst;
    int completion;
    int waiting;
};

void fcfs(struct Process processes[], int n) {
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrival) {
            currentTime = processes[i].arrival;
        }
        processes[i].completion = currentTime + processes[i].burst;
        processes[i].waiting = currentTime - processes[i].arrival; 
        currentTime = processes[i].completion;
    }
}

int main() {
    int n;
    float avgTurnaroundTime = 0;
    float avgWaitingTime = 0;
    struct Process processes[MAX_PROCESSES];
    printf("==========/First Come First Serve Algorithm/===========\n");

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter arrival time and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i + 1);
        scanf("%d %d", &processes[i].arrival, &processes[i].burst);
        processes[i].pid = i + 1;
    }
    fcfs(processes, n);

    for (int i = 0; i < n; i++) {
        avgTurnaroundTime += processes[i].completion - processes[i].arrival;
        avgWaitingTime += processes[i].waiting;
    }
    avgTurnaroundTime /= n;
    avgWaitingTime /= n;

    printf("\nGantt Chart:\n");
    printf(" ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n|");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst - 1; j++) {
            printf(" ");
        }
        printf("P%d", processes[i].pid);
        for (int j = 0; j < processes[i].burst - 1; j++) {
            printf(" ");
        }
        printf("|");
    }
    printf("\n ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n");
        printf("0");
    for (int i = 0; i < n; i++) {
        printf("   %d\t\t", processes[i].completion);
    }
    
    printf("\nFCFS Scheduling:\n");
    printf("PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].arrival, 
               processes[i].burst, processes[i].completion, 
               processes[i].completion - processes[i].arrival, processes[i].waiting);
    }

    printf("\nAverage Turnaround Time: %.2f\n", avgTurnaroundTime);
    printf("Average Waiting Time: %.2f\n", avgWaitingTime);

    return 0;
}
-----------------------
#include <stdio.h>

void findWaitingTime(int processes[], int n, int bt[], int wt[], int quantum) {
    int rem_bt[n];
    for (int i = 0; i < n; i++)
        rem_bt[i] = bt[i];

    int t = 0;

    while (1) {
        int done = 1;

        for (int i = 0; i < n; i++) {
            if (rem_bt[i] > 0) {
                done = 0;

                if (rem_bt[i] > quantum) {
                    t += quantum;
                    rem_bt[i] -= quantum;
                } else {
                    t += rem_bt[i];
                    wt[i] = t - bt[i];
                    rem_bt[i] = 0;
                }
            }
        }
        if (done == 1)
            break;
    }
}

void findTurnAroundTime(int processes[], int n, int bt[], int wt[], int tat[]) {
    for (int i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];
}

void findavgTime(int processes[], int n, int bt[], int quantum) {
    int wt[n], tat[n];
    int total_wt = 0, total_tat = 0;

    findWaitingTime(processes, n, bt, wt, quantum);

    findTurnAroundTime(processes, n, bt, wt, tat);

    printf("PN\tB\tWT\tTAT\n");

    for (int i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        printf("%d\t%d\t%d\t%d\n", processes[i], bt[i], wt[i], tat[i]);
    }

    printf("Average waiting time = %.2f\n", (float)total_wt / (float)n);
    printf("Average turn around time = %.2f\n", (float)total_tat / (float)n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int processes[n];
    printf("Enter process IDs: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &processes[i]);

    int burst_time[n];
    printf("Enter burst times for each process: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &burst_time[i]);

    int quantum;
    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    findavgTime(processes, n, burst_time, quantum);
    return 0;
}

#7 SJF and SRTF

#include <stdio.h>

struct Process {
    int p_id;
    int bt;
    int at; // Arrival time
    int wt; 
    int tat; 
    int completed;
};

int findShortestJob(struct Process processes[], int n, int currentTime) {
    int shortestJobIndex = -1;
    int shortestBurst = 9999;
    
    for (int i = 0; i < n; ++i) {
        if (!processes[i].completed && processes[i].bt < shortestBurst && processes[i].at <= currentTime && processes[i].bt > 0) {
            shortestBurst = processes[i].bt;
            shortestJobIndex = i;
        }
    }
    
    return shortestJobIndex;
}

void calculateTimes(struct Process processes[], int n) {
    int currentTime = 0;
    int totalBurst = 0;
    
    // Calculate total burst time
    for (int i = 0; i < n; ++i) {
        totalBurst += processes[i].bt;
    }
    
    printf("Gantt Chart:\n");
    printf("|");
    
    while (currentTime < totalBurst) {
        int shortestJobIndex = findShortestJob(processes, n, currentTime);
        if (shortestJobIndex == -1) {
            currentTime++;
            continue;
        }
        
        printf(" P%d(%d) |", processes[shortestJobIndex].p_id, processes[shortestJobIndex].bt);
        
        processes[shortestJobIndex].bt--;
        
        for (int i = 0; i < n; ++i) {
            if (i != shortestJobIndex && !processes[i].completed && processes[i].at <= currentTime) {
                processes[i].wt++;
            }
        }
        
        currentTime++;
        
        if (processes[shortestJobIndex].bt == 0) {
            processes[shortestJobIndex].completed = 1;
            processes[shortestJobIndex].tat = currentTime;
        }
    }
    printf("\n");
}

void calculateAverages(struct Process processes[], int n, float *avg_waiting_time, float *avg_turnaround_time) {
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    
    for (int i = 0; i < n; ++i) {
        total_waiting_time += processes[i].wt;
        total_turnaround_time += processes[i].tat;
    }
    
    *avg_waiting_time = (float)total_waiting_time / n;
    *avg_turnaround_time = (float)total_turnaround_time / n;
}

void displayProcesses(struct Process processes[], int n) {
    printf("Process ID\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; ++i) {
        printf("%d\t\t%d\t\t%d\n", processes[i].p_id, processes[i].wt, processes[i].tat);
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    struct Process processes[n];
    
    for (int i = 0; i < n; ++i) {
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].bt);
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].at);
        processes[i].p_id = i + 1;
        processes[i].wt = 0;
        processes[i].tat = 0;
        processes[i].completed = 0;
    }
    
    calculateTimes(processes, n);
    
    float avg_waiting_time, avg_turnaround_time;
    calculateAverages(processes, n, &avg_waiting_time, &avg_turnaround_time);
    
    displayProcesses(processes, n);
    
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    
    return 0;
}
---------

#include <stdio.h>
#include <limits.h>

struct Process {
    int pid;
    int bt;
    int art;
};

void findWaitingTime(struct Process proc[], int n, int wt[]) {
    int rt[n];
    for (int i = 0; i < n; i++)
        rt[i] = proc[i].bt;

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    int check = 0;

    while (complete != n) {
        for (int j = 0; j < n; j++) {
            if (proc[j].art <= t && rt[j] < minm && rt[j] > 0) {
                minm = rt[j];
                shortest = j;
                check = 1;
            }
        }

        if (check == 0) {
            t++;
            continue;
        }

        rt[shortest]--;

        minm = rt[shortest];
        if (minm == 0)
            minm = INT_MAX;

        if (rt[shortest] == 0) {
            complete++;
            check = 0;

            finish_time = t + 1;

            wt[shortest] = finish_time - proc[shortest].bt - proc[shortest].art;

            if (wt[shortest] < 0)
                wt[shortest] = 0;
        }
        t++;
    }
}

void findTurnAroundTime(struct Process proc[], int n, int wt[], int tat[]) {
    for (int i = 0; i < n; i++)
        tat[i] = proc[i].bt + wt[i];
}

void findavgTime(struct Process proc[], int n) {
    int wt[n], tat[n];
    int total_wt = 0, total_tat = 0;

    findWaitingTime(proc, n, wt);
    findTurnAroundTime(proc, n, wt, tat);

    printf("Processes\tBurst time\tWaiting time\tTurn around time\n");

    for (int i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        printf(" %d\t\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].bt, wt[i], tat[i]);
    }

    printf("Average waiting time = %.2f\n", (float)total_wt / (float)n);
    printf("Average turn around time = %.2f\n", (float)total_tat / (float)n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process proc[n];

    for (int i = 0; i < n; i++) {
        printf("Enter details for process %d (PID, Burst Time, Arrival Time): ", i + 1);
        scanf("%d %d %d", &proc[i].pid, &proc[i].bt, &proc[i].art);
    }

    findavgTime(proc, n);
    return 0;
}


#8 priority p and np
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Process {
	int processID;
	int arrivalTime;
	int priority;
	int burstTime;
	int remainingTime;
	int startTime;
	int completionTime;
};

// Function to find process with highest priority among the arrived processes at current time
int findHighestPriorityProcess(struct Process processes[], int n, int currentTime) {
	int highestPriority = INT_MAX;
	int highestPriorityIndex = -1;

	for (int i = 0; i < n; i++) {
		if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
			if (processes[i].priority < highestPriority) {
				highestPriority = processes[i].priority;
				highestPriorityIndex = i;
			}
		}
	}

	return highestPriorityIndex;
}

// Function to calculate waiting time, turnaround time, and average time
void calculateTimes(struct Process processes[], int n, int waitingTime[], int turnaroundTime[], float *averageWaitingTime, float *averageTurnaroundTime) {
	int totalWaitingTime = 0, totalTurnaroundTime = 0;

	for (int i = 0; i < n; i++) {
		turnaroundTime[i] = processes[i].completionTime - processes[i].arrivalTime;
		waitingTime[i] = turnaroundTime[i] - processes[i].burstTime;

		totalWaitingTime += waitingTime[i];
		totalTurnaroundTime += turnaroundTime[i];
	}

	*averageWaitingTime = (float)totalWaitingTime / n;
	*averageTurnaroundTime = (float)totalTurnaroundTime / n;
}

// Priority Preemptive Scheduling Algorithm
void priorityPreemptive(struct Process processes[], int n) {
	int currentTime = 0;
	int remainingProcesses = n;
	int waitingTime[n], turnaroundTime[n];
	float averageWaitingTime, averageTurnaroundTime;

	while (remainingProcesses > 0) {
		int highestPriorityIndex = findHighestPriorityProcess(processes, n, currentTime);

		if (highestPriorityIndex == -1) {
			currentTime++;
			continue;
		}

		processes[highestPriorityIndex].remainingTime--;

		if (processes[highestPriorityIndex].startTime == -1)
			processes[highestPriorityIndex].startTime = currentTime;

		if (processes[highestPriorityIndex].remainingTime == 0) {
			processes[highestPriorityIndex].completionTime = currentTime + 1;
			remainingProcesses--;
		}

		currentTime++;
	}

	calculateTimes(processes, n, waitingTime, turnaroundTime, &averageWaitingTime, &averageTurnaroundTime);

	printf("Process ID\tArrival Time\tPriority\tBurst Time\tStart Time\tCompletion Time\tWaiting Time\tTurnaround Time\n");
	for (int i = 0; i < n; i++) {
		printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
			processes[i].processID, processes[i].arrivalTime, processes[i].priority, processes[i].burstTime,
			processes[i].startTime, processes[i].completionTime, waitingTime[i], turnaroundTime[i]);
	}

	printf("Average Waiting Time: %.2f\n", averageWaitingTime);
	printf("Average Turnaround Time: %.2f\n", averageTurnaroundTime);
}

int main() {
	int n = 5;
	struct Process processes[] = {
		{1, 0, 3, 5, 5, -1, -1},
		{2, 1, 2, 4, 4, -1, -1},
		{3, 2, 1, 3, 3, -1, -1},
		{4, 3, 4, 2, 2, -1, -1},
		{5, 4, 5, 1, 1, -1, -1}
	};

	priorityPreemptive(processes, n);

	return 0;
}
------------------
#include <stdio.h>
 
//Function to swap two variables
void swap(int *a,int *b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}
int main()
{
    int n;
    printf("Enter Number of Processes: ");
    scanf("%d",&n);
 
    // b is array for burst time, p for priority and index for process id
    int b[n],p[n],index[n];
    for(int i=0;i<n;i++)
    {
        printf("Enter Burst Time and Priority Value for Process %d: ",i+1);
        scanf("%d %d",&b[i],&p[i]);
        index[i]=i+1;
    }
    for(int i=0;i<n;i++)
    {
        int a=p[i],m=i;
 
        //Finding out highest priority element and placing it at its desired position
        for(int j=i;j<n;j++)
        {
            if(p[j] > a)
            {
                a=p[j];
                m=j;
            }
        }
 
        //Swapping processes
        swap(&p[i], &p[m]);
        swap(&b[i], &b[m]);
        swap(&index[i],&index[m]);
    }
 
    // T stores the starting time of process
    int t=0;
 
    //Printing scheduled process
    printf("Order of process Execution is\n");
    for(int i=0;i<n;i++)
    {
        printf("P%d is executed from %d to %d\n",index[i],t,t+b[i]);
        t+=b[i];
    }
    printf("\n");
    printf("Process Id     Burst Time   Wait Time    TurnAround Time\n");
    int wait_time=0;
    for(int i=0;i<n;i++)
    {
        printf("P%d          %d          %d          %d\n",index[i],b[i],wait_time,wait_time + b[i]);
        wait_time += b[i];
    }
    return 0;
}

#9 priority p and FCFS

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Process {
	int processID;
	int arrivalTime;
	int priority;
	int burstTime;
	int remainingTime;
	int startTime;
	int completionTime;
};

// Function to find process with highest priority among the arrived processes at current time
int findHighestPriorityProcess(struct Process processes[], int n, int currentTime) {
	int highestPriority = INT_MAX;
	int highestPriorityIndex = -1;

	for (int i = 0; i < n; i++) {
		if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
			if (processes[i].priority < highestPriority) {
				highestPriority = processes[i].priority;
				highestPriorityIndex = i;
			}
		}
	}

	return highestPriorityIndex;
}

// Function to calculate waiting time, turnaround time, and average time
void calculateTimes(struct Process processes[], int n, int waitingTime[], int turnaroundTime[], float *averageWaitingTime, float *averageTurnaroundTime) {
	int totalWaitingTime = 0, totalTurnaroundTime = 0;

	for (int i = 0; i < n; i++) {
		turnaroundTime[i] = processes[i].completionTime - processes[i].arrivalTime;
		waitingTime[i] = turnaroundTime[i] - processes[i].burstTime;

		totalWaitingTime += waitingTime[i];
		totalTurnaroundTime += turnaroundTime[i];
	}

	*averageWaitingTime = (float)totalWaitingTime / n;
	*averageTurnaroundTime = (float)totalTurnaroundTime / n;
}

// Priority Preemptive Scheduling Algorithm
void priorityPreemptive(struct Process processes[], int n) {
	int currentTime = 0;
	int remainingProcesses = n;
	int waitingTime[n], turnaroundTime[n];
	float averageWaitingTime, averageTurnaroundTime;

	while (remainingProcesses > 0) {
		int highestPriorityIndex = findHighestPriorityProcess(processes, n, currentTime);

		if (highestPriorityIndex == -1) {
			currentTime++;
			continue;
		}

		processes[highestPriorityIndex].remainingTime--;

		if (processes[highestPriorityIndex].startTime == -1)
			processes[highestPriorityIndex].startTime = currentTime;

		if (processes[highestPriorityIndex].remainingTime == 0) {
			processes[highestPriorityIndex].completionTime = currentTime + 1;
			remainingProcesses--;
		}

		currentTime++;
	}

	calculateTimes(processes, n, waitingTime, turnaroundTime, &averageWaitingTime, &averageTurnaroundTime);

	printf("Process ID\tArrival Time\tPriority\tBurst Time\tStart Time\tCompletion Time\tWaiting Time\tTurnaround Time\n");
	for (int i = 0; i < n; i++) {
		printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
			processes[i].processID, processes[i].arrivalTime, processes[i].priority, processes[i].burstTime,
			processes[i].startTime, processes[i].completionTime, waitingTime[i], turnaroundTime[i]);
	}

	printf("Average Waiting Time: %.2f\n", averageWaitingTime);
	printf("Average Turnaround Time: %.2f\n", averageTurnaroundTime);
}

int main() {
	int n = 5;
	struct Process processes[] = {
		{1, 0, 3, 5, 5, -1, -1},
		{2, 1, 2, 4, 4, -1, -1},
		{3, 2, 1, 3, 3, -1, -1},
		{4, 3, 4, 2, 2, -1, -1},
		{5, 4, 5, 1, 1, -1, -1}
	};

	priorityPreemptive(processes, n);

	return 0;
}
--------------------
#include <stdio.h>

#define MAX_PROCESSES 10

struct Process {
    int pid;
    int arrival;
    int burst;
    int completion;
    int waiting;
};

void fcfs(struct Process processes[], int n) {
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrival) {
            currentTime = processes[i].arrival;
        }
        processes[i].completion = currentTime + processes[i].burst;
        processes[i].waiting = currentTime - processes[i].arrival; 
        currentTime = processes[i].completion;
    }
}

int main() {
    int n;
    float avgTurnaroundTime = 0;
    float avgWaitingTime = 0;
    struct Process processes[MAX_PROCESSES];
    printf("==========/First Come First Serve Algorithm/===========\n");

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter arrival time and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i + 1);
        scanf("%d %d", &processes[i].arrival, &processes[i].burst);
        processes[i].pid = i + 1;
    }
    fcfs(processes, n);

    for (int i = 0; i < n; i++) {
        avgTurnaroundTime += processes[i].completion - processes[i].arrival;
        avgWaitingTime += processes[i].waiting;
    }
    avgTurnaroundTime /= n;
    avgWaitingTime /= n;

    printf("\nGantt Chart:\n");
    printf(" ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n|");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst - 1; j++) {
            printf(" ");
        }
        printf("P%d", processes[i].pid);
        for (int j = 0; j < processes[i].burst - 1; j++) {
            printf(" ");
        }
        printf("|");
    }
    printf("\n ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n");
        printf("0");
    for (int i = 0; i < n; i++) {
        printf("   %d\t\t", processes[i].completion);
    }
    
    printf("\nFCFS Scheduling:\n");
    printf("PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].arrival, 
               processes[i].burst, processes[i].completion, 
               processes[i].completion - processes[i].arrival, processes[i].waiting);
    }

    printf("\nAverage Turnaround Time: %.2f\n", avgTurnaroundTime);
    printf("Average Waiting Time: %.2f\n", avgWaitingTime);

    return 0;
}


#10 priority np and SJF

#include <stdio.h>
 
//Function to swap two variables
void swap(int *a,int *b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}
int main()
{
    int n;
    printf("Enter Number of Processes: ");
    scanf("%d",&n);
 
    // b is array for burst time, p for priority and index for process id
    int b[n],p[n],index[n];
    for(int i=0;i<n;i++)
    {
        printf("Enter Burst Time and Priority Value for Process %d: ",i+1);
        scanf("%d %d",&b[i],&p[i]);
        index[i]=i+1;
    }
    for(int i=0;i<n;i++)
    {
        int a=p[i],m=i;
 
        //Finding out highest priority element and placing it at its desired position
        for(int j=i;j<n;j++)
        {
            if(p[j] > a)
            {
                a=p[j];
                m=j;
            }
        }
 
        //Swapping processes
        swap(&p[i], &p[m]);
        swap(&b[i], &b[m]);
        swap(&index[i],&index[m]);
    }
 
    // T stores the starting time of process
    int t=0;
 
    //Printing scheduled process
    printf("Order of process Execution is\n");
    for(int i=0;i<n;i++)
    {
        printf("P%d is executed from %d to %d\n",index[i],t,t+b[i]);
        t+=b[i];
    }
    printf("\n");
    printf("Process Id     Burst Time   Wait Time    TurnAround Time\n");
    int wait_time=0;
    for(int i=0;i<n;i++)
    {
        printf("P%d          %d          %d          %d\n",index[i],b[i],wait_time,wait_time + b[i]);
        wait_time += b[i];
    }
    return 0;
}
--------------------
#include <stdio.h>

struct Process {
    int p_id;
    int bt;
    int at; // Arrival time
    int wt; 
    int tat; 
    int completed;
};

int findShortestJob(struct Process processes[], int n, int currentTime) {
    int shortestJobIndex = -1;
    int shortestBurst = 9999;
    
    for (int i = 0; i < n; ++i) {
        if (!processes[i].completed && processes[i].bt < shortestBurst && processes[i].at <= currentTime && processes[i].bt > 0) {
            shortestBurst = processes[i].bt;
            shortestJobIndex = i;
        }
    }
    
    return shortestJobIndex;
}

void calculateTimes(struct Process processes[], int n) {
    int currentTime = 0;
    int totalBurst = 0;
    
    // Calculate total burst time
    for (int i = 0; i < n; ++i) {
        totalBurst += processes[i].bt;
    }
    
    printf("Gantt Chart:\n");
    printf("|");
    
    while (currentTime < totalBurst) {
        int shortestJobIndex = findShortestJob(processes, n, currentTime);
        if (shortestJobIndex == -1) {
            currentTime++;
            continue;
        }
        
        printf(" P%d(%d) |", processes[shortestJobIndex].p_id, processes[shortestJobIndex].bt);
        
        processes[shortestJobIndex].bt--;
        
        for (int i = 0; i < n; ++i) {
            if (i != shortestJobIndex && !processes[i].completed && processes[i].at <= currentTime) {
                processes[i].wt++;
            }
        }
        
        currentTime++;
        
        if (processes[shortestJobIndex].bt == 0) {
            processes[shortestJobIndex].completed = 1;
            processes[shortestJobIndex].tat = currentTime;
        }
    }
    printf("\n");
}

void calculateAverages(struct Process processes[], int n, float *avg_waiting_time, float *avg_turnaround_time) {
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    
    for (int i = 0; i < n; ++i) {
        total_waiting_time += processes[i].wt;
        total_turnaround_time += processes[i].tat;
    }
    
    *avg_waiting_time = (float)total_waiting_time / n;
    *avg_turnaround_time = (float)total_turnaround_time / n;
}

void displayProcesses(struct Process processes[], int n) {
    printf("Process ID\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; ++i) {
        printf("%d\t\t%d\t\t%d\n", processes[i].p_id, processes[i].wt, processes[i].tat);
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    struct Process processes[n];
    
    for (int i = 0; i < n; ++i) {
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].bt);
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].at);
        processes[i].p_id = i + 1;
        processes[i].wt = 0;
        processes[i].tat = 0;
        processes[i].completed = 0;
    }
    
    calculateTimes(processes, n);
    
    float avg_waiting_time, avg_turnaround_time;
    calculateAverages(processes, n, &avg_waiting_time, &avg_turnaround_time);
    
    displayProcesses(processes, n);
    
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    
    return 0;
}


#11 priority np and RR
#include <stdio.h>
 
//Function to swap two variables
void swap(int *a,int *b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}
int main()
{
    int n;
    printf("Enter Number of Processes: ");
    scanf("%d",&n);
 
    // b is array for burst time, p for priority and index for process id
    int b[n],p[n],index[n];
    for(int i=0;i<n;i++)
    {
        printf("Enter Burst Time and Priority Value for Process %d: ",i+1);
        scanf("%d %d",&b[i],&p[i]);
        index[i]=i+1;
    }
    for(int i=0;i<n;i++)
    {
        int a=p[i],m=i;
 
        //Finding out highest priority element and placing it at its desired position
        for(int j=i;j<n;j++)
        {
            if(p[j] > a)
            {
                a=p[j];
                m=j;
            }
        }
 
        //Swapping processes
        swap(&p[i], &p[m]);
        swap(&b[i], &b[m]);
        swap(&index[i],&index[m]);
    }
 
    // T stores the starting time of process
    int t=0;
 
    //Printing scheduled process
    printf("Order of process Execution is\n");
    for(int i=0;i<n;i++)
    {
        printf("P%d is executed from %d to %d\n",index[i],t,t+b[i]);
        t+=b[i];
    }
    printf("\n");
    printf("Process Id     Burst Time   Wait Time    TurnAround Time\n");
    int wait_time=0;
    for(int i=0;i<n;i++)
    {
        printf("P%d          %d          %d          %d\n",index[i],b[i],wait_time,wait_time + b[i]);
        wait_time += b[i];
    }
    return 0;
}
-----------
#include <stdio.h>

void findWaitingTime(int processes[], int n, int bt[], int wt[], int quantum) {
    int rem_bt[n];
    for (int i = 0; i < n; i++)
        rem_bt[i] = bt[i];

    int t = 0;

    while (1) {
        int done = 1;

        for (int i = 0; i < n; i++) {
            if (rem_bt[i] > 0) {
                done = 0;

                if (rem_bt[i] > quantum) {
                    t += quantum;
                    rem_bt[i] -= quantum;
                } else {
                    t += rem_bt[i];
                    wt[i] = t - bt[i];
                    rem_bt[i] = 0;
                }
            }
        }
        if (done == 1)
            break;
    }
}

void findTurnAroundTime(int processes[], int n, int bt[], int wt[], int tat[]) {
    for (int i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];
}

void findavgTime(int processes[], int n, int bt[], int quantum) {
    int wt[n], tat[n];
    int total_wt = 0, total_tat = 0;

    findWaitingTime(processes, n, bt, wt, quantum);

    findTurnAroundTime(processes, n, bt, wt, tat);

    printf("PN\tB\tWT\tTAT\n");

    for (int i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        printf("%d\t%d\t%d\t%d\n", processes[i], bt[i], wt[i], tat[i]);
    }

    printf("Average waiting time = %.2f\n", (float)total_wt / (float)n);
    printf("Average turn around time = %.2f\n", (float)total_tat / (float)n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int processes[n];
    printf("Enter process IDs: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &processes[i]);

    int burst_time[n];
    printf("Enter burst times for each process: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &burst_time[i]);

    int quantum;
    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    findavgTime(processes, n, burst_time, quantum);
    return 0;
}



#12 SRTF and FCFS
#include <stdio.h>
#include <limits.h>

struct Process {
    int pid;
    int bt;
    int art;
};

void findWaitingTime(struct Process proc[], int n, int wt[]) {
    int rt[n];
    for (int i = 0; i < n; i++)
        rt[i] = proc[i].bt;

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    int check = 0;

    while (complete != n) {
        for (int j = 0; j < n; j++) {
            if (proc[j].art <= t && rt[j] < minm && rt[j] > 0) {
                minm = rt[j];
                shortest = j;
                check = 1;
            }
        }

        if (check == 0) {
            t++;
            continue;
        }

        rt[shortest]--;

        minm = rt[shortest];
        if (minm == 0)
            minm = INT_MAX;

        if (rt[shortest] == 0) {
            complete++;
            check = 0;

            finish_time = t + 1;

            wt[shortest] = finish_time - proc[shortest].bt - proc[shortest].art;

            if (wt[shortest] < 0)
                wt[shortest] = 0;
        }
        t++;
    }
}

void findTurnAroundTime(struct Process proc[], int n, int wt[], int tat[]) {
    for (int i = 0; i < n; i++)
        tat[i] = proc[i].bt + wt[i];
}

void findavgTime(struct Process proc[], int n) {
    int wt[n], tat[n];
    int total_wt = 0, total_tat = 0;

    findWaitingTime(proc, n, wt);
    findTurnAroundTime(proc, n, wt, tat);

    printf("Processes\tBurst time\tWaiting time\tTurn around time\n");

    for (int i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        printf(" %d\t\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].bt, wt[i], tat[i]);
    }

    printf("Average waiting time = %.2f\n", (float)total_wt / (float)n);
    printf("Average turn around time = %.2f\n", (float)total_tat / (float)n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process proc[n];

    for (int i = 0; i < n; i++) {
        printf("Enter details for process %d (PID, Burst Time, Arrival Time): ", i + 1);
        scanf("%d %d %d", &proc[i].pid, &proc[i].bt, &proc[i].art);
    }

    findavgTime(proc, n);
    return 0;
}
------------------
#include <stdio.h>

#define MAX_PROCESSES 10

struct Process {
    int pid;
    int arrival;
    int burst;
    int completion;
    int waiting;
};

void fcfs(struct Process processes[], int n) {
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrival) {
            currentTime = processes[i].arrival;
        }
        processes[i].completion = currentTime + processes[i].burst;
        processes[i].waiting = currentTime - processes[i].arrival; 
        currentTime = processes[i].completion;
    }
}

int main() {
    int n;
    float avgTurnaroundTime = 0;
    float avgWaitingTime = 0;
    struct Process processes[MAX_PROCESSES];
    printf("==========/First Come First Serve Algorithm/===========\n");

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter arrival time and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i + 1);
        scanf("%d %d", &processes[i].arrival, &processes[i].burst);
        processes[i].pid = i + 1;
    }
    fcfs(processes, n);

    for (int i = 0; i < n; i++) {
        avgTurnaroundTime += processes[i].completion - processes[i].arrival;
        avgWaitingTime += processes[i].waiting;
    }
    avgTurnaroundTime /= n;
    avgWaitingTime /= n;

    printf("\nGantt Chart:\n");
    printf(" ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n|");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst - 1; j++) {
            printf(" ");
        }
        printf("P%d", processes[i].pid);
        for (int j = 0; j < processes[i].burst - 1; j++) {
            printf(" ");
        }
        printf("|");
    }
    printf("\n ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n");
        printf("0");
    for (int i = 0; i < n; i++) {
        printf("   %d\t\t", processes[i].completion);
    }
    
    printf("\nFCFS Scheduling:\n");
    printf("PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].arrival, 
               processes[i].burst, processes[i].completion, 
               processes[i].completion - processes[i].arrival, processes[i].waiting);
    }

    printf("\nAverage Turnaround Time: %.2f\n", avgTurnaroundTime);
    printf("Average Waiting Time: %.2f\n", avgWaitingTime);

    return 0;
}


#13 Banker Algo

#include <stdio.h>
int main()
{

	int n, m, i, j, k;
	n = 5;
	m = 3;
int alloc[5][3] = { { 1, 0, 0 },
                    { 2, 1, 0 },
                    { 1, 1, 1 },
                    { 1, 1, 0 },
                    { 0, 0, 2 } };

int max[5][3] = { {  5, 1, 1 },
                { 2, 2, 0 },
                { 1, 2, 1 },
                { 1, 1, 1 },
                { 0, 0, 2 } };

int avail[3] = { 3, 1, 1 };


	int f[n], ans[n], ind = 0;
	for (k = 0; k < n; k++) {
		f[k] = 0;
	}
	int need[n][m];
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++)
			need[i][j] = max[i][j] - alloc[i][j];
	}
        printf("Allocation Matrix \n");
        for(i=0;i<n;i++){
        for(j=0; j<m; j++){
        printf(" %d",alloc[i][j]);
        }
        printf("\n");
    }
        printf("Max Matrix \n");
        for(i=0;i<n;i++){
        for(j=0; j<m; j++){
        printf(" %d",max[i][j]);
        }
        printf("\n");
    }
    printf("Need Matrix \n");
        for(i=0;i<n;i++){
        for(j=0; j<m; j++){
        printf(" %d",need[i][j]);
        }
        printf("\n");
    }
	int y = 0;
	for (k = 0; k < 5; k++) {
		for (i = 0; i < n; i++) {
			if (f[i] == 0) {

				int flag = 0;
				for (j = 0; j < m; j++) {
					if (need[i][j] > avail[j]){
						flag = 1;
						break;
					}
				}

				if (flag == 0) {
					ans[ind++] = i;
					for (y = 0; y < m; y++)
						avail[y] += alloc[i][y];
					f[i] = 1;
				}
			}
		}
	}
    int flag = 1;
	for(int i=0;i<n;i++)
	{
	if(f[i]==0)
	{
		flag=0;
		printf("SYSTEM NOT SAFE");
		break;
	}
	}
	if(flag==1)
	{
	printf("SYSTEM SAFE\n");
	for (i = 0; i < n - 1; i++)
		printf(" P%d ->", ans[i]);
	printf(" P%d", ans[n - 1]);
	}
	return (0);
}


#14 Deadlock Detection

#include <stdio.h>
#include <stdlib.h>
static int mark[20];
int i, j, np, nr;

int main()
{
    int **alloc, **request, *avail, *r, *f;

    printf("\nEnter the no of the process: ");
    scanf("%d", &np);

    printf("\nEnter the no of resources: ");
    scanf("%d", &nr);

    alloc = (int **)malloc(np * sizeof(int *));
    request = (int **)malloc(np * sizeof(int *));
    avail = (int *)malloc(nr * sizeof(int));
    r = (int *)malloc(nr * sizeof(int));
    f = (int *)malloc(np * sizeof(int));

    for(i = 0;i < np;i ++)
        f[i] = 0;

    for (i = 0; i < nr; i++)
    {
        printf("\nTotal Amount of the Resource R % d: ", i + 1);
        scanf("%d", &r[i]);
    }

    printf("\nEnter the request matrix:");
    for (i = 0; i < np; i++)
    {
        request[i] = (int *)malloc(nr * sizeof(int));
        for (j = 0; j < nr; j++)
        {
            scanf("%d", &request[i][j]);
        }
    }

    printf("\nEnter the allocation matrix:");
    for (i = 0; i < np; i++)
    {
        alloc[i] = (int *)malloc(nr * sizeof(int));
        for (j = 0; j < nr; j++)
        {
            scanf("%d", &alloc[i][j]);
        }
    }

    for (j = 0; j < nr; j++)
    {
        avail[j] = r[j];
        for (i = 0; i < np; i++)
        {
            avail[j] -= alloc[i][j];
        }
    }

    for (i = 0; i < nr; i++)
    {
        printf("Resc %d :: %d", i, avail[i]);
    }

    while (1)
    {   
        int deadlock = 1;
        for (i = 0; i < np ;i++){
            int canBeProc = 1;
            for (j = 0; j < nr; j++){
                if (request[i][j] > avail[j]){
                    canBeProc = 0;
                }
            }
            if (canBeProc){
                deadlock = 0;
                f[i] = 1;
                for (j = 0; j < nr; j++){
                    avail[j] += alloc[i][j];
                }
            }
        }
        if (deadlock){
            printf("\n Deadlock detected");
            break;
        }
        int completed = 1;
        for (i = 0; i < np ; i++){
            if (f[i] == 0){
                completed = 0;
            }
        }
        if (completed){
            printf("\n Deadlock not detected");
            break;
        }
    }
}


#15 16 17 placement strategies

#include <stdio.h>

void firstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];

    for (int i = 0; i < n; i++) {
        allocation[i] = -1;

        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }

    printf("\nFirst Fit Allocation:\n");
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

void worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];

    for (int i = 0; i < n; i++) {
        allocation[i] = -1;

        int worstIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx]) {
                    worstIdx = j;
                }
            }
        }

        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
        }
    }

    printf("\nWorst Fit Allocation:\n");
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

void bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];

    for (int i = 0; i < n; i++) {
        allocation[i] = -1;

        int bestIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx]) {
                    bestIdx = j;
                }
            }
        }

        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }

    printf("\nBest Fit Allocation:\n");
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

void nextFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    int lastBlock = 0;

    for (int i = 0; i < n; i++) {
        allocation[i] = -1;

        for (int j = lastBlock; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                lastBlock = j;
                break;
            }
        }

        if (allocation[i] == -1) {
            for (int j = 0; j < lastBlock; j++) {
                if (blockSize[j] >= processSize[i]) {
                    allocation[i] = j;
                    blockSize[j] -= processSize[i];
                    lastBlock = j;
                    break;
                }
            }
        }
    }

    printf("\nNext Fit Allocation:\n");
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

int main() {
    int blockSize[] = {100, 500, 200, 300, 600};
    int processSize[] = {212, 417, 112, 426};
    int m = sizeof(blockSize) / sizeof(blockSize[0]);
    int n = sizeof(processSize) / sizeof(processSize[0]);

    firstFit(blockSize, m, processSize, n);
    worstFit(blockSize, m, processSize, n);
    bestFit(blockSize, m, processSize, n);
    nextFit(blockSize, m, processSize, n);

    return 0;
}

#18 FIFO and optimal
#include <stdio.h>
#include <stdbool.h>

// Function to check if a page is present in the frame
bool isPagePresent(int page, int frame[], int frameSize) {
    for (int i = 0; i < frameSize; i++) {
        if (frame[i] == page)
            return true;
    }
    return false;
}

// Function to find the index of the page that will be replaced
int findReplacementIndex(int page, int frame[], int frameSize, int referenceString[], int referenceSize, int currentIndex) {
    int index = -1, farthest = currentIndex;
    for (int i = 0; i < frameSize; i++) {
        int j;
        for (j = currentIndex; j < referenceSize; j++) {
            if (frame[i] == referenceString[j]) {
                if (j > farthest) {
                    farthest = j;
                    index = i;
                }
                break;
            }
        }
        if (j == referenceSize)
            return i; // if a page is not found in future, it will be replaced first
    }
    return (index == -1) ? 0 : index;
}

// First In First Out (FIFO) Page Replacement Algorithm
int fifo(int referenceString[], int referenceSize, int frameSize) {
    int frame[frameSize];
    int pageFaults = 0;
    int currentIndex = 0;

    for (int i = 0; i < frameSize; i++)
        frame[i] = -1; // Initialize frame slots with -1

    for (int i = 0; i < referenceSize; i++) {
        if (!isPagePresent(referenceString[i], frame, frameSize)) {
            frame[currentIndex] = referenceString[i];
            currentIndex = (currentIndex + 1) % frameSize;
            pageFaults++;
        }
    }
    return pageFaults;
}

// Optimal Page Replacement Algorithm
int optimal(int referenceString[], int referenceSize, int frameSize) {
    int frame[frameSize];
    int pageFaults = 0;

    for (int i = 0; i < frameSize; i++)
        frame[i] = -1; // Initialize frame slots with -1

    for (int i = 0; i < referenceSize; i++) {
        if (!isPagePresent(referenceString[i], frame, frameSize)) {
            int index = findReplacementIndex(referenceString[i], frame, frameSize, referenceString, referenceSize, i + 1);
            frame[index] = referenceString[i];
            pageFaults++;
        }
    }
    return pageFaults;
}

int main() {
    int referenceString[] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int referenceSize = sizeof(referenceString) / sizeof(referenceString[0]);
    int frameSize = 3;

    printf("Total number of page faults using FIFO: %d\n", fifo(referenceString, referenceSize, frameSize));
    printf("Total number of page faults using Optimal: %d\n", optimal(referenceString, referenceSize, frameSize));

    return 0;
}

#19 LRU and Second chance
#include <stdio.h>
#include <stdbool.h>

// Function to check if a page is present in the frame
bool isPagePresent(int page, int frame[], int frameSize) {
    for (int i = 0; i < frameSize; i++) {
        if (frame[i] == page)
            return true;
    }
    return false;
}

// Least Recently Used (LRU) Page Replacement Algorithm
int lru(int referenceString[], int referenceSize, int frameSize) {
    int frame[frameSize];
    int pageFaults = 0;
    int usedOrder[frameSize];
    int currentIndex = 0;

    for (int i = 0; i < frameSize; i++) {
        frame[i] = -1; // Initialize frame slots with -1
        usedOrder[i] = 0; // Initialize used order with 0
    }

    for (int i = 0; i < referenceSize; i++) {
        if (!isPagePresent(referenceString[i], frame, frameSize)) {
            int minUsedIndex = 0;
            for (int j = 1; j < frameSize; j++) {
                if (usedOrder[j] < usedOrder[minUsedIndex])
                    minUsedIndex = j;
            }
            frame[minUsedIndex] = referenceString[i];
            usedOrder[minUsedIndex] = i + 1; // Update the used order
            pageFaults++;
        } else {
            for (int j = 0; j < frameSize; j++) {
                if (frame[j] == referenceString[i]) {
                    usedOrder[j] = i + 1; // Update the used order
                    break;
                }
            }
        }
    }
    return pageFaults;
}

// Second Chance (Clock) Page Replacement Algorithm
int secondChance(int referenceString[], int referenceSize, int frameSize) {
    int frame[frameSize];
    bool referenced[frameSize];
    int pageFaults = 0;
    int currentIndex = 0;

    for (int i = 0; i < frameSize; i++) {
        frame[i] = -1; // Initialize frame slots with -1
        referenced[i] = false; // Initialize referenced flag with false
    }

    for (int i = 0; i < referenceSize; i++) {
        if (!isPagePresent(referenceString[i], frame, frameSize)) {
            while (true) {
                if (!referenced[currentIndex]) {
                    frame[currentIndex] = referenceString[i];
                    referenced[currentIndex] = true;
                    currentIndex = (currentIndex + 1) % frameSize;
                    pageFaults++;
                    break;
                } else {
                    referenced[currentIndex] = false;
                    currentIndex = (currentIndex + 1) % frameSize;
                }
            }
        } else {
            for (int j = 0; j < frameSize; j++) {
                if (frame[j] == referenceString[i]) {
                    referenced[j] = true;
                    break;
                }
            }
        }
    }
    return pageFaults;
}

int main() {
    int referenceString[] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int referenceSize = sizeof(referenceString) / sizeof(referenceString[0]);
    int frameSize = 3;

    printf("Total number of page faults using LRU: %d\n", lru(referenceString, referenceSize, frameSize));
    printf("Total number of page faults using Second Chance (Clock): %d\n", secondChance(referenceString, referenceSize, frameSize));

    return 0;
}


#20 LRU and Optimal

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Function to check if a page is present in the frame
bool isPagePresent(int page, int frame[], int frameSize) {
    for (int i = 0; i < frameSize; i++) {
        if (frame[i] == page)
            return true;
    }
    return false;
}

// Function to find the index of the page that will be replaced (LRU)
int findLRUIndex(int usedOrder[], int frameSize) {
    int minUsedIndex = 0;
    for (int i = 1; i < frameSize; i++) {
        if (usedOrder[i] < usedOrder[minUsedIndex])
            minUsedIndex = i;
    }
    return minUsedIndex;
}

// Function to find the index of the page that will be replaced (Optimal)
int findOptimalIndex(int referenceString[], int referenceSize, int frame[], int frameSize, int currentIndex) {
    int index = -1, farthest = currentIndex;
    for (int i = 0; i < frameSize; i++) {
        int j;
        for (j = currentIndex; j < referenceSize; j++) {
            if (frame[i] == referenceString[j]) {
                if (j > farthest) {
                    farthest = j;
                    index = i;
                }
                break;
            }
        }
        if (j == referenceSize)
            return i; // if a page is not found in future, it will be replaced first
    }
    return (index == -1) ? 0 : index;
}

// Least Recently Used (LRU) Page Replacement Algorithm
int lru(int referenceString[], int referenceSize, int frameSize) {
    int frame[frameSize];
    int usedOrder[frameSize];
    int pageFaults = 0;
    int currentIndex = 0;

    for (int i = 0; i < frameSize; i++) {
        frame[i] = -1; // Initialize frame slots with -1
        usedOrder[i] = 0; // Initialize used order with 0
    }

    for (int i = 0; i < referenceSize; i++) {
        if (!isPagePresent(referenceString[i], frame, frameSize)) {
            int index = findLRUIndex(usedOrder, frameSize);
            frame[index] = referenceString[i];
            usedOrder[index] = i + 1; // Update the used order
            pageFaults++;
        } else {
            for (int j = 0; j < frameSize; j++) {
                if (frame[j] == referenceString[i]) {
                    usedOrder[j] = i + 1; // Update the used order
                    break;
                }
            }
        }
    }
    return pageFaults;
}

// Optimal Page Replacement Algorithm
int optimal(int referenceString[], int referenceSize, int frameSize) {
    int frame[frameSize];
    int pageFaults = 0;

    for (int i = 0; i < frameSize; i++)
        frame[i] = -1; // Initialize frame slots with -1

    for (int i = 0; i < referenceSize; i++) {
        if (!isPagePresent(referenceString[i], frame, frameSize)) {
            int index = findOptimalIndex(referenceString, referenceSize, frame, frameSize, i + 1);
            frame[index] = referenceString[i];
            pageFaults++;
        }
    }
    return pageFaults;
}

int main() {
    int referenceString[] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int referenceSize = sizeof(referenceString) / sizeof(referenceString[0]);
    int frameSize = 3;

    printf("Total number of page faults using LRU: %d\n", lru(referenceString, referenceSize, frameSize));
    printf("Total number of page faults using Optimal: %d\n", optimal(referenceString, referenceSize, frameSize));

    return 0;
}


#21 LRU and FIFO

#include <stdio.h>
#include <stdbool.h>

// Function to check if a page is present in the frame
bool isPagePresent(int page, int frame[], int frameSize) {
    for (int i = 0; i < frameSize; i++) {
        if (frame[i] == page)
            return true;
    }
    return false;
}

// Least Recently Used (LRU) Page Replacement Algorithm
int lru(int referenceString[], int referenceSize, int frameSize) {
    int frame[frameSize];
    int usedOrder[frameSize];
    int pageFaults = 0;
    int currentIndex = 0;

    for (int i = 0; i < frameSize; i++) {
        frame[i] = -1; // Initialize frame slots with -1
        usedOrder[i] = 0; // Initialize used order with 0
    }

    for (int i = 0; i < referenceSize; i++) {
        if (!isPagePresent(referenceString[i], frame, frameSize)) {
            int index = 0, minUsedIndex = usedOrder[0];
            for (int j = 1; j < frameSize; j++) {
                if (usedOrder[j] < minUsedIndex) {
                    minUsedIndex = usedOrder[j];
                    index = j;
                }
            }
            frame[index] = referenceString[i];
            usedOrder[index] = i + 1; // Update the used order
            pageFaults++;
        } else {
            for (int j = 0; j < frameSize; j++) {
                if (frame[j] == referenceString[i]) {
                    usedOrder[j] = i + 1; // Update the used order
                    break;
                }
            }
        }
    }
    return pageFaults;
}

// First In First Out (FIFO) Page Replacement Algorithm
int fifo(int referenceString[], int referenceSize, int frameSize) {
    int frame[frameSize];
    int pageFaults = 0;
    int currentIndex = 0;

    for (int i = 0; i < frameSize; i++)
        frame[i] = -1; // Initialize frame slots with -1

    for (int i = 0; i < referenceSize; i++) {
        if (!isPagePresent(referenceString[i], frame, frameSize)) {
            frame[currentIndex] = referenceString[i];
            currentIndex = (currentIndex + 1) % frameSize;
            pageFaults++;
        }
    }
    return pageFaults;
}

int main() {
    int referenceString[] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int referenceSize = sizeof(referenceString) / sizeof(referenceString[0]);
    int frameSize = 3;

    printf("Total number of page faults using LRU: %d\n", lru(referenceString, referenceSize, frameSize));
    printf("Total number of page faults using FIFO: %d\n", fifo(referenceString, referenceSize, frameSize));

    return 0;
}


#22 DS FCFS and SSTF

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Function to calculate total head movements using FCFS algorithm
int fcfs(int requests[], int size, int initialPosition) {
    int totalHeadMovements = abs(requests[0] - initialPosition);
    for (int i = 1; i < size; i++) {
        totalHeadMovements += abs(requests[i] - requests[i - 1]);
    }
    return totalHeadMovements;
}

// Function to calculate total head movements using SSTF algorithm
int sstf(int requests[], int size, int initialPosition) {
    int totalHeadMovements = 0;
    int visited[size];
    for (int i = 0; i < size; i++) {
        visited[i] = 0;
    }

    int currentPosition = initialPosition;
    for (int i = 0; i < size; i++) {
        int minDistance = INT_MAX;
        int minIndex = -1;
        for (int j = 0; j < size; j++) {
            if (!visited[j]) {
                int distance = abs(currentPosition - requests[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    minIndex = j;
                }
            }
        }
        visited[minIndex] = 1;
        totalHeadMovements += minDistance;
        currentPosition = requests[minIndex];
    }
    return totalHeadMovements;
}

int main() {
    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};
    int size = sizeof(requests) / sizeof(requests[0]);
    int initialPosition = 53;

    printf("Total head movements using FCFS: %d\n", fcfs(requests, size, initialPosition));
    printf("Total head movements using SSTF: %d\n", sstf(requests, size, initialPosition));

    return 0;
}

#23 SCAN and CSCAN

#include <stdio.h>
#include <stdlib.h>

// Function to sort an array in ascending order
void sort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function to calculate total head movements using SCAN algorithm
int scan(int requests[], int size, int initialPosition, int upperBound) {
    int totalHeadMovements = 0;
    int direction = 1; // 1 for moving towards higher cylinder numbers, -1 for moving towards lower cylinder numbers
    int currentPosition = initialPosition;

    sort(requests, size);

    for (int i = 0; i < size; i++) {
        if (direction == 1) {
            // Move towards higher cylinder numbers
            for (int j = 0; j < size; j++) {
                if (requests[j] >= currentPosition) {
                    totalHeadMovements += abs(requests[j] - currentPosition);
                    currentPosition = requests[j];
                    break;
                }
            }
            if (currentPosition == upperBound) {
                direction = -1;
                totalHeadMovements += abs(upperBound - 0);
                currentPosition = 0;
            }
        } else {
            // Move towards lower cylinder numbers
            for (int j = size - 1; j >= 0; j--) {
                if (requests[j] <= currentPosition) {
                    totalHeadMovements += abs(requests[j] - currentPosition);
                    currentPosition = requests[j];
                    break;
                }
            }
            if (currentPosition == 0) {
                direction = 1;
                totalHeadMovements += abs(upperBound - 0);
                currentPosition = upperBound;
            }
        }
    }

    return totalHeadMovements;
}

// Function to calculate total head movements using C-SCAN algorithm
int cscan(int requests[], int size, int initialPosition, int upperBound) {
    int totalHeadMovements = 0;
    int currentPosition = initialPosition;

    sort(requests, size);

    // Move towards higher cylinder numbers
    int i;
    for (i = 0; i < size; i++) {
        if (requests[i] >= currentPosition) {
            totalHeadMovements += abs(requests[i] - currentPosition);
            currentPosition = requests[i];
            break;
        }
    }
    if (currentPosition != upperBound) {
        totalHeadMovements += abs(upperBound - currentPosition);
        currentPosition = 0;
    }

    // Move back to the lower bound
    totalHeadMovements += upperBound;

    // Move towards higher cylinder numbers again
    for (i = 0; i < size; i++) {
        totalHeadMovements += abs(requests[i] - currentPosition);
        currentPosition = requests[i];
    }

    return totalHeadMovements;
}

int main() {
    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};
    int size = sizeof(requests) / sizeof(requests[0]);
    int initialPosition = 53;
    int upperBound = 199; // The upper bound of the cylinder

    printf("Total head movements using SCAN: %d\n", scan(requests, size, initialPosition, upperBound));
    printf("Total head movements using C-SCAN: %d\n", cscan(requests, size, initialPosition, upperBound));

    return 0;
}

#24 SSTF and SCAN

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Function to calculate total head movements using SSTF algorithm
int sstf(int requests[], int size, int initialPosition) {
    int totalHeadMovements = 0;
    int visited[size];
    for (int i = 0; i < size; i++) {
        visited[i] = 0;
    }

    int currentPosition = initialPosition;
    for (int i = 0; i < size; i++) {
        int minDistance = INT_MAX;
        int minIndex = -1;
        for (int j = 0; j < size; j++) {
            if (!visited[j]) {
                int distance = abs(currentPosition - requests[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    minIndex = j;
                }
            }
        }
        visited[minIndex] = 1;
        totalHeadMovements += minDistance;
        currentPosition = requests[minIndex];
    }
    return totalHeadMovements;
}

// Function to calculate total head movements using SCAN algorithm
int scan(int requests[], int size, int initialPosition, int upperBound) {
    int totalHeadMovements = 0;
    int direction = 1; // 1 for moving towards higher cylinder numbers, -1 for moving towards lower cylinder numbers
    int currentPosition = initialPosition;

    // Sort the requests in ascending order
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (requests[j] > requests[j + 1]) {
                int temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        if (direction == 1) {
            // Move towards higher cylinder numbers
            for (int j = 0; j < size; j++) {
                if (requests[j] >= currentPosition) {
                    totalHeadMovements += abs(requests[j] - currentPosition);
                    currentPosition = requests[j];
                    break;
                }
            }
            if (currentPosition == upperBound) {
                direction = -1;
                totalHeadMovements += abs(upperBound - 0);
                currentPosition = 0;
            }
        } else {
            // Move towards lower cylinder numbers
            for (int j = size - 1; j >= 0; j--) {
                if (requests[j] <= currentPosition) {
                    totalHeadMovements += abs(requests[j] - currentPosition);
                    currentPosition = requests[j];
                    break;
                }
            }
            if (currentPosition == 0) {
                direction = 1;
                totalHeadMovements += abs(upperBound - 0);
                currentPosition = upperBound;
            }
        }
    }

    return totalHeadMovements;
}

int main() {
    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};
    int size = sizeof(requests) / sizeof(requests[0]);
    int initialPosition = 53;
    int upperBound = 199; // The upper bound of the cylinder

    printf("Total head movements using SSTF: %d\n", sstf(requests, size, initialPosition));
    printf("Total head movements using SCAN: %d\n", scan(requests, size, initialPosition, upperBound));

    return 0;
}

#25 FCFS and SCAN

#include <stdio.h>
#include <stdlib.h>

// Function to calculate total head movements using FCFS algorithm
int fcfs(int requests[], int size, int initialPosition) {
    int totalHeadMovements = abs(requests[0] - initialPosition);
    for (int i = 1; i < size; i++) {
        totalHeadMovements += abs(requests[i] - requests[i - 1]);
    }
    return totalHeadMovements;
}

// Function to calculate total head movements using SCAN algorithm
int scan(int requests[], int size, int initialPosition, int upperBound) {
    int totalHeadMovements = 0;
    int direction = 1; // 1 for moving towards higher cylinder numbers, -1 for moving towards lower cylinder numbers
    int currentPosition = initialPosition;

    // Sort the requests in ascending order
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (requests[j] > requests[j + 1]) {
                int temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        if (direction == 1) {
            // Move towards higher cylinder numbers
            for (int j = 0; j < size; j++) {
                if (requests[j] >= currentPosition) {
                    totalHeadMovements += abs(requests[j] - currentPosition);
                    currentPosition = requests[j];
                    break;
                }
            }
            if (currentPosition == upperBound) {
                direction = -1;
                totalHeadMovements += abs(upperBound - 0);
                currentPosition = 0;
            }
        } else {
            // Move towards lower cylinder numbers
            for (int j = size - 1; j >= 0; j--) {
                if (requests[j] <= currentPosition) {
                    totalHeadMovements += abs(requests[j] - currentPosition);
                    currentPosition = requests[j];
                    break;
                }
            }
            if (currentPosition == 0) {
                direction = 1;
                totalHeadMovements += abs(upperBound - 0);
                currentPosition = upperBound;
            }
        }
    }

    return totalHeadMovements;
}

int main() {
    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};
    int size = sizeof(requests) / sizeof(requests[0]);
    int initialPosition = 53;
    int upperBound = 199; // The upper bound of the cylinder

    printf("Total head movements using FCFS: %d\n", fcfs(requests, size, initialPosition));
    printf("Total head movements using SCAN: %d\n", scan(requests, size, initialPosition, upperBound));

    return 0;
}

#26 SSTF and CSCAN

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Function to calculate total head movements using SSTF algorithm
int sstf(int requests[], int size, int initialPosition) {
    int totalHeadMovements = 0;
    int visited[size];
    for (int i = 0; i < size; i++) {
        visited[i] = 0;
    }

    int currentPosition = initialPosition;
    for (int i = 0; i < size; i++) {
        int minDistance = INT_MAX;
        int minIndex = -1;
        for (int j = 0; j < size; j++) {
            if (!visited[j]) {
                int distance = abs(currentPosition - requests[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    minIndex = j;
                }
            }
        }
        visited[minIndex] = 1;
        totalHeadMovements += minDistance;
        currentPosition = requests[minIndex];
    }
    return totalHeadMovements;
}

// Function to calculate total head movements using C-SCAN algorithm
int cscan(int requests[], int size, int initialPosition, int upperBound) {
    int totalHeadMovements = 0;
    int currentPosition = initialPosition;

    // Sort the requests in ascending order
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (requests[j] > requests[j + 1]) {
                int temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }

    // Move towards higher cylinder numbers
    int i;
    for (i = 0; i < size; i++) {
        if (requests[i] >= currentPosition) {
            totalHeadMovements += abs(requests[i] - currentPosition);
            currentPosition = requests[i];
            break;
        }
    }
    if (currentPosition != upperBound) {
        totalHeadMovements += abs(upperBound - currentPosition);
        currentPosition = 0;
    }

    // Move back to the lower bound
    totalHeadMovements += upperBound;

    // Move towards higher cylinder numbers again
    for (i = 0; i < size; i++) {
        totalHeadMovements += abs(requests[i] - currentPosition);
        currentPosition = requests[i];
    }

    return totalHeadMovements;
}

int main() {
    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};
    int size = sizeof(requests) / sizeof(requests[0]);
    int initialPosition = 53;
    int upperBound = 199; // The upper bound of the cylinder

    printf("Total head movements using SSTF: %d\n", sstf(requests, size, initialPosition));
    printf("Total head movements using C-SCAN: %d\n", cscan(requests, size, initialPosition, upperBound));

    return 0;
}

#27 FCFS and CSCAN

#include <stdio.h>
#include <stdlib.h>

// Function to calculate total head movements using FCFS algorithm
int fcfs(int requests[], int size, int initialPosition) {
    int totalHeadMovements = abs(requests[0] - initialPosition);
    for (int i = 1; i < size; i++) {
        totalHeadMovements += abs(requests[i] - requests[i - 1]);
    }
    return totalHeadMovements;
}

// Function to calculate total head movements using C-SCAN algorithm
int cscan(int requests[], int size, int initialPosition, int upperBound) {
    int totalHeadMovements = 0;
    int currentPosition = initialPosition;

    // Sort the requests in ascending order
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (requests[j] > requests[j + 1]) {
                int temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }

    // Move towards higher cylinder numbers
    int i;
    for (i = 0; i < size; i++) {
        if (requests[i] >= currentPosition) {
            totalHeadMovements += abs(requests[i] - currentPosition);
            currentPosition = requests[i];
            break;
        }
    }
    if (currentPosition != upperBound) {
        totalHeadMovements += abs(upperBound - currentPosition);
        currentPosition = 0;
    }

    // Move back to the lower bound
    totalHeadMovements += upperBound;

    // Move towards higher cylinder numbers again
    for (i = 0; i < size; i++) {
        totalHeadMovements += abs(requests[i] - currentPosition);
        currentPosition = requests[i];
    }

    return totalHeadMovements;
}

int main() {
    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};
    int size = sizeof(requests) / sizeof(requests[0]);
    int initialPosition = 53;
    int upperBound = 199; // The upper bound of the cylinder

    printf("Total head movements using FCFS: %d\n", fcfs(requests, size, initialPosition));
    printf("Total head movements using C-SCAN: %d\n", cscan(requests, size, initialPosition, upperBound));

    return 0;
}


#28 cpu FCFS and Optimal

#include <stdio.h>

#define MAX_PROCESSES 10

struct Process {
    int pid;
    int arrival;
    int burst;
    int completion;
    int waiting;
};

void fcfs(struct Process processes[], int n) {
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrival) {
            currentTime = processes[i].arrival;
        }
        processes[i].completion = currentTime + processes[i].burst;
        processes[i].waiting = currentTime - processes[i].arrival; 
        currentTime = processes[i].completion;
    }
}

int main() {
    int n;
    float avgTurnaroundTime = 0;
    float avgWaitingTime = 0;
    struct Process processes[MAX_PROCESSES];
    printf("==========/First Come First Serve Algorithm/===========\n");

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter arrival time and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i + 1);
        scanf("%d %d", &processes[i].arrival, &processes[i].burst);
        processes[i].pid = i + 1;
    }
    fcfs(processes, n);

    for (int i = 0; i < n; i++) {
        avgTurnaroundTime += processes[i].completion - processes[i].arrival;
        avgWaitingTime += processes[i].waiting;
    }
    avgTurnaroundTime /= n;
    avgWaitingTime /= n;

    printf("\nGantt Chart:\n");
    printf(" ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n|");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst - 1; j++) {
            printf(" ");
        }
        printf("P%d", processes[i].pid);
        for (int j = 0; j < processes[i].burst - 1; j++) {
            printf(" ");
        }
        printf("|");
    }
    printf("\n ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n");
        printf("0");
    for (int i = 0; i < n; i++) {
        printf("   %d\t\t", processes[i].completion);
    }
    
    printf("\nFCFS Scheduling:\n");
    printf("PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].arrival, 
               processes[i].burst, processes[i].completion, 
               processes[i].completion - processes[i].arrival, processes[i].waiting);
    }

    printf("\nAverage Turnaround Time: %.2f\n", avgTurnaroundTime);
    printf("Average Waiting Time: %.2f\n", avgWaitingTime);

    return 0;
}

-----------

#include <stdio.h>

int findOptimal(int pages[], int n, int frames[], int frameSize) {
    int faults = 0;
    int index[frameSize];
    for (int i = 0; i < frameSize; i++)
        index[i] = -1;

    for (int i = 0; i < n; i++) {
        int j;
        for (j = 0; j < frameSize; j++) {
            if (frames[j] == pages[i]) {
                break;
            }
        }

        if (j == frameSize) {
            int k;
            for (k = 0; k < frameSize; k++) {
                if (index[k] == -1) {
                    break;
                }
            }

            if (k == frameSize) {
                int farthest = i + 1, pos = -1;
                for (int l = 0; l < frameSize; l++) {
                    int m;
                    for (m = i + 1; m < n; m++) {
                        if (frames[l] == pages[m]) {
                            break;
                        }
                    }
                    if (m == n) {
                        pos = l;
                        break;
                    }
                    if (m > farthest) {
                        farthest = m;
                        pos = l;
                    }
                }
                index[pos] = i;
                frames[pos] = pages[i];
                faults++;
            } else {
                index[k] = i;
                frames[k] = pages[i];
                faults++;
            }
        }
    }
    return faults;
}

int main() {
    int pages[] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int n = sizeof(pages) / sizeof(pages[0]);
    int frameSize = 3;
    int frames[frameSize];

    printf("Total number of page faults using Optimal page replacement algorithm: %d\n", findOptimal(pages, n, frames, frameSize));

    return 0;
}


#29 CPU SJF and FIFO
#include <stdio.h>

struct Process {
    int p_id;
    int bt;
    int at; // Arrival time
    int wt; 
    int tat; 
    int completed;
};

int findShortestJob(struct Process processes[], int n, int currentTime) {
    int shortestJobIndex = -1;
    int shortestBurst = 9999;
    
    for (int i = 0; i < n; ++i) {
        if (!processes[i].completed && processes[i].bt < shortestBurst && processes[i].at <= currentTime && processes[i].bt > 0) {
            shortestBurst = processes[i].bt;
            shortestJobIndex = i;
        }
    }
    
    return shortestJobIndex;
}

void calculateTimes(struct Process processes[], int n) {
    int currentTime = 0;
    int totalBurst = 0;
    
    // Calculate total burst time
    for (int i = 0; i < n; ++i) {
        totalBurst += processes[i].bt;
    }
    
    printf("Gantt Chart:\n");
    printf("|");
    
    while (currentTime < totalBurst) {
        int shortestJobIndex = findShortestJob(processes, n, currentTime);
        if (shortestJobIndex == -1) {
            currentTime++;
            continue;
        }
        
        printf(" P%d(%d) |", processes[shortestJobIndex].p_id, processes[shortestJobIndex].bt);
        
        processes[shortestJobIndex].bt--;
        
        for (int i = 0; i < n; ++i) {
            if (i != shortestJobIndex && !processes[i].completed && processes[i].at <= currentTime) {
                processes[i].wt++;
            }
        }
        
        currentTime++;
        
        if (processes[shortestJobIndex].bt == 0) {
            processes[shortestJobIndex].completed = 1;
            processes[shortestJobIndex].tat = currentTime;
        }
    }
    printf("\n");
}

void calculateAverages(struct Process processes[], int n, float *avg_waiting_time, float *avg_turnaround_time) {
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    
    for (int i = 0; i < n; ++i) {
        total_waiting_time += processes[i].wt;
        total_turnaround_time += processes[i].tat;
    }
    
    *avg_waiting_time = (float)total_waiting_time / n;
    *avg_turnaround_time = (float)total_turnaround_time / n;
}

void displayProcesses(struct Process processes[], int n) {
    printf("Process ID\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; ++i) {
        printf("%d\t\t%d\t\t%d\n", processes[i].p_id, processes[i].wt, processes[i].tat);
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    struct Process processes[n];
    
    for (int i = 0; i < n; ++i) {
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].bt);
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].at);
        processes[i].p_id = i + 1;
        processes[i].wt = 0;
        processes[i].tat = 0;
        processes[i].completed = 0;
    }
    
    calculateTimes(processes, n);
    
    float avg_waiting_time, avg_turnaround_time;
    calculateAverages(processes, n, &avg_waiting_time, &avg_turnaround_time);
    
    displayProcesses(processes, n);
    
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    
    return 0;
}
-----------
#include <stdio.h>

int fifo(int pages[], int n, int frames[], int frameSize) {
    int faults = 0;
    int rear = 0;

    // Initialize frame array with -1 (indicating empty frame)
    for (int i = 0; i < frameSize; i++) {
        frames[i] = -1;
    }

    // Iterate through page references
    for (int i = 0; i < n; i++) {
        int j;
        // Check if page is already in frame
        for (j = 0; j < frameSize; j++) {
            if (frames[j] == pages[i]) {
                break;
            }
        }
        // If page is not in frame, replace the oldest page (FIFO)
        if (j == frameSize) {
            frames[rear] = pages[i];
            rear = (rear + 1) % frameSize;
            faults++;
        }
    }

    return faults;
}

int main() {
    int pages[] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int n = sizeof(pages) / sizeof(pages[0]);
    int frameSize = 3;
    int frames[frameSize];

    printf("Total number of page faults using FIFO page replacement algorithm: %d\n", fifo(pages, n, frames, frameSize));

    return 0;
}


#30 RR and FIFO
#include <stdio.h>

void findWaitingTime(int processes[], int n, int bt[], int wt[], int quantum) {
    int rem_bt[n];
    for (int i = 0; i < n; i++)
        rem_bt[i] = bt[i];

    int t = 0;

    while (1) {
        int done = 1;

        for (int i = 0; i < n; i++) {
            if (rem_bt[i] > 0) {
                done = 0;

                if (rem_bt[i] > quantum) {
                    t += quantum;
                    rem_bt[i] -= quantum;
                } else {
                    t += rem_bt[i];
                    wt[i] = t - bt[i];
                    rem_bt[i] = 0;
                }
            }
        }
        if (done == 1)
            break;
    }
}

void findTurnAroundTime(int processes[], int n, int bt[], int wt[], int tat[]) {
    for (int i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];
}

void findavgTime(int processes[], int n, int bt[], int quantum) {
    int wt[n], tat[n];
    int total_wt = 0, total_tat = 0;

    findWaitingTime(processes, n, bt, wt, quantum);

    findTurnAroundTime(processes, n, bt, wt, tat);

    printf("PN\tB\tWT\tTAT\n");

    for (int i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        printf("%d\t%d\t%d\t%d\n", processes[i], bt[i], wt[i], tat[i]);
    }

    printf("Average waiting time = %.2f\n", (float)total_wt / (float)n);
    printf("Average turn around time = %.2f\n", (float)total_tat / (float)n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int processes[n];
    printf("Enter process IDs: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &processes[i]);

    int burst_time[n];
    printf("Enter burst times for each process: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &burst_time[i]);

    int quantum;
    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    findavgTime(processes, n, burst_time, quantum);
    return 0;
}
-------------
#include <stdio.h>

int fifo(int pages[], int n, int frames[], int frameSize) {
    int faults = 0;
    int rear = 0;

    // Initialize frame array with -1 (indicating empty frame)
    for (int i = 0; i < frameSize; i++) {
        frames[i] = -1;
    }

    // Iterate through page references
    for (int i = 0; i < n; i++) {
        int j;
        // Check if page is already in frame
        for (j = 0; j < frameSize; j++) {
            if (frames[j] == pages[i]) {
                break;
            }
        }
        // If page is not in frame, replace the oldest page (FIFO)
        if (j == frameSize) {
            frames[rear] = pages[i];
            rear = (rear + 1) % frameSize;
            faults++;
        }
    }

    return faults;
}

int main() {
    int pages[] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int n = sizeof(pages) / sizeof(pages[0]);
    int frameSize = 3;
    int frames[frameSize];

    printf("Total number of page faults using FIFO page replacement algorithm: %d\n", fifo(pages, n, frames, frameSize));

    return 0;
}


#31 DS FCFS and best fit
#include <stdio.h>
#include <stdlib.h>

// Function to calculate total head movements using FCFS algorithm
int fcfs(int requests[], int size, int initialPosition) {
    int totalHeadMovements = abs(requests[0] - initialPosition);
    for (int i = 1; i < size; i++) {
        totalHeadMovements += abs(requests[i] - requests[i - 1]);
    }
    return totalHeadMovements;
}

int main() {
    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};
    int size = sizeof(requests) / sizeof(requests[0]);
    int initialPosition = 53;

    printf("Total head movements using FCFS: %d\n", fcfs(requests, size, initialPosition));

    return 0;
}
-----------------
#include <stdio.h>

// Function to allocate memory using Best Fit algorithm
void bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n]; // Store block number assigned to a process

    // Initialize all blocks as not allocated
    for (int i = 0; i < n; i++) {
        allocation[i] = -1;
    }

    // Iterate through all processes and allocate memory blocks
    for (int i = 0; i < n; i++) {
        // Find the index of the smallest block that can accommodate the current process
        int bestFitIndex = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestFitIndex == -1 || blockSize[j] < blockSize[bestFitIndex]) {
                    bestFitIndex = j;
                }
            }
        }
        // If a block is found, allocate memory to the process
        if (bestFitIndex != -1) {
            allocation[i] = bestFitIndex;
            blockSize[bestFitIndex] -= processSize[i];
        }
    }

    // Print memory allocation details
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

int main() {
    int blockSize[] = {100, 500, 200, 300, 600};
    int m = sizeof(blockSize) / sizeof(blockSize[0]);
    int processSize[] = {212, 417, 112, 426};
    int n = sizeof(processSize) / sizeof(processSize[0]);

    bestFit(blockSize, m, processSize, n);

    return 0;
}


#32 SSTF and First Fit
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Function to find the index of the nearest request
int findNearest(int requests[], int n, int currentPosition) {
    int minDistance = INT_MAX;
    int index = -1;
    for (int i = 0; i < n; i++) {
        if (abs(requests[i] - currentPosition) < minDistance) {
            minDistance = abs(requests[i] - currentPosition);
            index = i;
        }
    }
    return index;
}

// Function to calculate total head movements using SSTF algorithm
int sstf(int requests[], int n, int initialPosition) {
    int totalHeadMovements = 0;
    int currentPosition = initialPosition;
    int visited[n];
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int nearestIndex = findNearest(requests, n, currentPosition);
        visited[nearestIndex] = 1;
        totalHeadMovements += abs(requests[nearestIndex] - currentPosition);
        currentPosition = requests[nearestIndex];
    }

    return totalHeadMovements;
}

int main() {
    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};
    int n = sizeof(requests) / sizeof(requests[0]);
    int initialPosition = 53;

    printf("Total head movements using SSTF: %d\n", sstf(requests, n, initialPosition));

    return 0;
}
------------------------
#include <stdio.h>

// Function to allocate memory using First Fit algorithm
void firstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n]; // Store block number assigned to a process

    // Initialize all blocks as not allocated
    for (int i = 0; i < n; i++) {
        allocation[i] = -1;
    }

    // Iterate through all processes and allocate memory blocks
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }

    // Print memory allocation details
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

int main() {
    int blockSize[] = {100, 500, 200, 300, 600};
    int m = sizeof(blockSize) / sizeof(blockSize[0]);
    int processSize[] = {212, 417, 112, 426};
    int n = sizeof(processSize) / sizeof(processSize[0]);

    firstFit(blockSize, m, processSize, n);

    return 0;
}


#33 SCAN and Worst fit

#include <stdio.h>
#include <stdlib.h>

// Function to calculate total head movements using SCAN algorithm
int scan(int requests[], int n, int initialPosition, char direction) {
    int totalHeadMovements = 0;
    int currentPosition = initialPosition;

    if (direction == 'l') {
        // Move towards lower cylinder numbers
        for (int i = currentPosition; i >= 0; i--) {
            int j;
            for (j = 0; j < n; j++) {
                if (requests[j] == i) {
                    totalHeadMovements += abs(currentPosition - requests[j]);
                    currentPosition = requests[j];
                    break;
                }
            }
            if (j < n) {
                // Request found, remove it from the list
                for (int k = j; k < n - 1; k++) {
                    requests[k] = requests[k + 1];
                }
                n--;
            }
        }
        // Move to the lowest cylinder number
        totalHeadMovements += currentPosition;
        currentPosition = 0;
        // Move towards higher cylinder numbers
        for (int i = 0; i < n; i++) {
            totalHeadMovements += abs(currentPosition - requests[i]);
            currentPosition = requests[i];
        }
    } else if (direction == 'r') {
        // Move towards higher cylinder numbers
        for (int i = currentPosition; i <= 199; i++) {
            int j;
            for (j = 0; j < n; j++) {
                if (requests[j] == i) {
                    totalHeadMovements += abs(currentPosition - requests[j]);
                    currentPosition = requests[j];
                    break;
                }
            }
            if (j < n) {
                // Request found, remove it from the list
                for (int k = j; k < n - 1; k++) {
                    requests[k] = requests[k + 1];
                }
                n--;
            }
        }
        // Move to the highest cylinder number
        totalHeadMovements += abs(currentPosition - 199);
        currentPosition = 199;
        // Move towards lower cylinder numbers
        for (int i = n - 1; i >= 0; i--) {
            totalHeadMovements += abs(currentPosition - requests[i]);
            currentPosition = requests[i];
        }
    }

    return totalHeadMovements;
}

int main() {
    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};
    int n = sizeof(requests) / sizeof(requests[0]);
    int initialPosition = 53;

    printf("Total head movements using SCAN (left): %d\n", scan(requests, n, initialPosition, 'l'));
    printf("Total head movements using SCAN (right): %d\n", scan(requests, n, initialPosition, 'r'));

    return 0;
}

-------------------

#include <stdio.h>

// Function to allocate memory using Worst Fit algorithm
void worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n]; // Store block number assigned to a process

    // Initialize all blocks as not allocated
    for (int i = 0; i < n; i++) {
        allocation[i] = -1;
    }

    // Iterate through all processes and allocate memory blocks
    for (int i = 0; i < n; i++) {
        // Find the index of the largest block that can accommodate the current process
        int worstFitIndex = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstFitIndex == -1 || blockSize[j] > blockSize[worstFitIndex]) {
                    worstFitIndex = j;
                }
            }
        }
        // If a block is found, allocate memory to the process
        if (worstFitIndex != -1) {
            allocation[i] = worstFitIndex;
            blockSize[worstFitIndex] -= processSize[i];
        }
    }

    // Print memory allocation details
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

int main() {
    int blockSize[] = {100, 500, 200, 300, 600};
    int m = sizeof(blockSize) / sizeof(blockSize[0]);
    int processSize[] = {212, 417, 112, 426};
    int n = sizeof(processSize) / sizeof(processSize[0]);

    worstFit(blockSize, m, processSize, n);

    return 0;
}





# 34 banker algo
#include <stdio.h>
int main()
{

	int n, m, i, j, k;
	n = 5;
	m = 3;
int alloc[5][3] = { { 1, 0, 0 },
                    { 2, 1, 0 },
                    { 1, 1, 1 },
                    { 1, 1, 0 },
                    { 0, 0, 2 } };

int max[5][3] = { {  5, 1, 1 },
                { 2, 2, 0 },
                { 1, 2, 1 },
                { 1, 1, 1 },
                { 0, 0, 2 } };

int avail[3] = { 3, 1, 1 };


	int f[n], ans[n], ind = 0;
	for (k = 0; k < n; k++) {
		f[k] = 0;
	}
	int need[n][m];
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++)
			need[i][j] = max[i][j] - alloc[i][j];
	}
        printf("Allocation Matrix \n");
        for(i=0;i<n;i++){
        for(j=0; j<m; j++){
        printf(" %d",alloc[i][j]);
        }
        printf("\n");
    }
        printf("Max Matrix \n");
        for(i=0;i<n;i++){
        for(j=0; j<m; j++){
        printf(" %d",max[i][j]);
        }
        printf("\n");
    }
    printf("Need Matrix \n");
        for(i=0;i<n;i++){
        for(j=0; j<m; j++){
        printf(" %d",need[i][j]);
        }
        printf("\n");
    }
	int y = 0;
	for (k = 0; k < 5; k++) {
		for (i = 0; i < n; i++) {
			if (f[i] == 0) {

				int flag = 0;
				for (j = 0; j < m; j++) {
					if (need[i][j] > avail[j]){
						flag = 1;
						break;
					}
				}

				if (flag == 0) {
					ans[ind++] = i;
					for (y = 0; y < m; y++)
						avail[y] += alloc[i][y];
					f[i] = 1;
				}
			}
		}
	}
    int flag = 1;
	for(int i=0;i<n;i++)
	{
	if(f[i]==0)
	{
		flag=0;
		printf("SYSTEM NOT SAFE");
		break;
	}
	}
	if(flag==1)
	{
	printf("SYSTEM SAFE\n");
	for (i = 0; i < n - 1; i++)
		printf(" P%d ->", ans[i]);
	printf(" P%d", ans[n - 1]);
	}
	return (0);
}


#35 Deadlock Algorithm

#include <stdio.h>
#include <stdlib.h>
static int mark[20];
int i, j, np, nr;

int main()
{
    int **alloc, **request, *avail, *r, *f;

    printf("\nEnter the no of the process: ");
    scanf("%d", &np);

    printf("\nEnter the no of resources: ");
    scanf("%d", &nr);

    alloc = (int **)malloc(np * sizeof(int *));
    request = (int **)malloc(np * sizeof(int *));
    avail = (int *)malloc(nr * sizeof(int));
    r = (int *)malloc(nr * sizeof(int));
    f = (int *)malloc(np * sizeof(int));

    for(i = 0;i < np;i ++)
        f[i] = 0;

    for (i = 0; i < nr; i++)
    {
        printf("\nTotal Amount of the Resource R % d: ", i + 1);
        scanf("%d", &r[i]);
    }

    printf("\nEnter the request matrix:");
    for (i = 0; i < np; i++)
    {
        request[i] = (int *)malloc(nr * sizeof(int));
        for (j = 0; j < nr; j++)
        {
            scanf("%d", &request[i][j]);
        }
    }

    printf("\nEnter the allocation matrix:");
    for (i = 0; i < np; i++)
    {
        alloc[i] = (int *)malloc(nr * sizeof(int));
        for (j = 0; j < nr; j++)
        {
            scanf("%d", &alloc[i][j]);
        }
    }

    for (j = 0; j < nr; j++)
    {
        avail[j] = r[j];
        for (i = 0; i < np; i++)
        {
            avail[j] -= alloc[i][j];
        }
    }

    for (i = 0; i < nr; i++)
    {
        printf("Resc %d :: %d", i, avail[i]);
    }

    while (1)
    {   
        int deadlock = 1;
        for (i = 0; i < np ;i++){
            int canBeProc = 1;
            for (j = 0; j < nr; j++){
                if (request[i][j] > avail[j]){
                    canBeProc = 0;
                }
            }
            if (canBeProc){
                deadlock = 0;
                f[i] = 1;
                for (j = 0; j < nr; j++){
                    avail[j] += alloc[i][j];
                }
            }
        }
        if (deadlock){
            printf("\n Deadlock detected");
            break;
        }
        int completed = 1;
        for (i = 0; i < np ; i++){
            if (f[i] == 0){
                completed = 0;
            }
        }
        if (completed){
            printf("\n Deadlock not detected");
            break;
        }
    }
}


#36 Banker Algo

#include <stdio.h>
int main()
{

	int n, m, i, j, k;
	n = 5;
	m = 3;
int alloc[5][3] = { { 1, 0, 0 },
                    { 2, 1, 0 },
                    { 1, 1, 1 },
                    { 1, 1, 0 },
                    { 0, 0, 2 } };

int max[5][3] = { {  5, 1, 1 },
                { 2, 2, 0 },
                { 1, 2, 1 },
                { 1, 1, 1 },
                { 0, 0, 2 } };

int avail[3] = { 3, 1, 1 };


	int f[n], ans[n], ind = 0;
	for (k = 0; k < n; k++) {
		f[k] = 0;
	}
	int need[n][m];
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++)
			need[i][j] = max[i][j] - alloc[i][j];
	}
        printf("Allocation Matrix \n");
        for(i=0;i<n;i++){
        for(j=0; j<m; j++){
        printf(" %d",alloc[i][j]);
        }
        printf("\n");
    }
        printf("Max Matrix \n");
        for(i=0;i<n;i++){
        for(j=0; j<m; j++){
        printf(" %d",max[i][j]);
        }
        printf("\n");
    }
    printf("Need Matrix \n");
        for(i=0;i<n;i++){
        for(j=0; j<m; j++){
        printf(" %d",need[i][j]);
        }
        printf("\n");
    }
	int y = 0;
	for (k = 0; k < 5; k++) {
		for (i = 0; i < n; i++) {
			if (f[i] == 0) {

				int flag = 0;
				for (j = 0; j < m; j++) {
					if (need[i][j] > avail[j]){
						flag = 1;
						break;
					}
				}

				if (flag == 0) {
					ans[ind++] = i;
					for (y = 0; y < m; y++)
						avail[y] += alloc[i][y];
					f[i] = 1;
				}
			}
		}
	}
    int flag = 1;
	for(int i=0;i<n;i++)
	{
	if(f[i]==0)
	{
		flag=0;
		printf("SYSTEM NOT SAFE");
		break;
	}
	}
	if(flag==1)
	{
	printf("SYSTEM SAFE\n");
	for (i = 0; i < n - 1; i++)
		printf(" P%d ->", ans[i]);
	printf(" P%d", ans[n - 1]);
	}
	return (0);
}

#37 deadlock Algo

#include <stdio.h>
#include <stdlib.h>
static int mark[20];
int i, j, np, nr;

int main()
{
    int **alloc, **request, *avail, *r, *f;

    printf("\nEnter the no of the process: ");
    scanf("%d", &np);

    printf("\nEnter the no of resources: ");
    scanf("%d", &nr);

    alloc = (int **)malloc(np * sizeof(int *));
    request = (int **)malloc(np * sizeof(int *));
    avail = (int *)malloc(nr * sizeof(int));
    r = (int *)malloc(nr * sizeof(int));
    f = (int *)malloc(np * sizeof(int));

    for(i = 0;i < np;i ++)
        f[i] = 0;

    for (i = 0; i < nr; i++)
    {
        printf("\nTotal Amount of the Resource R % d: ", i + 1);
        scanf("%d", &r[i]);
    }

    printf("\nEnter the request matrix:");
    for (i = 0; i < np; i++)
    {
        request[i] = (int *)malloc(nr * sizeof(int));
        for (j = 0; j < nr; j++)
        {
            scanf("%d", &request[i][j]);
        }
    }

    printf("\nEnter the allocation matrix:");
    for (i = 0; i < np; i++)
    {
        alloc[i] = (int *)malloc(nr * sizeof(int));
        for (j = 0; j < nr; j++)
        {
            scanf("%d", &alloc[i][j]);
        }
    }

    for (j = 0; j < nr; j++)
    {
        avail[j] = r[j];
        for (i = 0; i < np; i++)
        {
            avail[j] -= alloc[i][j];
        }
    }

    for (i = 0; i < nr; i++)
    {
        printf("Resc %d :: %d", i, avail[i]);
    }

    while (1)
    {   
        int deadlock = 1;
        for (i = 0; i < np ;i++){
            int canBeProc = 1;
            for (j = 0; j < nr; j++){
                if (request[i][j] > avail[j]){
                    canBeProc = 0;
                }
            }
            if (canBeProc){
                deadlock = 0;
                f[i] = 1;
                for (j = 0; j < nr; j++){
                    avail[j] += alloc[i][j];
                }
            }
        }
        if (deadlock){
            printf("\n Deadlock detected");
            break;
        }
        int completed = 1;
        for (i = 0; i < np ; i++){
            if (f[i] == 0){
                completed = 0;
            }
        }
        if (completed){
            printf("\n Deadlock not detected");
            break;
        }
    }
}


#38 Placement Strategies 

#include <stdio.h>

void firstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];

    for (int i = 0; i < n; i++) {
        allocation[i] = -1;

        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }

    printf("\nFirst Fit Allocation:\n");
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

void worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];

    for (int i = 0; i < n; i++) {
        allocation[i] = -1;

        int worstIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx]) {
                    worstIdx = j;
                }
            }
        }

        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
        }
    }

    printf("\nWorst Fit Allocation:\n");
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

void bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];

    for (int i = 0; i < n; i++) {
        allocation[i] = -1;

        int bestIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx]) {
                    bestIdx = j;
                }
            }
        }

        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }

    printf("\nBest Fit Allocation:\n");
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

void nextFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    int lastBlock = 0;

    for (int i = 0; i < n; i++) {
        allocation[i] = -1;

        for (int j = lastBlock; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                lastBlock = j;
                break;
            }
        }

        if (allocation[i] == -1) {
            for (int j = 0; j < lastBlock; j++) {
                if (blockSize[j] >= processSize[i]) {
                    allocation[i] = j;
                    blockSize[j] -= processSize[i];
                    lastBlock = j;
                    break;
                }
            }
        }
    }

    printf("\nNext Fit Allocation:\n");
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

int main() {
    int blockSize[] = {100, 500, 200, 300, 600};
    int processSize[] = {212, 417, 112, 426};
    int m = sizeof(blockSize) / sizeof(blockSize[0]);
    int n = sizeof(processSize) / sizeof(processSize[0]);

    firstFit(blockSize, m, processSize, n);
    worstFit(blockSize, m, processSize, n);
    bestFit(blockSize, m, processSize, n);
    nextFit(blockSize, m, processSize, n);

    return 0;
}

#39 FIFO and optimal

#include <stdio.h>
#include <stdlib.h>

// Function to check if a page is present in the page frames
int isInFrame(int page, int frame[], int frameSize) {
    for (int i = 0; i < frameSize; i++) {
        if (frame[i] == page) {
            return 1; // Page found in frame
        }
    }
    return 0; // Page not found in frame
}

// Function to find the index of the oldest page in the page frames
int findOldestPage(int frame[], int frameSize, int referenceString[], int referenceStringLength, int currentPosition) {
    int oldestIndex = -1;
    int maxIndex = -1;
    for (int i = 0; i < frameSize; i++) {
        int j;
        for (j = currentPosition; j < referenceStringLength; j++) {
            if (frame[i] == referenceString[j]) {
                if (j > maxIndex) {
                    maxIndex = j;
                    oldestIndex = i;
                }
                break;
            }
        }
        if (j == referenceStringLength) {
            return i; // Page not referenced again
        }
    }
    return oldestIndex;
}

// Function to calculate total page faults using FIFO algorithm
int fifo(int referenceString[], int referenceStringLength, int frameSize) {
    int frame[frameSize];
    int pageFaults = 0;

    // Initialize all frame entries to -1
    for (int i = 0; i < frameSize; i++) {
        frame[i] = -1;
    }

    int currentPosition = 0; // Current position in the reference string

    // Iterate through the reference string
    for (int i = 0; i < referenceStringLength; i++) {
        if (!isInFrame(referenceString[i], frame, frameSize)) {
            // Page fault
            pageFaults++;
            frame[currentPosition] = referenceString[i];
            currentPosition = (currentPosition + 1) % frameSize; // Move to the next position in frame
        }
    }

    return pageFaults;
}

int main() {
    int referenceString[] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int referenceStringLength = sizeof(referenceString) / sizeof(referenceString[0]);
    int frameSize = 3;

    printf("Total number of page faults using FIFO: %d\n", fifo(referenceString, referenceStringLength, frameSize));

    return 0;
}
-------------
#include <stdio.h>
#include <stdlib.h>

// Function to check if a page is present in the page frames
int isInFrame(int page, int frame[], int frameSize) {
    for (int i = 0; i < frameSize; i++) {
        if (frame[i] == page) {
            return 1; // Page found in frame
        }
    }
    return 0; // Page not found in frame
}

// Function to find the index of the page that will not be referenced for the longest time
int findOptimalPage(int frame[], int frameSize, int referenceString[], int referenceStringLength, int currentPosition) {
    int optimalIndex = -1;
    int maxDistance = -1;
    for (int i = 0; i < frameSize; i++) {
        int j;
        for (j = currentPosition; j < referenceStringLength; j++) {
            if (frame[i] == referenceString[j]) {
                if (j > maxDistance) {
                    maxDistance = j;
                    optimalIndex = i;
                }
                break;
            }
        }
        if (j == referenceStringLength) {
            return i; // Page not referenced again
        }
    }
    return optimalIndex;
}

// Function to calculate total page faults using Optimal algorithm
int optimal(int referenceString[], int referenceStringLength, int frameSize) {
    int frame[frameSize];
    int pageFaults = 0;

    // Initialize all frame entries to -1
    for (int i = 0; i < frameSize; i++) {
        frame[i] = -1;
    }

    // Iterate through the reference string
    for (int i = 0; i < referenceStringLength; i++) {
        if (!isInFrame(referenceString[i], frame, frameSize)) {
            // Page fault
            pageFaults++;
            int replaceIndex = findOptimalPage(frame, frameSize, referenceString, referenceStringLength, i);
            frame[replaceIndex] = referenceString[i];
        }
    }

    return pageFaults;
}

int main() {
    int referenceString[] = {7, 1, 0, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int referenceStringLength = sizeof(referenceString) / sizeof(referenceString[0]);
    int frameSize = 3;

    printf("Total number of page faults using Optimal: %d\n", optimal(referenceString, referenceStringLength, frameSize));

    return 0;
}


#40 SJF And SRTF

#include <stdio.h>

struct Process {
    int p_id;
    int bt;
    int at; // Arrival time
    int wt; 
    int tat; 
    int completed;
};

int findShortestJob(struct Process processes[], int n, int currentTime) {
    int shortestJobIndex = -1;
    int shortestBurst = 9999;
    
    for (int i = 0; i < n; ++i) {
        if (!processes[i].completed && processes[i].bt < shortestBurst && processes[i].at <= currentTime && processes[i].bt > 0) {
            shortestBurst = processes[i].bt;
            shortestJobIndex = i;
        }
    }
    
    return shortestJobIndex;
}

void calculateTimes(struct Process processes[], int n) {
    int currentTime = 0;
    int totalBurst = 0;
    
    // Calculate total burst time
    for (int i = 0; i < n; ++i) {
        totalBurst += processes[i].bt;
    }
    
    printf("Gantt Chart:\n");
    printf("|");
    
    while (currentTime < totalBurst) {
        int shortestJobIndex = findShortestJob(processes, n, currentTime);
        if (shortestJobIndex == -1) {
            currentTime++;
            continue;
        }
        
        printf(" P%d(%d) |", processes[shortestJobIndex].p_id, processes[shortestJobIndex].bt);
        
        processes[shortestJobIndex].bt--;
        
        for (int i = 0; i < n; ++i) {
            if (i != shortestJobIndex && !processes[i].completed && processes[i].at <= currentTime) {
                processes[i].wt++;
            }
        }
        
        currentTime++;
        
        if (processes[shortestJobIndex].bt == 0) {
            processes[shortestJobIndex].completed = 1;
            processes[shortestJobIndex].tat = currentTime;
        }
    }
    printf("\n");
}

void calculateAverages(struct Process processes[], int n, float *avg_waiting_time, float *avg_turnaround_time) {
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    
    for (int i = 0; i < n; ++i) {
        total_waiting_time += processes[i].wt;
        total_turnaround_time += processes[i].tat;
    }
    
    *avg_waiting_time = (float)total_waiting_time / n;
    *avg_turnaround_time = (float)total_turnaround_time / n;
}

void displayProcesses(struct Process processes[], int n) {
    printf("Process ID\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; ++i) {
        printf("%d\t\t%d\t\t%d\n", processes[i].p_id, processes[i].wt, processes[i].tat);
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    struct Process processes[n];
    
    for (int i = 0; i < n; ++i) {
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].bt);
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].at);
        processes[i].p_id = i + 1;
        processes[i].wt = 0;
        processes[i].tat = 0;
        processes[i].completed = 0;
    }
    
    calculateTimes(processes, n);
    
    float avg_waiting_time, avg_turnaround_time;
    calculateAverages(processes, n, &avg_waiting_time, &avg_turnaround_time);
    
    displayProcesses(processes, n);
    
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    
    return 0;
}

-----------------

#include <stdio.h>
#include <limits.h>

struct Process {
    int pid;
    int bt;
    int art;
};

void findWaitingTime(struct Process proc[], int n, int wt[]) {
    int rt[n];
    for (int i = 0; i < n; i++)
        rt[i] = proc[i].bt;

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    int check = 0;

    while (complete != n) {
        for (int j = 0; j < n; j++) {
            if (proc[j].art <= t && rt[j] < minm && rt[j] > 0) {
                minm = rt[j];
                shortest = j;
                check = 1;
            }
        }

        if (check == 0) {
            t++;
            continue;
        }

        rt[shortest]--;

        minm = rt[shortest];
        if (minm == 0)
            minm = INT_MAX;

        if (rt[shortest] == 0) {
            complete++;
            check = 0;

            finish_time = t + 1;

            wt[shortest] = finish_time - proc[shortest].bt - proc[shortest].art;

            if (wt[shortest] < 0)
                wt[shortest] = 0;
        }
        t++;
    }
}

void findTurnAroundTime(struct Process proc[], int n, int wt[], int tat[]) {
    for (int i = 0; i < n; i++)
        tat[i] = proc[i].bt + wt[i];
}

void findavgTime(struct Process proc[], int n) {
    int wt[n], tat[n];
    int total_wt = 0, total_tat = 0;

    findWaitingTime(proc, n, wt);
    findTurnAroundTime(proc, n, wt, tat);

    printf("Processes\tBurst time\tWaiting time\tTurn around time\n");

    for (int i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        printf(" %d\t\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].bt, wt[i], tat[i]);
    }

    printf("Average waiting time = %.2f\n", (float)total_wt / (float)n);
    printf("Average turn around time = %.2f\n", (float)total_tat / (float)n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process proc[n];

    for (int i = 0; i < n; i++) {
        printf("Enter details for process %d (PID, Burst Time, Arrival Time): ", i + 1);
        scanf("%d %d %d", &proc[i].pid, &proc[i].bt, &proc[i].art);
    }

    findavgTime(proc, n);
    return 0;
}

