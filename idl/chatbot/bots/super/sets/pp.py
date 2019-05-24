import os

f = open("job.txt", "r")
d = ""
for x in f:
  d += x.lower();
		
with open("job.txt", 'w') as file:
	file.write(d)


