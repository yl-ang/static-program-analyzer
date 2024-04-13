# Team 9 System Testing

## Contribute to system testing

If you want to create system tests, you will need to create a source file, a query file. Suppose if you want to create
for this milestone (i.e milestone2) and there are 2 existing source files in the milestone2 folder, please name your 
files as "milestone2_source_3.txt" and "milestone2_queries_3.txt".

### Running System Tests

- Note this will automate the running of all system test files, if you want to debug failed specific test (Windows), 
please use visual Studio debugger breakpoint, and configure launch.vs.json properly.

### Windows

1. Open your shell in IDE or stand-alone terminal , ensure you are in the Tests09 directory.
2. Run "run_autotester.bat" file in the Tests09 directory. This will run all the system tests files in the 3 milestone
folders.
3. Run "run_localhost.bat" file  in the Tests09 directory. This start 3 localhost servers at ports 8001, 8002, and 8003.
4. Open your browser and goto each localhost endpoints to view output (http://localhost:8001/, http://localhost:8002/, 
http://localhost:8003/)

### Mac

1. Open your shell in IDE or stand-alone terminal , ensure you are in the Tests09 directory.
2. Run "run_autotester.sh" file in the Tests09 directory. This will run all the system tests files in the 3 milestone
   folders.
3. Run "run_localhost.sh" file  in the Tests09 directory. This start 3 localhost servers at ports 8001, 8002, and 8003.
4. Open your browser and goto each localhost endpoints to view output (http://localhost:8001/, http://localhost:8002/,
   http://localhost:8003/)


### Folders
- tests_dev contains tests in development (i.e queries)

### Potential Issues
1. Mac users, if autotester binary not found, please locate the autotester binary (copy the path), and 
update the AUTOTESTER_BINARY variable in "run_autotester.sh" file to point to the correct path.