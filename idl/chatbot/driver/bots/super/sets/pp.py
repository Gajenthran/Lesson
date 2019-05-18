import os

f = open("animal.txt", "r")
d = ""
for x in f:
  d += x.lower();
		
with open("animal.txt", 'w') as file:
	file.write(d)


