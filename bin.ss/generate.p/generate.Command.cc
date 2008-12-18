/*
   generate - Version 10.
   Herramienta que crea los archivos de dependencias necesarios para crear de forma optima los ejecutables.

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
#include <stdio.h>

#include <algorithm>

#include <nemesis.defines.h>
#include <nemesis.Logger.h>

#include <smart.naming.File.h>

#include <smart.generate.Command.h>

#include <smart.description.RuleAgent.h>
#include <smart.description.Rule.h>
#include <smart.description.Target.h>
#include <smart.description.functions.h>
#include <smart.description.MetaCommand.h>
#include <smart.description.MetaLine.h>

#include <smart.analysis.CSource.h>
#include <smart.analysis.filesystem.Node.h>

#include <smart.generate.Makefile.h>
#include <smart.generate.Structure.h>

using namespace std;
using namespace nemesis;
using namespace smart;

generate::Command::Command () : 
   Prototype (Type::Command), 
   a_includes (true),
   a_dependences (true),
   a_predecessorNode (NULL)
{
}

void generate::Command::initialize (generate::Prototype* predecessor, const analysis::filesystem::Node* node) 
   throw (RuntimeException)
{
   Prototype::initialize (predecessor, node);   
   a_predecessorNode = predecessor->getNode ();
}

//----------------------------------------------------------------------------------------------
// Escribe las reglas de creacion de este archivo fuente. Recordar que todas las dependencias
// de un .p con su .db y/o .d han sido aplanadas durante el proceso de sintesis realizado por
// generate::SourceContainer.
// 
// (2) La primera iteracion sacara todas las dependencias del nodo actual, en las siquientes
// cada nodo solo dependera de su fuente anterior.
// (3) Busca la siquiente regla a aplicar.
//----------------------------------------------------------------------------------------------
void generate::Command::execute () 
   throw (RuntimeException)
{
   using namespace description;
   
   const Rule* rule = a_node->getRule ();   
   Makefile& makefile = static_cast <Structure*> (a_predecessor)->getMakefile ();      
   const analysis::CSource* csource = static_cast <const analysis::CSource*> (a_node);

   makefile.debug (FILE_LOCATION);
   string ofilename = writeOutput (makefile, rule, *csource);
   string ifilename = writeFirstInput (makefile, csource);
   writeCommand (makefile, rule, csource, ifilename, ofilename);
   
   forward (makefile, csource);
}
   
void generate::Command::forward (generate::Makefile& makefile,   const analysis::CSource* csource) 
   throw (RuntimeException)
{
   using namespace description;
   
   const Rule* rule = csource->getRule ();
   
   const Target* target;
   naming::File nextFile;
   string ofilename, ifilename;
   
   for (Rule::const_target_iterator tt = rule->target_begin (), maxtt = rule->target_end (); tt != maxtt; tt ++) {
      rule = (target = Rule::target (tt))->getRule ();

      if (rule->isASource () == false)
         continue;
      
      nextFile = *csource;
      target->rename (nextFile); 
      
      makefile.debug (FILE_LOCATION);
      ofilename = writeOutput (makefile, rule, nextFile);
      ifilename = writeOtherInput (makefile, csource, nextFile);
      writeCommand (makefile, rule, csource, ifilename, ofilename);
      
      LOGDEBUG (
         string msg ("generate::Command::forward | ");
         msg += rule->asString ();
         msg += " | NextFile: ";
         msg += nextFile.getName ();
         Logger::debug (msg, FILE_LOCATION);
      );
      
   }
   
   makefile.newline ();
}

//---------------------------------------------------------------------------------------------------
// Escribe la parte izquierda de la expresion 
// aa.cc aa.h : aa.asn1 (p.e).
//
// (1) Recordar que un mismo Target puede generar mas de un archivo. .asn1 => .cc y .h. p.e.
// (2) Al final el resultado ser� el corresponiente a la �ltima extensi�n indicada a la hora de
//     declarar la regla en el config.xml
//---------------------------------------------------------------------------------------------------
string generate::Command::writeOutput (generate::Makefile& makefile, const description::Rule* rule, const naming::File& input) 
   throw (RuntimeException)
{
   using namespace description;
   
   string result;

   if (rule->target_size () > 0) {                                                                       // (1)
      const Target* target;
      Rule::const_target_iterator tt, maxtt;
      
      naming::File output (input);
      
      for (tt = rule->target_begin (), maxtt = rule->target_end (); tt != maxtt; tt ++) {                // (2))
          target = Rule::target (tt);
          target->rename (output);
          makefile.write (result = getRelativeOutput (output.getName (), output));
          makefile.space ();
          output = input;
       }      
   }
   else {             
      makefile.write (result = getRelativeOutput (rule->getOutputName (&input), input));
      makefile.space ();
   }

   LOGDEBUG (
      string msg ("generate::Command::writeOutput | Input: ");
      msg += input.asString ();
      msg += " | Result: ";
      msg += result;
      Logger::debug (msg, FILE_LOCATION);
   );

   makefile.write (": ");
   
   return result;
}

//---------------------------------------------------------------------------------------------------
// Escribe la parte derecha de la expresion:
// aa.cc aa.h : aa.asn1 (p.e).
//
// (1) La primera iteracion sacara todas las dependencias del nodo actual, en las siquientes
// cada nodo solo dependera de su fuente anterior.
// (2) Ojo!! El path sera relativo al padre de este nodo, que sera el directorio que contedra
// la estructura de arbol donde se encuentra el archivo que estamos analizando.
//---------------------------------------------------------------------------------------------------
string generate::Command::writeFirstInput (generate::Makefile& makefile, const analysis::CSource* csource)
   throw (RuntimeException)
{   
   using namespace analysis;
   using namespace description;
   
   string result (csource->getRelativePath (a_predecessorNode));
     
   makefile.write (result);         
   
   CSource::const_include_iterator jj, maxjj;
   const analysis::filesystem::Node* include;

   for (jj = csource->include_begin (), maxjj = csource->include_end (); jj != maxjj; jj ++) {
      include = CSource::include (jj);         
      makefile.write (" \\\n\t");         
      makefile.write (include->getRelativePath (a_predecessorNode));
   }      

   LOGDEBUG (
      string msg ("generate::Command::writeFirstInput | Input: ");
      msg += csource->asString ();
      msg += " | Result: ";
      msg += result;
      Logger::debug (msg, FILE_LOCATION);
   );
   
   makefile.newline ();
   
   return result;
}

//---------------------------------------------------------------------------------------------------
// Escribe la parte derecha de la expresion:
// aa.cc aa.h : aa.asn1 (p.e).
//
// (1) La primera iteracion sacara todas las dependencias del nodo actual, en las siquientes
// cada nodo solo dependera de su fuente anterior.
// (2) Ojo!! El path sera relativo al padre de este nodo, que sera el directorio que contedra
// la estructura de arbol donde se encuentra el archivo que estamos analizando.
//---------------------------------------------------------------------------------------------------
string generate::Command::writeOtherInput (generate::Makefile& makefile, const analysis::CSource* csource, const naming::File& nextFile)
   throw (RuntimeException)
{   
   using namespace analysis;
   using namespace description;

   string result (getRelativeOutput (nextFile.getName (), nextFile));
   
   makefile.write (result);
   
   RuleAgent& ruleAgent = RuleAgent::instanciate ();
   CSource::const_include_iterator jj, maxjj;
   const filesystem::Node* include;
   const Rule* rule (NULL);
   Rule::const_target_iterator tt, maxtt;
   naming::File aux;
   const Target* target;
  
   CSource* auxCSource = const_cast <CSource*> (csource);
      
   for (jj = csource->include_begin (), maxjj = csource->include_end (); jj != maxjj; jj ++) {
      include = CSource::include (jj);
      
      if (include->isAHeader () == true) {
         makefile.write (" \\\n\t");         
         makefile.write (include->getRelativePath (a_predecessorNode));
      }
      else {
         if ((rule = include->getRule ())->generateHeader () == false)
            continue;
                  
         aux = *include;
         for (tt = rule->target_begin (), maxtt = rule->target_end (); tt != maxtt; tt ++) {
            target = Rule::target (tt);
            target->rename (aux);
            
            if ((rule = ruleAgent.find (aux.getFileClass ())) == NULL)
               continue;
            
            if (rule->isAHeader () == false) 
               continue;
              
            if ((include = auxCSource->search (auxCSource->getPath (), aux)) == NULL) {
               string msg ("Header generado ");
               msg += aux.asString ();
               msg += " no encontrado";
               throw RuntimeException (msg, FILE_LOCATION);
            }
            
            makefile.write (" \\\n\t");         
            makefile.write (include->getRelativePath (a_predecessorNode));
            break;
         }
      }
   }      

   LOGDEBUG (
      string msg ("generate::Command::writeOtherInput | Input: ");
      msg += csource->asString ();
      msg += " | Result: ";
      msg += result;
      Logger::debug (msg, FILE_LOCATION);
   );
   
   makefile.newline ();
   
   return result;
}

//---------------------------------------------------------------------------------------------------
// (1) En la primera fase saca la lista de directorios en las que este fuente tiene dependencias.
//  y luego conforma el texto con los directorios donde buscar segun el esquema del MetaLine.
// (2) Recordar que analysis::ASN1Source hereda de analysis::CSource. Pone los nombres de dependencias
// relativos al directorio desde el que se esta ejecutando.
// (3) Pone los nombres de dependencias relativos a los nombres de variable de entorno a partir
// del cual se pueden encontar el archivo.
//---------------------------------------------------------------------------------------------------
void generate::Command::writeCommand (generate::Makefile& makefile, const description::Rule* rule, const analysis::CSource* csource,
   const string& ifilename, const string& ofilename) 
   throw (RuntimeException)
{
   using namespace description;
   using namespace analysis;
   
   makefile.write ("\t@printf \"%s$(GMAKE_EOL)\" ");
   makefile.writenl (ifilename);

   const MetaCommand* metaCommand;
   
   if ((metaCommand = rule->getMetaCommand ()) == NULL) {
      string msg (rule->asString ());
      msg += " | Requiere tener asociado un description::MetaCommand";
      throw RuntimeException (msg, FILE_LOCATION);
   }   
   
   MetaCommand::const_metaline_iterator ii,maxii;
   const MetaLine* metaLine;
   const char* pattern;
   
   for (ii = metaCommand->metaline_begin (), maxii = metaCommand->metaline_end (); ii != maxii; ii ++) {
      metaLine = MetaCommand::metaline (ii);

      a_buffer = pattern = metaLine->getPattern ();
            
      if (metaLine->hasDynamicText (MetaLine::DynamicText::Input)) 
         setText (MetaLine::DynamicText::Input, ifilename.c_str ());        

      if (metaLine->hasDynamicText (MetaLine::DynamicText::InputOnlyName)) {
         const char* onlyName = ifilename.c_str ();
         
         string::size_type pos = ifilename.find_last_of ('/');
         
         if (pos != string::npos)
            onlyName += pos + 1;
         
         setText (MetaLine::DynamicText::InputOnlyName, onlyName);        
      }
      
      if (metaLine->hasDynamicText (MetaLine::DynamicText::Includes)) {                // (1)
         analysis::CSource::const_include_iterator jj, maxjj;
         const naming::File* directory;
      
         for (jj = csource->include_begin (), maxjj = csource->include_end (); jj != maxjj; jj ++) {
            directory = CSource::include (jj)->getParent ();            
            if (find (directory_begin (), directory_end (), directory) == directory_end ())
               a_includeDirectories.push_back (directory);               
         }
         
         a_includes.clear ();
         const char* indicator = metaLine->getIndicator (MetaLine::DynamicText::Includes);
         const int lindicator = nemesis_strlen (indicator);
         for (directory_iterator dd = directory_begin (), maxdd = directory_end (); dd != maxdd; dd ++) {
            a_includes.append (indicator, lindicator);
            a_includes += Command::directory (dd)->getRelativePath (a_predecessorNode);
            a_includes += ' ';            
         }
         
         a_includes += char (0);
         setText (MetaLine::DynamicText::Includes, a_includes.getData ());
      }
      
      if (metaLine->hasDynamicText (MetaLine::DynamicText::Dependences)) {             // (2)
         analysis::CSource::const_include_iterator jj, maxjj;
         const analysis::filesystem::Node* include;
         const char* indicator = metaLine->getIndicator (MetaLine::DynamicText::Dependences);
         const int lindicator = nemesis_strlen (indicator);
      
         a_dependences.clear ();
         for (jj = csource->include_begin (), maxjj = csource->include_end (); jj != maxjj; jj ++) {
            include = CSource::include (jj);
            a_dependences.append (indicator, lindicator);
            a_dependences += include->getRelativePath (a_predecessorNode);
            a_dependences += ' ';
         }
         
         a_dependences += char (0);         
         setText (MetaLine::DynamicText::Dependences, a_dependences.getData ());
      }

      if (metaLine->hasDynamicText (MetaLine::DynamicText::VDependences)) {             // (3)
         analysis::CSource::const_include_iterator jj, maxjj;
         const analysis::filesystem::Node* include;
         const char* indicator = metaLine->getIndicator (MetaLine::DynamicText::VDependences);
         const int lindicator = nemesis_strlen (indicator);
      
         a_dependences.clear ();
         for (jj = csource->include_begin (), maxjj = csource->include_end (); jj != maxjj; jj ++) {
            include = CSource::include (jj);
            
            const string& vpath = include->getVariablePath (); 
            
            if (vpath.length () > 0) {
               a_dependences.append (indicator, lindicator);
               a_dependences += vpath;
               a_dependences += ' ';
            }
         }
         
         a_dependences += char (0);         
         setText (MetaLine::DynamicText::VDependences, a_dependences.getData ());
      }

      if (metaLine->hasDynamicText (MetaLine::DynamicText::Output)) 
         setText (MetaLine::DynamicText::Output, ofilename.c_str ());

      makefile.write (a_buffer);
      makefile.newline ();
   }     
   
   makefile.newline ();
}

void generate::Command::clear () 
   throw ()
{
   Prototype::clear ();      
   a_buffer.clear ();
   a_includes.clear ();
   a_dependences.clear ();
   a_includeDirectories.clear ();
}

string generate::Command::getRelativeOutput (const std::string& outputName, const naming::File& node)
   throw (RuntimeException)
{
   const naming::File* myLogicalParent = a_predecessorNode;
   const naming::File* parent = node.getParent ();

   string result ("");
   
   while (myLogicalParent != parent && parent != NULL) {
      result.insert (0, "/");
      result.insert (0, parent->getName ());
      parent = parent->getParent ();
   }
   
   result += description::functions::getTargetDirectory ();
   result += '/';
   return result += outputName;
}

void generate::Command::setText (description::MetaLine::DynamicText::_v v, const char* text) 
   throw (RuntimeException)
{
   const char* id = description::MetaLine::DynamicText::asLiteral (v);
   
   string::size_type pos = a_buffer.find (id);
   
   if (pos != string::npos) 
      a_buffer.replace (pos, nemesis_strlen (id), text);  
}

