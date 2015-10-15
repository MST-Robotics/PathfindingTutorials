/*
 * Grid_2D.h
 *
 *  Created on: Oct 8, 2015
 *      Author: shae
 */

#ifndef GRID_2D_H_
#define GRID_2D_H_
#include <stdint.h>
#include <utility>
#include <fstream>
#include <math.h>
#include <iostream>

using namespace std;

enum cell_adjacent
{
	TOP,           	// 128 index 0
	TOP_LEFT,      	// 64  index 1
	TOP_RIGHT, 		// 32  index 2
	LEFT,			// 16  index 3
	RIGHT,			// 8   index 4
	BOTTOM,			// 4   index 5
	BOTTOM_LEFT,	// 2   index 6
	BOTTOM_RIGHT  	// 1   index 7
};

enum cell_display
{
	BLOCKED = 35, // '#'
	OPEN = 34, // '-'
	TRAVELED = 46, // '.'
};

class Grid_2D
{//table of char->value conversions?
	public:
		uint32_t width;
		uint32_t height;
		uint8_t *connections;
		uint8_t *displays;
		uint8_t *weights;
		uint8_t *path;



        Grid_2D(const uint32_t width, const uint32_t height);
        Grid_2D(const char filename[]);
        Grid_2D(const Grid_2D &rhs);
        Grid_2D& operator=(const Grid_2D &rhs);
        bool operator==(const Grid_2D &rhs);
        bool operator!=(const Grid_2D &rhs);
		//Grid_2D(uint width, uint height, uint8_t displays[]);
		//Grid_2D(uint width, uint height, uint8_t displays[], uint8_t weights[], uint8_t connections[]);
		//void set_weights(const uint8_t weights[]);
        //void set_displays(const uint8_t displays[]);
        //void set_connections(const uint8_t connections[]);
		pair<uint32_t, uint32_t> neighbor_to_pair(pair<uint32_t, uint32_t> node, cell_adjacent neighbor);
		pair<uint32_t, uint32_t> get_neighbor(pair<uint32_t,uint32_t> node, cell_adjacent neighbor);
		float get_edge(pair<uint32_t, uint32_t> start, cell_adjacent neighbor);
		bool is_neighbor(pair<uint32_t, uint32_t> node, pair<uint32_t, uint32_t> neighbor);

		inline uint32_t to_index(const uint32_t row, const uint32_t col) const
		{
			return ( row * this->width ) + col;
		};

	    inline uint8_t get_weight(pair<uint32_t, uint32_t> node) const
	    {
	    	return this->weights[to_index(node.first, node.second)];
	    };

	    inline bool is_valid_node(pair<uint32_t, uint32_t> node) const
	    {
	    	return to_index(node.first, node.second) < (width * height);
	    };

	    void set_display(pair<uint32_t, uint32_t> node, char display);
	    void place_path(pair<uint32_t, uint32_t> node);
	    void restart_path();
	    void print_grid();
	    void print_path();
	    ~Grid_2D();

	private:
	    void allocate_variables (const Grid_2D &rhs);
};


#endif /* GRID_2D_H_ */
