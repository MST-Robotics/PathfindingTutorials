/*
 * Grid_2D.cpp
 *
 *  Created on: Oct 12, 2015
 *      Author: shae
 */



#include "Grid_2D.h"
#define INF 255 //our fake infinity
#define MAX_NEIGHBORS 8

Grid_2D :: Grid_2D(uint32_t width, uint32_t height)
{
	this->width = width;
    this->height = height;
    this->connections = NULL;
    this->displays = NULL;
    this->weights = NULL;
}

Grid_2D :: Grid_2D(const char filename[])
{
	ifstream fin;
	fin.open(filename);
	fin >> this->width;
	fin >> this->height;
	this->displays = new uint8_t[this->width * this->height]();
	this->connections = new uint8_t[this->width * this->height]();
	this->weights= new uint8_t[this->width * this->height]();
	this->path = new uint8_t[this->width * this->height]();

	uint32_t index;

	for(uint32_t i = 0; i < this->height; i ++)
	{
		for(uint32_t j = 0; j < this->width; j++)
		{
			index = to_index(i , j);
			fin >> this->displays[index];
			if(this->displays[index] == BLOCKED)
			{
				this->weights[index] = INF;
			}
			else
			{
				this->weights[index] = 1;
			}
		}
	}
	fin.close();

	uint32_t top;
	uint32_t top_L;
	uint32_t top_R;
	uint32_t left;
	uint32_t right;
	uint32_t bot;
	uint32_t bot_L;
	uint32_t bot_R;

	bool not_top_edge;
	bool not_bot_edge;
	bool not_left_edge;
	bool not_right_edge;
	for(uint32_t i = 0; i < this->height; i ++)
	{
		for(uint32_t j = 0; j < this->width; j++)
		{
			index = to_index(i, j);
			top = to_index(i + 1, j);
			top_L = to_index(i + 1, j - 1);
			top_R = to_index(i + 1, j + 1);
			left = to_index(i, j - 1);
			right = to_index(i, j + 1);
			bot = to_index(i - 1, j);
			bot_L = to_index(i - 1, j - 1);
			bot_R = to_index(i - 1, j + 1);


			not_top_edge = i ? 1 : 0;
			not_bot_edge = !(height - (i + 1)) ? 1 : 0;
			not_left_edge = j ? 1 : 0;
			not_right_edge = !(width - (j + 1)) ? 1 : 0;

			this->connections[index] = 0;

			if(not_top_edge)
			{
				// index = 1000 0000 = 128
				this->connections[index] + (this->weights[top] != INF) ? 128 : 0;

				if(not_left_edge)
				{
					// index = 0100 0000 = 64
					this->connections[index] + (this->weights[top_L] != INF) ? 64 : 0;
				}
				if(not_right_edge)
				{
					// index = 0010 0000 = 32
					this->connections[index] + (this->weights[top_R] != INF) ? 32 : 0;
				}
			}

			if(not_left_edge)
			{
				// index = 0001 0000 = 16
				this->connections[index] + (this->weights[left] != INF) ? 16 : 0;
			}
			if(not_right_edge)
			{
				// index = 0000 1000 = 8
				this->connections[index] + (this->weights[left] != INF) ? 8 : 0;
			}

			if(not_bot_edge)
			{
				// index = 0000 0100 = 4
				this->connections[index] + (this->weights[bot] != INF) ? 4 : 0;

				if(not_left_edge)
				{
					// index = 0000 0010 = 2
					this->connections[index] + (this->weights[bot_L] != INF) ? 2 : 0;
				}
				if(not_right_edge)
				{
					// index = 0000 0001 = 1
					this->connections[index] + (this->weights[bot_R] != INF) ? 1 : 0;
				}
			}
		}
	}
}


