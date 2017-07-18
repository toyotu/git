#!/usr/bin/env python

import sys, os, math
# import numpy as np
# import scipy as sp
# import ROOT
# import pyfits as pf

def addNewCity(list):
    print ("add newcity");
    str = input()
    list.append(str)
    print(list)

def deleteCity(list):
    print("delete city");

def showList(list):
    print("shou city")

def saveList(list):
    print("saved the file")
    
def main():
    city_list = ["a", "b", "c"]
    while True:
        print("Menu(a:add, d:delete, l:list, s:sort, w:write file, r:read file q:quit):")
        str = input()
        print(str)
        print(city_list)
        if str == "q" :
            break
        elif str == "a":        
            addNewCity(city_list)
            print(city_list)
        elif str == "d":
            deleteCity(city_list)
        elif str == "l":
            showList(city_list)
main()
