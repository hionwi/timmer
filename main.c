#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

/**
 * 绘制终端进度条
 * @param current 当前秒数
 * @param total 总秒数
 */
void print_progress_bar(int current, int total)
{
    int width = 40; // 进度条宽度
    float percentage = (float)current / total;
    int pos = (int)(width * percentage);

    printf("\r[");
    for (int i = 0; i < width; ++i)
    {
        if (i < pos)
            printf("=");
        else if (i == pos)
            printf(">");
        else
            printf(" ");
    }

    int remaining = total - current;
    printf("] %d%% | 剩余时间: %02d:%02d ",
           (int)(percentage * 100),
           remaining / 60,
           remaining % 60);

    // 强制刷新输出缓冲区
    fflush(stdout);
}

void play_music(const char *file_path)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // 子进程：调用 VLC
        // --video-on-top: 窗口置顶
        // --fullscreen: 全屏播放
        // --play-and-exit: 播放完后退出进程
        execlp("vlc", "vlc", "--video-on-top", "--fullscreen", "--play-and-exit", file_path, (char *)NULL);
        exit(0);
    }
}

int main()
{
    const char *music_file = "./bc.mp3";
    const int total_seconds = 40 * 60; // 40 分钟

    printf(">>> 定时播放任务已启动: %s <<<\n", music_file);

    while (1)
    {
        // 1. 立即播放一次
        play_music(music_file);
        printf("\n播放指令已发送。开始倒计时...\n");

        // 2. 40 分钟倒计时循环
        for (int i = 0; i <= total_seconds; i++)
        {
            print_progress_bar(i, total_seconds);
            sleep(1); // 每秒更新一次
        }
        printf("\n时间到！准备下一次播放。\n");
    }

    return 0;
}
