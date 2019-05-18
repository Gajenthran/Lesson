import os

f = open("composant.txt", "r")
d = ""
for x in f:
  d += x.lower();
		
with open("composant.txt", 'w') as file:
	file.write(d)


