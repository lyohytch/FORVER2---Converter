#!/usr/bin/python

import os
import subprocess
import shutil

currDir = './'
docsDir = 'Docs/'
doxyfile = 'Doxyfile'
htmls = 'html/'
latexs = 'latex/'

def run_doxy(dirname):
    os.chdir(dirname)
    subprocess.call('doxygen')
    os.chdir('..')
    cpDir = docsDir + dirname + '/'
    if os.path.exists(cpDir):
        if os.path.isdir(cpDir):
            shutil.rmtree(cpDir)   
    shutil.copytree(dirname + '/' + htmls, cpDir + htmls)
    shutil.copytree(dirname + '/' + latexs, cpDir + latexs)
    shutil.rmtree(dirname + '/' + htmls)
    shutil.rmtree(dirname + '/' + latexs)    
#MAIN
if __name__ == '__main__':
    print 'Generate docs started...'
    dirList = os.listdir(currDir)
    for dirname in dirList:
        if os.path.exists(dirname + '/' + doxyfile):
            run_doxy(dirname)
    print 'Docs generated.'
    