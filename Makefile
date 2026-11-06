campuscompute : main.o job.o scheduler.o banker.o worker.o resource_limits.o logger.o ipc.o
	gcc main.o job.o scheduler.o banker.o worker.o resource_limits.o logger.o ipc.o -o campuscompute -pthread -lrt

main.o : source/main.c source/job.h source/scheduler.h source/banker.h source/worker.h source/resource_limits.h source/logger.h source/ipc.h
	gcc -Wall -c source/main.c -o main.o

job.o : source/job.c source/job.h
	gcc -Wall -c source/job.c -o job.o

scheduler.o : source/scheduler.c source/scheduler.h source/job.h
	gcc -Wall -c source/scheduler.c -o scheduler.o

banker.o : source/banker.c source/banker.h
	gcc -Wall -c source/banker.c -o banker.o

worker.o : source/worker.c source/worker.h
	gcc -Wall -c source/worker.c -o worker.o

resource_limits.o : source/resource_limits.c source/resource_limits.h
	gcc -Wall -c source/resource_limits.c -o resource_limits.o

logger.o : source/logger.c source/logger.h
	gcc -Wall -pthread -c source/logger.c -o logger.o

ipc.o : source/ipc.c source/ipc.h
	gcc -Wall -c source/ipc.c -o ipc.o

clean:
	rm -f *.o campuscompute