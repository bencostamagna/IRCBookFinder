/*
 * filehelper.cpp
 * Copyright (C) 2018 ben <ben@diziet>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "filehelper.h"

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
