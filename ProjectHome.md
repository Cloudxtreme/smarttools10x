Las SmartTools 10.0/Evo-X engloban una serie de herramientas que generan de automáticamente los ficheros de dependencias necesarios para crear los ejecutables a partir de un código fuente C/C++ y demás archivos fuentes reconocidos por la instalación que hayamos usado (Lex, Yacc, ASN1, IDL's, etc).

El principal objetivo de estas herramientas es generar los archivos de dependencias1 de nuestros archivos fuentes C/C++ en base a los archivos de cabeceras usados directa o indirectamente. Lo que permitirá a nuestro sistema conocer qué componentes debe volver a generar en caso de que cambie algún archivo contenido en el proyecto.

  * Permite tratar un número indeterminado de niveles de agrupación, tanto a nivel de subsistema como a nivel de paquetes incluidos en una librería o ejecutable.

  * El motor de búsqueda de archivos de cabecera es capaz de resolver gran parte de las ambigüedades de forma automática.

  * Incorpora el tratamiento de dependencias futuras, que se aplican en los analizadores específicos de ASN1 y en los IDL’s.

  * Amplía la definición de recursos disponibles sobre los que ajustar la compilación de los componentes de los proyectos.

  * Permite que un mismo proyecto sea generado por esta versión y por las versiones precedentes de las SmartTools.

  * Facilita la integración con entornos integrados de desarrollo. Actualmente ha sido probado con éxito desde el Eclipse ™.

Todas las mejoras en las estructuras internas se traduce en que la generación de los archivos de dependencias se realiza en un tiempo sensiblemente menor al empleado en versiones anteriores. Por ejemplo, para generar los archivos de dependencias del proyecto de la plataforma SDP original la versión 9.0 emplea unos 120 segundos, mientras que esta versión emplea unos 2 segundos.