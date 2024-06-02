# concentricWave
Code for concentrc wave focussing

## Ensure proper installation of Basilisk, Julia and Python in your PC

## Download the code repository by following command in your terminal
- `git clone https://github.com/Lohittitas/concentricWave`

## Running CFD simulation in Basilisk
- `cd concentricWave`
- `sh run.sh` will run this simulation on 4 cores of CPU. One may change number of cores. This should also generate the necessary dump files.

## Necessary dependencies in Julia for solving ODEs (equation 2.6) in paper.
- Invoke `julia` through terminal
- Enter package mode through `]` key
- `add DifferentialEquations Plots FastGaussQuadrature Trapz Bessels HDF5 QuadGK Pluto` This command is a one-time use and should take some time.
- `Pluto.run()` Opens Pluto-notebook to run `odeSolver.jl` file
- Open `odeSolver.jl` and run the code to solve ODEs. This file requires coefficient matrices which are saved in the `hd5_newdir` folder provided.

## Necessary dependencies in Python for inverse Laplace transform 
- Install `mpmath` and `scipy` module(check their documentation)
- Run `saveData.py` to generate the results from theory by solving equation 3.2 in paper by numerical inverse Laplace transform and to save the interface data.
