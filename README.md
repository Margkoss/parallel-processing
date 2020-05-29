# parallel-processing

School project on parallel processing utilizing OpenMP library.

---

## Install

If you don't already have `gcc` run:

```bash
sudo apt install gcc
```

and then `make serial` or `make parallel` for each use case.

## Usage

Pass the `matrix market` formatted file that represents an Directed Acyclic Graph (DAG) as a command line arguement. _Watch out_ because only the `large.mtx` is both directed and acyclic.

---

## How it works

The programm takes a file as input, parses and creates an [Adjacency Matrix](https://en.wikipedia.org/wiki/Adjacency_matrix). Then Kahn's algorithm is run on the adjacency matrix to topologically sort the graph. As the sort it self isn't the main focus of the excercise, there is _*no*_ output. The programms timings appear on standard output.
