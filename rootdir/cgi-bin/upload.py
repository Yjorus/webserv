#!/usr/bin/python3

import cgi, os
import cgitb; cgitb.enable()

form = cgi.FieldStorage()

# Get filename here
fileitem = form['image_uploads']

# Test if the file was uploaded
if fileitem.filename:
   open(os.getcwd() + '/rootdir/delete/' + os.path.basename(fileitem.filename), 'wb').write(fileitem.file.read())
   message = 'The file "' + os.path.basename(fileitem.filename) + '" was uploaded to ' + os.getcwd() + '/rootdir/delete'
else:
   message = 'Uploading Failed'
print("HTTP/1.1 200 OK")
print("Content-Type: text/html;charset=utf-8")
print ("Content-type: text/html\r\n\r")
print("<H1> " + message + " </H1>")
