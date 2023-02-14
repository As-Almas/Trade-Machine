#include "pch.h"
#include "png.h"
#include "zlib.h"

#include <vector>
#include <string>
 
//  ZEXTERN int ZEXPORT uncompress OF((Bytef *dest,   uLongf *destLen,
//          const Bytef* source, uLong sourceLen));

/// <summary>
/// Structure of png chunk. 
/// Have a 3 variables:
/// 1) data_length - a variable of unsigned __int32 type. This variable save a information about length 
/// of png chunk data
/// 2) name - a array of char's. Used for save a type(name) of png chunk. Max size 5 bytes (4 bytes for
/// name and 1 byte for null-terminated symbol('\0')
/// 3) data - a array of char's. Used for save data of chunk. Max size data_length+1 (last byte for null
/// terminated symbol('\0'). 
/// Note:
/// CRC32 is not saved. CRC32 would been checked in the function ParseOnChunks.
/// </summary>
struct PNG_CHUNK {
    unsigned __int32 data_length; // max - 4 bytes
    unsigned char* name; // max - 4 bytes
    unsigned char* data; // max - int(data_length)
};

struct PNG_PARAMETRS {
    unsigned __int32 width, height;
    char bit_depth;
    char color_type;
    char compress_type;
    char filter_type;
    char Interlance;
};

/// <summary>
/// Checked signature of png file
/// </summary>
/// <param name="file_text">Text of png file</param>
/// <returns>if signature of file equal to png signature return true else return false</returns>
bool CheckSignature(unsigned char* file_text) {
    bool isPNG = false;
    
    char* pngSignature = new char[8];
    //89 50(P) 4E(N) 47(G) 0D 0A 1A 0A
    pngSignature[0] = 89;
    pngSignature[1] = 'P';
    pngSignature[2] = 'N';
    pngSignature[3] = 'G';
    pngSignature[4] = 0x0D;
    pngSignature[5] = 0x0A;
    pngSignature[6] = 0x1A;
    pngSignature[7] = 0x0A;

    for (int i = 0; i < 8; i++) 
        isPNG = (file_text[i] == pngSignature[i]);
    
    delete pngSignature;
    return isPNG;
}

/// <summary>
/// Parse the png file text on the chunks. If file is corrupted possible critical errors
/// </summary>
/// <param name="file_text">Text of the png file</param>
/// <param name="length">Length of png file in bytes</param>
/// <param name="parsed">Pointer to the array, where should be saved a information from file</param>
/// <returns>return true, if file is OK else retunr false or call error</returns>
bool ParseOnChunks(unsigned char* file_text, int length, std::vector<PNG_CHUNK*>& parsed) {
    int i = 8; 
    HMODULE zlib = LoadLibraryA("zlib.dll");
    if (zlib == NULL)
        return false;
    typedef ULONG(*CRC32_TYPE)(ULONG crc, unsigned char*buf, unsigned int c);
    CRC32_TYPE crc32 = (CRC32_TYPE)GetProcAddress(zlib, "crc32");
    while (i < length) {
        PNG_CHUNK* chunk = new PNG_CHUNK;
        chunk->data_length = 0;

        chunk->data_length += (unsigned __int32)(file_text[i]);
        chunk->data_length <<= 24;
        chunk->data_length += (unsigned __int32)(file_text[i+1]);
        chunk->data_length <<= 16;
        chunk->data_length += (unsigned __int32)(file_text[i+2]);
        chunk->data_length <<= 8;
        chunk->data_length += (unsigned __int32)file_text[i+3];
        i += 4;
        
        chunk->name = new unsigned char[5];
        chunk->name[0] = file_text[i];
        chunk->name[1] = file_text[i + 1];
        chunk->name[2] = file_text[i + 2];
        chunk->name[3] = file_text[i + 3];
        chunk->name[4] = '\0';
        i += 4;
        
       
        
        unsigned __int32 computedCRC = crc32(0, &file_text[i - 4], 4 + chunk->data_length);
        

        chunk->data = new unsigned char[chunk->data_length+1];
        for (int x = i + chunk->data_length, j = 0; i < x; i++, j++)
            chunk->data[j] = file_text[i];
        chunk->data[chunk->data_length] = '\0';

        unsigned __int32 crcInFile = 0;
        crcInFile += (__int32)(file_text[i]) << 24;
        crcInFile += (__int32)(file_text[i + 1]) << 16;
        crcInFile += (__int32)(file_text[i + 2])<< 8;
        crcInFile += (__int32)file_text[i + 3];

        i += 4;
        if (crcInFile != computedCRC)
            return false;
        (parsed).push_back(chunk);
    }
    FreeLibrary(zlib);
    return true;
}

