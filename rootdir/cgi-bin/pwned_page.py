#! /usr/bin/python3

import os
import cgi

form = cgi.FieldStorage()

arg1 = form.getvalue('email')
# arg2 = form.getvalue('checkbox')

print("HTTP/1.1 200 OK")
print("Content-type: text/html\r\n\r\n")

	print("<html>")
	print("<head>")
	print("<!DOCTYPE html>")
	print("<html lang="en">")
	printf("<link href="../css/base.css" rel="stylesheet" type="text/css">")
	print("<style>")
	print("</style>")
	print("</head>")


	print("<html>")
	print("<body>")

if (arg2 == yes)
{
	print(arg1)
	print("<h1> You have been PWNED</h1>")
}
else
{
	print("<html>")
	print("<body>")
	print(arg1)
	print("<h1> Agree with our terms berfore starting using our services</h1>")
}
	print("</body>")
	print("</html>")