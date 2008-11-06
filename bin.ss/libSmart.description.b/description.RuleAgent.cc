/*
   Smart.description - Version 10.
   Libreria para la descripcion de los elementos software con los que vamos a trabajar.

   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2007 Telefonica Investigacion y Desarrollo. S.A. Unipersonal. 

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/
#include <nemesis.defines.h>
#include <nemesis.Tokenizer.h>
#include <nemesis.Logger.h>

#include <nemesis.xml.DTDMemory.h>
#include <nemesis.xml.Document.h>
#include <nemesis.xml.Parser.h>
#include <nemesis.xml.Node.h>
#include <nemesis.xml.Attribute.h>
#include <nemesis.xml.Text.h>

#include <smart.naming.File.h>
#include <smart.naming.FileClass.h>

#include <smart.description.RuleAgent.h>
#include <smart.description.Rule.h>
#include <smart.description.functions.h>
#include <smart.description.MetaLine.h>

using namespace std;
using namespace nemesis;
using namespace smart;

const char* description::RuleAgent::st_dtd = "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?> \
<!ELEMENT Description (TargetDirectory,Project,Makefile,Rule+)> \
<!ATTLIST Description Version CDATA #REQUIRED> \
<!ATTLIST Description Owner CDATA #REQUIRED> \
\
<!ELEMENT Project EMPTY> \
<!ATTLIST Project Configure CDATA #REQUIRED> \
<!ATTLIST Project Tune CDATA #REQUIRED> \
\
<!ELEMENT Makefile EMPTY> \
<!ATTLIST Makefile Name CDATA #REQUIRED> \
<!ATTLIST Makefile Configure CDATA #REQUIRED> \
<!ATTLIST Makefile Resource CDATA #REQUIRED> \
\
<!ELEMENT TargetDirectory EMPTY> \
<!ATTLIST TargetDirectory Prefix CDATA #REQUIRED> \
<!ATTLIST TargetDirectory Name CDATA #REQUIRED> \
<!ATTLIST TargetDirectory Extension CDATA #REQUIRED> \
\
<!ELEMENT Rule (FileClass,(Target|FineTarget)?,RelationShip?,Command?)> \
<!ATTLIST Rule Name CDATA #REQUIRED> \
<!ATTLIST Rule Type (FileSystem|Executable|Library|LogicalGroup|Header|AnsiC|ASN1|Plugin) \"AnsiC\"> \
\
<!ELEMENT FileClass EMPTY> \
<!ATTLIST FileClass Values CDATA #REQUIRED> \
\
<!ELEMENT RelationShip EMPTY> \
<!ATTLIST RelationShip Values CDATA #REQUIRED> \
\
<!ELEMENT Target EMPTY> \
<!ATTLIST Target Extensions CDATA #REQUIRED> \
<!ATTLIST Target Suffix CDATA #IMPLIED> \
\
<!ELEMENT FineTarget (FineEntry)+> \
<!ELEMENT FineEntry EMPTY> \
<!ATTLIST FineEntry Extensions CDATA #REQUIRED> \
<!ATTLIST FineEntry Suffix CDATA #IMPLIED> \
\
<!ELEMENT Command (Line+)> \
<!ELEMENT Line (Text,(Input,Text?)?,(InputOnlyName,Text?)?,(Includes,Text?)?,((Dependences|VDependences),Text?)?,(Output,Text?)?)> \
\
<!ELEMENT Text (#PCDATA)> \
\
<!ELEMENT Input EMPTY> \
<!ATTLIST Input Indicator CDATA #REQUIRED> \
\
<!ELEMENT InputOnlyName EMPTY> \
<!ATTLIST InputOnlyName Indicator CDATA #REQUIRED> \
\
<!ELEMENT Includes EMPTY> \
<!ATTLIST Includes Indicator CDATA #REQUIRED> \
\
<!ELEMENT Dependences EMPTY> \
<!ATTLIST Dependences Indicator CDATA #REQUIRED> \
\
<!ELEMENT VDependences EMPTY> \
<!ATTLIST VDependences Indicator CDATA #REQUIRED> \
\
<!ELEMENT Output EMPTY> \
<!ATTLIST Output Indicator CDATA #REQUIRED> \
";

description::RuleAgent::RuleAgent () :
   a_cache (NULL)
{
   Rule* rule = create ("HeaderRepository", Rule::Type::HeaderRepository);
   
   try {
      rule->setMask (Rule::Type::Header);
      a_ruleHeaderRepository = rule;
      
      rule = create ("Header", Rule::Type::Header);
      rule->directAdd (naming::FileClass::create ("h"));
      rule->directAdd (naming::FileClass::create ("hh"));
      rule->directAdd (naming::FileClass::create ("hxx"));
      rule->directAdd (naming::FileClass::create ("hpp")); 
   }
   catch (RuntimeException& ex) {
      ex.trace ();
   }
}

//----------------------------------------------------------------------------------
// Configura el sistema segun el documento XML recibido como parametro.
//
// En el primer bucle define el nombre y tipo de las reglas, crea las extensiones
// de archivo reconocidas por el sistema.
//
// En el segundo bucle (cuando ya estan definidas todas las extensiones reconocidas)
// enlaza las reglas con sus TARGET's asociados.
// 
// (1)Recordar que la primera regla siempre sera la referente a los HEADERs, que 
// se mete automaticamente al instanciar esta instancia.
//----------------------------------------------------------------------------------
void description::RuleAgent::config (const xml::Document& document)
   throw (RuntimeException)
{
   xml::DTDMemory xmlDTD;
   xml::Parser xmlParser;
   
   xmlDTD.initialize (st_dtd);
   
   const xml::Node* root = xmlParser.apply (document, xmlDTD);
   const xml::Node* node;   
   const xml::Node* ww;
   Rule* rule;
   Tokenizer tt;
   int mask;
   MetaCommand* command;
   MetaLine* line;   
   
   functions::setup (root);
   
   xml::Node::const_iterator ii, maxii;
   xml::Node::const_iterator jj, maxjj;
   Tokenizer::const_iterator kk, maxkk;
   xml::Node::const_iterator ll, maxll;
   xml::Node::const_iterator nn, maxnn;
   xml::Node::const_iterator first_rule;
   bool first (true);
   
   struct {
      const char* name;
      MetaLine::DynamicText::_v dynamicText;
      MetaLine::StaticText::_v staticText;
   } nodes [] =  {
      { "Input", MetaLine::DynamicText::Input, MetaLine::StaticText::PostInput },
      { "InputOnlyName", MetaLine::DynamicText::InputOnlyName, MetaLine::StaticText::PostInputOnlyName },
      { "Includes", MetaLine::DynamicText::Includes, MetaLine::StaticText::PostIncludes },
      { "Dependences", MetaLine::DynamicText::Dependences, MetaLine::StaticText::PostDependences },
      { "VDependences", MetaLine::DynamicText::VDependences, MetaLine::StaticText::PostVDependences },
      { "Output", MetaLine::DynamicText::Output, MetaLine::StaticText::PostOutput },
      { NULL, MetaLine::DynamicText::End, MetaLine::StaticText::End }
   };            
   
   for (ii = root->begin (), maxii = root->end (); ii != maxii; ii ++) {
      node = xml::Node::node (ii);
      
      if (nemesis_strcmp ("Rule", node->getName ()) == 0) {
         if (first == true) {
            first = false;
            first_rule = ii;
         }
      }
      else
         continue;
      
      rule = create (
         node->getAttribute ("Name")->getValue (), 
         Rule::Type::asEnum (node->getAttribute ("Type")->getCStringValue ())
      );
      
      for (jj = node->begin (), maxjj = node->end (); jj != maxjj; jj ++) {
         ww = xml::Node::node (jj);
         
         if (nemesis_strcmp (ww->getName (), "FileClass") == 0) {
            tt.apply (ww->getAttribute ("Values")->getValue (), "|");
      
            for (kk = tt.begin (), maxkk = tt.end (); kk != maxkk; kk ++) 
               rule->add (naming::FileClass::create (Tokenizer::data (kk).c_str ()));                   
         }
         else if (nemesis_strcmp (ww->getName (), "RelationShip") == 0) {         
            mask = 0;
            tt.apply (ww->getAttribute ("Values")->getValue (), "|");

            for (kk = tt.begin (), maxkk = tt.end (); kk != maxkk; kk ++) 
               mask |= Rule::Type::asEnum (Tokenizer::data (kk).c_str ());

            rule->setMask (mask);
         }
         else if (nemesis_strcmp (ww->getName (), "Command") == 0) {
            if (rule->isASource () == false) {
               string msg (rule->asString ());
               msg += " | Ya tiene un comando implicito definido en el sistema";
               Logger::warning (msg, FILE_LOCATION);
               continue;
            }
         
            command = rule->createMetaCommand ();
            
            for (ll = ww->begin (), maxll = ww->end (); ll != maxll; ll ++) {
               ww = xml::Node::node (ll);               
               
               nn = ww->begin ();
               maxnn = ww->end ();
               
               if (nn == maxnn) 
                  continue;

               line = command->createLine ();                                 
               line->setText (MetaLine::StaticText::Beginning, xml::Node::node (nn)->getText ()->getValue ());
               
               if (++ nn == maxnn) 
                  continue;
                  
               ww = xml::Node::node (nn);
               
               for (int index = 0; nodes [index].name != NULL; index ++) {
                  if (nemesis_strcmp (ww->getName (), nodes [index].name) != 0)
                     continue;
                     
                  line->setIndicator (nodes [index].dynamicText, ww->getAttribute ("Indicator")->getValue ());

                  if (++ nn == maxnn)
                     break;
                     
                  ww = xml::Node::node (nn);
                  
                  if (nemesis_strcmp (ww->getName (), "Text") == 0) {
                     line->setText (nodes [index].staticText, ww->getText ()->getValue ());
                     
                     if (++ nn == maxnn)
                        break;
                        
                     ww = xml::Node::node (nn);
                  }                  
               }               
            }            
            command->compile ();
         }
      }
   }

   iterator rr = begin () + 2, maxrr = end ();                    // (1)
   
   for (ii = first_rule; ii != maxii; ii ++, rr ++) {
      rule = RuleAgent::rule (rr);      
      
      if ((ww = xml::Node::node (ii)->find ("Target", false)) != NULL)
         append (rule, ww, tt);
      else if ((ww = xml::Node::node (ii)->find ("FineTarget", false)) != NULL) {
         for (jj = ww->begin (), maxjj = ww->end (); jj != maxjj; jj ++)
            append (rule, xml::Node::node (jj), tt);
      }         
   }   
   
   LOGINFORMATION (
      for (register iterator ii = begin (), maxii = end (); ii != maxii; ii ++) 
         Logger::information (RuleAgent::rule (ii)->asString (), FILE_LOCATION);
   );      
}

description::Rule* description::RuleAgent::create (const string& name, const description::Rule::Type::_v type)
   throw (RuntimeException)
{
   description::Rule* result (NULL);
   description::Rule* rule;

   for (register iterator ii = begin (), maxii = end (); ii != maxii; ii ++) {
      rule = RuleAgent::rule (ii);
      
      if (rule->getName () == name) {
         result = rule;
         break;
      }
   }   
   
   if (result == NULL) {
      result = new Rule (name, type);
      a_rules.push_back (result);
   }
   
   return result;
}

void description::RuleAgent::append (description::Rule* rule, const xml::Node* ww, Tokenizer& tt)
   throw (RuntimeException)
{
   Tokenizer::const_iterator kk, maxkk;
   const naming::FileClass* fileClass;
   const xml::Attribute* attr;
   const char* suffix;
   
   tt.apply (ww->getAttribute ("Extensions")->getValue (), "|");

   for (kk = tt.begin (), maxkk = tt.end (); kk != maxkk; kk ++) {
      fileClass = naming::FileClass::instance (Tokenizer::data (kk).c_str ());

      if (fileClass == NULL) {
         string msg (rule->asString ());
         msg += " | FileClass: ";
         msg += Tokenizer::data (kk);
         msg += " | Target de regla no registrado";
         throw RuntimeException (msg, FILE_LOCATION);
      }

      if ((attr = ww->getAttribute ("Suffix", false)) == NULL)
         suffix = NULL;
      else
         suffix = attr->getCStringValue ();

      rule->createTarget (fileClass, suffix);            
   }
}


bool description::RuleAgent::recognizeHeaderRepository (const naming::File& file) const 
   throw ()
{
   static const char* names [] = { "hdrs", "include", "includes", NULL };

   const char* name = file.getName ().c_str ();
   bool result (false);
   
   for (register int i = 0; names [i] != NULL; i ++) {
      if (nemesis_strcmp (name, names [i]) == 0) {
         result = true;
         break;
      }
   }
   
   return result;
}

const description::Rule* description::RuleAgent::find (const naming::FileClass* fileClass) const 
   throw ()
{   
   if (fileClass == NULL) 
      return NULL;
 
   const Rule* result (NULL);
  
   if (a_cache != NULL && a_cache->recognize (fileClass) == true) 
      result = a_cache;         
   
   if (result == NULL) {
      const Rule* rule;

      for (register const_iterator ii = begin (), maxii = end (); ii != maxii; ii ++) {
         rule = RuleAgent::rule (ii);

         if (rule->recognize (fileClass) == true) {
            result = const_cast <RuleAgent*> (this)->a_cache = rule;
            break;
         }
      }
   }
   
   return result;
}
