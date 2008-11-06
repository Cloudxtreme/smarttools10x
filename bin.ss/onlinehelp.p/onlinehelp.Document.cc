#include <nemesis.app.functions.h>

#include <smart.onlinehelp.Document.h>
#include <smart.onlinehelp.Class.h>

using namespace std;
using namespace nemesis;
using namespace smart;

onlinehelp::Document::Document (const Type type, const string& name, const char* title) :
   a_type (type)
{
   string str (name);
   char* aux = const_cast <char*> (str.data ());
   
   while (*aux) {
      if (*aux == ' ')
         *aux = '_';
      aux ++;
   }

   a_url = (type == Libraries) ? "libraries_": "classes_";
   a_url += str;
   a_url += ".html";
   
   a_title = (title == NULL) ? "": title;
}

void onlinehelp::Document::open () 
   throw (RuntimeException)
{
   a_stream.open (a_url.data (), ios_base::out | ios_base::trunc);
   
   if (a_stream.is_open () == true) {
      a_stream << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\">" << endl;
      a_stream << "<!--NewPage-->" << endl << "<html" << endl << "<head" << endl;
      a_stream << "<!-- Archivo generado por SmartDoc Versi�n ";
      a_stream << app::functions::getApp ().getVersion () << "-->" << endl;
      a_stream << "<title>" << a_title << "</title>" << endl;
      a_stream << "</head>" << endl;
      a_stream << "<body BGCOLOR=\"white\">" << endl;
      
      a_stream << "<table BORDER=\"0\" WIDTH=\"100%\">" << endl;
      a_stream << "<tr> <td NOWRAP bgcolor=\"#6666cc\"> <font size=\"+1\" ID=\"FrameTitleFont\">" << endl;
      a_stream << "<font color=\"#ffffff\"> <b fgcolor=\"white\">" << a_title << "</b><br></font></td></tr></table>" << endl;
      
      a_stream << "<table BORDER=\"0\" WIDTH=\"100%\">" << endl;
      a_stream << "<tr> <td>" << endl;

      if (a_type == Libraries)
         a_stream << "<b>" << endl;
   }
   else {
      string msg ("Can not open file ");
      msg += a_url;
      throw RuntimeException (msg.data (), FILE_LOCATION);
   }   
}

void onlinehelp::Document::reference (const Class& theClass) 
   throw (nemesis::RuntimeException)
{
   reference (theClass.getClassName ().data (), theClass.getHTMLFile ().data ());
}

void onlinehelp::Document::reference (const char* text, const char* url, const bool overview)
   throw (RuntimeException)
{
   if (a_stream.is_open () == false)
      open ();
      
   a_stream << "<font ID=\"FrameItemFont\"><a HREF=\"" << url << "\" TARGET=\"";   
   a_stream << ((a_type == Classes || overview == true) ? "classFrame": "classesFrame") << "\">";
   a_stream << text << "</a></font><br>" << endl;
}

void onlinehelp::Document::newLine () 
   throw (nemesis::RuntimeException)
{
   if (a_stream.is_open () == false)
      open ();
      
   a_stream << "<br>" << endl;
}

void onlinehelp::Document::close ()
{
   if (a_stream.is_open () == true) {
      if (a_type == Libraries)
         a_stream << "<b>" << endl;

      a_stream << "</td> </tr> </table>" << endl;
      a_stream << "</body>" << endl << "</html>" << endl;
   }
}

