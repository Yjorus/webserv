#! /usr/bin/python3

import os
import cgi

form = cgi.FieldStorage()

arg1 = form.getvalue('email')
arg2 = form.getvalue('checkbox')

print("HTTP/1.1 200 OK")
print("Content-type: text/html\r\n\r\n")

print("<html>")
print("<head>")
print("<!DOCTYPE html>")
print("<html lang=\"en\">")
print("<link href=\"../css/base.css\" rel=\"stylesheet\" type=\"text/css\">")
print("<style>")
print("html{")
print("background-color: rgb(29, 24, 57); color: white;}")

print("</style>")
print("</head>")

print("<body>")

print("<h1>" + arg1 + "<h1>")

if arg2 == 'on':
	print("<h1> You have been PWNED</h1>")
else:
	print("<h1> Agree with our terms berfore starting using our services</h1>")
print("</body>")
print("</html>")