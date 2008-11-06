#include <nemesis.app.functions.h>

#include <smart.onlinehelp.Frame.h>
#include <smart.onlinehelp.Document.h>

using namespace std;
using namespace nemesis;
using namespace smart;

onlinehelp::Frame::Frame (const string& name, const string& title) :
   a_title (title)
{
   string str (name);
   char* aux = const_cast <char*> (str.data ());
   
   while (*aux) {
      if (*aux == ' ')
         *aux = '_';
      aux ++;
   }

   a_url = "mainFrame_";
   a_url += str;
   a_url += ".html";
}

void onlinehelp::Frame::open () 
   throw (RuntimeException)
{
   a_stream.open (a_url.data (), ios_base::out | ios_base::trunc);
   
   if (a_stream.is_open () == true) {
      a_stream << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\">" << endl;
      a_stream << "<!--NewPage-->" << endl << "<html" << endl << "<head" << endl;
      a_stream << "<!-- Archivo generado por SmartDoc Versi�n";
      a_stream << app::functions::getApp ().getVersion () << "-->" << endl;
      a_stream << "<title>" << a_title << "</title>" << endl;         
      a_stream << "</head>" << endl;
      a_stream << "<frameset cols=\"20%,80%\">\n<frameset rows=\"30%,70%\">" << endl;
   }
   else {
      string msg ("Can not open file ");
      msg += a_url;
      throw RuntimeException (msg.data (), FILE_LOCATION);
   }   
}

void onlinehelp::Frame::set (const Document& libraries, const Document& classes, const std::string& overview)
   throw (RuntimeException)
{
   if (a_stream.is_open () == false)
      open ();
      
   a_stream << "<frame src=\"" << libraries.getURL () << "\" name=\"libraryList\">" << endl;
   a_stream << "<frame src=\"" << classes.getURL () << "\" name=\"classesFrame\">" << endl;
   a_stream << "</frameset>" << endl;
   
   a_stream << "<frame src=\"" << overview << "\" name=\"classFrame\">" << endl;
   a_stream << "</frameset>" << endl;
}

void onlinehelp::Frame::close ()
{
   if (a_stream.is_open () == true) 
         a_stream << "</html>" << endl;
}

