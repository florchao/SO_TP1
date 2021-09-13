#Trabajo Práctico N1: “Inter Process Communication”

#Instrucciones de compilación y ejecución
Para la compilación, al comenzar hay que instalar el minisat con el comando apt-get install minisat. Luego, para compilar los archivos hay que ejecutar el comando make en la carpeta principal. 
Para la ejecución se necesitan los archivos de extensión .cnf. Cabe destacar, que los archivos de extensión cnf deben ser agregados antes de ser ejecutados como es mencionado en la consigna. Para comenzar con la ejecución, primero hay que posicionarse en la carpeta bin donde se generan los ejecutables. Y luego hay dos formas de ejecución:
Primera forma:
./master pathCnfFiles: se ejecuta el proceso padre y se le envían los archivos dentro de pathCnfFiles. Por consola no se mostrará ningún resultado (sólo se imprime un dato importante para el proceso view), sino que serán exportados al archivo result que aparecerá en la carpeta principal. Para ir viendo los resultados, se debe ejecutar el proceso view.
Segunda forma: 
./master pathCnfFiles | ./view: se ejecuta el proceso padre y el proceso vista, quien se conectará a un espacio de memoria compartida para leer lo que escriba el proceso padre y lo imprimirá en la salida estándar. También, se exportarán los datos al archivo result.

