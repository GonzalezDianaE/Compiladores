Lenguaje: PASCAL-TDS - 2017 - Taller de Diseño de Software (Cod. 3306) -

Autores

	Boselli Elisa, Etcharren Leandro, Gonzalez Diana.

Repositorio Git: https://github.com/elisaboselli/Compiladores

Datos Sistema

	Sistema operativo: Mac OS (SIERRA), Linux.

	Compilador utilizado: gcc, version 4.2.1. de 64 bits (Si se complica con 32 pueden surgir problemas con los rangos de enteros)

	Herramientas: bison y flex.

Comando de compilación y Ejecución

	./pascal-tds <pathArchivoAEvaluar> [-d] 

	Siendo:
		- pascal-tds el script que compila los archivos flex y bison.
		- primer parametro (obligatiorio) el path al archivo de extension .ptds a evaluar.
		- segundo parametro (optiativo) el flag '-d' que muestra un debug de lo que se va evaluando.

	Nota: los parámetros deben pasarse en el orden mencionado.

	Como ejemplos se anexan  archivos .ptds para que puedan ser ejecutados (dentro de los directorios Pruebas y Pruebas-Semanticas).