void Grid_2D :: allocate_variables (const Grid_2D &rhs)
{
	this->displays = new uint8_t[this->width * this->height];
	this->connections = new uint8_t[this->width * this->height];
	this->weights= new uint8_t[this->width * this->height];
	this->path = new uint8_t[this->width * this->height]();

	this->width = rhs.width;
	this->height = rhs.height;
	uint32_t index;

	for(uint32_t i = 0; i < this->height; i++)
	{
		for(uint32_t j = 0; j < this->width; j++)
		{
			index = to_index(i, j);
			this->connections[index] = rhs.connections[index];
			this->displays[index] = rhs.displays[index];
			this->weights[index] = rhs.weights[index];
			this->path[index] = rhs.path[index];
		}
	}
}

Grid_2D :: Grid_2D(const Grid_2D &rhs)
{
	allocate_variables(rhs);
}

Grid_2D& Grid_2D :: operator=(const Grid_2D &rhs)
{
	if(this != &rhs)
	{
		delete this->connections;
		delete this->displays;
		delete this->weights;
		delete this->path;
		this->connections = NULL;
		this->displays = NULL;
	    this->weights = NULL;
	    this->path = NULL;
	    allocate_variables(rhs);
	}
	return *this;
}

bool Grid_2D :: operator==(const Grid_2D &rhs)
{
  if(this->width != rhs.width || this->height != rhs.height)
  {
	  return false;
  }
  uint32_t index;

  for(uint32_t i = 0; i < this->height; i++)
  	{
  		for(uint32_t j = 0; j < this->width; j++)
  		{
  			index = to_index(i, j);
  			if(this->connections[index] != rhs.connections[index])
  			{
  				return false;
  			}
  			if(this->displays[index] != rhs.displays[index])
  			{
  				return false;
  			}
  			if(this->weights[index] != rhs.weights[index])
  			{
  				return false;
  			}
  			if(this->path[index] != rhs.path[index])
  			{
  				return false;
  			}
  		}
  	}
  return true;
}

bool Grid_2D :: operator!=(const Grid_2D &rhs)
{
	return !(*this == rhs);
}

pair<uint32_t, uint32_t> Grid_2D :: neighbor_to_pair(pair<uint32_t, uint32_t> node, cell_adjacent neighbor)
{
	pair<uint32_t, uint32_t> return_node = node;
	switch(neighbor)
	{
	case TOP :
		return_node.first += 1;
		break;
	case TOP_LEFT :
		return_node.first += 1;
		return_node.second -= 1;
		break;
	case TOP_RIGHT :
		return_node.first += 1;
		return_node.second += 1;
		break;
	case LEFT :
		return_node.second -= 1;
		break;
	case RIGHT :
		return_node.second += 1;
		break;
	case BOTTOM :
		return_node.first -= 1;
		break;
	case BOTTOM_LEFT :
		return_node.first -= 1;
		return_node.second -= 1;
		break;
	case BOTTOM_RIGHT :
		return_node.first -= 1;
		return_node.second += 1;
		break;
	}

	return return_node;
}

pair<uint32_t, uint32_t> Grid_2D :: get_neighbor(pair<uint32_t,uint32_t> node, cell_adjacent neighbor)
{
	pair<uint32_t, uint32_t> neighbor_node = neighbor_to_pair(node, neighbor);
	if(!is_valid_node(node) || !is_neighbor(node, neighbor_node))
	{
		return node;
	}
	return neighbor_node;
}

float Grid_2D :: get_edge(pair<uint32_t, uint32_t> start, cell_adjacent neighbor)
{
	pair<uint32_t, uint32_t> neighbor_node = neighbor_to_pair(start, neighbor);
	if(!is_valid_node(start) || !is_neighbor(start, neighbor_node))
	{
		return INFINITY;
	}

	uint8_t start_weight = get_weight(start);
	uint8_t neighbor_weight = get_weight(get_neighbor(start, neighbor));

	if(start_weight == INF || neighbor_weight == INF)
	{
		return INFINITY;
	}

	float edge = start_weight + neighbor_weight;
	edge /= 2;
	if(neighbor == TOP_LEFT || neighbor == TOP_RIGHT
	   || neighbor == BOTTOM_LEFT || neighbor == BOTTOM_RIGHT)
	{
		edge *= sqrt(2);
	}
	return edge;
}

