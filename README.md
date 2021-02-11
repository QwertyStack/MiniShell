# MiniShell

la función de este es la de realizar un programa que actúe como intérprete de mandatos. Esta minishell interpreta y ejecuta mandatos 
leyéndolos de la entrada estándar, así como dando la posibilidad al usuario de utilizar redirecciones. En definitiva el interprete es capaz de:
  - Ejecutar una secuencia de uno o varios mandatos separados por el carácter ’|’.
  - Permitir redirecciones:
    - Entrada: < f ichero. Sólo puede realizarse sobre el primer mandato del pipe.
    - Salida: > f ichero. Sólo puede realizarse sobre el último mandato del pipe.
    - Error: > &f ichero. Sólo puede realizarse sobre el último mandato del pipe.
  - Permitir la ejecución en background de la secuencia de mandatos si termina con el carácter '&' sin bloquerse.
  - Mostrar en pantalla un prompt similar al original.
  - Leer una lı́nea del teclado.
  - Analizarla utilizando la librerı́a parse.
  - Ejecutar todos los mandatos de la lı́nea a la vez creando varios procesos hijo y comunicando unos con otros con las tuberı́as que sean necesarias, y realizando las redirecciones que sean necesarias. En caso de que no se ejecute en background, se espera a que todos los mandatos hayan finalizado para volver a mostrar el prompt y repetir el proceso.

Teniendo en cuenta que:
  - Si la lı́nea introducida no contiene ningún mandato o se ejecuta el mandato en background, se volverá a mostrar el prompt a la espera de una nueva lı́nea.
  - Si alguno de los mandatos a ejecutar no existe, el programa debe mostrar el error “mandato: No se encuentra el mandato”.
  - Si se produce algún error al abrir cualquiera de los ficheros de las redirecciones, debe mostrarse el error “fichero: Error. Descripción del error”.
  - Ni el minishell ni los procesos en background deben finalizar al recibir las señales desde teclado SIGINT (Ctrl − C) y SIGQUIT (Ctrl − / ) mientras que los procesos que se lancen deben actuar ante ellas, manteniendo la acción por defecto.
