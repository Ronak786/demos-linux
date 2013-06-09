#!/usr/bin/python3

"""
this script will check that all files that match
'*.c' '*.cc' '*.h' '*.hh' '*.S'
have, at their begining, the license specified in
'support/license.txt'
"""

import sys # for sys.argv
import os # for walk

new_lic=open("support/license.txt").read()

suffixes=['.c','.cc','.h','.hh','.S']
error=False
root_folder='.'
for root,dirs,files in os.walk(root_folder):
	for file in files:
		current_file=os.path.join(root,file)
		doit=False
		for suf in suffixes:
			if current_file.endswith(suf):
				doit=True
		if doit:
			#print('doing',current_file)
			with open(current_file) as in_f:
				f=in_f.read()
				if not f.startswith(new_lic):
					error=True;
					print("bad license for",current_file)
if error:
	raise ValueError("had errors")
