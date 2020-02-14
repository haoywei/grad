import math
import random
import numpy as np
import math
def cost_function(x,y,w,delta,lam):
    result = 0
    for i in range(len(x)):
        temp = np.matmul(np.transpose(w),x[i])
        if(y[i]>=temp+delta):
            result += 1/len(x)*(y[i]-temp-delta)**2
        if(y[i]<temp+delta and y[i]>temp-delta):
            result += 0
        if(y[i]<=temp-delta):
            result += 1/len(x)*(y[i]-temp+delta)**2
    for j in range(len(w)):
        result += lam*(w[j])**2
    return result
    
    
def update_weights(x,y, w, eta,delta,lam):
    result1 = 0
    for i in range(len(x)):
        temp = w[0]*x[i][0] +w[1]
        if(y[i]>=temp+delta):
            result1 += -1/len(x)*(y[i]-temp-delta)*x[i][0]*2
        if(y[i]<temp+delta and y[i]>temp-delta):
            result1 += 0
        if(y[i]<=temp-delta):
            result1 += -1/len(x)*(y[i]-temp+delta)*x[i][0]*2
    result1 += 2*lam*w[0]
    result2 = 0
    for i in range(len(x)):
        temp = w[0]*x[i][0] +w[1]
        if(y[i]>=temp+delta):
            result2 += -1/len(x)*(y[i]-temp-delta)*x[i][1]*2
        if(y[i]<temp+delta and y[i]>temp-delta):
            result2 += 0
        if(y[i]<=temp-delta):
            result2 += -1/len(x)*(y[i]-temp+delta)*x[i][1]*2
    result2 += 2*lam*w[1]
    w[0] = w[0] - result1*eta 
    w[1] = w[1] - result2*eta 
    return w

def update_weights_sdg(x,y, w, eta,delta,lam):
    result1 = 0
    result2 = 0
    i = random.randint(0,99)
    temp = w[0]*x[i][0] +w[1]
    if(y[i]>=temp+delta):
        result1 += -1/len(x)*(y[i]-temp-delta)*x[i][0]*2
        result2 += -1/len(x)*(y[i]-temp-delta)*x[i][1]*2
    if(y[i]<temp+delta and y[i]>temp-delta):
        result1 += 0
        result2 += 0
    if(y[i]<=temp-delta):
        result1 += -1/len(x)*(y[i]-temp+delta)*x[i][0]*2
        result2 += -1/len(x)*(y[i]-temp+delta)*x[i][1]*2
    result1 += 2*lam*w[0]
    result2 += 2*lam*w[1]
        
    
    w[0] = w[0] - result1*eta 
    w[1] = w[1] - result2*eta 
    return w            
    
    
def bgd_l2(data, y, w, eta, delta, lam, num_iter):
    new_w = w
    history_fw = []
    for i in range(num_iter):
        new_w = update_weights(data,y, new_w, eta,delta,lam)
        cost = cost_function(data,y,new_w,delta,lam)
        history_fw.append(cost)
    print(history_fw)
    return new_w, history_fw
def sgd_l2(data, y, w, eta, delta, lam, num_iter, i=-1):
    new_w = w
    history_fw = []
    if(i == -1):
        for i in range(num_iter):
            etax = eta/math.sqrt(i+1)
            new_w = update_weights_sdg(data,y, new_w, etax,delta,lam)
            cost = cost_function(data,y,new_w,delta,lam)
            history_fw.append(cost)
    else:
            etax = eta/math.sqrt(i+1)
            new_w = update_weights_sdg(data,y, new_w, etax,delta,lam)
            cost = cost_function(data,y,new_w,delta,lam)
            history_fw.append(cost)
    return new_w, history_fw