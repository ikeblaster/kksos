#pragma once
#include <atomic>
#include <sstream> 
#include <vector>
#include "..\common\api.h"

size_t Get_Last_Error();


int Create_Process(std::string process_name, std::vector<char> params, 
					std::vector<std::string> data, 
					const THandle hstdin, const THandle hstdout, const THandle hstderr
					);

bool Join_Process(int PID);

THandle Create_File(const char* file_name, size_t flags);
		//podle flags otevre, vytvori soubor a vrati jeho deskriptor
		//vraci nenulovy handle, kdyz vse OK
bool Write_File(const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &written);
		//zapise do souboru identifikovaneho deskriptor data z buffer o velikosti buffer_size a vrati pocet zapsanych dat ve writtent
		//vraci true, kdyz vse OK
bool Read_File(const THandle file_handle, const void **buffer, const size_t buffer_size, size_t &written);

bool Close_File(const THandle file_handle);
		//uzavre soubor identifikovany pomoci deskriptoru
		//vraci true, kdyz vse OK

int Seek_File(const fpos_t pos, std::ios_base::seekdir way);