#Solo es utilizable desde /var/tmp/develop

# Ejecutar desde el directorio gnu.install

rm -f cc hh

find .. -name "*.cc" |while read a
do
   printf "%s " $a >> cc
done

find .. -name "*.h" |while read a
do
    printf "%s " $a >> hh
done

echo Resultados en archivos 'cc' y 'hh'
