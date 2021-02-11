/*	*	*	*	*	*	*	*	*	*	*	*	*	*	*
 *	@author 											*
 *	QwertyStack											*
 *														*
 *	@description										*
 *	Programa encargado de imitar a pequeña escala un	*
 *	intérprete de comandos en distribución Linux.		*
 *	*	*	*	*	*	*	*	*	*	*	*	*	*	*/

//Librerías
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include "parser.h"

//Constantes
#define LECTURA 0
#define ESCRITURA 1
#define M 1024

//Cabecera de funciones
int ejecutaComando(char *buf, int entradaEstandar, int salidaEstandar, int errorEstandar);
void mantenerRedirecciones (int entradaEstandar, int salidaEstandar, int errorEstandar);
int valido (char* comando);
int cd();


//Variables globales
tline * line;
	

	
//----------------------------------------------------------------Programa principal--------------------------------------------------------------------------
int main(void) {
	
	//Variables del programa
	char buf[M];			//Buffer de almacenamiento de entrada stdin
	int fd;					//Descriptor de fichero
	char dirActual [M];		// Buffer almacenamiento del directorio actual

	//Variables usadas para reestablecer las entradas, salidas y error estándar
	int salidaEstandar = dup(STDOUT_FILENO);
	int entradaEstandar = dup(STDIN_FILENO);
	int errorEstandar = dup(STDERR_FILENO);
	
	//----------------------Cuerpo del programa---------------------------
	
	//Ignorar señales SIGINT y SIGQUIT (minishell)
	signal(2 , SIG_IGN);		//2 --> SIGINT
	signal(3, SIG_IGN);			//3 --> SIGQUIT
	
	printf("%s@%s$", getlogin(),getcwd (dirActual, M));						//prompt	
	while (fgets(buf, 1024, stdin)) {	//bucle encargado de la lectura de la entrada estándar	
		line = tokenize(buf);			//puntero a la cadena leída por la entrada estándar		
		
		signal(2 , SIG_IGN);		//2 --> SIGINT
		signal(3, SIG_IGN);
		
		if (line == NULL) {				//comprobación cadena vacía
			continue;
		}
		
		//redirección de entrada '<'
		if (line->redirect_input != NULL) {
			printf("\t redirección de entrada: %s\n", line->redirect_input);
			fd = open(line->redirect_input, O_RDONLY , S_IRUSR | S_IWUSR);			//abrir fichero modo lectura, usuario permisos de lectura y escritura
			//comprobación de errores
			if (fd < 0 ){
				printf("ERROR en redirección entrada [%s]\n", strerror(errno));
				//return 1;	La ejecución del programa finalizaría
				continue;
			}else{
				dup2(fd,STDIN_FILENO);				//redirección de entrada estándar al fichero
			}			
		}
		
		//redirección de salida '>'
		if (line->redirect_output != NULL) {
			printf("\t redirección de salida: %s\n", line->redirect_output);
			fd = open(line->redirect_output, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);		//abrir fichero con escritura y concatenación, usuario permisos de lectura y escritura
			//comprobación de errores
			if (fd < 0 ){
				printf("ERROR en redirección salida [%s]\n", strerror(errno));
				//return 1;	La ejecución del programa finalizaría
				continue;
			}else{
				dup2(fd,STDOUT_FILENO);			//redirección de salida estándar al fichero
			}
		}
		
		//redirección de error '>&'
		if (line->redirect_error != NULL) {
			printf("\t redirección de error: %s\n", line->redirect_error);
			fd = open(line->redirect_error, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);		//abrir fichero con escritura, concatenación y creación, usuario permisos de lectura y escritura
			//comprobación de errores
			if (fd < 0 ){
				printf("ERROR en redirección error [%s]\n", strerror(errno));
				//return 1;	La ejecución del programa finalizaría
				continue;
			}else{
				dup2(fd,STDERR_FILENO);			//redirección de salida de error estándar al fichero
			}
		}
		
		//comprobación procesos en background
		if (line->background) {
			printf("comando a ejecutarse en background\n");
			//Ignorar señales SIGINT y SIGQUIT (background)
			signal(2,SIG_IGN);	//2 --> SIGINT
			signal(3,SIG_IGN);	//3 --> SIGQUIT
						
		}else{
			//Acción por defecto ante señales SIGINT y SIGQUIT (foreground)
			signal(2,SIG_DFL);	//2 --> SIGINT
			signal(3,SIG_DFL);	//3 --> SIGQUIT
		}
		
		ejecutaComando(buf, entradaEstandar, salidaEstandar, errorEstandar);	//ejecución de mandato/s
		
		//restablecimiento de ignoración de sañales SIGINT y SIGQUIT 
		signal(2,SIG_IGN);	//2 --> SIGINT
		signal(3,SIG_IGN);	//3 --> SIGQUIT
		
		mantenerRedirecciones (entradaEstandar, salidaEstandar, errorEstandar);		//establecer entrada, salida y error estándar
					
		printf("%s@%s$", getlogin(),getcwd (dirActual, M));							//prompt
	}
	
	return 0;	//ejecución correcta
}



//---------------------------------------------------Funciones desarrolladas-----------------------------------------------------------

