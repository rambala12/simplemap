# Simple Map
This program will take in a text file containing information about a "map" and use the data to perform certain operations.
## Creating a map file
A map is a list of streets. A street is defined in a specific way: [Street Name] (x1 coord, y1 coord) (x2 coord, y2 coord). An example is shown below.
```
Elm  (10,20) (10,0)
```

## Using the program
The program prompts you with several options:
```
Commands:
m = read map file
a = print all streets
p = print named streets
c = print cross streets
f = find point
r = find route between two points
h = help
q = quit
```
You can use the above commands to get what information you need about the map. The most useful function would be figuring out a route between two points in the map. Some commands require certain formatting as well:
Printing named streets - returns information about specified street
```
p [Street Name]
```
Print cross streets - returns names of streets that intersect with specified street
```
c [Street Name]
```
Find point - returns street names that contain specified point
```
f (x coord, y coord)
```
Find route between two points - returns best route between two points
```
r (x1 coord, y1 coord) (x2 coord, y2 coord)
```