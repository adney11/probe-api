BEGIN {
	description = "strip_first_two_lines"
}
{
	a[n++] = $0
}
END {
	for (i = 2; i < n; i++)
		print a[i]
}