bool Grid_2D :: is_neighbor(pair<uint32_t, uint32_t> node, pair<uint32_t, uint32_t> neighbor)
{
	if(!is_valid_node(node))
	{
		return false;
	}

	int64_t i = node.first;
	int64_t j = node.second;

	bool neighbor_possible[MAX_NEIGHBORS];
	int64_t neighbors[MAX_NEIGHBORS];

	neighbors[TOP] 		    = to_index(i + 1, j);
	neighbors[TOP_LEFT] 	= to_index(i + 1, j - 1);
	neighbors[TOP_RIGHT] 	= to_index(i + 1, j + 1);
	neighbors[LEFT]		    = to_index(i, j - 1);
	neighbors[RIGHT] 		= to_index(i, j + 1);
	neighbors[BOTTOM] 		= to_index(i - 1, j);
	neighbors[BOTTOM_LEFT]  = to_index(i - 1, j - 1);
	neighbors[BOTTOM_RIGHT] = to_index(i - 1, j + 1);

	neighbor_possible[TOP] 		    = i < height;
	neighbor_possible[TOP_LEFT] 	= i < height && j != 0;
	neighbor_possible[TOP_RIGHT] 	= i < height && j < width;
	neighbor_possible[LEFT]		    = j != 0;
	neighbor_possible[RIGHT] 		= j < width;;
	neighbor_possible[BOTTOM] 		= i != 0;
	neighbor_possible[BOTTOM_LEFT]  = i != 0 && j != 0;
	neighbor_possible[BOTTOM_RIGHT] = i != 0 && j < width;

	uint32_t index = to_index(neighbor.first, neighbor.second);
	uint32_t max_index = this->width*this->height;

	for(uint32_t k = 0; k < MAX_NEIGHBORS; k ++)
	{
		if(index == neighbors[k] && neighbor_possible[k])
		{
			return true;
		}
	}
	return false;
}

void Grid_2D :: set_display(pair<uint32_t, uint32_t> node, char display)
{
	if(!is_valid_node(node))
	{
		uint32_t index = to_index(node.first, node.second);
		if(display != '.' && display != '#')
		{
			this->displays[index] = (uint8_t)display;
		}
	}
	return;
}

void Grid_2D :: place_path(pair<uint32_t, uint32_t> node)
{
	uint32_t index = to_index(node.first, node.second);
	if(!is_valid_node(node))
	{
		this->path[index] = TRAVELED;
	}
	return;
}

void Grid_2D :: restart_path()
{
	delete this->path;
	this->path = new uint8_t[this->width * this->height]();
}

void Grid_2D :: print_grid()
{
	uint32_t index;
	for(uint32_t i = 0; i < this->height; i++)
	{
		for(uint32_t j = 0; j < this->width; j++)
		{
			index = to_index(i,j);
			cout << (char)(this->displays[index]);
		}
		cout << endl;
	}
	cout << endl;
}

void Grid_2D :: print_path()
{
	uint32_t index;
	for(uint32_t i = 0; i < this->height; i++)
	{
		for(uint32_t j = 0; j < this->width; j++)
		{
			index = to_index(i,j);
			if(this->path[index] == TRAVELED)
			{
				cout << (char)TRAVELED;
			}
			else
			{
				cout << (char)(this->displays[index]);
			}
		}
		cout << endl;
	}
	cout << endl;
}

Grid_2D :: ~Grid_2D()
{
	delete this->connections;
	delete this->displays;
	delete this->weights;
	delete this->path;
	this->connections = NULL;
	this->displays = NULL;
    this->weights = NULL;    this->path = NULL;
}

