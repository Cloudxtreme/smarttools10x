#!/usr/bin/ksh

# En Solaris, es necesario ejecutar el proc (64 bits), pero al compilar
# es posible que se requieran usar las librerías de 32 bits, por eso
# eso quita el post-fijo.

new_arg=""
typeset -i oversized=0
typeset -i len=0
real_oname=""
aux_oname=""

for ii in $* 
do
  name=`echo $ii | cut -f 1 -d '='`

  if [ "$name" != "oname" ]
  then
     new_arg=$new_arg" "$ii
  else
     real_oname=${ii#oname=}
     len=`echo $real_oname | wc -c`

     # Evita la limitacion del nombre del fichero de salida que 
     # tiene el compilador ProC/C++
     if [ $len -gt 127 ]
     then
        aux_oname=$$.cc
        new_arg=$new_arg"  oname="$aux_oname
     else
        new_arg=$new_arg" "$ii
     fi
  fi
done

echo new_arg: $new_arg
echo real_oname: $real_oname
echo aux_oname: $aux_oname

export ORACLE_HOME=$GMAKE_ORACLE_HOME/${GMAKE_ORACLE_VERSION%_32bits}
export LD_LIBRARY_PATH=${ORACLE_HOME}/lib 

if [ "$aux_oname" == "" ]
then
   ${ORACLE_HOME}/bin/proc $*
   exit $?
else
   ${ORACLE_HOME}/bin/proc $new_arg
   mv $aux_oname $real_oname
   exit $?
fi


