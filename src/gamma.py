gamma = 3.0
max_in = 31
max_out = 31

out = ""
for i in range(max_in + 1):
	# Use +0.5 and int() for rounding to nearest integer
	num = int(pow(i / max_in, gamma) * max_out + 0.5)
	out += str(num) + ", "

print(out)