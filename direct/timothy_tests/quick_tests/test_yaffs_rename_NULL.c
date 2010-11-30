/*
 * YAFFS: Yet another FFS. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2010 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Timothy Manning <timothy@yaffs.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "test_yaffs_rename_NULL.h"


int test_yaffs_rename_NULL(void)
{
	int output=0;
	int error_code =0;

	if (0 !=  yaffs_access(DIR_PATH,0)) {
		output = yaffs_mkdir(DIR_PATH,S_IWRITE | S_IREAD);
		if (output < 0) {
			print_message("failed to create directory\n",2);
			return -1;
		}
	}
	output = yaffs_rename(NULL , RENAME_PATH);
	if (output<0){ 
		error_code=yaffs_get_error();
		if (abs(error_code)==EFAULT){
			return 1;
		} else {
			print_message("returned error does not match the the expected error\n",2);
			return -1;
		}
	} else{
		print_message("renamed a null path (which is a bad thing)\n",2);
		return -1;
	}	
}


int test_yaffs_rename_NULL_clean(void)
{
	return 1;
}

