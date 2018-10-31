#!/usr/bin/python
#
# flush_systran.py - script to setup/reset/flush the channel
#
# last modified 21 apr 06 - mjh
#
import os
import sys
import re

print 'starting/flushing systran'
temp_file = '/tmp/' + os.getenv('USER','mnsn') + '_slmon_ouput'

flushed = 0
driver_not_started = 0
sl240_home_not_defined = 0

for i in range(1,6):
  if flushed > 1: break
  if driver_not_started > 0: break
  if sl240_home_not_defined > 0: break
  os_command = '$SL240_HOME/bin/sl_mon 0 clrf >& ' + temp_file
  os.system(os_command)
  for line in open(temp_file,'r').readlines():
    print 'line =', line
    if re.search('^SL_MON:',line):
      if re.search('ERROR',line):
        driver_not_started = 1
      if re.search(' 0 Bytes',line):
        flushed = flushed + 1
    if re.search('No such file',line):
      sl240_home_not_defined = 1

if (sl240_home_not_defined == 1):
  print 'ERROR: SL240_HOME appears to not be defined'
  print 'If you have not already done so:'
  print '   source setup_monsoon   '
  sys.exit(-1)

if (flushed == 0):
  if (driver_not_started == 0):
    print 'ERROR: unable to flush systran'
    sys.exit(-1)

if (driver_not_started == 1):
# pick the flavor that works for you:
# os_command = 'pushd $SL240_HOME/driver; ./fxuninst; popd'
# os_command = 'pushd $SL240_HOME/driver; sudo ./fxuninst; popd'
  os_command = 'pushd $SL240_HOME/../; ./fxuninst_uiuc; popd'
  os.system(os_command)
# os_command = 'pushd $SL240_HOME/driver; ./fxinst; popd'
# os_command = 'pushd $SL240_HOME/driver; sudo ./fxinst; popd'
  os_command = 'pushd $SL240_HOME/../; ./fxinst_uiuc; popd'
  os.system(os_command)

os_command = '$SL240_HOME/bin/sl_mon 0 fpdp 6'
os.system(os_command)

os_command = 'sleep 5'
os.system(os_command)

os_command = '$SL240_HOME/bin/sl_mon 0 fpdp 0'
os.system(os_command)

flushed = 0
for i in range(1,6):
  if flushed > 1: break
  os_command = '$SL240_HOME/bin/sl_mon 0 clrf >& ' + temp_file
  os.system(os_command)
  for line in open(temp_file,'r').readlines():
    print 'line =', line
    if re.search('^SL_MON:',line):
      if re.search(' 0 Bytes',line):
        flushed = flushed + 1

if (flushed == 0):
  print 'ERROR: unable to flush systran'
  sys.exit(-1)
print 'flushed'

