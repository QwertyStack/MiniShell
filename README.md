# MiniShell

_La función es la dde realizar un programa que actúe como intérprete de mandatos. Esta minishell interpreta y ejecuta mandatos 
leyéndolos de la entrada estándar, así como dando la posibilidad al usuario de utilizar redirecciones._

### Pre-requisitos 📋

### Funcionamiento ⚙️
En definitiva el interprete es capaz de:
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

### Consideraciones 🤔
Se deberán tener en cuenta los siguientes casos:
  - Si la lı́nea introducida no contiene ningún mandato o se ejecuta el mandato en background, se volverá a mostrar el prompt a la espera de una nueva lı́nea.
  - Si alguno de los mandatos a ejecutar no existe, el programa debe mostrar el error “mandato: No se encuentra el mandato”.
  - Si se produce algún error al abrir cualquiera de los ficheros de las redirecciones, debe mostrarse el error “fichero: Error. Descripción del error”.
  - Ni el minishell ni los procesos en background deben finalizar al recibir las señales desde teclado SIGINT (Ctrl − C) y SIGQUIT (Ctrl − / ) mientras que los procesos que se lancen deben actuar ante ellas, manteniendo la acción por defecto.






# Título del Proyecto

_Acá va un párrafo que describa lo que es el proyecto_

## Comenzando 🚀

_Estas instrucciones te permitirán obtener una copia del proyecto en funcionamiento en tu máquina local para propósitos de desarrollo y pruebas._

Mira **Deployment** para conocer como desplegar el proyecto.


### Pre-requisitos 📋

_Que cosas necesitas para instalar el software y como instalarlas_

```
Da un ejemplo
```

### Instalación 🔧

_Una serie de ejemplos paso a paso que te dice lo que debes ejecutar para tener un entorno de desarrollo ejecutandose_

_Dí cómo será ese paso_

```
Da un ejemplo
```

_Y repite_

```
hasta finalizar
```

_Finaliza con un ejemplo de cómo obtener datos del sistema o como usarlos para una pequeña demo_

## Ejecutando las pruebas ⚙️

_Explica como ejecutar las pruebas automatizadas para este sistema_

### Analice las pruebas end-to-end 🔩

_Explica que verifican estas pruebas y por qué_

```
Da un ejemplo
```

### Y las pruebas de estilo de codificación ⌨️

_Explica que verifican estas pruebas y por qué_

```
Da un ejemplo
```

## Despliegue 📦

_Agrega notas adicionales sobre como hacer deploy_

## Construido con 🛠️

_Menciona las herramientas que utilizaste para crear tu proyecto_

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - El framework web usado
* [Maven](https://maven.apache.org/) - Manejador de dependencias
* [ROME](https://rometools.github.io/rome/) - Usado para generar RSS

## Contribuyendo 🖇️

Por favor lee el [CONTRIBUTING.md](https://gist.github.com/villanuevand/xxxxxx) para detalles de nuestro código de conducta, y el proceso para enviarnos pull requests.

## Wiki 📖

Puedes encontrar mucho más de cómo utilizar este proyecto en nuestra [Wiki](https://github.com/tu/proyecto/wiki)

## Versionado 📌

Usamos [SemVer](http://semver.org/) para el versionado. Para todas las versiones disponibles, mira los [tags en este repositorio](https://github.com/tu/proyecto/tags).

## Autores ✒️

_Menciona a todos aquellos que ayudaron a levantar el proyecto desde sus inicios_

* **Andrés Villanueva** - *Trabajo Inicial* - [villanuevand](https://github.com/villanuevand)
* **Fulanito Detal** - *Documentación* - [fulanitodetal](#fulanito-de-tal)

También puedes mirar la lista de todos los [contribuyentes](https://github.com/your/project/contributors) quíenes han participado en este proyecto. 

## Licencia 📄

Este proyecto está bajo la Licencia (Tu Licencia) - mira el archivo [LICENSE.md](LICENSE.md) para detalles

## Expresiones de Gratitud 🎁

* Comenta a otros sobre este proyecto 📢
* Invita una cerveza 🍺 o un café ☕ a alguien del equipo. 
* Da las gracias públicamente 🤓.
* etc.




