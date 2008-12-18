#!/usr/bin/ksh

# Permite ajustar los par�metros de invocaci�n a flex/lex 
# dependiendo del sistema operativo sobre el que lo estamos invocando.

# Hemos decidido esto despu�s de que en Solaris 10 fuera necesario instalar
# el paquete M4, y obtener el error:
# m4: fatal: libsigsegv.so.0: open failed: No such file or directory
# flex: error writing output file

osname=`uname -s`

if [ "$osname" != "SunOS" ]
then
#   echo flex $*
   flex $*
else
   osversion=`uname -r`
   if [ "$osversion" = "5.10" ]
   then
   # El comando recibido ser� flex.sh -l -o<fichero destino> <fichero origen>
   # Lo traducimos a formato LEX   
   # Recordar que la salida vendra como -o<y m�s cosas>, as� que hay que quitar
   # el "prefijo" -o para obtener el nombre del fichero de salida
      output=${2#-o}
#      echo "lex -t $3 > $output"
      lex -t $3 > $output
   else
#      echo flex $*
      flex $*
   fi
fi


