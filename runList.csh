#!/bin/tcsh

foreach line ("`cat $1`")
	echo "$line"
	root4star -b -q -l 'make.C( "'$line'" )'
end

# new line needed at end of tcsh script
