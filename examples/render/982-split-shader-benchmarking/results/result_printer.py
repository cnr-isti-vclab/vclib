import sys;

def variance(lis):
    avg = sum(lis)/len(lis);
    sq_dev_sum = 0;
    for el in lis:
        sq_dev_sum += pow(avg-el, 2);
    varnc=sq_dev_sum/(len(lis)-1);
    return varnc;
    

def main():
    if(len(sys.argv) != 4):
        print("Filenames required");
        exit(1);
    contents = list();
    for i in range(1, len(sys.argv)):
        with open(sys.argv[i]) as file:
            temp = file.read();
            if not temp:
                exit(1);
            contents.append(temp);
    contents = [[float(y) for y in x.strip().split('\n')] for x in contents];
    for (name, lis) in zip(["Uber   ", "Split  ", "Uber-if"], contents):
        print(f"{name}: mean {sum(lis)/len(lis)}, variance {variance(lis)}");
    return 0;

if(__name__ == "__main__"):
    main();