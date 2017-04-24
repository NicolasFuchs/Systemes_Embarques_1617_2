#pragma once
#indef SD_H
#define SD_H

#include "file_ops.h"

struct sd_ops {
	struct file_ops fops;
	struct dir_ops dops;
};


struct sd_ops* sd_init();

//struct file_ops* sd_init();

#endif