#! /usr/bin/python3

import os
import cgi
import datetime

form = cgi.FieldStorage()

arg1 = form.getvalue('email')
arg2 = form.getvalue('password')
arg3 = form.getvalue('checkbox')

print("HTTP/1.1 200 OK")
if arg1 != '' and arg2 == '':
	print("Set-Cookie:webserv = user="+ arg1 +",  password=" + arg2 + ", expires=Tuesday, 31-Dec-2024 23:12:40 GMT")
print("Content-type: text/html\r\n\r\n")

print("<html>")
print("<head>")
print("<!DOCTYPE html>")
print("<html lang=\"en\">")
print("<link href=\"../css/base.css\" rel=\"stylesheet\" type=\"text/css\">")
print("<link href=\"../css/glitch.css\" rel=\"stylesheet\" type=\"text/css\">")
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
print("<li><a href=\"whoami.py\">CONTACT</a></li>")
print("<li><a href=\"../leaks.html\">LEAKS</a></li>")
print("</ul>")
print("</nav>")
print("</div>")
print("</header>")

if arg1 and arg2 and arg3 == 'on':
	print("<h1 style=\"margin-top:15%; font-size: 3vw\";>" + arg1 + "<h1>")
	print("<div class=\"glitch-container\" style=\"margin-top:2%; padding-top:5%;\">")
	print(datetime.datetime.strftime(datetime.datetime.now(), "<h2 class=\"hero glitch layers\" ><span>You have been <br>HACKED<br> at: %H:%M:%S</span></h2>"))
	print("</div>")
	print("<h2>Your password <span style=\"color: red\">" + arg2 + "</span> was added to our DataBase</h2>")
	print("<h2>Starting process to clone your credits card</h2>")
elif arg1 and arg3 == 'on':
	print("<h1 style=\"margin-top:15%; font-size: 3vw\";>" + arg1 + "<h1>")
	print("<h1 style=\"margin-top:3%; color: green; border-bottom:0.1em solid green; font-size: 5vw;\"> Congratulations You're Safe </h1>")
	print("<h2 style=\"margin-top:3%; font-size: 3vw;\">(((For now)))</h2>")
elif arg2 and arg3 == 'on':
	print("<h2 style=\" margin-top:15%; font-size: 2vw;\">Your password <span style=\"color: red\">" + arg2 + "</span> was added to our DataBase</h2>")
	print("<h1 style=\" margin-top:3%; color: green; border-bottom:0.1em solid green; font-size: 5vw;\"> Dont worrie, we'll find your email </h1>")
	print("<h2 style=\"font-size: 3vw; margin-top:3%; font-size: 3vw\">(((Realy Soon)))</h2>")
elif arg3 == 'on':
	print("<h1 style=\"margin-top:15%; color: green; border-bottom:0.1em solid green; font-size: 5vw;\"> Congratulations You're Safe </h1>")
	print("<h2 style=\"margin-top:3%; font-size: 3vw;\">(((For now)))</h2>")
else:
	print("<h2 style=\"margin-top:15%; font-size: 3vw\";> Agree with our terms berfore starting using our services</h2>")
print("</body>")
print("</html>")