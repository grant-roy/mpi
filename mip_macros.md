## MPI INIT

The call to MPI_INIT is required in every MPI program and must be the first MPI
call. It establishes the MPI “environment.” Only one invocation of MPI_INIT
can occur in each program execution. Its only argument is an error code. It will return MPI_SUCCESS if the call 
succeeds, or failure, if the call fails. 

## MPI_COMM_WORLD

The Default context, defines one context and the set of all processes.  

## MPI_COMM_SIZE

MPI_COMM_SIZE returns (in &numprocs) the number of processes that
the user has started for this program. Precisely how the user caused these processes
to be started depends on the implementation, but any program can find out the
number with this call.

## MPI_COMM_RANK

By calling MPI_COMM_RANK, each process finds out its rank
in the group associated with a communicator. Thus, although each process in this
program will get the same number in numprocs, each will have a different number
for myid.

## MPI_BCAST

* Question - What are the five parameters passed to MPI_BCAST??

Note that all processes call MPI_BCAST,
both the process sending the data (with rank zero) and all of the other processes
MPI_COMM_WORLD.

The call to MPI_BCAST results in every process (in the group associated with the communicator given in the fifth argument) ending up
with a copy of n.  The data to be communicated is described by the address (n),
the datatype (MPI_INTEGER), and the number of items (1). The process with
the original copy is specified by the fourth argument (0 in this case, the manager
process, which just reads it from the user). (MPI assigns a type to every data item.
MPI datatypes are described in full in Section 5.1.)
Thus, after the call to MPI_BCAST, all processes have n and their own identifiers,
which is enough information for each one to compute its contribution, mypi. Each
process computes the area of every numprocs’th rectangle, starting with myid+1.
Next, all of the values of mypi held by the individual processes need to be added up.
 
                                                       
## MPI_REDUCE                                                       
                                                      
                                                       



