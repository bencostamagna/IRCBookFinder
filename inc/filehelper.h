/*
 * filehelper.h
 * Copyright (C) 2018 ben <ben@diziet>
 *
 * Distributed under terms of the GPL3 license.
 */

#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <fstream>

class FileHelper
{
    public:
	static bool init_write(const char* filename);
	static bool write_buffer(const char* data, int length);
	static bool end_write();

    private:
	static FILE *m_file;

};
#endif /* !FILEHELPER_H */
