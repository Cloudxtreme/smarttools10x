# Ejmplo d regla con la que se invoca
#
#.$(GMAKE_TARGET).X/SizeC.cc .$(GMAKE_TARGET).X/SizeH.h .$(GMAKE_TARGET).X/SizeStub.h .$(GMAKE_TARGET).X/SizeProxy.cc .$(GMAKE_TARGET).X/SizeProxy.h .$(GMAKE_TARGET).X/SizeService.cc .$(GMAKE_TARGET).X/SizeService.h : Size.wsdl
#        @printf "%s$(GMAKE_EOL)" Size.wsdl
#        $(VIEWER)$(WSDL) Size.wsdl $(SOAP_NAME) $(SOAP_HOME) $(WSDL_FLAGS) $(USER_WSDL_FLAGS) $(SOAP_RPC_ENCODING) $(SOAP_SIDE) $(SOAP_FLAGS) $(USER_SOAP_FLAGS)

input=$1
tool_name=$2
soap_home=$3
wsdl_param=$4
user_wsdl_param=$5

if [ "$wsdl_param" == "<none>" ]
then
   wsdl_param=""
fi

if [ "$user_wsdl_param" == "<none>" ]
then
   user_wsdl_param=""
fi

shift 5

# Por ahora no se usa para nada, pero en un futuro podríamos soportar distintas utilidades de SOAP
if [ "$tool_name" != "gsoap" ]
then
   echo $0: Tool \"$tool_name\" is not recognized
   exit 1
fi

soap_version=`basename $GMAKE_SOAP`
echo wsdl: $soap_home/$GMAKE_SYSTEM/$soap_version/wsdl2h

filename=${input%.wsdl}
output=$GMAKE_TARGET_DIR/$filename.gsoap

$soap_home/$GMAKE_SYSTEM/$soap_version/wsdl2h $wsdl_param $user_wsdl_param -o $output $input
result=$?

if [ ! $result -eq 0 ]
then
   exit $result
fi

`dirname $0`/soap.sh $output $tool_name $soap_home -I$soap_home/$GMAKE_SYSTEM/$soap_version/import $*
exit $?

