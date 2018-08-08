
#pragma once
#include <time.h>
#include <conio.h>
#include <vector>
#include <list>
#include <stdarg.h>

#define UINT8	unsigned char
#define UINT16	unsigned short
#define UINT32	unsigned int

#define INT8	char
#define INT16	short
#define INT32	int

#define MINE_TRUE		1
#define	MINE_FALSE		0

#define MINE_NUM_0		0
#define MINE_NUM_1		1
#define MINE_NUM_2		2
#define MINE_NUM_3		3
#define MINE_NUM_4		4
#define MINE_NUM_5		5
#define MINE_NUM_6		6
#define MINE_NUM_7		7
#define MINE_NUM_8		8
#define MINE_BOOM		9
#define MINE_BOOMED		10
#define MINE_TRIANGLE	11
#define MINE_QUESTION	12
#define MINE_WIN		13
#define MINE_LOSE		14
#define MINE_PROCESS	15
#define MINE_CLOSED		16

#define SUBMENUGRAY_NULL	0
#define SUBMENUGRAY_PRIMACY	1
#define SUBMENUGRAY_MEDIUM	2
#define SUBMENUGRAY_SENIOR	3
#define SUBMENUGRAY_ALL		4

#define GRADE_PRIMACY_MAP_SIZE	10
#define GRADE_MEDIUM_MAP_SIZE	20
#define GRADE_SENIOR_MAP_SIZE	30

#define GRADE_PRIMACY_MINE_COUNT	10
#define GRADE_MEDIUM_MINE_COUNT		40
#define GRADE_SENIOR_MINE_COUNT		90

//#define GET_DEBUG_INFO

typedef struct tagMineNode {
	UINT8	x;
	UINT8	y;

	UINT8	isMine;
	UINT8	isMineNumCalc;
	UINT8	isFlag;
	UINT8	isQuestion;
	UINT8	isChosen;

	UINT8	type;
}S_MineNode, *pS_MineNode;

typedef struct tagMineBlockSize {
	UINT8	w;
	UINT8	h;
}mineBlockSize;

typedef struct tagLocation {
	UINT16	x;
	UINT16	y;
}Location, *pLocation;

typedef struct tagmineLocation {
	UINT8	x;
	UINT8	y;
}mineLocation, *pmineLocation;

typedef struct tagdebugInfo {
	UINT32  len;
	CString	info;
}debugInfo;

