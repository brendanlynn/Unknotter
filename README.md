# Unknotter
This program was designed by computer scientist Brendan Lynn as an algorithm to solve the [Unknotting problem](https://en.wikipedia.org/wiki/Unknotting_problem): determining whether or not a given mathematical knot is the unknot. Bear in mind that this algorithm is based on an evidence-supported but nonetheless unproven hypothesis.

> [!IMPORTANT]\
> Due to unexpected complications in the implementation of the algorithm this repository is based on, a new algorithm had been hypothesised. This new algorithm is being developed in [Unknotter2](https://github.com/brendanlynn/Unknotter2/). Thus, this repository and its compagnion, [Unknotter: Preformatted Knots](https://github.com/brendanlynn/Unknotter_PreformattedKnots), have been archived.

## Usage
Knots must be specified in a base-0, absolute, paired version of Dowker-Thistlethwaite notation, with the index crossing over on the left and the index crossing under on the right. For instance, the trefoil knot could be notated as $`(0, 3), (4, 1), (2, 5)`$.

Entering this into the console application is slightly different. The parentheses and commas are removed. To indicate the end of the set, type "end", case-sensitive, exactally like that. For instance, the trefoil knot could be notated as such (the application adding the `1: `, `2: `, et cetera):
```
1: 0 3
2: 4 1
3: 2 5
4: end
```
An option would be to create multiline strings elsewhere, or appropriate preprepared ones, and copy-paste them into the console. For instance, that same trefoil knot could be notated as:
```
0 3
4 1
2 5
end

```
to leave the answering of confirmation prompt to the operative, or as:
```
0 3
4 1
2 5
end
y

```
to prelude the confirmation prompt, without user input. Bear in mind that in both cases, an endline character at the end is necessary for complete automation.

## Time Complexity

In the worst case, my algorithm iterates through every pair of crossovers an amount proportional to the amount of crossovers. This would therefore be $`O(N^3)`$ time if the detection and removal operation was $`O(1)`$. However, I believe the evaluation and removal is $`O(N\times log(N))`$. This would make the overall worst case time complexity $`O(N^4\times log(N))`$. I have not yet proved this, and it could be greater or lesser, but what I am most certain of is that it is polynomial.

## Preformatted Knot Examples

Preformatted knot examples can be found in the companion repository, [Unknotter: Preformatted Knots](https://github.com/brendanlynn/Unknotter_PreformattedKnots/).
