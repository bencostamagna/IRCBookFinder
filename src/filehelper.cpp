/*
 * filehelper.cpp
 * Copyright (C) 2018 ben <ben@diziet>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "filehelper.h"

FILE* FileHelper::m_file = NULL;

bool FileHelper::init_write(const char* filename)
{
    m_file = fopen (filename, "w");

    if (!m_file)
	return false;
    return true;
}

bool FileHelper::end_write()
{
    fclose(m_file);
    m_file=NULL;
    return true;
}

bool FileHelper::write_buffer(const char* data, int length)
{
    if (!m_file)
	return false;
    fwrite(data, sizeof(char), length, m_file);
    return true;
}
