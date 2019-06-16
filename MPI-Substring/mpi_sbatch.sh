# Paula Mendez
# Running MPI programs under Slurm
module load OpenMPI

#Compile code
Makefile

!/bin/bash -l
 -l h_rt=0:01:00             # ask for 1 hour runtime
SBATCH --constraint=dwarves     # only run on dwarves


mpirun /homes/paulipotter/MPI-Substring-Project/test #change to match the path to your code

#Submit your MPI job to the scheduler. In this case asking for 4 cores on 1 node.
sbatch --ntasks-per-node=4 --nodes=1 mpi_sbatch.sh
