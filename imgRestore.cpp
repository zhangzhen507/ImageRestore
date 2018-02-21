/*
 * imgRestore.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: zzhen
 *
 *      The test taker must implement the test() function within the User code.
 *      Maximize the SCORE within the given 5 sec.
 */

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

using namespace std;

const int cnt = 100;
const int BLOCK = 32;
const int X = 2048;
const int Y = 2048;

extern void test(char bitmap[][X]);

static char bitmap[Y][X];
static char bitmap_org[Y][X];

static void output(char bitmap[][X]);

static void rect(int sx, int sy, int ex, int ey) {
	memset(&bitmap[sy][sx], 1, ex - sx + 1);

	memset(&bitmap[ey][sx], 1, ex - sx + 1);

	for (register int y = sy + 1; y <= ey - 1; y++) {

		bitmap[y][sx] = 1;

		bitmap[y][ex] = 1;
	}
}

static void swap(int sx, int sy, int dx, int dy) {
	for (register int y = 0; y < BLOCK; y++) {
		for (register int x = 0; x < BLOCK; x++) {
			register int t = bitmap[sy + y][sx + x];
			bitmap[sy + y][sx + x] = bitmap[dy + y][dx + x];
			bitmap[dy + y][dx + x] = t;
		}
	}
}
/**
 * Test function
 */
static void output(char bitmap[][X]) {
	for (register int y = 0; y < Y; y++) {
		for (register int x = 0; x < X; x++) {
			printf("%d ", bitmap[y][x]);
		}
		printf("\n");
	}
	printf("\n");
}
static void build(void) {
	memset(bitmap, 0, sizeof(bitmap));

	for (register int c = 0; c < cnt; c++) {
		int sx, sy, ex, ey;
		do {
			sx = rand() % (X - BLOCK);
		} while (sx % BLOCK == 0 || sx % BLOCK == BLOCK - 1);

		do {
			sy = rand() % (Y - BLOCK);
		} while (sy % BLOCK == 0 || sy % BLOCK == BLOCK - 1);

		do {
			ex = rand() % (X - (sx + BLOCK)) + (sx + BLOCK);
		} while (ex % BLOCK == 0 || ex % BLOCK == BLOCK - 1);

		do {
			ey = rand() % (Y - (sy + BLOCK)) + (sy + BLOCK);
		} while (ey % BLOCK == 0 || ey % BLOCK == BLOCK - 1);

		rect(sx, sy, ex, ey);
	}

	printf("Build square:\n");
	output(bitmap);

	for (register int y = 0; y < Y; y++) {
		memcpy(bitmap_org[y], bitmap[y], X);
	}

	for (register int c = 0; c < cnt; c++) {
		int sx = (rand() % (X / BLOCK)) * BLOCK;
		int sy = (rand() % (Y / BLOCK)) * BLOCK;
		int dx = (rand() % (X / BLOCK)) * BLOCK;
		int dy = (rand() % (Y / BLOCK)) * BLOCK;
		if ((sx == dx) && (sy == dy)) {
			c--;
			continue;
		}
		swap(sx, sy, dx, dy);
	}

	printf("After swap: \n");
	output(bitmap);
}

static bool check(void) {
	for (register int y = 0; y < Y; y++) {
		if (memcmp(bitmap_org[y], bitmap[y], X) != 0) {
			return false;
		}
	}
	return true;
}

int main() {
	int count = 0;
	int SCORE = 0;

	freopen("out.txt", "w", stdout);

	srand(3);

//	while (true) {
	for (int i = 0; i < 1; i++) {
		build();

		test(bitmap);

		if (check() == true) {
			SCORE++;
		}

		cout << "#" << ++count << " " << SCORE << endl;
	}
	return 0;
}

typedef struct Point {
	int i;
	int j;
} Point;

typedef struct Vertical {
	Point up;
	Point down;
} Vertical;

typedef struct Horizontal {
	Point left;
	Point right;
} Horizontal;

typedef struct LeftDown {
	Point up;
	Point right;
} LeftDown;

