#
#   makefile para instalar la configuracion de las SmartTools 10.0/Evo-X
#   (c) Copyright 2005-2007 Telefonica Investigacion y Desarrollo. S.A. Unipersonal. 
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#   
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
#
VIEWER=@
EXTENSION=

# Se definen como variables para poder cambiarlos desde el exterior
BIN_DIR=../../bin.ss
MAKE_DIR=..

dataDirectory=$(MYPJ)/data
binDirectory=$(MYPJ)/bin

sourceMainBinary=$(BIN_DIR)/$(GMAKE_TARGET_DIR)
sourceGadgetBinary=$(MAKE_DIR)/$(GMAKE_TARGET_DIR)

binaryTarget=$(binDirectory)/$(GMAKE_SYSTEM)

checksumDirectory=$(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE)

all : $(dataDirectory) $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE) \
	$(binaryTarget) \
	$(dataDirectory)/config.xml \
	$(dataDirectory)/empty.include \
	$(binDirectory)/SmartConfigure \
	$(binDirectory)/upgrade_config \
	$(binaryTarget)/generate$(EXTENSION) $(binaryTarget)/browse$(EXTENSION) $(binaryTarget)/configure$(EXTENSION) \
	$(binaryTarget)/onlinehelp$(EXTENSION) \
	$(binaryTarget)/has_resources$(EXTENSION) \
	$(binaryTarget)/ils$(EXTENSION) \
	$(binaryTarget)/ils_mkdir$(EXTENSION) \
	$(binaryTarget)/smake$(EXTENSION) \
	$(binaryTarget)/checksum$(EXTENSION) \
	$(dataDirectory)/$(GMAKE_OSNAME)/.keycode \
	$(checksumDirectory)/.checksum 

clobber:
	@echo Borrando instalacion $(binaryTarget)
	@rm -rf $(binaryTarget)
   
clean: 
	@echo Borrando instalacion $(binaryTarget)
	@rm -rf $(binaryTarget)
	@echo Borrando datos $(dataDirectory)/$(GMAKE_OSNAME)
	@rm -rf  $(dataDirectory)/$(GMAKE_OSNAME)

