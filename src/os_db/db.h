/*
 * Wazuh SQLite integration
 * Copyright (C) 2016 Wazuh Inc.
 * June 06, 2016.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#ifndef DB_H
#define DB_H

#include "sqlite3.h"
#include "analysisd/decoders/decoder.h"

/* Global SQLite database */
extern sqlite3 *db;

/* Open global database */
void db_open();

/* Find file: returns ID, or 0 if it doesn't exists, or -1 on error. */
int db_find_file(const char *path);

/* Insert file, Returns -1 on error. */
int db_insert_file(const char *path);

/* Insert FIM entry. Returns -1 on error. */
int db_insert_fim(int id_agent, const char *f_name, const char *event, const SyscheckSum *sum, long int time);

#endif