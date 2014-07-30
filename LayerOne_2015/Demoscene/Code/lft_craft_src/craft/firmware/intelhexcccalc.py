s = '1001F000CA01D201D801DE01E401EA012F6C6674'

sumofs = 0

for i in range(0, len(s)/2):
	e = i*2
	print int("0x"+s[e:e+2],16)
	sumofs += int("0x"+s[e:e+2],16)

print hex(sumofs)
twoscomp = hex(((abs(sumofs) ^ 0xffff) + 1) & 0xffff)
print twoscomp
c = hex(int(twoscomp,16) & 0x00ff)
print c