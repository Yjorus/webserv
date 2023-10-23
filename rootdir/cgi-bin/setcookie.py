
#!/usr/bin/python3

import os
import datetime
import cgi
from http import cookies

cookie = cookies.SimpleCookie()
print("HTTP/1.1 200 OK")
print("Set-Cookie:UserID = User=XYZ, Password=XYZ123, Expires=Tuesday")
print("Content-type: text/html\r\n\r\n")
print("<html>")
print("<head>")
print(datetime.datetime.strftime(datetime.datetime.now(), "<h1>  %H:%M:%S </h1>"))
print("</head>")
print("</html>")