/* include main */
#include	"unpipc.h"

#define	MAXNTHREADS	100
#define	NAME	"incr_pxsem2"

int		nloop;

struct {
  sem_t	*mutex;		/* pointer to the named semaphore */
  long	counter;
} shared;

void	*incr(void *);

int
main(int argc, char **argv)
{
	int		i, nthreads;
	pthread_t	tid[MAXNTHREADS];

	if (argc != 3)
		err_quit("usage: incr_pxsem2 <#loops> <#threads>");
	nloop = atoi(argv[1]);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);

		/* 4initialize named semaphore to 0 */
	sem_unlink(Px_ipc_name(NAME));		/* error OK */
	shared.mutex = Sem_open(Px_ipc_name(NAME), O_CREAT | O_EXCL, FILE_MODE, 0);

		/* 4create all the threads */
	Set_concurrency(nthreads);
	for (i = 0; i < nthreads; i++) {
		Pthread_create(&tid[i], NULL, incr, NULL);
	}
		/* 4start the timer and release the semaphore */
	Start_time();
	Sem_post(shared.mutex);

		/* 4wait for all the threads */
	for (i = 0; i < nthreads; i++) {
		Pthread_join(tid[i], NULL);
	}
	printf("microseconds: %.0f usec\n", Stop_time());
	if (shared.counter != nloop * nthreads)
		printf("error: counter = %ld\n", shared.counter);
	Sem_unlink(Px_ipc_name(NAME));

	exit(0);
}
/* end main */

/* include incr */
void *
incr(void *arg)
{
	int		i;

	for (i = 0; i < nloop; i++) {
		Sem_wait(shared.mutex);
		shared.counter++;
		Sem_post(shared.mutex);
	}
	return(NULL);
}
/* end incr */
