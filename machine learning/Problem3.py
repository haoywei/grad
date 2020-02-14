import numpy as np
import scipy as sp
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import metrics
import math
import random
def eucl_dist(x1,x2):
    return math.sqrt((x1[0]-x2[0])**2 +(x1[1]-x2[1])**2)
def nearest_dist(point,cluster):
    min_dist = math.inf#assume min to be inf
    for i in cluster: #loop all clusters to find the smallest distance
        dist = eucl_dist(point,i)
        if(dist<min_dist):
            min_dist = dist
    return min_dist
def k_init(X, k):
    """ k-means++: initialization algorithm

    Parameters
    ----------
    X: array, shape(n ,d)
        Input array of n samples and d features

    k: int
        The number of clusters

    Returns
    -------
    init_centers: array (k, d)
        The initialize centers for kmeans++
    """
    temp = []
    rand = np.random.randint(0, len(X))# firstly randomly pick one cluster
    temp.append(X[rand])
    dist = []
    for i in range(len(X)):
        dist.append(0)
    for i in range(1,k):
        total_sum = 0
        for j in range(len(X)):
            dist[j] = nearest_dist(X[j], temp)#find the nearest cluster
            total_sum += dist[j]#add distance to the total
        total_sum = total_sum*random.random()#multiply by a random number
        for j in range(len(dist)):
            total_sum -= dist[j]
            if(total_sum <= 0):
                temp.append(X[j])# when total < 0 means that we need to pick this number
                break
    return temp


def k_means_pp(X, k, max_iter):
    """ k-means++ clustering algorithm

    step 1: call k_init() to initialize the centers
    step 2: iteratively refine the assignments

    Parameters
    ----------
    X: array, shape(n ,d)
        Input array of n samples and d features

    k: int
        The number of clusters

    max_iter: int
        Maximum number of iteration

    Returns
    -------
    final_centers: array, shape (k, d)
        The final cluster centers
    """
    ori_cluster = k_init(X, k) #intialize cluster
    err = 0
    

    
    for i in range(max_iter):
        cluster_map = assign_data2clusters(X, ori_cluster)  #update cluster map to put point to clusters          
        for i in range(k): #calculate value in the same cluster only
            count = 0
            sumx = 0
            sumy = 0
            for j in range(len(cluster_map)):
                if(cluster_map[j] == i):
                    sumx += X[j][0]
                    sumy += X[j][1]
                    count += 1
            
            ori_cluster[i] = [sumx/count,sumy/count]        # using mean value of the points in the cluster to be the new cluster
 
            
                       
    return ori_cluster


def assign_data2clusters(X, C):
    """ Assignments of data to the clusters
    Parameters
    ----------
    X: array, shape(n ,d)
        Input array of n samples and d features

    C: array, shape(k ,d)
        The final cluster centers

    Returns
    -------
    data_map: array, shape(n, k)
        The binary matrix A which shows the assignments of data points (X) to
        the input centers (C).
    """
    cluster_map = []
    for i in range(len(X)):
        cluster_map.append([])
    
    for j in range(len(X)):
        distance = []
        for t in C:
            distance.append(eucl_dist(X[j],t))#get the distance between point and cluster
                
        temp_min_dist = math.inf
        index = -1
        for t in range(len(distance)):     
            if(distance[t]<temp_min_dist):
                index = t
                temp_min_dist = distance[t]#find the closest distance between the point and the cluster
        cluster_map[j] = index#update cluster map
    return cluster_map

def compute_objective(X, C):
    """ Compute the clustering objective for X and C
    Parameters
    ----------
    X: array, shape(n ,d)
        Input array of n samples and d features

    C: array, shape(k ,d)
        The final cluster centers

    Returns
    -------
    accuracy: float
        The objective for the given assigments
    """
    cluster_map = assign_data2clusters(X, C)
    score = metrics.silhouette_score(X, cluster_map) #using silhouette_score as the accuracy score for the cluster evaluation
    return score

if __name__ == '__main__':
    df = pd.read_csv("iris.data",names=["sepal_l","sepal_w","petal_l","petal_w","class_1"])
    rows = df.shape[0]
    c = []
    x_1 = []
    x_2 = []
    plt.figure(1)
    for i in range(rows):
            c.append([])
            x_1.append([])
            x_2.append([])
            c[i] = (df["class_1"][i])
            x_1[i] = (df["sepal_l"][i]/df["sepal_w"][i]) # calculate x_1
            x_2[i] = (df["petal_l"][i]/df["petal_w"][i]) # calculate x_2
            if(c[i] == 'Iris-setosa'):#use three differenct colors to show different groups
                plt.scatter(x_1[i], x_2[i], color="red")
            if(c[i] == 'Iris-versicolor'):
                plt.scatter(x_1[i], x_2[i], color="green")
            if(c[i] == 'Iris-virginica'):
                plt.scatter(x_1[i], x_2[i], color="blue")
    
    plt.title('clusters')
    plt.xlabel('sepal length/sepal width')
    plt.ylabel('petal length/petal width')
    plt.show() #make the first plot with x_1 as x_axis and x_2 as y_axis
    x_f = [] 
    scores = []
    for i in range(rows):
        x_f.append([x_1[i],x_2[i]])
    for k in range(1,6):
        cluster = k_means_pp(x_f,k,50)#get the final cluster group of the points with input k =1 to 5 and 50 iterations
        if(k == 1):
            scores.append(0) #when k = 1 we cannot run the score function then the score is 0
        else:
            scores.append(compute_objective(np.array(x_f), np.array(cluster))) #when k != 1 then we can run the score function to get the score
    plt.figure(2) # make the second plot with accuracy vs number of clusters
    plt.plot([1,2,3,4,5],scores)
    plt.title('k mean++ accuracy vs k value')
    plt.xlabel('k value')
    plt.ylabel('accuracy')
    plt.show()    
    plt.figure(3)    # from the plot we can choose k as 3 as the best choice
    scores = []
    t = []
    for i in range(1,100): #loop iterations from one to 100
        cluster = k_means_pp(x_f,3,i)#get the final cluster group of the points with input k =3 and 1 to 99 iterations
        scores.append(compute_objective(np.array(x_f), np.array(cluster)))
        t.append(i)
    plt.plot(t,scores)
    plt.title('k mean++ accuracy vs iterations')
    plt.xlabel('iterations')
    plt.ylabel('accuracy')
    plt.show()
    plt.figure(4) 
    cluster = k_means_pp(x_f,3,50)
    cluster_map = assign_data2clusters(x_f, cluster)
    print(cluster_map)
    for i in range(rows):
        if(cluster_map[i] == 0):#use three differenct colors to show different groups
            plt.scatter(x_f[i][0], x_f[i][1], color="black")
        if(cluster_map[i] == 1):
            plt.scatter(x_f[i][0], x_f[i][1], color="yellow")
        if(cluster_map[i] == 2):
            plt.scatter(x_f[i][0], x_f[i][1], color="pink") 
    
    plt.plot(cluster[0][0], cluster[0][1], 'rp', markersize=40,color="black")
    plt.plot(cluster[1][0], cluster[1][1], 'rp', markersize=40,color="yellow")
    plt.plot(cluster[2][0], cluster[2][1], 'rp', markersize=40,color="pink")
    plt.title('clusters')
    plt.xlabel('sepal length/sepal width')
    plt.ylabel('petal length/petal width')
    plt.show() #make the final plot with x_1 as x_axis and x_2 as y_axis