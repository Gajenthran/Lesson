import os

f = open("insulte.txt", "r")
d = ""
for x in f:
  d += x.lower();
		
with open("insulte.txt", 'w') as file:
	file.write(d)


