#Solo es utilizable desde /var/tmp/develop

cp ../../bin.ss/libSmart.core.b/AUTHORS .
cp ../../bin.ss/libSmart.core.b/COPYING .
cp ../../bin.ss/libSmart.core.b/INSTALL .
cvs add COPYING AUTHORS INSTALL

cp -r ../../bin.ss/libSmart.core.b/gnu.install .

rm -r gnu.install/CVS
cvs add gnu.install

cd gnu.install 
cvs add COPYING INSTALL Makefile.am NEWS configure configure.in AUTHORS ChangeLog  README
rm -r *.o *.a aclocal.m4 Makefile.in config.log config.status depcomp .deps autom4te.cache Makefile
cd -

