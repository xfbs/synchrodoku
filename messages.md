# Messages

I need to define what the messages that get passed between
the different threads look like.


## Messages between manager and worker thread

The manager sends the worker threads sudokus (or
partial solutions thereof), and they reply with
either a solution, or with a partial solution.

### Manager sends worker thread sudoku

When the manager sends a worker thread a sudoku,
it must also send an ID along with it so it can
later identify the solution. 

```
{
    "type": "task"
    "sudoku": [12, 23, 45, ...],
    "id": 5
}
```

### Manager sends worker thread shutdown message

When the manager sends a worker thread a shutdown
message, it must shut down as soon as possible.

```
{
    "type": "shutdown"
}
```

### Worker thread sends manager solution

When a worker thread found a solution for
a sudoku, it notifies the manager by sending
the solution along with the ID of the sudoku.

```
{
    "done": true,
    "data": [12, 45, 23, ...],
    "id": 5
}
```

### Worker thread sends manager partial solutions

When a worker thread has to diverge, it sends
all the partial solutions back to the manager
along with the ID of the sudoku it's working
on.

```
{
    "done": false,
    "divs": [
        [12, 45, 23, ...],
        [67, 23, 12, ...],
        ...
    ],
    "id": 5
}
```

