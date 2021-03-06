/*
 * lwan - simple web server
 * Copyright (c) 2013 Leandro A. F. Pereira <leandro@hardinfo.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#pragma once

#define ONE_MINUTE 60
#define ONE_HOUR (ONE_MINUTE * 60)
#define ONE_DAY (ONE_HOUR * 24)
#define ONE_WEEK (ONE_DAY * 7)
#define ONE_MONTH (ONE_DAY * 31)
#define ONE_YEAR (ONE_MONTH * 12)

#include <stdio.h>
#include <stdbool.h>

#include "strbuf.h"

typedef struct config_t_ config_t;
typedef struct config_line_t_ config_line_t;

typedef enum {
    CONFIG_LINE_TYPE_LINE,
    CONFIG_LINE_TYPE_SECTION,
    CONFIG_LINE_TYPE_SECTION_END
} config_line_type_t;

struct config_t_ {
    FILE *file;
    char *path, *error_message;
    strbuf_t *strbuf;
    struct {
        long end;
    } isolated;
    int line;
};

struct config_line_t_ {
    union {
        struct {
            char *name, *param;
        } section;
        struct {
            char *key, *value;
        } line;
    };
    config_line_type_t type;
    char buffer[1024];
};

bool config_open(config_t *conf, const char *path);
void config_close(config_t *conf);
bool config_error(config_t *conf, const char *fmt, ...);
bool config_read_line(config_t *conf, config_line_t *l);

bool config_isolate_section(config_t *current_conf,
    config_line_t *current_line, config_t *isolated);
bool config_skip_section(config_t *conf, config_line_t *line);

bool parse_bool(const char *value, bool default_value);
long parse_long(const char *value, long default_value);
int parse_int(const char *value, int default_value);
unsigned int parse_time_period(const char *str, unsigned int default_value);
