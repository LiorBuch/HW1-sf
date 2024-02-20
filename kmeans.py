import copy
import math
k = 3
n=800
d=3
iter = 600
path:str = "input_1.txt"
nodes = [] # will be 2D list
EPSILON = 0.001

def handleErrors(msg:str ="An Error Has Occurred"):
    print(msg)
    SystemExit(1)

def verifyData():
    if n<1 or not isinstance(n,int): handleErrors("Invalid number of points!")
    if 1 > k or k>n: handleErrors("Invalid number of clusters!")
    if not isinstance(d,int) or d<1: handleErrors("Invalid dimension of point!")
    if iter <= 1 or iter >=1000: handleErrors("Invalid maximum iteration!")


def euclidianDist(pointA:list,pointB: list)->float:
    if(len(pointA) != len(pointB)): handleErrors()
    dist = 0
    for i in range(0,len(pointA)):
        dist =math.pow((pointA[i]-pointB[i]),2) + dist
    return math.sqrt(dist)

def readFromFile(path: str):
    try:
        f = open(path, "r")
        for line in f:
            coords = line.split(',')
            newNode = []
            try:
                for coord in coords:
                    newNode.append(float(coord))
            except:
                handleErrors()
            nodes.append(newNode)
    except:
        handleErrors()
# Start of Script
verifyData()
# Read file
readFromFile(path)
# Initilize Centers for the first time
clusters = [[] for i in range(0,k)]

centers = [nodes[i] for i in range(0,k)]
prevCenters = copy.deepcopy(centers)

for i in range(0,k):
    clusters[i].append(centers[i])

def generateCenters():
    for index,cluster in enumerate(clusters):
        newCenter = [0 for i in range(0,d)]
        for i in range(0,d): # coord index
            for j in range(0,len(cluster)): # point index
                newCenter[i] = (newCenter[i] + cluster[j][i])
            newCenter[i] = newCenter[i]/len(cluster)
        centers[index] = newCenter

def calcClusters():
    for point in nodes:
        index = 0
        dist:float = -1
        for i in range(0,len(centers)):
            if dist == -1:
                dist = euclidianDist(centers[i],point)
            else:
                newDist =euclidianDist(centers[i],point)
                if newDist  < dist:
                    dist = newDist
                    index = i
        clusters[index].append(point)
# Iteration loop 
while iter>0:
    iter = iter-1
    calcClusters()
    generateCenters()
    for i in range(0,len(centers)):
        if euclidianDist(centers[i],prevCenters[i]) > EPSILON:
            prevCenters = copy.deepcopy(centers)
            break
    else:
        break
print(iter)
print(centers)
#format output