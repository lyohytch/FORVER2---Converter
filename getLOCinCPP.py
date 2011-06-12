#! /usr/bin/python

import os
import sys

searchExts = ['.c', '.cpp', '.h', '.hpp']
total = 0

def getLOC(arg, dirname, names):
    for fname in names:
        pathToFile = os.path.join(dirname, fname)   
        if os.path.splitext(pathToFile)[1] in searchExts:
            global total
            local = sum (1 for l in open(pathToFile, "r"))
            total = total + local
            print 'File:\t',fname,'\tLOC:\t',local,'\tFolder:\t',os.path.dirname(pathToFile)
#MAIN
if __name__ == '__main__':
    wrkDir = os.getcwd()
    if len(sys.argv) > 1:
        if os.path.exists(sys.argv[1]) and os.path.isdir(sys.argv[1]):
            wrkDir = sys.argv[1] 
    os.path.walk( wrkDir, getLOC, None)
    global total
    print 'Total:', total

                            
                  
        
