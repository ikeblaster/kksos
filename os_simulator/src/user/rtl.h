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

THandle Get_Std_Handle(DWORD nStdHandle);


//podle flags otevre, vytvori soubor a vrati jeho deskriptor
//vraci nenulovy handle, kdyz vse OK
THandle Create_File(const char* file_name, size_t flags);

//zapise do souboru identifikovaneho deskriptor data z buffer o velikosti buffer_size a vrati pocet zapsanych dat ve writtent
//vraci true, kdyz vse OK
bool Write_File(const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &written);

bool Read_File(const THandle file_handle, const void **buffer, const size_t buffer_size, size_t &written);

fpos_t Seek_File(const THandle file_handle, const fpos_t pos, std::ios_base::seekdir way);

//uzavre soubor identifikovany pomoci deskriptoru
//vraci true, kdyz vse OK
bool Close_File(const THandle file_handle);
