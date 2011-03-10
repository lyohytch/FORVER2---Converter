#! /usr/bin/python

import os
import sys

sourceFile = ""
targetFile = ""


punctuationMarks = ['-', '.', '!', ',','?',':','\"','\'', ';','\\','/',' ','\t' ]

def splitStr(sstr, markList, rlist):
    buf = ""
    for sym in sstr:
        if sym not in markList:
            buf += sym
        elif len(buf):
            rlist.append(buf)
            buf = ""

def processLines(source, sourceDiff):
    for line in source:
        splitStr(line, punctuationMarks, sourceDiff)
             
def removeEqualsWords(srcDiffs, trgDiffs):
    i = 0
    while i < len(srcDiffs):
        if srcDiffs[i] in trgDiffs:
            trgDiffs.remove(srcDiffs[i])
            del srcDiffs[i]
        else:
            i = i + 1

def compare( source, target, sourceDiffs, targetDiffs):
    processLines(source, sourceDiffs)
    processLines(target, targetDiffs)   
    if len(sourceDiffs) > len(targetDiffs):
          removeEqualsWords(targetDiffs, sourceDiffs)               
    else:
          removeEqualsWords(sourceDiffs, targetDiffs)     

def out(sourceDiffs, targetDiffs):
    if len(sourceDiffs) or len(targetDiffs):
        print 'Source:\t', sourceFile
        if len(sourceDiff):
            for sourceName in sourceDiffs:
                print '\t\t', sourceName
        print 'Target:\t', targetFile
        if len(targetDiff):
            for targetName in targetDiffs:
                print '\t\t', targetName
    else:
        print 'Files are equal'
#MAIN
if __name__ == '__main__':
    print 'Puncuation marks:', punctuationMarks
    if len(sys.argv) > 2:
        sourceFile = sys.argv[1]
        targetFile = sys.argv[2]
        if os.path.exists(sourceFile) and os.path.isfile(sourceFile):
            print 'Source file is OK'
            if os.path.exists(targetFile) and os.path.isfile(targetFile):
                print 'Target file is OK'
                sourceDiff = []
                targetDiff = []
                compare(open(sourceFile, 'r'), open(targetFile, 'r'), sourceDiff, targetDiff)
                out(sourceDiff, targetDiff)
            else:
                print 'Target file is unavailable'
        else:
            print 'Source file is unavailable'
    else:
        print "Nothing to compare. Please point files which you will compare in arguments"
