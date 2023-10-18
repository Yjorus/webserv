#! /usr/bin/python3

import os
import cgi

form = cgi.FieldStorage()

arg1 = form.getvalue('email')
arg2 = form.getvalue('radio')

print("HTTP/1.1 200 OK")
print("Content-type: text/html\r\n\r\n")

print("<html>")
print("<head>")
print("<h2>Environment:</h2><br>")


print("<html>")
print("<body>")
print(arg1)
print(arg2)

print("</body>")
print("</html>")