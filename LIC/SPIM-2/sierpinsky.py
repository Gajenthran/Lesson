x = 1
while x < 10: # = 2**31
    n = x
    print('')
    #x ^= x << 1
    x = x ^ (x * 2);
    print(x)
    print(n)

