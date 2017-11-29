  stata-reader-writer
=======================

Program to read and write Stata files

What I am working on is a library that will let me read and write Stata files to and from SQLite files.
All the solutions I have seen usually use just CSV files for import and output. Unfortunately you lose a lot 
of the richness that is provided by a full Stata dataset.

Reads format 117/118/119 (Stata 13, 14 and 15) and extracts the metadata. Only works on LSB datasets.

Writing output files is not started yet, I will focus on SQLite first.

Program options:

```bash

stata-reader-writer$ ./stata_readwrite
Usage: stata_readwrite [options] <stata file>
Allowed options:

General options:
  --help                          produce help message
  --stata-file arg                stata input file

Database options:
  -d [ --db-type ] arg            select database type [sqlite]
  -c [ --connection-string ] arg  connection string
```

Checking the metadata of a file:

```bash

stata-reader-writer$ ./stata_readwrite --stata-file myhousehold118.dta
Information on Stata file: myhousehold118.dta
[Variables]: 135
[Observations]: 1
[Time Stamp]:  4 Oct 2017 10:50
[Data Label]:
[Release]: 119
[Bitness]: Little Endian
```