#include "tdas/queue.h"
#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

typedef struct
{
  char nombre[51];
  short edad;
  char sintoma[51];
  short prioridad;
  int numero;
  time_t hora;
}info;
  
// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  //getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Hospitalaria");
  puts("========================================");

  puts("1) Registrar paciente");
  puts("2) Asignar prioridad a paciente");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente paciente");
  puts("5) Mostrar pacientes por prioridad");
  puts("6) Salir");
}
// Convierte el nombre en formato general
void formato(char *nombre)
{
  for(int i = 0; nombre[i] != '\0'; i++)
    {
      if(i == 0) nombre[i] = toupper(nombre[i]);
      else nombre[i] = tolower(nombre[i]);
    }
}

// Convierte string en short
short strToHu(char *edad)
{
  short num = 0;
  for(int i = 0; edad[i] != '\0'; i++)
    {
      if(isdigit(edad[i]))
      {
        num = num * 10 + (edad[i] - '0');
      }
    }
  return num;
}

// Verifica si hay caracter alfabetico
bool hayAlpha(char *alp)
{
  for(int i = 0; alp[i] != '\0'; i++)
    {
      if(isalpha(alp[i])) return true;
    }
  return false;
}

// Funcion para leer la edad
void comprobarEdad(char *edad, info *temp)
{
  short num;
  scanf("%s", edad);
  while(hayAlpha(edad))
    {
      printf("Edad inválida!!(Contiene carácter alfabético)\n");
      printf("Ingrese una edad válida: ");
      scanf("%s", edad);
    }
  num = strToHu(edad);
  temp->edad = num;
}

// Funcion para ordenar la lista
void ordenar(List *pacientes, info *temp)
{
  int pos = 0;
  list_popCurrent(pacientes);
  info *current = list_first(pacientes);

  while(true && current != NULL)
  {
    if(temp->prioridad >= current->prioridad)
    {
      if(temp->prioridad > current->prioridad) break;
      if(temp->prioridad == current->prioridad && temp->numero <              current->numero) break;
    }
    pos++;
    current = list_next(pacientes);
  }
  current = list_first(pacientes);
  if(pos == 0) list_pushFront(pacientes, temp);
  else
  {
    for(int i = 1; i < pos; i++)
    {
      current = list_next(pacientes);    
    }
    list_pushCurrent(pacientes, temp);
  }
}

// MENU 1: REGISTRAR PACIENTE----------------------------------------
void registrar_paciente(List *pacientes, int num) 
{
  printf("==========================================\n");
  printf("         Registrar nuevo paciente\n");
  printf("==========================================\n");
  // Aquí implementarías la lógica para registrar un nuevo paciente
  info *temp = (info *)malloc(sizeof(info));
  char edad[10];
  printf("Ingrese el nombre:  ");
  scanf("%50[^\n]s", temp->nombre);
  printf("Ingrese la edad:  ");
  comprobarEdad(edad, temp);
  getchar();
  printf("Ingrese el sintoma:  ");
  scanf("%50[^\n]s", temp->sintoma);
  temp->prioridad = 1;
  temp->numero = num;
  temp->hora = time(NULL);
  formato(temp->nombre);
  
  list_pushBack(pacientes, temp);
}
//-------------------------------------------------------------------

// MENU 2: ASIGNAR PRIORIDAD A PACIENTE------------------------------
void asignar_prioridad(List *pacientes)
{
  info *temp = list_first(pacientes);
  printf("==========================================\n");
  printf("            Asignar prioridad\n");
  printf("==========================================\n");
  if(temp == NULL)
  {
    printf("        NO HAY PACIENTES EN ESPERA\n");
    printf("==========================================\n");
    return;
  }
  char nombreT[51];
  char prioridad;
  printf("Ingrese el nombre:  ");
  scanf("%50[^\n]s", nombreT);
  while(getchar() != '\n');
  formato(nombreT);
  while(temp != NULL)
  {
    if(strcmp(temp->nombre, nombreT) == 0) break;
    temp = list_next(pacientes);
  }
  if(temp == NULL)
  {
    printf("==========================================\n");
    printf("       No se encontró el paciente\n");
    printf("==========================================\n");
    return;
  }
  printf("Asigna prioridad al paciente\n");
  printf("1) Baja\n");
  printf("2) Media\n");
  printf("3) Alta\n");
  scanf("%1c", &prioridad);
  while(getchar() != '\n');
  while((prioridad < '1' || prioridad > '3') || isalpha(prioridad))
    {
      printf("Porfavor ingrese número 1~3: ");
      scanf("%1c", &prioridad);
      while(getchar() != '\n');
    }
  temp->prioridad = strToHu(&prioridad);
  ordenar(pacientes, temp);
}
//-------------------------------------------------------------------

