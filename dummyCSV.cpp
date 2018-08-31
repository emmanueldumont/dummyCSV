#include <stdio.h>
#include <stdlib.h>
#include <a.out.h>
#include <string>
#include <cstring>
#include <iostream>

#define CSV_NAME "dummycsv.csv"
#define MAX_CELL_SIZE 32
#define MAX_STRING_TABLE 512
const char ME[MAX_CELL_SIZE] = "irSensor";
const char HE[MAX_CELL_SIZE] = "Kinect";
const char SHE[MAX_CELL_SIZE] = "Aura";

using namespace std;

typedef struct cell{
    char string[MAX_CELL_SIZE];

    struct cell * right;
    struct cell * down;
}t_cell;

char table[MAX_STRING_TABLE];


t_cell * firstCell;

void printTable(){

  t_cell * freeCell = firstCell;
  t_cell * freeDown = firstCell;

  while (freeCell->right != NULL){
    freeCell = freeCell->right;
    cout << freeCell->string;

    if (freeCell->down != NULL){
      freeDown = freeCell->down;
      cout << freeDown->string;
      while (freeDown->down !=NULL){
        freeDown = freeDown->down;
        cout << freeDown->string;
      }
    }
    cout << endl;
  }

  cout << endl <<"-----"<< endl;

}

void addRightCell(t_cell * currentCell, const char * sensor){

  t_cell * freeCell = firstCell;
  t_cell * newCell = (t_cell *) malloc(sizeof(t_cell));
  snprintf(newCell->string, MAX_CELL_SIZE, "%s", sensor);
  newCell->right = NULL;
  newCell->down = NULL;

  if (currentCell == NULL){

    while(freeCell->right != NULL){
      freeCell = freeCell->right;
    }

    freeCell->right = newCell;
  }else{

    currentCell->right = newCell;
  }
}

void addDownCell(const char * sensor, const char * start, const char* stop){

  t_cell * freeCell = firstCell;
  t_cell * newCellStart = (t_cell *) malloc(sizeof(t_cell));
  t_cell * newCellStop = (t_cell *) malloc(sizeof(t_cell));
  snprintf(newCellStart->string, MAX_CELL_SIZE, ",%s", start);
  newCellStart->right = NULL;
  newCellStart->down = newCellStop;

  snprintf(newCellStop->string, MAX_CELL_SIZE, ",%s,", stop);
  newCellStop->right = NULL;
  newCellStop->down = NULL;

  // Find if sensor is already registered
  while( strcmp(freeCell->string, sensor) != 0 ) {

    if (freeCell->right != NULL){

      freeCell = freeCell->right;
    }
    else{
      addRightCell(freeCell, sensor);
      break;
    }
  }

  while(freeCell->down != NULL){
    freeCell = freeCell->down;
  }

  freeCell->down = newCellStart;

}

void generateTable(){

  t_cell * freeCell = firstCell;
  t_cell * freeDown = firstCell;

  while (freeCell->right != NULL){
    freeCell = freeCell->right;
    strncat(table, freeCell->string, MAX_STRING_TABLE-strlen(table));

    if (freeCell->down != NULL){
      freeDown = freeCell->down;
      strncat(table, freeDown->string, MAX_STRING_TABLE-strlen(table));
      while (freeDown->down !=NULL){
        freeDown = freeDown->down;
        strncat(table, freeDown->string, MAX_STRING_TABLE-strlen(table));
      }
    }
    cout << endl;
    strncat(table, "\n\0", MAX_STRING_TABLE-strlen(table));
  }
}


int main (){

  firstCell = (t_cell*) malloc(sizeof(t_cell));
  firstCell->right = NULL;
  firstCell->down = NULL;
  memset(firstCell->string, 0, MAX_CELL_SIZE);

  addRightCell(NULL, ME);
  addRightCell(NULL, HE);
  addRightCell(NULL, SHE);
  printTable();
  addDownCell(ME, "1", "2");printTable();
  addDownCell(ME, "3", "4");printTable();
  addDownCell(ME, "5", "6");printTable();

  addDownCell(HE, "0", "1");printTable();
  addDownCell(HE, "1", "2");printTable();

  addDownCell(SHE, "6", "7");printTable();

  generateTable();

  cout << table << endl;

  char cmdCSV[MAX_STRING_TABLE+20];
  snprintf(cmdCSV, MAX_STRING_TABLE+20, "echo \"%s\" > %s", table, CSV_NAME);

  system(cmdCSV);
  
  return 0;
}
