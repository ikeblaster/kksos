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

/* Vytvori soubor v adresari "parent" se jmenem "name" a obsahem "data" */
void create_file(fsdir *parent, std::string name, std::string data);


/* Vytvori v adresari "parent" novy adresar se jmenem "name" */
void create_dir(fsdir *parent, std::string name);


/* Smaze soubor "name" v adresari "parent" */
void delete_file(fsdir *parent, std::string name);


/* Smaze adresar "name" v adresari "parent" */
void delete_dir(fsdir *parent, std::string name);

//void open_file(fsdir *parent, std::string name);


/* Vrati obsah souboru "name" v adresari "parent" */
std::string read_file(fsdir *parent, std::string name);


/* Vrati predka uzlu "node" */
fsdir get_parent(fsnode *node);


/* Zkopiruje soubor "name" z adresare "from" do adresare "to" */
void copy_file(fsdir *from, fsdir *to, std::string name);


/* Premisti soubor "name" z adresare "from" do adresare "to" */
void move_file(fsdir *from, fsdir *to, std::string name);