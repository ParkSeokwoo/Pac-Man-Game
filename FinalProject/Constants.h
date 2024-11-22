#pragma once

/* OpenGL window constants */
#define WINDOW_X 300
#define WINDOW_Y 120

#define WINDOW_W 640		// window's width
#define WINDOW_H 640		// window's height

#define BOUNDARY_X (WINDOW_W)/3.3
#define BOUNDARY_Y (WINDOW_H)/3.3



/* GAME constants */
#define NUM_ROW 31
#define NUM_COL 28

#define BLOCK_SIZE 10
//14번 수정
#define MOVE_SPEED 0.8f
//

// map boundary
#define LEFT_BOUNDARY (-(NUM_COL / 2) * BLOCK_SIZE)
#define RIGHT_BOUNDARY ((NUM_COL / 2) * BLOCK_SIZE)
#define TOP_BOUNDARY ((NUM_ROW / 2) * BLOCK_SIZE)
#define BOTTOM_BOUNDARY (-(NUM_ROW / 2) * BLOCK_SIZE)

//15번 수정
#define M_PI 3.141592f
#define R_RATIO 1.6
//