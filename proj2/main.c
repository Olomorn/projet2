#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//mode, read, nom fichier
//test

int main(int argc, char *argv[])
{
	int * message;
	int * tableau2;
	int pid_fils;
	int tube[2];
	long taille=0;
	FILE *f;
	
	pipe(tube);
	pid_fils = fork();

	if(argc >= 3 ) //verifie qu'on a bien le bon nombre d'argument
	{
		if(pid_fils != 0 ) //Pere (Emetteur ) 
		{
			f = fopen(argv[1], "r"); //Ouverture fichier
			if(f!=NULL)
			{
			
				fseek(f,0,SEEK_END);
				taille = ftell(f);
				rewind(f);				//Replacement au debut
				message = (int*)malloc(taille * sizeof(int));
		
				for(int i =0; i< taille; i++)
				{
					message[i] = fgetc(f);		
				}
				
				//Gestion envoie pipe
				//Envoie des instructions : taille, valeur paramaetre, tableau
				write(tube[1], &taille, sizeof(taille));
				write(tube [1],argv[2], sizeof(char));
				write(tube[1],message, taille*sizeof(int));
				wait(NULL);
				
				//Fermeture fichier + pipe
				fclose(f);
				close(tube[1]);		
				
			}
		

		}
		else	  			//Fils (recepteur ) 
		{
			char mode;
			f = fopen("reception.txt", "w"); //Ouverture / Creation fichier
			//Receptions des infos
			read(tube[0],&taille,sizeof(taille));
			printf("%i", taille);
			read(tube[0],&mode, sizeof(char));
			printf("%c", mode);
			message = (int*)malloc(taille * sizeof(int));
			read(tube[0], message, taille*sizeof(int));
			
			int nbvoyelle;
			
			switch(mode ) //Opperations sur le message
			{
				case 1:
				tableau2= (int*)malloc(taille * sizeof(int));
				for(int i =0; i< taille; i++)
				{
					tableau2[i] = message[taille-i] ;
				}
				break;
				
				case 2:
				
				nbvoyelle = 0;
				for(int i =0; i< taille; i++)
				{
					if(message[i] == 'a' || message[i] == 'e' || 
					message[i] == 'i' || message[i] == 'o' || 
					message[i] == 'u' || message[i] == 'y')
					{
						nbvoyelle ++;			
					}
				}
				tableau2= (int*)malloc((taille-nbvoyelle) * sizeof(int));
				int j = 0;
				for(int i =0; i< taille; i++)
				{
					if(message[i] != 'a' || message[i] != 'e' || 
					message[i] != 'i' || message[i] != 'o' || 
					message[i] != 'u' || message[i] != 'y')
					{
						tableau2[j] = message[i];
						j++;				
					}
				}
				taille = taille-nbvoyelle;
				break;
			
			}
			
			//Ecriture du message modifié
			for(int i =0; i< taille; i++)
			{
				fputc(tableau2[i],f);
			}
			
			//Fermeture fichier et pipe
			fclose(f);
			close(tube[0]);		
		}
	}

}
