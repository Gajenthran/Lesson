import os

f = open("city2country.txt", "r")
d = ""


for x in f:
  d += x.lower();

d = d.split("\n");
d = sorted(d);
d = "\n".join(d);
print(d)

with open("city2country.txt", 'w') as file:
	file.write(d)