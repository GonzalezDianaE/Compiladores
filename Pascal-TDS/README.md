Lenguaje: PASCAL-TDS - 2017 - Taller de Diseño de Software (Cod. 3306) -

Autores

	Boselli Elisa, Etcharren Leandro, Gonzalez Diana.

Repositorio Git: https://github.com/elisaboselli/Compiladores

Datos Sistema

	Sistema operativo: Mac OS (SIERRA), Linux.

	Compilador utilizado: gcc, version 4.2.1. de 64 bits (Si se complica con 32 pueden surgir problemas con los rangos de enteros)

	Herramientas: bison y flex.

Comandos Incluidos:
	./generate-compiler 

		Compila el codigo fuente del compilador (flez, yacc y c). 
		No lleva parametros. Genera el ejecutable "pascaltds".

	./ptds-compiler-linux [flag] [target] <path/archivo/a/evaluar.ptds>
	or
	./ptds-compiler-mac [flag] [target] <path/archivo/a/evaluar.ptds>

		Without flag (and target), is normal execution (assemblerCode executable generated).
		The flag could be:
    		-o      : Renames the excecutable. It needs a target that will be the new name (generates executable).
    		-target : Proceeds until target. It needs a target that could be:
    		            * scan      : proceeds until flex scan.
    		            * parse     : proceeds until yacc parse.
    		            * codinter  : proceeds until intermediate code generation.
    		            * assembler : proceeds until assembler code generation.
    		-debug  : Debug execution (generates executable).
    		-ic     : Shows intermediate code (generates executable).
    		-ac     : Shows assembler code (generates executable).

    ./ptds-run-test [-m]
    	Ejecuta todos los test creados hasta el momento (en los directorios Pruebas, Pruebas-Semanticas y Pruebas-Codigo-Intermedio).
    	Con el flag -m ejecuta los mismos para el sistema operativo macOs, sin el para linux.


(DEPRECATED)	

		./ptds-compiler <pathArchivoAEvaluar> [-d] 
		Utiliza el compilador pascaltds para compilar el archivo pasado como parámetro.
		El primer parámetro (obligatiorio) es el path al archivo de extension .ptds a evaluar.
		El segundo parámetro (optiativo) es el flag '-d' que muestra un debug de lo que se va evaluando.

(DEPRECATED)

	./ptds-run-test
		Ejecuta todos los test creados hasta el momento (en los directorios Pruebas, Pruebas-Semanticas y Pruebas-Codigo-Intermedio).


(DEPRECATED) Comando de compilación y Ejecución

	./pascal-tds <pathArchivoAEvaluar> [-d] 

	Siendo:
		- pascal-tds el script que compila los archivos flex y bison.
		- primer parametro (obligatiorio) el path al archivo de extension .ptds a evaluar.
		- segundo parametro (optiativo) el flag '-d' que muestra un debug de lo que se va evaluando.

	Nota: los parámetros deben pasarse en el orden mencionado.

	Como ejemplos se anexan  archivos .ptds para que puedan ser ejecutados (dentro de los directorios Pruebas, Pruebas-Semanticas y Pruebas-Codigo-Intermedio).