typedef struct RightUp {
	Point left;
	Point down;
} RightUp;

typedef struct RightDown {
	Point up;
	Point left;
} RightDown;

typedef struct LeftUp {
	Point right;
	Point down;
} LeftUp;

typedef struct Block {

	Vertical vertical[cnt];
	int vLen;

	Horizontal horizontal[cnt];
	int hLen;

	LeftUp leftup[cnt];
	int lupLen;

	LeftDown leftdown[cnt];
	int ldownLen;

	RightUp rightup[cnt];
	int rupLen;

	RightDown rightdown[cnt];
	int rdownLen;
} Block;

Block blocks[64][64];

void addVertical(Block* block, int x1, int x2, int y) {
	int n = block->vLen;
	block->vertical[n].up.i = x1;
	block->vertical[n].up.j = y;
	block->vertical[n].down.i = x2;
	block->vertical[n].down.j = y;
	n++;
	block->vLen = n;
}

void addHorizontal(Block* block, int x, int y1, int y2) {
	int n = block->hLen;
	block->horizontal[n].left.i = x;
	block->horizontal[n].left.j = y1;
	block->horizontal[n].right.i = x;
	block->horizontal[n].right.j = y2;
	n++;
	block->hLen = n;
}

void addRightUp(Block* block, int leftX, int leftY, int downX, int downY) {
	int n = block->rupLen;
	block->rightup[n].left.i = leftX;
	block->rightup[n].left.j = leftY;
	block->rightup[n].down.i = downX;
	block->rightup[n].down.j = downY;

	n++;
	block->rupLen = n;
}

void addRightDown(Block* block, int leftX, int leftY, int upX, int upY) {
	int n = block->rdownLen;
	block->rightdown[n].left.i = leftX;
	block->rightdown[n].left.j = leftY;
	block->rightdown[n].up.i = upX;
	block->rightdown[n].up.j = upY;

	n++;
	block->rdownLen = n;
}

void addLeftUp(Block* block, int rightX, int rightY, int downX, int downY) {
	int n = block->lupLen;
	block->leftup[n].right.i = rightX;
	block->leftup[n].right.j = rightY;
	block->leftup[n].down.i = downX;
	block->leftup[n].down.j = downY;

	n++;
	block->lupLen = n;
}

void addLeftDown(Block* block, int rightX, int rightY, int upX, int upY) {
	int n = block->ldownLen;
	block->leftdown[n].right.i = rightX;
	block->leftdown[n].right.j = rightY;
	block->leftdown[n].up.i = upX;
	block->leftdown[n].up.j = upY;

	n++;
	block->ldownLen = n;
}
int checkVertical(char bitmap[][X], int x1, int x2, int y) {
	int i;
	for (i = x1; i <= x2; i++) {
		if (bitmap[i][y] != 1) {
			return 0;
		}
	}
	return 1;
}

int checkHorizontal(char bitmap[][X], int x, int y1, int y2) {
	int j;
	for (j = y1; j <= y2; j++) {
		if (bitmap[x][j] != 1) {
			return 0;
		}
	}
	return 1;
}

int checkRightUp(char bitmap[][X], int leftX, int leftY, int downX, int downY) {
	for (register int j = leftY; j <= downY; j++) {
		if (bitmap[leftX][j] != 1) {
			return 0;
		}
	}

	for (register int i = leftX; i <= downX; i++) {
		if (bitmap[i][downY] != 1) {
			return 0;
		}
	}
	return 1;
}

int checkRightDown(char bitmap[][X], int leftX, int leftY, int upX, int upY) {
	for (register int j = leftY; j <= upY; j++) {
		if (bitmap[leftX][j] != 1) {
			return 0;
		}
	}

	for (register int i = upX; i <= leftX; i++) {
		if (bitmap[i][upY] != 1) {
			return 0;
		}
	}
	return 1;
}

