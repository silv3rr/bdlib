/*
 * Copyright (c) 2006-2014, Bryan Drewery <bryan@shatow.net>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* AtomicFile.h
 */
#ifndef _BD_ATOMICFILE_H
#define _BD_ATOMICFILE_H 1

#include "bdlib.h"
#include "String.h"

BDLIB_NS_BEGIN

/**
 * @class AtomicFile
 * @brief Write out files atomically via tmpfile/rename, with a backup.
 */
class AtomicFile {
  private:
    String _fname;
    String _tmpname;
    int _fd;
  public:
    AtomicFile(const String& fname) : _fname(), _tmpname(), _fd(-1) {
      this->open(fname);
    };
    AtomicFile(AtomicFile& f) : _fname(f._fname), _tmpname(), _fd(f._fd) {};
    virtual ~AtomicFile();

    /**
     * @brief Open a file for writing.
     */
    void open(const String& fname);

    /**
     * @brief Check if a file was successfully opened.
     */
    bool is_open() const {
      return _fd != -1;
    }

    /**
     * @brief Return an fd for the open file.
     * @return Returns -1 if not open.
     */
    int fd() {
      return (this->is_open() ? _fd : -1);
    }

    /**
     * @brief Write out the file and close the descriptor.
     */
    bool close();
};
BDLIB_NS_END
#endif /* _BD_ATOMICFILE_H */ 
/* vim: set sts=2 sw=2 ts=8 et: */
