pickdir = "../Apps/XCarViewer.Assets/Data" 
pickfiletag = "bmw"

infile = pickdir + "/"+pickfiletag+".obj"
outfile = pickdir +"/"+pickfiletag+".alx"

vertexdefs = []
texturedefs = []
normaldefs = []
facedefs = []
final = []

vertexcount = 0

def header(f):
    print('//786//')
    print('obj[ '+pickfiletag+' ] {')

def footer(f):
    print('}')
    print('vertex_count['+str(vertexcount)+']')
    print("")
    print("//Good Luck !!")

def get4(line):
    allin = line.split(' ')
    return allin[0].strip(),allin[1].strip(),allin[2].strip(),allin[3].strip()

def get3(line):
    allin = line.split(' ')
    return allin[0].strip(),allin[1].strip(),allin[2].strip()

def aline(sz):
    global vertexcount
    if line[0]=='v' and line[1]==' ':
        v = get4(line)
        vertexdefs.append('v[ '+v[1]+', '+v[2]+', '+v[3]+']')
        vertexcount = vertexcount + 1
    if line[0]=='v' and line[1]=='n' and line[2]==' ':
        v = get4(line)
        normaldefs.append('n[ '+v[1]+' ,'+v[2]+' ,'+v[3]+']')
    if line[0]=='v' and line[1]=='t' and line[2]==' ':
        v = get3(line)
        texturedefs.append('t[ '+v[1]+' ,'+v[2]+']')
    if line[0]=='f' and line[1]==' ':
        fac = line.split(' ')
        facedefs.append(fac[1].strip())
        facedefs.append(fac[2].strip())
        facedefs.append(fac[3].strip())    

with open(infile) as f:
    header(f)
    for line in f:
        # Remove any trailing whitespace including newline
        clean_line = line.strip()
        aline(clean_line)

    for i in range(len(facedefs)):
        vtn = (facedefs[i]).split('/')
        
        if (len(vtn[0])>0):
            i_v = int(vtn[0])-1
            print(vertexdefs[i_v])
        else:
            pass
        
        if (len(vtn[1])>0):
            i_t = int(vtn[1])-1
            print(texturedefs[i_t])
        else:
            pass
        
        if (len(vtn[2])>0):
            i_n = int(vtn[2])-1
            print(normaldefs[i_n])
        else:
            pass
        
    footer(f)
