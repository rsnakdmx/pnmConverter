#include<stdio.h>
#include<stdlib.h>

int contarOffset(int);
int leerImagen(int*, int*, int*, char[]);
void procesImagen(int, int, int, char[], int);

int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		printf("Error!\n");
		return -1;
	}

	else
	{
		char user = '\0';
		int alto = 0, ancho = 0, posicion = 0, ejecucion = 0, nomax = 0;

		nomax = leerImagen(&alto, &ancho, &posicion, argv[1]);

		if (nomax == -1)
			return -1;

		procesImagen(alto, ancho, posicion, argv[1], nomax);
		printf("Los nombres de los archivos son: 'EscalaGrises.pnm' y 'Negativo.pnm'.\n");

		return 0x0;
	}
}

int contarDigito(int numero)
{
	int tmp = numero, digito = 0;

	while (tmp > 0)
	{
		tmp/=10;
		digito++;
	}

	return (-1) * digito;
}

int leerImagen(int *alto, int *ancho, int *posicion, char nombreimg[])
{
	char busqueda[256];
	FILE *leer;
	int offset = 0,recor = 0, nomax = 0;

	leer = fopen(nombreimg, "r");

	if (leer == NULL)
	{
		printf("El archivo no existe."); 
		return -1;
	}

	while (recor == 0)
	{
		fscanf(leer, "%s", busqueda);
		recor = atoi(busqueda);

		if (recor < 10)
			recor = 0;
	}

	offset = contarDigito(recor);

	if (offset == 0)
		return -1;

	fseek(leer, offset, SEEK_CUR);
	fscanf(leer, "%d %d", &*ancho, &*alto);
	fscanf(leer, "%d", &nomax);
	*posicion = ftell(leer);
	fclose(leer);

	return nomax;
}

void procesImagen(int alto, int ancho, int posicion, char nombreimg[], int nomax)
{
	int **red = NULL, **green = NULL, **blue = NULL, **byn = NULL, **negativo = NULL;
	FILE *procesar = NULL, *gris = NULL, *negativa = NULL; 
	char disc[32], * dif = NULL;
	int x = 0;

	red = (int**)malloc(ancho*sizeof(int*));
	green = (int**)malloc(ancho*sizeof(int*));
	blue = (int**)malloc(ancho*sizeof(int*));
	byn = (int**)malloc(ancho*sizeof(int*));
	negativo = (int**)malloc(ancho*sizeof(int*));

	for(int i = 0; i < ancho; i++)
	{
		red[i] = (int*)malloc(alto*sizeof(int));
		green[i] = (int*)malloc(alto*sizeof(int));
		blue[i] = (int*)malloc(alto*sizeof(int));
		byn[i] = (int*)malloc(alto*sizeof(int));
		negativo[i] = (int*)malloc(alto*sizeof(int));
	}

	procesar = fopen(nombreimg, "r");
	fseek(procesar, posicion, SEEK_SET);
	
	for (int i = 0; i < ancho; i++)
	{
		for (int j = 0; j < alto; j++)
		{
			x = (-1);
			
			while (x == (-1))
			{
				fscanf(procesar, "%s", disc);
				x = strtol(disc, &dif, 10);
				
				if (disc == dif)
					x = (-1);
			}

			red[i][j] = x; //guardado rojo
			x = (-1);

			while (x == (-1))
			{
				fscanf(procesar, "%s", disc);
				x = strtol(disc, &dif, 10);

				if (disc == dif)
					x = (-1);
			}
			
			green[i][j] = x; //guardado verde
			x = (-1);

			while (x == (-1))
			{
				fscanf(procesar, "%s", disc);
				x = strtol(disc, &dif, 10);

				if (disc == dif)
					x = (-1);
			}

			blue[i][j] = x; //guardado azul
		}
	}

	fclose(procesar);
	
	for (int i = 0; i < ancho; i++)
	{ //conversion de las imagenes a su escala en gris y negativo.
		for (int j = 0; j < alto; j++)
		{
			byn[i][j] = (red[i][j] + green[i][j] + blue[i][j]) / 3;
			negativo[i][j] = nomax - byn[i][j];
		}
	}

	gris = fopen("EscalaGrises.pnm", "w"); //impresion de las imagenes terminadas
	negativa = fopen("Negativo.pnm", "w");
	fprintf(gris, "P2\n"); 
	fprintf(negativa, "P2\n"); //se usa la P2, corresponde a PGM(grayscale) de tipo ASCII
	fprintf(gris, "#Created by Snake\n"); 
	fprintf(negativa,"#Created by Snake\n");
	fprintf(gris, "%d %d\n", ancho, alto); 
	fprintf(negativa, "%d %d\n", ancho, alto);
	fprintf(gris, "%d\n", nomax); 
	fprintf(negativa, "%d\n", nomax);

	for (int i = 0; i < ancho; i++)
	{
		for (int j = 0; j < alto; j++)
		{
			fprintf(gris,"%d\n", byn[i][j]);
			fprintf(negativa,"%d\n", negativo[i][j]);
		}
	}

	fclose(gris);
	fclose(negativa);

	for (int i = 0; i < alto; i++)
	{
		free(red[i]);
		free(green[i]);
		free(blue[i]);
		free(byn[i]);
		free(negativo[i]);
	}

	free(red);
	free(green);
	free(blue);
	free(byn);
	free(negativo);

	return;
}