# If you want MPE to log MPI calls, you have to add the two lines
# below at the very beginning of your main bootstrap script.
import mpi4py.rc
mpi4py.rc.profile('MPE', logfile='cpilog')

# Import the MPI extension module
from mpi4py import MPI
if 0: # <- use '1' to disable logging of MPI calls
    MPI.Pcontrol(0)

# Import the MPE extension module
from mpi4py import MPE
if 1: # <- use '0' to disable user-defined logging
    # This has to be explicitly called !
    MPE.initLog(logfile='cpilog')
    # Set the log file name (note: no extension)
    MPE.setLogFileName('cpilog')

import numpy
from numpy import random

# User-defined MPE events
cpi_begin = MPE.newLogEvent("ComputePi-Begin", "yellow")
cpi_end   = MPE.newLogEvent("ComputePi-End",   "pink")
# User-defined MPE states
synchronization = MPE.newLogState("Synchronize", "orange")
communication   = MPE.newLogState("Comunicate",  "red")
computation     = MPE.newLogState("Compute",     "green")


cpi_begin
comm=MPI.COMM_WORLD
rank=comm.Get_rank()
size=comm.Get_size()

if(rank==0):
    v=random.random((1000,4801))
    if size>=len(v):
        size=len(v)
    slice_size=int(numpy.ceil(float(len(v))/float(size)))
    slice_for_last_node=len(v)-(size-1)*slice_size      
    #xtra_slices=len(v)%size

    rows=len(v)
    print "slice_size:",slice_size
    print "slice_for_last_node:",slice_for_last_node
else:
    slice_size=slice_for_last_node=rows=None

size=comm.bcast(size,root=0)
slice_size=comm.bcast(slice_size,root=0)
slice_for_last_node=comm.bcast(slice_for_last_node,root=0)
rows=comm.bcast(rows,root=0)

def doSum(x):
        return numpy.sum(x)

if rank==0:

        print 'thats v_random:\n', v

    	count=1
    	cur_dest=0

        for i in range(len(v)): 
        	if(count>slice_size and cur_dest<size-1):
            		cur_dest+=1
            		count=1
        	if(cur_dest>=size-1):
            		cur_dest=size-1

            	comm.send(v[i],dest=cur_dest)
        	count+=1

if rank<size-1:
    count=1
    while count<=slice_size: #slices per proc
        data=comm.recv(source=0)
        count+=1
        print 'my rank is {0} and my output is {1}\n'.format(rank,doSum(data))


elif rank==size-1:
    count=1
    while count<=slice_for_last_node:
    	data=comm.recv(source=0)
        count+=1
        print 'my rank is {0} and my output is {1}\n'.format(rank,doSum(data))

