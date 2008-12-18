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
#ifndef _smart_description_Rule_h
#define _smart_description_Rule_h

#include <nemesis.RuntimeException.h>

#include <smart.description.MetaCommand.h>

namespace smart {

namespace naming {
   class FileClass;
   class File;
}

namespace description {

class RuleAgent;
class Target;
class MetaCommand;

using namespace nemesis;

class Rule {
public:
   //
   // Ojo: Todo lo que pueda corresponder a un directorio debe tener un Type::_v < Header
   //
   struct Type {
      enum _v { 
         FileSystem = 0x01, Executable = 0x02, Library = 0x04, LogicalGroup = 0x08, 
         HeaderRepository = 0x10, 
         Header = 0x20,  AnsiC = 0x40, ASN1 = 0x80, Plugin = 0x100
      };
      
      struct Translate {
         const char* text;
         Type::_v type;
      };

      static Translate st_translate [];

      static const char* asChar (const _v type) throw ();
      static _v asEnum (const char*) throw (RuntimeException);
   };

   typedef const Target* const* const_target_iterator;
   typedef const naming::FileClass* const* const_fileclass_iterator;

   const std::string& getName () const throw () { return a_name; }
   Type::_v getType () const throw () { return a_type; }
   const bool isAPath () const throw () { return a_type < Type::Header; }
   const bool isASource () const throw () { return a_type >= Type::AnsiC && a_type <= Type::ASN1; }
   const bool isACSource () const throw () { return a_type == Type::AnsiC; }
   const bool isAHeader () const throw () { return a_type == Type::Header; }
   const int getMask () const throw () { return a_mask; }   
   const bool generateHeader () const throw () { return a_generateHeader; }
   std::string getOutputName (const naming::File*) const throw (RuntimeException);
   std::string getAllOutputNames (const std::string& outputDirectory, const naming::File*) const throw (RuntimeException);
   const MetaCommand* getMetaCommand () const throw () { return a_metaCommand; }

   void setMask (const int mask) throw () { a_mask = mask; }

   void add (const naming::FileClass*) throw (RuntimeException);

   void createTarget (const naming::FileClass*, const char* suffix = NULL) throw (RuntimeException);
   MetaCommand* createMetaCommand () throw () { return a_metaCommand = new MetaCommand (this); }

   int target_size () const throw () { return a_maxTarget; }
   
   const_target_iterator target_begin () const throw () { return a_targets; }
   const_target_iterator target_end () const throw () { return a_targets + a_maxTarget; }
   static const Target* target (const_target_iterator& ii) throw () { return *ii; }

   const_fileclass_iterator fileclass_begin () const throw () { return a_classes; }
   const_fileclass_iterator fileclass_end () const throw () { return a_classes + a_maxClass; }
   static const naming::FileClass* fileclass (const_fileclass_iterator ii) throw () { return *ii; }

   std::string asString () const throw ();

private:      
   static const int MaxClass = 10;
   static const int MaxTarget = 10;

   const Type::_v a_type;
   const std::string a_name;
   const naming::FileClass* a_classes [MaxClass];
   int a_maxClass;   
   const Target* a_targets [MaxTarget];
   int a_maxTarget;
   bool a_generateHeader;
   int a_mask;
   MetaCommand* a_metaCommand;
   
   Rule (const std::string& name, const Type::_v type);
   void directAdd (const naming::FileClass*) throw (RuntimeException);   
   bool recognize (const naming::FileClass*) const throw ();
   
   friend class RuleAgent;
};

}
}

#endif

