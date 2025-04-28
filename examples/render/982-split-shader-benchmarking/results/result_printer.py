import sys;

def variance(lis):
    avg = sum(lis)/len(lis);
    sq_dev_sum = 0;
    for el in lis:
        sq_dev_sum += pow(avg-el, 2);
    varnc=sq_dev_sum/(len(lis)-1);
    return varnc;
    

def main():
    if(len(sys.argv) < 2):
        print("Filename required");
        exit(1);
    contents = None;
    with open(sys.argv[1]) as file:
        contents = file.read();
    if(contents == None):
        exit(1);
    contents = contents.split("\n");
    res = [[],[],[]];
    index = 0;
    for el in contents:
        stripped = el.strip();
        if not stripped:
            index+=1;
            continue;
        res[index].append(float(stripped));
    for (name, lis) in zip(["Uber   ", "Split  ", "Uber-if"], res):
        print(f"{name}: mean {sum(lis)/len(lis)}, variance {variance(lis)}");
    return 0;

if(__name__ == "__main__"):
    main();