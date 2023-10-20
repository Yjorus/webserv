#! /usr/bin/python3

import os
from http import cookies

import cgi, cgitb 

print("HTTP/1.1 200 OK")
print("Content-type: text/html\r\n\r\n")

print("<html>")
print("<head>")
print("<!DOCTYPE html>")
print("<html lang=\"en\">")
print("<link href=\"../css/base.css\" rel=\"stylesheet\" type=\"text/css\">")
print("<style>")
print("body{background-color: rgb(29, 24, 57); color: white; text-align: center;}")

print("</style>")
print("</head>")

print("<body>")

print("<header>")
print("<div id =\"headBlock\">")
print("<nav class=\"menu\">")
print("<ul>")
print("<li class=\"hackermen\"><a href=\"../index.html\">HACKERMEN</a></li>")
print("<li><a href=\"../index.html\">HOME</a></li>")
print("<li><a href=\"../services.html\">SERVICES</a></li>")
print("<li><a href=\"../about.html\">ABOUT</a></li>")
print("<li><a href=\"whoaim.py\">CONTACT</a></li>")
print("<li><a href=\"../leaks.html\">LEAKS</a></li>")
print("</ul>")
print("</nav>")
print("</div>")
print("</header>")

cookie = cookies.SimpleCookie()
if 'HTTP_COOKIE' in os.environ:
	cookie = os.environ["HTTP_COOKIE"]
	cookie = cookie.split('; ')
	print("<h1 style=\"margin-top: 10%; margin-bottom: 5%;\">Your cookie is:<br><br>")
	print(cookie)
	print("\h1")
else:
	print("<h1>Cookie was not found !<\h1>")

print("</body>")
print("</html>")
