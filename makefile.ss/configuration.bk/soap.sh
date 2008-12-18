# Ejmplo d regla con la que se invoca
#
# generate.Command.cc (82)
#.$(GMAKE_TARGET).X/example.SizeC.cc .$(GMAKE_TARGET).X/example.SizeH.h .$(GMAKE_TARGET).X/example.SizeStub.h .$(GMAKE_TARGET).X/example.SizeProxy.cc .$(GMAKE_TARGET).X/example.SizeProxy.h .$(GMAKE_TARGET).X/example.SizeService.cc .$(GMAKE_TARGET).X/example.SizeService.h : example.Size.gsoap
#        @printf "%s$(GMAKE_EOL)" example.Size.gsoap
#        $(VIEWER)$(SOAP) example.Size.gsoap $(SOAP_NAME) $(SOAP_HOME) $(SOAP_RPC_ENCODING) $(SOAP_SIDE) $(SOAP_FLAGS) $(USER_SOAP_FLAGS)
#
# generate.Command.cc (110)
#.$(GMAKE_TARGET).X/example.SizeC.o : .$(GMAKE_TARGET).X/example.SizeC.cc
#        @printf "%s$(GMAKE_EOL)" .$(GMAKE_TARGET).X/example.SizeC.cc
#        $(VIEWER)$(CPP) -c .$(GMAKE_TARGET).X/example.SizeC.cc  $(USER_INCLUDES) $(SYSTEM_INCLUDES) -o $@ $(CPP_DEBUG_INDICATOR) $(CPP_FLAGS) $(USER_CPP_FLAGS) $(USER_CPP_FLAG_RTTI) -DSmartTools_10_0

input=$1
tool_name=$2
soap_home=$3

shift 3

# Por ahora no se usa para nada, pero en un futuro podríamos soportar distintas utilidades de SOAP
if [ "$tool_name" != "gsoap" ]
then
   echo $0: Tool \"$tool_name\" is not recognized
   exit 1
fi

soap_version=`basename $GMAKE_SOAP`
echo soap: $soap_home/$GMAKE_SYSTEM/$soap_version/soapcpp2

#El soap generará:        		    .makefile espera
#   <namespace>C.cpp 			---> <filename>C.cc
#   <namespace>H.h   			---> <filename>H.h, también deja el original
#   <namespace><service>Proxy.cpp	---> <filename>Proxy.cc
#   <namespace><service>Proxy.h		---> <filename>Proxy.h, también deja el original
#   <namespace><service>Service.cpp	---> <filename>Service.cc
#   <namespace><service>Service.h	---> <filename>Service.h, también deja el original

$soap_home/$GMAKE_SYSTEM/$soap_version/soapcpp2 $* $input
result=$?

if [ ! $result -eq 0 ]
then
   exit $result
fi

filename=`basename ${input%.gsoap}`
#namespace=`grep namespace $input | grep -v service | cut -f 2 -d ' '`
namespace=`grep namespace $input | grep -v "//gsoap" | grep "{" | cut -f 2 -d ' '`
service=`grep "//gsoap" $input | grep "service" | grep name: | cut -f 2 -d ':'`

if [ "$namespace" == "" ]
then
   namespace="soap"
fi

echo filename: $filename--
echo namespace: $namespace--
echo service: $service--

cd $GMAKE_TARGET_DIR > /dev/null
mv ${namespace}C.cpp ${filename}C.cc
cp ${namespace}H.h ${filename}H.h

AUX=`printf "%s%sProxy.cpp" $namespace $service`
mv $AUX ${filename}Proxy.cc
echo Convirtiendo $AUX ${filename}Proxy.cc

AUX=`printf "%s%sProxy.h" $namespace $service`
cp $AUX ${filename}Proxy.h
echo Convirtiendo $AUX ${filename}Proxy.h

AUX=`printf "%s%sService.cpp" $namespace $service`
mv $AUX ${filename}Service.cc
echo Convirtiendo $AUX ${filename}Service.cc

AUX=`printf "%s%sService.h" $namespace $service`
cp $AUX ${filename}Service.h
echo Convirtiendo $AUX ${filename}Service.h

exit $result
