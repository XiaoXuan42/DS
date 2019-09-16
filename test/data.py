from random import randint

def generate(n):
    l = 0
    op_array = []
    for i in range(n):
        op = randint(0, 3)
        while op == 3 and l == 0:
            op = randint(0, 2)
        if op == 0: #append
            x = randint(1, 1000)
            op_array.append((op, x))
            l += 1
        elif op == 1: #search
            x = randint(0, l)
            op_array.append((op, x))
        elif op == 2: #insert
            x = randint(0, l)
            y = randint(1, 1000)
            op_array.append((op, x, y))
            l += 1
        elif op == 3: #delete
            x = randint(0, l)
            op_array.append((op, x))
            l -= 1
    return op_array
            

n = randint(10000000, 100000000)
outcome = generate(n)
with open('in', 'w') as f:
    f.write(str(n) + '\n')
    for i in outcome:
        for j in i:
            f.write(str(j) + ' ')
        f.write('\n')