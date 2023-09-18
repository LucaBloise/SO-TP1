ITBA-SO-TP1
Los siguientes comandos pueden usarse desde el directorio en el que se encuentren los archivos de programa:

- make md5/ make slave/ make vista: compilan el archivo correspondiente.
- make all: compila todos los archivos.
- make clean: elimina todos los binarios y archivos objeto.

Para ejecutar únicamente el proceso aplicación, se debe correr ./md5, pasando como argumento la lista de archivos a utilizar. Esto generará un archivo “results.txt” que contiene el output. Para ejecutar el proceso aplicación con el proceso vista, es necesario pasar la información que md5 imprime en stdout a vista. Vista puede recibir esta información como argumento, o mediante stdin si no se envía ningún argumento. 

Esto permite varias opciones para la ejecución, por ejemplo:

- Con un pipe: ./md5 <archivos> | ./vista
- Ejecutando ./md5 <archivos> en una terminal, y luego ./vista <info> en otra
- Ejecutar md5 en background (./md5 <archivos> &), y luego vista en foreground (./vista <info>)
