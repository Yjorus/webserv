#!/usr/bin/python3

import datetime
import cgi

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
print("<li><a href=\"../cgi-bin/whoaim.py\">CONTACT</a></li>")
print("<li><a href=\"../leaks.html\">LEAKS</a></li>")
print("</ul>")
print("</nav>")
print("</div>")
print("</header>")

# print("<h1 style=\"margin-top:30%; font-size: 3vw\";>" + arg1 + "<h1>")

# print("<h1 style=\"color: red; border-bottom:0.1em solid red; font-size: 5vw;\"> You have been COMPROMISED</h1>")
# print("<h2>Change your passwords ASAP</h2>")

print(datetime.datetime.strftime(datetime.datetime.now(), "<h1 styel>  %H:%M:%S </h1>"))

print("</body>")
print("</html>")