BEGIN {
	description = "strip_first_line"
}
{
	a[n++] = $0
}
END {
	for (i = 1; i < n; i++)
		print a[i]
}
