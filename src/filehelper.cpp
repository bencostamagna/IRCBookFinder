/*
 * filehelper.cpp
 * Copyright (C) 2018 ben <ben@diziet>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "filehelper.h"

#include <iostream>
#include <zip.h>

#define BUFSIZE 1024

std::map<uint, FILE*> FileHelper::m_files;

bool FileHelper::init_write(uint dccid, const char* filename)
{
    if (m_files.find(dccid) != m_files.end())
	return false;
    FILE* file = fopen (filename, "w");
    
    if (!file)
	return false;

    m_files[dccid] = file;
    return true;
}

bool FileHelper::end_write(uint dccid)
{
    if (m_files.find(dccid) == m_files.end())
	return false;
    fclose(m_files[dccid]);
    m_files.erase(dccid);
    return true;
}

bool FileHelper::write_buffer(uint dccid, const char* data, int length)
{
    if (!m_files[dccid])
	return false;
    fwrite(data, sizeof(char), length, m_files[dccid]);
    return true;
}

bool FileHelper::extract_zip(QString filename, QString &content)
{
    int zerror = 0;
    zip_t* z = zip_open(filename.toUtf8(), ZIP_RDONLY, &zerror);
    if (z == NULL)
	return false;
    zip_int64_t nfiles = zip_get_num_entries(z, ZIP_FL_UNCHANGED);
   
    for (zip_int64_t i = 0; i < nfiles; ++i)
    {
	std::cout << zip_get_name(z, i, ZIP_FL_ENC_RAW) << std::endl;

	zip_file_t *f = zip_fopen_index(z, i, 0);
	char buf[BUFSIZE+1];
	
	int len = 0;
	while ((len = zip_fread(f, buf, BUFSIZE)) > 0)
	{
	    buf[len] = 0;
	    content.append(buf);
	}
	zip_fclose(f);
    }
    zip_close(z);
    std::remove(filename.toUtf8());
    return true;
}

