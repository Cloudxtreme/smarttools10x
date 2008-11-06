/*
   Smart.analysis - Libreria para el analisis de depedencias entre componentes software.
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
#include <nemesis.Logger.h>

#include <smart.analysis.ASN1Source.h>

using namespace std;
using namespace nemesis;
using namespace smart;

//----------------------------------------------------------------------------------------
// (1) en el FROM solo se indica el nombre del archivo ASN1 que hay que importar, pero
// no indica la extension => ponemos la misma que tenga el archivo que estamos analizando.
//----------------------------------------------------------------------------------------
analysis::ASN1Source::ASN1Source (filesystem::Path* parent, const naming::File& file, const description::Rule* rule) :
   analysis::CSource (parent, file, rule),
   a_inFrom (false)
{
   setSingleCommentIndicator ("--");
   
   const std::string& name = file.getName ();   
   string::size_type pos = name.find_last_of ('.');
   
   const char* ext = name.c_str () + pos + 1;
   
   a_upperExtension = (nemesis_strcmp (ext, "ASN1") == 0);
}

/*
 * Permite indicar manualmente los archivos ASN1 de los que depente el archivo que está analizado.
 * 
 * Normalmente el componente del FROM <c> debería estar definido en el <c>.asn1, pero en el 
 * Prepago esta regla no se cumple. Asi vamos suponer que si se poner un "-- FROM" permitira
 * indicar manualmente los nombres de ASN1 a incluir
 */
bool analysis::ASN1Source::specialLine (char* line) 
   throw ()
{
   return (nemesis_strstr (line, "-- FROM") != NULL);
}   

//-------------------------------------------------------------------------------
// tiene que interpretar formas como:
//
// FROM
//    a, 
//    b,
//    c
// ;
//-------------------------------------------------------------------------------
bool analysis::ASN1Source::extractFile (char* line, naming::File& file) 
   throw ()
{
   static const int from_len = nemesis_strlen ("FROM");
   register char* aux (NULL);

   if (a_inFrom == false) {
      if ((aux = const_cast <char*> (nemesis_strstr (line, "FROM"))) == NULL)
         return false;

      aux += from_len;

      switch (*aux) {
         case 0: a_inFrom = true; return false;
         case ' ': 
         case '\t': a_inFrom = true; break;
      }
   }
   else 
      aux = line;

   if (a_inFrom == false)
      return false;
            
   while (*aux && (*aux == ' ' || *aux == '\t'))
      aux ++;

  if (*aux == 0)
      return false;

   bool result (false);

   char* includeName = aux;

   while (*aux && *aux != ';' && *aux != ',')
      aux ++;

   if (*aux == ';')
      a_inFrom = false;

   if (aux != includeName) {
      *aux = 0;
      char* extension = nemesis_strchr (includeName, '.');      
      if (extension != NULL) {
         *extension = 0;
         file.setName (includeName, extension + 1);
      }
      else
         file.setName (includeName, (a_upperExtension ? "ASN1": "asn1"));
      result = true;
   }
   
   return result;
}