/// <summary>
/// Clear the memory. Delete from memory array of chunks.
/// </summary>
/// <param name="parsed">Array of chunks</param>
void FreeChunks(std::vector<PNG_CHUNK*>& parsed) {
    for (auto x : parsed) {
        delete x->data;
        delete x;
    }
    parsed.clear();
}

/// <summary>
/// This function compare the chunk name and required name 
/// </summary>
/// <param name="name">name of chunk (max length 4 symbols)</param>
/// <param name="needName">required name (max length 4 symbols)</param>
/// <returns>return true, if name == needName else return false</returns>
bool CheckChunkName(unsigned char* name, LPCSTR needName) {
    return (
        needName[0] == name[0] &&
        needName[1] == name[1] &&
        needName[2] == name[2] &&
        needName[3] == name[3] 
        );
}

/// <summary>
/// This function delete the option chunks from array
/// </summary>
/// <param name="parsed">array of chunks</param>
/// <returns>return true, if array is not empty else return false</returns>
bool SortChunks(std::vector<PNG_CHUNK*>& parsed) {
    for (int i = 0; i < parsed.size(); i++) {
        if (!(
            CheckChunkName(parsed[i]->name, "IHDR") ||
            CheckChunkName(parsed[i]->name, "IDAT") ||
            CheckChunkName(parsed[i]->name, "IEND")
            )) {
            delete parsed[i]->name;
            delete parsed[i]->data;
            for (int j = i; j < parsed.size()-1; j++) {
                parsed[j] = parsed[j + 1];
            }
            i--;
            parsed.pop_back();
        }
    }
    if (parsed.empty())
        return false;
    return true;
}

/// <summary>
/// Function for load from IHDR chunk of png a parametrs of png (width,height and etc...).
/// </summary>
/// <param name="parsed">A array of png chunks after of SortChunks name</param>
/// <param name="params">Result of of function</param>
/// <returns>If IHDR chunk is not corrupted return true and png parametrs in params else return false.</returns>
bool GetPNGParametrs(std::vector<PNG_CHUNK*>& parsed, PNG_PARAMETRS* params) {
    if (parsed.size() < 1)
        return false;
    PNG_CHUNK* IHDR = parsed[0];
    if (!CheckChunkName(IHDR->name, "IHDR"))
        return false;

    if (IHDR->data_length < 13)
        return false;

    params->width = 0;
    params->height = 0;

    params->width += (unsigned __int32)(IHDR->data[0])<<24;
    params->width += (unsigned __int32)(IHDR->data[1])<<16;
    params->width += (unsigned __int32)(IHDR->data[2])<<8;
    params->width += (unsigned __int32)(IHDR->data[3]);

    params->height += (unsigned __int32)(IHDR->data[4]) << 24;
    params->height += (unsigned __int32)(IHDR->data[5]) << 16;
    params->height += (unsigned __int32)(IHDR->data[6]) << 8;
    params->height += (unsigned __int32)(IHDR->data[7]);
    
    params->bit_depth = IHDR->data[8];
    params->color_type = IHDR->data[9];
    params->compress_type = IHDR->data[10];
    params->filter_type = IHDR->data[11];
    params->Interlance = IHDR->data[12];

    return true;
}



/// <summary>
/// This function can be called from the any programm for load a png file.
/// </summary>
/// <param name="path">Path to png file</param>
/// <returns>If successful load of png return array of pixels else return null.</returns>
unsigned char* LoadPNG(const char* path)
{

    HANDLE file = CreateFileA(
        path,
        GENERIC_READ,
        0,
        0,
        OPEN_EXISTING,
        0,
        0
    );

    if (file == INVALID_HANDLE_VALUE) {
        SetLastError(2);
        return nullptr;
    }

    
    DWORD size = GetFileSize(file, 0);

    unsigned char* file_text = new unsigned char[size];

    ReadFile(
        file,
        file_text,
        size,
        0, 0
    );
    CloseHandle(file);

     
    if (size < 8)
        return nullptr;
    
    if (!CheckSignature(file_text)) {
        return nullptr;
    }
    
    std::vector<PNG_CHUNK*> chunks;
    if (!ParseOnChunks(file_text, size, chunks)) {
        delete file_text;
        FreeChunks(chunks);
        return nullptr;
    }
    delete file_text;

    
    SortChunks(chunks);

    PNG_PARAMETRS* parametrs = new PNG_PARAMETRS;
    GetPNGParametrs(chunks, parametrs);

    FreeChunks(chunks);

    delete parametrs;

    return nullptr;
}
