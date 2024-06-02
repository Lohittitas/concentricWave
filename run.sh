rm a.out
#~ rm ./data/*
#~ rm ./vtkfiles/*
#~ rm ./dump/*

CC99='mpicc -std=c99' qcc -Wall -O2 -grid=quadtree -D_MPI=1 cavity.c -lm


mpirun -np 4 ./a.out
