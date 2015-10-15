/*
 * main.cpp
 *
 *  Created on: Oct 12, 2015
 *      Author: shae
 */

#include "Grid_2D.h"

using namespace std;

int main()
{
	char filename[] = "test_map.txt";
	Grid_2D grid(filename);
	grid.print_grid();
	return 0;
}
