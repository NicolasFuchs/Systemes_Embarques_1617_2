// ...

static struct dnode* get_node 

struct dnode {
    char name[MAX_FILENAME];

    struct dir_ops* dops;
    struct file_ops* fops;

    struct dnode* b_node;
    struct dnode* c_node;
};

static struct dnode root = {
    .name = "/",
    .fops = 0,
    .dops = 0,
}

int vfs_init()
{
    return 0;
}


int vfs_mount(const char* path, struct dir_ops* dops, struct file_ops* fops)
{
    root.dops = dops;
    root.fops = fops;
    return 0;
}

int vfs_mkdir(const char* name)
{
    return -1;
}

int vfs_rmdir(const char* name)
{
    return -1;
}

//...