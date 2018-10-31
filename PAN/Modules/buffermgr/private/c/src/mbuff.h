
#define IOCTL_MBUFF_INFO 0
#define IOCTL_MBUFF_ALLOCATE 1
#define IOCTL_MBUFF_DEALLOCATE 2
#define IOCTL_MBUFF_SELECT 3
#define IOCTL_MBUFF_LAST IOCTL_MBUFF_SELECT


#define MBUFF_DEV_NAME "/dev/mbuff"
#define MBUFF_NAME_LEN 32
#define MBUFF_MAX_MMAPS 16



struct mbuff_request_struct{
        unsigned int flags;

        char name[MBUFF_NAME_LEN+1];

        size_t size;

        unsigned int reserved[4];
};


