#!/bin/bash
  #PBS -l nodes=4:ppn=32
  #PBS -l walltime=00:10:00
  #PBS -N notakto
  #PBS -q debug_cpu

  aprun -n 4 -N 4 -d 8 search -ram 7 -threads 7 -n 5 -dir test/