
cur_list = []
with open('in', 'r') as fin:
    with open('out1', 'w') as fout:
        n = int(fin.readline().rstrip())
        for i in range(n):
            op_array = fin.readline().rstrip().split(' ')
            op = int(op_array[0])
            if op == 0:
                val = int(op_array[1])
                cur_list.append(val)
            elif op == 1:
                pos = int(op_array[1])
                if pos < len(cur_list):
                    fout.write(str(cur_list[pos]) + '\n')
            elif op == 2:
                pos, val = (int(op_array[1]), int(op_array[2]))            
                cur_list.insert(pos, val)
            elif op == 3:
                pos = int(op_array[1])
                if pos < len(cur_list):
                    cur_list.pop(pos)

