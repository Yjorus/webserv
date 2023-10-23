#! /usr/bin/python3

import os
from http import cookies
# Import modules for CGI handling 
import cgi, cgitb 

# Get data from fields
cookie = cookies.SimpleCookie()
if 'HTTP_COOKIE' in os.environ:
	cookie = os.environ["HTTP_COOKIE"]
	cookie = cookie.split('; ')
	print("HTTP/1.1 200 OK")
	print("Content-Type: text/plain\r\n")
	print(cookie)
else:
    print("HTTP/1.1 200 OK")
    print("Content-Type: text/plain\r\n")
    print("Cookie was not found !")