#ifndef PLAYERS_H
#define PLAYERS_H

#include <stdint.h>
#include "go.h"

struct player;
typedef struct player {
	const char* name;
	move_result (*const play)(struct player*, state*, move*);
	void (*const observe)(struct player*, state*, color, move*);
	void* params;
} player;

move_result human_play(player*, state*, move*);

move_result randy_play(player*, state*, move*);

typedef struct {
	int N;
} karl_params;

move_result karl_play(player*, state*, move*);


#define N_TERESA_NODES 80000*(COUNT*0)+1
#define TERESA_RESIGN_THRESHOLD 0.01

struct teresa_node;
typedef struct teresa_node {
	struct teresa_node* parent;	// 8b
	struct teresa_node* sibling;// 8b
	struct teresa_node* child;	// 8b = 24
	float pwin;					// 4b
	float sqlg_visits;			// 4b	// BUG ONLY READ USING node_sqlg_visits(&node)
	float rsqrt_visits;			// 4b	// BUG ONLY READ USING node_rsqrt_visits(&node)
	uint32_t wins;				// 4b	// BUG Must also set pwin NAN!
	uint32_t visits;			// 4b	// BUG Must also set pwin, sqlg_visits, rsqrt_visits NAN!
	move mv;					// 2b
	color pl;					// 1b
} teresa_node;

struct teresa_pool;
typedef struct teresa_pool {
	struct teresa_node mem[N_TERESA_NODES];
	struct teresa_node* free;
	struct teresa_node* used;
} teresa_pool;

typedef struct {
	int N;
	float C;
	float FPU;
	struct teresa_pool* pool;
	struct teresa_node* root;
} teresa_params;

move_result teresa_play(player*, state*, move*);
void teresa_observe(player*, state*, color, move*);

void g(teresa_node*);
void g2(teresa_node*, const char*, int, int);

#endif