data:
	@echo "Creando datos de configuracion " $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE) 
	$(VIEWER)rm -rf  $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE)
	$(VIEWER)mkdir -p $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE)
	$(VIEWER)cp -r $(GMAKE_OSNAME).schema/$(GMAKE_PROCESSORTYPE)/* $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE)
	$(VIEWER)find $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE) -name "CVS" -exec rm -rf {} \; -depth
	make -rs $(checksumDirectory)/.checksum
	
#data: 
#	@echo "Creando datos de configuracion " $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE) 
##	$(VIEWER)rm -rf $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE) $(dataDirectory)/$(GMAKE_OSNAME)/.keycode
#	$(VIEWER)mkdir -p $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE)
#	$(VIEWER)cp -r $(GMAKE_OSNAME).schema/$(GMAKE_PROCESSORTYPE)/* $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE)
#	$(VIEWER)find $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE) -name "CVS" -exec rm -rf {} \; -depth
#	@make -rs $(dataDirectory)/$(GMAKE_OSNAME)/.keycode

$(dataDirectory) :
	@echo "Creando " $(dataDirectory) 
	$(VIEWER)mkdir -p $(dataDirectory)
   
$(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE) : $(GMAKE_OSNAME).schema/$(GMAKE_PROCESSORTYPE) whats_new
	@echo "Creando datos de configuracion " $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE) 
	$(VIEWER)mkdir -p $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE)
	$(VIEWER)cp -r $(GMAKE_OSNAME).schema/$(GMAKE_PROCESSORTYPE)/* $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE)
	$(VIEWER)find $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE) -name "CVS" -exec rm -rf {} \; -depth
	$(VIEWER)find $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE) -name ".svn" -exec rm -rf {} \; -depth
   
$(binaryTarget) :
	@echo "Creando " $(binaryTarget)
	$(VIEWER)mkdir -p $(binaryTarget)
   
$(dataDirectory)/config.xml : config.xml
	@echo "Actualizando " $@
	$(VIEWER)cp config.xml $@
   
$(dataDirectory)/empty.include :
	@echo "Creando " $(dataDirectory)/empty.include 
	$(VIEWER)> $(dataDirectory)/empty.include 

 $(binDirectory)/SmartConfigure : ../configure.sh/SmartConfigure 
	@echo "Actualizando " $@
	$(VIEWER)cp ../configure.sh/SmartConfigure $@
  
 $(binDirectory)/upgrade_config : ../upgrade_config.sh/upgrade_config 
	@echo "Actualizando " $@
	$(VIEWER)cp ../upgrade_config.sh/upgrade_config $@
  
$(binaryTarget)/generate$(EXTENSION) : $(sourceMainBinary)/generate$(EXTENSION)
	@echo "Actualizando " $@
	$(VIEWER)cp $(sourceMainBinary)/generate$(EXTENSION) $@
  
$(binaryTarget)/browse$(EXTENSION) : $(sourceMainBinary)/browse$(EXTENSION)
	@echo "Actualizando " $@
	$(VIEWER)cp $(sourceMainBinary)/browse$(EXTENSION) $@

$(binaryTarget)/configure$(EXTENSION) : $(sourceMainBinary)/configure$(EXTENSION)
	@echo "Actualizando " $@
	$(VIEWER)cp $(sourceMainBinary)/configure$(EXTENSION) $@

$(binaryTarget)/onlinehelp$(EXTENSION) : $(sourceMainBinary)/onlinehelp$(EXTENSION)
	@echo "Actualizando " $@
	$(VIEWER)cp $(sourceMainBinary)/onlinehelp$(EXTENSION) $@

# Para permitir que se instale el SPAWN sin tener que haber creado el onlinehelp
$(sourceMainBinary)/onlinehelp$(EXTENSION) :
	> $(sourceMainBinary)/onlinehelp$(EXTENSION)

$(binaryTarget)/has_resources$(EXTENSION) : $(sourceGadgetBinary)/has_resources$(EXTENSION)
	@echo "Actualizando " $@
	$(VIEWER)cp $(sourceGadgetBinary)/has_resources$(EXTENSION) $@

$(binaryTarget)/ils$(EXTENSION) : $(sourceGadgetBinary)/ils$(EXTENSION)
	@echo "Actualizando " $@
	$(VIEWER)cp $(sourceGadgetBinary)/ils$(EXTENSION) $@

$(binaryTarget)/ils_mkdir$(EXTENSION) : $(sourceGadgetBinary)/ils_mkdir$(EXTENSION)
	@echo "Actualizando " $@
	$(VIEWER)cp $(sourceGadgetBinary)/ils_mkdir$(EXTENSION) $@

$(binaryTarget)/smake$(EXTENSION) : $(sourceGadgetBinary)/smake$(EXTENSION)
	@echo "Actualizando " $@
	$(VIEWER)cp $(sourceGadgetBinary)/smake$(EXTENSION) $@

$(binaryTarget)/checksum$(EXTENSION) : $(sourceGadgetBinary)/checksum$(EXTENSION)
	@echo "Actualizando " $@
	$(VIEWER)cp $(sourceGadgetBinary)/checksum$(EXTENSION) $@

$(dataDirectory)/$(GMAKE_OSNAME)/.keycode : $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE) config.xml \
	$(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE)
	@echo "Actualizando KeyCode " $(dataDirectory)/$(GMAKE_OSNAME)/.keycode
	$(VIEWER)$(binaryTarget)/checksum -tc $(TEXTCODE) > newkeycode
	$(VIEWER)if [ -f $(dataDirectory)/$(GMAKE_OSNAME)/.keycode ] ; \
	then \
		if diff newkeycode $(dataDirectory)/$(GMAKE_OSNAME)/.keycode > /dev/null ; \
		then \
			mv newkeycode $(dataDirectory)/$(GMAKE_OSNAME)/.keycode ; \
		else \
			exit 21 ; \
		fi ; \
	else \
		mv newkeycode $(dataDirectory)/$(GMAKE_OSNAME)/.keycode ; \
	fi ;

# Los nombres de archivos contenidos en el .checksum debe estar referenciados
# respecto del directorio $GMAKE_CONFIG que el directorio del que parte
# el 'smake' al ejecutar el CBS con la información contenida en el 
# fichero $(dataDirectory)/$(GMAKE_OSNAME)/$(GMAKE_PROCESSORTYPE)/.checksum
#
# Dónde dataDirectory = ..../data

$(checksumDirectory)/.checksum : $(dataDirectory)/$(GMAKE_OSNAME)/.keycode $(checksumDirectory) \
	$(dataDirectory)/config.xml
	@echo "Actualizando lista de verificacion " $(checksumDirectory)/.checksum 
	$(VIEWER)cd $(checksumDirectory) ; \
	keycode=`cat ../.keycode` ; \
	checksum=`$(binaryTarget)/checksum -kc $$$0keycode -f ${dataDirectory}/config.xml |cut -f 2 -d ' '` ; \
	echo ../../config.xml  $$$0checksum > /tmp/$(GMAKE_OSNAME).checksum ; \
	rm -rf  $(checksumDirectory)/.checksum ; \
	find . -type f | while read file ; \
	do \
		checksum=`$(binaryTarget)/checksum -kc $$$0keycode -f $$$0file |cut -f 2 -d ' '` ; \
		echo $$$0file $$$0checksum >> /tmp/$(GMAKE_OSNAME).checksum ; \
	done ; \
	mv /tmp/$(GMAKE_OSNAME).checksum .checksum ; 

