#include "mpi.h"
#include <math.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int n, myid, numprocs, i;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;

    /* Called at the start of every MPI program */
    MPI_Init(&argc,&argv);

    /* This will determine the number of processes in the 'MPI_COMM_WORLD', which is
       the number of processes participating in the computation */
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    /* This will determine the indivdual processes 'Rank' or process number */
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    while (1) {
        if (myid == 0) {
            printf("Enter the number of intervals: (0 quits) ");
            scanf("%d",&n);
        }

        /* After MPI_BCast is called, each process has a copy of 'n'
          
           - MPI_Bcast(the data to send, the length/num of pieces of data, the type of the data, rank of the 
           controller process, the communication group for this calculation)
        */
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        if (n == 0)
            break;
        else {
            h   = 1.0 / (double) n;
            sum = 0.0;
            for (i = myid + 1; i <= n; i += numprocs) {
                x = h * ((double)i - 0.5);
                sum += (4.0 / (1.0 + x*x));
            }
            mypi = h * sum;

            /* After MPI_Reduce() is called, each processes's contribution has been aggregated and summed 
            
              - MPI_Reduce(a single processes contribution, the result,length of data, type of data,
              aggregate reduction operation to be performed, the communication group for this calculation)
            */
            MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0,
                       MPI_COMM_WORLD);
            if (myid == 0)
                printf("pi is approximately %.16f, Error is %.16f\n",
                       pi, fabs(pi - PI25DT));
        }
    }

    /* Called at the end of every MPI Program  */
    MPI_Finalize();
    return 0;
}
