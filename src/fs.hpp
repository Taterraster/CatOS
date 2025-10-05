#pragma once

void fs_init();

int fs_mkdir(const char* name);
int fs_rmdir(const char* name);
int fs_cd(const char* name);
void fs_ls();
const char* fs_pwd();

int fs_touch(const char* name);
int fs_rm(const char* name);
int fs_write(const char* name, const char* data);
const char* fs_read(const char* name);
