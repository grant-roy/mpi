#include "mpi.h"

int main (argc,argv[]) {

 static int const MAX_ROWS = 1000;
 static int const MAX_COLS = 1000;
 #define MIN(x, y) (((x) < (y)) ? (x) : (y))

//define array of size MAX_ROWS x MAX_COLUMNS
    double a[MAX_ROWS][MAX_COLS];
    double b[MAX_COLS]; //vector of size: max columns
    double c[MAX_ROWS]; //vector of size: max rows

    int myid,      //myid: MPI rank of the process
        manager,   //manager: is this process the manager
        numprocs,  //numprocs: number of processes in the MPI_COMM_WORLD
        ierr,      //ierr: any error generated by MPI function calls
        status(MPI_STATUS_SIZE);


    int i,          //i: loop variable
        j,          //j: loop variable
        numsent,    //numsent:
        sender;     //sender:
        anstype,    //anstype:
        row;        //row:

 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
 MPI_Comm_rank(MPI_COMM_WORLD, &myid);

 manager = 0;
 rows = 100;
 cols = 100;

 /* manager part */
 if (myid == manager) {

     /* here we are simply initializing a and b */
     for (j = 0; j < cols ; j++){

         b[j] = 1;

         for (i = 0; i < rows; i++){

             a[i,j] = i;
         }
     }
     numsent = 0;

     /* send b to each worker process */
     MPI_Bcast(b,cols,MPI_DOUBLE_PRECISION, manager, MPI_COMM_WORLD);

     int halt = MIN(numprocs,rows);

     /* send a row to each worker process; tag with row number */
     for ( i = 0; i < halt ; i++ ){
        for( j = 0; j < cols; j++ ){
            buffer[j] = a[i][j];
        }

        MPI_Send(buffer,cols,MPI_DOUBLE_PRECISION,i, &i, MPI_COMM_WORLD);
        numsent = numsent + 1;
     }

     for ( i = 0; i < rows; i++ ){

         MPI_Recv(ans,1,MPI_DOUBLE_PRECISION,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,status);

         sender = status(MPI_SOURCE);
         anstype = status(MPI_TAG);
         c[anstype] = ans;

         if( numsent < rows){

             for( j = 0; j < cols; j++){
                 buffer[j] = a[numsent+1][j];
             }

             MPI_Send(buffer,cols,MPI_DOUBLE_PRECISION,sender,numsent+1,MPI_COMM_WORLD);
             numsent = numsent + 1;
         }
         else{
             MPI_Send(MPI_BOTTOM,0,MPI_DOUBLE_PRECISION,sender,0,MPI_COMM_WORLD);
         }
     }
 }
 /* worker part-- workers receive b, then compute dot products until done message is received */
 else{
     MPI_Bcast(b,cols,MPI_DOUBLE_PRECISION,manager,MPI_COMM_WORLD);

     /* skip if there are more processes than work, since myid starts at zero and
      * rows starts at 1, we need to do rows-1......maybe not */
     if(myid <= rows-1){

         while(1){
             MPI_Recv(buffer,cols,MPI_DOUBLE_PRECISION,manager,MPI_ANY_TAG,MPI_COMM_WORLD,status);

             if(status(MPI_TAG) == 0)
                 return;

             row = status(MPI_TAG);
             ans = 0.0;

             for(i = 0; i < cols; i++){
                 ans = ans+ buffer[i] * b[i];
             }

             MPI_Send(ans,1,MPI_DOUBLE_PRECISION,manager,row,MPI_COMM_WORLD);
         }
     }
 }

 MPI_Finalize();
 return 0;
}
