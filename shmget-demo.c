#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*Demo output
    Shared memory created with ID: 98317
    Shared memory attached at address: 0x7f5e89a01000
    Message written to shared memory: Hello from the parent process!
    Child process attached to shared memory at address: 0x7f5e89a00000
    Message read from shared memory: Hello from the parent process!
    Shared memory removed
*/

#define SHM_KEY 1234  // 共享内存键值
#define SHM_SIZE 1024 // 共享内存大小

int main()
{
    // 创建共享内存
    int shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id < 0)
    {
        perror("shmget failed");
        exit(1);
    }
    printf("Shared memory created with ID: %d\n", shm_id);

    // 将共享内存映射到父进程的地址空间
    char *shm_ptr = (char *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (char *)-1)
    {
        perror("shmat failed");
        exit(1);
    }
    printf("Shared memory attached at address: %p\n", shm_ptr);

    // 父进程写入数据
    const char *message = "Hello from the parent process!";
    snprintf(shm_ptr, SHM_SIZE, "%s", message);
    printf("Message written to shared memory: %s\n", message);

    // 创建子进程来读取共享内存
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0)
    { // 子进程
        // 等待父进程写入数据
        sleep(1);

        // 将共享内存映射到子进程的地址空间
        char *child_shm_ptr = (char *)shmat(shm_id, NULL, 0);
        if (child_shm_ptr == (char *)-1)
        {
            perror("shmat in child failed");
            exit(1);
        }
        printf("Child process attached to shared memory at address: %p\n", child_shm_ptr);

        // 子进程读取共享内存中的数据
        printf("Message read from shared memory: %s\n", child_shm_ptr);

        // 分离共享内存
        if (shmdt(child_shm_ptr) == -1)
        {
            perror("shmdt failed in child");
            exit(1);
        }
    }
    else
    { // 父进程
        // 父进程等待子进程完成
        wait(NULL);

        // 分离共享内存
        if (shmdt(shm_ptr) == -1)
        {
            perror("shmdt failed in parent");
            exit(1);
        }

        // 删除共享内存
        if (shmctl(shm_id, IPC_RMID, NULL) == -1)
        {
            perror("shmctl failed");
            exit(1);
        }
        printf("Shared memory removed\n");
    }

    return 0;
}