int checkLeftUp(char bitmap[][X], int rightX, int rightY, int downX,
		int downY) {
	for (register int j = downY; j <= rightY; j++) {
		if (bitmap[rightX][j] != 1) {
			return 0;
		}
	}

	for (register int i = rightX; i <= downX; i++) {
		if (bitmap[i][downY] != 1) {
			return 0;
		}
	}
	return 1;
}

int checkLeftDown(char bitmap[][X], int rightX, int rightY, int upX, int upY) {
	for (register int j = upY; j <= rightY; j++) {
		if (bitmap[rightX][j] != 1) {
			return 0;
		}
	}

	for (register int i = upX; i <= rightX; i++) {
		if (bitmap[i][upY] != 1) {
			return 0;
		}
	}
	return 1;
}

void initBlock(Block block[][64], int i, int j, char bitmap[][X], int x, int y,
		int n) {
	//init vertical
	int regx, regy;
	char bit1, bit2;
	for (register int b = 0; b < n; b++) {
		regy = y + b;
		bit1 = bitmap[x][regy];
		bit2 = bitmap[x + n - 1][regy];
		if (1 == bit1 && 1 == bit2) {
			if (checkVertical(bitmap, x, x + n - 1, regy) == 1) {
				addVertical(&block[i][j], x, x + n - 1, regy);
			}
		}
	}

	//init horizontal
	for (register int a = 0; a < n; a++) {
		regx = x + a;
		bit1 = bitmap[regx][y];
		bit2 = bitmap[regy][y + n - 1];
		if (1 == bit1 && 1 == bit2) {
			if (checkHorizontal(bitmap, regx, y, y + n - 1) == 1) {
				addHorizontal(&block[i][j], regx, y, y + n - 1);
			}
		}
	}

	// init rightUp
	for (register int a = 0; a < n; a++) {
		bit1 = bitmap[x + a][y];
		for (register int b = 0; b < n; b++) {
			bit2 = bitmap[x + n - 1][y + b];
			if (1 == bit1 && 1 == bit2) {
				if (checkRightUp(bitmap, x + a, y, x + n - 1, y + b) == 1) {
					addRightUp(&block[i][j], x + a, y, x + n - 1, y + b);
				}
			}
		}
	}

	// init rightDown
	for (register int a = 0; a < n; a++) {
		bit1 = bitmap[x + a][y];
		for (register int b = 0; b < n; b++) {
			bit2 = bitmap[x][y + b];
			if (1 == bit1 && 1 == bit2) {
				if (checkRightDown(bitmap, x + a, y, x, y + b) == 1) {
					addRightDown(&block[i][j], x + a, y, x, y + b);
				}
			}
		}
	}

	// init leftUp
	for (register int a = 0; a < n; a++) {
		bit1 = bitmap[x + a][y + n - 1];
		for (register int b = 0; b < n; b++) {
			bit2 = bitmap[x + n - 1][y + b];
			if (1 == bit1 && 1 == bit2) {
				if (checkLeftUp(bitmap, x + a, y + n - 1, x + n - 1, y + b)
						== 1) {
					addLeftUp(&block[i][j], x + a, y + n - 1, x + n - 1, y + b);
				}
			}
		}
	}

	// init leftDown
	for (register int a = 0; a < n; a++) {
		bit1 = bitmap[x + a][y + n - 1];
		for (register int b = 0; b < n; b++) {
			bit2 = bitmap[x][y + b];
			if (1 == bit1 && 1 == bit2) {
				if (checkLeftDown(bitmap, x + a, y + n - 1, x, y + b) == 1) {
					addLeftDown(&block[i][j], x + a, y + n - 1, x, y + b);
				}
			}
		}
	}
}

void initBlockMap(Block blocks[][64], char bitmap[][X]) {
	for (register int i = 0; i < 64; i++) {
		for (register int j = 0; j < 64; j++) {
			int x = 32 * i;
			int y = 32 * j;
			initBlock(blocks, i, j, bitmap, x, y, 32);
		}
	}
}

/**
 * Implement this test function
 */
void test(char bitmap[][X]) {

	// init block map
	initBlockMap(blocks, bitmap);


}

