# Unknotter
This program was designed by computer scientist Brendan Lynn as a algorithm to detect whether or not a given mathematical knot is the unknot. Bear in mind that this algorithm is of an evidence-supported but nonetheless unproven hypothesis.

## Usage
Knots must be specified in a base-0, absolute, paired version of Dowker-Thistlethwaite notation, with the index crossing over on the left and the index crossing under on the right. For instance, the trefoil knot could be notated as "(3, 0), (4, 1), (2, 5)".

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