// MENU 3: MOSTRAR LISTA DE ESPERA-----------------------------------
void mostrar_lista_pacientes(List *pacientes) 
{
  // Mostrar pacientes en la cola de espera
  printf("==========================================\n");
  printf("           Pacientes en espera \n");
  printf("==========================================\n");
  // Aquí implementarías la lógica para recorrer y mostrar los pacientes
  if(list_first(pacientes) != NULL)
  {
    for(info *temp = list_first(pacientes); temp != NULL; temp =         list_next(pacientes))
    {
      printf("Nombre: %s\n", temp->nombre);
      printf("Edad: %hu\n", temp->edad);
      printf("Sintoma: %s\n", temp->sintoma);
      switch(temp->prioridad)
        {
          case 1: printf("Prioridad: Baja\n"); break;
          case 2: printf("Prioridad: Media\n"); break;
          case 3: printf("Prioridad: Alta\n"); break;
        }
      //printf("Número: %d\n", temp->numero);
      printf("Hora de registro: %s\n", ctime(&temp->hora));
      printf("==========================================\n");
    }
  }
  else
  {
    printf("        NO HAY PACIENTES EN ESPERA\n");
    printf("==========================================\n");
  }
}
//-------------------------------------------------------------------

// MENU 4: ATENDER AL SIGUIENTE PACIENTE-----------------------------
void atiende_paciente(List *pacientes)
{
  info *temp = list_first(pacientes);
  printf("==========================================\n");
  printf("           Atiende al paciente\n");
  printf("==========================================\n");
  if(temp != NULL)
  {
    printf("Nombre: %s\n", temp->nombre);
    printf("Edad: %hu\n", temp->edad);
    printf("Sintoma: %s\n", temp->sintoma);
  }
  else printf("        NO HAY PACIENTES EN ESPERA\n");
  printf("==========================================\n");
  list_popFront(pacientes);
}
//-------------------------------------------------------------------

// MENU 5: MOSTRAR PACIENTES POR PRIORIDAD---------------------------
void buscar_prioridad(List *pacientes)
{
  short prioridad;
  bool hay = false;
  printf("==========================================\n");
  printf("      Mostrar pacientes por prioridad\n");
  printf("==========================================\n");
  printf("Ingrese la prioridad a buscar:\n");
  printf("1) Baja\n");
  printf("2) Media\n");
  printf("3) Alta\n");
  scanf("%hu", &prioridad);
  while(prioridad < 1 || prioridad > 3)
  {
    printf("Porfavor ingrese número 1~3: ");
    scanf("%hu", &prioridad);
  }
  info *temp = list_first(pacientes);
  while(temp != NULL)
    {
      if(temp->prioridad == prioridad)
      {
        printf("==========================================\n");
        printf("Nombre: %s\n", temp->nombre);
        printf("Edad: %hu\n", temp->edad);
        printf("Sintoma: %s\n", temp->sintoma);
        switch(temp->prioridad)
        {
          case 1: printf("Prioridad: Baja\n"); break;
          case 2: printf("Prioridad: Media\n"); break;
          case 3: printf("Prioridad: Alta\n"); break;
        }
        if(!hay) hay = true;
      }
      temp = list_next(pacientes);
    }
  if(!hay) 
  {
    printf("==========================================\n");
    printf("    NO HAY PACIENTES CON ESA PRIORIDAD\n");
  }
  printf("==========================================\n");
}
//-------------------------------------------------------------------


int main() {
  int num = 0;
  char opcion;
  //List *pacientes = list_create(); // puedes usar una lista para gestionar los pacientes 
  List *pacientes = list_create(); // puedes usar una cola para gestionar la cola de espera

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior
    limpiarPantalla();
    while(getchar() != '\n');
    switch (opcion) {
    case '1':
      num++;
      registrar_paciente(pacientes, num);
      while(getchar() != '\n');
      break;
    case '2':
      asignar_prioridad(pacientes);
      // Lógica para asignar prioridad
      break;
    case '3':
      mostrar_lista_pacientes(pacientes);
      while(getchar() != '\n');
      break;
    case '4':
      // Lógica para atender al siguiente paciente
      atiende_paciente(pacientes);
      while(getchar() != '\n');
      break;
    case '5':
      // Lógica para mostrar pacientes por prioridad
      buscar_prioridad(pacientes);
      while(getchar() != '\n');
      break;
    case '6':
      puts("Saliendo del sistema de gestión hospitalaria...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '6');

  // Liberar recursos, si es necesario
  list_clean(pacientes);

  return 0;
}
