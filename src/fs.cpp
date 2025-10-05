#include "fs.hpp"
#include "vga.hpp"
#include "string.hpp"
#include <cstring>
struct FSNode {
    char name[32];
    bool is_dir;
    char content[512];
    FSNode* parent;
    FSNode* children[16];
    int child_count;
};

static FSNode root;
static FSNode* cwd = &root;

void fs_init() {
    strcpy(root.name, "#");
    root.is_dir = true;
    root.parent = nullptr;
    root.child_count = 0;
}

static FSNode* fs_find(const char* name) {
    for (int i = 0; i < cwd->child_count; i++) {
        if (strcmp(cwd->children[i]->name, name) == 0) {
            return cwd->children[i];
        }
    }
    return nullptr;
}

int fs_mkdir(const char* name) {
    if (fs_find(name)) return -1;
    if (cwd->child_count >= 16) return -2;

    FSNode* node = new FSNode;
    strcpy(node->name, name);
    node->is_dir = true;
    node->child_count = 0;
    node->parent = cwd;

    cwd->children[cwd->child_count++] = node;
    return 0;
}

int fs_rmdir(const char* name) {
    FSNode* node = fs_find(name);
    if (!node || !node->is_dir) return -1;
    if (node->child_count > 0) return -2;

    for (int i = 0; i < cwd->child_count; i++) {
        if (cwd->children[i] == node) {
            for (int j = i; j < cwd->child_count - 1; j++) {
                cwd->children[j] = cwd->children[j + 1];
            }
            cwd->child_count--;
            break;
        }
    }
    delete node;
    return 0;
}

int fs_cd(const char* name) {
    if (strcmp(name, "..") == 0) {
        if (cwd->parent) cwd = cwd->parent;
        return 0;
    }
    FSNode* node = fs_find(name);
    if (!node || !node->is_dir) return -1;
    cwd = node;
    return 0;
}

void fs_ls() {
    for (int i = 0; i < cwd->child_count; i++) {
        FSNode* n = cwd->children[i];
        vga_print(n->name);
        if (n->is_dir)
            vga_println("#");
        else
            vga_println("");
    }
}

const char* fs_pwd() {
    static char path[256];
    FSNode* cur = cwd;
    path[0] = '\0';

    char temp[256] = "";
    while (cur) {
        char segment[64];
        strcpy(segment, "/");
        strcat(segment, cur->name);
        strcat(segment, temp);
        strcpy(temp, segment);
        cur = cur->parent;
    }

    strcpy(path, temp);
    if (strcmp(path, "//") == 0) strcpy(path, "/");
    return path;
}

int fs_touch(const char* name) {
    if (fs_find(name)) return -1;
    if (cwd->child_count >= 16) return -2;

    FSNode* node = new FSNode;
    strcpy(node->name, name);
    node->is_dir = false;
    node->content[0] = '\0';
    node->parent = cwd;

    cwd->children[cwd->child_count++] = node;
    return 0;
}

int fs_rm(const char* name) {
    FSNode* node = fs_find(name);
    if (!node || node->is_dir) return -1;

    for (int i = 0; i < cwd->child_count; i++) {
        if (cwd->children[i] == node) {
            for (int j = i; j < cwd->child_count - 1; j++) {
                cwd->children[j] = cwd->children[j + 1];
            }
            cwd->child_count--;
            break;
        }
    }
    delete node;
    return 0;
}

int fs_write(const char* name, const char* data) {
    FSNode* node = fs_find(name);
    if (!node || node->is_dir) return -1;

    strncpy(node->content, data, sizeof(node->content) - 1);
    node->content[sizeof(node->content) - 1] = '\0';
    return 0;
}

const char* fs_read(const char* name) {
    FSNode* node = fs_find(name);
    if (!node || node->is_dir) return nullptr;
    return node->content;
}
