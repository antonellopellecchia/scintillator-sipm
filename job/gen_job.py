#!/usr/bin/python3

import sys, os, stat

fjob = open('model/xray_copper.job')
fjobRead = fjob.read()

fsh = open('model/xray_copper.sh')
fshRead = fsh.read()

runPath = 'run_jobs.sh'
with open(runPath, 'w') as runFile:
    thicknesses = [ 5+5*i for i in range(20) ] 
    for thickness in thicknesses:
        fjobFormatted = fjobRead.format('%03dum'%(thickness))
        fshFormatted = fshRead.format('%03d'%(thickness), thickness)
        path = 'jobs/%03dum/'%(thickness)
        try: os.mkdir(path)
        except FileExistsError: pass
        with open(path+'xray_copper.job', 'w') as fjobOut:
            fjobOut.write(fjobFormatted)
        with open(path+'xray_copper.sh', 'w') as fshOut:
            fshOut.write(fshFormatted)
        os.chmod(path+'xray_copper.sh', 0o755)
        runFile.write('condor_submit {0} -name ettore'.format(path+'xray_copper.job'))
        runFile.write('\n')
