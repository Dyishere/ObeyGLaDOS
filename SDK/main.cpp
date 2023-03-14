#include <iostream>
using namespace std;


FILE *fp;

bool readUntilOK() {
    char line[1024];
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }
        //do something
        // 将line保存到log.txt当中
        fputs(line, fp);

    }
    return false;
}

int main() {

    fp = fopen("log.txt", "w");

    readUntilOK();
    puts("OK");
    fflush(stdout);
    int frameID;
    while (scanf("%d", &frameID) != EOF) {
        readUntilOK();
        printf("%d\n", frameID);
        int lineSpeed = 3;
        double angleSpeed = 1.5;

        if(frameID == 1) {

        for(int robotId = 0; robotId < 4; robotId++){
            printf("forward %d %d\n", robotId, lineSpeed);
            printf("rotate %d %f\n", robotId, angleSpeed);
        }

        }
        printf("OK\n", frameID);
        fflush(stdout);
    }

    fclose(fp);
    
    return 0;
}
