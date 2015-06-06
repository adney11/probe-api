#!/bin/awk -f
# =========================================
BEGIN{	# begin section
}
# =========================================
# loop section
{
	a[n++] = $0
}
# =========================================
END{	# end section
	#IGNORECASE = 1
	for (i = 0; i < n; i++)
	{
		# Catch macroses like:
		# {{command/line/PROGRAM ARGUMENT1 ARGUMENT2}}

		if(match(a[i], /^{{(.+)}}/, m))
		{
			# and replace them by results of executing external process:
			cmd = m[1]
			fflush()
			system(cmd)
		}
		else
		{
			print a[i]
		}
	}
}
# =========================================
