import os

f = open("religion.txt", "r")
d = ""


for x in f:
  d += x.lower();

d = d.split("\n");
d = sorted(d);
d = "\n".join(d);
print(d)

with open("religion.txt", 'w') as file:
	file.write(d)