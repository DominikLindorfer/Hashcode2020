# Solution to Hashcode2020 Qualifying Round

**Individual Scores:**

A - 21 

B - 5,822,900

C - 5,689,822

D - 5,037,435

E - 5,071,740

F - 5,340,296

**Final Score**: ***26,962,214***

## Solution Approach:

We define a time-difference 

    td = days - library signup_time 

and calculate a library score by
    
    while(book_id <  td * book_per_day of library  &&  book_id < number of books in library)
    {
        score += score for book with book_id
        book_id++
    }
and the score function

    score /= library signup_time

gives us the library score.

The library with the highest score is added to the solution and we erase each book of the selected library which have not yet been added to the solution. All highest scores have been achieved by varying the score function, e.g. to 

    score = score * score / library signup_time
    
 and by switching score functions after a given number of days.
