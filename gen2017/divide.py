# -*- coding: utf-8 -*-
"""
Created on Sat Jan 28 00:08:33 2017

@author: alessandro
"""
import numpy as np

def find_digits(a,b,n):
    """ Simple division a/b to the nth-digit """
    res = []

    c = a//b
    res.append(c)
    i = 0    
    while i < n:
        a -= b*c        
        if a == 0:
            return res
        if a < b:
            a *= 10
        c = a//b
        res.append(c)
        i += 1
        
    return res
    
def fx(n,p):
    x1 = (n+1)**n
    x2 = (  n)**n
    res = find_digits(x1,x2,p)
    return res
    
def fy(n,p):
    x1 = (n+1)**(n+1)
    x2 = (  n)**(n+1)
    res = find_digits(x1,x2,p)
    return res

def check_norepetition(mylist):
    return len(mylist) == len(set(mylist)) and np.size(mylist)>2
#%%
p = 1000
prova = [fx(n,p) for n in range(101)]  
#%%
final = [[[i],prova[i]] for i in range(np.shape(prova)[0]) if np.size(prova[i])<p]
#%%
print(np.shape(final)[0])
#%%
final8 = [[final[i][0],(final[i][1][-8:])] for i in range(np.shape(final)[0])]
#%%
final8
#%%
winners = [check_norepetition(final8[i][1]) for i in range(np.shape(final8)[0])]
#%%
winners
#%%
winner = np.argwhere(np.array(winners)==True)
#%%
for win in winner[0]:
    print(final[win][0])
    print("\n x = ", final[win][1][-10:])
    print("\n y = ", fy(final[win][0][0],p)[-10:])
