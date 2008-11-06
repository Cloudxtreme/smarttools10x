#include <nemesis.Logger.h>

#include <nemesis.xml.DocumentFile.h>
#include <nemesis.xml.DTDMemory.h>
#include <nemesis.xml.Parser.h>
#include <nemesis.xml.Node.h>
#include <nemesis.xml.Attribute.h>

#include <smart.configure.Tuning.h>

using namespace std;
using namespace nemesis;
using namespace smart;

const char* configure::Tuning::st_dtd = "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?> \
<!ELEMENT Tuning (Resource+)> \
<!ELEMENT Resource EMPTY> \
<!ATTLIST Resource Name CDATA #REQUIRED> \
<!ATTLIST Resource Key CDATA #IMPLIED Value CDATA #IMPLIED> \
";

void configure::Tuning::initialize (const char* filename) 
   throw (RuntimeException)
{
   xml::DTDMemory xmlDTD;
   xml::Parser xmlParser;
   xml::DocumentFile xmlDoc;
   
   xmlDoc.initialize (filename);   
   xmlDTD.initialize (st_dtd);
   
   const xml::Node* root = xmlParser.apply (xmlDoc, xmlDTD);
   xml::Node::const_iterator ii, maxii;
   const xml::Node* node;
   const xml::Attribute* attr;

   entry e;
   
   for (ii = root->begin (), maxii = root->end (); ii != maxii; ii ++) {
      node = xml::Node::node (ii);
      
      e.first = node->getAttribute ("Name")->getCStringValue ();
      
      if ((attr = node->getAttribute ("Key", false)) != NULL)
         e.second = attr->getIntegerValue ();
      else if ((attr = node->getAttribute ("Value", false)) != NULL)
         e.second = attr->getCStringValue ();
      else {
         string msg ("configure::Tuning::initialize | ResourceName: ");
         msg += e.first;
         msg += " | No indica ninguna seleccion";
         throw RuntimeException (msg, FILE_LOCATION);
      }

      LOGINFORMATION (
         string msg ("configure::Tuning::initialize | ResourceName: ");
         msg += e.first;
         msg += " | ";
         msg += e.second.asString ();
         Logger::information (msg, FILE_LOCATION);
      );
      
      a_container.push_back (e);
   }
}

configure::Tuning::const_iterator configure::Tuning::find (const ResourceName& resourceName) const
   throw ()
{
   for (const_iterator ii = begin (), maxii = end (); ii != maxii; ii ++) {
      if (resource_name (ii) == resourceName)
         return ii;
   }
   
   return end ();
}

string configure::Tuning::Selection::asString () const 
   throw ()
{
   string result;
   
   if (a_value != NULL) {
      result = "Value: ";
      result += *a_value;
   }
   else if (a_key != -1) 
      result += functions::asText ("Key: ", a_key);
   else
      result += "<null>";
   
   return result;
}
