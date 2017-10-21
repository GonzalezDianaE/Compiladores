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


	./ptds-compiler <pathArchivoAEvaluar> [-d] 
		Utiliza el compilador pascaltds para compilar el archivo pasado como parámetro.
		El primer parámetro (obligatiorio) es el path al archivo de extension .ptds a evaluar.
		El segundo parámetro (optiativo) es el flag '-d' que muestra un debug de lo que se va evaluando.

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