//ejecución de uno o más mandatos con o sin pipes
int ejecutaComando(char *buf , int entradaEstandar, int salidaEstandar, int errorEstandar){
	//variables de la función
	line = tokenize(buf);
	int status, i, j;		//status--> valor de retorno proceso hijo	i,j--> iteradores
	int fd[2];				//descriptor de fichero
	pid_t pid, pid_2;		//almacenamiento pids procesos
	
	//------------------------------------------------Cuerpo de la función-------------------------------------------------
	//confirmación de valor en la entrada estándar
	if (line->ncommands > 0){	
		
		//------------------------------------un mandato--------------------------
		if ( line->ncommands == 1){
			
			//comprobación si se trata del mandato 'cd'
			if ( strncmp(line->commands[0].argv[0], "cd", 2) == 0 ){
				cd ();		//llamada a la ejecución de 'cd'
			}
			//comprobación de la validez del mantato						
			else if (valido(line->commands[0].filename)){
				
				pid = fork();	//creación proceso
				//control de errores
				if (pid < 0){
					printf("ERROR en el fork: %s\n", strerror(errno));
					return 1;
				} 
				
				// proceso hijo
				if (pid == 0){	
					execvp(line->commands[0].filename, line->commands[0].argv);		//ejecución mandato
					//control de errores, si llega hasta aquí execvp no ha ido correctamente
					printf("ERROR en la ejecución de execvp [%s]\n", strerror(errno));
					return 1;
				
				// proceso padre
				} else{	
					//comprobación ejecución en background
					if (!line->background){
						wait (&status);		//espera hijos si no está en background
					}
					
					//comprobación de errores
					if (WIFEXITED(status) != 0) {
						if (WEXITSTATUS(status) != 0){
							printf("El comando no se ejecutó correctamente\n");
						}
					}
					
					
						
				}
			}
		}
		
		//------------------------------------varios mandatos-----------------------------
		if (line->ncommands > 1) {	
			for (i = 0; i<line->ncommands; i++){	//se ejecuta la secuencia Hijo-Padre hasta el último mandato	
				
				//Creación del pipe + control de errores
				if (pipe(fd) < 0){
					printf("ERROR en el pipe : %s\n", strerror(errno));
					return 1;
				}
				
				pid_2 = fork();		//creación proceso
				//control de errores
				if (pid_2 < 0){
					printf("ERROR en el fork: %s\n", strerror(errno));
					return 1;
				}
				
				//procesos HIJOS
				if (pid_2 == 0)	{	
					
					//último mandato
					if (i == (line->ncommands-1) ){	
						//No se modifica la entrada porque ya la tiene (padre), y la salida no hay que modificarla porque es la estándar
						
						//ejecución mandato + control de errores
						if  (0 != execvp(line->commands[line->ncommands-1].filename, line->commands[line->ncommands-1].argv) ) {
							printf("ERROR en el execvp del último comando: %s\n", strerror(errno));
							return 1;
						}
						
					} else{ //primero-penúltimo mandato
						dup2(fd[ESCRITURA], STDOUT_FILENO);		//modificación de la salida estándar
						close(fd[LECTURA]);						//cierre de extremo del pipe inutilizado
						close(fd[ESCRITURA]);					//cierre de extremo del pipe después de trabajar con él
						
						//ejecución mandato + control de errores
						if  (0 != execvp(line->commands[i].filename, line->commands[i].argv) ) {
							printf("ERROR en el execvp: %s\n", strerror(errno));
							return 1;
						}
					}
				
				//procesos PADRE
				}else{		
					dup2(fd[LECTURA], STDIN_FILENO);	//modificación de la entrada estándar				
					//cierre extremos pipe
					close(fd[LECTURA]);					
					close(fd[ESCRITURA]);		
				}			
			}
			
			//for de espera para todos los hijos
			for (j=0; j<line->ncommands; j++){
				wait(&status);		//espera hijos si no está en background
				
				//comprobación de errores
				if (WIFEXITED(status) != 0) {
					if (WEXITSTATUS(status) != 0){
						printf("El comando no se ejecutó correctamente\n");
					}
				}
			}
			
			
		}
				
	}
	
	
}

//restablecimiento de entrada, salida y error estándar
void mantenerRedirecciones (int entradaEstandar, int salidaEstandar, int errorEstandar){
	dup2(entradaEstandar, STDIN_FILENO);
	dup2(salidaEstandar, STDOUT_FILENO);
	dup2(errorEstandar, STDERR_FILENO);
}

//comporbación existencia del mandato introducido
int valido (char* comando){
	if (comando == NULL){
		printf("Comando no valido\n");
		return false;
	}
	return true;
}

//ejecución mandato 'cd'
int cd(){
	//variables de la función
	char buffer[1024];
	
	//------------cuerpo de la función-----------
	
	//comprobación cantidad de argumentos (máximo 1)
	if (line->ncommands > 1){
		printf("No se puede acceder a más de un direcctorio\n");
		return 1;
		
	//va a ser un solo comando, hemos comprobado antes de la llamada
	} else{	
		//ir a $HOME (poniendo solo 'cd')
		if (line->commands[0].argc == 1){
			printf("No hay comando vamos a ir a $HOME\n");
			
			//verificar que $HOME tiene una ruta
			if(getenv("HOME") == NULL){
				printf("Error al buscar $HOME\n [%s]", strerror(errno))	;
			//cambiar de directorio a $HOME
			} else{
				//control de errores
				if (0 != chdir (getenv("HOME") ) ) {
					printf("Error al cambiar de directorio [%s]\n", strerror(errno));
				}
			}
		//cambio de directorio según los argumentos + control de errores
		}else if (chdir (line->commands[0].argv[1]) != 0 ){
			printf("Error al cambiar de directorio [%s]\n", strerror(errno));
		}
	}
	
	printf("El directorio en el que estás es [%s]\n", getcwd (buffer, sizeof(buffer)) );	//información sobre la nueva ubicación
}

/*--
void jobs(struct Procesos proceso){
	printf("PID\tPGID\tCOMANDO\n");
	printf("%d\t%d\t%s\n", proceso.pid, proceso.pid_grupo, proceso.comando);
}
*/


