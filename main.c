#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
HANDLE hsem1,hsem2;
long int count=0,sum=0;
HANDLE ht1,ht2,ht3;

DWORD WINAPI Counter(LPVOID param)
{
    long int i=0;
	while (count<1000000)
	{
        WaitForSingleObject(hsem1,INFINITE);
        if (count==1000000) goto L;
	 count++;
	 sum=sum+rand()%100;
	 i++;
      L :  ReleaseSemaphore(hsem1,1,NULL);
	}
	  printf("Thread %d generated %d numbers\n",GetCurrentThreadId(),i);
	  ReleaseSemaphore(hsem2,1,NULL);
}

int main() {
    LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER t1, t2;           // ticks
    double elapsedTime;

    // get ticks per second
    QueryPerformanceFrequency(&frequency);

    // start timer
    QueryPerformanceCounter(&t1);

	DWORD ThreadID1,ThreadID2,ThreadID3;
	char c,Sem1,Sem2;
    hsem1 = CreateSemaphore(NULL,1,1,&Sem1);
    hsem2 = CreateSemaphore(NULL,0,1,&Sem2);
	srand (time(NULL));

	ht1 = CreateThread(NULL,0,Counter,NULL,CREATE_SUSPENDED,&ThreadID1);
    ht2 = CreateThread(NULL,0,Counter,NULL,CREATE_SUSPENDED,&ThreadID2);
    ht3 = CreateThread(NULL,0,Counter,NULL,CREATE_SUSPENDED,&ThreadID3);

    SetThreadAffinityMask(ht1,1);
    SetThreadAffinityMask(ht2,2);
    SetThreadAffinityMask(ht3,3);

    SetThreadPriority(ht1,THREAD_PRIORITY_TIME_CRITICAL);
    SetThreadPriority(ht2,THREAD_PRIORITY_IDLE);
    SetThreadPriority(ht3,THREAD_PRIORITY_IDLE);

	printf("Thread 1 id is %d \n",ThreadID1);
  	printf("Thread 2 id is %d \n",ThreadID2);
	printf("Thread 3 id is %d \n",ThreadID3);



	ResumeThread(ht1);
	ResumeThread(ht2);
	ResumeThread(ht3);


    WaitForSingleObject(hsem2,INFINITE);
    WaitForSingleObject(hsem2,INFINITE);
    WaitForSingleObject(hsem2,INFINITE);
    printf("Count reached %d \n",count);
    printf("Sum reached %d \n",sum);
	printf("Average is %f \n",(float)sum/(float)count);


    QueryPerformanceCounter(&t2);
	elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
    printf ("Time=%f ms.\n",elapsedTime);

	while(c != 'e') {c = getche();}

	return 0;
}
