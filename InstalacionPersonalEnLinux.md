#Esta página explica como instalar una versión personal sobre Linux.

# Introducción #

SmartTools 10.y/Evo-X requiere los módulos más básicos de la **Suite NemesisRD 1.x** , por tanto, la instalación tiene dos fases claramente diferenciadas; en la primera fase tendremos que instalar los cuatro módulos básicos de NemesisRD, en la segunda fase generaremos la instalación de SmartTools 10.x propiamente dicha.

La **Suite NemesisRD 1.x** está disponible en el proyecto http://code.google.com/p/nemesisrd/


Los módulos de NemesisRD 1.x requeridos por las SmartTools 10.x son: NemesisRD.core, NemesisRD.io, NemesisRD.xml y NemesisRD.app.

Para compilar correctamente NemesisRD.xml es necesario verificar con el comando "rpm -aq |grep xml" y con "rpm -aq | grep sub" que tenemos instalados, al menos, los siguientes paquetes.

  * libxml2-2.5.10-5
  * xml-common-0.6.3-14
  * libxml-1.8.17-9.1
  * libxml2-devel-2.5.10-5
  * subversion-1.1.4-2.ent

Ten en cuenta que la versión específica de las librerías no es demasiado importante. Si no están instalados es muy probable que estén disponibles en los discos de instalación del sistema o en http://rpm.pbone.net/index.php3

# Fase 1. Instalación de los paquete básicos de NemesisRD 1.x #

En esta fase vamos a obtener y crear las librerías de los cuatro paquetes de NemesisRD 1.x que hemos usado en el desarrollo de las SmartTools. A continuación se detallan los comandos a ejecutar.

```
mkdir -p /var/tmp/develop
cd /var/tmp/develop
svn checkout -N http://nemesisrd.googlecode.com/svn/trunk/ NemesisRD
cd NemesisRD

svn checkout -N http://nemesisrd.googlecode.com/svn/trunk/official.ss official.ss
 
svn checkout  http://nemesisrd.googlecode.com/svn/trunk/official.ss/libNemesis.core.b official.ss/libNemesis.core.b

svn checkout  http://nemesisrd.googlecode.com/svn/trunk/official.ss/libNemesis.io.b official.ss/libNemesis.io.b

svn checkout  http://nemesisrd.googlecode.com/svn/trunk/official.ss/libNemesis.xml.b official.ss/libNemesis.xml.b

svn checkout  http://nemesisrd.googlecode.com/svn/trunk/official.ss/libNemesis.app.b official.ss/libNemesis.app.b

./linux.spawn
```

Este último comando generará las librerías y ubicará los archivos de cabecera en el directorio donde las SmarTools esperan encontrarlos; para ello hace uso de las autotools. Si todo ha ido bien deberías tener un directorio /var/tmp/develop/nemesis con el siguiente árbol de directorios:

```
./lib
./include/core
./include/io
./include/xml
./include/app
```

El directorio /var/tmp/develop/nemesis/lib contendrá algo parecido a la lista:

```
-rw-r--r--    1 comun    cvs        503152 nov  6 11:52 libNemesis.app.a
-rw-r--r--    1 comun    cvs       1843664 nov  6 11:52 libNemesis.core.a
-rw-r--r--    1 comun    cvs        335760 nov  6 11:52 libNemesis.io.a
-rw-r--r--    1 comun    cvs       1344950 nov  6 11:52 libNemesis.xml.a
```

# Fase 2. Instalación personal de las SmartTools #

En esta fase vamos a crear e instalar los archivos requeridos para crear una instalación personal de SmartTools 1.x/Evo-X. A continuación se detallan los comandos a ejecutar:

```
cd /var/tmp/develop

svn checkout http://smarttools10x.googlecode.com/svn/trunk/ SmartTools.pj

cd SmartTools.pj

./linux.spawn
```

Si todo ha ido como esperamos sólo tendrás que escribir el archivo de configuración y hacerlo apuntar con la variable GMAKE\_HOME al directorio /var/tmp/develop/SmartTools.pj.

# Ejemplo de fichero de configuración #

A continuación os mostramos el fichero de configuración usado en NemesisRD 1.x usado para generar la versión completa de la Suite:

```
export GMAKE_HOME=/var/tmp/develop/SmartTools.pj
export MYPJ=`pwd`
export BASEPJ=`pwd`

. $GMAKE_HOME/bin/SmartConfigure -noauto -t warning 
```

Para más información ver http://smarttools10x.googlecode.com/files/UserManual-SmartToolsV10EvoX.pdf