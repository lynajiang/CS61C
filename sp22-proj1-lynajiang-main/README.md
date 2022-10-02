# 61C Spring 2022 Project 1: Snake

Spec: [https://cs61c.org/sp22/projects/proj1/](https://cs61c.org/sp22/projects/proj1/)

TODO: describe what you did
Task 1: I implemented the board by character instead of string. Looking back, it would have been easier to hardcode it. However, I was able to quickly implement what I had from task 1 to task 5.

Task 2: It was straightforward to free everything that I had malloc/calloced.

Task 3: Also straightforward to print out things.

Task 4: The helper functions were a big help in the later functions. The update_tail was definitely the hardest out of the task to implement because of the details needed to be coded.

Task 5: I copied all allocations from task 1. I read the file to garner all information about the size of the rectangle. I then created the board. I read the file a second time to get all the inside components (snake, fruit information).

Task 6: Using information given about the tail coordinates, I looped through until the next square of the snake is empty.

Task 7: Called all functions in the main to create 