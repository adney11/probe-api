{
	description = "strip_middle_lines; input variabled: first - number of kept first lines, last - number of kept last lines, insert - number of dot lines inserted instead of middle lines"
	a[n++] = $0;
}
END {
	b = 1;
	for (i = 0; i < n; i++)
	{
		if( i < first || i >= n-last )
		{
			print a[i];
		}
		else if( b > 0 )
		{
			for(k = 0; k < insert; k++)
				print "..."
			b = 0
		}
	}
}