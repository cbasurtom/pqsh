# pqsh

pqsh is a C homebrewed operative system scheduler originally designed as a project for my [Operating Systems](https://www3.nd.edu/~pbui/teaching/cse.30341.fa24/) course. Its main purpose was to create a multiqueue scheduler with the capability of employing different scheduling policies such as FIFO (First In First Out) or Round Robin, multi-core concurrency and measuring different scheduling metrics such as turnaround time and response time. This project was extraordinarily useful to comprehending the true nature of operating systems and is only the beginning of my fascination.

## Usage

```python
'''Usage: ./bin/pqsh [options]

Options:
    -n CORES           Number of CPU cores to utilize
    -p POLICY          Scheduling policy (fifo, rdrn)
    -t MICROSECONDS    Timer interrupt interval
    -h                 Print this help message

   Commands:
      add    command    Add command to waiting queue.
      status [queue]    Display status of specified queue (default is all).
      help              Display help message.
      exit|quit         Exit shell.'''
```

## Licence

[MIT](https://choosealicense.com/licenses/mit/)
