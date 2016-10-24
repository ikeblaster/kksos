#pragma once

#define FS_DIRECTORY 0
#define FS_FILE 1

#include <string>
#include <map>


typedef struct fsnode {
	int type;
	struct fsnode *parent;
} fsnode;

typedef struct fsfile : fsnode {
	std::string data;
};

typedef struct fsdir : fsnode {
	std::map<std::string, struct fsnode*> children;
};
