#include "Tiles.h"

bool isSolid(int tileID){ //no Tiles:: bc there is no Tiles class or namespace
	switch(tileID){
		// Ground
		case STONE_LEFT:        case STONE_INNER:        case STONE_RIGHT:
		case STONE_LEFT_RISER:  case STONE_INNER_RISER:  case STONE_RIGHT_RISER:
		case PATTERN_BLOCK:
		// Pipes
		case PIPE_TOP_LEFT: case PIPE_TOP_RIGHT:
		case PIPE_LEFT_EDGE: case PIPE_RIGHT_EDGE:
		// Item blocks (still solid even after being hit, for now)
		case QUESTION_BLOCK_1: case QUESTION_BLOCK_2:
		case QUESTION_BLOCK_3: case QUESTION_BLOCK_4:
		case BRICK_BLOCK_1:    case BRICK_BLOCK_2:
		case BRICK_BLOCK_3:    case BRICK_BLOCK_4:
			return true;
		default:
			return false;
	}
}