#!/usr/bin/perl -w

  use CGI;

  $upload_dir = "/tmp/";

  $query = new CGI;

  $filename = $query->param("photo");
  $filename =~ s/.*[\/\\](.*)/$1/;
  $upload_filehandle = $query->upload("photo");

  open UPLOADFILE, ">$upload_dir/$filename";

  while ( <$upload_filehandle> )
  {
    print UPLOADFILE;
  }

  close UPLOADFILE;

  print $query->header ( );
