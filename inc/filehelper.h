/*
 * filehelper.h
 * Copyright (C) 2018 ben <ben@diziet>
 *
 * Distributed under terms of the GPL3 license.
 */

#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <fstream>
#include <map>

#include <QString>

class FileInfo
{
    public:
	FileInfo() {}
	FileInfo(QString f, unsigned long s);

	QString filename;
	unsigned long size;
	unsigned long received;
};


class FileHelper
{
    public:
	static bool init_write(uint dccid, const char* filename);
	static bool write_buffer(uint dccid, const char* data, int length);
	static bool end_write(uint dccid);

	static bool extract_zip(QString filename, QString &content);
    private:
	static std::map<uint, FILE*> m_files;

};
#endif /* !FILEHELPER_H */
